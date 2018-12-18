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
#include <cassert.h>
#include <debug.h>
#include <delay_timer.h>
#include <mmio.h>
#include <string.h>
#include "bluefield_ddr.h"
#include "bluefield_ddr_print.h"
#include "bluefield_private.h"
#include "bluefield_system.h"
#include "tyu_def.h"

/* Default values for the ddr_params struct. */
const struct ddr_params default_values = {
	.dimm_num = 0,
	.dimm = { {.ddr_3ds = 1}, {.ddr_3ds = 1}},
	.speed_bin = GET_VAL_FROM_SPD,
	.tck_max = 1500000,
	.ddr_max_freq_khz = 1200000,
	.wr_dbi_en = 0,
	.rd_dbi_en = 0,
	.parity_en = 0,
	.crc_en = 0,
	.mem_rtt_nom = RZQ_DIV_1,
	.mem_rtt_wr = DISABLED,
	.mem_rtt_park = RZQ_DIV_2,
	.mem_odic = RZQ_DIV_7,
	.asr_en = 0,
	.mem_vref = 850,
	.phy_wr_drv = RZQ_DIV_7,
	.phy_rd_odt = RZQ_DIV_5,
	.phy_rd_vref = 850,
	.sys_name = TARGET_SYSTEM,
	.ddr_clk = {0, 0},
	.ddr_addr_ctrl = 0,
	.wlrdqsg_lcdl_norm = 0,
	.vref_train_bypass = 0,
	.vref_train_host_en = 1,
	.vref_train_mem_en = 1,
	.vref_train_pdaen = 0,
	.vref_train_hres2 = 0,
	.vref_train_dqres2 = 1,
	.ddr_clk_ref = BF_REF_CLK_IN_HZ,
};
/*
 * Get and return the minimum support CL bigger or equal to min_cl and
 * is supported in the cl_map. If none is supported return 0.
 */
static uint8_t get_min_supported_cl(uint8_t min_cl, uint64_t cl_map)
{
	/* The max supported CL that can be indicated is 52. */
	for (int i = min_cl; i < 53; i++)
		if ((cl_map >> i) & 1)
			return i;

	return 0;
}

/*
 * Check if the CRC value computed matches the one stored.
 * We are assuming that the last two bytes are storing the CRC value.
 * Return 1 if the CRC matches, or 0 if it doesn't.
 */
static int check_crc(const uint8_t *buf, int buflen)
{
	uint16_t exp = (buf[buflen - 1] << 8) | buf[buflen - 2];
	uint16_t act = 0;

	for (int i = 0; i < buflen - 2; i++)
	{
		act ^= buf[i] << 8;
		for (int j = 0; j < 8; j++)
			if (act & 0x8000)
				act = act << 1 ^ 0x1021;
			else
				act = act << 1;
	}

	if (act != exp) {
		MEM_ERR("SPD CRC value mismatch: Expected: 0x%x Actual: 0x%x\n",
			 exp, act);
		return 0;
	}
	return 1;
}

/*
 * Parse the given SPD data and fill in the ddr_params struct.
 * Return 1 on success or 0 on error.
 */
