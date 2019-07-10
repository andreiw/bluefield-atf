/*
 * Copyright (c) 2017, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of Mellanox nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <console.h>
#include <debug.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "bluefield_ddr.h"
#include "bluefield_ddr_bist.h"
#include "bluefield_private.h"
#include "delay_timer.h"
#include "emc.h"
#include "emc_def.h"


#define ONE_GIGABIT				(1 << 30)
#define PRINT_CHARS_PER_LINE			18
/* Maximum value for the MSS mask (exclusive). */
#define MAX_MC_MSK				(1 << MAX_MEM_CTRL)
/* Maximum value for the physical rank mask (exclusive). */
#define MAX_PMSK				(1 << MAX_RANKS_PER_MEM_CTRL)

#define DEBUG_LEVEL_MAX		11

#ifdef ATF_CONSOLE
  #define ADD_GLOBAL_VAR(name, default_val) int name = default_val;
#else
  #define ADD_GLOBAL_VAR(name, val) static const int name = val;
#endif
/* Setting variables that may be changed from the console. */
ADD_GLOBAL_VAR(g_debug_level, 1)
ADD_GLOBAL_VAR(g_scan_mode, BIST_SCAN_MODE__LOGICAL_RANK)
ADD_GLOBAL_VAR(g_bist_rand_scan_seed, EMC_IFC_BIST_ADDR_LFSR__SEED_RESET_VAL)
ADD_GLOBAL_VAR(g_bist_high_addr, 0)
ADD_GLOBAL_VAR(g_bist_low_addr, 0)

static uint8_t bist_current_patt;

/* The per MSS per BIST pattern dqmasks. */
static uint8_t ifc_bist_mask[MAX_MEM_CTRL][BIST_PATTERN_NUM][DQ_MASK_SIZE];

/* Used in counting how many BIST failed when doing the loop command. */
static uint loop_results[BIST_PATTERN_NUM][MAX_MEM_CTRL];

/*
 * The ifc_bist_mask_state tracks the state of the DQMASKs. The default value
 * of 0 is uninitialized.
 */
static uint8_t ifc_bist_mask_state[MAX_MEM_CTRL][BIST_PATTERN_NUM];

static int reset_dqmask(uint32_t mc_mask, uint32_t pattern);
/****************************************************************************/
/* Dictionaries used to get the string representation of the enum used.     */
/****************************************************************************/
#ifdef ATF_CONSOLE
const char pattern_2_str[BIST_PATTERN_NUM][12] = {
	[KILLER_PATTERN] = "killer",
	[WALKING_1_PATTERN] = "walking 1",
	[WALKING_0_PATTERN] = "walking 0",
	[SSO_PATTERN] = "sso",
	[AA_55_PATTERN] = "aa 55",
	[ADDRESS_PATTERN] = "address",
	[INIT_00_PATTERN] = "init 00",
	[INIT_FF_PATTERN] = "init FF",
	[RANDOM_PATTERN] = "random",
	[DQ0_1_010] = "DQ0 1e 010",
	[DQ0_8_010] = "DQ0 8e 010",
	[DQ0_1_101] = "DQ0 1e 101",
	[DQ0_8_101] = "DQ0 8e 101",
	[DQ28_1_010] = "DQ28 1e 010",
	[DQ28_8_010] = "DQ28 8e 010",
	[DQ28_1_101] = "DQ28 1e 101",
	[DQ28_8_101] = "DQ28 8e 101",
	[DQ53_1_010] = "DQ53 1e 010",
	[DQ53_8_010] = "DQ53 8e 010",
	[DQ53_1_101] = "DQ53 1e 101",
	[DQ53_8_101] = "DQ53 8e 101",
};

const char op_mode_2_str[OP_MODE_NUM][14] = {
	[COMPARE_ONLY] = "COMPARE_ONLY",
	[WRITE_ONLY] = "WRITE_ONLY",
	[WRITE_COMPARE] = "WRITE_COMPARE",
};

const char rep_mode_2_str[REP_MODE_NUM][12] = {
	[ENDLESS] = "ENDLESS",
	[SINGLE] = "SINGLE",
	[UNTIL_ERROR] = "UNTIL_ERROR",
};
#endif

/****************************************************************************/
/* Simple helper functions here.                                            */
/****************************************************************************/
#ifdef ATF_CONSOLE
static uint64_t randseed = 1;
/*
 * Isolate the seed so other functions which call random won't mess up this
 * seed.
 */
static void bist_srandom(uint64_t seed)
{
	dbg_printf(3, "seed changed from %lld to %lld", randseed, seed);
	randseed = seed;
}

static uint64_t bist_random(void)
{
	return __random(&randseed);
}
#endif

/*
 * Get the high DIMM address based on the known data.
 * high address = ((device_density / (num_of_banks *data_width)) - 1)
 */
static uint32_t get_high_addr(void)
{
	uint32_t density_dev, data_width, num_banks;
	uint32_t high_address;

	CASSERT(DENSITY_2Gbit == 0, density_enum_match_2Gb);
	CASSERT(DENSITY_4Gbit == 1, density_enum_match_4Gb);
	CASSERT(DENSITY_8Gbit == 2, density_enum_match_8Gb);
	CASSERT(DENSITY_16Gbit == 3, density_enum_match_16Gb);

	/* We are using byte resolution here so right shift by 3. */
	density_dev = (ONE_GIGABIT >> 3) << (1 + MAX(dp->dimm[0].density,
						     dp->dimm[1].density));

	CASSERT(PACKAGE_x4 == 0, package_enum_match_x4);
	CASSERT(PACKAGE_x8 == 1, package_enum_match_x8);
	CASSERT(PACKAGE_x16 == 2, package_enum_match_x16);

	data_width = 1 << (2 + dp->package);

	num_banks = dp->ddr_max_bank;

	high_address = ((density_dev / (num_banks * data_width)) - 1);

	dbg_printf(3, "num_banks=%d data_width=%d density_dev=%d"
		   " get_high_addr: 0x%x\n",
		   num_banks, data_width, density_dev, high_address);

	return high_address;
}

/*
 * Check if the current DIMM configuration supports doing the BIST machine
 * random scan mode.
 * Returns 1 if the configuration is supported, 0 if it isn't.
 */
static int check_rnd_scan_support(void)
{
	/*
	 * Due to a bug, random mode is not supported for x16/x8 DIMMs with 2Gb
	 * components (14 bits of row address).
	 */
	if (dp->dimm[0].density == DENSITY_2Gbit &&
	    (dp->package == PACKAGE_x8 || dp->package == PACKAGE_x16))
		return 0;

	/*
	 * Don't support the case the system is occupied with non-equal DIMMs
	 * per side.
	 */
	if (dp->dimm_num > 1) {
		if (dp->dimm[0].ranks != dp->dimm[1].ranks)
			return 0;
		if (dp->dimm[0].density != dp->dimm[1].density)
			return 0;
	}

	return 1;
}

/*
 * Calculates the value of the EMC_IFC_BIST_RND_BITS register based on
 * the current setup. If cnt_offset_addr isn't NULL, also writes to it
 * the cnt_offset value at the end.
 */
static uint32_t get_rnd_bits(int *cnt_offset_addr)
{
	const int package_x4_rowbits_lut[NUM_DENSITY] = {
		[DENSITY_2Gbit] = 15,	[DENSITY_4Gbit] = 16,
		[DENSITY_8Gbit] = 17,	[DENSITY_16Gbit] = 18,
	};
	const int package_x8x16_rowbits_lut[NUM_DENSITY] = {
		[DENSITY_2Gbit] = 14,	[DENSITY_4Gbit] = 15,
		[DENSITY_8Gbit] = 16,	[DENSITY_16Gbit] = 17,
	};

	int row_bits;
	int density;
	int cnt_offset = 0;
	EMC_IFC_BIST_RND_BITS_t  rnd_bits = { .word = 0 };

	density = dp->dimm_num == 1 ?
				MAX(dp->dimm[0].density, dp->dimm[1].density) :
				MIN(dp->dimm[0].density, dp->dimm[1].density);

	if (dp->package ==PACKAGE_x4)
		row_bits = package_x4_rowbits_lut[density];
	else
		row_bits = package_x8x16_rowbits_lut[density];

#define ADD_RBIT(cond, field)		do {		\
	if (cond) {					\
		rnd_bits.field##_en = 1;		\
		rnd_bits.field##_offs = cnt_offset;	\
		cnt_offset++;				\
	}						\
} while (0)

	ADD_RBIT(row_bits >= 18, addr_24);
	ADD_RBIT(row_bits >= 17, addr_23);
	ADD_RBIT(row_bits >= 16, addr_22);
	ADD_RBIT(dp->package == PACKAGE_x4 || dp->package == PACKAGE_x8, bank);
	ADD_RBIT(dp->ddr_3ds >= 2, lr_0);
	ADD_RBIT(dp->ddr_3ds >= 4, lr_1);
	ADD_RBIT(dp->dimm[0].ranks + dp->dimm[1].ranks >= 2, pr_0);
	ADD_RBIT(dp->dimm[0].ranks + dp->dimm[1].ranks >= 4, pr_1);

	if (cnt_offset_addr != NULL)
		*cnt_offset_addr = cnt_offset;

	return rnd_bits.word;
}
/****************************************************************************/
/* Helper functions for hardware access here.                               */
/****************************************************************************/
/*
 * Write one SRAM line to the memory controller.
 * Returns 1 if successful or 0 if the read failed.
 */
static int write_emem_mc_sram_line(uint32_t *data, uint32_t address)
{
	EMC_IND_CMD_t ind_cmd = {
		.op = EMC_IND_CMD__OP_VAL_WRITE,
		.mem_id = EMC_IND_CMD__MEM_ID_VAL_IFC,
	};
	EMC_IND_ADDR_t ind_addr = {
		.val = address,
	};

	for (int i = 0; i < EMC_IND_DATA__LENGTH; i++) {
		emc_write(EMC_IND_DATA__FIRST_WORD + i, data[i]);
	}

	emc_write(EMC_IND_ADDR, ind_addr.word);
	emc_write(EMC_IND_CMD, ind_cmd.word);

	return 1;
}

