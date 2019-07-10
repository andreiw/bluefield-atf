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

#include <assert.h>
#include <console.h>
#include <debug.h>
#include <delay_timer.h>
#include <limits.h>
#include <mmio.h>
#include <pl011.h>
#include <platform_def.h>
#include <string.h>
#include "bluefield_ddr.h"
#include "bluefield_ddr_bist.h"
#include "bluefield_ddr_print.h"
#include "bluefield_private.h"
#include "bluefield_system.h"
#include "emc.h"
#include "emi.h"
#include "pub.h"

#define WARNING_RET_VAL		0x00000005
#define INVALID_VAL		(-1)
#define D2_VREF_MAX_VAL		74
#define D2_R_VREF_MAX_VAL	64
#define D2_DLL_MAX_VAL		128
/*
 * We are using 1 bit for each data to indicate if the BIST passed or not
 * to save space. (0 = failed, 1 = passed).
 */
#define D2_DLL_ARRAY_IDX(dll_idx)	(((dll_idx)) / (8 * sizeof(int)))
#define D2_DLL_BIT_IDX(dll_idx)		(((dll_idx)) % (8 * sizeof(int)))
#define D2_DLL_INT_USED			D2_DLL_ARRAY_IDX(D2_DLL_MAX_VAL) + \
					(D2_DLL_BIT_IDX(D2_DLL_MAX_VAL) != 0)
#define D2_BIST_PASS			1
#define D2_BIST_FAIL			0

#define SET_D2_BIST_RESULT(array, dll, result)	\
	array[D2_DLL_ARRAY_IDX(dll)] = (array[D2_DLL_ARRAY_IDX(dll)] &	\
					~(0x1 << D2_DLL_BIT_IDX(dll))) |\
					((result) << D2_DLL_BIT_IDX(dll))

#define GET_D2_BIST_RESULT(array, dll)		\
	((array[D2_DLL_ARRAY_IDX(dll)] >> D2_DLL_BIT_IDX(dll)) & 0x1)

#define	GET_BITS(__src, __start_pos, __length)\
		(((__src) >> (__start_pos)) & ((1 << __length) - 1))

#define PHY_RD_VREF_DQ(x)	(get_host_vref_dq_val(x))
#define VREF_DQ_2(x)		(4500 + 65 * (x))
#define VREF_DQ_1(x)		(6000 + 65 * (x))

#define RECORD_GET(type, idx)		(reg_info[type].reg_records[idx])

#define RECORD_GET_DATA(type, idx) (reg_info[type].reg_data[dp->mss_index][idx])

struct per_data_lane_data {
	uint8_t lane_num;
	uint8_t is_x4_other;
	uint32_t tr_val;
	uint32_t right_edge;
	uint32_t left_edge;
	uint32_t bl_tap_fs;
};

struct per_bl_eye_data {
	/* Fields describing which bytelane this corresponds to. */
	uint8_t mss_num;
	uint8_t rank_num;
	uint8_t bytelane_num;
	uint8_t is_x4_other;
	/* Common data regarding read/write 2d eye. */
	uint32_t bl_iprd;
	uint32_t bl_tprd;
	uint32_t bl_tap_fs;
	/* Data specific to read/write 2d eye. */
	union {
		struct {
			uint32_t vref_tr_val;
			uint32_t wdqd_tr_val;
		} wr;
		struct {
			uint32_t vref_tr_val;
			uint32_t rdqsd_tr_val;
			uint32_t rdqsnd_tr_val;
			uint32_t dx_x_gcr5_val;
			uint32_t dx_x_gcr8_val;
		} rd;
	}d2;
};

struct per_bytelane_margin {
	uint8_t bytelane_num;
	uint8_t is_x4_other;
	uint32_t central_left_marg_fs;
	uint32_t central_right_marg_fs;
	uint32_t vref_high_marg_volt_mul100;
	uint32_t vref_low_marg_volt_mul100;
};

/*
 * These are arrays used in various debug functions here. They are not declared
 * locally as our stack size is pretty limited, instead they are stored in the
 * bss section. Only one (set of) array(s) is used at a time so we use an union
 * to share space with all the arrays.
 */
static union {
	/* Used by the DDR PHY Per-Bit-Skew Margin ops. */
	struct per_data_lane_data res_xbd[BIT_ENTRIES_NUM];
	/* Used by the DDR PHY Per-Data-Lane Centralization Margin ops. */
	struct { /* Write */
		struct per_data_lane_data res_wdqd[BYTELANE_NUM * 2];
	} dlwc;
	struct { /* Read */
		struct per_data_lane_data res_rdqsd[BYTELANE_NUM * 2];
		struct per_data_lane_data res_rdqsnd[BYTELANE_NUM * 2];
	} dlrc;
	/* Used by the DDR PHY Per-Data-Lane Data Eye Plotting ops. */
	struct {
		struct per_bl_eye_data res_vrt[BYTELANE_NUM * 2];
		struct per_bytelane_margin pbm[BYTELANE_NUM * 2];
		int res_2d[D2_VREF_MAX_VAL][D2_DLL_INT_USED];
	} dlep;
	struct {
		struct console_instance ci;
		struct ddr_params dp;
		struct ddr_params dp2;
	} dcc;
} *dbg_arrays = (void *)CMD_END;

CASSERT(sizeof(*dbg_arrays) + CMD_END < BL31_LIMIT, DBG_SIZE_TOO_BIG);

/* Global settings which may be changeable by the "settings" command. */
int eye_2d_inhvt = 0; /* Set to enable setting INHVT when do eye plotting. */
int phy_bist_cycle = 0x20;	/* The PHY BIST burst cycle. */
int phy_bist_loop = 1; /* Times phy bist is run before calling pass */
int display_eye_plot = 1; /* Whether to display the 2d eye plot result graph. */
/* The way to restore the Vref after a 2d write eye plot. */
enum { W2DR_PER_DRAM, W2DR_STATIC, W2DR_RANK_AVG} w2d_vref_rstr = W2DR_PER_DRAM;
int ddr_nolimit_flag = 0; /* Set this to remove ddr frequency limit */
int debug_cl = 0; /* Set this to be as the CL value instead of default CL */
int debug_cwl = 0; /* Set this to be as the CWL value instead of default CL */

int get_vref_index(uint32_t table_num, uint32_t input)
{
	uint32_t vref_prec, vref_idx, size;

	if (table_num == 1)
		size = 51;
	else if (table_num == 2)
		size = 51;
	else
		return -1;

	vref_prec = input;/*(input * 10000) / MEM_VREF_MAX_VAL;*/
	for (vref_idx = 0; vref_idx < size; vref_idx++) {
		if (table_num == 1) {
			if (vref_prec < VREF_DQ_1(vref_idx))
				break;
		} else {
			if (vref_prec < VREF_DQ_2(vref_idx))
				break;
		}
	}
	if (table_num == 1) {
		if ((vref_prec - VREF_DQ_1(vref_idx - 1)) <
				(VREF_DQ_1(vref_idx) - vref_prec))
			vref_idx--;
	} else {
		if ((vref_prec - VREF_DQ_2(vref_idx - 1)) <
				(VREF_DQ_2(vref_idx) - vref_prec))
			vref_idx--;
	}
	return vref_idx;
}

static int get_bdlr_bd_field(int bl_idx, int bit_idx, int nibble)
{
	PUB_DXnBDLR0_t dx_x_bdlr0;
	PUB_DXnBDLR1_t dx_x_bdlr1;
	PUB_DXnBDLR3_t dx_x_bdlr3;
	PUB_DXnBDLR4_t dx_x_bdlr4;
	int bd_field;

	switch (nibble) {
	case 0:
		dx_x_bdlr0.word = pub_read(ADDR_DXn(BDLR0, bl_idx));

		switch (bit_idx) {
		case 0:
			bd_field = dx_x_bdlr0.dq0wbd;
			break;
		case 1:
			bd_field = dx_x_bdlr0.dq1wbd;
			break;
		case 2:
			bd_field = dx_x_bdlr0.dq2wbd;
			break;
		default:
			bd_field = dx_x_bdlr0.dq3wbd;
			break;
		}
		break;
	case 1:
		dx_x_bdlr1.word = pub_read(ADDR_DXn(BDLR1, bl_idx));

		switch (bit_idx) {
		case 4:
			bd_field = dx_x_bdlr1.dq4wbd;
			break;
		case 5:
			bd_field = dx_x_bdlr1.dq5wbd;
			break;
		case 6:
			bd_field = dx_x_bdlr1.dq6wbd;
			break;
		default:
			bd_field = dx_x_bdlr1.dq7wbd;
			break;
		}
		break;
	case 3:
		dx_x_bdlr3.word = pub_read(ADDR_DXn(BDLR3, bl_idx));

		switch (bit_idx) {
		case 0:
			bd_field = dx_x_bdlr3.dq0rbd;
			break;
		case 1:
			bd_field = dx_x_bdlr3.dq1rbd;
			break;
		case 2:
			bd_field = dx_x_bdlr3.dq2rbd;
			break;
		default:
			bd_field = dx_x_bdlr3.dq3rbd;
			break;
		}
		break;
	case 4:
		dx_x_bdlr4.word = pub_read(ADDR_DXn(BDLR4, bl_idx));

		switch (bit_idx) {
		case 4:
			bd_field = dx_x_bdlr4.dq4rbd;
			break;
		case 5:
			bd_field = dx_x_bdlr4.dq5rbd;
			break;
		case 6:
			bd_field = dx_x_bdlr4.dq6rbd;
			break;
		default:
			bd_field = dx_x_bdlr4.dq7rbd;
			break;
		}
		break;
	default:
		ERROR("nibble value (%d) not supported.\n", nibble);
		bd_field = 0;
	}

	return bd_field;
}

static void set_bdlr_bd_field(int bl_idx, int bit_idx, int nibble,
			      int bd_field)
{
	PUB_DXnBDLR0_t dx_x_bdlr0;
	PUB_DXnBDLR1_t dx_x_bdlr1;
	PUB_DXnBDLR3_t dx_x_bdlr3;
	PUB_DXnBDLR4_t dx_x_bdlr4;

	switch (nibble) {
	case 0:
		dx_x_bdlr0.word = pub_read(ADDR_DXn(BDLR0, bl_idx));

		switch (bit_idx) {
		case 0:
			dx_x_bdlr0.dq0wbd = bd_field;
			break;
		case 1:
			dx_x_bdlr0.dq1wbd = bd_field;
			break;
		case 2:
			dx_x_bdlr0.dq2wbd = bd_field;
			break;
		default:
			dx_x_bdlr0.dq3wbd = bd_field;
			break;
		}
		pub_write(ADDR_DXn(BDLR0, bl_idx), dx_x_bdlr0.word);
		break;
	case 1:
		dx_x_bdlr1.word = pub_read(ADDR_DXn(BDLR1, bl_idx));

		switch (bit_idx) {
		case 4:
			dx_x_bdlr1.dq4wbd = bd_field;
			break;
		case 5:
			dx_x_bdlr1.dq5wbd = bd_field;
			break;
		case 6:
			dx_x_bdlr1.dq6wbd = bd_field;
			break;
		default:
			dx_x_bdlr1.dq7wbd = bd_field;
			break;
		}
		pub_write(ADDR_DXn(BDLR1, bl_idx), dx_x_bdlr1.word);
		break;
	case 3:
		dx_x_bdlr3.word = pub_read(ADDR_DXn(BDLR3, bl_idx));

		switch (bit_idx) {
		case 0:
			dx_x_bdlr3.dq0rbd = bd_field;
			break;
		case 1:
			dx_x_bdlr3.dq1rbd = bd_field;
			break;
		case 2:
			dx_x_bdlr3.dq2rbd = bd_field;
			break;
		default:
			dx_x_bdlr3.dq3rbd = bd_field;
			break;
		}
		pub_write(ADDR_DXn(BDLR3, bl_idx), dx_x_bdlr3.word);
		break;
	case 4:
		dx_x_bdlr4.word = pub_read(ADDR_DXn(BDLR4, bl_idx));

		switch (bit_idx) {
		case 4:
			dx_x_bdlr4.dq4rbd = bd_field;
			break;
		case 5:
			dx_x_bdlr4.dq5rbd = bd_field;
			break;
		case 6:
			dx_x_bdlr4.dq6rbd = bd_field;
			break;
		default:
			dx_x_bdlr4.dq7rbd = bd_field;
			break;
		}
		pub_write(ADDR_DXn(BDLR4, bl_idx), dx_x_bdlr4.word);
		break;
	default:
		ERROR("nibble value (%d) not supported.\n", nibble);
	}
}

static void phy_bist_setup(void)
{
	PUB_BISTRR_t bistrr;
	PUB_BISTAR1_t bistar1;
	PUB_BISTAR2_t bistar2;

	bistrr.word = pub_read(PUB_BISTRR);
	bistrr.bmode = 0x1;
	bistrr.bdpat = 0xf;
	bistrr.bdxen = 0x1;
	pub_write(PUB_BISTRR, bistrr.word);

	SET_MEM_REG_FIELD(pub, PUB_BISTWCR, bwcnt, phy_bist_cycle);

	bistar1.word = pub_read(PUB_BISTAR1);
	bistar1.bainc = 0x8;
	pub_write(PUB_BISTAR1, bistar1.word);

	bistar2.word = pub_read(PUB_BISTAR2);
	bistar2.bmbank = dp->ddr_max_bank;
	bistar2.bmcol = dp->ddr_max_col;
	pub_write(PUB_BISTAR2, bistar2.word);

	SET_MEM_REG_FIELD(pub, PUB_BISTAR4, bmrow, dp->ddr_max_row);
}
/*
 * Reads the argument and parse which DDR it selects, return -1
 * if the given argument is not entered correctly.
 */
static int get_entered_mss_map(char * const arg)
{
	int mss_map;

	if (!strcmp("all", arg) || !strcmp("ALL", arg))
		mss_map = (1 << MAX_MEM_CTRL) - 1;
	else if (!strncmp("DDR", arg, 3) &&
		 arg[3] >= '0' && arg[3] < '0' + MAX_MEM_CTRL && arg[4] == 0)
		mss_map = 1 << (arg[3] - '0');
	else {
		tf_printf("DDR<n> can only be ALL or DDR<n>\n");
		tf_printf("Where 0 <= n < %d\n", MAX_MEM_CTRL);
		return -1;
	}
	return mss_map;
}

/*
 * Change the stored DDR params according to the desired freqeuncy.
 * Call do_configure_emem() after this to set up the memory according to the new
 * frequency.
 */
int ddr_operating_frequency_change(int argc, char * const argv[])
{
	unsigned int mss_map, freq_khz;
	int dot_place = -1;
	uint64_t tck;

	if (argc != 3) {
		tf_printf("Unsupported argument number. Should be:\n");
		tf_printf("%s DDR<n> XXX.YYY\n", __func__);
		tf_printf("DDR<n> is the frequency for which to change.\n");
		tf_printf("XXX.YYY is the frequency to change to (MHz)\n");
		return -1;
	}

	/* See the DDR on which MSS we are going to change the frequency of. */
	mss_map = get_entered_mss_map(argv[1]);
	if (mss_map == -1)
		return -1;

	/* Parse the frequency that we are going to change to. */
	for (int i = 0; i < strlen(argv[2]); i++) {
		if (argv[2][i] == '.') {
			if (dot_place == -1)
				dot_place = i;
			else {
				tf_printf("Invalid frequency.\n");
				return -1;
			}
		} else if (argv[2][i] < '0' || argv[2][i] > '9') {
			tf_printf("Invalid frequency.\n");
			return -1;
		}
	}
	if (dot_place >= 0 && strlen(argv[2]) - dot_place - 1 > 3) {
		tf_printf("Only support max 3 digits after period.\n");
		return -1;
	}
	freq_khz = 0;
	if (dot_place != -1) {
		int append_zeros = 3 - (strlen(argv[2]) - dot_place - 1);

		freq_khz = strtol(argv[2] + dot_place + 1, NULL, 10);
		while (append_zeros > 0) {
			freq_khz *= 10;
			append_zeros--;
		}
	}
	if (dot_place != 0)
		freq_khz += 1000 * strtol(argv[2], NULL, 10);

	if (freq_khz == 0) {
		tf_printf("Does not support 0 MHz frequency.\n");
		return -1;
	}
	/* Convert the frequency to tcks in femtoseconds. */
	tck = FS_PER_MS / freq_khz;

	tf_printf("Set frequency to %d kHz, which is %lld fs,\n", freq_khz, tck);
	/* Update the actual frequencies. */
	for (int i = 0; i < MAX_MEM_CTRL; i++) {
		uint64_t prev_tck;

		if (!(mss_map & (1 << i)))
			continue;

		if (!ddr_switch_current_mss(i))
			continue;

		prev_tck = dp->tck;
		dp->tck = tck;
		/* Also update any parameters using the tck. */
		if (!ddr_get_info_rest(dp)) {
			tf_printf("Entered frequency not supported.\n");
			tf_printf("Using previous one of %lld fs.\n", prev_tck);
			dp->tck = prev_tck;
			ddr_get_info_rest(dp);
			return -1;
		}
	}
	return 0;
}

/* Rerun the memory configuration setup. */
int do_configure_emem(int argc, char * const argv[])
{
	unsigned int mss_map;

	if (argc != 2) {
		tf_printf("Unsupported argument number. Should be:\n");
		tf_printf("%s DDR<n>\n", __func__);
		tf_printf("DDR<n> is the device for which to rerun config.\n");
		return -1;
	}

	mss_map = get_entered_mss_map(argv[1]);
	if (mss_map == -1)
		return -1;
	for (int i = 0; i < MAX_MEM_CTRL; i++) {
		if (!(mss_map & (1 << i)))
			continue;
		if (!ddr_switch_current_mss(i))
			continue;
		ddr_do_actual_setup();
	}
	return 0;
}

/*
 * Find the register by name/address.
 * If found, return the index of the register in the array, else return -1.
 */
int find_record(int reg_type, int addr_mode, int address, char *name)
{
	int match = 0;

	for (int idx = 0; idx < reg_info[reg_type].reg_num; idx++) {
		if (addr_mode)
			match = (RECORD_GET(reg_type, idx).address == address);
		else
			match = !strcmp(name, RECORD_GET(reg_type, idx).name);

		if (match) {
			VERBOSE("TEMP PRINT: FOUND INDEX = %d\n", idx);
			return idx;
		}
	}

	return -1;
}

/******************************************************/
/* Core register access functions used.               */
/******************************************************/
static void reg_record(int reg_type, int reg_idx,
		       __attribute__((unused)) int val)
{
	int offset = 0;
	if (reg_type == PUB_RANK_REG)
		offset= GET_MEM_REG_FIELD(pub, PUB_RANKIDR, rankrid) *
			   reg_info[reg_type].reg_num;
	RECORD_GET_DATA(reg_type, reg_idx + offset) =
	  reg_info[reg_type].reg_read(RECORD_GET(reg_type, reg_idx).address);

#if DEBUG
	tf_printf("Debug print: RECORD info:\n");
	print_reg(RECORD_GET(reg_type, reg_idx).name,
		  RECORD_GET(reg_type, reg_idx).address,
		  RECORD_GET_DATA(reg_type, reg_idx + offset));
#endif
}