static int ddr_get_info_spd(struct ddr_params *dp, const uint8_t *spd, int len)
{

/* Macro for computing the timing provided the medium and fine timebases. */
#define CALC_TIMING(spd_mtb_val, spd_ftb_val) \
	FS_PER_PS * (mtb_ps * (spd_mtb_val) + ftb_ps * (int8_t)(spd_ftb_val))

/* Macro for getting the timing with index of the medium and fine timebase. */
#define	CALC_TIMING_IDX(spd_mtb_idx, spd_ftb_idx) \
	CALC_TIMING(spd[(spd_mtb_idx)], spd[(spd_ftb_idx)])

	/* Print out the content of the SPD in verbose mode. */
	if (ddr_verbose_flag) {
		tf_printf("Content of MSS%d DIMM SPD:", dp->mss_index);
		print_spd_val(len, spd);
	}

	if (len < SPD_SIZE) {
		MEM_ERR("Given SPD size too small.\n");
		return 0;
	}

	/*
	 * Now parse and validate the SPD, and extract useful data into the
	 * ddr_params struct.
	 */
	/* Verify the bytes used is at least SPD_SIZE. */
	if (((spd[0] & 0xf) * 128) < SPD_SIZE) {
		MEM_ERR("SPD size too small to contain all needed info.\n");
		return 0;
	}

	/* Verify device type. */
	if (spd[2] != 0xC) {
		MEM_ERR("Unsupported device type 0x%x\n", spd[2]);
		return 0;
	}

	/* Get the DIMM type. */
	switch (spd[3] & 0xF) {
	case 1:  /* RDIMM */
	case 5:  /* Mini-RDIMM */
	case 8:  /* 72b-SO-RDIMM */
		dp->type = RDIMM;
		break;

	case 2:  /* UDIMM */
	case 3:  /* SO_DIMM */
	case 6:  /* Mini-UDIMM */
	case 9:  /* 72b-SO-UDIMM */
	case 0xC:  /* 16b-SO-UDIMM */
	case 0xD:  /* 32b-SO-UDIMM */
		dp->type = UDIMM;
		break;

	case 4:  /* LRDIMM */
		dp->type = LRDIMM;
		break;

	default:
		MEM_ERR("Unsupported memory type 0x%x.\n", spd[3] & 0xf);
		return 0;
	}

	/* @TODO Remove this once we add support for LRDIMM. */
	if (dp->type == LRDIMM) {
		MEM_ERR("LRDIMM DIMM support currently not implemented.\n");
		return 0;
	}

	/* Also check to see if DIMM is a hybrid NVDIMM. */
	dp->dimm[0].is_nvdimm = 0;
	if ((spd[3] & 0xF0) == 0x90)
		dp->dimm[0].is_nvdimm = 1;

	/*
	 * Verify revision. Code written based on Revision
	 * UDIMM 1.1 RDIMM 1.2 LRDIMM 1.2 NVDIMM-N 1.1
	 */
	if (spd[1] >> 4 != 1) {
		MEM_ERR("Unsupported SPD revision %d.%d\n",
			  spd[1] >> 4, spd[1] & 0xf);
		return 0;
	}

	/* Verify the CRC for the SPD block by block. */
	if (!check_crc(spd, 128)) {
		MEM_ERR("SPD bytes 0~127 CRC error.\n");
		return 0;
	}
	if (len >= 256) {
		if (!check_crc(spd + 128, 128)) {
			MEM_ERR("SPD bytes 128~255 CRC error.\n");
			return 0;
		}
	}
	if (len >= 320) {
		if (!check_crc(spd + 256, 64)) {
			MEM_ERR("SPD bytes 256~319 CRC error.\n");
			return 0;
		}
	}

	dp->dimm_num = 1;

	/* Verify module nominal voltage support. */
	if ((spd[11] & 0x3) != 0x3) {
		MEM_ERR("Device operation voltage not supported. "
			"spd[11] = 0x%x\n", spd[11]);
		return 0;
	}

	/* Extract ranks. */
	dp->dimm[0].ranks = ((spd[12] >> 3) & 0x7) + 1;
	if (spd[12] & 0x40) {
		MEM_ERR("Unsupported asymmetrical configuration.\n");
		return 0;
	}

	/* Extract capacity. We only support 2, 4, 8, 16Gbit at the moment. */
	int density = spd[4] & 0xF;

	if (density > 6 || density < 3) {
		MEM_ERR("Unsupported density value %d %cb,"
			"only supports 2Gb, 4Gb, 8Gb or 16Gb.\n",
			density < 2 ? (density + 1) * 256 :
			density < 8 ? 1 << (density - 1) : (density - 7) * 12,
			density < 2 ? 'M' : 'G');
		return 0;
	}

	CASSERT(DENSITY_2Gbit == 0, density_enum_match_2Gb);
	CASSERT(DENSITY_4Gbit == 1, density_enum_match_4Gb);
	CASSERT(DENSITY_8Gbit == 2, density_enum_match_8Gb);
	CASSERT(DENSITY_16Gbit == 3, density_enum_match_16Gb);
	dp->dimm[0].density = density - 3;

	/* Extract package configuration. We only support x4, x8, x16 now. */
	if ((spd[12] & 0x7) > 2) {
		MEM_ERR("Unsupported package configuration x%d.\n",
			 1 << (dp->package + 2));
		return 0;
	}

	CASSERT(PACKAGE_x4 == 0, package_enum_match_x4);
	CASSERT(PACKAGE_x8 == 1, package_enum_match_x8);
	CASSERT(PACKAGE_x16 == 2, package_enum_match_x16);
	dp->package = spd[12] & 0x7;
	int sdram_width_bits = 1 << (dp->package + 2);

	/* Extract the die count if this is a 3DS device, else set to 1. */
	dp->dimm[0].ddr_3ds = ((spd[6] & 0x80) && ((spd[6] & 0x3) == 0x2)) ?
				((spd[6] >> 4) & 0x7) + 1 : 1;
	dp->ddr_3ds = dp->dimm[0].ddr_3ds;

	/* Extract the primary bus width. */
	int bus_width_bits = 1 << ((spd[13] & 0x7) + 3);

	/* Verify that the memory supports ECC. */
	if ((spd[13] & 0x18) != 0x8) {
		MEM_ERR("DIMM does not support ECC.\n");
		return 0;
	}

	/* Calculate the capacity of this module (in gigabits). */
	dp->dimm[0].capacity_gbit = (1 << (dp->dimm[0].density + 1)) *
			bus_width_bits / sdram_width_bits * dp->dimm[0].ranks *
			dp->dimm[0].ddr_3ds;

	/* There is only one defined medium timebase in DDR4 specs (125 ps). */
	if (((spd[17] >> 2) & 0x3) != 0x0)
		return 0;
	uint64_t mtb_ps = 125;

	/* There is only one defined fine timebase in DDR4 specs (1 ps). */
	if ((spd[17] & 0x3) != 0x0)
		return 0;
	uint64_t ftb_ps = 1;

	/* Extract minimum period. */
	dp->tck_min = CALC_TIMING_IDX(18, 125);
	dp->tck = dp->tck_min;

	/* Extract maximum period. */
	dp->tck_max = CALC_TIMING_IDX(19, 124);

	/* Extract supported CAS latencies. */
	dp->cl_map = (((uint32_t) spd[23] << 24) | ((uint32_t) spd[22] << 16) |
		     ((uint32_t) spd[21] << 8) | (uint32_t) spd[20]);
	/* Change the JEDEC CL mapping to just plain bit mapping. */
	if (dp->cl_map & (1 << 31))
		dp->cl_map = (dp->cl_map & ~(1 << 31)) << 23;
	else
		dp->cl_map = dp->cl_map << 7;

	/* Extract minimum CAS latency time. */
	dp->taa = CALC_TIMING_IDX(24, 123);

	/* Extract minimum RAS# to CAS# delay time. */
	dp->trcd = CALC_TIMING_IDX(25, 122);

	/* Extract minimum row precharge delay time. */
	dp->trp = CALC_TIMING_IDX(26, 121);

	/* Extract minimum active to precharge delay time. */
	dp->tras = CALC_TIMING(((spd[27] & 0xF) << 8) | spd[28], 0);

	/* Extract minimum active to active/refresh delay time. */
	dp->trc = CALC_TIMING(((spd[27] & 0xF0) << 4) | spd[29], spd[120]);

	/* Extract minimum refresh recovery delay time for 1x mode. */
	dp->trfc1 = CALC_TIMING((spd[31] << 8) + spd[30], 0);
	dp->trfc = dp->trfc1;

	/* Extract minimum refresh recovery delay time for 2x mode. */
	dp->trfc2 = CALC_TIMING((spd[33] << 8) + spd[32], 0);

	/* Extract minimum refresh recovery delay time for 4x mode. */
	dp->trfc4 = CALC_TIMING((spd[35] << 8) + spd[34], 0);

	/* Extract minimum four activate window delay time. */
	dp->tfaw = CALC_TIMING(((spd[36] & 0xF) << 8) | spd[37], 0);

	/*
	 * Extract minimum activate to activate delay time, different bank
	 * group.
	 */
	dp->trrd_s = CALC_TIMING_IDX(38, 119);

	/* Extract minimum activate to activate delay time, same bank group. */
	dp->trrd_l = CALC_TIMING_IDX(39, 118);

	/* Extract minimum CAS to CAS delay time, same bank group. */
	dp->tccd_l = CALC_TIMING_IDX(40, 117);

	/* Extract minimum write_to_read time, different bank group. */
	dp->twtr_s_fs = CALC_TIMING(((spd[43] & 0xF) << 8) | spd[44], 0);

	/* Extract minimum write_to_read time, same bank group. */
	dp->twtr_fs = CALC_TIMING((((spd[43] & 0xF0)) << 4) | spd[45], 0);
	dp->twtr_l_fs = dp->twtr_fs;

	/* Extract minimum write recovery time. */
	dp->twr = CALC_TIMING(((spd[41] & 0xF) << 8) | spd[42], 0);

	/* Extract the address mirroring. */
	if (dp->type == RDIMM || dp->type == LRDIMM)
		dp->system_address_mirror = (spd[136] & 1) << 1;
	else if (dp->type == UDIMM)
		dp->system_address_mirror = (spd[131] & 1) << 1;

	/*
	 * Not only the second rank, but also the fourth rank is mirrored in a
	 * quad rank DIMM setup.
	 */
	if (dp->dimm[0].ranks > 3 && dp->system_address_mirror)
		dp->system_address_mirror |= 1 << 3;

	if (ddr_verbose_flag)
		print_spd(dp);

	return 1;
}