/*
 * Read one SRAM line from the memory controller.
 * Returns 1 if successful or 0 if the read failed.
 */
static int read_emem_mc_sram_line(uint32_t *line, uint32_t address)
{
	EMC_IND_CMD_t ind_cmd = {
		.op = EMC_IND_CMD__OP_VAL_READ,
		.mem_id = EMC_IND_CMD__MEM_ID_VAL_IFC,
	};

	EMC_IND_ADDR_t ind_addr = {
		.val = address,
	};

	emc_write(EMC_IND_ADDR, ind_addr.word);
	emc_write(EMC_IND_CMD, ind_cmd.word);

	/*
	 * The ability to read from the SRAM was only added in BlueField, and
	 * the hardware team decided not to add a status indicator. Thus this
	 * code is only safe to run when BIST is not running, when no other
	 * traffic is running through the EMEM_MC. This read from SRAM takes one
	 * memory controller cycle to finish, so we wait 10ns to ensure it has
	 * passed one cycle in the memory controller.
	 */
	ndelay(10);

	for (int i = 0; i < EMC_IND_DATA__LENGTH; i++)
		line[i] = emc_read(EMC_IND_DATA__FIRST_WORD + i);

	return 1;
}

/*
 * Changes the arbiter algorithm mode, 1 for functional operations and
 * 0 for BIST operations.
 */
static void set_arb_algorithm(int bist_off)
{
	EMC_ARB_ALGORITHM_t emc_arb_algorithm;

	emc_arb_algorithm.word = emc_read(EMC_ARB_ALGORITHM);
	emc_arb_algorithm.mode = bist_off;
	emc_write(EMC_ARB_ALGORITHM, emc_arb_algorithm.word);
}

/* Reenable the memory controller to do normal operation after the BIST. */
static void enable_emem_mc_hw(uint32_t mc_mask)
{
	for (int abs_mc = 0; abs_mc < MAX_MEM_CTRL; abs_mc++) {
		if (!(mc_mask & (1 << abs_mc)))
			continue;
		if (!ddr_switch_current_mss(abs_mc))
			continue;
		emc_write(EMC_IFC_BIST_EN, 0);
		/* Re-enable ARB_ALG */
		set_arb_algorithm(1);
	}
}
/****************************************************************************/
/* Function mainly used to print out stuff to console here.                 */
/****************************************************************************/
/*
 * Print len number of bytes as pointed by *bytes to the console. If brac_cnt
 * is bigger than 0, also for each index specified in the brac_idx array print
 * the bytes with [] surrounded.
 * bytes:     Array of bytes to print.
 * len:       The length of the bytes to print.
 * brac_cnt:  The number of bytes to be printed square bracketed.
 * brac_idx:  The index to the bytes to be square bracketed.
 */
static void print_bytes(uint8_t *bytes, int len, int brac_cnt, int *brac_idx)
{
	int next_brac;
	int bracketed = 0;

	if (brac_cnt > 0)
		next_brac = brac_idx[bracketed];
	else
		next_brac = -1;

	for (int i = 0; i < len; i++) {
		if ((i % PRINT_CHARS_PER_LINE) == 0)
			tf_printf("\n ");

		tf_printf(" %s0x%s%x%s",
			 (i == next_brac) ? "[" : " ",
			 bytes[i] < 0x10 ? "0" : "",
			 bytes[i],
			 (i == next_brac) ? "]" : " ");

		if (i == next_brac) {
			bracketed++;
			if (bracketed < brac_cnt)
				next_brac = brac_idx[bracketed];
		}
	}
	tf_printf("\n");
}

static void print_bist_parameters(const struct bist_parameters *bist_params,
				  int show_run_specific_params, int pattern)
{
#ifdef ATF_CONSOLE
	tf_printf("\n-------------------------------------------\n");
	tf_printf(" BIST Parameters:\n");
	if (show_run_specific_params) {
		tf_printf("     mc_mask        = 0x%x\n", bist_params->mc_mask);
		tf_printf("     mc0_pr_mask    = 0x%x\n",
			bist_params->mc_pr_mask[0]);
		tf_printf("     mc1_pr_mask    = 0x%x\n",
			bist_params->mc_pr_mask[1]);
	}
	tf_printf("     pattern_length = 0x%x(%d)\n",
		bist_params->pattern_length, bist_params->pattern_length);
	tf_printf("     rep_mode       = %s\n",
		rep_mode_2_str[bist_params->rep_mode]);
	tf_printf("     op_mode        = %s\n",
		  op_mode_2_str[bist_params->op_mode]);
	tf_printf("     pattern =\n");
	print_bytes(bist_params->pattern, bist_params->pattern_length, 0, NULL);
	tf_printf("     dq mask =\n");
	for (int abs_mc = 0; abs_mc < MAX_MEM_CTRL; abs_mc++) {
		if (!(bist_params->mc_mask & (1 << abs_mc)))
			continue;
		if (ifc_bist_mask_state[abs_mc][pattern] ==
						BIST_MASK_UNINITIALIZED)
			reset_dqmask(bist_params->mc_mask, pattern);
		if (bist_params->mc_mask & ~(1 << abs_mc))
			tf_printf("  [MSS %d]\n", abs_mc);
		print_bytes(ifc_bist_mask[abs_mc][pattern], DQ_MASK_SIZE,
			    0, NULL);

	}
	tf_printf("\n-------------------------------------------\n");
#endif /* ATF_CONSOLE */
}

/*
 * Print out the exact error on the error bit.
 * For sending a 72 byte (x8 = 576 bits) line data, this is how we are
 * sending it:
 *      ^     |     ^     |     ^     |     ^     |
 *      |     v     |     v     |     v     |     v
 * DQ0  : 0     8   :16    24   :288   296  :304   312
 *      :           :           :           :
 * DQ1  : 1     9   :17    25   :289   297  :305   313
 *      :           :           :           :
 * DQ2  : 2    10   :18    26   :290   298  :306   314
 *  .   :           :           :           :
 *  .   :           :           :           :
 *  .   :           :           :           :
 * DQ7  : 7    15   :23    31   :295   303  :313   319
 *      :           :           :           :
 * DQ8  :32    40   :48    56   :320   328  :336   344
 *  .   :           :           :           :
 *  .   :           :           :           :
 *  .   :           :           :           :
 * DQ62 :230   238  :246   254  :518   526  :534   542
 *      :           :           :           :
 * DQ63 :231   239  :247   255  :519   527  :535   543
 *      :           :           :           :
 * CB0  :256   264  :272   280  :544   552  :560   568
 *      :           :           :           :
 * CB1  :257   265  :273   281  :545   553  :561   569
 *  .   :           :           :           :
 *  .   :           :           :           :
 *  .   :           :           :           :
 * CB7  :263   271  :279   287  :551   559  :567   575
 */
static void print_dq_state_string(uint32_t bit_place, uint32_t expected_bit)
{
	uint32_t normalize;
	uint32_t adjust_edge = 0;
	uint32_t bit;

	if (bit_place >= 288) {
		adjust_edge = 1;
		bit = bit_place - 288;
	} else
		bit = bit_place;

	if (BF_BIST_IS_CB(bit) == 0)
		tf_printf("  DQ %d (bit %d) ", BF_BIST_GET_DQ(bit), bit_place);
	else
		tf_printf("  CB %d (bit %d) ", BF_BIST_GET_CB(bit), bit_place);

	normalize = BF_BIST_NORM_IND(bit);

	if (adjust_edge == 0) {
		switch (normalize / 8) {
		case 0:
			tf_printf(" First Rising ");
			break;
		case 1:
			tf_printf(" First Falling ");
			break;
		case 2:
			tf_printf(" Second Rising ");
			break;
		case 3:
			tf_printf(" Second Falling ");
			break;
		}
	} else {
		switch (normalize / 8) {
		case 0:
			tf_printf(" Third Rising ");
			break;
		case 1:
			tf_printf(" Third Falling ");
			break;
		case 2:
			tf_printf(" Fourth Rising ");
			break;
		case 3:
			tf_printf(" Fourth Falling ");
			break;
		}
	}

	if (expected_bit == 0)
		tf_printf(" (0 --> 1)\n");
	else
		tf_printf(" (1 --> 0)\n");

}