static void reg_restore(int reg_type, int reg_idx,
			__attribute__((unused)) int val)
{
	uint32_t write_val;
	int offset = 0;

	if (reg_type == PUB_RANK_REG)
		offset += GET_MEM_REG_FIELD(pub, PUB_RANKIDR, rankwid) *
			   reg_info[reg_type].reg_num;
	/* Prohibit restore for PUB Registers: PIR, SCHCR0 & BISTRR. */
	if (reg_type == PUB_REG &&
	    ((RECORD_GET(reg_type, reg_idx).address == PUB_PIR) ||
	    (RECORD_GET(reg_type, reg_idx).address == PUB_SCHCR0) ||
	    (RECORD_GET(reg_type, reg_idx).address == PUB_BISTRR))) {
#if DEBUG
		tf_printf("Debug print: Skip %s reg restore\n",
			  RECORD_GET(reg_type, reg_idx).name);
#endif
		return;
	}

	write_val = RECORD_GET_DATA(reg_type, reg_idx + offset);
	reg_info[reg_type].reg_write(RECORD_GET(reg_type, reg_idx).address,
				     write_val);
#if DEBUG
	tf_printf("Debug print: RESTORE info:\n");
	print_reg(RECORD_GET(reg_type, reg_idx).name,
			RECORD_GET(reg_type, reg_idx).address, write_val);
#endif
}

static void reg_write(int reg_type, int reg_idx, int write_val)
{
	reg_info[reg_type].reg_write(RECORD_GET(reg_type, reg_idx).address,
				     write_val);
#if DEBUG
	tf_printf("Debug print: WRITE info:\n");
	print_reg(RECORD_GET(reg_type, reg_idx).name,
		  RECORD_GET(reg_type, reg_idx).address, write_val);
#endif
}

static void reg_read(int reg_type, int reg_idx, __attribute__((unused)) int val)
{
	uint32_t read_val;

	read_val =
	  reg_info[reg_type].reg_read(RECORD_GET(reg_type, reg_idx).address);
	print_reg(RECORD_GET(reg_type, reg_idx).name,
		  RECORD_GET(reg_type, reg_idx).address, read_val);
}

/*
 * This command is in charge of basic memory register access. It should be
 * called by one of it's alias names to do the corresponding action.
 */
int do_ddr_basic(int argc, char * const argv[])
{
	unsigned int mss_map;
	int all = 0;
	int reg_type;
	void (*op)(int, int, int);
	const char *cmd;

	cmd = argv[0];

	/* Note that we compare pub_indirect before pub here. */
	if (!strncmp(cmd, "pub_indirect", strlen("pub_indirect"))) {
		cmd += strlen("pub_indirect");
		reg_type = PUB_INDIRECT_REG;
	} else if (!strncmp(cmd, "pub", strlen("pub"))) {
		cmd += strlen("pub");
		reg_type = PUB_REG;
	} else if (!strncmp(cmd, "emc", strlen("emc"))) {
		cmd += strlen("emc");
		reg_type = EMC_REG;
	} else if (!strncmp(cmd, "emi", strlen("emi"))) {
		cmd += strlen("emi");
		reg_type = EMI_REG;
	} else {
		tf_printf("Invalid register type to access.\n");
		return -1;
	}

	if (*cmd != '_') {
		tf_printf("Invalid command %s\n", argv[0]);
		return -1;
	}

	cmd++;

	if (!strcmp("record", cmd)) {
		op = reg_record;
	} else if (!strcmp("restore", cmd)) {
		op = reg_restore;
	} else if (!strcmp("dump", cmd)) {
		/* We treat dump as read with "ALL" option. */
		op = reg_read;
		all = 1;
	} else if (!strcmp("write", cmd)) {
		op = reg_write;
	} else if (!strcmp("read", cmd)) {
		op = reg_read;
	} else if (!strcmp("info", cmd)) {
		tf_printf("%s_NUM = %d\n", reg_info[reg_type].prefix,
					   reg_info[reg_type].reg_num);
		for (int i = 0; i < reg_info[reg_type].reg_num; i++)
			tf_printf("0x%x\t%s\n",
				reg_info[reg_type].reg_records[i].address,
				reg_info[reg_type].reg_records[i].name);
		return 0;
	} else {
		tf_printf("Invalid cmd %s, should be one of:\n", argv[0]);
		tf_printf("<record/restore/dump/write/read>\n");
		return -1;
	}

	if (argc < 2) {
		tf_printf("Please specify the DDR<n> to do operation on!\n");
		tf_printf("%s DDR<n>\n", argv[0]);
		return -1;
	}

	mss_map = get_entered_mss_map(argv[1]);
	if (mss_map == -1)
		return -1;

	if (!all) {
		if (argc < 3) {
			tf_printf("Please enter the register to use.\n");
			tf_printf("Register can be input by name(UPPERCASE)\n");
			tf_printf("or by address (starting with 0x in hex)\n");
			return -1;
		}
		all = (!strcmp(argv[2], "ALL")) || (!strcmp(argv[2], "all"));
	}

	if (all && op == reg_write) {
		tf_printf("%s does not support \"ALL\" option!\n", argv[0]);
		return -1;
	}

	for (int i = 0; i < MAX_MEM_CTRL; i++) {
		if (!(mss_map & (1 << i)))
			continue;
		if (!ddr_switch_current_mss(i))
			continue;

		if (all) {
			for (int j = 0; j < reg_info[reg_type].reg_num; j++)
				op(reg_type, j, 0);

			/*
			 * The per rank PUB registers need to modify the RANKIDR
			 * PUB register in order to be correctly accessed.
			 */
			if (reg_type == PUB_REG) {
				/* Record the RANKIDR original value. */
				uint32_t rankidr_orig = pub_read(PUB_RANKIDR);

				reg_type = PUB_RANK_REG;
				for (int r = 0; r < MAX_ACTIVE_RANKS; r++) {
					if ((dp->active_ranks & (1 << r)) == 0)
						continue;
					if (op == reg_read)
						tf_printf("Rank%d:\n", r);
					pub_switch_rankidr(r, 1, 1);
					for (int j = 0; j < reg_info[reg_type].
								reg_num; j++)
						op(reg_type, j, 0);
				}
				reg_type = PUB_REG;

				/* Restore the RANKIDR original value. */
				pub_write(PUB_RANKIDR, rankidr_orig);
			}
			continue;
		}

		/*
		 * If not all the registers, we read the arguments one by one
		 * and do the operation.
		 */
		for (int j = 2; j < argc; j++) {
			int mode, reg_idx;
			uint32_t reg_addr = -1;
			char *reg_name = NULL;
			int val = 0;

			mode = (!memcmp("0x", argv[j], 2) ||
					!memcmp("0X", argv[j], 2));

			VERBOSE("address mode = %d\n", mode);

			if (mode)
				reg_addr = (uint32_t)strtol(argv[j], NULL, 16);
			else
				reg_name = argv[j];

			reg_idx = find_record(reg_type, mode, reg_addr,
					      reg_name);

			/*
			 * If we don't find it in the normal PUB registers,
			 * it might be in the rank specific PUB registers.
			 */
			if (reg_idx == -1 && reg_type == PUB_REG) {
				reg_type = PUB_RANK_REG;
				reg_idx = find_record(reg_type, mode, reg_addr,
						      reg_name);
			}

			VERBOSE("reg_idx = %d\n", reg_idx);
			if (reg_idx == -1) {
				ERROR("Invalid register %s\n",
					(mode ? "address" : "name"));
				return -1;
			}

			/* We also need to parse the value to write. */
			if (op == reg_write) {
				j++;
				if (j >= argc) {
					tf_printf("Need reg write value!\n");
					return -1;
				}
				val = (uint32_t)strtol(argv[j], NULL, 16);
			}

			/* Do the actual operation. */
			op(reg_type, reg_idx, val);

			if (reg_type == PUB_RANK_REG)
				reg_type = PUB_REG;
		}
	}

	return 0;
}

/*
 * This commands sets up the specified dto signal to the output pad for probing.
 */
int do_dto_probing(int argc, char * const argv[])
{
	unsigned int mss_map;
	uint32_t dto_msel, dto_isel;

	if (argc != 4) {
		tf_printf("Unsupported argument number. Should be:\n");
		tf_printf("%s DDR<n> <DTO_MSEL> <DTO_ISEL>\n", __func__);
		tf_printf("DDR<n> is the device for which to probe.\n");
		tf_printf("<DTO_MSEL> 5 bit code for the DDR PHY Macro.\n");
		tf_printf("<DTO_ISEL> 4 bit code for the internal signal.\n");
		return -1;
	}

	mss_map = get_entered_mss_map(argv[1]);
	if (mss_map == -1)
		return -1;

	dto_msel = (uint32_t)strtol(argv[2], NULL, 16);
	if (dto_msel > 0x1f) {
		tf_printf("Entered dto_msel val (0x%x) have more than 5 bits\n",
			   dto_msel);
		return -1;
	}

	dto_isel = (uint32_t)strtol(argv[3], NULL, 16);
	if (dto_isel > 0xf) {
		tf_printf("Entered dto_isel val (0x%x) have more than 4 bits\n",
			   dto_isel);
		return -1;
	}

	for (int i = 0; i < MAX_MEM_CTRL; i++) {
		PUB_PGCR0_t pgcr0;
		PUB_PGCR1_t pgcr1;
		PUB_PGCR7_t pgcr7;
		PUB_PLLCR_t pllcr;
		PUB_DSGCR_t dsgcr;

		if (!(mss_map & (1 << i)))
			continue;
		if (!ddr_switch_current_mss(i))
			continue;

		/* ===== 1 ===== */
		pgcr0.word = pub_read(PUB_PGCR0);
		pgcr0.dtosel = dto_msel;
		pub_write(PUB_PGCR0, pgcr0.word);
		pgcr1.word = pub_read(PUB_PGCR1);
		pgcr1.dtomode = GET_BIT(dto_isel, 3);
		pub_write(PUB_PGCR1, pgcr1.word);
		pgcr7.word = pub_read(PUB_PGCR7);
		pgcr7.acdtosel = GET_BIT(dto_isel, 0);
		pgcr7.dxdtosel = GET_BITS(dto_isel, 0, 2);
		pub_write(PUB_PGCR7, pgcr7.word);
		pllcr.word = pub_read(PUB_PLLCR);
		pllcr.dtc = GET_BITS(dto_isel, 0, 3);
		pub_write(PUB_PLLCR, pllcr.word);
		/* ===== 2 ===== */
		dsgcr.word = pub_read(PUB_DSGCR);
		dsgcr.dtopdr = 0; // The original field dtopdd was not found
		dsgcr.dtooe = 1;
		pub_write(PUB_DSGCR, dsgcr.word);

		tf_printf("The DTO[1:0] output PADs are ready for probing.\n");
	}

	return 0;
}

/*
 * This commands sets up the specified ato signal to the output pad for probing.
 */
int do_ato_probing(int argc, char * const argv[])
{
	unsigned int mss_map;
	uint32_t ato_msel, ato_isel;

	if (argc != 4) {
		tf_printf("Unsupported argument number. Should be:\n");
		tf_printf("%s DDR<n> <ATO_MSEL> <ATO_ISEL>\n", __func__);
		tf_printf("DDR<n> is the device for which to probe.\n");
		tf_printf("<ATO_MSEL> 4 bit code for the DDR PHY Macro.\n");
		tf_printf("<ATO_ISEL> 4 bit code for the internal signal.\n");
		return -1;
	}

	mss_map = get_entered_mss_map(argv[1]);
	if (mss_map == -1)
		return -1;

	ato_msel = (uint32_t)strtol(argv[2], NULL, 16);
	if (ato_msel > 0xf) {
		tf_printf("Entered ato_msel val (0x%x) have more than 4 bits\n",
			   ato_msel);
		return -1;
	}

	ato_isel = (uint32_t)strtol(argv[3], NULL, 16);
	if (ato_isel > 0xf) {
		tf_printf("Entered ato_isel val (0x%x) have more than 4 bits\n",
			   ato_isel);
		return -1;
	}

	if (ato_isel == 0 || ato_isel == 0x6 || ato_isel >= 0xa) {
		tf_printf("Forbidden value for ato_isel 0x%x, abort.\n",
			   ato_isel);
		return -1;
	}

	for (int i = 0; i < MAX_MEM_CTRL; i++) {
		PUB_PLLCR_t pllcr;
		PUB_DSGCR_t dsgcr;

		if (!(mss_map & (1 << i)))
			continue;
		if (!ddr_switch_current_mss(i))
			continue;

		pllcr.word = pub_read(PUB_PLLCR);
		pllcr.atc = ato_isel;
		pllcr.atoen = ato_msel;
		pub_write(PUB_PLLCR, pllcr.word);

		dsgcr.word = pub_read(PUB_DSGCR);
		dsgcr.atoae = 1;
		pub_write(PUB_DSGCR, dsgcr.word);

		tf_printf("The ATO output PAD is ready for probing.\n");
	}
	return 0;
}

/*
 * This commands sets up the specified Vref signal to the output pad for probing.
 */
int do_vref_probing(int argc, char * const argv[])
{
	enum {VREF_PROBE_AC, VREF_PROBE_ZQ, VREF_PROBE_DQ};

	int mss_idx, vref_module, rank_idx, bl_idx;
	int nb_idx = 0;
	int parsed;
	uint32_t tmp;

	PUB_DXnGCR5_t gcr5;
	PUB_DXnGCR8_t gcr8;

	parsed = (argc >= 3) &&
	  get_arg_str(argv[1], &mss_idx, "DDR<n>", 2, "DDR0", 0, "DDR1", 1) &&
	  get_arg_str(argv[2], &vref_module, "vref_module", 3,
			"AC", VREF_PROBE_AC,
			"ZQ", VREF_PROBE_ZQ,
			"DQ", VREF_PROBE_DQ);

	if (parsed && vref_module == VREF_PROBE_DQ)
		parsed = (argc >= 5) &&
		  get_arg_int(argv[3], &rank_idx, 0, 0, 0, "rank_index") &&
		  get_arg_int(argv[4], &bl_idx, 1, 0, BYTELANE_NUM, "bl") &&
		  (argc < 6 || get_arg_int(argv[5], &nb_idx, 1, 0, 2, "nb"));

	if (!parsed) {
		tf_printf("Unsupported argument number. Should be:\n");
		tf_printf("%s DDR<n> AC/ZQ\n", __func__);
		tf_printf("%s DDR<n> DQ <rank> <bl_index> [<nb_index>]\n",
			  __func__);
		return -1;
	}
	if (!ddr_switch_current_mss(mss_idx)) {
		tf_printf("MSS%d not configured!\n", mss_idx);
		return -1;
	}

	if (vref_module == VREF_PROBE_DQ) {
		if ((dp->active_ranks & (1 << rank_idx)) == 0) {
			tf_printf("Entered rank not valid present in MSS.\n");
			return -1;
		}

		if (dp->package != PACKAGE_x4 && nb_idx == 1) {
			tf_printf("nb = 1 only supported on x4 config.\n");
			return -1;
		}
	}

	/* Actual operations start here. */
	SET_MEM_REG_FIELD(pub, PUB_IOVCR0, acvrefpen, 0x0);
	SET_MEM_REG_FIELD(pub, PUB_IOVCR1, zqvrefpen, 0x0);
	for (int byte = 0; byte < BYTELANE_NUM; byte++) {
		SET_PUB_REG_FIELD_DXn(GCR4, byte, dxrefpen, 0);
	}

	if (vref_module == VREF_PROBE_AC) {
		SET_MEM_REG_FIELD(pub, PUB_IOVCR0, acvrefpen, 0x1);
		return 0;
	}

	if (vref_module == VREF_PROBE_ZQ) {
		SET_MEM_REG_FIELD(pub, PUB_IOVCR1, zqvrefpen, 0x1);
		return 0;
	}

	/* vref_module == VREF_PROBE_DQ */
	gcr5.word = pub_read(ADDR_DXn(GCR5, bl_idx));
	gcr8.word = pub_read(ADDR_DXn(GCR8, bl_idx));

	if (nb_idx == 0) {
		tmp = GET_RANK_FIELD(gcr5, dxrefiselr, rank_idx);
		SET_RANK_FIELD(gcr8, x4dxrefiselr, rank_idx, tmp);
		pub_write(ADDR_DXn(GCR8, bl_idx), gcr8.word);
	} else {
		tmp = GET_RANK_FIELD(gcr8, x4dxrefiselr, rank_idx);
		SET_RANK_FIELD(gcr5, dxrefiselr, rank_idx, tmp);
		pub_write(ADDR_DXn(GCR5, bl_idx), gcr5.word);
	}
	SET_PUB_REG_FIELD_DXn(GCR4, bl_idx, dxrefimon, rank_idx);
	SET_PUB_REG_FIELD_DXn(GCR4, bl_idx, dxrefpen, 0x1);

	return 0;
}

/*
 * Get the TAP delay value entered. Also check if the entered value is followed
 * by "ps" to indicate that the entered value is in picoseconds and not in raw
 * TAPs.
 * Return 1 if successful or 0 if error.
 */
static int get_tap_delay(char *const arg, int *tap, int *is_ps)
{
	char *end;
	long value = strtol(arg, &end, 0);

	*is_ps = 0;

	/* Check if the value is followed by "ps" */
	if (*end != 0) {
		if (!strcmp("ps", end))
			*is_ps = 1;
		else {
			tf_printf("\"%s\" is not a valid TAP value.\n", arg);
			return 0;
		}
	}

	*tap = value;
	return 1;
}

/* Convert a TAP delay from PS to raw TAPs. */
static inline int convert_tap(int tap_delay, int is_ps, int bl_idx)
{
	int tap_fs_delay;
	int is_neg;
	int bl_iprd;
	PUB_DXnMDLR0_t dx_x_mdlr0;

	if (!is_ps)
		return tap_delay;

	dx_x_mdlr0.word = pub_read(ADDR_DXn(MDLR0, bl_idx));
	bl_iprd = dx_x_mdlr0.iprd;

	if (tap_delay < 0) {
		is_neg = 1;
		tap_delay = -tap_delay;
	} else
		is_neg = 0;

	tap_fs_delay = dp->tck / (2 * bl_iprd);
	tap_delay = (tap_delay * 1000 + tap_fs_delay / 2) / tap_fs_delay;
	tap_delay = is_neg ? -tap_delay : tap_delay;

	tf_printf("Using closest TAP of %d, which is approx %dps\n",
		   tap_delay, tap_delay * tap_fs_delay / 1000);

	return tap_delay;
}