/*
 * Merge the two spd_info structs we respectively got from the 2 SPDs we read.
 * dp1: Pointer to the first ddr_params we read and where the final merged
 * ddr_params struct is stored.
 * dp2: Pointer to the second ddr_params we read.
 * Returns 1 on success or 0 if failed (e.g. conflict when merging.)
 */
static int ddr_merge_spd_info(struct ddr_params *dp1, struct ddr_params *dp2)
{
/* Several macros only used here. */
#define	MSI_SAME(param) \
	do { \
		if (dp1->param != dp2->param) { \
			MEM_ERR("Invalid memory configuration. DIMMs on" \
				"controller %d have different values for %s" \
				"(%d != %d). Controller will be disabled.\n", \
				dp1->mss_index ,#param, \
				dp1->param, dp2->param); \
			return 0; \
		} \
	} while (0)

#define MSI_MAX(param) \
	dp1->param = MAX(dp1->param, dp2->param)

#define MSI_MIN(param) \
	dp1->param = MIN(dp1->param, dp2->param)

	MSI_SAME(type);
	dp1->dimm_num = 2;
	dp1->dimm[1] = dp2->dimm[0];
	dp1->ddr_3ds = MAX(dp1->dimm[0].ddr_3ds, dp1->dimm[1].ddr_3ds);
	MSI_SAME(package);
	MSI_MAX(tck);
	MSI_MIN(tck_max);
	MSI_MAX(trp);
	MSI_MAX(tras);
	MSI_MAX(trc);
	MSI_MAX(tccd_l);
	MSI_MAX(trrd_s);
	MSI_MAX(trrd_l);
	MSI_MAX(tfaw);
	MSI_MAX(trfc);
	MSI_MAX(trfc1);
	MSI_MAX(trfc2);
	MSI_MAX(trfc4);
	MSI_MAX(twtr_fs);
	MSI_MAX(twtr_s_fs);
	MSI_MAX(twtr_l_fs);
	MSI_MAX(twr);
	MSI_MAX(taa);
	dp1->cl_map &= dp2->cl_map;
	dp1->system_address_mirror |= dp2->system_address_mirror << 2;

	return 1;
}
/*
 * Initialize the parameters which are related to the nominal frequency but
 * can't be read directly from the SPD.
 * Return 1 on success or 0 if failed.
 */
static int ddr_get_info_freq(struct ddr_params *dp)
{
	if (dp->tck_min < 750 * FS_PER_PS)
		return 0;
	else if (dp->tck_min < 833 * FS_PER_PS)
		dp->freq = DDR4_2666;
	else if (dp->tck_min < 937 * FS_PER_PS)
		dp->freq = DDR4_2400;
	else if (dp->tck_min < 1071 * FS_PER_PS)
		dp->freq = DDR4_2133;
	else if (dp->tck_min < 1250 * FS_PER_PS)
		dp->freq = DDR4_1866;
	else if (dp->tck_min < 1500 * FS_PER_PS)
		dp->freq = DDR4_1600;
	else
		return 0;

	dp->pl = sbin_pl[dp->freq];

	dp->tccd_l_slr_fs = sbin_tccd_l_slr[dp->freq];
	dp->tccd_dlr_fs = sbin_tccd_dlr[dp->freq];
	dp->trrd_s_slr = sbin_trrd_s_slr[dp->freq];
	dp->trrd_l_slr = sbin_trrd_l_slr[dp->freq];

	dp->tfaw_slr = dp->package == PACKAGE_x4 ?
			 sbin_tfaw_slr_512b[dp->freq] :
			 dp->package == PACKAGE_x8 ?
			 sbin_tfaw_slr_1kb[dp->freq] :
			 sbin_tfaw_slr_2kb[dp->freq];

	dp->tcke = sbin_tcke[dp->freq];
	dp->txp = sbin_txp[dp->freq];
	dp->twlo = sbin_twlo[dp->freq];
	dp->tdqsck = sbin_tdqsck[dp->freq];
	dp->tdllk = sbin_tdllk[dp->freq];

	return 1;
}