static void print_error_info(uint32_t abs_mc)
{
	uint32_t byte_start;
	uint32_t pat_length;
	int error_idx[PATTERN_LINE_LENGTH];
	int error_cnt = 0;
	uint32_t *data;

	uint8_t error_data[MAX_BIST_BLOCK_SIZE] = {0};
	uint8_t expected_data[MAX_BIST_BLOCK_SIZE] = {0};

	BF_BIST_ERR_ROW_COL_t bist_err_addr;
	EMC_IFC_BIST_STATUS_t ifc_bist_status;
	EMC_IFC_BIST_CFG_t ifc_bist_cfg;

	/* First print out the general data about the error. */
	ifc_bist_status.word = emc_read(EMC_IFC_BIST_STATUS);
	ifc_bist_cfg.word = emc_read(EMC_IFC_BIST_CFG);

	bist_err_addr.word = emc_read(EMC_IFC_BIST_ERR_ADDR);

	tf_printf(" -----------------------------\n");
	tf_printf(" Error Address = 0x%x\n", emc_read(EMC_IFC_BIST_ERR_ADDR));
	tf_printf(" Error Row = 0x%x\n", bist_err_addr.err_row);
	tf_printf(" Error Column = 0x%x\n", bist_err_addr.err_col);
	tf_printf(" Error Bank = 0x%x :\n", ifc_bist_status.err_bank);
	tf_printf(" Error Physical Rank = 0x%x :\n", ifc_bist_status.err_pr);
	tf_printf(" Error Logical Rank = 0x%x :\n", ifc_bist_status.err_lr);
	tf_printf(" Error Chunk = 0x%x :\n", ifc_bist_status.err_chunk);
	tf_printf(" Number of errors = 0x%x :\n",
					emc_read(EMC_IFC_BIST_ERR_CNTR));

	if (ifc_bist_cfg.test_mode == 0) {
		if (ifc_bist_status.err_chunk == 0)
			tf_printf(" SRAM entry 0 = 0x%x :\n",
					ifc_bist_status.err_sram_entry_0);
		else
			tf_printf(" SRAM entry 1 = 0x%x :\n",
					ifc_bist_status.err_sram_entry_1);
	}

	/*
	 * Figure out where the error occurred and read the expected and error
	 * data there.
	 */
	data = (uint32_t *)error_data;
	for (int i = 0; i < EMC_IFC_BIST_ERR_DATA__LENGTH; i++)
		data[i] = emc_read(EMC_IFC_BIST_ERR_DATA__FIRST_WORD + i);

	read_emem_mc_sram_line((uint32_t *)&expected_data[0],
				ifc_bist_status.err_sram_entry_0);
	read_emem_mc_sram_line((uint32_t *)&expected_data[36],
				ifc_bist_status.err_sram_entry_1);

	byte_start = ifc_bist_status.err_chunk * PATTERN_LINE_LENGTH;

	pat_length = MIN((unsigned int)PATTERN_LINE_LENGTH,
			 bist_configs[bist_current_patt].pattern_length);

	/* Find the index of the data which are different. */
	for (int i = 0; i < pat_length; i++) {
		uint8_t dq_mask = ifc_bist_mask[abs_mc][bist_current_patt][i];

		if ((expected_data[byte_start + i] & dq_mask) !=
		    (error_data[byte_start + i] & dq_mask))
			error_idx[error_cnt++] = i;
	}

	/*
	 * Print out the expected data and the error data bytes, highlighting
	 * the different bytes with square brackets.
	 */
	tf_printf("\n     Expected Data:");
	print_bytes(&expected_data[byte_start], pat_length,
		    error_cnt, error_idx);
	tf_printf("\n     Error Data:");
	print_bytes(&error_data[byte_start], pat_length,
		    error_cnt, error_idx);

	/* Now compare the bits and figure out which DQ pin causes the error. */
	for (int i = 0; i < error_cnt; i++) {
		uint8_t err_ch, exp_ch;
		int byte_idx = error_idx[i] + byte_start;

		uint8_t dq_mask =
			 ifc_bist_mask[abs_mc][bist_current_patt][error_idx[i]];

		exp_ch = expected_data[byte_idx] & dq_mask;
		err_ch = error_data[byte_idx] & dq_mask;

		tf_printf("Byte %d: Expected 0x%x(%d) Received 0x%x(%d)\n",
			  byte_idx, exp_ch, exp_ch, err_ch, err_ch);

		for (int bit_idx = 0; bit_idx < 8; bit_idx++) {
			uint8_t exp_bit, err_bit;

			if (!(dq_mask & (1 << bit_idx)))
				continue;

			exp_bit = GET_BIT(exp_ch, bit_idx);
			err_bit = GET_BIT(err_ch, bit_idx);

			if (exp_bit != err_bit)
				print_dq_state_string(8 * byte_idx + bit_idx,
						      exp_bit);
		}
	}
}
/****************************************************************************/
/* Dqmask management related functions here.                                */
/****************************************************************************/
/*
 * Reset the DQMASK array for the given MSS and pattern to the default value of
 * all ones. This should be called before the first time running BIST on any
 * MSS/pattern as the uninitialized DQMASK array has value of all zeros.
 * Return 0 if successful.
 */
static int reset_dqmask(uint32_t mc_mask, uint32_t pattern)
{
	dbg_printf(3, "reset_dqmask mc_mask=%d pattern=%d\n", mc_mask, pattern);

	for (int abs_mc = 0; abs_mc < MAX_MEM_CTRL; abs_mc++) {
		if (!(mc_mask & (1 << abs_mc)))
			continue;
		if (!ddr_switch_current_mss(abs_mc))
			continue;

		memset((ifc_bist_mask[abs_mc][pattern]), 0xff, DQ_MASK_SIZE);

		ifc_bist_mask_state[abs_mc][pattern] = BIST_MASK_DEFAULT;
	}
	return 0;
}

#ifdef ATF_CONSOLE
/* Modifies one byte of the dqmask for the given MSS and pattern. */
static int set_dqmask(uint32_t mc_mask, uint32_t pattern, uint32_t byte_num,
		      uint32_t byte_mask)
{
	dbg_printf(5, "mc_mask = 0x%x\n", mc_mask);
	dbg_printf(5, "pattern = %s pattern\n", pattern_2_str[pattern]);
	dbg_printf(5, "byte_num = 0x%x\n", byte_num);
	dbg_printf(5, "byte_mask = 0x%x\n", byte_mask);

	if (pattern >= BIST_PATTERN_NUM) {
		ERROR("Invalid pattern %d\n", pattern);
		return -1;
	}

	for (int abs_mc = 0; abs_mc < MAX_MEM_CTRL; abs_mc++) {
		if (!(mc_mask & (1 << abs_mc)))
			continue;
		if (!ddr_switch_current_mss(abs_mc))
			continue;

		if (ifc_bist_mask_state[abs_mc][pattern] ==
						BIST_MASK_UNINITIALIZED)
			reset_dqmask(mc_mask, pattern);

		ifc_bist_mask[abs_mc][pattern][byte_num] = byte_mask;
		ifc_bist_mask_state[abs_mc][pattern] = BIST_MASK_MODIFIED;

		tf_printf("Modified DQ Mask for %s pattern:",
				pattern_2_str[pattern]);

		print_bytes(ifc_bist_mask[abs_mc][pattern], DQ_MASK_SIZE,
			    0, NULL);
	}
	return 0;
}

/* Print out the dqmask pattern. */
static int show_dqmask(uint32_t mc_mask, uint32_t pattern)
{
	dbg_printf(3, "mc_mask = 0x%x\n", mc_mask);
	dbg_printf(5, "pattern = %s pattern\n", pattern_2_str[pattern]);

	if (pattern >= BIST_PATTERN_NUM) {
		ERROR("Invalid pattern %d\n", pattern);
		return -1;
	}

	for (int abs_mc = 0; abs_mc < MAX_MEM_CTRL; abs_mc++) {
		if (!(mc_mask & (1 << abs_mc)))
			continue;
		if (!ddr_switch_current_mss(abs_mc))
			continue;

		if (ifc_bist_mask_state[abs_mc][pattern] ==
						BIST_MASK_UNINITIALIZED)
			reset_dqmask(mc_mask, pattern);

		tf_printf("%s DQ Mask for %s pattern::",
		  ifc_bist_mask_state[abs_mc][pattern] == BIST_MASK_MODIFIED ?
		  "Modified" : "Default", pattern_2_str[pattern]);

		print_bytes(ifc_bist_mask[abs_mc][pattern], DQ_MASK_SIZE,
			    0, NULL);
	}
	return 0;
}
#endif /* ATF_CONSOLE */
/****************************************************************************/
/* Core BIST functions here.                                                */
/****************************************************************************/
/*
 * Write the BIST pattern as specified by parameters into the SRAM.
 * This operation is skipped if we are in COMPARE_ONLY mode.
 * Return 0 if failed or 1 if successful.
 */
static int set_bist_pattern(struct bist_parameters *params)
{
	int lines;

	if (params->op_mode == COMPARE_ONLY)
		return 1;

	if (params->pattern_length == 0) {
		tf_printf("ERROR: BIST data pattern length can't be 0\n");
		return 0;
	}
	if (params->pattern_length > BIST_MAX_PATTERN_SIZE) {
		tf_printf("ERROR: Pattern length is bigger than %d bytes(%u)\n",
			  BIST_MAX_PATTERN_SIZE, params->pattern_length);
		return 0;
	}
	if (params->pattern_length % PATTERN_LINE_LENGTH) {
		tf_printf("ERROR: Pattern length does not fit exactly in"
			  " SRAM whole lines.\n");
		return 0;
	}

	lines = params->pattern_length / PATTERN_LINE_LENGTH;

	for (int abs_mc = 0; abs_mc < MAX_MEM_CTRL; abs_mc++) {
		if (!(params->mc_mask & (1 << abs_mc)))
			continue;
		if (!ddr_switch_current_mss(abs_mc))
			continue;

		uint32_t address = 0;
		uint8_t *curr_pattern = params->pattern;

		dbg_printf(3, "set_bist_pattern MSS=%d writing sram lines\n",
			   abs_mc);

		for (int line = 0; line < lines; line++) {
			write_emem_mc_sram_line((uint32_t *)curr_pattern,
						address);
			address++;
			curr_pattern += PATTERN_LINE_LENGTH;
		}
	}

	return 1;
}

/*
 * Set up the BIST parameter registers in the memory controller according to
 * the BIST that we are going to perform.
 */