/* Modify a Single Byte Lane Read/Write Centralization TAP. */
int edit_xctap(int argc, char * const argv[])
{
	int parsed;
	int rank_idx, bl_idx, nb_idx, tap_delay, is_ps;
	int tr_res, bl_iprd;
	int mss_idx;
	int status = 0;
	int is_write;

	uint32_t reg_data; /* The initial MC_DDR_IF reg data. */

	PUB_DXnMDLR0_t dx_x_mdlr0;

	parsed = (argc == 6) &&
	  get_arg_str(argv[0], &is_write, "Command", 2,
		"edit_wctap", 1, "edit_rctap", 0) &&
	  get_arg_str(argv[1], &mss_idx, "DDR<n>", 2, "DDR0", 0, "DDR1", 1) &&
	  get_arg_int(argv[2], &rank_idx, 0, 0, 0, "rank_index") &&
	  get_arg_int(argv[3], &bl_idx, 1, 0, BYTELANE_NUM, "bl_index") &&
	  get_arg_int(argv[4], &nb_idx, 1, 0, 2, "nb_index") &&
	  get_tap_delay(argv[5], &tap_delay, &is_ps);

	if (!parsed) {
		tf_printf("Invalid arguments, should be:\n");
		tf_printf("%s DDR<n> <rank_index> <bl_index> <nb_index>"
			  " <tap_delay>(ps)\n", argv[0]);
		return -1;
	}

	/* Switch the operating MSS to the specified one. */
	if (!ddr_switch_current_mss(mss_idx)) {
		tf_printf("Entered MSS not present or has no DIMMs present.\n");
		return -1;
	}

	if ((dp->active_ranks & (1 << rank_idx)) == 0) {
		tf_printf("Entered rank not valid present in MSS.\n");
		return -1;
	}

	/* Actual operation starts here. */
	reg_data = emc_read(EMC_MC_DDR_IF);
	emc_write(EMC_MC_DDR_IF, 0x0);
	pub_switch_rankidr(rank_idx, 1, 1);

	dx_x_mdlr0.word = pub_read(ADDR_DXn(MDLR0, bl_idx));
	bl_iprd = dx_x_mdlr0.iprd;

	tap_delay = convert_tap(tap_delay, is_ps, bl_idx);

#define	MOD_LCDLRx_FIELD(LCDLRx, field)		do {			\
	PUB_DXn##LCDLRx##_t lcdlrx = {					\
		.word = pub_read(ADDR_DXn(LCDLRx, bl_idx)),		\
	};								\
									\
	if (nb_idx == 0)						\
		tr_res = lcdlrx.field;					\
	else								\
		tr_res = lcdlrx.x4##field;				\
									\
	if ((tr_res + tap_delay) > bl_iprd) {				\
		tf_printf("WARNING: ((x4)" #field " %d + tap_delay %d) "\
			"> bl_iprd %d\n", tr_res, tap_delay, bl_iprd);	\
		tr_res = bl_iprd;					\
		status = WARNING_RET_VAL;				\
									\
	} else if (tr_res + tap_delay < 0) {				\
		tf_printf("WARNING: ((x4)" #field " %d + tap_delay %d) "\
			"< 0\n", tr_res, tap_delay);			\
		tr_res = 0;						\
		status = WARNING_RET_VAL;				\
	} else								\
		tr_res = tr_res + tap_delay;				\
									\
	if (nb_idx == 0)						\
		lcdlrx.field = tr_res;					\
	else								\
		lcdlrx.x4##field = tr_res;				\
									\
	pub_write(ADDR_DXn(LCDLRx, bl_idx), lcdlrx.word);		\
} while (0)


	if (is_write) { /* Modify Single Data Lane Write Centralization TAP. */
		MOD_LCDLRx_FIELD(LCDLR1, wdqd);
	} else { /* Modify Single Data Lane Read Centralization TAP. */
		MOD_LCDLRx_FIELD(LCDLR3, rdqsd);
		MOD_LCDLRx_FIELD(LCDLR4, rdqsnd);
	}

	pub_switch_rankidr(0, 1, 1);
	emc_write(EMC_MC_DDR_IF, reg_data);

	return status;
}

/* Modify a Single DQ Read/Write Delay TAP. */
int edit_xdtap(int argc, char * const argv[])
{
	int parsed;
	int bit_idx, bl_idx, tap_delay, is_ps;
	int tr_res, nibble;
	int mss_idx;
	int status = 0;
	int is_write;

	uint32_t reg_data; /* The initial MC_DDR_IF reg data. */

	parsed = (argc == 5) &&
	  get_arg_str(argv[0], &is_write, "Command", 2,
		"edit_wdtap", 1, "edit_rdtap", 0) &&
	  get_arg_str(argv[1], &mss_idx, "DDR<n>", 2, "DDR0", 0, "DDR1", 1) &&
	  get_arg_int(argv[2], &bl_idx, 1, 0, BYTELANE_NUM, "bl_index") &&
	  get_arg_int(argv[3], &bit_idx, 1, 0, 8, "bit_index") &&
	  get_tap_delay(argv[4], &tap_delay, &is_ps);

	if (!parsed) {
		tf_printf("Invalid arguments, should be:\n");
		tf_printf("%s DDR<n> <bl_index> <bit_index> <tap_index>(ps)\n",
			  argv[0]);
		return -1;
	}

	/* Switch the operating MSS to the specified one. */
	if (!ddr_switch_current_mss(mss_idx)) {
		tf_printf("Entered MSS not present or has no DIMMs present.\n");
		return -1;
	}

	/* Actual operation starts here. */
	reg_data = emc_read(EMC_MC_DDR_IF);
	emc_write(EMC_MC_DDR_IF, 0x0);

	tap_delay = convert_tap(tap_delay, is_ps, bl_idx);

	if (is_write)
		nibble = (bit_idx / 4); /* can be 0 or 1 */
	else
		nibble = 3 + (bit_idx / 4); /* can be 3 or 4 */

	tr_res = get_bdlr_bd_field(bl_idx, bit_idx, nibble);

	if ((tr_res + tap_delay) > 0x3F) {
		tf_printf("(WARNING: tr_res %d + tap_delay %d) > 0x3F\n",
			  tr_res, tap_delay);
		tr_res = 0x3F;
		status = WARNING_RET_VAL;

	} else if ((tr_res + tap_delay) < 0) {
		tf_printf("WARNING: (tr_res %d + tap_delay %d) < 0\n",
			  tr_res, tap_delay);
		tr_res = 0;
		status = WARNING_RET_VAL;

	} else
		tr_res = tr_res + tap_delay;

	set_bdlr_bd_field(bl_idx, bit_idx, nibble, tr_res);

	emc_write(EMC_MC_DDR_IF, reg_data);

	return status;
}

/* Modify a Single Data Lane DQS Write Leveling. */
int edit_wltap(int argc, char * const argv[])
{
	int parsed;
	int rank_idx, bl_idx, nb_idx, tap_delay, is_ps;
	int tr_res, wlsl, bl_iprd;
	int mss_idx;
	int status = 0;
	int is_write;

	uint32_t reg_data; /* The initial MC_DDR_IF reg data. */
	PUB_DXnLCDLR0_t dx_x_lcdlr0;
	PUB_DXnMDLR0_t dx_x_mdlr0;
	PUB_DXnGTR0_t dx_x_gtr0;

	parsed = (argc == 6) &&
	  get_arg_str(argv[0], &is_write, "Command", 1, "edit_wltap", 1) &&
	  get_arg_str(argv[1], &mss_idx, "DDR<n>", 2, "DDR0", 0, "DDR1", 1) &&
	  get_arg_int(argv[2], &rank_idx, 0, 0, 0, "rank_index") &&
	  get_arg_int(argv[3], &bl_idx, 1, 0, BYTELANE_NUM, "bl_index") &&
	  get_arg_int(argv[4], &nb_idx, 1, 0, 2, "nb_index") &&
	  get_tap_delay(argv[5], &tap_delay, &is_ps);

	if (!parsed) {
		tf_printf("Invalid arguments, should be:\n");
		tf_printf("%s DDR<n> <rank_index> <bl_index> <nb_index>"
			  " <tap_delay>(ps)\n", argv[0]);
		return -1;
	}

	/* Switch the operating MSS to the specified one. */
	if (!ddr_switch_current_mss(mss_idx)) {
		tf_printf("Entered MSS not present or has no DIMMs present.\n");
		return -1;
	}

	/* Actual operation starts here. */
	reg_data = emc_read(EMC_MC_DDR_IF);
	emc_write(EMC_MC_DDR_IF, 0x0);
	pub_switch_rankidr(rank_idx, 1, 1);

	dx_x_mdlr0.word = pub_read(ADDR_DXn(MDLR0, bl_idx));
	bl_iprd = dx_x_mdlr0.iprd;

	tap_delay = convert_tap(tap_delay, is_ps, bl_idx);

	dx_x_lcdlr0.word = pub_read(ADDR_DXn(LCDLR0, bl_idx));
	dx_x_gtr0.word = pub_read(ADDR_DXn(GTR0, bl_idx));

	if (nb_idx == 0) {
		tr_res = dx_x_lcdlr0.wld;
		wlsl = dx_x_gtr0.wlsl;
	} else {
		tr_res = dx_x_lcdlr0.x4wld;
		wlsl = dx_x_gtr0.x4wlsl;
	}

	if ((tr_res + tap_delay) > 0x1FF) {
		tf_printf("(WARNING: tr_res %d + tap_delay %d) > 0x1FF\n",
			  tr_res, tap_delay);
		tr_res = 0x1FF;
		status = WARNING_RET_VAL;

	} else if ((tr_res + tap_delay) < 0) {
		if (wlsl > 0) {
			wlsl--;
			tr_res += bl_iprd;
		}
		if ((tr_res + tap_delay) < 0) {
			tf_printf("WARNING: (tr_res %d + tap_delay %d) < 0\n",
				  tr_res, tap_delay);
			tr_res = 0;
			status = WARNING_RET_VAL;
		} else
			tr_res = tr_res + tap_delay;
	} else
		tr_res = tr_res + tap_delay;

	if (nb_idx == 0) {
		if (dx_x_gtr0.wlsl != wlsl) {
			dx_x_gtr0.wlsl = wlsl;
			pub_write(ADDR_DXn(GTR0, bl_idx), dx_x_gtr0.word);
		}
		dx_x_lcdlr0.wld = tr_res;
	} else {
		if (dx_x_gtr0.x4wlsl != wlsl) {
			dx_x_gtr0.x4wlsl = wlsl;
			pub_write(ADDR_DXn(GTR0, bl_idx), dx_x_gtr0.word);
		}
		dx_x_lcdlr0.x4wld = tr_res;
	}

	pub_write(ADDR_DXn(LCDLR0, bl_idx), dx_x_lcdlr0.word);

	pub_switch_rankidr(rank_idx, 0, 0);
	emc_write(EMC_MC_DDR_IF, reg_data);

	return status;
}

/* Shift the output enable timing. */
int edit_wetap(int argc, char * const argv[])
{
	int parsed;
	int nb_idx, bl_idx, tap_delay, is_ps;
	int tr_res;
	int mss_idx;
	int status = 0;
	int is_write;

	uint32_t reg_data; /* The initial MC_DDR_IF reg data. */
	uint32_t bdlrx;

	parsed = (argc == 5) &&
	  get_arg_str(argv[0], &is_write, "Command", 1, "edit_wetap", 1) &&
	  get_arg_str(argv[1], &mss_idx, "DDR<n>", 2, "DDR0", 0, "DDR1", 1) &&
	  get_arg_int(argv[2], &bl_idx, 1, 0, BYTELANE_NUM, "bl_index") &&
	  get_arg_int(argv[3], &nb_idx, 1, 0, 2, "nb_index") &&
	  get_tap_delay(argv[4], &tap_delay, &is_ps);

	if (!parsed) {
		tf_printf("Invalid arguments, should be:\n");
		tf_printf("%s DDR<n> <bl_index> <nibble> <tap_index>(ps)\n",
			  argv[0]);
		return -1;
	}

	/* Switch the operating MSS to the specified one. */
	if (!ddr_switch_current_mss(mss_idx)) {
		tf_printf("Entered MSS not present or has no DIMMs present.\n");
		return -1;
	}

	/* Actual operation starts here. */
	reg_data = emc_read(EMC_MC_DDR_IF);
	emc_write(EMC_MC_DDR_IF, 0x0);

	tap_delay = convert_tap(tap_delay, is_ps, bl_idx);

	if (nb_idx == 0) {
		bdlrx = pub_read(ADDR_DXn(BDLR2, bl_idx));
		tr_res = ((PUB_DXnBDLR2_t *)&bdlrx)->dsoebd;
	} else {
		bdlrx = pub_read(ADDR_DXn(BDLR7, bl_idx));
		tr_res = ((PUB_DXnBDLR7_t *)&bdlrx)->x4dsoebd;
	}

	if ((tr_res + tap_delay) > 0x3F) {
		tf_printf("(WARNING: tr_res %d + tap_delay %d) > 0x3F\n",
			  tr_res, tap_delay);
		tr_res = 0x3F;
		status = WARNING_RET_VAL;

	} else if ((tr_res + tap_delay) < 0) {
		tf_printf("WARNING: (tr_res %d + tap_delay %d) < 0\n",
			  tr_res, tap_delay);
		tr_res = 0;
		status = WARNING_RET_VAL;

	} else
		tr_res = tr_res + tap_delay;

	if (nb_idx == 0) {
		((PUB_DXnBDLR2_t *)&bdlrx)->dsoebd = tr_res;
		pub_write(ADDR_DXn(BDLR2, bl_idx), bdlrx);
	} else {
		((PUB_DXnBDLR7_t *)&bdlrx)->x4dsoebd = tr_res;
		pub_write(ADDR_DXn(BDLR7, bl_idx), bdlrx);
	}

	emc_write(EMC_MC_DDR_IF, reg_data);

	return status;
}

/* This command modifies the DDR PHY TAP results. */
int do_edit_tap_results(int argc, char * const argv[])
{
	if (strcmp(argv[0], "edit_wctap") == 0 ||
	    strcmp(argv[0], "edit_rctap") == 0)
		return edit_xctap(argc, argv);
	else if (strcmp(argv[0], "edit_wdtap") == 0 ||
		 strcmp(argv[0], "edit_rdtap") == 0)
		return edit_xdtap(argc, argv);
	else if (strcmp(argv[0], "edit_wltap") == 0)
		return edit_wltap(argc, argv);
	else if (strcmp(argv[0], "edit_wetap") == 0)
		return edit_wetap(argc, argv);

	tf_printf("Please call this cmd with one of the following alias:\n"
		  "To modify a Single Data Lane Write Centralization TAP:\n"
		  "edit_wctap DDR<n> rank_index bl_index nb_index tap_delay\n"
		  "To modify a Single Data Lane Read Centralization TAP:\n"
		  "edit_rctap DDR<n> rank_index bl_index nb_index tap_delay\n"
		  "To modify a Single DQ Write Delay TAP:\n"
		  "edit_wdtap DDR<n> bl_index bit_index tap_index\n"
		  "To modify a Single DQ Read Delay TAP:\n"
		  "edit_rdtap DDR<n> bl_index bit_index tap_index\n");
	return -1;
}

/* Modify SDRAM Vref Setup. */
static int edit_sdram_vref(int dimm_idx,int rank_idx,int bl_idx,
			   int nb_idx,int vref_val)
{
	int phy_rank;
	uint32_t mc_ddr_if;
	uint32_t sdram_vref_val, sdram_vref_rng, sdram_vref_data;
	uint32_t tmp_mr3_pda, schcr0, schcr1, pda_dq;
	uint32_t mr6_vref_set, mr6_vref_unset;

	mc_ddr_if = emc_read(EMC_MC_DDR_IF);
	emc_write(EMC_MC_DDR_IF, 0);

	mr6_get_vrefdq(vref_val, &sdram_vref_val, &sdram_vref_rng);
	sdram_vref_data = (sdram_vref_rng << 6) | sdram_vref_val;

	phy_rank = ((dimm_idx << 1) + rank_idx) << 2;
	tmp_mr3_pda = MR(3, rank_idx) | 0x0010;
	schcr1 = (phy_rank << 28) | (tmp_mr3_pda << 8) | 0x30;
	schcr0 = 0x10;

	if (!ddr_phy_scheduler_single_op(schcr0, schcr1)) {
		MEM_ERR(1, "DDR PHY Scheduler Single op timed out"
			" (step 9.).\n");
		return -1;
	}

	if (!ddr_phy_scheduler_single_nop(schcr0, schcr1)) {
		MEM_ERR(1, "DDR PHY Scheduler Single nop timed out"
			" (step 10.).\n");
		return -1;
	}

	if (dp->type == RDIMM) {
		schcr1 = (phy_rank << 28) | (tmp_mr3_pda << 8) | 0xb0;
		schcr0 = 0x10;
		if (!ddr_phy_scheduler_single_op(schcr0, schcr1)) {
			MEM_ERR(1, "DDR PHY Scheduler Single op timed out"
				" (step 11)c.).\n");
			return 0;
		}
		if (!ddr_phy_scheduler_single_nop(schcr0, schcr1)) {
			MEM_ERR(1, "DDR PHY Scheduler Single nop timed out"
				" (step 11)d.).\n");
			return 0;
		}
	}

	pda_dq = dp->package != PACKAGE_x4 ? 0x3ffff ^ (0x3 << (bl_idx * 2)) :
				   0x3ffff ^ ((1 << nb_idx) << (bl_idx * 2));

	mr6_vref_set = (MR(6, rank_idx) & 0xff00) | 0x80 | sdram_vref_data;
	schcr1 = (phy_rank << 28) | (mr6_vref_set << 8) | 0x60;
	if (dp->type == RDIMM && bl_idx > 4)
		schcr1 |= 0x80;
	schcr0 = (pda_dq << 14) | 0x10;

	if (!ddr_phy_scheduler_single_op(schcr0, schcr1)) {
		MEM_ERR(1, "DDR PHY Scheduler Single op timed out"
			" (step 18.).\n");
		return -1;
	}

	if (!ddr_phy_scheduler_single_nop(schcr0, schcr1)) {
		MEM_ERR(1, "DDR PHY Scheduler Single nop timed out"
			" (step 19.).\n");
		return -1;
	}

	mr6_vref_unset = (MR(6, rank_idx) & 0xff00) | sdram_vref_data;
	schcr1 = (phy_rank << 28) | (mr6_vref_unset << 8) | 0x60;
	if (dp->type == RDIMM && bl_idx > 4)
		schcr1 |= 0x80;
	schcr0 = (pda_dq << 14) | 0x10;

	if (!ddr_phy_scheduler_single_op(schcr0, schcr1)) {
		MEM_ERR(1, "DDR PHY Scheduler Single op timed out"
			" (step 24.).\n");
		return 0;
	}
	if (!ddr_phy_scheduler_single_nop(schcr0, schcr1)) {
		MEM_ERR(1, "DDR PHY Scheduler Single nop timed out"
			" (step 25.).\n");
		return 0;
	}

	schcr1 = (phy_rank << 28) | (MR(3, rank_idx) << 8) | 0x30;
	schcr0 = 0x10;

	if (!ddr_phy_scheduler_single_op(schcr0, schcr1)) {
		MEM_ERR(1, "DDR PHY Scheduler Single op timed out"
			" (step 28).\n");
		return 0;
	}
	if (!ddr_phy_scheduler_single_nop(schcr0, schcr1)) {
		MEM_ERR(1, "DDR PHY Scheduler Single nop timed out"
			" (step 29).\n");
		return 0;
	}

	if (dp->type == RDIMM) {

		schcr1 = (phy_rank << 28) | (MR(3, rank_idx) << 8) | 0xb0;
		schcr0 = 0x10;

		if (!ddr_phy_scheduler_single_op(schcr0, schcr1)) {
			MEM_ERR(1, "DDR PHY Scheduler Single op timed out"
				" (step 30)c.).\n");
			return 0;
		}
		if (!ddr_phy_scheduler_single_nop(schcr0, schcr1)) {
			MEM_ERR(1, "DDR PHY Scheduler Single nop timed out"
				" (step 30)d).\n");
			return 0;
		}
	}
	pub_write(PUB_SCHCR0, 0);
	pub_write(PUB_SCHCR1, 0);

	if (nb_idx == 0)
		dp->res_mr6_vref[rank_idx][bl_idx] = sdram_vref_data;
	else
		dp->res_mr6_vref_x4[rank_idx][bl_idx] = sdram_vref_data;

	emc_write(EMC_MC_DDR_IF, mc_ddr_if);
	return 0;
}

/* Modify DDR PHY Vref Setup. */
static int edit_ddr_phy_vref(int dimm_idx,int rank_idx,int bl_idx,
			     int nb_idx,int vref_val)
{
	int phy_rank;
	uint32_t vdq_isel;
	uint32_t mc_ddr_if;

	PUB_DXnGCR5_t gcr5;
	PUB_DXnGCR8_t gcr8;

	mc_ddr_if = emc_read(EMC_MC_DDR_IF);
	emc_write(EMC_MC_DDR_IF, 0);

	phy_rank = (dimm_idx << 1) + rank_idx;

	vdq_isel = get_host_vrefdq_isel(vref_val);

	if (nb_idx == 0 || dp->package != PACKAGE_x4) {
		gcr5.word = pub_read(ADDR_DXn(GCR5, bl_idx));
		SET_RANK_FIELD(gcr5, dxrefiselr, phy_rank, vdq_isel);
		pub_write(ADDR_DXn(GCR5, bl_idx), gcr5.word);
	}

	if (nb_idx == 1 || dp->package != PACKAGE_x4) {
		gcr8.word = pub_read(ADDR_DXn(GCR8, bl_idx));
		SET_RANK_FIELD(gcr8, x4dxrefiselr, phy_rank, vdq_isel);
		pub_write(ADDR_DXn(GCR8, bl_idx), gcr8.word);
	}

	emc_write(EMC_MC_DDR_IF, mc_ddr_if);
	return 0;
}