/*
 * Initialize the speed bin parameters based on the speed bin class and using
 * the hardcoded lookup tables based on the JEDEC DDR4 specs.
 * Return 1 on success or 0 if failed.
 */
int ddr_get_info_speed_bin(struct ddr_params *dp)
{
	if (dp->tck < 750 * FS_PER_PS) {
		return 0;
	} else if (dp->tck < 833 * FS_PER_PS) {
		dp->cl = sbin_cl_0_750[dp->speed_bin];
	} else if (dp->tck < 937 * FS_PER_PS) {
		dp->cl = sbin_cl_0_833[dp->speed_bin];
	} else if (dp->tck < 1071 * FS_PER_PS) {
		dp->cl = sbin_cl_0_937[dp->speed_bin];
	} else if (dp->tck < 1250 * FS_PER_PS) {
		dp->cl = sbin_cl_1_071[dp->speed_bin];
	} else if (dp->tck < 1500 * FS_PER_PS) {
		dp->cl = sbin_cl_1_250[dp->speed_bin];
	} else {
		MEM_ERR("tck value %lld fs not supported.\n", dp->tck);
		return 0;
	}

	if (dp->cl == 0) {
		MEM_ERR("tck value %lld fs not valid for specified speed bin.\n",
			dp->tck);
		return 0;
	}

	switch (dp->speed_bin) {
	case DDR4_1600J:
	case DDR4_1600K:
	case DDR4_1600L:
	case DDR4_1600K_E:
		dp->freq = DDR4_1600;
		dp->tck_min = 1250 * FS_PER_PS;
		break;
	case DDR4_1866L:
	case DDR4_1866M:
	case DDR4_1866N:
	case DDR4_1866M_E:
		dp->freq = DDR4_1866;
		dp->tck_min = 1071 * FS_PER_PS;
		break;
	case DDR4_2133N:
	case DDR4_2133P:
	case DDR4_2133R:
	case DDR4_2133P_E:
		dp->freq = DDR4_2133;
		dp->tck_min = 937 * FS_PER_PS;
		break;
	case DDR4_2400P:
	case DDR4_2400R:
	case DDR4_2400T:
	case DDR4_2400U:
	case DDR4_2400T_E:
		dp->freq = DDR4_2400;
		dp->tck_min = 833 * FS_PER_PS;
		break;
	case DDR4_2666T:
	case DDR4_2666U:
	case DDR4_2666V:
	case DDR4_2666W:
		dp->freq = DDR4_2666;
		dp->tck_min = 750 * FS_PER_PS;
		break;
	default:
		MEM_ERR("Speed bin not supported.\n");
		return 0;
	}

	/* The CL value increases for the same speed bin when is 3DS. */
	if (dp->ddr_3ds > 1) {
		if (dp->tck_min != dp->tck) {
			MEM_ERR("Reduced frequency speed bin CL data not "
				"currently implemented for 3DS DIMMs.\n");
			return 0;
		}
		switch (dp->speed_bin) {
		case DDR4_2400T: /* DDR4-2400T-3DS2A */
			dp->cl = 19;
			break;
		case DDR4_2666V: /* DDR4-2666V-3DS3A */
			dp->cl = 22;
			break;
		default:
			MEM_ERR("Speed bin for 3DS device not supported.\n");
			return 0;
		}
	}

	dp->trcd = sbin_trcd[dp->speed_bin];
	dp->trp = sbin_trp[dp->speed_bin];
	dp->tras = sbin_tras[dp->freq];
	dp->trc = sbin_trc[dp->speed_bin];
	dp->tccd_l = sbin_tccd_l[dp->freq];

	dp->trrd_s = dp->package == PACKAGE_x4 ? sbin_trrd_s_512b[dp->freq] :
		     dp->package == PACKAGE_x8 ? sbin_trrd_s_1kb[dp->freq] :
		     sbin_trrd_s_2kb[dp->freq];

	dp->trrd_l = dp->package == PACKAGE_x4 ? sbin_trrd_l_512b[dp->freq] :
		     dp->package == PACKAGE_x8 ? sbin_trrd_l_1kb[dp->freq] :
		     sbin_trrd_l_2kb[dp->freq];

	dp->tfaw = dp->package == PACKAGE_x4 ? sbin_tfaw_512b[dp->freq] :
		   dp->package == PACKAGE_x8 ? sbin_tfaw_1kb[dp->freq] :
		   sbin_tfaw_2kb[dp->freq];

	int density_max = MAX(dp->dimm[0].density, dp->dimm[1].density);
	dp->trfc = sbin_trfc[density_max];
	dp->trfc1 = sbin_trfc1[density_max];
	dp->trfc2 = sbin_trfc2[density_max];
	dp->trfc4 = sbin_trfc4[density_max];

	dp->twtr_fs = 7500 * FS_PER_PS;
	dp->twtr_s_fs = 2500 * FS_PER_PS;
	dp->twtr_l_fs = 7500 * FS_PER_PS;
	dp->twr = 15000 * FS_PER_PS;

	return ddr_get_info_freq(dp);
}
/*
 * Initialize the rest of the parameters which are just calculations of the
 * existing parameters.
 * Return 1 on success or 0 if failed.
 */