static void configure_bist(struct bist_parameters *params, uint32_t pattern)
{
	uint32_t base_address;
	uint32_t high_address;

	EMC_IFC_BIST_DIMM_RANGE_t emc_ifc_bist_dimm_range;

	/*
	 * These are the timing values tuned for BlueField. It is fixed for
	 * all the different BIST setups.
	 */
	EMC_IFC_BIST_TIMING_PARAMS_t ifc_bist_timing_params = {
		.ack_data_gap = 0x2,
		.ack_addr_gap = 0x4,
	};

	/* The configuration is the same for all the MSS. */
	EMC_IFC_BIST_CFG_t ifc_bist_cfg = {
		.scan_mode = g_scan_mode,
		.rd_enable = params->op_mode == COMPARE_ONLY ||
			     params->op_mode == WRITE_COMPARE,
		.wr_enable = params->op_mode == WRITE_ONLY ||
			     params->op_mode == WRITE_COMPARE,
		.seq_len =
		      params->pattern_length / PATTERN_LINE_LENGTH,
		.inc_addr_operand = BIST_INC_ADDR_1_BYTE,
		.rep_num = params->rep_mode,
	};

	/*
	 * Update the global value which indicates which BIST was last
	 * configured
	 */
	bist_current_patt = pattern;

	dbg_printf(3, "configure_bist\n");

	/*
	 * Write BIST configuration to the relevant parameters.
	 * All devices are configured with the same values.
	 */
	for (int abs_mc = 0; abs_mc < MAX_MEM_CTRL; abs_mc++) {
		if (!(params->mc_mask & (1 << abs_mc)))
			continue;
		if (!ddr_switch_current_mss(abs_mc))
			continue;

		dbg_printf(3, "configure_bist start for mss: %d mc_mask=%d\n",
			   abs_mc, params->mc_mask);

		/*
		 * Set up the range of ranks where the BIST happens. Right now
		 * we are using a fixed configuration of running on all logical
		 * ranks if we have a 3DS device.
		 */
		emc_ifc_bist_dimm_range = (EMC_IFC_BIST_DIMM_RANGE_t) {
			.min_lr = 0,
			.max_lr = __builtin_ctz(dp->ddr_3ds),
			.min_pr = __builtin_ctz(params->mc_pr_mask[abs_mc]),
			.max_pr = __builtin_ctz(params->mc_pr_mask[abs_mc]) +
				  popcount(params->mc_pr_mask[abs_mc]) - 1,
		};
		emc_write(EMC_IFC_BIST_DIMM_RANGE,
			  emc_ifc_bist_dimm_range.word);

		/*
		 * We are writing from the beginning of the SRAM the BIST
		 * pattern unless we have the g_bist_xxx_addr global setting
		 * variable set.
		 */
		base_address = 0;

		high_address = get_high_addr();

		if (g_bist_high_addr != 0)
			high_address = MIN((uint32_t)g_bist_high_addr,
					    high_address);
		if (g_bist_low_addr != 0)
			base_address = MIN(MAX(base_address,
					       (uint32_t)g_bist_low_addr),
					   high_address);

		dbg_printf(4, "base_address=0x%x high_address=0x%x\n",
			   base_address, high_address);

		/* Ensure that BIST is not running before updating values. */
		emc_write(EMC_IFC_BIST_EN, 0);

		/* Write Base\High addr data */
		emc_write(EMC_IFC_BIST_BASE_ADDR, base_address);

		emc_write(EMC_IFC_BIST_HIGH_ADDR, high_address);
		emc_write(EMC_IFC_BIST_TIMING_PARAMS,
			  ifc_bist_timing_params.word);

		uint32_t *bist_mask =
				(uint32_t *)ifc_bist_mask[abs_mc][pattern];

		if (ifc_bist_mask_state[abs_mc][pattern] ==
		    BIST_MASK_UNINITIALIZED)
			reset_dqmask(1 << abs_mc, pattern);

		for (int i = 0; i < EMC_IFC_BIST_MASK__LENGTH; i++)
			emc_write(EMC_IFC_BIST_MASK__FIRST_WORD + i,
				  bist_mask[i]);

		emc_write(EMC_IFC_BIST_CFG, ifc_bist_cfg.word);

		dbg_printf(4, "configure_bist Base Address: 0x%x(%d) "
			   "High Address: 0x%x(%d)\n", base_address,
			   base_address, high_address, high_address);

		/*
		 * If we are doing BIST in random mode, there are additional
		 * settings that we need to do, e.g. setting the seed.
		 */
		if (g_scan_mode == BIST_SCAN_MODE__RANDOM) {
			int cnt_offset;

			if (!check_rnd_scan_support()) {
				tf_printf("Random scan mode not supported for "
					  "current DIMM config in MSS%d\n",
					  abs_mc);
				params->mc_mask &= ~(1 << abs_mc);
				break;
			}

			emc_write(EMC_IFC_BIST_ADDR_LFSR,
				  g_bist_rand_scan_seed);
			emc_write(EMC_IFC_BIST_RND_BITS,
				  get_rnd_bits(&cnt_offset));
			emc_write(EMC_IFC_BIST_RND_BITS_LEN,
				  (1 << cnt_offset) - 1);
			emc_write(EMC_IFC_BIST_RND_SEQ_LEN,
				  MIN((1ull << 32) - 1,
				       1ull << (25 + cnt_offset)));
		}
	}

}

/* Execute the actual BIST. */
static void execute_bist(struct bist_parameters *params)
{
	EMC_IFC_BIST_EN_t ifc_bist_en = {
		.val = 1, /* Set this bit to enable BIST. */
	};

	for (int abs_mc = 0; abs_mc < MAX_MEM_CTRL; abs_mc++) {
		if (!(params->mc_mask & (1 << abs_mc)))
			continue;
		if (!ddr_switch_current_mss(abs_mc))
			continue;

		dbg_printf(3, "execute_bist start for mss: %d\n", abs_mc);

		/* Disable the arbiter alogorithm so BIST can work. */
		set_arb_algorithm(0);

		/* start bist */
		emc_write(EMC_IFC_BIST_EN, ifc_bist_en.word);
	}
}

/*
 * Get the result of the previous BIST run.
 * Return 0 if the BIST was successful or the corresponding bit
 * set to 1 if BIST failed on that memory controller.
 */
static int get_bist_results(uint32_t mc_mask, int print_info)
{
	uint32_t status = 0;

	dbg_printf(3, "get_bist_results\n");

	for (int abs_mc = 0; abs_mc < MAX_MEM_CTRL; abs_mc++) {
		if (!(mc_mask & (1 << abs_mc)))
			continue;
		if (!ddr_switch_current_mss(abs_mc))
			continue;

		uint32_t errors_num;

		dbg_printf(5, "abs_mc=%d, mc_mask=%d\n", abs_mc, mc_mask);
		dbg_printf(3, "get_bist_results start for mss: %d\n", abs_mc);

		errors_num = emc_read(EMC_IFC_BIST_ERR_CNTR);

		if (errors_num > 0 && g_debug_level > 0)
			tf_printf("  Memory Device: %d BIST Failed\n", abs_mc);
		else if (print_info)
			tf_printf("  Memory Device: %d BIST Passed\n", abs_mc);

		dbg_printf(3, "\tBIST Status = 0x%x :\n",
			emc_read(EMC_IFC_BIST_STATUS));

		if (errors_num > 0) {
			status |= 1 << abs_mc;
			loop_results[bist_current_patt][abs_mc]++;
			if (g_debug_level > 0)
				print_error_info(abs_mc);
		}
	}

	return status;
}
/*
 * Wait for the BIST machine to complete.
 * Return 1 if it completed or 0 if timed out or user exited with Ctrl-C.
 */
static int stop_bist(uint32_t mc_mask, uint32_t print_info)
{
	/*
	 * The BIST can take up to 105s to finish if we are in random scan mode
	 * for 4 rank RDIMMs with total capacity of 64GB. So we wait 2 mintues
	 * here. This might needs to be bumped up further when we test LRDIMMs
	 * with total capacity of more than 64GB.
	 */
	const unsigned int timeout_ms = 120000;
	unsigned int ms;
	EMC_IFC_BIST_STATUS_t bist_status;
	EMC_IFC_BIST_EN_t bist_en;

	for (int abs_mc = 0; abs_mc < MAX_MEM_CTRL; abs_mc++) {
		if (!(mc_mask & (1 << abs_mc)))
			continue;
		if (!ddr_switch_current_mss(abs_mc))
			continue;

		dbg_printf(3, "\nstop_bist MSS %d\n", abs_mc);

		/* Disable BIST. */
		bist_en.word = emc_read(EMC_IFC_BIST_EN);
		bist_en.val = 0;
		emc_write(EMC_IFC_BIST_EN, bist_en.word);

		if (print_info)
			tf_printf("waiting for BIST to complete:\n");

		ms = 0;

		while (ms < timeout_ms) {

			mdelay(1);

			bist_status.word = emc_read(EMC_IFC_BIST_STATUS);

			if (bist_status.end_of_test == 1)
				break;
			if (ctrlc()) {
				ms = timeout_ms;
				break;
			}
			ms++;
		}

		if (ms >= timeout_ms) {
			ERROR("Unable to stop bist at MSS %d\n", abs_mc);
			return 0;
		}
	}
	return 1;
}

/*
 * Start a BIST run based on the given params. If we are running in single
 * run mode we also wait for it to stop.
 * Return 0 if the BIST was successful, -1 if BIST can't be stopped at single
 * mode or the bitmask of the memory controllers whose BIST failed.
 */
static int start_bist(struct bist_parameters *params, uint32_t pattern,
		      int print_info)
{
	int status = 0;

	if (print_info)
		print_bist_parameters(params, 1, pattern);

	configure_bist(params, pattern);

	execute_bist(params);

	if (params->rep_mode == SINGLE) {
		if (!stop_bist(params->mc_mask, print_info)) {
			tf_printf("Unable to stop test at single mode\n");
			return -1;
		}
		if (params->op_mode != WRITE_ONLY)
			status = get_bist_results(params->mc_mask,
						  print_info);

		enable_emem_mc_hw(params->mc_mask);

		/* Mark the ranks as zeroed if the zero bist passed */
		if (pattern == INIT_00_PATTERN && status == 0)
			for (int i = 0; i < MAX_MEM_CTRL; i++)
				if (params->mc_mask & (1 << i))
					dps[i].zeroed |= params->mc_pr_mask[i];
	}

	return status;
}

#ifdef ATF_CONSOLE
/****************************************************************************/
/* BIST loop related functions.                                             */
/****************************************************************************/
/*
 * Do the random BIST run iterations number of times, each time writing the
 * random pattern once and then reading it read_num times.
 * mc_mask	: Mask of the memory controllers to do the BIST run.
 * mc_pr_mask	: Masks of the physical ranks to do the BIST on.
 * iterations	: Number of iterations to do the BIST run.
 * read_num	: Number of times to read the BIST pattern from DRAM for each
 *		  iteration.
 * is_fixed_seed: If set, then we use the given seed as the fixed seed
 *		  throughout each iteration, thus having the same random pattern
 *		  for each iteration. Else the seed is just used as the base
 *		  seed to generate different patterns for each iteration.
 * seed		: The seed used to generate the random pattern.
 */