/* This command modifies the Vref results. */
int do_vref(int argc, char * const argv[])
{
	int parsed, op;
	int mss_idx, dimm_idx, rank_idx, bl_idx, vref_val;
	int nb_idx = 0;

	parsed = (argc >= 6) &&
	  get_arg_str(argv[0], &op, "Command", 2,
		"edit_sdram_vref", 0, "edit_ddr_phy_vref", 1) &&
	  get_arg_str(argv[1], &mss_idx, "DDR<n>", 2, "DDR0", 0, "DDR1", 1) &&
	  get_arg_int(argv[2], &dimm_idx, 1, 0, MAX_DIMM_PER_MEM_CTRL,
		"dimm_index") &&
	  get_arg_int(argv[3], &rank_idx, 1, 0, MAX_RANKS_PER_MEM_CTRL,
		"rank_index") &&
	  get_arg_int(argv[4], &bl_idx, 1, 0, BYTELANE_NUM, "bl_index") &&
	  ((argc == 6 &&
	  get_arg_int(argv[5], &vref_val, 1, 0, 1200, "vref_val")) ||
	  ((argc > 6 && get_arg_int(argv[5], &nb_idx, 1, 0, 2, "nb_index") &&
	  get_arg_int(argv[6], &vref_val, 1, 0, 1200, "vref_val"))));

	if (!parsed) {
		tf_printf("Invalid arguments, should be:\n");
		tf_printf("%s DDR<n> <dimm> <rank> <bl> [<nb>] <vref_val>\n",
			  argv[0]);
		tf_printf("<nb> should only be specified in x4 systems.\n");
		return -1;
	}

	if (!ddr_switch_current_mss(mss_idx)) {
		tf_printf("MSS%d not configured!\n", mss_idx);
		return -1;
	}

	if (dp->dimm[dimm_idx].ranks == 0) {
		tf_printf("Entered DIMM not present in MSS.\n");
		return -1;
	}

	if ((dp->active_ranks & (1 << rank_idx)) == 0) {
		tf_printf("Entered rank not present in MSS.\n");
		return -1;
	}

	if (dp->package != PACKAGE_x4 && nb_idx == 1) {
		tf_printf("nb_index = 1 only supported on x4 configuration.\n");
		return -1;
	}

	if (op == 0)
		return edit_sdram_vref(dimm_idx, rank_idx, bl_idx,
				       nb_idx, vref_val);
	else
		return edit_ddr_phy_vref(dimm_idx, rank_idx, bl_idx,
					 nb_idx,vref_val);

	return -1;
}

int do_phy_bist(int argc, char * const argv[])
{
	unsigned int mss_map;
	uint32_t bist_result;

	if (argc != 2) {
		tf_printf("Unsupported argument number. Should be:\n");
		tf_printf("%s DDR<n>\n", __func__);
		return -1;
	}

	mss_map = get_entered_mss_map(argv[1]);
	if (mss_map == -1)
		return -1;

	for (int i = 0; i < MAX_MEM_CTRL; i++) {

		if (!(mss_map & (1 << i)))
			continue;
		if (!ddr_switch_current_mss(i))
			continue;

		phy_bist_setup();
		if (phy_bist_run(&bist_result)) {
			ERROR("PHY BIST timed out!\n");
			continue;
		}

		if (!bist_result)
			tf_printf("PHY BIST on MSS%d PASS.\n", i);
		else
			tf_printf("PHY BIST on MSS%d FAIL.\n", i);
	}

	return 0;
}

/* Data needed for the find_margin() function. */
struct margin_reg_data {
	/* This is the minimum value the register field can have. */
	uint32_t min_val;
	/* This is the maximum value the register field can have. */
	uint32_t max_val;
	/* This is where the found minimum value is written back to. */
	uint32_t *min_ret_addr;
	/* This is where the found maximum value is written back to. */
	uint32_t *max_ret_addr;
	/* The name of the field (used in verbose printing). */
	char *field_name;
	/*
	 * This is the function which modifies the register's field.
	 * The offset is the offset value which should be applied the the
	 * register's field value. It returns the field's value after
	 * modification. A second void * argument is used in case this function
	 * requires some extra data to work properly.
	 */
	uint32_t (*mod_reg_field)(int offset, void *extra);
	/* The extra argument to be given to mod_reg_field(). */
	void *extra;
};

/*
 * This function finds the minimum and maximum (exclusive) value of the given
 * register's corresponding field for the PHY BIST to pass.
 * reg_num is the length of the reg_data array, as this may be more than one
 * in case we are doing the per-data-lane read centralization margin with
 * regards to both DXnLCDLR3.RDQSD and DXnLCDLR4.RDQSND.
 * Returns 0 if everything went well, 1 if the original value of the register
 * exceeds the given limit or -1 if the PHY BIST timed out.
 */
int find_margin(int reg_num, struct margin_reg_data *reg_data)
{
	uint32_t bist_result;
	uint32_t cur_val, orig_val;
	int ret_status = 0;
	int is_limit = 0;

	/*
	 * We first get the original values of the register's field.
	 * We are using the place to return the final max value to temporaily
	 * store the original value.
	 * We also check that the default does not hit the limit value.
	 */
	for (int i = 0; i < reg_num; i++) {
		cur_val = reg_data[i].mod_reg_field(0, reg_data[i].extra);

		if (cur_val < reg_data[i].min_val) {
			is_limit = 1;
			ret_status = 1;
			tf_printf("ERROR: %s %u < %u\n",
			  reg_data[i].field_name, cur_val, reg_data[i].min_val);
		} else if (cur_val == reg_data[i].min_val)
			is_limit = 1;

		*reg_data[i].max_ret_addr = cur_val;
	}

	if (ddr_verbose_level >= 3) {
		MEM_VERB(2, "Run right bist,");
		for (int i = 0; i < reg_num; i++)
			MEM_VERB(2, " %s = %u", reg_data[i].field_name,
					    *reg_data[i].max_ret_addr);
		MEM_VERB(2, "\n\n");
	}

	if (phy_bist_run(&bist_result))
		return -1;

	MEM_VERB(3, "phy_bist_run %s\n", bist_result ? "fail":"pass");

	/* Loop until BIST fails or (one of) the reg field hit the limit. */
	while ((bist_result == 0) && !is_limit) {

		MEM_VERB(3, "Run right bist,");

		for (int i = 0; i < reg_num; i++) {
			/* Decrement the field value by 1. */
			cur_val = reg_data[i].mod_reg_field(-1,
							    reg_data[i].extra);

			if (cur_val == reg_data[i].min_val)
				is_limit = 1;

			MEM_VERB(3, " %s = %u", reg_data[i].field_name, cur_val);
		}
		MEM_VERB(3, "\n");

		if (phy_bist_run(&bist_result))
			return -1;

		MEM_VERB(3, "phy_bist_run %s\n", bist_result ? "fail":"pass");
	}

	/*
	 * Record the reg's field value when the right limit is hit.
	 * Also restore the original value to the field and see if it hits the
	 * other limit.
	 */
	is_limit = 0;
	for (int i = 0; i < reg_num; i++) {
		cur_val = reg_data[i].mod_reg_field(0, reg_data[i].extra);

		/* Write the minimum value (exclusive) which BIST passed. */
		*reg_data[i].min_ret_addr = cur_val;

		/* Restore the original value of the reg's field. */
		orig_val = *reg_data[i].max_ret_addr;
		cur_val = reg_data[i].mod_reg_field(orig_val - cur_val,
						    reg_data[i].extra);

		if (cur_val > reg_data[i].max_val) {
			is_limit = 1;
			ret_status = 1;
			tf_printf("ERROR: %s %u > bl_iprd %u\n",
			  reg_data[i].field_name, cur_val, reg_data[i].max_val);
		} else if (cur_val == reg_data[i].max_val)
			is_limit = 1;
	}

	if (ddr_verbose_level >= 3) {
		MEM_VERB(2, "Run left bist,");
		for (int i = 0; i < reg_num; i++)
			MEM_VERB(3, " %s = %u", reg_data[i].field_name,
					    *reg_data[i].max_ret_addr);
		MEM_VERB(3, "\n\n");
	}

	if (phy_bist_run(&bist_result))
		return -1;

	MEM_VERB(3, "phy_bist_run %s\n", bist_result ? "fail":"pass");

	/* Loop until BIST fails or (one of) the reg field hit the limit. */
	while ((bist_result == 0) && !is_limit) {

		MEM_VERB(3, "Run left bist,");

		for (int i = 0; i < reg_num; i++) {
			/* Increment the field value by 1. */
			cur_val = reg_data[i].mod_reg_field(1,
							    reg_data[i].extra);

			if (cur_val == reg_data[i].max_val)
				is_limit = 1;

			MEM_VERB(3, " %s = %u", reg_data[i].field_name, cur_val);
		}
		MEM_VERB(3, "\n");

		if (phy_bist_run(&bist_result))
			return -1;

		MEM_VERB(3, "phy_bist_run %s\n", bist_result ? "fail":"pass");
	}

	/*
	 * Record the reg's field value when the left limit is hit.
	 * Also restore the original value to the field.
	 */
	for (int i = 0; i < reg_num; i++) {
		cur_val = reg_data[i].mod_reg_field(0, reg_data[i].extra);

		orig_val = *reg_data[i].max_ret_addr;

		/* Write the maximum value (exclusive) which BIST passed. */
		*reg_data[i].max_ret_addr = cur_val;

		/* Restore the original value of the reg's field. */
		reg_data[i].mod_reg_field((int)orig_val - (int)cur_val,
					  reg_data[i].extra);
	}

	return ret_status;
}

/* Print out the per-data-lane margin data. */
static void show_margin_data(int len, struct per_data_lane_data *data)
{
	printf("%-10s %-18s %-18s %-18s %-18s\n",
		"entry", "TR_VAL", "RIGHT_EDGE", "LEFT_EDGE", "BL_TAP_PS");

	for (int i = 0; i < len; i++) {
		printf("%-6u%s %-18u %-18u %-18u %u.%03u\n",
			data[i].lane_num,
			data[i].is_x4_other ? "(x4)" : "    ",
			data[i].tr_val,
			data[i].right_edge,
			data[i].left_edge,
			data[i].bl_tap_fs / 1000,
			data[i].bl_tap_fs % 1000);
	}
	tf_printf("\n");
}

/* Struct of the extra arguments mod_bdlr_bd_field() needs to do it's work. */
struct mod_bdlr_bd_field_args {
	int bl_idx;
	int bit_idx;
	int nibble;
};

/* A mod_reg_field() function which modifies the corresponding DXnBDLRn reg. */
uint32_t mod_bdlr_bd_field(int offset, void *extra)
{
	struct mod_bdlr_bd_field_args *args =
					(struct mod_bdlr_bd_field_args *)extra;

	int val = get_bdlr_bd_field(args->bl_idx, args->bit_idx, args->nibble);

	if (offset != 0) {
		val += offset;
		set_bdlr_bd_field(args->bl_idx, args->bit_idx, args->nibble,
				  val);
	}

	return val;
}

/*
 * Do DDR PHY Per-Data-Lane Write/Read Per-Bit-Skew Margin. Assuming the active
 * MSS is already set before calling this function.
 */
void show_skew_margin(int rank_idx, int is_write, int bl_map, int bit_map)
{
	int status;
	uint32_t bl_iprd;
	uint32_t bl_tap_fs;
	uint32_t nibble, dqxbd_tr_res;
	uint32_t ret_status = 0;
	int phy_rank = 4 * rank_idx;
	int count = 0;

	struct per_data_lane_data *res_xbd = dbg_arrays->res_xbd;

	PUB_PGCR6_t pgcr6;
	PUB_DXnMDLR0_t dx_x_mdlr0;
	PUB_BISTRR_t bistrr, bistrr_orig;

	/* If default value of 0 is given, we display all bytelanes/bits. */
	if (bl_map == 0)
		bl_map = (1 << BYTELANE_NUM) - 1;
	if (bit_map == 0)
		bit_map = (1 << 8) - 1;

	bistrr_orig.word = pub_read(PUB_BISTRR);

	pgcr6.word = pub_read(PUB_PGCR6);
	pgcr6.inhvt = 1;
	pub_write(PUB_PGCR6, pgcr6.word);

	MEM_VERB(3, "Calling phy_bist_setup\n");
	phy_bist_setup();

	SET_MEM_REG_FIELDS(pub, PUB_BISTAR1,
		reg.brank = phy_rank;
		reg.bmrank = phy_rank);

	for (int bl_idx = 0; bl_idx < BYTELANE_NUM; bl_idx++) {

		if ((bl_map & (1 << bl_idx)) == 0)
			continue;

		dx_x_mdlr0.word = pub_read(ADDR_DXn(MDLR0, bl_idx));

		bl_iprd = dx_x_mdlr0.iprd;

		bl_tap_fs = dp->tck / (2 * bl_iprd);

		MEM_VERB(3, "bl_tap_fs = tck /(2 * bl_iprd) = ");
		MEM_VERB(3, "%llu / (2 * %u) = %u\n", dp->tck, bl_iprd, bl_tap_fs);

		bistrr.word = pub_read(PUB_BISTRR);
		bistrr.bdxsel = bl_idx;
		pub_write(PUB_BISTRR, bistrr.word);

		for (int bit_idx = 0; bit_idx < 8; bit_idx++) {

			if ((bit_map & (1 << bit_idx)) == 0)
				continue;

			if (is_write)
				nibble = (bit_idx / 4); /* can be 0 or 1 */
			else
				nibble = 3 + (bit_idx / 4);/* can be 3 or 4 */

			dqxbd_tr_res =
				get_bdlr_bd_field(bl_idx, bit_idx, nibble);

			res_xbd[count].tr_val = dqxbd_tr_res;
			res_xbd[count].bl_tap_fs = bl_tap_fs;
			res_xbd[count].lane_num = bl_idx * 8 + bit_idx;
			res_xbd[count].is_x4_other = 0;

			MEM_VERB(3, "\nOn byte %u, bit %u\n", bl_idx, bit_idx);

			struct mod_bdlr_bd_field_args args = {
				.bl_idx = bl_idx,
				.bit_idx = bit_idx,
				.nibble = nibble,
			};

			struct margin_reg_data reg_data = {
				.min_val = 0,
				.max_val = MIN(bl_iprd, 63u),
				.min_ret_addr = &res_xbd[count].right_edge,
				.max_ret_addr = &res_xbd[count].left_edge,
				.mod_reg_field = mod_bdlr_bd_field,
				.extra = &args,
			};

			if (is_write)
				reg_data.field_name = "wbd";
			else
				reg_data.field_name = "rbd";

			status = find_margin(1, &reg_data);

			count++;

			if (status == -1)
				return;
			else if (status == 1)
				ret_status = 1;
		}
	}

	pgcr6.word = pub_read(PUB_PGCR6);
	pgcr6.inhvt = 0;
	pub_write(PUB_PGCR6, pgcr6.word);

	pub_write(PUB_BISTRR, bistrr_orig.word);

	tf_printf("Show %s skew margin for DDR PHY MSS%d\n\n",
		  is_write ? "WRITE" : "READ", dp->mss_index);
	tf_printf("status = %s\n", ((ret_status) ? "FAIL" : "PASS"));

	show_margin_data(count, res_xbd);
}