int ddr_get_info_rest(struct ddr_params *dp)
{

#define MIN_TCK(param, tck_num)			do {		\
	if (dp->bak_##param == 0)				\
		dp->bak_##param = dp->param;			\
	dp->param = MAX(dp->bak_##param, (tck_num) * dp->tck);	\
} while(0)

	if (dp->tck > dp->tck_max || dp->tck < dp->tck_min ||
	    dp->tck < MAX(dp->tck, FS_PER_MS / dp->ddr_max_freq_khz)) {
		MEM_ERR("Specified tck value of %lld fs not supported by DIMM. "
			"Supported region: [%lld, %lld] fs.\n",
			dp->tck,
			MAX(dp->tck_min, FS_PER_MS / dp->ddr_max_freq_khz),
			dp->tck_max);
		return 0;
	}

	if (dp->tck < 750 * FS_PER_PS) {
		MEM_ERR("tck value too low (%lld fs).\n", dp->tck);
		return 0;
	} else if (dp->tck < 833 * FS_PER_PS)
		dp->freq = DDR4_2666;
	else if (dp->tck < 937 * FS_PER_PS)
		dp->freq = DDR4_2400;
	else if (dp->tck < 1071 * FS_PER_PS)
		dp->freq = DDR4_2133;
	else if (dp->tck < 1250 * FS_PER_PS)
		dp->freq = DDR4_1866;
	else if (dp->tck < 1500 * FS_PER_PS)
		dp->freq = DDR4_1600;
	else {
		MEM_ERR("tck value too high (%lld fs).\n", dp->tck);
		return 0;
	}

	/*
	 * We know the exact frequency the memory should be running when the
	 * following tcks are specified, thus we use the exact value for these.
	 */
	if (dp->tck == 833 * FS_PER_PS)
		dp->tck = 833333;
	else if (dp->tck == 937 * FS_PER_PS)
		dp->tck = 937500;
	else if (dp->tck == 1071 * FS_PER_PS)
		dp->tck = 1071500;

	if (dp->tck_min < 833 * FS_PER_PS)
		dp->bank_rdy_pre_rff = dp->tck > 770 * FS_PER_PS;
	else if (dp->tck_min < 937 * FS_PER_PS)
		dp->bank_rdy_pre_rff = dp->tck > 850 * FS_PER_PS;
	else if (dp->tck_min < 1071 * FS_PER_PS)
		dp->bank_rdy_pre_rff = dp->tck > 960 * FS_PER_PS;
	else if (dp->tck_min < 1250 * FS_PER_PS)
		dp->bank_rdy_pre_rff = dp->tck > 1100 * FS_PER_PS;
	else if (dp->tck_min < 1500 * FS_PER_PS)
		dp->bank_rdy_pre_rff = dp->tck > 1300 * FS_PER_PS;

	/*
	 * On some SPDs encountered, the value for twr and twtr are specified
	 * with all zeros. There is only one valid value for each of these
	 * parameters and thus the vendor of the DIMM might just ignore setting
	 * them in the SPD. In such case, just give it the only valid value.
	 */
	if (dp->twtr_fs == 0)
		dp->twtr_fs = 7500 * FS_PER_PS;
	if (dp->twtr_s_fs == 0)
		dp->twtr_s_fs = 2500 * FS_PER_PS;
	if (dp->twtr_l_fs == 0)
		dp->twtr_l_fs = 7500 * FS_PER_PS;
	if (dp->twr == 0)
		dp->twr = 15000 * FS_PER_PS;

	dp->twtr = MAX(4ull, DDR_CEIL_DIV(dp->twtr_fs, dp->tck));
	dp->twtr_s = MAX(2ull, DDR_CEIL_DIV(dp->twtr_s_fs, dp->tck));
	dp->twtr_l = MAX(4ull,  DDR_CEIL_DIV(dp->twtr_l_fs, dp->tck));

	dp->cwl = sbin_cwl[dp->freq];

	if (!dp->parity_en || dp->type == RDIMM || dp->type == LRDIMM)
		dp->pl = 0;

	MIN_TCK(tfaw, 16 + 4 * ((1 << dp->package) - 1));
	MIN_TCK(tccd_l, 5);
	MIN_TCK(tccd_l_slr_fs, 5);
	MIN_TCK(tccd_dlr_fs, 4);
	MIN_TCK(trrd_s, 4);
	MIN_TCK(trrd_l, 4);
	MIN_TCK(trrd_s_slr, 4);
	MIN_TCK(trrd_l_slr, 4);
	MIN_TCK(tcke, 3);
	MIN_TCK(txp, 4);

	int density_max = MAX(dp->dimm[0].density, dp->dimm[1].density);
	dp->trfc_dlr1 = sbin_trfc_dlr1[density_max];
	dp->trfc_dlr2 = sbin_trfc_dlr2[density_max];
	dp->trfc_dlr4 = sbin_trfc_dlr4[density_max];
	dp->trefi = sbin_trefi[density_max];

	dp->tccd_l_slr = DDR_CEIL_DIV(dp->tccd_l_slr_fs, dp->tck);
	dp->tccd_dlr = DDR_CEIL_DIV(dp->tccd_dlr_fs, dp->tck);
	dp->tccd_s = 4 * FS_PER_NS;
	dp->tccd_s_slr = 4 * FS_PER_NS;
	dp->trrd_dlr = 4 * FS_PER_NS;
	dp->tfaw_dlr = 16 * FS_PER_NS;
	dp->trtp = MAX(4 * dp->tck, 7500ull * FS_PER_PS);
	dp->tmrd = 8 * FS_PER_NS;
	dp->tmod = MAX(24 * dp->tck, 15ull * FS_PER_NS);
	dp->twlmrd = 40 * FS_PER_NS;
	dp->txs = 1 + DDR_CEIL_DIV((dp->trfc + 10 * FS_PER_NS), dp->tck);
	dp->trtodt = 0;
	dp->trtw = 0;
	dp->tzqcs = 128 * dp->tck;

	dp->al = dp->cl - 1 - 2 * (dp->ddr_3ds > 1);
	dp->mc_3ds_al_add = 2 * (dp->ddr_3ds > 1);

	/*
	 * Set the round trip delay value to the default value if not set
	 * already.
	 */
	if (dp->phy_rtd == 0)
		dp->phy_rtd = dp->tck * 25 / 2;

	int density_min = dp->dimm_num > 1 ? MIN(dp->dimm[0].density,
						 dp->dimm[1].density) :
			  dp->dimm[0].ranks > 0 ? dp->dimm[0].density
						: dp->dimm[1].density;
	dp->ddr_max_bank = (dp->package == PACKAGE_x16) ? 0x7 : 0xf;
	dp->ddr_max_col = 0x3FF;
	dp->ddr_max_row = sbin_ddr_max_row[density_min][dp->package];
	/* This field is only used if type is RDIMM/LRDIMM. */
	dp->rcd_cs_mode = dp->dimm[0].ranks > 3;
	dp->rcd_dimm_type = dp->type == RDIMM ? 1 : 0;
	dp->rcd_addr_lat = ((dp->type == RDIMM) && (dp->parity_en == 0)) * 4;
	dp->rdimm_ca_latency = (dp->type != RDIMM && dp->type != LRDIMM) ? 0 :
			       dp->rcd_addr_lat > 3 ? 0 : dp->rcd_addr_lat + 1;
	dp->rdimm_rd_wr_gap =
		dp->type == RDIMM ?
		CEIL_DIV(1500 * FS_PER_PS + (dp->tck / 2), dp->tck) :
		dp->type == LRDIMM ?
		CEIL_DIV(1500 * FS_PER_PS + 5 * (dp->tck / 2), dp->tck) : 0;

	/*
	 * Setting the prefered vref training parameters should only be done the
	 * first time. If this functions is later called indirectly via the
	 * ATF console, the changes of the user made Vref settings should
	 * be preserved.
	 */
	if (dp->configured == 0) {
		dp->configured = 1;

		/* Vref_train_pdaen shoud be disabled for x8 SDRAM config.*/
		if (dp->package == PACKAGE_x8)
			dp->vref_train_pdaen = 0;

		if (dp->dimm[0].ranks + dp->dimm[1].ranks > 1) {
			dp->vref_train_hres2 = 0;
			dp->vref_train_dqres2 = 0;
		}
		/*
		 * dqres2 can only be performed at 750mV ~ 900mV where the two
		 * SDRAM Vref ranges overlap.
		 */
		if (dp->mem_vref > 900 || dp->mem_vref < 750)
			dp->vref_train_dqres2 = 0;

		/*
		 * The Host Vref training can only be performed below 975mV
		 * or it will go beyond it's seek range and result in failure.
		 */
		if (dp->phy_rd_vref > 975)
			dp->vref_train_host_en = 0;

		/*
		 * There is no need to print out the parameters when called
		 * from the ATF console.
		 */
		if (ddr_verbose_flag)
			print_ddr_params(dp);
	} else {
		/*
		 * If we are called from the console when the tck value
		 * have changed, we need to update our CL value as well.
		 * Unfortunately it would be messy to do it when the speed
		 * bin is specified and we don't have the taa value, so
		 * only update the CL value when we are reading from SPD.
		 */
		if (dp->speed_bin == GET_VAL_FROM_SPD) {
			dp->cl = get_min_supported_cl(
					DDR_CEIL_DIV(dp->taa, dp->tck),
					dp->cl_map);
			if (!dp->cl)
				return 0;

			dp->al = dp->cl - 1 - 2 * (dp->ddr_3ds > 1);
		}
	}

	return 1;
}

/*
 * Get all the ddr info needed to setup the memory controller.
 * Return 1 on success or 0 if failed.
 */
int ddr_get_info(struct ddr_params *dp, uintptr_t mss_addr, int mss_num)
{
	/* The temp ddr_params used when dealing with the second DIMM. */
	struct ddr_params dp2;
	uint32_t mem_speed;
	int spd_len;
#if MAX_DIMM_PER_MEM_CTRL != 2
#error Code assumes MAX_DIMM_PER_MEM_CTRL == 2, must fix
#endif

	/* Set ddr_params struct to default values before doing anything. */
	*dp = default_values;
	dp2 = default_values;

	dp->mss_addr = mss_addr;
	dp->mss_index = mss_num;
	dp2.mss_addr = mss_addr;
	dp2.mss_index = mss_num;

	MEM_VERB("Getting DDR parameters for MSS %d.\n", mss_num);

	/* Make sure we are not disabled before interacting with devices. */
	mem_speed = mmio_read_32(TYU_BASE_ADDRESS + TYU_MSS_SPEED_ADDR);
	mem_speed = dp->mss_index == 0 ? (mem_speed >> TYU_MSS0_SPEED_SHIFT) :
					 (mem_speed >> TYU_MSS1_SPEED_SHIFT);
	mem_speed &= TYU_MSS_SPEED_RMASK;

	/* A value of all ones means to disable the controller. */
	if (mem_speed == TYU_MSS_SPEED_RMASK)
		return 1;

	/*
	 * First populate the SPD with actual data.
	 * If the size read is 0 then it means this DIMM slot
	 * does not contain a DIMM.
	 * If only one DIMM is present, it should be in the first slot
	 * rather than the second slot.
	 */
	spd_len = bf_sys_get_spd(dp->dimm[0].spd, 0,
			sizeof(dp->dimm[0].spd), dp->mss_index, 0);

	/*
	 * We continue here even though we don't find a DIMM attached because:
	 * 1) We see if a DIMM is attached to the second slot and complain.
	 * 2) We might have a direct attached DRAM setup which doesn't have an
	 *    SPD to read, in which case we call bf_sys_ddr_get_info_user()
	 *    to get the correct parameters.
	 */
	if (spd_len != 0) {
		if (!ddr_get_info_spd(dp, dp->dimm[0].spd, spd_len)) {
			MEM_ERR("Aborting memory controller %d setup due to SPD"
				" on DIMM 0 having incompatible parameters.\n",
				mss_num);
			return 0;
		}
	} else if (ddr_verbose_flag)
		tf_printf("No SPD detected on MSS%d DIMM0\n", mss_num);

	spd_len = bf_sys_get_spd(dp2.dimm[0].spd, 0,
			sizeof(dp2.dimm[0].spd), dp->mss_index, 1);

	/* If we have a second DIMM, we merge the info into the first one. */
	if (spd_len != 0) {

		if (!ddr_get_info_spd(&dp2, dp2.dimm[0].spd, spd_len)) {
			MEM_ERR("Aborting memory controller %d setup due to SPD"
				" on DIMM 1 having incompatible parameters.\n",
				mss_num);
			return 0;
		}
		if (!dp->dimm_num) {
			tf_printf("WARNING: DIMM detected on second slot but"
				" not first may impact memory peformance!\n");
			/* When only second DIMM is populated */
			*dp = dp2;
			dp->dimm[1] = dp->dimm[0];
			dp->dimm[0] = default_values.dimm[0];
			dp->system_address_mirror =
					dp->system_address_mirror << 2;
		} else if (!ddr_merge_spd_info(dp, &dp2)) {
			MEM_ERR("Exiting memory controller %d setup.\n",
				mss_num);
			return 0;
		}
		/*
		 * We require that if there is a regular DIMM among a NVDIMM
		 * inserted then the regular DIMM is inserted in the first
		 * slot.
		 */
		if (dp->dimm[0].is_nvdimm && !dp->dimm[1].is_nvdimm) {
			ERROR("Please insert NVDIMM in slot 1 instead of 0!");
			return 0;
		}
	} else if (ddr_verbose_flag)
		tf_printf("No SPD detected on MSS%d DIMM1\n", mss_num);

	/* Get the additional timings related to the nominal frequency. */
	ddr_get_info_freq(dp);

	/* Check if the user has specified a memory speed to use. */
	if (mem_speed != 0) {
		uint64_t tck = mem_speed * 125 * FS_PER_PS;
		tck = MAX(tck, dp->tck_min);
		tck = MIN(tck, dp->tck_max);
		dp->tck = tck;
	}

	/*
	 * Make sure the frequency we are running does not exceed the max
	 * supported frequency.
	 */
	dp->tck = MAX(dp->tck, FS_PER_MS / dp->ddr_max_freq_khz);

	/*
	 * Get any parameters specific to the board and user that are not their
	 * default values.
	 */
	if (!bf_sys_ddr_get_info_board(dp)) {
		MEM_ERR("Board parameters error, "
			"abort memory controller %d setup.\n", mss_num);
		return 0;
	}
	if (!bf_sys_ddr_get_info_user(dp)) {
		MEM_ERR("User function error, "
			"abort memory controller %d setup.\n", mss_num);
		return 0;
	}

	/* No point in continuing if we don't have any memory attached. */
	if (dp->dimm_num < 1)
		return 1;

	/*
	 * We expect the user to have provided the speed bin in a directly
	 * mounted SDRAM configuration.
	 */
	if (dp->type == ONBOARD && dp->speed_bin == GET_VAL_FROM_SPD) {
		MEM_ERR("No speed bin provided for directly mounted SDRAM.\n");
		return 0;
	}

	/*
	 * If we are using the speed bin values, then overwrite the timing
	 * provided by the SPD. Else we need to calculate the CL cycle value
	 * as we don't have a speed bin table to read it out from.
	 */
	if (dp->speed_bin != GET_VAL_FROM_SPD) {
		if (!ddr_get_info_speed_bin(dp)) {
			MEM_ERR("Aborting memory controller %d setup due to"
				" incompatible speed bin option.\n", mss_num);
			return 0;
		}
	} else {
		/* Calculate the minimum supported CAS latency. */
		dp->cl = get_min_supported_cl(DDR_CEIL_DIV(dp->taa, dp->tck),
						dp->cl_map);
		if (!dp->cl) {
			MEM_ERR("No supported CL cycle with tck = %lld fs.\n",
				dp->tck);
			return 0;
		}
	}

	/*
	 * Calculate the rest of the parameters and make sure they meet the
	 * JEDEC requirements.
	 */
	if (!ddr_get_info_rest(dp)) {
		MEM_ERR("Aborting memory controller %d setup due to"
			" incompatible timing parameter.\n", mss_num);
		return 0;
	}

	MEM_VERB("DDR parameters for MSS %d acquired.\n", mss_num);

	return 1;
}

/*
 * This function should fill in all the per-board specific memory parameters.
 * ddr_params is defined in plat/mellanox/bluefield/include/bluefield_def.h
 * though you should only be modifying the parameters as shown in the example
 * below. This function is called once per memory controller after the system
 * has read the SPD of the DIMM(s) attached and populated ddr_params *dp with
 * the corresponding information and before doing any calculations and
 * memory setup. For cases where there are no SPDs attached (e.g. direct
 * attached DRAM configuration), the DIMM configurations are not set.
 * In that case, this function shouldn't rely on on it to determine the analog
 * parameters.
 * This function should return 1 on success or 0 if failed, in which case the
 * configuration for the current memory controller would be skipped.
 */
int bf_sys_ddr_get_info_board_default(struct ddr_params *dp)
{
	/*
	 * When this function is called *dp is alreadly populated. The mss_index
	 * indicates which memory controller is being set up, this can be
	 * used to only change memory configurations on a specific memory
	 * controller as follows:
	 */
	/*
	 * if (dp->mss_index != 0) {
	 *	return 1;
	 * }
	 */
	/*
	 * We skip the board configurations when no DIMMs are present.
	 * If we are hardcoding the information via ddr_get_info_user() we
	 * are expected to call this function at the end of ddr_get_info_user()
	 */
	if (dp->dimm_num == 0)
		return 1;
	/*
	 * Following are the analog parameters which are board and DDR
	 * configuration related. This should have different values for every
	 * system. This example is taken from the BlueWhale reference platform,
	 * please change it to be relavent with the working system. The
	 * information of the DIMMs are already populated (except
	 * for the case where no SPD is read), so we are using it here to
	 * determine the specific analog parameters based on the DIMM
	 * configurations.
	 */
	int dimm_num = dp->dimm_num;
	int dimm_ranks = MAX(dp->dimm[0].ranks, dp->dimm[1].ranks);

	/* For LRDIMM we are using the RDIMM 1 rank values. */
	if (dp->type == LRDIMM)
		dimm_ranks = 1;

	switch (dimm_num) {
	case 1:
		switch (dimm_ranks) {
		case 1:
			/*
			 * DDR PHY output driver impedance required value.
			 * RZQ_DIV_n means value of reference resistor divided
			 * by 7, value of n can be between 1 and 8. This value
			 * can also be DISABLED or HIGH_Z.
			 */
			dp->phy_wr_drv = RZQ_DIV_7;
			/* DDR PHY read ODT required value. */
			dp->phy_rd_odt = RZQ_DIV_5;
			/* DDR PHY read DQ VREF required value in mV. */
			dp->phy_rd_vref = 903;
			/*
			 * DDR SDRAM memory ODT. Dynamic ODT mode Write RTT
			 * required values.
			 */
			/* dp->mem_rtt_wr = DISABLED; */
			/*
			 * DDR SDRAM Memory Output Driver Impedance required
			 * value.
			 */
			dp->mem_odic = RZQ_DIV_7;
			/*
			 * DDR SDRAM memory ODT. Dynamic ODT mode Nom. RTT
			 * required values.
			 */
			dp->mem_rtt_nom = RZQ_DIV_4;
			/*
			 * DDR SDRAM memory ODT. Dynamic ODT mode Park. RTT
			 * required values.
			 */
			dp->mem_rtt_park = DISABLED;
			/*
			 * DDR SDRAM Memory Auto Self-Refresh operation
			 * supported.
			 */
			/* dp->asr_en = 0; */
			/* DDR4 SDRAM Type DQ VREF required values in mV. */
			dp->mem_vref = 870;
			break;
		case 2:
			dp->phy_wr_drv = RZQ_DIV_7;
			dp->phy_rd_odt = RZQ_DIV_5;
			dp->phy_rd_vref = 928;
			dp->mem_odic = RZQ_DIV_7;
			dp->mem_rtt_nom = RZQ_DIV_4;
			dp->mem_rtt_park = RZQ_DIV_1;
			dp->mem_vref = 903;
			break;
		case 4:
			dp->phy_wr_drv = RZQ_DIV_7;
			dp->phy_rd_odt = RZQ_DIV_5;
			dp->phy_rd_vref = 1047;
			dp->mem_odic = RZQ_DIV_7;
			dp->mem_rtt_nom = RZQ_DIV_1;
			dp->mem_rtt_park = RZQ_DIV_4;
			dp->mem_vref = 1001;
			break;
		default:
			MEM_ERR("Unsupported DIMM ranks %d\n", dimm_ranks);
			return 0;
		}
		break;
	case 2:
		switch (dimm_ranks) {
		case 1:
			dp->phy_wr_drv = RZQ_DIV_7;
			dp->phy_rd_odt = RZQ_DIV_5;
			dp->phy_rd_vref = 1001;
			dp->mem_odic = RZQ_DIV_7;
			dp->mem_rtt_nom = RZQ_DIV_2;
			dp->mem_rtt_park = RZQ_DIV_7;
			dp->mem_vref = 926;
			break;
		case 2:
			dp->phy_wr_drv = RZQ_DIV_7;
			dp->phy_rd_odt = RZQ_DIV_5;
			dp->phy_rd_vref = 1047;
			dp->mem_odic = RZQ_DIV_7;
			dp->mem_rtt_nom = RZQ_DIV_1;
			dp->mem_rtt_park = RZQ_DIV_4;
			dp->mem_vref = 1001;
			break;
		default:
			MEM_ERR("Unsupported DIMM ranks %d\n", dimm_ranks);
			return 0;
		}
		break;
	default:
		MEM_ERR("Unsupported DIMM number %d\n", dimm_num);
		return 0;
	}

	/*
	 * DDR PHY round-trip delay (for read operations).
	 * Defines the round trip delay of data ready as a function of DDR
	 * clocks and a constant maximum delay in femtoseconds.
	 */
	dp->phy_rtd = dp->tck * 25 / 2;
	/*
	 * DDR SDRAM Components maximum case temperature in Celsius (may depend
	 * on application.
	 */
	dp->ddr_tcase = 80;
	/*
	 * Board required Device CLK Delay in femtoseconds.
	 * The device ddr_clk[m] differential clock signals may require to have
	 * a delay relatively to the rest of the Address Control signals. This
	 * delay is application specific and the user may specify which Clock
	 * Signal pair may require a delay. The default is 0 fSec delay.
	 * In order to have a negative delay setup of the differential clock
	 * relatively to the rest of the Address Control signals, a delay can
	 * be added to the Address Control signals instead of the differential
	 * clock signals. This way, any combination of a delay setup can be
	 * achieved using different delay setup values.
	 */
	/* dp->ddr_clk[0] = 0; */
	/* dp->ddr_clk[1] = 0; */
	/* dp->ddr_addr_ctrl = 0; */
	return 1;
}