static void read_bist_loop(uint32_t mc_mask, int mc_pr_mask[MAX_MEM_CTRL],
			   uint32_t iterations, uint32_t read_num,
			   int is_fixed_seed, uint64_t seed)
{
	int bist_failed = 0;
	struct bist_parameters params;

	dbg_printf(3, "read_bist_loop mc_mask=0x%x iterations=%d read_num=%d "
		   "fix_seed=%d seed=0x%llx\n", mc_mask, iterations, read_num,
		   is_fixed_seed, seed);

	params = bist_configs[RANDOM_PATTERN];
	params.mc_mask = mc_mask;
	params.mc_pr_mask[0] = mc_pr_mask[0];
	params.mc_pr_mask[1] = mc_pr_mask[1];

	for (uint32_t bist = 0; bist < iterations; bist++) {
		dbg_printf(3, "iteration %d\n", bist);

		tf_printf("writing seed %lld\n", seed);

		/* Generate and write the random pattern to SRAM. */
		bist_srandom(seed);
		for (int i = 0; i < params.pattern_length; i++)
			params.pattern[i] = bist_random() & 0xFF;

		if (!set_bist_pattern(&params)) {
			tf_printf("set_bist_pattern failed\n");
			return;
		}
		/* Write the pattern to DRAM. */
		params.op_mode = WRITE_ONLY;
		bist_failed |= start_bist(&params, RANDOM_PATTERN, 0);

		for (int reads = 0; reads < read_num; reads++) {
			params.op_mode = COMPARE_ONLY;
			bist_failed |= start_bist(&params, RANDOM_PATTERN, 0);
			tf_printf(".");
		}
		tf_printf("\n");

		if (!is_fixed_seed) {
			dbg_printf(3, "changing seed on iteration %d\n", bist);
			seed = bist_random() & 0xFFFFFFFF;
			bist_srandom(seed);
		}
	}
	/* Report the BIST result. */
	for (int abs_mc = 0; abs_mc < MAX_MEM_CTRL; abs_mc++) {
		if (!(mc_mask & (1 << abs_mc)))
			continue;

		if (bist_failed & (1 << abs_mc))
			tf_printf("  Memory Device: %d BIST Failed\n", abs_mc);
		else
			tf_printf("  Memory Device: %d BIST Passed\n", abs_mc);
	}
}
/*
 * Do the random BIST run iterations number of times.
 * mc_mask	: Mask of the memory controllers to do the BIST run.
 * mc_pr_mask	: Masks of the phyical ranks to do the BIST on.
 * iterations	: Number of iterations to do the BIST run.
 * is_fixed_seed: If set, then we use the given seed as the fixed seed
 *		  throughout each iteration, thus having the same random pattern
 *		  for each iteration. Else the seed is just used as the base
 *		  seed to generate different patterns for each iteration.
 * seed		: The seed used to generate the random pattern.
 */
static void bist_loop_rand(uint32_t mc_mask, int mc_pr_mask[MAX_MEM_CTRL],
			   uint32_t iterations, int is_fixed_seed,
			   uint64_t seed)
{
	struct bist_parameters params;

	dbg_printf(3, "bist_loop_rand mc_mask=0x%x mc0=0x%x mc1=0x%x "
		   "iterations=%d is_fixed_seed=%d seed=%lld\n",
		   mc_mask, mc_pr_mask[0], mc_pr_mask[1], iterations,
		   is_fixed_seed, seed);

	tf_printf("is_fixed_seed = %s\n", is_fixed_seed ? "true" : "false");

	params = bist_configs[RANDOM_PATTERN];
	params.mc_mask = mc_mask;
	params.mc_pr_mask[0] = mc_pr_mask[0];
	params.mc_pr_mask[1] = mc_pr_mask[1];

	for (int bist = 0; bist < iterations; bist++) {
		tf_printf("Seed %llu\n", seed);

		bist_srandom(seed);
		for (int i = 0; i < params.pattern_length; i++)
			params.pattern[i] = bist_random() & 0xFFFFFFFF;

		tf_printf("set_bist_pattern index=%d\n", bist);
		if (!set_bist_pattern(&params)) {
			printf("set_bist_pattern failed\n");
			return;
		}

		if (ctrlc())
			break;

		tf_printf("start_bist_pattern index=%d\n", bist);
		start_bist(&params, 8, 0);

		if (!is_fixed_seed) {
			seed = bist_random() & 0xFFFFFFFF;
			bist_srandom(seed);
		}
	}
	tf_printf("\nSeed is %llu\n", seed);
}

/*
 * Do BIST on each of the BIST patterns iterations number of times.
 * mc_mask	: Mask of the memory controllers to do BIST on.
 * mc_pr_mask	: Mask of the physical ranks to do BIST on.
 * iterations	: Number of iterations to do the BIST on each pattern.
 * Return 0 if successful or non-zero if something failed.
 */
static int bist_loop(uint32_t mc_mask, int mc_pr_mask[MAX_MEM_CTRL],
		     uint32_t iterations)
{
	struct bist_parameters params;

	params.mc_mask = mc_mask;
	params.mc_pr_mask[0] = mc_pr_mask[0];
	params.mc_pr_mask[1] = mc_pr_mask[1];

	memset(loop_results, 0x0, sizeof(loop_results));

	for (int config = 0; config < RANDOM_PATTERN; config++) {
		tf_printf("current config=%d\n", config);

		params = bist_configs[config];
		params.mc_mask = mc_mask;
		params.mc_pr_mask[0] = mc_pr_mask[0];
		params.mc_pr_mask[1] = mc_pr_mask[1];

		tf_printf("setting bist pattern\n");
		if (!set_bist_pattern(&params)) {
			tf_printf("set_bist_pattern failed\n");
			return -1;
		}
		for (int bist = 0; bist < iterations; bist++) {
			tf_printf("bist iteration %d\n", bist);

			if (ctrlc())
				break;

			start_bist(&params, config, 0);
		}
	}
	tf_printf("\n\t");

	/* Print out the BIST results. */
	for (int mc = 0; mc < MAX_MEM_CTRL; mc++) {
		if (!(mc_mask & (1 << mc)))
			continue;
		tf_printf("\tMSS%d", mc);
	}
	tf_printf("\n");
	for (int config = 0; config < RANDOM_PATTERN; config++) {
		tf_printf("config %d:", config);
		for (int mc = 0; mc < MAX_MEM_CTRL; mc++) {
			if (!(mc_mask & (1 << mc)))
				continue;
			tf_printf("\t %d", loop_results[config][mc]);
		}
		tf_printf("\n");
	}

	return 0;
}
/*****************************************************************************/
/* The actual BIST handling functions (and help text) lies here		     */
/****************************************************************************/
/*
 * Piece of code which checks if the help argument was entered and prints the
 * help string.
 */
#define HELP_CHECK(cmd)	do {					\
	if (argc >= 2 && strcmp(argv[1], "help") == 0) {	\
		tf_printf(bist_##cmd##_help_str);		\
		return 0;					\
	}							\
} while (0)

/*
 * Piece of code which checks if parsed is 0, if it is it hints the user to use
 * help command and return from the function.
 */
#define CHECK_PARSE(cmd) do {					\
	if (parsed == 0) {					\
		tf_printf("Enter \"ddr_bist " #cmd " help\" "	\
			  "for help with this command.\n");	\
		return 1;					\
	}							\
} while (0)

/*
 * Piece of code which checks if the pattern number entered is correct.
 * If the random pattern, it turns it into the internally used
 * value. Complains and quits if the value entered is not valid.
 */
#define PROCESS_PATTERN() do {						\
	if (pattern >= BIST_RANDOM_MODE && 				\
	    pattern < BIST_PATTERN_NUM + PATT_SHIFT)			\
		pattern -= PATT_SHIFT;					\
	else if (pattern >= RANDOM_PATTERN) {				\
		tf_printf("Pattern value entered is not valid!\n");	\
		return 1;						\
	}								\
	fill_pattern(pattern);						\
} while (0)


/*
 * Piece of code which checks that the entered mc_mask and mc_pr_mask are
 * valid.
 */
#define CHECK_MASK_VALID() do {						\
	for (int i = 0; i < MAX_MEM_CTRL; i++) {			\
		if (!(mc_mask & (1 << i)) && mc_pr_mask[i] > 0) {	\
			tf_printf("mc%d_pr_mask non-zero while mc_mask"	\
				  " masked it with 0.\n", i);		\
			return 1;					\
		}							\
		if (!(mc_mask & (1 << i)))				\
			continue;					\
		if (mc_pr_mask[i] == 0)	{				\
			tf_printf("mc_mask specified MSS%d but mc%d"	\
				  "_pr_mask have value of 0.\n", i, i);	\
			return 1;					\
		}							\
		/* ddr_switch_current_mss() will print the error msg. */\
		if (!ddr_switch_current_mss(i))				\
			return 1;					\
		if (popcount(mc_pr_mask[i]) +				\
		    __builtin_ctz(mc_pr_mask[i]) +			\
		    __builtin_clz(mc_pr_mask[i]) !=			\
		    sizeof(mc_pr_mask[i]) * 8) {			\
			tf_printf("mc%d_pr_mask have non-adjacent rank"	\
				  " combinations!\n", i);		\
			return 1;					\
		}							\
		uint32_t active_ranks = emc_read(EMC_ACTIVE_RANKS);	\
		for (int r = 0; r < MAX_ACTIVE_RANKS; r++) {		\
			if (!(mc_pr_mask[i] & (1 << r)))		\
				continue;				\
			if (!(active_ranks & (1 << (8 * r)))) {		\
				tf_printf("Rank %d of MSS %d not "	\
				  "present in hardware.\n", r, i);	\
				return 1;				\
			}						\
		}							\
	}								\
} while (0)