/* Define a mod_reg_field() function which modifies the specified DDXLCDLRn. */
#define DEFINE_MOD_LCDLRn_FIELD(n, field)				\
uint32_t mod_lcdlr##n##_##field##_field(int offset, void *extra)	\
{									\
	int bl_idx = *(int *)extra;					\
	int val;							\
									\
	PUB_DXnLCDLR##n##_t lcdlrx = {					\
		.word = pub_read(ADDR_DXn(LCDLR##n, bl_idx)),		\
	};								\
									\
	val = lcdlrx.field;						\
									\
	if (offset != 0) {						\
		val += offset;						\
		lcdlrx.field = val;					\
		pub_write(ADDR_DXn(LCDLR##n, bl_idx), lcdlrx.word);	\
	}								\
	return val;							\
}

DEFINE_MOD_LCDLRn_FIELD(1, wdqd)
DEFINE_MOD_LCDLRn_FIELD(1, x4wdqd)
DEFINE_MOD_LCDLRn_FIELD(3, rdqsd)
DEFINE_MOD_LCDLRn_FIELD(4, rdqsnd)
DEFINE_MOD_LCDLRn_FIELD(3, x4rdqsd)
DEFINE_MOD_LCDLRn_FIELD(4, x4rdqsnd)

/*
 * Do DDR PHY Per-Data-Lane Write Centralization Margin. Assuming the active
 * MSS is already set before calling this function.
 */
void show_centralization_margin(int rank_idx, int is_write, int bl_map,
				int nb_map)
{
	uint32_t bl_iprd;
	uint32_t bl_tap_fs;
	uint32_t ret_status = 0;
	struct per_data_lane_data *res_wdqd = dbg_arrays->dlwc.res_wdqd;
	struct per_data_lane_data *res_rdqsd = dbg_arrays->dlrc.res_rdqsd;
	struct per_data_lane_data *res_rdqsnd = dbg_arrays->dlrc.res_rdqsnd;
	struct margin_reg_data reg_data[2];
	int phy_rank = 4 * rank_idx;

	PUB_PGCR6_t pgcr6;
	PUB_DXnMDLR0_t dx_x_mdlr0;
	PUB_BISTRR_t bistrr, bistrr_orig;
	PUB_DXnLCDLR1_t dx_x_lcdlr1;
	PUB_DXnLCDLR3_t dx_x_lcdlr3;
	PUB_DXnLCDLR4_t dx_x_lcdlr4;

	int status;
	int count = 0;

	/* If default value of 0 is given, we display all bytelanes/nibble. */
	if (bl_map == 0)
		bl_map = (1 << BYTELANE_NUM) - 1;
	if (nb_map == 0)
		nb_map = (1 << 2) - 1;

	bistrr_orig.word = pub_read(PUB_BISTRR);

	pgcr6.word = pub_read(PUB_PGCR6);
	pgcr6.inhvt = 1;
	pub_write(PUB_PGCR6, pgcr6.word);

	MEM_VERB(3, "Call phy_bist_setup\n");
	phy_bist_setup();

	SET_MEM_REG_FIELDS(pub, PUB_BISTAR1,
		reg.brank = phy_rank;
		reg.bmrank = phy_rank);

	pub_switch_rankidr(rank_idx, 1, 1);

	for (int bl_idx = 0; bl_idx < BYTELANE_NUM && (nb_map & 1); bl_idx++) {

		if ((bl_map & (1 << bl_idx)) == 0)
			continue;

		dx_x_mdlr0.word = pub_read(ADDR_DXn(MDLR0, bl_idx));

		bl_iprd = dx_x_mdlr0.iprd;

		bl_tap_fs = dp->tck / (2 * bl_iprd);

		MEM_VERB(3, "bl_tap_fs = tck / (2 * bl_iprd) = ");
		MEM_VERB(3, "%llu / (2 * %u) = %u\n", dp->tck, bl_iprd, bl_tap_fs);

		if (is_write) {
			dx_x_lcdlr1.word = pub_read(ADDR_DXn(LCDLR1, bl_idx));
			res_wdqd[count].tr_val = dx_x_lcdlr1.wdqd;
			res_wdqd[count].bl_tap_fs = bl_tap_fs;
			res_wdqd[count].lane_num = bl_idx;
			res_wdqd[count].is_x4_other = 0;
		} else {
			dx_x_lcdlr3.word = pub_read(ADDR_DXn(LCDLR3, bl_idx));
			dx_x_lcdlr4.word = pub_read(ADDR_DXn(LCDLR4, bl_idx));
			res_rdqsd[count].tr_val = dx_x_lcdlr3.rdqsd;
			res_rdqsnd[count].tr_val = dx_x_lcdlr4.rdqsnd;
			res_rdqsd[count].bl_tap_fs = bl_tap_fs;
			res_rdqsnd[count].bl_tap_fs = bl_tap_fs;
			res_rdqsd[count].lane_num = bl_idx;
			res_rdqsd[count].is_x4_other = 0;
			res_rdqsnd[count].lane_num = bl_idx;
			res_rdqsnd[count].is_x4_other = 0;
		}

		bistrr.word = pub_read(PUB_BISTRR);
		bistrr.bdxsel = bl_idx;
		pub_write(PUB_BISTRR, bistrr.word);

		MEM_VERB(3, "\nOn byte %u\n", bl_idx);

		reg_data[0].min_val = 0;
		reg_data[0].max_val = bl_iprd;
		reg_data[0].extra = &bl_idx;

		if (is_write) {
			reg_data[0].min_ret_addr = &res_wdqd[count].right_edge;
			reg_data[0].max_ret_addr = &res_wdqd[count].left_edge;
			reg_data[0].field_name = "wdqd";
			reg_data[0].mod_reg_field = mod_lcdlr1_wdqd_field;

			status = find_margin(1, reg_data);
		} else {
			reg_data[0].min_ret_addr = &res_rdqsd[count].right_edge;
			reg_data[0].max_ret_addr = &res_rdqsd[count].left_edge;
			reg_data[0].field_name = "rdqsd";
			reg_data[0].mod_reg_field = mod_lcdlr3_rdqsd_field;

			reg_data[1].min_val = 0;
			reg_data[1].max_val = bl_iprd;
			reg_data[1].extra = &bl_idx;
			reg_data[1].min_ret_addr =
					       &res_rdqsnd[count].right_edge;
			reg_data[1].max_ret_addr = &res_rdqsnd[count].left_edge;
			reg_data[1].field_name = "rdqsnd";
			reg_data[1].mod_reg_field = mod_lcdlr4_rdqsnd_field;

			status = find_margin(2, reg_data);
		}
		count++;

		if (status == -1)
			return;
		else if (status == 1)
			ret_status = 1;
	}

	for (int bl_idx = 0; dp->package == PACKAGE_x4 && (nb_map & 2) &&
		 bl_idx < BYTELANE_NUM; bl_idx++) {

		if ((bl_map & (1 << bl_idx)) == 0)
			continue;

		dx_x_mdlr0.word = pub_read(ADDR_DXn(MDLR0, bl_idx));

		bl_iprd = dx_x_mdlr0.iprd;

		bl_tap_fs = dp->tck / (2 * bl_iprd);

		MEM_VERB(3, "bl_tap_fs = tck / (2 * bl_iprd) = ");
		MEM_VERB(3, "%llu / (2 * %u) = %u\n", dp->tck, bl_iprd, bl_tap_fs);

		if (is_write) {
			dx_x_lcdlr1.word = pub_read(ADDR_DXn(LCDLR1, bl_idx));
			res_wdqd[count].tr_val = dx_x_lcdlr1.x4wdqd;
			res_wdqd[count].bl_tap_fs = bl_tap_fs;
			res_wdqd[count].lane_num = bl_idx;
			res_wdqd[count].is_x4_other = 1;
		} else {
			dx_x_lcdlr3.word = pub_read(ADDR_DXn(LCDLR3, bl_idx));
			dx_x_lcdlr4.word = pub_read(ADDR_DXn(LCDLR4, bl_idx));
			res_rdqsd[count].tr_val = dx_x_lcdlr3.x4rdqsd;
			res_rdqsnd[count].tr_val = dx_x_lcdlr4.x4rdqsnd;
			res_rdqsd[count].bl_tap_fs = bl_tap_fs;
			res_rdqsnd[count].bl_tap_fs = bl_tap_fs;
			res_rdqsd[count].lane_num = bl_idx;
			res_rdqsd[count].is_x4_other = 1;
			res_rdqsnd[count].lane_num = bl_idx;
			res_rdqsnd[count].is_x4_other = 1;
		}

		bistrr.word = pub_read(PUB_BISTRR);
		bistrr.bdxsel = bl_idx;
		pub_write(PUB_BISTRR, bistrr.word);

		MEM_VERB(3, "\nOn byte %u\n", bl_idx);

		reg_data[0].min_val = 0;
		reg_data[0].max_val = bl_iprd;
		reg_data[0].extra = &bl_idx;

		if (is_write) {
			reg_data[0].min_ret_addr = &res_wdqd[count].right_edge;
			reg_data[0].max_ret_addr = &res_wdqd[count].left_edge;
			reg_data[0].field_name = "x4wdqd";
			reg_data[0].mod_reg_field = mod_lcdlr1_x4wdqd_field;

			status = find_margin(1, reg_data);
		} else {
			reg_data[0].min_ret_addr = &res_rdqsd[count].right_edge;
			reg_data[0].max_ret_addr = &res_rdqsd[count].left_edge;
			reg_data[0].field_name = "x4rdqsd";
			reg_data[0].mod_reg_field = mod_lcdlr3_x4rdqsd_field;

			reg_data[1].min_val = 0;
			reg_data[1].max_val = bl_iprd;
			reg_data[1].extra = &bl_idx;
			reg_data[1].min_ret_addr =
						&res_rdqsnd[count].right_edge;
			reg_data[1].max_ret_addr = &res_rdqsnd[count].left_edge;
			reg_data[1].field_name = "x4rdqsnd";
			reg_data[1].mod_reg_field = mod_lcdlr4_x4rdqsnd_field;

			status = find_margin(2, reg_data);
		}
		count++;

		if (status == -1)
			return;
		else if (status == 1)
			ret_status = 1;

	}

	pgcr6.word = pub_read(PUB_PGCR6);
	pgcr6.inhvt = 0;
	pub_write(PUB_PGCR6, pgcr6.word);

	pub_write(PUB_BISTRR, bistrr_orig.word);

	tf_printf("Show %s centralization margin for DDR PHY MSS%d\n\n",
		  is_write ? "WRITE" : "READ", dp->mss_index);
	tf_printf("status = %s\n", ((ret_status) ? "FAIL" : "PASS"));

	if (is_write)
		show_margin_data(count, res_wdqd);
	else {
		tf_printf("RES_RDQSD TABLE:\n");
		show_margin_data(count, res_rdqsd);

		tf_printf("RES_RDQSND TABLE:\n");
		show_margin_data(count, res_rdqsnd);
	}
}

#define	D2_WRITE_FIND_VREF_IDX_PERCENT_VAL(_vref_idx_) \
	(((_vref_idx_) > 24) ? VREF_DQ_1((_vref_idx_) - (73 - 50)) \
			     : VREF_DQ_2((_vref_idx_)))

#define	D2_READ_FIND_VREF_IDX_PERCENT_VAL(_vref_idx_) \
	(PHY_RD_VREF_DQ((_vref_idx_)))

#define D2_BIST_RESULT_PASS	\
	(GET_D2_BIST_RESULT(res_2d[vref_idx], dll_idx) == D2_BIST_PASS)

#define D2_BIST_RESULT_FAIL	\
	(GET_D2_BIST_RESULT(res_2d[vref_idx], dll_idx) == D2_BIST_FAIL)

/*
 * Plots out the eye plot for each bytelane. Also returns the calculated
 * margins from the raw eye pass/fail results.
 */
static void process_eye_plot_data(struct per_bl_eye_data data,
				  int res_2d[D2_VREF_MAX_VAL][D2_DLL_INT_USED],
				  int is_write, struct per_bytelane_margin *pbm)
{
	uint32_t cross_vref_idx;

	uint32_t central_left_margin;
	uint32_t central_right_margin;
	uint32_t vref_high_margin;
	uint32_t vref_low_margin;
	uint32_t central_left_margin_fs;
	uint32_t central_right_margin_fs;
	uint32_t vref_high_margin_volt_mul100;
	uint32_t vref_low_margin_volt_mul100;

	uint32_t percent_val, percent_actual;

	uint32_t central_left_position = INVALID_VAL;
	uint32_t central_right_position = INVALID_VAL;
	uint32_t vref_high_position = INVALID_VAL;
	uint32_t vref_low_position = INVALID_VAL;

	uint32_t vref_tr_val, xdqd_tr_val;
	int vref_len, dll_max;
	int dll_idx, vref_idx;

	if (is_write) {
		vref_len =  D2_VREF_MAX_VAL;
		dll_max = data.bl_iprd + 10;
		vref_tr_val = data.d2.wr.vref_tr_val;
		xdqd_tr_val = data.d2.wr.wdqd_tr_val;
	} else {
		vref_len =  D2_R_VREF_MAX_VAL;
		dll_max = data.bl_iprd;
		vref_tr_val = data.d2.rd.vref_tr_val;
		xdqd_tr_val = MIN(data.d2.rd.rdqsd_tr_val,
				  data.d2.rd.rdqsnd_tr_val);
	}

	/*
	 * First figure out the VREF index our current setup is using
	 * (cross_vref_idx).
	 */
	if (is_write) {
		int vref_range = (vref_tr_val >> 6) & 1;
		int vref_range_val = vref_tr_val & 0x3F;

		if (vref_range == 0) {
			/* range 1 */
			if (vref_range_val >= 2)
				/*
				 * We are also using the range 1 table for the
				 * vrefdq, so it is just a direct mapping here.
				 */
				cross_vref_idx = vref_range_val + 23;
			else {
				/*
				 * We are using the other range table, so we
				 * first need to find the corresponding percent
				 * value, and then compute what the index would
				 * be in the other table.
				 */
				percent_actual = VREF_DQ_1(vref_range_val);

				cross_vref_idx = get_vref_index(2,
								percent_actual);
			}
		} else {
			/* range 2 */
			if (vref_range_val <= 24)
				cross_vref_idx = vref_range_val;
			else {
				percent_actual = VREF_DQ_2(vref_range_val);

				cross_vref_idx = get_vref_index(1,
							percent_actual) + 23;
			}
		}
	} else
		cross_vref_idx = vref_tr_val;


	/*
	 * Get the highest/lowest vrefdq value which passed the PHY BIST test
	 * when we are using the default tr_val.
	 */
	dll_idx = xdqd_tr_val;
	for (vref_idx = cross_vref_idx; vref_idx < vref_len; vref_idx++){
		if (D2_BIST_RESULT_FAIL)
			break;
		vref_high_position = vref_idx;
	}
	for (vref_idx = cross_vref_idx; vref_idx >= 0; vref_idx--){
		if (D2_BIST_RESULT_FAIL)
			break;
		vref_low_position = vref_idx;
	}

	/*
	 * Get the smallest/biggest dll value which passed the PHY BIST test
	 * when we are using the default vrefdq.
	 */
	vref_idx = cross_vref_idx;
	for (dll_idx = xdqd_tr_val; dll_idx < dll_max; dll_idx++){
		if (D2_BIST_RESULT_FAIL)
			break;
		central_right_position = dll_idx;
	}
	for (dll_idx = xdqd_tr_val; dll_idx >= 0; dll_idx--){
		if (D2_BIST_RESULT_FAIL)
			break;
		central_left_position = dll_idx;
	}

	/* treat corner cases */
	if (central_left_position == INVALID_VAL)
		central_left_position = xdqd_tr_val;

	if (central_right_position == INVALID_VAL)
		central_right_position = xdqd_tr_val;

	if (vref_high_position == INVALID_VAL)
		vref_high_position = cross_vref_idx;

	if (vref_low_position == INVALID_VAL)
		vref_low_position = cross_vref_idx;

	/*
	 * Print right and left margins in picoseconds, high and low margins
	 * in voltage percents.
	 */

	central_left_margin = xdqd_tr_val - central_left_position;
	central_right_margin = central_right_position - xdqd_tr_val;

	vref_high_margin = vref_high_position - cross_vref_idx;
	vref_low_margin = cross_vref_idx - vref_low_position;

	central_left_margin_fs = central_left_margin * data.bl_tap_fs;
	central_right_margin_fs = central_right_margin * data.bl_tap_fs;

	vref_high_margin_volt_mul100 =
		D2_WRITE_FIND_VREF_IDX_PERCENT_VAL(vref_high_position) -
		D2_WRITE_FIND_VREF_IDX_PERCENT_VAL(cross_vref_idx);

	vref_low_margin_volt_mul100 =
		D2_WRITE_FIND_VREF_IDX_PERCENT_VAL(cross_vref_idx) -
		D2_WRITE_FIND_VREF_IDX_PERCENT_VAL(vref_low_position);

	if (pbm != NULL) {
		pbm->bytelane_num = data.bytelane_num;
		pbm->is_x4_other = data.is_x4_other;
		pbm->central_left_marg_fs = central_left_margin_fs;
		pbm->central_right_marg_fs = central_right_margin_fs;
		pbm->vref_high_marg_volt_mul100 = vref_high_margin_volt_mul100;
		pbm->vref_low_marg_volt_mul100 = vref_low_margin_volt_mul100;
	}

	if (!display_eye_plot)
		return;

	/* Draw the actual eye plotting result. */
	for (int vref_idx = vref_len - 1; vref_idx >= 0; vref_idx--) {

		if (is_write)
			percent_val =
				D2_WRITE_FIND_VREF_IDX_PERCENT_VAL(vref_idx);
		else
			percent_val =
				D2_READ_FIND_VREF_IDX_PERCENT_VAL(vref_idx);

		printf("%02d (%2d.%02d%%) ", vref_idx, (percent_val / 100),
						       (percent_val % 100));

		for (int dll_idx = 0; dll_idx < dll_max; dll_idx++) {
			if ((vref_idx == cross_vref_idx) &&
			    (dll_idx == xdqd_tr_val))
				tf_printf("+");
			else if (D2_BIST_RESULT_FAIL)
				tf_printf("X");
			else if (D2_BIST_RESULT_PASS) {
				if (vref_idx == cross_vref_idx)
					tf_printf("-");
				else if (dll_idx == xdqd_tr_val)
					tf_printf("|");
				else
					tf_printf(" ");
			} else
				tf_printf(".");
		}
		tf_printf("\n");
	}
	tf_printf("            0");
	for (int dll_idx = 1; dll_idx < xdqd_tr_val; dll_idx++)
		tf_printf(" ");
	printf("%02d", xdqd_tr_val);
	for (int dll_idx = xdqd_tr_val + 3; dll_idx < dll_max;
	     dll_idx++)
		tf_printf(" ");

	printf("%02d", dll_max);
	tf_printf("\n");

	printf("%-26s %u.%03u pSec (%u taps)\n",
			"central_left_margin = ",
			central_left_margin_fs / 1000,
			central_left_margin_fs % 1000,
			central_left_margin);

	printf("%-26s %u.%03u pSec (%u taps)\n",
			"central_right_margin = ",
			central_right_margin_fs / 1000,
			central_right_margin_fs % 1000,
			central_right_margin);

	printf("%-26s %u.%02u%% (%u indexes)\n",
			"vref_high_margin = ",
			(vref_high_margin_volt_mul100 / 100),
			(vref_high_margin_volt_mul100 % 100),
			vref_high_margin);

	printf("%-26s %u.%02u%% (%u indexes)\n",
			"vref_low_margin = ",
			(vref_low_margin_volt_mul100 / 100),
			(vref_low_margin_volt_mul100 % 100),
			vref_low_margin);
	tf_printf("\n\n");
}

/* Print out the eye plotting summary regarding the margins. */
static void print_eye_margin_summary(struct per_bytelane_margin *pbm,
				     struct per_bl_eye_data *data,
				     int count, int is_write)
{
	uint32_t central_left_marg_fs = 0;
	uint32_t central_right_marg_fs = 0;
	uint32_t vref_high_marg_volt_mul100 = 0;
	uint32_t vref_low_marg_volt_mul100 = 0;

	/* First print out the center point of each eye margin. */
	tf_printf("Per-Byte-Lane %s Data Eye Plotting for DDR PHY MSS%d:\n",
		  is_write ? "WRITE" : "READ", dp->mss_index);
	if (is_write)
		printf("%-10s %-15s %-15s %-15s %-15s %-15s\n", "byte_lane",
		      "VREF_TR_VAL", "WDQD_TR_VAL", "BL_IPRD", "BL_TPRD",
		      "BL_TAP_PS");
	else
		printf("%-10s %-15s %-15s %-15s %-15s %-15s %-15s\n",
		"byte_lane", "VREF_TR_VAL", "RDQSD_TR_VAL", "RDQSND_TR_VAL",
		"BL_IPRD", "BL_TPRD", "BL_TAP_PS");

	for (int i = 0; i < count; i++) {
		printf("%-6u%s ",
			data[i].bytelane_num,
			data[i].is_x4_other ? "(x4)" : "    ");
		if (is_write)
			printf("%-15u %-15u",
				data[i].d2.wr.vref_tr_val,
				data[i].d2.wr.wdqd_tr_val);
		else
			printf("%-15u %-15u %-15u",
				data[i].d2.rd.vref_tr_val,
				data[i].d2.rd.rdqsd_tr_val,
				data[i].d2.rd.rdqsd_tr_val);
		printf(" %-15u %-15u %u.%03u\n",
			data[i].bl_iprd,
			data[i].bl_tprd,
			data[i].bl_tap_fs / 1000,
			data[i].bl_tap_fs % 1000);
	}

	/* Then print out the margin data. */
	printf("%-10s %-15s %-15s %-15s %-15s\n", "byte_lane",
	      "left_central_marg", "right_central_marg", "high_vref_marg",
	      "low_vref_marg");

	for (int i = 0; i < count; i++) {
		central_left_marg_fs += pbm[i].central_left_marg_fs;
		central_right_marg_fs += pbm[i].central_right_marg_fs;
		vref_high_marg_volt_mul100 += pbm[i].vref_high_marg_volt_mul100;
		vref_low_marg_volt_mul100 += pbm[i].vref_low_marg_volt_mul100;

		printf("%6u%s %11u.%03ups %11u.%03ups %11u.%02u%% %11u.%02u%%\n",
			pbm[i].bytelane_num,
			pbm[i].is_x4_other ? "(x4)" : "    ",
			pbm[i].central_left_marg_fs / 1000,
			pbm[i].central_left_marg_fs % 1000,
			pbm[i].central_right_marg_fs / 1000,
			pbm[i].central_right_marg_fs % 1000,
			(pbm[i].vref_high_marg_volt_mul100 / 100),
			(pbm[i].vref_high_marg_volt_mul100 % 100),
			(pbm[i].vref_low_marg_volt_mul100 / 100),
			(pbm[i].vref_low_marg_volt_mul100 % 100));
	}

	central_left_marg_fs /= count;;
	central_right_marg_fs /= count;
	vref_high_marg_volt_mul100 /= count;
	vref_low_marg_volt_mul100 /= count;

	printf("%10s %11u.%03ups %11u.%03ups %11u.%02u%% %11u.%02u%%\n",
		"average",
		central_left_marg_fs / 1000,
		central_left_marg_fs % 1000,
		central_right_marg_fs / 1000,
		central_right_marg_fs % 1000,
		(vref_high_marg_volt_mul100 / 100),
		(vref_high_marg_volt_mul100 % 100),
		(vref_low_marg_volt_mul100 / 100),
		(vref_low_marg_volt_mul100 % 100));
}

static uint32_t d2_write_change_vref(int vref_idx, int bl_idx, int rank_idx)
{
	uint32_t mr6_temp;
	int vref_range, vref_range_val;
	int phy_rank = 4 * rank_idx;
	uint32_t status = 0;
	uint32_t scbg_val;

	PUB_SCHCR0_t schcr0;
	/*
	 * Setup the PUB to perform MR6 commands using the
	 * Scheduler Command Registers. We are using the second
	 * table for index [0,24] for VREFDQ from 45% up to
	 * 60.60%, then we switch to the first table for index
	 * [25,73] where the index is (vref_idx - 23) covering
	 * VREFDQ from 61.30% to 92.50%.
	 */
	vref_range = (vref_idx >= 25) ? 0 : 1;

	vref_range_val = (vref_range << 6) |
			 (vref_idx - (23 * (1 - vref_range)));

	for (int j = 0; j < 1 + (dp->type == RDIMM); j++) {

		if (j == 0)
			scbg_val = 1;
		else
			scbg_val = 3;

		/*
		 * Have an outer loop which changes the mr6_temp value,
		 * and another inner loop which doesn't change anything
		 * but repeat the steps a second time when setting it.
		 * Condense these two loops into one single loop for style.
		 */
		for (int i = 0; i < 3; i++) {

			if (i < 2)
				mr6_temp = (MR(6, rank_idx) & 0xFF00) | 0x0080 |
					    vref_range_val;
			else
				mr6_temp = (MR(6, rank_idx) & 0xFF00) |
					    vref_range_val;

			SET_MEM_REG_FIELD(pub, PUB_PGCR1, pubmode, 1);
			SET_MEM_REG_FIELD(pub, PUB_SCHCR0, cmd, 1);
			SET_MEM_REG_FIELD(pub, PUB_SCHCR1, allrank, 0);
			SET_MEM_REG_FIELD(pub, PUB_SCHCR1, scbk, 2);
			SET_MEM_REG_FIELD(pub, PUB_SCHCR1, scbg, scbg_val);
			SET_MEM_REG_FIELD(pub, PUB_SCHCR1, scaddr, mr6_temp);
			SET_MEM_REG_FIELD(pub, PUB_SCHCR1, scrnk, phy_rank);
			SET_MEM_REG_FIELD(pub, PUB_SCHCR0, schtrig, 1);

			schcr0.word = 0;
			schcr0.schtrig = 1;

			if (!read_loop(pub_read, PUB_SCHCR0,
			    schcr0.word, 0, 1 * NS_PER_US, 10, NULL)) {
				ERROR("frst D2: retries exceeded\n");
				status = 1;
			}

			SET_MEM_REG_FIELD(pub, PUB_PGCR1, pubmode, 0);
			mem_config_ndelay(500);
			SET_MEM_REG_FIELD(pub, PUB_PGCR1, pubmode, 1);
			SET_MEM_REG_FIELDS(pub, PUB_SCHCR1,
				reg.scbk = 0;
				reg.scbg = 0;
				reg.scaddr = 0;
			);
			SET_MEM_REG_FIELD(pub, PUB_SCHCR0, cmd, 0);
			SET_MEM_REG_FIELD(pub, PUB_SCHCR0, schtrig, 1);

			schcr0.word = 0;
			schcr0.schtrig = 1;

			if (!read_loop(pub_read, PUB_SCHCR0,
			    schcr0.word, 0, 1 * NS_PER_US, 10, NULL)) {
				MEM_ERR(1, "sec D2: retries exceeded\n");
				status = 1;
			}
			SET_MEM_REG_FIELD(pub, PUB_PGCR1, pubmode, 0);
			mem_config_ndelay(500);
		}
	}

	return status;
}

static uint32_t d2_read_change_vref(int vref_idx, int bl_idx, int rank_idx)
{
	PUB_DXnGCR5_t dx_x_gcr5;
	PUB_DXnGCR8_t dx_x_gcr8;

	/*
	 * Change the current Vref value.
	 * Note that we are required to change the Vref value for both nibbles
	 * even though we might be just testing one of nibbles.
	 */
	dx_x_gcr5.word = pub_read(ADDR_DXn(GCR5, bl_idx));
	SET_RANK_FIELD(dx_x_gcr5, dxrefiselr, rank_idx, vref_idx);
	pub_write(ADDR_DXn(GCR5, bl_idx), dx_x_gcr5.word);

	dx_x_gcr8.word = pub_read(ADDR_DXn(GCR8, bl_idx));
	SET_RANK_FIELD(dx_x_gcr8, x4dxrefiselr, rank_idx, vref_idx);
	pub_write(ADDR_DXn(GCR8, bl_idx), dx_x_gcr8.word);

	return 0;
}

static uint32_t d2_write_change_dll(int dll_idx, int bl_idx,
				    struct per_bl_eye_data *data,
				    int is_x4_other_nibble)
{
	PUB_DXnLCDLR1_t dx_x_lcdlr1;

	dx_x_lcdlr1.word = pub_read(ADDR_DXn(LCDLR1, bl_idx));
	if (!is_x4_other_nibble)
		dx_x_lcdlr1.wdqd = dll_idx;
	else
		dx_x_lcdlr1.x4wdqd = dll_idx;
	pub_write(ADDR_DXn(LCDLR1, bl_idx), dx_x_lcdlr1.word);

	return 0;

}

static uint32_t d2_read_change_dll(int dll_idx, int bl_idx,
				   struct per_bl_eye_data *data,
				   int is_x4_other_nibble)
{
	int rdqsd_delta, rdqsnd_delta;

	PUB_DXnLCDLR3_t dx_x_lcdlr3;
	PUB_DXnLCDLR4_t dx_x_lcdlr4;

	rdqsd_delta = 0;
	rdqsnd_delta = 0;

	if (data->d2.rd.rdqsd_tr_val > data->d2.rd.rdqsnd_tr_val)
		rdqsd_delta = data->d2.rd.rdqsd_tr_val -
			      data->d2.rd.rdqsnd_tr_val;
	else
		rdqsnd_delta = data->d2.rd.rdqsnd_tr_val -
			       data->d2.rd.rdqsd_tr_val;

	/* Change the dll offset. */
	dx_x_lcdlr3.word = pub_read(ADDR_DXn(LCDLR3, bl_idx));
	dx_x_lcdlr4.word = pub_read(ADDR_DXn(LCDLR4, bl_idx));

	if (!is_x4_other_nibble) {
		dx_x_lcdlr3.rdqsd  = dll_idx + rdqsd_delta;
		dx_x_lcdlr4.rdqsnd = dll_idx + rdqsnd_delta;
	} else {
		dx_x_lcdlr3.x4rdqsd  = dll_idx + rdqsd_delta;
		dx_x_lcdlr4.x4rdqsnd = dll_idx + rdqsnd_delta;
	}

	pub_write(ADDR_DXn(LCDLR3, bl_idx), dx_x_lcdlr3.word);
	pub_write(ADDR_DXn(LCDLR4, bl_idx), dx_x_lcdlr4.word);

	return 0;
}

static uint32_t d2_read_record_bl_data(int bl_idx, int rank_idx,
				       struct per_bl_eye_data *data,
				       int is_x4_other_nibble)
{
	PUB_DXnGCR5_t dx_x_gcr5;
	PUB_DXnGCR8_t dx_x_gcr8;
	PUB_DXnMDLR0_t dx_x_mdlr0;
	PUB_DXnLCDLR3_t dx_x_lcdlr3;
	PUB_DXnLCDLR4_t dx_x_lcdlr4;

	uint32_t rdqsd_tr_res, rdqsnd_tr_res, vref_tr_res;

	data->mss_num = dp->mss_index;
	data->rank_num = rank_idx;
	data->bytelane_num = bl_idx;
	data->is_x4_other = is_x4_other_nibble;
	/*
	 * Retrieve the current values of the dll/Vref values for later
	 * restoration and plotting purposes.
	 */
	dx_x_mdlr0.word = pub_read(ADDR_DXn(MDLR0, bl_idx));

	data->bl_iprd = dx_x_mdlr0.iprd;
	data->bl_tprd = dx_x_mdlr0.tprd;
	data->bl_tap_fs = dp->tck / (2 * data->bl_iprd);

	MEM_VERB(2, "bl_tap_fs = tck /(2 * bl_iprd) =");
	MEM_VERB(2, "%llu / (2 * %u) = %u\n", dp->tck, data->bl_iprd,
					  data->bl_tap_fs);

	dx_x_lcdlr3.word = pub_read(ADDR_DXn(LCDLR3, bl_idx));
	dx_x_lcdlr4.word = pub_read(ADDR_DXn(LCDLR4, bl_idx));

	if (!is_x4_other_nibble) {
		rdqsd_tr_res = dx_x_lcdlr3.rdqsd;
		rdqsnd_tr_res = dx_x_lcdlr4.rdqsnd;
	} else {
		rdqsd_tr_res = dx_x_lcdlr3.x4rdqsd;
		rdqsnd_tr_res = dx_x_lcdlr4.x4rdqsnd;
	}
	data->d2.rd.rdqsd_tr_val = rdqsd_tr_res;
	data->d2.rd.rdqsnd_tr_val = rdqsnd_tr_res;

	dx_x_gcr5.word = pub_read(ADDR_DXn(GCR5, bl_idx));
	dx_x_gcr8.word = pub_read(ADDR_DXn(GCR8, bl_idx));

	if (!is_x4_other_nibble)
		vref_tr_res = GET_RANK_FIELD(dx_x_gcr5, dxrefiselr, rank_idx);
	else
		vref_tr_res = GET_RANK_FIELD(dx_x_gcr8, x4dxrefiselr, rank_idx);

	data->d2.rd.vref_tr_val = vref_tr_res;
	data->d2.rd.dx_x_gcr5_val = dx_x_gcr5.word;
	data->d2.rd.dx_x_gcr8_val = dx_x_gcr8.word;

	return 0;
}

static uint32_t d2_read_restore_bl_data(int bl_idx, int rank_idx,
					struct per_bl_eye_data *data,
					int is_x4_other_nibble)
{
	PUB_DXnLCDLR3_t dx_x_lcdlr3;
	PUB_DXnLCDLR4_t dx_x_lcdlr4;

	dx_x_lcdlr3.word = pub_read(ADDR_DXn(LCDLR3, bl_idx));
	dx_x_lcdlr4.word = pub_read(ADDR_DXn(LCDLR4, bl_idx));

	if (!is_x4_other_nibble) {
		dx_x_lcdlr3.rdqsd  = data->d2.rd.rdqsd_tr_val;
		dx_x_lcdlr4.rdqsnd = data->d2.rd.rdqsnd_tr_val;
	} else {
		dx_x_lcdlr3.x4rdqsd =  data->d2.rd.rdqsd_tr_val;
		dx_x_lcdlr4.x4rdqsnd = data->d2.rd.rdqsnd_tr_val;
	}
	pub_write(ADDR_DXn(LCDLR4, bl_idx), dx_x_lcdlr4.word);
	pub_write(ADDR_DXn(LCDLR3, bl_idx), dx_x_lcdlr3.word);

	pub_write(ADDR_DXn(GCR5, bl_idx), data->d2.rd.dx_x_gcr5_val);
	pub_write(ADDR_DXn(GCR8, bl_idx), data->d2.rd.dx_x_gcr8_val);

	return 0;
}

static uint32_t d2_write_record_bl_data(int bl_idx, int rank_idx,
					struct per_bl_eye_data *data,
					int is_x4_other_nibble)
{
	PUB_DXnMDLR0_t dx_x_mdlr0;
	PUB_DXnLCDLR1_t dx_x_lcdlr1;

	uint32_t wdqd_tr_res, vref_tr_res;

	data->mss_num = dp->mss_index;
	data->rank_num = rank_idx;
	data->bytelane_num = bl_idx;
	data->is_x4_other = is_x4_other_nibble;

	dx_x_mdlr0.word = pub_read(ADDR_DXn(MDLR0, bl_idx));

	data->bl_iprd = dx_x_mdlr0.iprd;
	data->bl_tprd = dx_x_mdlr0.tprd;

	data->bl_tap_fs = dp->tck / (2 * data->bl_iprd);

	MEM_VERB(2, "bl_tap_fs = tck /(2 * bl_iprd) =");
	MEM_VERB(2, "%llu / (2 * %u) = %u\n", dp->tck, data->bl_iprd,
					  data->bl_tap_fs);

	dx_x_lcdlr1.word = pub_read(ADDR_DXn(LCDLR1, bl_idx));

	if (!is_x4_other_nibble) {
		wdqd_tr_res = dx_x_lcdlr1.wdqd;
		vref_tr_res = dp->res_mr6_vref[rank_idx][bl_idx] & 0x7f;
	} else {
		wdqd_tr_res = dx_x_lcdlr1.x4wdqd;
		vref_tr_res = dp->res_mr6_vref_x4[rank_idx][bl_idx] & 0x7f;
	}
	data->d2.wr.wdqd_tr_val = wdqd_tr_res;
	data->d2.wr.vref_tr_val = vref_tr_res;

	return 0;
}

static uint32_t d2_write_restore_bl_data(int bl_idx, int rank_idx,
					 struct per_bl_eye_data *data,
					 int is_x4_other_nibble)
{
	PUB_DXnLCDLR1_t dx_x_lcdlr1;

	dx_x_lcdlr1.word = pub_read(ADDR_DXn(LCDLR1, bl_idx));
	if (!is_x4_other_nibble)
		dx_x_lcdlr1.wdqd = data->d2.wr.wdqd_tr_val;
	else
		dx_x_lcdlr1.x4wdqd = data->d2.wr.wdqd_tr_val;
	pub_write(ADDR_DXn(LCDLR1, bl_idx), dx_x_lcdlr1.word);

	return 0;
}

/* We can only restore the write Vref values per rank. */
static uint32_t d2_write_restore_rank(int rank_idx)
{
	switch (w2d_vref_rstr) {
	case W2DR_PER_DRAM:
		per_dram_addressability_vref_setup(rank_idx);
		pub_write(PUB_SCHCR0, 0);
		pub_write(PUB_SCHCR1, 0);
		break;
	case W2DR_STATIC: {
		/* This is the static Vref setup process. */
		uint32_t mr = MR_RANK(rank_idx);
		int dimm_idx = dp->dimm_num > 1 && rank_idx > 1;

		mrs_write(mr, 6, MR(6, rank_idx) | 0x80, 0, 0x4, 1);
		mem_config_ndelay(500);
		mrs_write(mr, 6, MR(6, rank_idx) | 0x80, 0, 0x4, 1);
		mem_config_ndelay(500);
		mrs_write(mr, 6, MR(6, rank_idx), 0, 0x4, 1);
		mem_config_ndelay(500);

		if (dp->type == LRDIMM || dp->type == RDIMM) {
			/* For RDIMM or LRDIMM, repeat for side B access. */
			mrs_write_inv(mr, 6, MR(6, rank_idx) | 0x80, 0, 0x4, 1,
				      RC(00, dimm_idx));
			mem_config_ndelay(500);
			mrs_write_inv(mr, 6, MR(6, rank_idx) | 0x80, 0, 0x4, 1,
				      RC(00, dimm_idx));
			mem_config_ndelay(500);
			mrs_write_inv(mr, 6, MR(6, rank_idx), 0, 0x4, 1,
				      RC(00, dimm_idx));
			mem_config_ndelay(500);
		}
		break;
	}
	case W2DR_RANK_AVG:
		/* This is the per-rank average Vref setup process. */
		return vref_rank_avg_restore(rank_idx);
		break;
	default:
		return 1;
	}
	return 0;
}

/* All the different operation for the read vs the write 2d eye plotting. */
struct d2_eye_operations {
	/* Function to change the Vref as provided. */
	uint32_t (*change_vref)(int, int, int);
	/* Function to change the DLL as provided. */
	uint32_t (*change_dll)(int, int, struct per_bl_eye_data *, int);
	/* Function to record the initial state of the bytelane(nibble). */
	uint32_t (*record_bl_data)(int, int, struct per_bl_eye_data *, int);
	/* Function to restore to the initial state of the bytelane(nibble). */
	uint32_t (*restore_bl_data)(int, int, struct per_bl_eye_data *, int);
};

static const struct d2_eye_operations d2_eye_op[2] = {
	[0] = { /* Read */
		.change_vref = d2_read_change_vref,
		.change_dll  = d2_read_change_dll,
		.record_bl_data  = d2_read_record_bl_data,
		.restore_bl_data = d2_read_restore_bl_data,
	},
	[1] = { /* Write */
		.change_vref = d2_write_change_vref,
		.change_dll  = d2_write_change_dll,
		.record_bl_data  = d2_write_record_bl_data,
		.restore_bl_data = d2_write_restore_bl_data,
	},
};

/*
 * Do the per Vref value operation of the 2D write eye plotting.
 * Writes the result of the PHY BIST to bist_result.
 * Return 1 if success or 0 if phy bist timed out.
 */
static int d2_eye_dll_ops(int bl_idx, int dll_idx, uint32_t *bist_result,
			  struct per_bl_eye_data *data, int is_x4_other_nibble,
			  int is_write)
{
	uint32_t pbist_dqmask;

	d2_eye_op[is_write].change_dll(dll_idx, bl_idx, data,
				       is_x4_other_nibble);

	/* Setup the PHY BIST mask to just one nibble if we are x4. */
	pbist_dqmask = dp->package != PACKAGE_x4 ? 0 :
		       !is_x4_other_nibble ? 0xffff0000 : 0x0000ffff;

	SET_MEM_REG_FIELD(pub, PUB_BISTMSKR2, dqmsk, pbist_dqmask);

	/* Do the actual PHY BIST and return the result. */
	if (phy_bist_run(bist_result))
		return 0;

	return 1;
}

/*
 * Do the per Vref value operation of the 2D write eye plotting.
 * Return 1 if success or 0 if phy bist timed out.
 */
static int d2_eye_vref_ops(int bl_idx, int rank_idx, int vref_idx,
			   struct per_bl_eye_data *data, int *res_2d,
			   int is_x4_other_nibble, int is_write)
{
	int max_dll = data->bl_iprd + 10 * is_write;

	if (d2_eye_op[is_write].change_vref(vref_idx, bl_idx, rank_idx))
		return 0;

	for (int dll_idx = 0;
	     dll_idx <= MIN(max_dll, D2_DLL_MAX_VAL - 1);
	     dll_idx++) {
		uint32_t bist_result;

		/* Do the per dll operations. */
		if (!d2_eye_dll_ops(bl_idx, dll_idx, &bist_result, data,
				    is_x4_other_nibble, is_write))
			return 0;

		SET_D2_BIST_RESULT(res_2d, dll_idx,
				   bist_result ? D2_BIST_FAIL : D2_BIST_PASS);
	}
	return 1;
}

/*
 * Do the per bytelane operation of the 2D write eye plotting.
 * Return 1 if success or 0 if phy bist timed out.
 */
static int d2_eye_bytelane_ops(int bl_idx,
			       int rank_idx,
			       struct per_bl_eye_data *data,
			       struct per_bytelane_margin *pbm,
			       int is_x4_other_nibble,
			       int is_write)
{
	PUB_BISTRR_t bistrr;

	int (*res_2d)[D2_DLL_INT_USED] = dbg_arrays->dlep.res_2d;

	memset(res_2d, 0, sizeof(dbg_arrays->dlep.res_2d));

	d2_eye_op[is_write].record_bl_data(bl_idx, rank_idx, data,
					   is_x4_other_nibble);

	bistrr.word = pub_read(PUB_BISTRR);
	bistrr.bdxsel = bl_idx;
	pub_write(PUB_BISTRR, bistrr.word);

	for (int vref_idx = D2_VREF_MAX_VAL - 1; vref_idx >= 0;
	     vref_idx--) {

		if (!d2_eye_vref_ops(bl_idx, rank_idx, vref_idx, data,
				     res_2d[vref_idx],
				     is_x4_other_nibble,
				     is_write))
			return 0;
	}

	/* Restore the previous DLL and Vref values for this bytelane. */
	d2_eye_op[is_write].restore_bl_data(bl_idx, rank_idx, data,
					    is_x4_other_nibble);

	if (display_eye_plot) {
		if (bl_idx == 4)
			tf_printf("byte_lane %d (CB0 ~ CB7) "
				  "2D %s representation:\n\n",
				  bl_idx,
				  is_write ? "WRITE" : "READ");
		else if (bl_idx < 4)
			tf_printf("byte_lane %d (DQ%d ~ DQ%d) "
				  "2D %s representation:\n\n",
				  bl_idx, bl_idx * 8, bl_idx * 8 + 7,
				  is_write ? "WRITE" : "READ");
		else
			tf_printf("byte_lane %d (DQ%d ~ DQ%d) "
				  "2D %s representation:\n\n",
				  bl_idx, (bl_idx - 1) * 8,
				  bl_idx * 8 - 1,
				  is_write ? "WRITE" : "READ");

		if (is_x4_other_nibble)
			tf_printf("x4 other nibble:\n");
	}
	process_eye_plot_data(*data, res_2d, is_write, pbm);

	return 1;
}

void d2_eye_plotting(int rank_idx, int is_write, int bl_map, int nb_map)
{
	struct per_bl_eye_data *data = dbg_arrays->dlep.res_vrt;
	struct per_bytelane_margin *pbm = dbg_arrays->dlep.pbm;
	int count = 0;
	int phy_rank = 4 * rank_idx;

	PUB_BISTRR_t bistrr_orig;
	PUB_BISTAR1_t bistar1;

	/* If default value of 0 is given, we display all bytelanes/nibble. */
	if (bl_map == 0)
		bl_map = (1 << BYTELANE_NUM) - 1;
	if (nb_map == 0)
		nb_map = (1 << 2) - 1;

	bistrr_orig.word = pub_read(PUB_BISTRR);

	if (eye_2d_inhvt)
		SET_MEM_REG_FIELD(pub, PUB_PGCR6, inhvt, 1);

	MEM_VERB(2, "Call phy_bist_setup\n");
	phy_bist_setup();

	bistar1.word = pub_read(PUB_BISTAR1);
	bistar1.brank = phy_rank;
	bistar1.bmrank = phy_rank;
	pub_write(PUB_BISTAR1, bistar1.word);

	pub_switch_rankidr(rank_idx, 1, 1);

	for (int bl_idx = 0; bl_idx < BYTELANE_NUM; bl_idx++) {

		if ((bl_map & (1 << bl_idx)) == 0)
			continue;

		if (((nb_map & 1) != 0)) {
			if (!d2_eye_bytelane_ops(bl_idx,rank_idx,
						 &data[count],
						 &pbm[count], 0, is_write))
				return;
			count++;
		}

		if (dp->package != PACKAGE_x4 || ((nb_map & 2) == 0))
			continue;

		if (!d2_eye_bytelane_ops(bl_idx,rank_idx,
					 &data[count],
					 &pbm[count], 1, is_write))
			return;
		count++;
	}	/* bl_index loop */

	if (is_write)
		d2_write_restore_rank(rank_idx);

	if (eye_2d_inhvt)
		SET_MEM_REG_FIELD(pub, PUB_PGCR6, inhvt, 0);

	pub_write(PUB_BISTRR, bistrr_orig.word);

	print_eye_margin_summary(pbm, data, count, is_write);
}

int do_marg_and_d2(int argc, char * const argv[])
{
	int mss_map, rank_idx, is_write, parsed;
	uint32_t reg_data;
	void (*op)(int rank_idx, int is_write, int bl_map, int bit_nb_map);
	int bl_map = 0;
	int bit_nb_map = 0;

	if (!strcmp("show_wcm", argv[0])) {
		op = show_centralization_margin;
		is_write = 1;
	} else if (!strcmp("show_rcm", argv[0])) {
		op = show_centralization_margin;
		is_write = 0;
	} else if (!strcmp("show_wsm", argv[0])) {
		op = show_skew_margin;
		is_write = 1;
	} else if (!strcmp("show_rsm", argv[0])) {
		op = show_skew_margin;
		is_write = 0;
	} else if (!strcmp("write_2d", argv[0])) {
		op = d2_eye_plotting;
		is_write = 1;
	} else if (!strcmp("read_2d", argv[0])) {
		op = d2_eye_plotting;
		is_write = 0;
	} else {
		tf_printf("Please call cmd by one of it's alias:\n"
			  "show_wcm: show write centralization margin\n"
			  "show_rcm: show read  centralization margin\n"
			  "show_wsm: show write skew margin\n"
			  "show_rsm: show read  skew margin\n"
			  "write_2d: show 2D write eye plotting\n"
			  "read_2d : show 2D read  eye plotting\n");
		return -1;
	}

	parsed = (argc >= 3) &&
	  get_arg_str(argv[1], &mss_map, "DDR<n>", 4, "DDR0", 1, "DDR1", 2,
		"ALL", 3, "all", 3) &&
	  get_arg_int(argv[2], &rank_idx, 0, 0, 0, "rank_index") &&
	  (argc < 4 ||
	   get_arg_int(argv[3], &bl_map, 1, 0, 1 << BYTELANE_NUM, "bl_map")) &&
	/* 5th argument is bit_map for show_skew_margin and nb_map for rest. */
	  (argc < 5 ||
	   get_arg_int(argv[4], &bit_nb_map, 1, 0,
			op == show_skew_margin ? 1 << 8 : 1 << 2,
			op == show_skew_margin ? "bit" : "nibble"));

	if (!parsed) {
		tf_printf("Invalid arguments, should be:\n");
		tf_printf("%s DDR<n> <rank_index> (<bl_map> <%s_map>\n",
			  argv[0],
			  op == show_skew_margin ? "bit" : "nibble");
		return -1;
	}

	for (int i = 0; i < MAX_MEM_CTRL; i++) {

		if (!(mss_map & (1 << i)))
			continue;
		if (!ddr_switch_current_mss(i))
			continue;

		if ((dp->active_ranks & (1 << rank_idx)) == 0) {
			tf_printf("Rank %d not valid present in MSS%d.\n",
				  rank_idx, i);
			tf_printf("Skipping operation on MSS %d.\n", i);
			continue;
		}

		reg_data = emc_read(EMC_MC_DDR_IF);
		emc_write(EMC_MC_DDR_IF, 0x0);

		op(rank_idx, is_write, bl_map, bit_nb_map);

		emc_write(EMC_MC_DDR_IF, reg_data);

		dp->zeroed = 0;
	}
	return 0;
}

/* Function which turns on/off the pci and verbose logging. */
int log(int argc, char * const argv[])
{
	int parsed, log_type, turn_on;
	const char *help_txt =
	"log <log_type> <on/off>\n"
	"Turns on or off the DDR log\n"
	"<log_type> : verbose : The DDR training verbose logging.\n"
	"             error   : The DDR training error logging.\n"
	"             pci     : The MSS read/write interaction log.\n"
	"<on/off>   : on      : Turn on the corresponding logging.\n"
	"             off     : Turn off the corresponding logging.\n"
	"             basic   : Only turn on basic logging if supported.\n"
	"             all     : Turn on all thorough logging if supported.\n";

	parsed = (argc == 3) &&
	  get_arg_str(argv[1], &log_type, "log_type", 4,
	    "verbose", 0, "pci", 1, "reg", 2, "error", 3) &&
	  get_arg_str(argv[2], &turn_on, "on/off", 4,
	    "all", 3, "on", 2, "basic", 1, "off", 0);

	if (!parsed) {
		tf_printf(help_txt);
		return -1;
	}

	switch (log_type) {
	case 0:
		ddr_verbose_level = turn_on;
		break;
	case 1:
		ddr_log_flag = turn_on;
		break;
	case 2:
		ddr_reg_flag = turn_on;
		break;
	case 3:
		ddr_error_level = turn_on;
		break;
	default:
		panic();
	}

	return 0;
}

/*
 * Get an number from the command line. We are not using get_arg_int here as we
 * may be receiving long numbers and get_arg_int only supports int.
 */
static uint64_t get_num(char *const arg, int *success)
{
	char *end;
	long value = strtol(arg, &end, 0);

	*success = (*end == 0);
	return value;
}

/* Get the analog parameter value from the command line. */
static enum ddr_rzq get_rzq(char *const arg, int *success)
{
	int result;

	*success = get_arg_enum(arg, &result, "ddr_rzq", NUM_RZQ,
		ddr_rzq_fields);

	return result;
}

/* Function which manages the DDR params struct. */
int ddr_params(int argc, char * const argv[])
{
/*
 * Piece of code which checks if the field specified is the one
 * given and update the corresponding parameter.
 */
#define CHECK_CHANGE_PARAM(field, type)				\
	else if (!strcmp(#field, argv[3])) {			\
		int good_val;					\
		uint64_t value;					\
								\
		value = get_##type(argv[4], &good_val);		\
		if (!good_val) {				\
			tf_printf("Not valid value.\n");	\
			return -1;				\
		}						\
		dp->field = value;				\
	}


	int parsed, mss_idx, action;
	enum {PARAM_SHOW, PARAM_CHANGE};

	parsed = (argc >= 3) &&
	  get_arg_str(argv[1], &mss_idx, "DDR<n>", 2, "DDR0", 0, "DDR1", 1) &&
	  get_arg_str(argv[2], &action, "action", 2,
	    "show", PARAM_SHOW, "change", PARAM_CHANGE);

	if (!parsed) {
		tf_printf("ddr_params DDR<n> show\n");
		tf_printf("ddr_params DDR<n> change <field> <value>\n");
		return -1;
	}
	if (!ddr_switch_current_mss(mss_idx)) {
		tf_printf("MSS%d not configured!\n", mss_idx);
		return -1;
	}

	if (action == PARAM_SHOW) {
		print_ddr_params(dp);
		return 0;
	}

	if (argc != 5) {
		tf_printf("Please specify the field and value to change.\n");
		return -1;
	}
	/* These are all the parameters we support changing. */
	CHECK_CHANGE_PARAM(system_address_mirror, num)
	CHECK_CHANGE_PARAM(wr_dbi_en, num)
	CHECK_CHANGE_PARAM(rd_dbi_en, num)
	CHECK_CHANGE_PARAM(parity_en, num)
	CHECK_CHANGE_PARAM(crc_en, num)
	CHECK_CHANGE_PARAM(mem_rtt_nom, rzq)
	CHECK_CHANGE_PARAM(mem_rtt_wr, rzq)
	CHECK_CHANGE_PARAM(mem_rtt_park, rzq)
	CHECK_CHANGE_PARAM(mem_odic, rzq)
	CHECK_CHANGE_PARAM(asr_en, num)
	CHECK_CHANGE_PARAM(mem_vref, num)
	CHECK_CHANGE_PARAM(phy_wr_drv, rzq)
	CHECK_CHANGE_PARAM(phy_rd_odt, rzq)
	CHECK_CHANGE_PARAM(phy_rd_vref, num)
	CHECK_CHANGE_PARAM(phy_rtd, num)
	CHECK_CHANGE_PARAM(ddr_tcase, num)
	CHECK_CHANGE_PARAM(ddr_clk[0], num)
	CHECK_CHANGE_PARAM(ddr_clk[1], num)
	CHECK_CHANGE_PARAM(ddr_addr_ctrl, num)
	CHECK_CHANGE_PARAM(sysrtd_max, num)
	CHECK_CHANGE_PARAM(wlrdqsg_lcdl_norm, num)
	CHECK_CHANGE_PARAM(vref_train_bypass, num)
	CHECK_CHANGE_PARAM(vref_train_host_en, num)
	CHECK_CHANGE_PARAM(vref_train_mem_en, num)
	CHECK_CHANGE_PARAM(vref_train_pdaen, num)
	CHECK_CHANGE_PARAM(vref_train_hres2, num)
	CHECK_CHANGE_PARAM(vref_train_dqres2, num)
	CHECK_CHANGE_PARAM(ddr_clk_ref, num)
	else {
		tf_printf("Field %s not found or supported.\n", argv[3]);
		return -1;
	}
	dp->trained = 0;

	return 0;
}

/* Update the given ndp based on a preset, return 1 if successful */
static int ddr_update_dp_preset(char * const preset, struct ddr_params *ndp)
{
	for (int i = 0; ddr_fixed_configs[i] != NULL; i++) {
		if(strcmp(preset, ddr_fixed_configs[i]->config) == 0) {
			ndp->dimm[0] = dp_default_values.dimm[0];
			ndp->dimm[1] = dp_default_values.dimm[1];
			ndp->dimm_num = 0;
			ddr_fixed_configs[i]->info_board(ndp);
			ddr_fixed_configs[i]->info_user(ndp);
			return 1;
		}
	}
	return 0;
}

static void ddr_show_config_simple(struct ddr_params *sdp)
{
	if (sdp->dimm_num == 0) {
		tf_printf("Empty DDR configuration\n");
		return;
	}
	if (sdp->dimm_num == 1) {
		int s = sdp->dimm[1].ranks > 0;

		tf_printf("Dimm in slot %d\n", s);
		tf_printf("  %d physical ranks\n", sdp->dimm[s].ranks);
		tf_printf("  %d logical ranks\n", sdp->dimm[s].ddr_3ds);
		tf_printf("  %s\n", ddr_density_fields[sdp->dimm[s].density]);
		tf_printf("  %s NVDIMM\n",
				sdp->dimm[s].is_nvdimm ? "is" : "not");
	} else if (sdp->dimm_num == 2){
		tf_printf("    Dimm 0\t\t    Dimm 1\n");
		tf_printf("  %d physical ranks\t  %d physical ranks\n",
			sdp->dimm[0].ranks, sdp->dimm[1].ranks);
		tf_printf("  %d logical ranks\t  %d logical ranks\n",
			sdp->dimm[0].ddr_3ds, sdp->dimm[1].ddr_3ds);
		tf_printf("  %s\t\t  %s\n",
			ddr_density_fields[sdp->dimm[0].density],
			ddr_density_fields[sdp->dimm[1].density]);
		tf_printf("  %s NVDIMM\t\t  %s NVDIMM\n",
				sdp->dimm[0].is_nvdimm ? "is" : "not",
				sdp->dimm[1].is_nvdimm ? "is" : "not");
	}
	tf_printf("DDR4 %s %s\n",
		dimm_type_fields[sdp->type],
		ddr_package_fields[sdp->package]);
	if (sdp->speed_bin == GET_VAL_FROM_SPD)
		tf_printf("Rated at %lldMHz CL %d\n",
			sdp->tck_min == 833000 ? 2400 :
			sdp->tck_min == 1071000 ? 2133 :
			2000000000 / sdp->tck_min,
			get_min_supported_cl(DDR_CEIL_DIV(sdp->taa,
					sdp->tck_min), sdp->cl_map));
	else
		tf_printf("Rated at %s\n",
			ddr_speed_bin_fields[sdp->speed_bin]);
	tf_printf("Running at tck = %lldps (%lld MHz)\n",
			sdp->tck / 1000, 2000000000 / sdp->tck);
}

/*
 * Update the DDR config based on the user input, return 1 if successful
 * or 0 if nothing entered, no match or invalid parameter.
 */
static int ddr_update_param_manual(struct ddr_params *sdp,
				   int argc, char * const argv[])
{
	int dimm, val, rc;
	int dimm_param = 0;

#define PRINT_ENUM_OPTIONS(enum_type, num)	do {		\
	for (int i = 0; i < num; i++)				\
		tf_printf("%s%s", enum_type##_fields[i],	\
			i < num - 1 ? "/" : ">\n");		\
	} while(0)

	if (argc <= 1)
		goto no_change;

	if (strcmp(argv[0], "dimm0") == 0) {
		dimm = 0;
		dimm_param = 1;
	} else if (strcmp(argv[0], "dimm1") == 0) {
		dimm = 1;
		dimm_param = 1;
	}

	if (dimm_param) {
		if (argc <= 2)
			goto no_change;

		if(strcmp(argv[1], "rank") == 0) {
			rc = get_arg_int(argv[2], &val, 1, 0, 5, "rank");
			if (!rc)
				goto no_change;
			sdp->dimm[dimm].ranks = val;
		} else if(strcmp(argv[1], "lrank") == 0) {
			rc = get_arg_int(argv[2], &val, 1, 0, 9, "lrank");
			if (!rc)
				goto no_change;
			sdp->dimm[dimm].ddr_3ds = val;
		} else if(strcmp(argv[1], "density") == 0) {
			rc = get_arg_enum(argv[2], &val, "density",
					  NUM_DENSITY, ddr_density_fields);
			if (!rc)
				goto no_change;
			sdp->dimm[dimm].density = val;
		} else if(strcmp(argv[1], "nvdimm") == 0) {
			rc = get_arg_str(argv[2], &val, "nvdimm", 2,
					"is", 1, "not", 0);
			if (!rc)
				goto no_change;
			sdp->dimm[dimm].is_nvdimm = val;
		} else
			goto no_change;
	} else {
		if(strcmp(argv[0], "type") == 0) {
			rc = get_arg_enum(argv[1], &val, "type",
					  4, dimm_type_fields);
			if (!rc)
				goto no_change;
			sdp->type = val;
		} else if(strcmp(argv[0], "package") == 0) {
			rc = get_arg_enum(argv[1], &val, "package",
					  NUM_PACKAGE, ddr_package_fields);
			if (!rc)
				goto no_change;
			sdp->package = val;
		} else if(strcmp(argv[0], "speed_bin") == 0) {
			rc = get_arg_enum(argv[1], &val, "speed_bin",
				  NUM_OF_SPEED_BIN, ddr_speed_bin_fields);
			if (!rc)
				goto no_change;
			sdp->speed_bin = val;
		} else if(strcmp(argv[0], "tck") == 0) {
			rc = get_arg_int(argv[1], &val, 0, 0, 0, "tck");
			if (!rc)
				goto no_change;
			if (argc >= 3) {
				rc = get_arg_str(argv[2], &val, "ps/fs", 2,
					"fs", val, "ps", val * 1000);
				if (!rc)
					goto no_change;
			} else if (val <= 10000)
				val *= 1000;
			sdp->tck = val;
			sdp->phy_rtd = sdp->tck * 25 / 2;
		} else
			goto no_change;
	}

	return 1;

no_change:
	tf_printf("Valid paramater changes are:\n"
		  "set <dimm0/dimm1> rank <number>\n"
		  "                  lrank <number>\n"
		  "                  density <");
	PRINT_ENUM_OPTIONS(ddr_density, NUM_DENSITY);
	tf_printf("                  nvdimm <is/not>\n"
		  "    type <");
	PRINT_ENUM_OPTIONS(dimm_type, 4);
	tf_printf("    package <");
	PRINT_ENUM_OPTIONS(ddr_package, NUM_PACKAGE);
	tf_printf("    speed_bin <");
	PRINT_ENUM_OPTIONS(ddr_speed_bin, NUM_OF_SPEED_BIN);
	tf_printf("    tck <value> [<ps/fs>]\n");

	return 0;
}

/* Function which let user manually update the DDR configuration */
int ddr_config_change(int argc, char * const argv[])
{
	struct ddr_params *np = &dbg_arrays->dcc.dp;
	struct console_instance *ci = &dbg_arrays->dcc.ci;
	char *cmd_argv[MAX_ARG_NUM];
	int cmd_argc;
	int parsed;
	int mss_idx;
	int changed = 0;

	parsed = (argc >= 2) &&
	  get_arg_str(argv[1], &mss_idx, "DDR<n>", 2, "DDR0", 0, "DDR1", 1);

	if (!parsed) {
		tf_printf("ddr_config_change DDR<n>\n");
		return -1;
	}

	/*
	 * We aren't using ddr_switch_current_mss() here as the dp struct might
	 * be empty and ddr_switch_current_mss fails.
	 */
	dp = &(dps[mss_idx]);

	memset(ci, 0, sizeof(*ci));

	*np = *dp;
	tf_printf("Changing the DDR parameter for DDR%d\n", mss_idx);

	while (1) {
		tf_printf("#>");
		get_next_cmd(ci);

		memcpy(ci->bufs[ci->buf_next], ci->buf, MAX_BUF_LEN);

		cmd_argc = process_buffer(ci->buf, ci->buf_len, cmd_argv);

		if (cmd_argc == 0)
			continue;
		if (strcmp(cmd_argv[0], "q") == 0 ||
		    strcmp(cmd_argv[0], "quit") == 0) {
			tf_printf("Change aborted\n");
			return 0;
		}
		if (strcmp(cmd_argv[0], "d") == 0 ||
		    strcmp(cmd_argv[0], "done") == 0)
			break;

		ci->buf_next = (ci->buf_next + 1) % MAX_CMD_HIS;

		if (strcmp(cmd_argv[0], "preset") == 0) {
			if (cmd_argc == 1) {
				tf_printf("preset <preset name>\n");
				tf_printf("Available presets:\n");
				for (int i = 0; ddr_fixed_configs[i] != NULL;
				     i++) {
					tf_printf("\t%s\n",
						ddr_fixed_configs[i]->config);
				}
				continue;
			}
			if (!ddr_update_dp_preset(cmd_argv[1], np))
				tf_printf("Preset [%s] not found\n",
					  cmd_argv[1]);
			else
				changed = 1;
			continue;
		} else if (strcmp(cmd_argv[0], "SPD") == 0) {
			struct ddr_params *np2 = &dbg_arrays->dcc.dp2;
			int rc0, rc1;
			int valid_spd = 0;

			tf_printf("Initializing DDR via SPD parameters...\n");
			np->dimm[0] = dp_default_values.dimm[0];
			np->dimm[1] = dp_default_values.dimm[1];
			np->dimm_num = 0;
			*np2 = *np;
			tf_printf("Reading (if any) SPD data for DIMM0...\n");
			rc0 = ddr_get_info_spd(np, np->dimm[0].spd,
				bf_sys_get_spd_default(np->dimm[0].spd, 0,
					sizeof(np->dimm[0].spd), mss_idx, 0));
			tf_printf("Reading (if any) SPD data for DIMM1...\n");
			rc1 = ddr_get_info_spd(np2, np2->dimm[0].spd,
				bf_sys_get_spd_default(np2->dimm[0].spd, 0,
					sizeof(np->dimm[0].spd), mss_idx, 1));
			switch ((rc1 << 1) | rc0) {
			case 1:  /* Dimm on slot 0 */
				valid_spd = 1;
				break;
			case 2:  /* Dimm on slot 1 */
				*np = *np2;
				np->dimm[1] = np->dimm[0];
				np->dimm[0] = dp_default_values.dimm[0];
				np->system_address_mirror =
					np->system_address_mirror << 2;
				valid_spd = 1;
				break;
			case 3:  /* Dimm on both slots */
				valid_spd = ddr_merge_spd_info(np, np2);
				break;
			default: /* No SPD */
				tf_printf("No valid SPD detected\n");
			}
			if (!valid_spd)
				continue;
			ddr_get_info_freq(dp);
			bf_sys_ddr_get_info_board_default(np);
			np->cl = get_min_supported_cl(DDR_CEIL_DIV(np->taa,
						np->tck), np->cl_map);
			changed = np->cl ? 1 : changed;
		} else if (strcmp(cmd_argv[0], "show") == 0) {
			ddr_show_config_simple(np);
		} else if (strcmp(cmd_argv[0], "set") == 0) {
			changed |= ddr_update_param_manual(np, cmd_argc - 1,
							   &cmd_argv[1]);
		} else if (strcmp(cmd_argv[0], "analog") == 0) {
			changed |= bf_sys_ddr_get_info_board_default(np);
		} else {
			tf_printf("Unknown cmd [%s]\n", cmd_argv[0]);
			tf_printf("Valid commands are:\n");
			tf_printf("preset: Use one of the preset DDR config\n");
			tf_printf("SPD: Populate DDR config via reading SPD\n");
			tf_printf("show: Show the configured DDR config\n");
			tf_printf("set: Change a field to the DDR config\n");
			tf_printf("done: Commit the changed DDR config\n");
			tf_printf("quit: Abort the changed DDR config\n");
		}
		/* Might have stall data for the dimm number after update */
		np->dimm_num = (np->dimm[0].ranks > 0) +
			       (np->dimm[1].ranks > 0);
	}

	if (!changed) {
		tf_printf("No changes made\n");
		return 0;
	}

	np->configured = 0;
	np->ddr_3ds = MAX(np->dimm[0].ddr_3ds, np->dimm[0].ddr_3ds);
	if (np->speed_bin != GET_VAL_FROM_SPD)
		ddr_get_info_speed_bin(np);
	if (!ddr_get_info_rest(np)) {
		tf_printf("Invalid settings, abort changes\n");
		return -1;
	}
	/* Update the capacity field */
	np->dimm[0].capacity_gbit = (1 << (np->dimm[0].density + 1)) *
			64 / (1 << (np->package + 2)) * np->dimm[0].ranks *
			np->dimm[0].ddr_3ds;
	np->dimm[1].capacity_gbit = (1 << (np->dimm[1].density + 1)) *
			64 / (1 << (np->package + 2)) * np->dimm[1].ranks *
			np->dimm[1].ddr_3ds;
	*dp = *np;
	dp->trained = 0;

	tf_printf("New changed updated, use [ddr_params] cmd for fine tune\n");
	return 0;
}

int ddr_sweep(int argc, char * const argv[])
{
	int parsed;
	int min, step, max;
	int mss_idx;
	size_t offset, param_size;
	uint64_t current_val;
	uint64_t original_val;
	int orig_log;
	int orig_bist;
	int do_bist = 0;
	int reconfig = 0;

#define CHECK_PARAM_PLACE(field)		\
	else if (!strcmp(#field, argv[2])) {				\
		offset = offsetof(struct ddr_params, field);		\
		param_size = sizeof(((struct ddr_params *)0)->field);	\
	}

	parsed = (argc >= 6) &&
	  get_arg_str(argv[1], &mss_idx, "DDR<n>", 2, "DDR0", 0, "DDR1", 1) &&
	  get_arg_int(argv[3], &min, 0, 0, 0, "min") &&
	  get_arg_int(argv[4], &step, 0, 0, 0, "step") &&
	  get_arg_int(argv[5], &max, 0, 0, 0, "max") &&
	  (argc < 7 || get_arg_str(argv[6], &do_bist, "bist", 1, "bist", 1));

	if (!parsed) {
		tf_printf("ddr_sweep DDR<n> field min step max [bist]\n");
		return -1;
	} else if (min >= max) {
		tf_printf("Specified min value is bigger than max\n");
		return -1;
	}
	/* Those are all the parameters which we support sweeping. */
	if (!strcmp("tck", argv[2])) {
		offset = offsetof(struct ddr_params, tck);
		param_size = sizeof(((struct ddr_params *)0)->tck);
		/*
		 * We have to reconfig the timing values each time we change the
		 * memory frequency.
		 */
		reconfig = 1;
	}
	CHECK_PARAM_PLACE(mem_rtt_nom)
	CHECK_PARAM_PLACE(mem_rtt_wr)
	CHECK_PARAM_PLACE(mem_rtt_park)
	CHECK_PARAM_PLACE(mem_odic)
	CHECK_PARAM_PLACE(asr_en)
	CHECK_PARAM_PLACE(mem_vref)
	CHECK_PARAM_PLACE(phy_wr_drv)
	CHECK_PARAM_PLACE(phy_rd_odt)
	CHECK_PARAM_PLACE(phy_rd_vref)
	CHECK_PARAM_PLACE(phy_rtd)
	CHECK_PARAM_PLACE(ddr_tcase)
	CHECK_PARAM_PLACE(ddr_clk[0])
	CHECK_PARAM_PLACE(ddr_clk[1])
	CHECK_PARAM_PLACE(ddr_addr_ctrl)
	CHECK_PARAM_PLACE(ddr_clk_ref)
	else {
		tf_printf("parameter [%s] not found or supported\n", argv[2]);
		return -1;
	}

	if (!ddr_switch_current_mss(mss_idx)) {
		tf_printf("MSS%d not configured!\n", mss_idx);
		return -1;
	}

	/* Backup the original value. */
	memcpy(&original_val, (char *)dp + offset, param_size);

	tf_printf("%s\ttrain_result%s\n", argv[2], do_bist ? "\tbist" : "");
	orig_log = ddr_error_level;
	ddr_error_level = 0;
	orig_bist = g_debug_level;
	g_debug_level = 0;

	for (current_val = min; current_val < max; current_val += step  ) {
		int result;

		memcpy((char *)dp + offset, &current_val, param_size);

		tf_printf("%llu", current_val);

		if (reconfig)
			if (!ddr_get_info_rest(dp)) {
				tf_printf("\t\tnot supported\n");
				continue;
			}

		result = ddr_do_actual_setup();

		tf_printf("\t\t%s", result ? "pass" : "fail");

		if (do_bist) {
			if (result) {
				int mc_mask = 1 << mss_idx;
				uint32_t mc_pr_mask[MAX_MEM_CTRL] = {0};

				mc_pr_mask[mss_idx] = dp->active_ranks;
				result = !run_default_bist(mc_mask, mc_pr_mask);

				tf_printf("\t\t%s", result ? "pass" : "fail");
			} else {
				tf_printf("\t\tskipped");
			}
		}
		tf_printf("\n");

		if (ctrlc()) {
			tf_printf("Terminated\n");
			break;
		}
	}

	/* Restore the original value. */
	memcpy((char *)dp + offset, &original_val, param_size);
	ddr_error_level = orig_log;
	g_debug_level = orig_bist;

	if (reconfig)
		ddr_get_info_rest(dp);
	dp->trained = 0;

	return 0;
}

/* The type of value the option should receive. */
enum param_type {
	TYPE_INT, TYPE_STR
};

/* A struct describing a valid string option value. */
struct setting_param_val {
	int  val;	/* The value stored. */
	char *name;	/* The name of this option. */
	char *desc;	/* Optional description if not NULL. */
};

/* A struct describing the values if the parameter takes str values. */
struct settings_params_str {
	int num;	/* How many options are supported. */
	const struct setting_param_val *vals; /* The options. */
};

/* A struct describing the values if the parameter takes int values. */
struct settings_params_int {
	int is_bounded;	/* True if the value is size constrained. */
	int min;	/* Min (inclusive) value if size constrained. */
	int max;	/* Max (exclusive) value if size constrained. */
};

struct setting_var {
	char *name;	/* Name of the parameter. */
	char *desc;	/* Description of the parameter. */
	int  *addr;	/* Address parameter is stored. */
	enum param_type type; /* Parameter type */
	union {
		struct settings_params_str str;
		struct settings_params_int num;
	}val;	/* Description of the valid values. */
};

/* List of parameters that we can change */
static const struct setting_var params_info[] = { {
	.name = "bist_scan_mode",
	.desc = "BIST machine Bank/Address Scanning Mode",
	.addr = &g_scan_mode,
	.type = TYPE_STR,
	.val = {.str = {
		.num = 2,
		.vals = (const struct setting_param_val[]){{
			.val = BIST_SCAN_MODE__LOGICAL_RANK,
			.name = "logical_rank",
		}, {
			.val = BIST_SCAN_MODE__RANDOM,
			.name = "random",
			.desc = "Random address scan mode",
		},},
	} },
}, {
	.name = "bist_rand_scan_seed",
	.desc = "Seed used when in BIST random scan mode",
	.addr = &g_bist_rand_scan_seed,
	.type = TYPE_INT,
	.val = {.num = {
		.is_bounded = 1,
		.min = 0,
		.max = EMC_IFC_BIST_ADDR_LFSR__SEED_MASK + 1,
	} },
}, {
	.name = "eye_2d_inhvt",
	.desc = "Enable setting INHVT when doing eye plotting",
	.addr = &eye_2d_inhvt,
	.type = TYPE_INT,
	.val = {.num = {
		.is_bounded = 1,
		.min = 0,
		.max = 2,
	} },
}, {
	.name = "bist_high_addr",
	.desc = "The high DIMM address to limit the BIST machine from writing",
	.addr = &g_bist_high_addr,
	.type = TYPE_INT,
	.val = {.num = {
		.is_bounded = 0,
	} },
}, {
	.name = "bist_low_addr",
	.desc = "The low DIMM address to limit the BIST machine from writing",
	.addr = &g_bist_low_addr,
	.type = TYPE_INT,
	.val = {.num = {
		.is_bounded = 0,
	} },
}, {
	.name = "phy_bist_cycle",
	.desc = "The burst cycle number used when doing the PHY BIST operation",
	.addr = &phy_bist_cycle,
	.type = TYPE_INT,
	.val = {.num = {
		.is_bounded = 0,
	} },
}, {
	.name = "phy_bist_loop",
	.desc = "How many times we do the PHY BIST before declaring pass",
	.addr = &phy_bist_loop,
	.type = TYPE_INT,
	.val = {.num = {
		.is_bounded = 1,
		.min = 1,
		.max = INT_MAX,
	} },
}, {
	.name = "disp_eye_plot",
	.desc = "Display the actual eye plot when running 2D eye plot",
	.addr = &display_eye_plot,
	.type = TYPE_STR,
	.val = {.str = {
		.num = 2,
		.vals = (const struct setting_param_val[]){{
			.val = 1,
			.name = "on",
			.desc = "Eye plots are displayed",
		}, {
			.val = 0,
			.name = "off",
			.desc = "Only final summary is displayed",
		},},
	} },
}, {
	.name = "w2d_vref_restore",
	.desc = "How Vref is restored after a write 2d eye plot",
	.addr = (int *)&w2d_vref_rstr,
	.type = TYPE_STR,
	.val = {.str = {
		.num = 3,
		.vals = (const struct setting_param_val[]){{
			.val = W2DR_PER_DRAM,
			.name = "per_dram",
			.desc = "Perform Per-DRAM Addressability VREF Setup",
		}, {
			.val = W2DR_STATIC,
			.name = "static_vref",
			.desc = "Perform static VREF Setup",
		}, {
			.val = W2DR_RANK_AVG,
			.name = "rank_avg",
			.desc = "Setup VREF based on the rank's average value",
		},},
	} },
}, {
	.name = "bist_verbose_level",
	.desc = "Controls the verbosity of the BIST commands",
	.addr = &g_debug_level,
	.type = TYPE_INT,
	.val = {.num = {
		.is_bounded = 1,
		.min = 0,
		.max = 12,
	} },
}, {
	.name = "nolimit_freq",
	.desc = "**Testing only** Don't limit DDR frequency",
	.addr = &ddr_nolimit_flag,
	.type = TYPE_INT,
	.val = {.num = {
		.is_bounded = 1,
		.min = 0,
		.max = 2,
	} },
}, {
	.name = "dbg_cl",
	.desc = "**Testing only** Debug CL value when no known valid",
	.addr = &debug_cl,
	.type = TYPE_INT,
	.val = {.num = {
		.is_bounded = 1,
		.min = 0,
		.max = 53,
	} },
}, {
	.name = "dbg_cwl",
	.desc = "**Testing only** Debug CWL value when no known valid",
	.addr = &debug_cwl,
	.type = TYPE_INT,
	.val = {.num = {
		.is_bounded = 1,
		.min = 0,
		.max = 64,
	} },
}, };

/* Print out the given value of the given parameter. */
static void print_current_param_val(const struct setting_var *sv)
{
	tf_printf("<%s> : %s\n", sv->name, sv->desc);
	tf_printf("\tCurrent value : ");
	if (sv->type == TYPE_INT)
		tf_printf("%d (0x%x)", *sv->addr, *sv->addr);
	else {
		int j = 0;
		while (j < sv->val.str.num &&
		       *sv->addr != sv->val.str.vals[j].val)
			j++;
		if (j == sv->val.str.num)
			tf_printf("<unknown value %d>", *sv->addr);
		else {
			tf_printf("%s", sv->val.str.vals[j].name);
			if (sv->val.str.vals[j].desc != NULL)
				tf_printf(" (%s)", sv->val.str.vals[j].desc);
		}
	}
	tf_printf("\n\n");
}

/* Print out all the valid options for an parameter. */
static void print_param_valid_vals(const struct setting_var *sv)
{
	/* First print out the parameter's current value. */
	print_current_param_val(sv);

	if (sv->type == TYPE_INT) {
		/* No need to print anything else if value is not bounded. */
		if (sv->val.num.is_bounded == 0)
			return;
		if (sv->val.num.min == INT_MIN)
			tf_printf("\tValue must be less than %d.\n",
				  sv->val.num.max);
		else if (sv->val.num.max == INT_MAX)
			tf_printf("\tValue must be greater or equal to %d.\n",
				  sv->val.num.min);
		else
			tf_printf("\tValue must be in [%d, %d).\n",
				  sv->val.num.min, sv->val.num.max);
		return;
	}
	/* sv->type == TYPE_STR */
	tf_printf("\tValid values are:\n");
	for (int i = 0; i < sv->val.str.num; i++) {
		tf_printf("\t\t%s", sv->val.str.vals[i].name);
		if (sv->val.str.vals[i].desc != NULL)
			tf_printf(" : %s", sv->val.str.vals[i].desc);
		tf_printf("\n");
	}
}

/* Find the index of the given argument in the parameter list. */
static int find_param(const struct setting_var *sv_l, int len, char * const arg)
{
	for (int i = 0; i < len; i++) {
		if (!strcmp(arg, sv_l[i].name)) {
			return i;
		}
	}
	return -1;
}

/*
 * Change the given parameter based on the entered value.
 * Return 1 if the change was successful or 0 if the argument given is invalid.
 */
static int change_param(const struct setting_var *sv, char * const arg)
{
	int idx = -1;

	if(sv->type == TYPE_INT)
		return get_arg_int(arg,
				   sv->addr,
				   sv->val.num.is_bounded,
				   sv->val.num.min,
				   sv->val.num.max,
				   sv->name);
	/* sv->type == TYPE_STR */
	for (int i = 0; i < sv->val.str.num; i++) {
		if (!strcmp(arg, sv->val.str.vals[i].name)) {
			idx = i;
			break;
		}
	}
	if (idx == -1) {
		tf_printf("Value \"%s\" not supported.\n", arg);
		return 0;
	}

	*sv->addr = sv->val.str.vals[idx].val;

	return 1;
}

/*
 * This command allows the user to change some global parameter values
 * affecting various commands in the ATF console.
 */
int settings(int argc, char * const argv[])
{
	int param_idx;

	/* Print out all the parameter values if no parameters are specified. */
	if (argc == 1) {
		tf_printf("Settings supports the following values:\n");
		for (int i = 0; i < ARRAYSIZE(params_info); i++) {
			print_current_param_val(&params_info[i]);
		}
		return 0;
	}

	/* If at least one argument is specified, it must be the param name. */
	param_idx = find_param(params_info, ARRAYSIZE(params_info), argv[1]);

	if (param_idx < 0) {
		tf_printf("Specified parameter [%s] not found.\n", argv[1]);
		tf_printf("Supported parameters are:\n");
		for (int i = 0; i < ARRAYSIZE(params_info); i++) {
			tf_printf("\t%s\n", params_info[i].name);
		}
		return -1;
	}

	/*
	 * If only one arguement (beside function name) is specified, details
	 * about that parameter is printed out.
	 */
	if (argc == 2) {
		print_param_valid_vals(&params_info[param_idx]);
		return 0;
	}

	/* We have argc >= 3, treat it as to change the parameter values. */
	if (!change_param(&params_info[param_idx], argv[2]))
		return -1;

	return 0;
}