static const char bist_verbose_help_str[] =
"  ddr_bist verbose\n"
"  -----------------\n"
"	      <debug-level> = 0 - 10 (default is 1)\n"
"			  a higher number will result in more output.\n"
"\n";

int ddr_bist_verbose(int argc, char *const argv[])
{
	int debug_level;
	int parsed;

	HELP_CHECK(verbose);

	parsed = (argc >= 2) &&
	  get_arg_int(argv[1], &debug_level, 1, 0, DEBUG_LEVEL_MAX + 1,
		"Debug level");

	CHECK_PARSE(verbose);

	tf_printf("changing DDR BIST debug level from %d to %d\n",
		  g_debug_level, debug_level);

	g_debug_level = debug_level;

	return 0;
}

static const char bist_dqmask_help_str[] =
"ddr_bist dqmask			User Command\n"
"\n"
"  ddr_bist dqmask <command>\n"
"		   <command> = set\n"
"			       reset\n"
"			       show\n"
"			       help\n"
"  ddr_bist dqmask help\n"
"  --------------------\n"
"  This command displays help for the ddr_bist dqmask command.\n"
"\n"
"  ddr_bist dqmask set\n"
"  -------------------\n"
"  This command sets the byte mask for a particular byte in the dqmask\n"
"      ddr_bist dqmask set <mc_mask> <pattern> <dqmask_bytenum> <dq_mask>\n"
"	      <mc_mask> = bit mask indicating which mem controller\n"
"			  1 - mem controller 1\n"
"			  2 - mem controller 2\n"
"	      <pattern>  =  pattern-index\n"
"			 0-7: inclusive for a pattern index\n"
"	      <dqmask_bytenum> = byte number (valid values: 0-35)\n"
"	      <dq_mask> = mask value for the byte specified in\n"
"			  <dqmask_bytenum>\n"
"\n"
"  ddr_bist dqmask reset\n"
"  ---------------------\n"
"  This command resets the entire DQ mask(36 bytes) to its default state\n"
"      ddr_bist dqmask reset <mc_mask> <pattern>\n"
"	      <mc_mask> = bit mask indicating which mem controller\n"
"			  1 - mem controller 1\n"
"			  2 - mem controller 2\n"
"	      <pattern>  =  pattern-index\n"
"			 0-7: inclusive for a pattern index\n"
"\n"
"  ddr_bist dqmask show\n"
"  ---------------------\n"
"  This command displays the entire DQ mask(36 bytes) that is active for a\n"
"  pattern\n"
"      ddr_bist dqmask show <mc_mask> <pattern>\n"
"	      <mc_mask> = bit mask indicating which mem controller\n"
"			  1 - mem controller 1\n"
"			  2 - mem controller 2\n"
"	      <pattern>  =  pattern-index\n"
"			 0-7: inclusive for a pattern index\n"
"\n";

int ddr_bist_dqmask(int argc, char *const argv[])
{
	enum {
		CMD_SET = 0,
		CMD_RESET,
		CMD_SHOW,
	};

	int parsed;
	int cmd;

	HELP_CHECK(dqmask);

	parsed = (argc >= 2) &&
	  get_arg_str(argv[1], &cmd, "Command",
		      3, "set", CMD_SET, "reset", CMD_RESET, "show", CMD_SHOW);

	CHECK_PARSE(dqmask);

	switch (cmd) {
	case CMD_SET:
	{
		int mc_mask;
		int pattern;
		int byte_num;
		int byte_mask;

		parsed = (argc >= 6) &&
		  get_arg_int(argv[2], &mc_mask, 1, 0, MAX_MC_MSK, "Mc_mask") &&
		  get_arg_int(argv[3], &pattern, 0, 0, 0, "Pattern") &&
		  get_arg_int(argv[4], &byte_num, 0, 0, DQ_MASK_SIZE,
			"Byte number") &&
		  get_arg_int(argv[5], &byte_mask, 1, 0, 0x100, "Byte mask");

		CHECK_PARSE(dqmask);
		PROCESS_PATTERN();

		tf_printf("byte_num=%d dq-mask-size=%d\n",
			  byte_num, DQ_MASK_SIZE);
		if (byte_num >= DQ_MASK_SIZE) {
			tf_printf("Byte number %d equates to ", byte_num);
			byte_num = byte_num % DQ_MASK_SIZE;
			tf_printf("DQMask byte %d\n", byte_num);
		}
		set_dqmask(mc_mask, pattern, byte_num, byte_mask);
		return 0;
	}
	case CMD_RESET:
	{
		int mc_mask;
		int pattern;

		parsed = (argc >= 4) &&
		  get_arg_int(argv[2], &mc_mask, 1, 0, MAX_MC_MSK, "Mc_mask") &&
		  get_arg_int(argv[3], &pattern, 0, 0, 0, "Pattern");

		CHECK_PARSE(dqmask);
		PROCESS_PATTERN();

		reset_dqmask(mc_mask, pattern);
		return 0;
	}
	case CMD_SHOW:
	{
		int mc_mask;
		int pattern;

		parsed = (argc >= 4) &&
		  get_arg_int(argv[2], &mc_mask, 1, 0, MAX_MC_MSK, "Mc_mask") &&
		  get_arg_int(argv[3], &pattern, 0, 0, 0, "Pattern");

		CHECK_PARSE(dqmask);
		PROCESS_PATTERN();

		show_dqmask(mc_mask, pattern);
		return 0;
	}
	default:
		tf_printf(bist_dqmask_help_str);
		return 0;
	}

}

static const char bist_show_help_str[] =
"  ddr_bist show\n"
"  -------------\n"
"  ddr_bist show <pattern>\n"
"      Display the pattern information for the given <pattern>\n"
"	  <pattern> is the pattern index in range of [0:8) & [11:24).\n"
"			0: Killer Pattern\n"
"			1: Walking 1\n"
"			2: Walking 0\n"
"			3: Repeating FF_00\n"
"			4: Repeating AA_55\n"
"			5: Address Pattern\n"
"			6: All 00_00\n"
"			7: All FF_FF\n"
"			11: Last generated random pattern\n"
"			12: DQ0 single edge 0=>1=>0\n"
"			13: DQ0 eight edges 0=>1=>0\n"
"			14: DQ0 single edge 1=>0=>1\n"
"			15: DQ0 eight edges 1=>0=>1\n"
"			16: DQ28 single edge 0=>1=>0\n"
"			17: DQ28 eight edges 0=>1=>0\n"
"			18: DQ28 single edge 1=>0=>1\n"
"			19: DQ28 eight edges 1=>0=>1\n"
"			20: DQ53 single edge 0=>1=>0\n"
"			21: DQ53 eight edges 0=>1=>0\n"
"			22: DQ53 single edge 1=>0=>1\n"
"			23: DQ53 eight edges 1=>0=>1\n"
"\n";

int ddr_bist_show(int argc, char *const argv[])
{
	struct bist_parameters bp;
	int parsed;
	int pattern;

	HELP_CHECK(show);

	parsed = (argc >= 2) &&
	  get_arg_int(argv[1], &pattern, 0, 0, 0, "Pattern");

	CHECK_PARSE(show);
	PROCESS_PATTERN();

	/*
	 * Tell print_bist_parameters to show the dqmasks of all the MSS
	 * which are present on chip.
	 */
	bp = bist_configs[pattern];
	bp.mc_mask = 0;
	for (int i = 0; i < MAX_MEM_CTRL; i++)
		if (dps[i].dimm_num)
			bp.mc_mask |= 1 << i;

	print_bist_parameters(&bp, 0, pattern);

	return 0;
}

static const char bist_run_help_str[] =
"  ddr_bist run\n"
"  ---------------\n"
"	This command runs a bist command.\n"
"	By default it will run the BIST in logical rank scan mode.\n"
"	To change to random scan mode, use the \"settings\" command and\n"
"	change the \"bist_scan_mode\" parameter to \"random\".\n"
"\n"
"	syntax for all patterns (excluding random pattern):\n"
"      ddr_bist run <mc_mask> <mc0_pr_mask> <mc1_pr_mask> <pattern>\n"
"		    [<rep-mode> <op-mode>]\n"
"	syntax for random pattern:\n"
"      ddr_bist run <mc_mask> <mc0_pr_mask> <mc1_pr_mask> 11 <seed>\n"
"		    [<rep-mode> <op-mode>]\n"
"	      <mc_mask> = bit mask indicating which mem controller\n"
"			  1 - mem controller 1\n"
"			  2 - mem controller 2\n"
"	      <mc0_pr_mask> = bit mask indicating physical rank of MSS 0\n"
"			  1 - indicates 1st rank\n"
"			  2-  indicates 2nd rank\n"
"			  4 - indicates 3rd rank\n"
"			  8 - indicates 4th rank\n"
"			  Note this is a mask; operations on non-adjacent\n"
"			  combination are not supported.\n"
"	      <mc1_pr_mask> = bit mask indicating physical rank of MSS 1\n"
"			  1 - indicates 1st rank\n"
"			  2-  indicates 2nd rank\n"
"			  4 - indicates 3rd rank\n"
"			  8 - indicates 4th rank\n"
"			  Note this is a mask; operations on non-adjacent\n"
"			  combination are not supported.\n"
"	      <pattern>  =  pattern-index\n"
"			 0-7: inclusive for a pattern index\n"
"			 11 -- BIST_RANDOM_MODE - Note that this pattern type\n"
"						  have different syntax\n"
"	      <rep-mode>  = BIST rep mode\n"
"			  endless -- BIST runs forever\n"
"			  single  -- BIST runs once\n"
"			  until_error -- BIST runs until it errors\n"
"	      <op-mode>  = BIST operation mode\n"
"			  write_only	   -- only performs writes\n"
"			  compare_only	   -- only compares\n"
"			  write_compare    -- BIST writes then compares\n"
"	      <seed>	 =  integer value of seed for random function\n"
"\n";

int ddr_bist_run(int argc, char *const argv[])
{
	struct bist_parameters params;
	int mc_mask, mc_pr_mask[MAX_MEM_CTRL], pattern, seed, parsed;
	int rep_mode, op_mode;

	HELP_CHECK(run);

	/* Get the arguments up to the pattern arguemnts. */
	parsed = (argc >= 5) &&
	  get_arg_int(argv[1], &mc_mask, 1, 0, MAX_MC_MSK, "Mc_mask") &&
	  get_arg_int(argv[2], &mc_pr_mask[0], 1, 0, MAX_PMSK, "Mc0_pr_mask") &&
	  get_arg_int(argv[3], &mc_pr_mask[1], 1, 0, MAX_PMSK, "Mc1_pr_mask") &&
	  get_arg_int(argv[4], &pattern, 0, 0, 0, "Pattern");

	CHECK_PARSE(run);
	CHECK_MASK_VALID();
	PROCESS_PATTERN();

	/*
	 * Get the default values of the optional rep_mode and op_mode arguemnts
	 * before continue parsing the input.
	 */
	params = bist_configs[pattern];

	rep_mode = params.rep_mode;
	op_mode = params.op_mode;

	if (pattern == RANDOM_PATTERN) {

		/* Parse remaining arguemnts for the random pattern format. */
		parsed = (argc >= 6) &&
		  get_arg_int(argv[5], &seed, 1, 1, INT_MAX, "Seed") &&
		  (argc < 7 || /* rep_mode optional argument. */
		  get_arg_str(argv[6], &rep_mode, "Rep-mode",
		     3, "endless", ENDLESS,
			"single", SINGLE,
			"until_error", UNTIL_ERROR)) &&
		  (argc < 8 || /* op_mode optional argument. */
		  get_arg_str(argv[7], &op_mode, "Op-mode",
		     3, "write_only", WRITE_ONLY,
			"compare_only", COMPARE_ONLY,
			"write_compare", WRITE_COMPARE));

		CHECK_PARSE(run);

		/*
		 * Now we generate the pseudo random bist pattern based on the
		 * given seed.
		 */
		bist_srandom(seed);

		dbg_printf(3, "ddr_print run_mode - random pattern");

		for (int index = 0; index < params.pattern_length; index++)
			params.pattern[index] = bist_random() & 0xFF;

	} else {

		/* Parse remaining arguemnts for the normal pattern format. */
		parsed = (argc < 6 || /* rep_mode optional argument. */
		  get_arg_str(argv[5], &rep_mode, "Rep-mode",
		     3, "endless", ENDLESS,
			"single", SINGLE,
			"until_error", UNTIL_ERROR)) &&
		  (argc < 7 || /* op_mode optional argument. */
		  get_arg_str(argv[6], &op_mode, "Op-mode",
		     3, "write_only", WRITE_ONLY,
			"compare_only", COMPARE_ONLY,
			"write_compare", WRITE_COMPARE));

		CHECK_PARSE(run);
	}

	/*
	 * Fill in the parameters got from the command line. If the optional
	 * parameters are not specificed, the variables will have the default
	 * values.
	 */
	params.mc_mask = mc_mask;
	params.mc_pr_mask[0] = mc_pr_mask[0];
	params.mc_pr_mask[1] = mc_pr_mask[1];
	params.rep_mode = rep_mode;
	params.op_mode = op_mode;

	tf_printf("params.length=0x%x\n", params.pattern_length);

	/* Set the bist pattern to SRAM. */
	if (!set_bist_pattern(&params)) {
		printf("set_bist_pattern failed\n");
		return -1;
	}
	start_bist(&params, pattern, 1);

	return 0;
}

static const char bist_dump_help_str[] =
"  ddr_bist dump\n"
"  -------------\n"
"      Stop BIST and get BIST results if error.\n"
"      Note:  This is only relevant if until_error or endless\n"
"	      modes were activated.\n"
"      ddr_bist dump <mc_mask> stop\n"
"	      <mc_mask> = bit mask indicating which mem controller\n"
"		     1 - mem controller 1\n"
"		     2 - mem controller 2\n"
"	       stop  Relevant ONLY if error or endless modes were\n"
"		     activated. If not, the test will not stop.\n"
"\n";

int ddr_bist_dump(int argc, char *const argv[])
{
	int mc_mask, stop = 0;
	int parsed;

	HELP_CHECK(dump);

	parsed = (argc >= 2) &&
	  get_arg_int(argv[1], &mc_mask, 1, 0, MAX_MC_MSK, "Mc_mask") &&
	  (argc < 3 || get_arg_str(argv[2], &stop, NULL, 1, "stop", 1));

	CHECK_PARSE(dump);

	if (stop) {
		if (!stop_bist(mc_mask, 1)) {
			get_bist_results(mc_mask, 1);
			tf_printf("stop bist failed\n");
			return 1;
		}
		get_bist_results(mc_mask, 1);
		enable_emem_mc_hw(mc_mask);
	} else
		get_bist_results(mc_mask, 1);

	return 0;
}

static const char bist_pattern_help_str[] =
"  ddr_bist pattern\n"
"  ----------------\n"
"      Update an existing defined BIST pattern in SRAM as defined by\n"
"      pattern-type by inverting the contents of the byte\n"
"      (which is indexed by change-byte)\n"
"\n"
"      ddr_bist pattern <mc_mask> <pattern-type> <change_byte> [<value>]\n"
"	      <mc_mask> = bit mask indicating which mem controller\n"
"		     1 - mem controller 1\n"
"		     2 - mem controller 2\n"
"	  <pattern-type> is the pattern index in range of [0:8) or 11.\n"
"	  <change-byte>  byte index in the pattern whose value\n"
"			 will be inverted\n"
"	  [<value>] - optional argument - if specified the\n"
"		      change-byte in pattern-type will be set to this\n"
"		      provided value; otherwise, the value of the\n"
"		      specified change-byte will be inverted.\n"
"\n";

int ddr_bist_pattern(int argc, char *const argv[])
{
	struct bist_parameters params;
	int mc_mask, pattern, change_byte, value;
	int parsed;

	HELP_CHECK(pattern);

	/*
	 * We do the parsing in three steps here, first get the pattern index so
	 * we can know the length of the pattern to check if the input byte
	 * length is valid. Second we get the byte index so that we can get the
	 * default value of the value to change (the inversed value). Finally we
	 * read the value to change if there is one.
	 */
	parsed = (argc >= 3) &&
	  get_arg_int(argv[1], &mc_mask, 1, 0, MAX_MC_MSK, "Mc_mask") &&
	  get_arg_int(argv[2], &pattern, 0, 0, 0, "Pattern");

	CHECK_PARSE(pattern);
	PROCESS_PATTERN();

	params = bist_configs[pattern];

	parsed = (argc >= 4) &&
	  get_arg_int(argv[3], &change_byte, 1, 0, params.pattern_length,
		"Change byte");

	CHECK_PARSE(pattern);

	value = (~params.pattern[change_byte]) & 0xff;

	parsed = (argc < 5) ||
	  get_arg_int(argv[4], &value, 1, 0, 0x100, "Value");

	CHECK_PARSE(pattern);

	dbg_printf(3, "ddr_bist pattern mc_mask=0x%x %s pattern change byte "
		   "%d\n", mc_mask, pattern_2_str[pattern], change_byte);

	tf_printf("Changed byte %d in %s pattern from 0x%x to 0x%x\n",
		  change_byte, pattern_2_str[pattern],
		  params.pattern[change_byte], value);

	params.pattern[change_byte] = value;
	params.mc_mask = mc_mask;

	/* Verbose print the newly changed pattern. */
	if (g_debug_level > 5)
		print_bytes(params.pattern, params.pattern_length, 1,
			    &change_byte);

	/*
	 * Actually change the pattern written in the SRAM to be this updated
	 * one.
	 */
	if (!set_bist_pattern(&params)) {
		tf_printf("set_bist_pattern failed\n");
		return -1;
	}
	return 0;
}
static const char bist_loop_help_str[] =
"  ddr_bist loop\n"
"  -------------\n"
"      Perform a single BIST number of times according to iterations.\n"
"      If the start_seed/fixed seed and seed is specified it will perform\n"
"      BIST on random patterns each iteration, else it will perform each of\n"
"      the stored basic BIST pattern iteration times.\n"
"      ddr_bist loop <mc_mask> <mc0_pr_mask> <mc1_pr_mask> <iterations>\n"
"		     start_seed/fixed_seed <seed>\n"
"	      <mc_mask> = bit mask indicating which mem controller\n"
"			  1 - mem controller 1\n"
"			  2 - mem controller 2\n"
"	      <mc0_pr_mask> = bit mask indicating physical rank of MSS 0\n"
"			  1 - indicates 1st rank\n"
"			  2-  indicates 2nd rank\n"
"			  4 - indicates 3rd rank\n"
"			  8 - indicates 4th rank\n"
"			  Note this is a mask; operations on non-adjacent\n"
"			  combination are not supported.\n"
"	      <mc1_pr_mask> = bit mask indicating physical rank of MSS 1\n"
"			  1 - indicates 1st rank\n"
"			  2-  indicates 2nd rank\n"
"			  4 - indicates 3rd rank\n"
"			  8 - indicates 4th rank\n"
"			  Note this is a mask; operations on non-adjacent\n"
"			  combination are not supported.\n"
"	      <iterations> = number of iterations\n"
"	      start_seed/fixed_seed = seed for random generator\n"
"				start_seed - switches seed randomly\n"
"				      between loop's iterations\n"
"				fixed_seed - same seed for all\n"
"				      loop iterations\n"
"	      <seed> = first iteration's seed\n"
"\n";

int ddr_bist_loop(int argc, char *const argv[])
{
	int mc_mask, mc_pr_mask[MAX_MEM_CTRL], iterations;
	int seed = 0;
	int is_fixed_seed;
	int parsed;

	HELP_CHECK(loop);

	parsed = (argc >= 5) &&
	  get_arg_int(argv[1], &mc_mask, 1, 0, MAX_MC_MSK, "Mc_mask") &&
	  get_arg_int(argv[2], &mc_pr_mask[0], 1, 0, MAX_PMSK, "Mc0_pr_mask") &&
	  get_arg_int(argv[3], &mc_pr_mask[1], 1, 0, MAX_PMSK, "Mc1_pr_mask") &&
	  get_arg_int(argv[4], &iterations, 1, 0, INT_MAX, "Iterations") &&
	  (argc < 6 || get_arg_str(argv[5], &is_fixed_seed,
	    "Start_seed/fixed_seed", 2, "start_seed", 0, "fixed_seed", 1)) &&
	  (argc < 7 || get_arg_int(argv[6], &seed, 1, 1, INT_MAX, "Seed"));

	CHECK_PARSE(loop);
	CHECK_MASK_VALID();

	/*
	 * Seed can't have a value of 0, so if it is 0 then the user must not
	 * have entered the seed value, thus we do the non-rand BIST loop.
	 */
	if (seed)
		bist_loop_rand(mc_mask, mc_pr_mask, iterations,
			is_fixed_seed, seed);
	else
		bist_loop(mc_mask, mc_pr_mask, iterations);

	return 0;
}

static const char bist_rd_loop_help_str[] =
"  ddr_bist rd_loop\n"
"  ---------------\n"
"      This operation write one time the SRAM test data to DDR\n"
"      and then reads the data.\n"
"      ddr_bist rd_loop <mc_mask> <mc0_pr_mask> <mc1_pr_mask> <iterations>\n"
"			<compare_num> start_seed/fixed_seed <seed>\n"
"	      <mc_mask> = bit mask indicating which mem controller\n"
"			  1 - mem controller 1\n"
"			  2 - mem controller 2\n"
"	      <mc0_pr_mask> = bit mask indicating physical rank of MSS 0\n"
"			  1 - indicates 1st rank\n"
"			  2-  indicates 2nd rank\n"
"			  4 - indicates 3rd rank\n"
"			  8 - indicates 4th rank\n"
"			  Note this is a mask; operations on non-adjacent\n"
"			  combination are not supported.\n"
"	      <mc1_pr_mask> = bit mask indicating physical rank of MSS 1\n"
"			  1 - indicates 1st rank\n"
"			  2-  indicates 2nd rank\n"
"			  4 - indicates 3rd rank\n"
"			  8 - indicates 4th rank\n"
"			  Note this is a mask; operations on non-adjacent\n"
"			  combination are not supported.\n"
"	      <iterations> = number of iterations of the entire sequence\n"
"	      <compeare_num> = number of times the data is read and compared\n"
"			       to the SRAM test data per iterations\n"
"	      start_seed/fixed_seed = seed for random generator\n"
"				start_seed - switches seed randomly\n"
"				      between loop's iterations\n"
"				fixed_seed - same seed for all\n"
"				      loop iterations\n"
"	      <seed> = first iteration's seed\n"
"\n";

int ddr_bist_rd_loop(int argc, char *const argv[])
{
	int mc_mask, mc_pr_mask[MAX_MEM_CTRL], iterations;
	int seed, is_fixed_seed;
	int read_num;
	int parsed;

	HELP_CHECK(rd_loop);

	parsed = (argc >= 8) &&
	  get_arg_int(argv[1], &mc_mask, 1, 0, MAX_MC_MSK, "Mc_mask") &&
	  get_arg_int(argv[2], &mc_pr_mask[0], 1, 0, MAX_PMSK, "Mc0_pr_mask") &&
	  get_arg_int(argv[3], &mc_pr_mask[1], 1, 0, MAX_PMSK, "Mc1_pr_mask") &&
	  get_arg_int(argv[4], &iterations, 1, 0, INT_MAX, "Iterations") &&
	  get_arg_int(argv[5], &read_num, 1, 0, INT_MAX, "Compare number") &&
	  get_arg_str(argv[6], &is_fixed_seed, "Start_seed/fixed_seed",
		2, "start_seed", 0, "fixed_seed", 1) &&
	  get_arg_int(argv[7], &seed, 1, 1, INT_MAX, "Seed");

	CHECK_PARSE(rd_loop);
	CHECK_MASK_VALID();

	read_bist_loop(mc_mask, mc_pr_mask, iterations, read_num,
		is_fixed_seed, seed);

	return 0;
}
/****************************************************************************/
/* General help functions that depend on the handling functions above	    */
/****************************************************************************/
struct ddr_bist_action {
	int (*function)(int argc, char *const argv[]);
	const char *help_str;
	const char name[10];
};

#define ADD_BIST_CMD(cmd)				\
	{						\
		.function = ddr_bist_##cmd,		\
		.help_str = bist_##cmd##_help_str,	\
		.name = #cmd,				\
	},

const struct ddr_bist_action bist_action_list[] = {
ADD_BIST_CMD(show)
ADD_BIST_CMD(dump)
ADD_BIST_CMD(run)
ADD_BIST_CMD(pattern)
ADD_BIST_CMD(dqmask)
ADD_BIST_CMD(loop)
ADD_BIST_CMD(rd_loop)
ADD_BIST_CMD(verbose)
};

#define BIST_ACTION_NUM			ARRAYSIZE(bist_action_list)

static void print_ddr_bist_help(int show_all)
{
	tf_printf("ddr_bist\n");
	tf_printf("\n");
	tf_printf("  ddr_bist <command>\n");
	for (int i = 0; i < BIST_ACTION_NUM; i++) {
		if (i == 0)
			tf_printf("	      <command> = %s\n",
				  bist_action_list[i].name);
		else
			tf_printf("			  %s\n",
				  bist_action_list[i].name);
	}
	tf_printf("\n");
	if (show_all) {
		for (int i = 0; i < BIST_ACTION_NUM; i++)
			tf_printf(bist_action_list[i].help_str);
	} else {
		tf_printf("Type \"ddr_bist help all\" to see all help text\n");
	}
}
/****************************************************************************/
/* Function that can be directly called by the command line lies here	    */
/****************************************************************************/

int ddr_bist(int argc, char *const argv[])
{
	if (argc < 2) {
		tf_printf("Please specify BIST command!\n");
		print_ddr_bist_help(0);
		return 1;
	}

	if (strcmp(argv[1], "help") == 0) {
		if (argc >= 3 && strcmp(argv[2], "all") == 0)
			print_ddr_bist_help(1);
		else
			print_ddr_bist_help(0);
		return 0;
	}

	for (int i = 0; i < BIST_ACTION_NUM; i++) {
		if (strcmp(argv[1], bist_action_list[i].name) == 0)
			return bist_action_list[i].function(argc - 1, argv + 1);
	}

	tf_printf("Invalid operation (%s)\n", argv[1]);
	print_ddr_bist_help(0);
	return 1;
}
#endif /* ATF_CONSOLE */

/****************************************************************************/
/* Other functions which can be called externally from other places.        */
/****************************************************************************/
/*
 * Run a BIST with the given pattern number.
 * Return 0 if the BIST finished successfully or non-zero if it failed.
 */
int run_standalone_bist(uint32_t mc_mask, uint32_t mc_pr_mask[MAX_MEM_CTRL],
			int pattern)
{
	uint32_t status = 0;
	int one_run = 1;
	struct bist_parameters bist_params;

	INFO("run_standalone_bist skip_mc=0x%x\n", mc_mask);
	bist_params = bist_configs[pattern];
	bist_params.mc_mask = mc_mask;
	/*
	 * set_bist_pattern() just writes the BIST pattern to the memory
	 * controller's SRAM, thus it doesn't care which ranks are specified.
	 */
	if (!set_bist_pattern(&bist_params)) {
		ERROR("set_bist_pattern failed\n");
		return -1;
	}
	/*
	 * Check that if we can run BIST on all the ranks at once, as BIST can
	 * only be run on adjacent ranks at a time.
	 */
	for (int i = 0; i < MAX_MEM_CTRL; i++)
		if (popcount(mc_pr_mask[i]) + __builtin_ctz(mc_pr_mask[i]) +
		    __builtin_clz(mc_pr_mask[i]) != sizeof(mc_pr_mask[i]) * 8) {
			one_run = 0;
			break;
		}

	if (one_run) {
		for (int i = 0; i < MAX_MEM_CTRL; i++)
			bist_params.mc_pr_mask[i] = mc_pr_mask[i];

		status = start_bist(&bist_params, pattern, 0);

		return status;
	}
	/*
	 * If we can't run the BIST in one run, we start it in sections of the
	 * rank that the BIST can handle.
	 */
	for (int i = 0; i < MAX_MEM_CTRL; i++) {
		if (!(mc_mask & (1 << i)))
			continue;

		uint32_t ranks_left = mc_pr_mask[i];

		bist_params.mc_mask = 1 << i;
		memset(bist_params.mc_pr_mask, 0,
		       sizeof(bist_params.mc_pr_mask));

		while (ranks_left) {
			int start = __builtin_ctz(ranks_left);
			int ranks = __builtin_ctz((~(ranks_left >> start)));
			uint32_t bist_ranks = ((1 << ranks) - 1) << start;

			ranks_left &= ~bist_ranks;
			bist_params.mc_pr_mask[i] = bist_ranks;
			status |= start_bist(&bist_params, pattern, 0);
		}
	}

	return status;
}
/*
 * The BIST we should do regularly when booting up system to ensure that the
 * memory is working.
 * Return 0 if the BIST finished successfully or non-zero if it failed.
 */
int run_default_bist(uint32_t mc_mask, uint32_t mc_pr_mask[MAX_MEM_CTRL])
{
	return run_standalone_bist(mc_mask, mc_pr_mask, KILLER_PATTERN);
}

/*
 * The BIST to do to zero out all the memory for system initialization.
 * Return 0 if it finished successfully or non-zero if it failed.
 */
int init_ddr_by_bist(uint32_t mc_mask, uint32_t mc_pr_mask[MAX_MEM_CTRL])
{
	return run_standalone_bist(mc_mask, mc_pr_mask, INIT_00_PATTERN);
}

