/*
 * Copyright (c) 2017 - 2019, Mellanox Technologies and Contributors.
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
#include <debug.h>
#include <delay_timer.h>
#include <mmio.h>
#include <string.h>
#include "bluefield_ddr.h"
#include "bluefield_ddr_print.h"
#include "bluefield_def.h"
#include "emc.h"
#include "emi.h"
#include "pub.h"
#include "tyu_def.h"

/*
 * Values which are used but aren't formally defined in the memory
 * configuration document. The assumed values are used here.
 */
#define	RFRSH_BURST		1
#define tREFI1			(7800ULL * FS_PER_NS)
#define tREFI2			(3900ULL * FS_PER_NS)
#define tREFI4			(1950ULL * FS_PER_NS)

/* The PHY read/write delays. */
#define	PHY_RD_DEL		4
#define PHY_WR_DEL		2

/* The place where we store the DDR parameters for the current setup. */
struct ddr_params *dp;
/* Struct storing the ddr parameters. */
struct ddr_params dps[MAX_MEM_CTRL];

/* Functions for calculating the vrefdq_tr_val and _rng with the given vref */
static inline unsigned int vref_2_vrefdq(uint32_t vref)
{
	return (unsigned long)vref * 100 / 12 - 33;
}

static inline uint32_t vrefdq_2_vref(unsigned int vrefdq)
{
	return ((unsigned long)vrefdq + 33) * 12 / 100;
}

static uint32_t mr6_get_vrefdq_r1(uint32_t vref)
{
	unsigned int vrefdq = vref_2_vrefdq(vref);

	return vrefdq < 6000 ? 0 :
	       vrefdq >= 9185 ? 50 :
	       (vrefdq - 6000 + 64) / 65;
}

static uint32_t mr6_get_vrefdq_r1_inv(uint32_t t1_index)
{
	unsigned int vrefdq = t1_index == 0 ? 6000 :
			      t1_index == 50 ? 9185 :
			      65 * t1_index + 6000 - 32;

	return vrefdq_2_vref(vrefdq);
}

static uint32_t mr6_get_vrefdq_r2(uint32_t vref)
{
	unsigned int vrefdq = vref_2_vrefdq(vref);

	return vrefdq < 4500 ? 0 :
	       vrefdq >= 7684 ? 50 :
	       (vrefdq - 4500 + 64) / 65;
}

static uint32_t mr6_get_vrefdq_r2_inv(uint32_t t2_index)
{
	unsigned int vrefdq = t2_index == 0 ? 4500 :
			      t2_index == 50 ? 7684 :
			      65 * t2_index + 4500 - 32;

	return vrefdq_2_vref(vrefdq);
}

void mr6_get_vrefdq(uint32_t vref, uint32_t *vrefdq_tr_val,
		    uint32_t *vrefdq_tr_rng)
{
	uint32_t t1_index = mr6_get_vrefdq_r1(vref);
	uint32_t t2_index = mr6_get_vrefdq_r2(vref);

	unsigned int rang1_margin = MIN(0x32 - t1_index, t1_index);
	unsigned int rang2_margin = MIN(0x32 - t2_index, t2_index);

	if (rang1_margin > rang2_margin) {
		if (vrefdq_tr_val)
			*vrefdq_tr_val = t1_index;
		if (vrefdq_tr_rng)
			*vrefdq_tr_rng = 0;
	} else {
		if (vrefdq_tr_val)
			*vrefdq_tr_val = t2_index;
		if (vrefdq_tr_rng)
			*vrefdq_tr_rng = 1;
	}
}

/* The difference in actual value for host vref_dq with 54 * idx. */
static const int8_t phy_vref_diff[] = {
	0,	2,	2,	4,	3,	4,	5,	6,
	3,	4,	5,	6,	5,	7,	7,	8,
	2,	4,	4,	5,	5,	6,	6,	8,
	5,	6,	6,	7,	7,	8,	8,	9,
	-7,	-6,	-6,	-4,	-5,	-4,	-3,	-2,
	-5,	-4,	-3,	-2,	-3,	-1,	-1,	0,
	-6,	-5,	-4,	-3,	-3,	-2,	-2,	-1,
	-4,	-3,	-2,	-1,	-2,	-1,	0,	1,
};
int get_host_vref_dq_val(int vrefdq_isel)
{
	assert(vrefdq_isel >= 0 && vrefdq_isel < ARRAYSIZE(phy_vref_diff));
	return 3154 + (vrefdq_isel * 54) + phy_vref_diff[vrefdq_isel];
}

int get_host_vrefdq_isel(int phy_rd_vref)
{
	/* Estimate the approx index. */
	int val = phy_rd_vref * 100 / 16;
	int idx = (val - 3154 + (54 / 2)) / 54;
	int dif = ABS(val - get_host_vref_dq_val(idx));

	/* Then make sure it is the best index. */
	while (idx < ARRAYSIZE(phy_vref_diff) - 1 &&
	       dif > ABS(val - get_host_vref_dq_val(idx + 1))) {
		idx++;
		dif = ABS(val - get_host_vref_dq_val(idx));
	}
	while (idx > 0 &&
	       dif > ABS(val - get_host_vref_dq_val(idx - 1))) {
		idx--;
		dif = ABS(val - get_host_vref_dq_val(idx));
	}
	return idx;
}

static void dimm_reset(int reset_cmd)
{
	int gpio_pin;

	int mode0;
	int mode1;

	int functional_en0;
	int functional_en1;

	int data_out;

	gpio_pin = 1 << dp->mss_index;

	MEM_VERB(2, "%setting DIMM at side %d\n", reset_cmd ? "S" : "Res",
					       dp->mss_index);

	mmio_write_32(TYU_BASE_ADDRESS + TYU_GPIO_LOCK,
		      (0xd42f << TYU_GPIO_LOCK_SET_LOCK));

	mode0 = mmio_read_32(TYU_BASE_ADDRESS + TYU_GPIO_GPIO0 +
			     TYU_GPIO_GPIOX_MODE0_OFFSET);

	mode1 = mmio_read_32(TYU_BASE_ADDRESS + TYU_GPIO_GPIO0 +
			     TYU_GPIO_GPIOX_MODE1_OFFSET);

	MEM_VERB(2, "Previous mode0 = 0x%x mode1 = 0x%x\n", mode0, mode1);

	mmio_write_32(TYU_BASE_ADDRESS + TYU_GPIO_GPIO0 +
		      TYU_GPIO_GPIOX_MODE0_OFFSET, mode0|(gpio_pin));
	mmio_write_32(TYU_BASE_ADDRESS + TYU_GPIO_GPIO0 +
		      TYU_GPIO_GPIOX_MODE1_OFFSET, mode1 & (~(gpio_pin)));


	if (ddr_verbose_level >= 2) {
		mode0 = mmio_read_32(TYU_BASE_ADDRESS + TYU_GPIO_GPIO0 +
			     TYU_GPIO_GPIOX_MODE0_OFFSET);

		mode1 = mmio_read_32(TYU_BASE_ADDRESS + TYU_GPIO_GPIO0 +
			     TYU_GPIO_GPIOX_MODE1_OFFSET);

		tf_printf("Current mode0 = 0x%x mode1 = 0x%x\n", mode0, mode1);
	}

	functional_en0 = mmio_read_32(TYU_BASE_ADDRESS + TYU_GPIO_GPIO0 +
				      TYU_GPIO_GPIOX_FUNCTIONAL_ENABLE0_OFFSET);
	functional_en1 = mmio_read_32(TYU_BASE_ADDRESS + TYU_GPIO_GPIO0 +
				      TYU_GPIO_GPIOX_FUNCTIONAL_ENABLE1_OFFSET);


	mmio_write_32(TYU_BASE_ADDRESS + TYU_GPIO_GPIO0 +
		      TYU_GPIO_GPIOX_FUNCTIONAL_ENABLE0_OFFSET,
		      functional_en0 & ~gpio_pin);

	mmio_write_32(TYU_BASE_ADDRESS + TYU_GPIO_GPIO0 +
		      TYU_GPIO_GPIOX_FUNCTIONAL_ENABLE1_OFFSET,
		      functional_en1 & ~gpio_pin);

	mmio_write_32(TYU_BASE_ADDRESS + TYU_GPIO_LOCK,
		      0x0 << TYU_GPIO_LOCK_SET_LOCK);

	data_out = mmio_read_32(TYU_BASE_ADDRESS + TYU_GPIO_GPIO0 +
				TYU_GPIO_GPIOX_DATA_OUT_OFFSET);


	if (reset_cmd == 1)
		mmio_write_32(TYU_BASE_ADDRESS + TYU_GPIO_GPIO0 +
			      TYU_GPIO_GPIOX_DATA_OUT_OFFSET,
			      data_out | gpio_pin);
	else
		mmio_write_32(TYU_BASE_ADDRESS + TYU_GPIO_GPIO0 +
			      TYU_GPIO_GPIOX_DATA_OUT_OFFSET,
			      data_out & ~gpio_pin);


	if (ddr_verbose_level >= 2) {
		functional_en0 =
			mmio_read_32(TYU_BASE_ADDRESS + TYU_GPIO_GPIO0 +
				     TYU_GPIO_GPIOX_FUNCTIONAL_ENABLE0_OFFSET);
		functional_en1 =
			mmio_read_32(TYU_BASE_ADDRESS + TYU_GPIO_GPIO0 +
				     TYU_GPIO_GPIOX_FUNCTIONAL_ENABLE1_OFFSET);

		tf_printf("Current functional_en0 = %d functional_en1 = %d\n",
			  functional_en0, functional_en1);
	}

	mem_config_ndelay(1 * NS_PER_MS);
}


/* Set up the EMC APB interface, return 1 on success or 0 if failed.*/
static int apb_setup(void)
{
	uint64_t rand_state = 2;
	/* Setup the EMC APB interface. */
	EMC_APB_IFC_t eai = { .word = 0 };

	eai.cont_rst = 1;
	eai.cont_rl_cfg = 4;
	emc_write(EMC_APB_IFC, eai.word);
	eai.cfg_clk_div = 0;
	eai.cont_clk_div_en = 1;
	emc_write(EMC_APB_IFC, eai.word);
	mem_config_ndelay(1000);
	eai.cont_clk_en = 1;
	emc_write(EMC_APB_IFC, eai.word);
	eai.cont_rst = 0;
	emc_write(EMC_APB_IFC, eai.word);

	/* Verify that the interface is correctly setup. */
	if (pub_read(PUB_RIDR) != 0x00260311) {
		MEM_ERR(1, "EMC APB setup failed.\n");
		return 0;
	}
	MEM_VERB(2, "Finished EMC APB setup.\n");

	uint32_t temp_data0, temp_data1;
	do {
		temp_data0 = (uint32_t)(rand_gen(&rand_state) & 0xfffffff0);
		temp_data1 = (uint32_t)(rand_gen(&rand_state) & 0xfffffff0);
	} while (pub_read(PUB_GPR0) == temp_data0 ||
		 pub_read(PUB_GPR1) == temp_data1);

	pub_write(PUB_GPR0, temp_data0);
	pub_write(PUB_GPR1, temp_data1);
	uint32_t td0 = pub_read(PUB_GPR0);
	uint32_t td1 = pub_read(PUB_GPR1);
	if (td0 != temp_data0 || td1 != temp_data1) {
		MEM_ERR(1, "EMC APB interface not working correctly.\n");
		MEM_ERR(2, "GPR0 expected 0x%x got 0x%x\n", temp_data0, td0);
		MEM_ERR(2, "GPR1 expected 0x%x got 0x%x\n", temp_data1, td1);
		return 0;
	}
	MEM_VERB(2, "EMC APB interface working.\n");

	return 1;
}

/*
 * Calculate the PLL parameters given the reference frequency and requested tck
 * value based on the following formula:
 *
 *                              CORE_F + 1
 * OUT_FREQ =  IN_FREQ  * ----------------------------
 *                       (CORE_R + 1) * (CORE_OD + 1)
 *
 * We try all the possible CORE_F values and calculate the
 * corresponding CORE_R/OD value and see which one has the smallest error.
 * Note that we are calculating the values with the + 1 embedded in the value.
 * Returns 1 if we at least found something reasonable, else 0.
 */
static int calc_pll_params(uint64_t fout, uint64_t fin, uint32_t *core_f,
			   uint32_t *core_r, uint32_t *core_od)
{
	const uint64_t VCO_MIN = 700000000ULL;
	const uint64_t VCO_MAX = 3500000000ULL;
	const uint64_t REF_MIN = 427246;
	const uint64_t REF_MAX = 3500000000ULL;

	const uint32_t R_MAX = 64;
	const uint32_t F_MAX = 4096;
	const uint32_t OD_MAX = 16;

	uint64_t error = UINT64_MAX;

	uint32_t f_min = CEIL_DIV(fout, fin);

	for (uint32_t f = f_min; f <= F_MAX; f++) {
		uint32_t od, r;
		/* rod is the value of r * od. */
		uint32_t rod = CEIL_DIV(f * fin, fout);

		if (rod * REF_MAX < fin)
			continue;

		uint32_t cur_error = fout - fin * f / rod;

		/*
		 * If we come up with two solutions which yield the same error
		 * (as in case of expanding f and rod by the same factor), we
		 * should keep the later one which has bigger parameter values.
		 */
		if (error < cur_error)
			continue;
		/*
		 * We only calculate even values for od (unless it is 1).
		 * We see what is the biggest value od can be for r to also
		 * be an integer.
		 */
		uint32_t od_max = MIN(OD_MAX, (uint32_t)(VCO_MAX / fout));
		for (od = (od_max & ~1); od > 1; od -= 2) {
			if (od > rod)
				continue;
			if (rod % od == 0)
				break;
		}
		if (od == 0 || rod % od != 0)
			od = 1;

		r = rod / od;

		/* Verify that all parameters are within requirements. */
		if (r >= R_MAX)
			continue;

		uint32_t vco = fin / r * f;
		if (vco < VCO_MIN || vco > VCO_MAX)
			continue;

		if (fin < REF_MIN * r)
			continue;

		/* Update calculated values. */
		error = cur_error;
		*core_f = f - 1;
		*core_r = r - 1;
		*core_od = od - 1;
	}
	return error != UINT64_MAX;
}

static void ddr_set_dll_frequency(void)
{
	uint32_t pll_bwadj;
	uint32_t pll_cfg;
	uint32_t core_f = 0;
	uint32_t core_r = 0;
	uint32_t core_od = 0;
	uint32_t core_bwadj = 0;

	pll_bwadj = mmio_read_32(TYU_BASE_ADDRESS +
				TYU_CORE_PLL_BWADJ(dp->mss_index + 2));
	pll_cfg = mmio_read_32(TYU_BASE_ADDRESS +
				TYU_CORE_PLL_CFG(dp->mss_index + 2));

	pll_cfg = SET_CORE_PLL_FIELD(CORE_BYPASS, pll_cfg, 1);
	mmio_write_32(TYU_BASE_ADDRESS + TYU_CORE_PLL_CFG(dp->mss_index + 2),
		      pll_cfg);
	pll_cfg = SET_CORE_PLL_FIELD(CORE_PD, pll_cfg, 1);
	mmio_write_32(TYU_BASE_ADDRESS + TYU_CORE_PLL_CFG(dp->mss_index + 2),
		      pll_cfg);

	/*
	 * See if the requested frequency have optimized PLL values, if
	 * not then manually calculate the nearest value.
	 */
	if (dp->ddr_clk_ref == 156250000) {
		switch (dp->tck) {
		case 1250000:
			core_r = 24;
			core_f = 511;
			core_bwadj = 255;
			core_od = 7;
			break;
		case 1071500:
			core_r = 24;
			core_f = 447;
			core_bwadj = 223;
			core_od = 5;
			break;
		case 1034500:
			core_r = 24;
			core_f = 463;
			core_bwadj = 231;
			core_od = 5;
			break;
		case 1000000:
			core_r = 4;
			core_f = 95;
			core_bwadj = 47;
			core_od = 5;
			break;
		case 967743:
			core_r = 24;
			core_f = 495;
			core_bwadj = 245;
			core_od = 5;
			break;
		case 937500:
			core_r = 24;
			core_f = 511;
			core_bwadj = 255;
			core_od = 5;
			break;
		case 909091:
			core_r = 24;
			core_f = 527;
			core_bwadj = 263;
			core_od = 5;
			break;
		case 882353:
			core_r = 24;
			core_f = 543;
			core_bwadj = 271;
			core_od = 5;
			break;
		case 857143:
			core_r = 4;
			core_f = 111;
			core_bwadj = 55;
			core_od = 5;
			break;
		case 833333:
			core_r = 24;
			core_f = 383;
			core_bwadj = 191;
			core_od = 3;
			break;
		default:
			MEM_VERB(2, "Using custom DDR frequency\n");
			if (!calc_pll_params(500ULL * FS_PER_MS / dp->tck,
					     dp->ddr_clk_ref, &core_f,  &core_r,
					     &core_od)) {
				MEM_ERR(1, "Requested frequency unsupported.\n");
				return;
			}
			core_bwadj = core_f / 2;
		}
	} else if (dp->ddr_clk_ref == 100000000) {
		switch (dp->tck) {
		case 1250000:
			core_r = 0;
			core_f = 31;
			core_bwadj = 15;
			core_od = 7;
			break;
		case 1071500:
			core_r = 0;
			core_f = 27;
			core_bwadj = 13;
			core_od = 5;
			break;
		case 1034500:
			core_r = 2;
			core_f = 86;
			core_bwadj = 43;
			core_od = 5;
			break;
		case 1000000:
			core_r = 0;
			core_f = 29;
			core_bwadj = 14;
			core_od = 5;
			break;
		case 967743:
			core_r = 2;
			core_f = 92;
			core_bwadj = 46;
			core_od = 5;
			break;
		case 937500:
			core_r = 0;
			core_f = 31;
			core_bwadj = 15;
			core_od = 5;
			break;
		case 909091:
			core_r = 0;
			core_f = 32;
			core_bwadj = 16;
			core_od = 5;
			break;
		case 882353:
			core_r = 0;
			core_f = 33;
			core_bwadj = 16;
			core_od = 5;
			break;
		case 857143:
			core_r = 2;
			core_f = 104;
			core_bwadj = 52;
			core_od = 5;
			break;
		case 833333:
			core_r = 0;
			core_f = 35;
			core_bwadj = 8;
			core_od = 5;
			break;
		default:
			MEM_VERB(2, "Using custom DDR frequency\n");
			if (!calc_pll_params(500ULL * FS_PER_MS / dp->tck,
					     dp->ddr_clk_ref, &core_f,  &core_r,
					     &core_od)) {
				MEM_ERR(1, "Requested frequency unsupported.\n");
				return;
			}
			core_bwadj = core_f / 2;
		}
	} else {
		MEM_VERB(2, "Using custom DDR frequency\n");
		if (!calc_pll_params(500ULL * FS_PER_MS / dp->tck,
				     dp->ddr_clk_ref, &core_f,  &core_r,
				     &core_od)) {
			MEM_ERR(1, "Requested frequency unsupported.\n");
			return;
		}
		core_bwadj = core_f / 2;
	}

	MEM_VERB(2, "Setting the PLL frequency. core_f/r/od = %d/%d/%d\n",
		 core_f, core_r, core_od);
	MEM_VERB(2, "Requested frequency:%lluHz Generated frequency:%lluHz\n",
		  500ULL * FS_PER_MS / dp->tck, (uint64_t)dp->ddr_clk_ref *
		  (core_f + 1) / ((core_r + 1) * (core_od + 1)));

	pll_cfg = SET_CORE_PLL_FIELD(CORE_R, pll_cfg, core_r);
	pll_cfg = SET_CORE_PLL_FIELD(CORE_F, pll_cfg, core_f);
	pll_bwadj = SET_CORE_PLL_FIELD(CORE_BWADJ, pll_bwadj, core_bwadj);
	pll_cfg = SET_CORE_PLL_FIELD(CORE_OD, pll_cfg, core_od);

	mmio_write_32(TYU_BASE_ADDRESS + TYU_CORE_PLL_BWADJ(dp->mss_index + 2),
			pll_bwadj);
	mmio_write_32(TYU_BASE_ADDRESS + TYU_CORE_PLL_CFG(dp->mss_index + 2),
			pll_cfg);
	mem_config_ndelay(1 * NS_PER_MS);
	pll_cfg = SET_CORE_PLL_FIELD(CORE_PD, pll_cfg, 0);
	mmio_write_32(TYU_BASE_ADDRESS + TYU_CORE_PLL_CFG(dp->mss_index + 2),
			pll_cfg);
	pll_cfg = SET_CORE_PLL_FIELD(CORE_BYPASS, pll_cfg, 0);
	mmio_write_32(TYU_BASE_ADDRESS + TYU_CORE_PLL_CFG(dp->mss_index + 2),
			pll_cfg);
}

static void ddr_interface_freq_setup(void)
{
	uint32_t tyu_mss_rst;
	uint32_t tyu_mss_rst_orig;
	uint32_t rst_pin_s_umx;
	uint32_t rst_pin_g;

	if (dp->mss_index == 0) {
		rst_pin_s_umx = MSS0_DDR_MC_SRST_EN | MSS0_UMX_MC_DDR_RESET_EN;
		rst_pin_g = MSS0_DDR_MC_GRST_EN;
	} else if (dp->mss_index == 1) {
		rst_pin_s_umx = MSS1_DDR_MC_SRST_EN | MSS1_UMX_MC_DDR_RESET_EN;
		rst_pin_g = MSS1_DDR_MC_GRST_EN;
	} else {
		MEM_ERR(1, "Unsupported MSS index %d.\n", dp->mss_index);
		return;
	}
	/* a) Set the DDR DIMM's or SDRAM Systen Reset to be active. */
	dimm_reset(0);

	/* b) Set the MSS EMI and DDR PHY Reset. */
	tyu_mss_rst = mmio_read_32(TYU_BASE_ADDRESS + TYU_MSS_RESET);
	tyu_mss_rst_orig = tyu_mss_rst;
	tyu_mss_rst |= rst_pin_s_umx;
	mmio_write_32(TYU_BASE_ADDRESS + TYU_MSS_RESET, tyu_mss_rst);
	tyu_mss_rst |= rst_pin_g;
	mmio_write_32(TYU_BASE_ADDRESS + TYU_MSS_RESET, tyu_mss_rst);

	MEM_VERB(2, "Setting the MSS EMI and DDR PHY Reset.\n");

	mem_config_ndelay(500);

	/* c) Set the PLL frequency parameters. */
	ddr_set_dll_frequency();

	/* d) Release MSS EMI and DDR PHY Reset. */
	/* Restore the original value of SRST and DDR_RESET. */
	tyu_mss_rst = (tyu_mss_rst & ~rst_pin_s_umx) |
			(tyu_mss_rst_orig & rst_pin_s_umx);
	mmio_write_32(TYU_BASE_ADDRESS + TYU_MSS_RESET, tyu_mss_rst);
	MEM_VERB(2, "Release the MSS EMI and DDR PHY Reset.\n");
	mem_config_ndelay(500);
}

/* EMEM_MI performace specific setup. */
static void mem_interface_config(void)
{
	EMI_SPARE_t es = { .word = emi_read(EMI_SPARE) };
	es.val = (16 << 4) + 1;
	emi_write(EMI_SPARE, es.word);

	EMI_REQ_SEQUENCE_t eqs = { .word = emi_read(EMI_REQ_SEQUENCE) };
	eqs.num_open_req = 127;
	emi_write(EMI_REQ_SEQUENCE, eqs.word);

	/*
	 * The following registers control the Memory Controller input request
	 * arbitration (read/write from various data clients). The registers
	 * affect the overall system performance, are determined according to a
	 * specific system DDR data profile, and may change from system to
	 * system. Current values are according to system architecture data flow
	 * profile simulations.
	 */
	EMI_WR_RD_REQ_SM_t ewrrs = { .word = emi_read(EMI_WR_RD_REQ_SM) };
	ewrrs.consider_low_load = 0;
	emi_write(EMI_WR_RD_REQ_SM, ewrrs.word);

	EMI_RDWR_SM_PROFILE_0_t ersp0 = {
		.word = emi_read(EMI_RDWR_SM_PROFILE_0)
	};
	ersp0.rd_th3 = 122;
	ersp0.wr_th3 = 138;
	emi_write(EMI_RDWR_SM_PROFILE_0, ersp0.word);

	EMI_RDWR_SM_PROFILE_1_t ersp1 = {
		.word = emi_read(EMI_RDWR_SM_PROFILE_1)
	};
	ersp1.rd_th3 = 132;
	ersp1.wr_th3 = 128;
	emi_write(EMI_RDWR_SM_PROFILE_1, ersp1.word);

	EMI_RDWR_SM_PROFILE_2_t ersp2 = {
		.word = emi_read(EMI_RDWR_SM_PROFILE_2)
	};
	ersp2.rd_th3 = 142;
	ersp2.wr_th3 = 118;
	emi_write(EMI_RDWR_SM_PROFILE_2, ersp2.word);

	EMI_RDWR_SM_PROFILE_3_t ersp3 = {
		.word = emi_read(EMI_RDWR_SM_PROFILE_3)
	};
	ersp3.rd_th3 = 152;
	ersp3.wr_th3 = 108;
	emi_write(EMI_RDWR_SM_PROFILE_3, ersp3.word);

	EMI_RDWR_SM_PROFILE_4_t ersp4 = {
		.word = emi_read(EMI_RDWR_SM_PROFILE_4)
	};
	ersp4.rd_th3 = 162;
	ersp4.wr_th3 = 98;
	emi_write(EMI_RDWR_SM_PROFILE_4, ersp4.word);

	EMI_RDWR_SM_PROFILE_5_t ersp5 = {
		.word = emi_read(EMI_RDWR_SM_PROFILE_5)
	};
	ersp5.rd_th3 = 190;
	ersp5.wr_th3 = 70;
	emi_write(EMI_RDWR_SM_PROFILE_5, ersp5.word);

	EMI_RDWR_SM_PROFILE_6_t ersp6 = {
		.word = emi_read(EMI_RDWR_SM_PROFILE_6)
	};
	ersp6.rd_th3 = 210;
	ersp6.wr_th3 = 50;
	emi_write(EMI_RDWR_SM_PROFILE_6, ersp6.word);

	EMI_RDWR_SM_PROFILE_7_t ersp7 = {
		.word = emi_read(EMI_RDWR_SM_PROFILE_7)
	};
	ersp7.rd_th3 = 230;
	ersp7.wr_th3 = 30;
	emi_write(EMI_RDWR_SM_PROFILE_7, ersp7.word);
}

/* Setup the memory controller configuration registers. */
static void mem_ctrl_config_regs(void)
{
	EMC_TIMING1_t et1 = { .word = emc_read(EMC_TIMING1) };
	et1.wra_trp_gap = dp->cwl + dp->al + dp->mc_3ds_al_add + dp->pl +
			  4 + DDR_CEIL_DIV(dp->twr + dp->trp, dp->tck);
	et1.rda_trp_gap = dp->al + dp->mc_3ds_al_add +
			  DDR_CEIL_DIV(dp->trtp + dp->trp, dp->tck);
	et1.rd_al = dp->cl + dp->al + dp->mc_3ds_al_add + dp->pl +
		    dp->rdimm_ca_latency + 12;
	et1.wr_al = dp->cwl + dp->al + dp->mc_3ds_al_add + dp->pl +
		    dp->rdimm_ca_latency;
	emc_write(EMC_TIMING1, et1.word);

	EMC_TIMING3_t et3 = { .word = emc_read(EMC_TIMING3) };
	et3.odt_duration = 6 + dp->crc_en;
	et3.trc = DDR_CEIL_DIV(dp->trc, dp->tck);
	et3.dqs_rd_en = dp->cl + dp->al + dp->mc_3ds_al_add + dp->pl +
			dp->rdimm_ca_latency - PHY_RD_DEL;
	et3.dqs_wr_en = dp->cwl + dp->al + dp->mc_3ds_al_add + dp->pl +
			dp->rdimm_ca_latency - PHY_WR_DEL;
	emc_write(EMC_TIMING3, et3.word);

	uint64_t trrd_s_select, trrd_l_select, tccd_s_select, tccd_l_select;
	if (dp->ddr_3ds == 1) {
		trrd_s_select = DDR_CEIL_DIV(dp->trrd_s, dp->tck);
		trrd_l_select = DDR_CEIL_DIV(dp->trrd_l, dp->tck);
		tccd_s_select = dp->tccd_s / FS_PER_NS;
		tccd_l_select = DDR_CEIL_DIV(dp->tccd_l, dp->tck);
	} else {
		trrd_s_select = DDR_CEIL_DIV(dp->trrd_s_slr, dp->tck);
		trrd_l_select = DDR_CEIL_DIV(dp->trrd_l_slr, dp->tck);
		tccd_s_select = dp->tccd_s_slr / FS_PER_NS;
		tccd_l_select = DDR_CEIL_DIV(dp->tccd_l_slr, dp->tck);
	}

	uint64_t tfaw_select;
	if (dp->ddr_3ds == 1)
		tfaw_select = dp->tfaw;
	else
		tfaw_select = dp->tfaw_slr;

	uint64_t trrd_l_tccd_l_select;
	if (dp->ddr_3ds == 1)
		trrd_l_tccd_l_select = MAX(trrd_l_select, tccd_l_select);
	else
		trrd_l_tccd_l_select =
			MAX(MAX(trrd_l_select, tccd_l_select),
			    MAX(dp->trrd_dlr/FS_PER_NS, dp->tccd_dlr));
	uint64_t timing4_tfaw_slr =
			CEIL_DIV(DDR_CEIL_DIV(tfaw_select, dp->tck), 2);
	EMC_TIMING4_t et4 = { .word = emc_read(EMC_TIMING4) };
	et4.act2act_gap_dlr = MAX(MAX((dp->trrd_dlr/FS_PER_NS), dp->tccd_dlr),
				  (uint64_t)(6 * dp->crc_en));
	et4.act2act_gap_s_slr =
	    MAX(CEIL_DIV(MAX(trrd_s_select, tccd_s_select), 2),
	        (uint64_t)(3 * dp->crc_en));
	et4.act2act_gap_l_slr =
	    MAX(CEIL_DIV(MAX(trrd_l_select, tccd_l_select), 2),
	        (uint64_t)3 * dp->crc_en);
	et4.tfaw_dlr = DDR_CEIL_DIV(dp->tfaw_dlr, 2 * FS_PER_NS);
	et4.tfaw_slr = timing4_tfaw_slr;
	et4.tfaw_keep_phase = timing4_tfaw_slr - 6;
	et4.keep_phase_duration =
		MAX(timing4_tfaw_slr - 6, CEIL_DIV(trrd_l_tccd_l_select, 2));
	emc_write(EMC_TIMING4, et4.word);

	uint32_t lrdimm_gap_factor = dp->type == LRDIMM ? 2 : 0;
	uint32_t sys_rtd_gap = DDR_CEIL_DIV(dp->sysrtd_max + dp->tdqsck +
					    500 * FS_PER_PS, dp->tck);
	EMC_TIMING5_t et5 = { .word = emc_read(EMC_TIMING5) };
	et5.prank_switch_gap = MAX(14u, 6 + lrdimm_gap_factor);
	et5.prank_wr_rd_gap = CEIL_DIV(dp->cwl + 4, 2);
	et5.prank_rd_wr_gap = CEIL_DIV(dp->cl + 4 - dp->cwl + 2 +
			      dp->rdimm_rd_wr_gap + sys_rtd_gap, 2);

	if (dp->dimm[0].ranks + dp->dimm[1].ranks <= 1)
		emc_write(EMC_TIMING5, 0);
	else
		emc_write(EMC_TIMING5, et5.word);

	EMC_TIMING6_t et6 = { .word = emc_read(EMC_TIMING6) };
	et6.tccd_l = CEIL_DIV(tccd_l_select, 2);
	emc_write(EMC_TIMING6, et6.word);

	EMC_RFRSH_PARAMS_1X_t erp1 = { .word = emc_read(EMC_RFRSH_PARAMS_1X) };
	erp1.trefi = (RFRSH_BURST * (tREFI1 / dp->tck)) / 2;
	erp1.trfc = CEIL_DIV(DDR_CEIL_DIV(dp->trfc1, dp->tck), 2);
	erp1.rfrsh_burst = 1;
	erp1.mode = 0;
	emc_write(EMC_RFRSH_PARAMS_1X, erp1.word);

	EMC_RFRSH_PARAMS_2X_t erp2 = { .word = emc_read(EMC_RFRSH_PARAMS_2X) };
	erp2.trefi = (RFRSH_BURST * (tREFI2 / dp->tck)) / 2;
	erp2.trfc = CEIL_DIV(DDR_CEIL_DIV(dp->trfc2, dp->tck), 2);
	erp2.rfrsh_burst = 1;
	emc_write(EMC_RFRSH_PARAMS_2X, erp2.word);

	EMC_RFRSH_PARAMS_4X_t erp4 = { .word = emc_read(EMC_RFRSH_PARAMS_4X) };
	erp4.trefi = (RFRSH_BURST * (tREFI4 / dp->tck)) / 2;
	erp4.trfc = CEIL_DIV(DDR_CEIL_DIV(dp->trfc4, dp->tck), 2);
	erp4.rfrsh_burst = 1;
	emc_write(EMC_RFRSH_PARAMS_4X, erp4.word);

	EMC_RFRSH_PARAMS_DLR_t erpd = { .word = emc_read(EMC_RFRSH_PARAMS_DLR) };
	erpd.trfc_1x = CEIL_DIV(DDR_CEIL_DIV(dp->trfc_dlr1, dp->tck), 2);
	erpd.trfc_2x = CEIL_DIV(DDR_CEIL_DIV(dp->trfc_dlr2, dp->tck), 2);
	erpd.trfc_4x = CEIL_DIV(DDR_CEIL_DIV(dp->trfc_dlr4, dp->tck), 2);
	emc_write(EMC_RFRSH_PARAMS_DLR, erpd.word);

	EMC_CALIB_PARAMS_t ecp = { .word = emc_read(EMC_CALIB_PARAMS) };
	ecp.th = 100;
	ecp.tzqcs = CEIL_DIV(dp->tzqcs, 2 * dp->tck);
	emc_write(EMC_CALIB_PARAMS, ecp.word);

	EMC_DDR_PROPERTIES_t edp = { .word = emc_read(EMC_DDR_PROPERTIES) };
	edp.package_rank_num =
		(dp->dimm[0].ranks + dp->dimm[1].ranks > 2) ? 0x2 :
		(dp->dimm[0].ranks == 0 && dp->dimm[1].ranks > 0) ? 0x2 :
		(dp->dimm[0].ranks + dp->dimm[1].ranks > 1) ? 0x1 : 0x0;
	edp.logic_rank_num = dp->ddr_3ds == 1 ? 0x0 :
			     dp->ddr_3ds == 2 ? 0x1 :
			     dp->ddr_3ds == 4 ? 0x2 : 0x3;
	edp.dq_pins = dp->package;
	edp.two_single_rank = dp->dimm[0].ranks == 1 && dp->dimm[1].ranks == 1;
	edp.single_sdram = dp->type == ONBOARD;
	edp.single_cas = 0;
	edp.al_3ds_mode = dp->ddr_3ds > 1;
	edp.wr_dbi_en = dp->wr_dbi_en;
	edp.rd_dbi_en = dp->rd_dbi_en;
	emc_write(EMC_DDR_PROPERTIES, edp.word);

	uint32_t wr_rd_gap_lo_add = ((et5.prank_switch_gap % 2) > 0) ||
	((edp.package_rank_num > 1) && (edp.logic_rank_num > 0) &&
	((et4.act2act_gap_dlr % 2) > 0));

	EMC_TIMING2_t et2 = { .word = emc_read(EMC_TIMING2) };
	et2.wr_rd_gap_sh = DDR_CEIL_DIV(dp->cwl + 4 + dp->twtr_s, 2);
	et2.wr_rd_gap_lo = DDR_CEIL_DIV(dp->cwl + 4 + dp->twtr_l, 2) +
			   wr_rd_gap_lo_add;
	et2.rd_wr_gap = DDR_CEIL_DIV(dp->cl + 4 - dp->cwl + 2 +
				     dp->rdimm_rd_wr_gap + sys_rtd_gap, 2);
	emc_write(EMC_TIMING2, et2.word);

	/*
	 * The CRC_EN bit at the DDR_DEBUG register should be set only after
	 * the Memory Controller Post DDR PHY Data Training step has completed
	 * and before the DDR Memory Controller operation is enabled.
	 */
	EMC_DDR_DEBUG_t edd = { .word = emc_read(EMC_DDR_DEBUG) };
	edd.parity_en = dp->parity_en;
	edd.parity_delay = (dp->rdimm_ca_latency != 0) &&
			   ((RC(0E, 0) & 1) == 0);
	emc_write(EMC_DDR_DEBUG, edd.word);

	uint32_t pr_bm_0 = (1 << dp->ddr_3ds) - 1;
	EMC_ACTIVE_RANKS_t ear = { .word = emc_read(EMC_ACTIVE_RANKS) };
	ear.pr_bm_0 = dp->dimm[0].ranks > 0 ? pr_bm_0 : 0;
	ear.pr_bm_1 = dp->dimm[0].ranks > 1 ? pr_bm_0 : 0;
	ear.pr_bm_2 = dp->dimm[1].ranks > 0 || dp->dimm[0].ranks > 3 ? pr_bm_0
								     : 0;
	ear.pr_bm_3 = dp->dimm[1].ranks > 1 || dp->dimm[0].ranks > 3 ? pr_bm_0
								     : 0;
	emc_write(EMC_ACTIVE_RANKS, ear.word);

	EMC_MC_QUAD_MODE_t emqm = { .word = emc_read(EMC_MC_QUAD_MODE) };
	emqm.rcd_mode = 0;
	emqm.c0_over_c2 = 0;
	emqm.odt_quad_mode = (dp->dimm[0].ranks > 2);
	emc_write(EMC_MC_QUAD_MODE, emqm.word);

	EMC_MC_MI_IF_t emmi = { .word = emc_read(EMC_MC_MI_IF) };
	emmi.ack_addr_gap = 4;
	emmi.ack_data_gap = 3;
	emmi.ret_data_gap = 1;
	emmi.bank_rdy_pre = 4 + 4 - dp->bank_rdy_pre_rff;
	emmi.mc_highest_bank = dp->package == PACKAGE_x16 ? 7 : 15;
	emc_write(EMC_MC_MI_IF, emmi.word);

	EMC_EXT_MC_LATENCY_t eeml = { .word = emc_read(EMC_EXT_MC_LATENCY) };
	eeml.ext_mc_rl = 20;
	emc_write(EMC_EXT_MC_LATENCY, eeml.word);

	EMC_MC_ADDR_MIRROR_t emam = { .word = emc_read(EMC_MC_ADDR_MIRROR) };
	emam.enable = dp->system_address_mirror;
	emc_write(EMC_MC_ADDR_MIRROR, emam.word);

	EMC_PHY_UPDATE_t epu = { .word = emc_read(EMC_PHY_UPDATE) };
	epu.mc_maxpw = 8;
	emc_write(EMC_PHY_UPDATE, epu.word);

	int total_ranks = dp->ddr_3ds * (dp->dimm[0].ranks + dp->dimm[1].ranks);
	uint32_t rfrsh_trefi_interval = (DDR_CEIL_DIV(dp->trefi, dp->tck) / 2) /
					total_ranks;
	uint32_t prank_0_en = dp->dimm[0].ranks > 0;
	uint32_t prank_1_en = dp->dimm[0].ranks > 1;
	uint32_t prank_2_en = dp->dimm[0].ranks > 3 || dp->dimm[1].ranks > 0;
	uint32_t prank_3_en = dp->dimm[0].ranks > 3 || dp->dimm[1].ranks > 1;
	uint32_t prnk_factor = dp->dimm[0].ranks > 3 ? 2 :
				MAX(dp->dimm[0].ranks, dp->dimm[1].ranks);

	EMC_RFRSH_TREFI_OFFS_t erto0 = {
		.word = emc_read(EMC_RFRSH_TREFI_OFFS__FIRST_WORD)
	};
	erto0.val_0 = prank_0_en * 1 * rfrsh_trefi_interval;
	erto0.val_1 = prank_0_en * 2 * rfrsh_trefi_interval;
	emc_write(EMC_RFRSH_TREFI_OFFS__FIRST_WORD, erto0.word);

	EMC_RFRSH_TREFI_OFFS_t erto1 = {
		.word = emc_read(EMC_RFRSH_TREFI_OFFS__FIRST_WORD + 1)
	};
	erto1.val_0 = prank_0_en * 3 * rfrsh_trefi_interval;
	erto1.val_1 = prank_0_en * 4 * rfrsh_trefi_interval;
	emc_write(EMC_RFRSH_TREFI_OFFS__FIRST_WORD + 1, erto1.word);

	EMC_RFRSH_TREFI_OFFS_t erto2 = {
		.word = emc_read(EMC_RFRSH_TREFI_OFFS__FIRST_WORD + 2)
	};
	erto2.val_0 = prank_1_en * (dp->ddr_3ds + 1) * rfrsh_trefi_interval;
	erto2.val_1 = prank_1_en * (dp->ddr_3ds + 2) * rfrsh_trefi_interval;
	emc_write(EMC_RFRSH_TREFI_OFFS__FIRST_WORD + 2, erto2.word);

	EMC_RFRSH_TREFI_OFFS_t erto3 = {
		.word = emc_read(EMC_RFRSH_TREFI_OFFS__FIRST_WORD + 3)
	};
	erto3.val_0 = prank_1_en * (dp->ddr_3ds + 3) * rfrsh_trefi_interval;
	erto3.val_1 = prank_1_en * (dp->ddr_3ds + 4) * rfrsh_trefi_interval;
	emc_write(EMC_RFRSH_TREFI_OFFS__FIRST_WORD + 3, erto3.word);

	EMC_RFRSH_TREFI_OFFS_t erto4 = {
		.word = emc_read(EMC_RFRSH_TREFI_OFFS__FIRST_WORD + 4)
	};
	erto4.val_0 = prank_2_en *
		      (prnk_factor * dp->ddr_3ds + 1) * rfrsh_trefi_interval;
	erto4.val_1 = prank_2_en *
		      (prnk_factor * dp->ddr_3ds + 2) * rfrsh_trefi_interval;
	emc_write(EMC_RFRSH_TREFI_OFFS__FIRST_WORD + 4, erto4.word);

	EMC_RFRSH_TREFI_OFFS_t erto5 = {
		.word = emc_read(EMC_RFRSH_TREFI_OFFS__FIRST_WORD + 5)
	};
	erto5.val_0 = prank_2_en * (prnk_factor * dp->ddr_3ds + 3) *
		      rfrsh_trefi_interval;
	erto5.val_1 = prank_2_en * (prnk_factor * dp->ddr_3ds + 4) *
		      rfrsh_trefi_interval;
	emc_write(EMC_RFRSH_TREFI_OFFS__FIRST_WORD + 5, erto5.word);

	EMC_RFRSH_TREFI_OFFS_t erto6 = {
		.word = emc_read(EMC_RFRSH_TREFI_OFFS__FIRST_WORD + 6)
	};
	erto6.val_0 = prank_3_en * (prnk_factor * dp->ddr_3ds +
		      dp->ddr_3ds + 1) * rfrsh_trefi_interval;
	erto6.val_1 = prank_3_en * (prnk_factor * dp->ddr_3ds +
		      dp->ddr_3ds + 2) * rfrsh_trefi_interval;
	emc_write(EMC_RFRSH_TREFI_OFFS__FIRST_WORD + 6, erto6.word);

	EMC_RFRSH_TREFI_OFFS_t erto7 = {
		.word = emc_read(EMC_RFRSH_TREFI_OFFS__FIRST_WORD + 7)
	};
	erto7.val_0 = prank_3_en * (prnk_factor * dp->ddr_3ds +
		      dp->ddr_3ds + 3) * rfrsh_trefi_interval;
	erto7.val_1 = prank_3_en * (prnk_factor * dp->ddr_3ds +
		      dp->ddr_3ds + 4) * rfrsh_trefi_interval;
	emc_write(EMC_RFRSH_TREFI_OFFS__FIRST_WORD + 7, erto7.word);

	EMC_IFC_BIST_TIMING_PARAMS_t eibtp = {
		.word = emc_read(EMC_IFC_BIST_TIMING_PARAMS)
	};
	eibtp.ack_addr_gap = 4;
	eibtp.ack_data_gap = 2;
	emc_write(EMC_IFC_BIST_TIMING_PARAMS, eibtp.word);

	mem_interface_config();
}

/* Memory controller configuration. */
static void mem_ctrl_config(void)
{
	uint32_t tyu_mss_rst;
	uint32_t rst_pin_umx;
	uint32_t rst_pin_s;
	uint32_t rst_pin_g;

	if (dp->mss_index == 0) {
		rst_pin_umx = MSS0_UMX_MC_DDR_RESET_EN;
		rst_pin_s = MSS0_DDR_MC_SRST_EN;
		rst_pin_g = MSS0_DDR_MC_GRST_EN;
	} else if (dp->mss_index == 1) {
		rst_pin_umx = MSS1_UMX_MC_DDR_RESET_EN;
		rst_pin_s = MSS1_DDR_MC_SRST_EN;
		rst_pin_g = MSS1_DDR_MC_GRST_EN;
	} else {
		MEM_ERR(1, "Unsupported MSS index %d.\n", dp->mss_index);
		return;
	}
	/* Set EMI Functional Reset. */
	tyu_mss_rst = mmio_read_32(TYU_BASE_ADDRESS + TYU_MSS_RESET);
	tyu_mss_rst |= rst_pin_s;
	mmio_write_32(TYU_BASE_ADDRESS + TYU_MSS_RESET, tyu_mss_rst);
	MEM_VERB(2, "Set EMI Functional Reset.\n");

	/* Clear EMI Configuration Reset. */
	tyu_mss_rst &= ~(rst_pin_g | rst_pin_umx);
	mmio_write_32(TYU_BASE_ADDRESS + TYU_MSS_RESET, tyu_mss_rst);
	MEM_VERB(2, "Clear EMI Configuration Reset.\n");

	/* Write Memory Controller Configuration registers. */
	mem_ctrl_config_regs();
	MEM_VERB(2, "Configured EMC config registers.\n");

	/* Release EMI Functional Reset. */
	tyu_mss_rst &= ~rst_pin_s;
	mmio_write_32(TYU_BASE_ADDRESS + TYU_MSS_RESET, tyu_mss_rst);
	MEM_VERB(2, "Released EMI Reset.\n");
}

/* Setup the values for the rcd_regs[] array. */
static void rcd_reg_val_setup(void)
{
	for (int i = 0; i < MAX_DIMM_PER_MEM_CTRL; i++) {

		if (dp->dimm[i].ranks == 0)
			continue;

		RC(00, i) = 0;
		RC(01, i) = 0;
		RC(02, i) = 0;
		RC(03, i) = (dp->dimm[i].spd[137] >> 4) & 0xf;
		RC(04, i) = ((dp->dimm[i].spd[137] & 0x3) << 2) |
			    ((dp->dimm[i].spd[137] & 0xc) >> 2);
		RC(05, i) = ((dp->dimm[i].spd[138] & 0x3) << 2) |
			    ((dp->dimm[i].spd[138] & 0xc) >> 2);
		RC(06, i) = 0;
		RC(07, i) = 0;
		RC(08, i) = 0;
		RC(09, i) = 0;
		RC(0A, i) = dp->tck >= 1250000 ? 0 :
			    dp->tck >= 1071000 ? 1 :
			    dp->tck >= 937000 ? 2 :
			    dp->tck >= 833000 ? 3 :
			    dp->tck >= 750000 ? 4 :
			    dp->tck >= 681000 ? 5 :
			    dp->tck >= 625000 ? 6 : 7;
		assert(RC(0A, i) != 7);
		RC(0B, i) = 0;
		RC(0C, i) = 0;
		RC(0D, i) = ((dp->type == RDIMM) << 2) | dp->rcd_cs_mode |
			  (!!(dp->system_address_mirror & (2 << (2 * i))) << 3);
		RC(0E, i) = dp->parity_en * 0xd;
		RC(0F, i) = dp->rcd_addr_lat;
		RC(1x, i) = 0;
		RC(2x, i) = 0;
		RC(3x, i) = ((2000000000 / dp->tck) - 1240) / 20;
		RC(4x, i) = 0;
		RC(5x, i) = 0;
		RC(6x, i) = 0;
		RC(7x, i) = 0;
		RC(8x, i) = 0;
		RC(9x, i) = 0;
		RC(Ax, i) = 0;
		RC(Bx, i) = 0;

		if (ddr_verbose_level >= 2) {
			int m = RCD_NUM % 2;

			tf_printf("Calculating RCD values for DIMM%d\n", i);
			for (int j = 0; j < RCD_NUM / 2; j++)
				tf_printf("RC0%x = 0x%x\tRC%xx = 0x%x\n", j,
				  dp->dimm[i].rcd_regs[j], j + m,
				  dp->dimm[i].rcd_regs[j + RCD_NUM / 2 + m]);
			if (m)
				tf_printf("RC0%x = 0x%x\n", RCD_NUM / 2,
				  dp->dimm[i].rcd_regs[RCD_NUM / 2]);

		}
	}
}

/* Setup the values for the db_regs[] array. */
static void db_reg_val_setup(void)
{
	for (int i = 0; i < MAX_DIMM_PER_MEM_CTRL; i++) {

		if (dp->dimm[i].ranks == 0)
			continue;

		F0BC(00, i) = dp->mem_rtt_nom == DISABLED ? 0 :
			      dp->mem_rtt_nom == RZQ_DIV_4 ? 1 :
			      dp->mem_rtt_nom == RZQ_DIV_2 ? 2 :
			      dp->mem_rtt_nom == RZQ_DIV_6 ? 3 :
			      dp->mem_rtt_nom == RZQ_DIV_1 ? 4 :
			      dp->mem_rtt_nom == RZQ_DIV_5 ? 5 :
			      dp->mem_rtt_nom == RZQ_DIV_3 ? 6 :
			      dp->mem_rtt_nom == RZQ_DIV_7 ? 7 : 8;
		assert(F0BC(00, i) != 8);

		F0BC(01, i) = dp->mem_rtt_wr == DISABLED ? 0 :
			      dp->mem_rtt_wr == RZQ_DIV_4 ? 1 :
			      dp->mem_rtt_wr == RZQ_DIV_2 ? 2 :
			      dp->mem_rtt_wr == RZQ_DIV_1 ? 4 :
			      dp->mem_rtt_wr == RZQ_DIV_3 ? 6 :
			      dp->mem_rtt_wr == HIGH_Z ? 7 : 8;
		assert(F0BC(01, i) != 8);

		F0BC(02, i) = dp->mem_rtt_park == DISABLED ? 0 :
			      dp->mem_rtt_park == RZQ_DIV_4 ? 1 :
			      dp->mem_rtt_park == RZQ_DIV_2 ? 2 :
			      dp->mem_rtt_park == RZQ_DIV_6 ? 3 :
			      dp->mem_rtt_park == RZQ_DIV_1 ? 4 :
			      dp->mem_rtt_park == RZQ_DIV_5 ? 5 :
			      dp->mem_rtt_park == RZQ_DIV_3 ? 6 :
			      dp->mem_rtt_park == RZQ_DIV_7 ? 7 : 8;
		assert(F0BC(02, i) != 8);

		F0BC(03, i) = dp->mem_odic == RZQ_DIV_6 ? 0 :
			      dp->mem_odic == RZQ_DIV_7 ? 1 :
			      dp->mem_odic == RZQ_DIV_5 ? 2 : 8;
		assert(F0BC(03, i) != 8);

		F0BC(04, i) = dp->tck >= 1071000 ? dp->dimm[i].spd[145] & 0x7 :
			      dp->tck >= 833000 ? dp->dimm[i].spd[146] & 0x7 :
			      dp->tck >= 625000 ? dp->dimm[i].spd[147] & 0x7 :
			      8;
		assert(F0BC(04, i) != 8);

		assert(dp->tck >= 625000);
		F0BC(05, i) = ((dp->tck >= 1071000 ? dp->dimm[i].spd[145] :
				dp->tck >= 833000 ? dp->dimm[i].spd[146] :
				dp->dimm[i].spd[147]) & 0x70) >> 4;

		F0BC(07, i) = 0xf ^ ((1 << dp->dimm[i].ranks) - 1);
		F0BC(08, i) = 0;
		F0BC(09, i) = 0;
		F0BC(0A, i) = dp->tck >= 1600000 ? 0 :
			      dp->tck >= 1250000 ? 0 :
			      dp->tck >= 1071000 ? 1 :
			      dp->tck >= 937000 ? 2 :
			      dp->tck >= 833000 ? 3 :
			      dp->tck >= 750000 ? 4 :
			      dp->tck >= 681000 ? 5 :
			      dp->tck >= 625000 ? 6 : 8;
		assert(F0BC(0A, i) != 7);
		F0BC(0B, i) = 0;
		F0BC(0C, i) = 0;
		F0BC(0D, i) = 0;
		F0BC(0E, i) = 0;

		FyBC(0, 6x, i) = ((2000000000 / dp->tck) - 1240) / 20;
		FyBC(0, 7x, i) = 0;

		uint32_t vrefdq_tr_val, vrefdq_tr_rng;

		mr6_get_vrefdq(dp->mem_vref, &vrefdq_tr_val, &vrefdq_tr_rng);

		FyBC(5, 5x, i) = vrefdq_tr_val | (vrefdq_tr_rng << 6);

		FyBC(5, 6x, i) = dp->dimm[i].spd[144] & 0x3f;

		FyBC(6, 4x, i) = (vrefdq_tr_rng << 6) |
				 ((dp->dimm[i].spd[155] & 0x10) >> 2);
	}
}

/*
 * Setup the values for the mr_regs[] array.
 * Return 1 on success or 0 if error.
 */
static int mr_reg_val_setup(void)
{
	const uint8_t mr0_cl_lookup[] = {
		[9] = 0,	[10] = 1,	[11] = 2,	[12] = 3,
		[13] = 4,	[14] = 5,	[15] = 6,	[16] = 7,
		[18] = 8,	[20] = 9,	[22] = 10,	[24] = 11,
		[23] = 12,	[17] = 13,	[19] = 14,	[21] = 15,
		[25] = 16,	[26] = 17,	[27] = 18,	[28] = 19,
		[30] = 21,	[32] = 23,
	};
	const uint8_t mr1_mr5_rtt_lookup[] = {
		[DISABLED] = 0,		[RZQ_DIV_4] = 1,	[RZQ_DIV_2] = 2,
		[RZQ_DIV_6] = 3,	[RZQ_DIV_1] = 4,	[RZQ_DIV_5] = 5,
		[RZQ_DIV_3] = 6,	[RZQ_DIV_7] = 7,
	};
	const uint8_t mr2_cwl_lookup[] = {
		[9] = 0,	[10] = 1,	[11] = 2,	[12] = 3,
		[14] = 4,	[16] = 5,	[18] = 6,	[20] = 7,
	};

	for (int r = 0; r < MAX_ACTIVE_RANKS; r++) {
		/* Figure out which dimm this rank belongs to. */
		int di = dp->dimm[1].ranks > 0 && r > 1;

		if ((dp->active_ranks & (1 << r)) == 0)
			continue;

		/*
		 * The MR registers have nonadjacent bits representing a field
		 * so it can't be easily defined as a union-struct type.
		 */
		uint32_t mr0_cl = mr0_cl_lookup[dp->cl];
		uint32_t wr = DDR_CEIL_DIV(dp->twr, dp->tck);
		uint32_t mr0_wr = ((wr + 1) / 2) - 5;

		if ((dp->cl != 9 && mr0_cl == 0) ||
		     dp->cl > ARRAYSIZE(mr0_cl_lookup)) {
			MEM_ERR(1, "CL = %d not supported.\n", dp->cl);
			return 0;
		} else if ((dp->cl == 25 || dp->cl == 27) && dp->ddr_3ds < 2) {
			MEM_ERR(1, "CL = %d supported only for 3ds.\n", dp->cl);
			return 0;
		}

		if (wr < 10 || wr > 26) {
			MEM_ERR(1, "twr value %lld not supported. (wr = %d)\n",
				dp->twr, wr);
			return 0;
		}

		if (wr > 20 && wr < 25)
			mr0_wr ^= 0x1;

		MR(0, r) = ((mr0_cl & 0x10) << 8) | ((mr0_cl & 0xe) << 3) |
			   ((mr0_cl & 0x1) << 2) | ((mr0_wr & 0x8) << 10) |
			   ((mr0_wr & 0x7) << 9);

		uint32_t mr1_odic = dp->type != LRDIMM ?
		    (dp->mem_odic == RZQ_DIV_7 ? 0 : 1) :
		    ((dp->tck >= 1071000 ? dp->dimm[di].spd[148] :
		      dp->tck >= 833000  ? dp->dimm[di].spd[148] >> 2 :
					   dp->dimm[di].spd[148] >> 4) & 0x3);

		uint32_t mr1_al = dp->al > 0 ? dp->cl - dp->al : 0;

		uint32_t mr1_rtt_nom = dp->type != LRDIMM ?
			(mr1_mr5_rtt_lookup[dp->mem_rtt_nom]) :
			(dp->tck >= 1071000 ? dp->dimm[di].spd[149] :
			 dp->tck >= 833000  ? dp->dimm[di].spd[150] :
					      dp->dimm[di].spd[151]) & 0x7;

		MR(1, r) = 0x1 | (mr1_odic << 1) | (mr1_al << 3) |
			   (mr1_rtt_nom << 8);

		uint32_t mr2_cwl = mr2_cwl_lookup[dp->cwl];

		if ((dp->cwl != 9 && mr2_cwl == 0) ||
		     dp->cwl > ARRAYSIZE(mr2_cwl_lookup)) {
			MEM_ERR(1, "CWL = %d not supported.\n", dp->cwl);
			return 0;
		}

		uint32_t mr2_lpasr = dp->asr_en ? 0x3 :
				     dp->ddr_tcase > 85 ? 0x2 : 0x0;
		uint32_t mr2_rtt_wr = dp->type != LRDIMM ?
				(dp->mem_rtt_wr == DISABLED	? 0 :
				 dp->mem_rtt_wr == RZQ_DIV_2	? 1 :
				 dp->mem_rtt_wr == RZQ_DIV_1	? 2 :
				 dp->mem_rtt_wr == HIGH_Z	? 3 :
				 dp->mem_rtt_wr == RZQ_DIV_3	? 4 : 3) :
			(dp->tck >= 1071000 ? dp->dimm[di].spd[149] :
			 dp->tck >= 833000  ? dp->dimm[di].spd[150] :
					      dp->dimm[di].spd[151]) & 0x7;



		MR(2, r) = (mr2_cwl << 3) | (mr2_lpasr << 6) |
			   (mr2_rtt_wr << 9);

		MR(3, r) = 0;
		MR(4, r) = 0;

		uint32_t mr5_ca_par_lmode = dp->pl == 0 ? 0 :
					    dp->pl == 4 ? 1 :
					    dp->pl == 5 ? 2 :
					    dp->pl == 6 ? 3 :
					    4;

		uint32_t mr5_rtt_park = dp->type != LRDIMM ?
			mr1_mr5_rtt_lookup[dp->mem_rtt_park] :
			((dp->tck >= 1071000 ? dp->dimm[di].spd[152] :
			  dp->tck >= 833000  ? dp->dimm[di].spd[153] :
					       dp->dimm[di].spd[154]) >>
			  (2 * (r > 1))) & 0x7;

		MR(5, r) = mr5_ca_par_lmode | (mr5_rtt_park << 6) |
			   ((dp->wr_dbi_en != 0) << 11) |
			   ((dp->rd_dbi_en != 0) << 12);

		uint32_t mr6_vrefdq_tr_val, mr6_vrefdq_tr_rng;

		if (dp->type != LRDIMM)
			mr6_get_vrefdq(dp->mem_vref, &mr6_vrefdq_tr_val,
					&mr6_vrefdq_tr_rng);
		else {
			mr6_vrefdq_tr_val = dp->dimm[di].spd[140 + r] & 0x1f;
			mr6_vrefdq_tr_rng = (dp->dimm[di].spd[155] >> r) & 0x1;
		}

		uint32_t tccd_l_nck = DDR_CEIL_DIV(dp->tccd_l, dp->tck);
		uint32_t mr6_tccd_l = tccd_l_nck - 4;

		MR(6, r) = mr6_vrefdq_tr_val | (mr6_vrefdq_tr_rng << 6) |
			   (mr6_tccd_l << 10);

		if (ddr_verbose_level >= 2) {
			tf_printf("Calculated MRS values for rank %d:\n", r);
			for (int j = 0; j < 7; j++)
				tf_printf("MR%d = 0x%x\n", j, MR(j, r));
		}
	}

	return 1;
}

/* This is implemented differently for Sella. */
#pragma weak setup_ZQnPR
void setup_ZQnPR(void)
{
	uint32_t zq_ocdi = dp->phy_wr_drv == RZQ_DIV_4 ? 7 :
			   dp->phy_wr_drv == RZQ_DIV_5 ? 9 :
			   dp->phy_wr_drv == RZQ_DIV_6 ? 11 :
			   dp->phy_wr_drv == RZQ_DIV_7 ? 13 :
			   dp->phy_wr_drv == RZQ_DIV_8 ? 15 : 0;
	assert(zq_ocdi != 0);
	uint32_t zq_odt = dp->phy_rd_odt == RZQ_DIV_2 ? 3 :
			  dp->phy_rd_odt == RZQ_DIV_3 ? 5 :
			  dp->phy_rd_odt == RZQ_DIV_4 ? 7 :
			  dp->phy_rd_odt == RZQ_DIV_5 ? 9 :
			  dp->phy_rd_odt == RZQ_DIV_6 ? 9 :
			  dp->phy_rd_odt == RZQ_DIV_7 ? 13 :
			  dp->phy_rd_odt == RZQ_DIV_7 ? 15 : 0;
	assert(zq_odt != 0);
	uint32_t zq_div_data = (16 * zq_odt) + zq_ocdi;

	PUB_ZQnPR_t zq0pr = { .word = pub_read(PUB_ZQ0PR) };
	zq0pr.zqdiv = zq_div_data;
	zq0pr.zprog_asym_drv_pu = zq_ocdi;
	zq0pr.zprog_asym_drv_pd = zq_ocdi;
	zq0pr.zprog_pu_odt_only = zq_odt;
	pub_write(PUB_ZQ0PR, zq0pr.word);

	PUB_ZQnPR_t zq1pr = { .word = pub_read(PUB_ZQ1PR) };
	zq1pr.zqdiv = zq_div_data;
	zq1pr.zprog_asym_drv_pu = zq_ocdi;
	zq1pr.zprog_asym_drv_pd = zq_ocdi;
	zq1pr.zprog_pu_odt_only = zq_odt;
	pub_write(PUB_ZQ1PR, zq1pr.word);


}
/* DDR PHY SDRAM system specific registers configuration */
static void ddr_phy_config(void)
{
	PUB_PGCR1_t pgcr1 = { .word = pub_read(PUB_PGCR1) };
	pgcr1.updmstrc0 = 1;
	pgcr1.prcfg_en = dp->dimm_num == 2;
	pub_write(PUB_PGCR1, pgcr1.word);

	PUB_PGCR2_t pgcr2 = { .word = pub_read(PUB_PGCR2) };
	pgcr2.csncidmux = dp->dimm[0].ranks > 2;
	pub_write(PUB_PGCR2, pgcr2.word);

	PUB_PGCR8_t pgcr8 = { .word = pub_read(PUB_PGCR8) };
	pgcr8.dcalsval = 1;
	pgcr8.dcaltype = 1;
	pub_write(PUB_PGCR8, pgcr8.word);

	PUB_PLLCR_t pllcr = { .word = pub_read(PUB_PLLCR) };
	pllcr.frqsel = dp->tck < 1075000 ? 0 :
		       dp->tck < 1818000 ? 1 :
		       3;
	pub_write(PUB_PLLCR, pllcr.word);

	PUB_DXCCR_t dxccr = { .word = pub_read(PUB_DXCCR) };
	dxccr.dqsres = 0x0;
	dxccr.dqsnres = 0x8;
	dxccr.x4mode = dp->package == PACKAGE_x4;
	dxccr.x4dqsmd = dp->package != PACKAGE_x4;
	pub_write(PUB_DXCCR, dxccr.word);

	PUB_DSGCR_t dsgcr = { .word = pub_read(PUB_DSGCR) };
	dsgcr.puren = 0;
	dsgcr.ctlzuen = 1;
	pub_write(PUB_DSGCR, dsgcr.word);

	PUB_DCR_t dcr = { .word = pub_read(PUB_DCR) };
	dcr.ddrmd = 0x4;
	dcr.nosra = (dp->type == RDIMM || dp->type == LRDIMM) ||
		    (dp->dimm_num > 1) || (dp->system_address_mirror != 0);
	dcr.udimm = dp->type == UDIMM && dp->system_address_mirror != 0;
	pub_write(PUB_DCR, dcr.word);

	PUB_DTCR0_t dtcr0 = { .word = pub_read(PUB_DTCR0) };
	dtcr0.dtmpr = 1;
	dtcr0.dtwbddm = 0;
	dtcr0.rfshdt = 0;
	pub_write(PUB_DTCR0, dtcr0.word);

	uint32_t dimm_0_ranks_map = (1 << (4 * dp->dimm[0].ranks)) - 1;
	uint32_t dimm_1_ranks_map = ((1 << (4 * dp->dimm[1].ranks)) - 1) << 8;
	PUB_DTCR1_t dtcr1 = { .word = pub_read(PUB_DTCR1) };
	dtcr1.ranken = (dimm_1_ranks_map | dimm_0_ranks_map) & 0x1111;
	dtcr1.dtrank = 2 * (dp->dimm[0].ranks == 0 && dp->dimm[1].ranks > 0);
	pub_write(PUB_DTCR1, dtcr1.word);

	PUB_IOVCR0_t iovcr0 = { .word = pub_read(PUB_IOVCR0) };
	iovcr0.acvrefisel = 0x0b;
	pub_write(PUB_IOVCR0, iovcr0.word);

	PUB_IOVCR1_t iovcr1 = { .word = pub_read(PUB_IOVCR1) };
	iovcr1.zqvrefisel = 0x22;
	pub_write(PUB_IOVCR1, iovcr1.word);

	PUB_VTCR1_t vtcr1 = { .word = pub_read(PUB_VTCR1) };
	vtcr1.hvio = 0;
	vtcr1.shren = 0;
	pub_write(PUB_VTCR1, vtcr1.word);

	PUB_DTPR0_t dtpr0 = { .word = pub_read(PUB_DTPR0) };
	dtpr0.trtp = DDR_CEIL_DIV(dp->trtp, dp->tck);
	dtpr0.trp = DDR_CEIL_DIV(dp->trp, dp->tck);
	dtpr0.tras = DDR_CEIL_DIV(dp->tras, dp->tck);
	dtpr0.trrd = DDR_CEIL_DIV(dp->trrd_l, dp->tck);
	pub_write(PUB_DTPR0, dtpr0.word);

	PUB_DTPR1_t dtpr1 = { .word = pub_read(PUB_DTPR1) };
	dtpr1.tmrd = dp->tmrd / FS_PER_NS;
	dtpr1.tmod = DDR_CEIL_DIV(dp->tmod, dp->tck) - 24;
	dtpr1.tfaw = DDR_CEIL_DIV(dp->tfaw, dp->tck);
	dtpr1.twlmrd = dp->twlmrd / FS_PER_NS;
	pub_write(PUB_DTPR1, dtpr1.word);

	PUB_DTPR2_t dtpr2 = { .word = pub_read(PUB_DTPR2) };
	dtpr2.txs = MAX(dp->txs, dp->tdllk);
	dtpr2.tcke = DDR_CEIL_DIV(dp->tcke, dp->tck);
	dtpr2.trtodt = dp->trtodt / FS_PER_NS;
	dtpr2.trtw = dp->trtw / FS_PER_NS;
	pub_write(PUB_DTPR2, dtpr2.word);

	PUB_DTPR3_t dtpr3 = { .word = pub_read(PUB_DTPR3) };
	dtpr3.tdqsck = 0;
	dtpr3.tdqsckmax = 0;
	dtpr3.tdllk = dp->tdllk;
	dtpr3.tccd = 0;
	dtpr3.tofdx = 0;
	pub_write(PUB_DTPR3, dtpr3.word);

	PUB_DTPR4_t dtpr4 = { .word = pub_read(PUB_DTPR4) };
	dtpr4.txp = DDR_CEIL_DIV(dp->txp, dp->tck);
	dtpr4.twlo = DDR_CEIL_DIV(dp->twlo, dp->tck);
	dtpr4.trfc = DDR_CEIL_DIV(dp->trfc, dp->tck);
	dtpr4.taond_taofd = 0;
	pub_write(PUB_DTPR4, dtpr4.word);

	PUB_DTPR5_t dtpr5 = { .word = pub_read(PUB_DTPR5) };
	dtpr5.twtr = dp->twtr_l;
	dtpr5.trcd = DDR_CEIL_DIV(dp->trcd, dp->tck);
	dtpr5.trc = DDR_CEIL_DIV(dp->trc, dp->tck);
	pub_write(PUB_DTPR5, dtpr5.word);

	if (dp->type == RDIMM || dp->type == LRDIMM) {
		PUB_RDIMMGCR0_t rdimmgcr0 = { .word = pub_read(PUB_RDIMMGCR0) };
		rdimmgcr0.rdimm = (dp->type == RDIMM) || (dp->type == LRDIMM);
		rdimmgcr0.lrdimm = (dp->type == LRDIMM);
		pub_write(PUB_RDIMMGCR0, rdimmgcr0.word);
	}

	/* Set up the per DIMM RDIMMCRx registers. */
	if (dp->type == RDIMM || dp->type == LRDIMM) {
		for (int dimm = 0; dimm < MAX_DIMM_PER_MEM_CTRL; dimm++) {

			if (dp->dimm[dimm].ranks == 0)
				continue;

			pub_switch_rankidr(dimm, 1, 1);

			MEM_VERB(2, "Writing RDIMMCRx register for DIMM%d.\n",
				 dimm);

			PUB_RDIMMCR0_t rdimmcr0 = {
				.rc0 = RC(00, dimm),
				.rc1 = RC(01, dimm),
				.rc2 = RC(02, dimm),
				.rc3 = RC(03, dimm),
				.rc4 = RC(04, dimm),
				.rc5 = RC(05, dimm),
				.rc6 = RC(06, dimm),
				.rc7 = RC(07, dimm),
			};
			pub_write(PUB_RDIMMCR0, rdimmcr0.word);

			PUB_RDIMMCR1_t rdimmcr1 = {
				.rc8 = RC(08, dimm),
				.rc9 = RC(09, dimm),
				.rc10 = RC(0A, dimm),
				.rc11 = RC(0B, dimm),
				.rc12 = RC(0C, dimm),
				.rc13 = RC(0D, dimm),
				.rc14 = RC(0E, dimm),
				.rc15 = RC(0F, dimm),
			};
			pub_write(PUB_RDIMMCR1, rdimmcr1.word);

			PUB_RDIMMCR2_t rdimmcr2 = {
				.rc1x = RC(1x, dimm),
				.rc2x = RC(2x, dimm),
				.rc3x = RC(3x, dimm),
				.rc4x = RC(4x, dimm),
			};
			pub_write(PUB_RDIMMCR2, rdimmcr2.word);

			PUB_RDIMMCR3_t rdimmcr3 = {
				.rc5x = RC(5x, dimm),
				.rc6x = RC(6x, dimm),
				.rc7x = RC(7x, dimm),
				.rc8x = RC(8x, dimm),
			};
			pub_write(PUB_RDIMMCR3, rdimmcr3.word);

			PUB_RDIMMCR4_t rdimmcr4 = {
				.rc9x = RC(9x, dimm),
				.rcax = RC(Ax, dimm),
				.rcbx = RC(Bx, dimm),
				.rccx = RC(Cx, dimm),
			};
			pub_write(PUB_RDIMMCR4, rdimmcr4.word);
		}
	}

	/* Set up the per RANK MRS registers (and also ODTCR). */
	for (int rank = 0; rank < MAX_ACTIVE_RANKS; rank++) {

		if ((dp->active_ranks & (1 << rank)) == 0)
			continue;

		pub_switch_rankidr(rank, 1, 1);

		if (dp->dimm[0].ranks >= 4) {
			PUB_ODTCR_t odtcr = {
				.word = pub_read(PUB_ODTCR)
			};
			odtcr.wrodt = (rank & 1) + 1;
			pub_write(PUB_ODTCR, odtcr.word);
		}

		MEM_VERB(2, "Writing MR register for RANK%d.\n", rank);

		pub_write(PUB_MR0, MR(0, rank));
		pub_write(PUB_MR1, MR(1, rank));
		pub_write(PUB_MR2, MR(2, rank));
		pub_write(PUB_MR3, MR(3, rank));
		pub_write(PUB_MR4, MR(4, rank));
		pub_write(PUB_MR5, MR(5, rank));
		pub_write(PUB_MR6, MR(6, rank));
	}

	PUB_ZQCR_t zqcr = { .word = pub_read(PUB_ZQCR) };
	assert(dp->tck < 12500000 && dp->tck >= 750000);
	zqcr.pgwait = dp->tck >= 5000000 ? 0 :
		      dp->tck >= 2500000 ? 1 :
		      dp->tck >= 1875000 ? 2 :
		      dp->tck >= 1666000 ? 3 :
		      dp->tck >= 1500000 ? 4 :
		      dp->tck >= 1250000 ? 5 :
		      dp->tck >= 1071000 ? 6 : 7;
	zqcr.asym_drv_en = 1;
	zqcr.pu_odt_only = 1;
	zqcr.dis_mon_lin_comp = 0;
	pub_write(PUB_ZQCR, zqcr.word);

	setup_ZQnPR();

	dp->vrefdq_isel_val = get_host_vrefdq_isel(dp->phy_rd_vref);
	PUB_DXnGCR5_t dxngcr5 = {
		.dxrefiselr0 = dp->vrefdq_isel_val,
		.dxrefiselr1 = dp->vrefdq_isel_val,
		.dxrefiselr2 = dp->vrefdq_isel_val,
		.dxrefiselr3 = dp->vrefdq_isel_val,
	};
	pub_write(PUB_DX0GCR5, dxngcr5.word);
	pub_write(PUB_DX1GCR5, dxngcr5.word);
	pub_write(PUB_DX2GCR5, dxngcr5.word);
	pub_write(PUB_DX3GCR5, dxngcr5.word);
	pub_write(PUB_DX4GCR5, dxngcr5.word);
	pub_write(PUB_DX5GCR5, dxngcr5.word);
	pub_write(PUB_DX6GCR5, dxngcr5.word);
	pub_write(PUB_DX7GCR5, dxngcr5.word);
	pub_write(PUB_DX8GCR5, dxngcr5.word);

	PUB_DXnGCR8_t dxngcr8 = {
		.x4dxrefiselr0 = dp->vrefdq_isel_val,
		.x4dxrefiselr1 = dp->vrefdq_isel_val,
		.x4dxrefiselr2 = dp->vrefdq_isel_val,
		.x4dxrefiselr3 = dp->vrefdq_isel_val,
	};
	pub_write(PUB_DX0GCR8, dxngcr8.word);
	pub_write(PUB_DX1GCR8, dxngcr8.word);
	pub_write(PUB_DX2GCR8, dxngcr8.word);
	pub_write(PUB_DX3GCR8, dxngcr8.word);
	pub_write(PUB_DX4GCR8, dxngcr8.word);
	pub_write(PUB_DX5GCR8, dxngcr8.word);
	pub_write(PUB_DX6GCR8, dxngcr8.word);
	pub_write(PUB_DX7GCR8, dxngcr8.word);
	pub_write(PUB_DX8GCR8, dxngcr8.word);
}

/*
 * Initiate the hardware to do DDR PHY VREF training.
 * Return 1 on success or 0 if failed.
 */
static int ddr_phy_vref_training(void)
{
	/* Start the PUB control operation of the DDR PHY VREF training. */
	PUB_PGSR0_t pgsr0_done = { .vdone = 1 };
	PUB_PGSR0_t pgsr0_err = { .verr = 1 };

	if (!pir_cmd(0x00060001, 5 * NS_PER_MS, "DDR PHY Vref leveling",
	    pgsr0_done.word, pgsr0_err.word))
		return 0;

	return 1;
}

/*
 * DDR PHY Initialization (Initialize DDR PHY PLL/DLL/ZCAL process).
 * Return 1 on success or 0 if failed.
 */
static int ddr_phy_init(void)
{
	/* Start PUB control operation of the DDR PHY initialization. */
	pub_write(PUB_PIR, 0x00000033);

	/* Wait (max 10ms) for the DDR PHY initization to complete. */
	PUB_STATUS_t ps = {
		.dfi_init_complete = 1,
	};
	if (!read_loop(pub_indirect_read, PUB_STATUS, ps.word, ps.word,
		       10 * NS_PER_MS, 100 * NS_PER_US, NULL)) {
		MEM_ERR(1, "Timed out waiting for completion of DDR PHY init.\n");
		goto ddr_phy_init_fail;
	}

	/* Verify PGSR0 PUB register have the correct value. */
	PUB_PGSR0_t pgsr0 = { .word = pub_read(PUB_PGSR0) };
	if (!pgsr0.idone || !pgsr0.pldone || !pgsr0.dcdone || !pgsr0.zcdone ||
	    pgsr0.zcerr) {
		MEM_ERR(1, "DDR PHY init ended with pgsr0 = 0x%x.\n", pgsr0.word);
		goto ddr_phy_init_fail;
	}
	MEM_VERB(2, "Hardware DDR PHY initialization complete.\n");

	/* Force VT update to the impedance calibration FSM. */
	PUB_ZQCR_t zqcr = { .word = pub_read(PUB_ZQCR) };
	zqcr.force_zcat_vt_updat = 1;
	pub_write(PUB_ZQCR, zqcr.word);
	mem_config_ndelay(10 * NS_PER_US);
	zqcr.force_zcat_vt_updat = 0;
	pub_write(PUB_ZQCR, zqcr.word);
	mem_config_ndelay(10 * NS_PER_US);

	/* Set the PUB into SDRAM initialization mode.  */
	pub_write(PUB_PIR, 0x00040001);

	return 1;

ddr_phy_init_fail:

	ERR_SHOW_PUB_REG(PUB_ZQ0SR);
	ERR_SHOW_PUB_REG(PUB_ZQ1SR);
	ERR_SHOW_PUB_REG(PUB_ACMDLR0);
	ERR_SHOW_PUB_DXn(MDLR0);
	ERR_SHOW_PUB_DXn(GSR0);
	if (dp->package == PACKAGE_x4)
		ERR_SHOW_PUB_DXn(GSR4);

	return 0;
}

/* DDR PHY CK deferential pair signals delay setup. */
static void ddr_phy_ck_setup(void)
{
	PUB_ACMDLR0_t acmdlr0 = { .word = pub_read(PUB_ACMDLR0) };

	/* Skip this part if all the clk delays are less than 15ps. */
	if (MAX(dp->ddr_clk[0], dp->ddr_clk[1]) >= 15 * FS_PER_PS) {

		PUB_ACBDLR0_t acbdlr0 = { .word = pub_read(PUB_ACBDLR0) };

		MEM_VERB(2, "Setting DDR PHY CK deferential pair signal delay.\n");

		for (int i = 0; i < 2; i++) {
			/* Skip if the delay is less than 15ps. */
			if (dp->ddr_clk[i] < 15 * FS_PER_PS)
				continue;

			uint32_t ck_bdl = (dp->ddr_clk[i] * 2 *
					   acmdlr0.iprd + dp->tck / 2) / dp->tck;
			if (i == 0)
				acbdlr0.ck0bd = ck_bdl;
			else
				acbdlr0.ck1bd = ck_bdl;
		}
		MEM_VERB(2, "ACBDLR0 = 0x%x.\n", acbdlr0.word);
		pub_write(PUB_ACBDLR0, acbdlr0.word);
	}

	if (dp->ddr_addr_ctrl >= 15 * FS_PER_PS) {
		uint32_t ac_bdl = (dp->ddr_addr_ctrl * 2 *
				   acmdlr0.iprd + dp->tck / 2) / dp->tck;

		MEM_VERB(2, "ac_bdl = 0x%x.\n", ac_bdl);

		PUB_ACBDLR1_t acbdlr1 = {
			.parbd = ac_bdl,
			.a16bd = ac_bdl,
			.a17bd = ac_bdl,
			.actbd = ac_bdl,
		};
		pub_write(PUB_ACBDLR1, acbdlr1.word);

		PUB_ACBDLR2_t acbdlr2 = {
			.bg1bd = ac_bdl,
			.bg0bd = ac_bdl,
			.ba1bd = ac_bdl,
			.ba0bd = ac_bdl,
		};
		pub_write(PUB_ACBDLR2, acbdlr2.word);

		PUB_ACBDLR3_t acbdlr3 = {
			.cs3bd = ac_bdl,
			.cs2bd = ac_bdl,
			.cs1bd = ac_bdl,
			.cs0bd = ac_bdl,
		};
		pub_write(PUB_ACBDLR3, acbdlr3.word);

		PUB_ACBDLR4_t acbdlr4 = {
			.odt3bd = ac_bdl,
			.odt2bd = ac_bdl,
			.odt1bd = ac_bdl,
			.odt0bd = ac_bdl,
		};
		pub_write(PUB_ACBDLR4, acbdlr4.word);

		PUB_ACBDLR5_t acbdlr5 = {
			.cke3bd = ac_bdl,
			.cke2bd = ac_bdl,
			.cke1bd = ac_bdl,
			.cke0bd = ac_bdl,
		};
		pub_write(PUB_ACBDLR5, acbdlr5.word);

		PUB_ACBDLR6_t acbdlr6 = {
			.a03bd = ac_bdl,
			.a02bd = ac_bdl,
			.a01bd = ac_bdl,
			.a00bd = ac_bdl,
		};
		pub_write(PUB_ACBDLR6, acbdlr6.word);

		PUB_ACBDLR7_t acbdlr7 = {
			.a07bd = ac_bdl,
			.a06bd = ac_bdl,
			.a05bd = ac_bdl,
			.a04bd = ac_bdl,
		};
		pub_write(PUB_ACBDLR7, acbdlr7.word);

		PUB_ACBDLR8_t acbdlr8 = {
			.a11bd = ac_bdl,
			.a10bd = ac_bdl,
			.a09bd = ac_bdl,
			.a08bd = ac_bdl,
		};
		pub_write(PUB_ACBDLR8, acbdlr8.word);

		PUB_ACBDLR9_t acbdlr9 = {
			.a15bd = ac_bdl,
			.a14bd = ac_bdl,
			.a13bd = ac_bdl,
			.a12bd = ac_bdl,
		};
		pub_write(PUB_ACBDLR9, acbdlr9.word);

		PUB_ACBDLR10_t acbdlr10 = {
			.cid0bd = ac_bdl,
			.cid1bd = ac_bdl,
			.cid2bd = ac_bdl,
		};
		pub_write(PUB_ACBDLR10, acbdlr10.word);
	}
}

/* DIMM's or SDRAM system reset release. */
static void dimm_system_release(void)
{
	/* Deactivate the DDR DIMM's or SDRAM System Reset */
	dimm_reset(1);
	mem_config_ndelay(500 * NS_PER_US);
}

/* RCD configuration sequence. */
static void rcd_config_seq(void)
{
#define RCD_W(reg, end)	\
	do { \
		mrs_write(mr, 7, RC ## reg + RC(reg, dimm_idx), 0, 0x10, end); \
		DELAY_TCK(32, dp->tck); \
	} while (0)

	for (int i = (dp->dimm[0].ranks ? 0 : 2);
	     i <= (dp->dimm[1].ranks ? 2 : 0); i += 2) {
		/* MR_RANK value for the given rank. */
		int mr = MR_RANK(i);
		int dimm_idx = dp->dimm[1].ranks > 0 && i > 1;

		MEM_VERB(2, "Writing RCD registers for rank %d\n", i);

		RCD_W(0D, 0);
		RCD_W(00, 0);
		RCD_W(01, 0);
		RCD_W(02, 0);
		RCD_W(03, 0);
		RCD_W(04, 0);
		RCD_W(05, 0);
		RCD_W(08, 0);
		RCD_W(09, 0);
		RCD_W(0A, 0);
		RCD_W(0B, 0);
		RCD_W(0E, 0);
		RCD_W(0F, 0);
		RCD_W(3x, 1);
	}
}

/* DB configuration sequence. */
static void db_config_seq(void)
{
#define DB_W(pfx, reg, val, end)	\
	do { \
		mrs_write(mr, 7, 0x1000 + pfx ## reg + val, 0, \
			  0x10, end); \
		DELAY_TCK(32, dp->tck); \
	} while (0)

	for (int i = 0; i <= (dp->dimm[1].ranks ? 2 : 0); i += 2) {
		/* MR_RANK value for the given rank. */
		int mr = MR_RANK(i);
		int dimm_idx = dp->dimm[1].ranks > 0 && i > 1;

		DB_W(FyBC, 7x, 0, 0);
		DB_W(F0BC, 00, F0BC(00, dimm_idx), 0);
		DB_W(F0BC, 01, F0BC(01, dimm_idx), 0);
		DB_W(F0BC, 02, F0BC(02, dimm_idx), 0);
		DB_W(F0BC, 03, F0BC(03, dimm_idx), 0);
		DB_W(F0BC, 04, F0BC(04, dimm_idx), 0);
		DB_W(F0BC, 05, F0BC(05, dimm_idx), 0);
		DB_W(F0BC, 07, F0BC(07, dimm_idx), 0);
		DB_W(F0BC, 0A, F0BC(0A, dimm_idx), 0);
		DB_W(FyBC, 6x, FyBC(0, 6x, dimm_idx), 1);
		DB_W(FyBC, 7x, 6, 1);
		DB_W(FyBC, 4x, FyBC(6, 4x, dimm_idx), 1);
		DB_W(FyBC, 7x, 5, 1);
		DB_W(FyBC, 5x, FyBC(5, 5x, dimm_idx), 0);
		DB_W(FyBC, 6x, FyBC(5, 6x, dimm_idx), 1);
		DB_W(FyBC, 7x, 0, 1);
	}
}

/* DDR4 MRS initialization sequence. */
static void mrs_config_seq(void)
{
#define MRS_W(reg, end) \
	do { \
		mrs_write(mr, reg, MR(reg, i), 0, 4, end); \
		DELAY_TCK(8, dp->tck); \
	} while (0)

#define MRS_W_INV(reg, end) \
	do { \
		mrs_write_inv(mr, reg, MR(reg, i), 0, 4, \
			      end, RC(00, dimm_idx)); \
		DELAY_TCK(8, dp->tck); \
	} while (0)

	for (int i = 0; i < MAX_ACTIVE_RANKS; i++) {
		if ((dp->active_ranks & (1 << i)) == 0)
			continue;

		/* MR_RANK value for the given rank. */
		int mr = MR_RANK(i);
		int dimm_idx = dp->dimm[1].ranks > 0 && i > 1;

		/* Set the DLL_RESET bit for first SDRAM init write. */
		 MR(0, i) |= MR0_DLL_RESET;

		MEM_VERB(2, "Writing MRS registers for rank %d\n", i);

		MRS_W(3, 0);
		MRS_W(6, 0);
		MRS_W(5, 0);
		MRS_W(4, 0);
		MRS_W(2, 0);
		MRS_W(1, 0);
		MRS_W(0, 1);

		if ((dp->type == RDIMM) || (dp->type == LRDIMM)) {
			MRS_W_INV(3, 0);
			MRS_W_INV(6, 0);
			MRS_W_INV(5, 0);
			MRS_W_INV(4, 0);
			MRS_W_INV(2, 0);
			MRS_W_INV(1, 0);
			MRS_W_INV(0, 1);
		}

		/* Clear the DLL_RESET bit for following SDRAM init write. */
		 MR(0, i) &= ~MR0_DLL_RESET;
	}
}

/*
 * Perform the VREF static setup on the given physical rank.
 * If mr6_vref(_x4)_bl isn't NULL, also write back the value from
 * MR6 back to it.
 */
static void vref_static_setup_rank(int rank, uint32_t *mr6_vref_bl,
				   uint32_t *mr6_vref_x4_bl)
{
	uint32_t  mr = MR_RANK(rank);
	int dimm_idx = dp->dimm[1].ranks > 0 && rank > 1;

	for (int j = 0; j < 2; j++) {
		mrs_write(mr, 6, MR(6, rank) | 0x80, 0, 0x4, 1);
		mem_config_ndelay(500);
	}

	mrs_write(mr, 6, MR(6, rank), 0, 0x4, 1);
	mem_config_ndelay(500);
	for (int j = 0; j < BYTELANE_NUM; j++) {
		if (mr6_vref_bl != NULL)
			mr6_vref_bl[j] = MR(6, rank) & 0xff;
		if (mr6_vref_x4_bl != NULL)
			mr6_vref_x4_bl[j] = MR(6, rank) & 0xff;
	}

	if (dp->type != LRDIMM && dp->type != RDIMM)
		return;

	/* For RDIMM or LRDIMM, repeat for side B access. */
	for (int j = 0; j < 2; j++) {
		mrs_write_inv(mr, 6, MR(6, rank) | 0x80, 0, 0x4, 1,
			      RC(00, dimm_idx));
		mem_config_ndelay(500);
	}
	mrs_write_inv(mr, 6, MR(6, rank), 0, 0x4, 1, RC(00, dimm_idx));
	mem_config_ndelay(500);
}


/* VREF static setup. */
static void vref_static_setup(void)
{
	for (int i = 0; i < MAX_ACTIVE_RANKS; i++) {
		if ((dp->active_ranks & (1 << i)) == 0)
			continue;

		vref_static_setup_rank(i, dp->res_mr6_vref[i],
				       dp->res_mr6_vref_x4[i]);
	}
}

/* This function restores the Vref value of a rank to the average value */
int vref_rank_avg_restore(int rank_idx)
{
	uint32_t sdram_vref_range = dp->res_mr6_vref[rank_idx][0];
	uint32_t sdram_vref_sum   = 0;
	uint32_t sdram_vref_average;
	uint32_t sdram_vref_data;
	uint32_t mr = MR_RANK(rank_idx);
	int dimm_idx = dp->dimm_num > 1 && rank_idx > 1;

	for (int byte = 0; byte < BYTELANE_NUM; byte++) {
		uint32_t sdram_vref_temp = dp->res_mr6_vref[rank_idx][byte];

		/*
		 * Our average only works if all the bytes in the same rank
		 * are using the same Vref range, else the Vref index would
		 * have different meaning and average makes no sense.
		 */
		if ((sdram_vref_temp ^ sdram_vref_range) & (1 << 6)){
			MEM_ERR(1, "Vref range different within rank bytes!\n");
			return 1;
		}
		sdram_vref_temp &= 0x3f;
		sdram_vref_sum  += sdram_vref_temp;
	}
	sdram_vref_average = (sdram_vref_sum + BYTELANE_NUM / 2) / BYTELANE_NUM;
	sdram_vref_data    = sdram_vref_average | (sdram_vref_range & (1 << 6));

	mrs_write(mr, 6, (MR(6, rank_idx) & 0xff00) | 0x80 | sdram_vref_data,
		  0, 0x4, 1);
	mem_config_ndelay(500);
	mrs_write(mr, 6, (MR(6, rank_idx) & 0xff00) | 0x80 | sdram_vref_data,
		  0, 0x4, 1);
	mem_config_ndelay(500);
	mrs_write(mr, 6, (MR(6, rank_idx) & 0xff00) | sdram_vref_data, 0, 0x4, 1);
	mem_config_ndelay(500);

	if (dp->type == LRDIMM || dp->type == RDIMM) {
		/* For RDIMM or LRDIMM, repeat for side B access. */
		mrs_write_inv(mr, 6, (MR(6, rank_idx) & 0xff00) | 0x80 |
			      sdram_vref_data, 0, 0x4, 1, RC(00, dimm_idx));
		mem_config_ndelay(500);
		mrs_write_inv(mr, 6, (MR(6, rank_idx) & 0xff00) | 0x80 |
			      sdram_vref_data, 0, 0x4, 1, RC(00, dimm_idx));
		mem_config_ndelay(500);
		mrs_write_inv(mr, 6, (MR(6, rank_idx) & 0xff00) |
			      sdram_vref_data, 0, 0x4, 1, RC(00, dimm_idx));
		mem_config_ndelay(500);
	}
	return 0;
}

/* DIMM's or SDRAM initialization. */
static void dimm_init(void)
{
	/* Activate CKE output. */
	EMC_PHY_CTRL_t epc = {
		.ck_en = (4 * ((1 << dp->dimm[1].ranks) - 1) +
			 (1 << dp->dimm[0].ranks) - 1),
	};
	emc_write(EMC_PHY_CTRL, epc.word);

	mem_config_ndelay(1500);

	if (dp->type == RDIMM || dp->type == LRDIMM)
		rcd_config_seq();

	if (dp->type == LRDIMM)
		db_config_seq();

	mrs_config_seq();

	mem_config_ndelay(500);

	/* Perform the ZQCL Command to all present Physical ranks. */
	for (int i = 0; i < MAX_ACTIVE_RANKS; i++)
		if (dp->active_ranks & (1 << i)) {
			MRS_SDRAM_CMD(MR_RANK(i), ZQCL_DATA);
			mem_config_ndelay(2000);
		}

	/* Perform the DB ZQCL Command to Rank 0 of present DIMMs if LRDIMM. */
	if (dp->type == LRDIMM) {
		for (int i = 0; i < 1 + 2 * (dp->dimm[1].ranks > 0); i += 2) {
			mrs_write(MR_RANK(i), 7, FyBC7x + 0x1000, 0, 0x10, 0);
			mrs_write(MR_RANK(i), 7, F0BC06 + 0x1001, 0, 0x10, 1);
			mem_config_ndelay(2000);
		}
	}

	mem_config_ndelay(500);

	MEM_VERB(2, "Performing static VREF setup.\n");
	vref_static_setup();
}

/*
 * Initiate the hardware to do DDR PHY write leveling.
 * Return 1 on success or 0 if failed.
 */
static int ddr_phy_write_leveling(void)
{
	/*
	 * Start the PUB control operation of the DDR PHY write
	 * leveling.
	 */
	PUB_PGSR0_t pgsr0_done = { .wldone = 1 };
	PUB_PGSR0_t pgsr0_err = { .wlerr = 1 };

	if (!pir_cmd(0x00040201, 1 * NS_PER_MS, "DDR PHY write leveling",
	    pgsr0_done.word, pgsr0_err.word)) {
		ERR_SHOW_PUB_DXn(GSR0);
		ERR_EACH_POP_RANK(
			ERR_SHOW_PUB_DXn(GTR0);
			ERR_SHOW_PUB_DXn(LCDLR0);
		);
		if (dp->package == PACKAGE_x4)
			ERR_SHOW_PUB_DXn(GSR4);
		return 0;
	}

	/* Perform DDR PHY Standalone DQS Gate Training. */
	PUB_DTCR1_t dtcr1 = { .word = pub_read(PUB_DTCR1) };
	dtcr1.rdprmbl_trn = 1;
	pub_write(PUB_DTCR1, dtcr1.word);

	return 1;
}

/*
 * Initiate the hardware to do DDR PHY DQS gate training.
 * Return 1 on success or 0 if failed.
 */
static int ddr_phy_dqs_gate_training(void)
{

	/* Perform DDR PHY Standalone DQS Gate Training. */
	PUB_DTCR1_t dtcr1 = { .word = pub_read(PUB_DTCR1) };
	dtcr1.rdprmbl_trn = 1;
	pub_write(PUB_DTCR1, dtcr1.word);

	PUB_DXCCR_t dxccr = { .word = pub_read(PUB_DXCCR) };
	dxccr.qscnten = 0;
	pub_write(PUB_DXCCR, dxccr.word);

	/* Start the PUB control operation for PHY DQS gate training. */
	PUB_PGSR0_t pgsr0_done = { .qsgdone = 1 };
	PUB_PGSR0_t pgsr0_err = { .qsgerr = 1 };

	if (!pir_cmd(0x00040401, 1 * NS_PER_MS, "DDR PHY DQS gate training",
	    pgsr0_done.word, pgsr0_err.word)) {
		ERR_SHOW_PUB_DXn(GSR0);
		ERR_SHOW_PUB_DXn(RSR0);
		ERR_SHOW_PUB_DXn(RSR1);
		ERR_EACH_POP_RANK(
			ERR_SHOW_PUB_DXn(GTR0);
			ERR_SHOW_PUB_DXn(LCDLR2);
		);
		if (dp->package == PACKAGE_x4)
			ERR_SHOW_PUB_DXn(GSR4);
		return 0;
	}

	dxccr.word = pub_read(PUB_DXCCR);
	dxccr.qscnten = 1;
	pub_write(PUB_DXCCR, dxccr.word);

	return 1;
}

/*
 * Read all PHY Byte Lanes static data located at the DBDQ field of the DXnGSR2
 * PUB register into an array with given places to store.
 */
static void lrdimm_get_dxngsr2_dbdq(int dist, uint8_t *result)
{
	for (int byte = 0; byte < BYTELANE_NUM; byte++) {
		/* FIXME We don't have DXnGSR2 defined currently. */
		uint32_t dxngsr2;
		int dxngsr2_id;
		uint32_t dbdq = UINT32_MAX;

		dxngsr2_id = PUB_DX0GSR2 +
			     byte * (PUB_DX1GSR2 - PUB_DX0GSR2);

		for (int j = 0; j < 10; j++) {
			/* Get the DBDQ value from DXnGSR2. */
			dxngsr2 = pub_read(dxngsr2_id);
			dbdq &= dxngsr2 & 0xff;
		}

		result[byte * dist] = dbdq;
	}

}

/* Error message when a data buffer train sequence yields no result. */
#define DB_ERR_MSG(step, dimm, rank, byte, nibble, result) \
	MEM_ERR(1, "%s training dimm %d rank %d byte %d %s nibble failed. " \
		"Results all %ss.\n", step, dimm, rank, byte, \
		nibble ? "upper" : "lower", result ? "one" : "zero")

/*
 * For each byte lane find where the transition point is and store the index
 * in the given nib[][] array.
 * Sets the value pointed to by rval pointer if failed.
 */
static void lrdimm_find_nibble_change(uint8_t datas[BYTELANE_NUM][64],
				      uint8_t nib[BYTELANE_NUM][2],
				      int is_l2h, char *step,
				      int dimm, int rank, int *rval)
{
	int exp_val = 0xf * is_l2h;

	for (int byte = 0; byte < BYTELANE_NUM; byte++) {
		/*
		 * The state for each nibble seeking for change:
		 * 0: initial, 1: was opposite, 2: found changed
		 */
		int nib_state[2] = {0, 0};

		for (int i = 0; i < 64; i++) {
			uint8_t data = datas[byte][i];

			/* Analyze each nibble separately. */
			for (int j = 0; j < 2; j++) {
				uint8_t nib_data = (data >> (4 * j)) & 0xf;

				if (nib_state[j] == 0) {
					if ((nib_data ^ exp_val) == 0xf)
						nib_state[j] = 1;
				} else if (nib_state[j] == 1)
					if (nib_data == exp_val) {
						nib_state[j] = 2;
						nib[byte][j] = i;
					}
			}
		}

		for (int j = 0; j < 2; j++)
			if (nib_state[j] != 2) {
				*rval = 0;
				DB_ERR_MSG(step, dimm, rank, byte, j,
					   is_l2h ^ nib_state[j]);
			}
	}
}

/*
 * For each byte lane find where the transition center is and store the indexs
 * in the required converted way in the nibs and bytes array.
 * Sets the value pointed to by rval pointer to zero if failed.
 */
static void lrdimm_find_center_index(uint8_t datas[BYTELANE_NUM][5][31],
				     uint8_t nibs[BYTELANE_NUM][2],
				     uint8_t bytes[BYTELANE_NUM],
				     char *step, int dimm, int rank, int *rval)
{
	const int bcxx_calc[5] = {0x6, 0x5, 0x0, 0x1, 0x2};

	/* Find the middle index for each byte lane. */
	for (int byte = 0; byte < BYTELANE_NUM; byte++) {
		int nib_reg0[2] = {0, 0};
		int nib_reg1[2] = {0, 0};
		int start_idx[2] = {0, 0};
		int end_idx[2] = {0, 0};
		int state[2] = {0, 0}; /* 0 for scanning 0->f, 1 for f->0. */

		for (int j  = 0; j < 5; j++) for (int i = 0; i < 31; i++) {
			for (int k = 0; k < 2; k++)
				if (state[k] == 0 &&
				    (datas[byte][j][i] & (0xf << (k * 4))) ==
				    (0xf << (k * 4))) {
					state[k] = 1;
					start_idx[k] = j * 31 + i;
				} else if (state[k] == 1 &&
				  (datas[byte][j][i] & (0xf << (k * 4))) == 0) {
					state[k] = 2;
					end_idx[k] = j * 31 + i;
				}
		}

		/* Get the center value for each nibble. */
		for (int k = 0; k < 2; k++) {
			int middle_idx;

			if (start_idx[k] == 0 || state[k] != 2) {
				*rval = 0;
				DB_ERR_MSG(step, dimm, rank, byte, k,
					   state[k] == 1);
			}
			middle_idx = (start_idx[k] + end_idx[k]) / 2;

			nib_reg0[k] = middle_idx / 31;
			nib_reg1[k] = middle_idx % 31;

			/* Convert to the per nibble index. */
			nib_reg0[k] = bcxx_calc[nib_reg0[k]];
			nib_reg1[k] = nib_reg1[k] < 15 ? 0x1f - nib_reg1[k] :
							 nib_reg1[k] - 0xf;

			/* Save the results. */
			bytes[byte] = (bytes[byte] & (0xf << (4 * k))) |
					(nib_reg0[k] << (4 * k));
			nibs[byte][k] = nib_reg1[k];
		}
	}
}

static void lrdimm_db_training_pre_condition(int is_mwd_stage)
{
	for (int byte = 0; byte < BYTELANE_NUM; byte++) {
		/*
		 * @FIXME We don't currently have the definitions for the
		 * DXnGCR1 register.
		 */
		uint32_t dxngcr1;
		PUB_DXnGCR2_t dxngcr2;
		int dxngcr1_id;
		int dxngcr2_id;

		dxngcr1_id = PUB_DX0GCR1 + byte * (PUB_DX1GCR1 - PUB_DX0GCR1);
		dxngcr2_id = PUB_DX0GCR2 + byte * (PUB_DX1GCR2 - PUB_DX0GCR2);

		dxngcr1 = pub_read(dxngcr1_id);
		/* Set the DXPDRMODE field to 0xAAAA. */
		dxngcr1 = (dxngcr1 & ~0xffff) | 0xaaaa;
		pub_write(dxngcr1_id, dxngcr1);

		dxngcr2.word = pub_read(dxngcr2_id);
		dxngcr2.dxtemode = 0x5555;
		if (is_mwd_stage)
			dxngcr2.dxoemode = 0xaaaa;
		pub_write(dxngcr2_id, dxngcr2.word);
	}

	PUB_PGCR1_t pgcr1 = { .word = pub_read(PUB_PGCR1) };
	pgcr1.lrdimmst = 1;
	pub_write(PUB_PGCR1, pgcr1.word);
}

static void lrdimm_db_training_post_restore(int is_mwd_stage)
{
	for (int byte = 0; byte < BYTELANE_NUM; byte++) {
		/*
		 * @FIXME We don't currently have the definitions for the
		 * DXnGCR1 register.
		 */
		uint32_t dxngcr1;
		PUB_DXnGCR2_t dxngcr2;
		int dxngcr1_id;
		int dxngcr2_id;

		dxngcr1_id = PUB_DX0GCR1 + byte * (PUB_DX1GCR1 - PUB_DX0GCR1);
		dxngcr2_id = PUB_DX0GCR2 + byte * (PUB_DX1GCR2 - PUB_DX0GCR2);

		dxngcr1 = pub_read(dxngcr1_id);
		/* Set the DXPDRMODE field to 0x0000. */
		dxngcr1 = dxngcr1 & ~0xffff;
		pub_write(dxngcr1_id, dxngcr1);

		dxngcr2.word = pub_read(dxngcr2_id);
		dxngcr2.dxtemode = 0x0000;
		if (is_mwd_stage)
			dxngcr2.dxoemode = 0x0000;
		pub_write(dxngcr2_id, dxngcr2.word);
	}

	PUB_PGCR1_t pgcr1 = { .word = pub_read(PUB_PGCR1) };
	pgcr1.lrdimmst = 0;
	pub_write(PUB_PGCR1, pgcr1.word);
}

static void ddr_phy_pre_buffer_addressability_setup(void)
{
	for (int byte = 0; byte < BYTELANE_NUM; byte++) {
		PUB_DXnGCR2_t dxngcr2;
		PUB_DXnGCR3_t dxngcr3;
		PUB_DXnGCR7_t dxngcr7;
		int dxngcr2_id;
		int dxngcr3_id;
		int dxngcr7_id;

		dxngcr2_id = PUB_DX0GCR2 + byte * (PUB_DX1GCR2 - PUB_DX0GCR2);
		dxngcr3_id = PUB_DX0GCR3 + byte * (PUB_DX1GCR3 - PUB_DX0GCR3);
		dxngcr7_id = PUB_DX0GCR7 + byte * (PUB_DX1GCR7 - PUB_DX0GCR7);

		dxngcr2.word = pub_read(dxngcr2_id);
		dxngcr2.dxoemode = 0x5555;
		pub_write(dxngcr2_id, dxngcr2.word);

		dxngcr3.word = pub_read(dxngcr3_id);
		dxngcr3.dsoemode = 0x2;
		pub_write(dxngcr2_id, dxngcr3.word);

		dxngcr7.word = pub_read(dxngcr7_id);
		dxngcr7.x4dsoemode = 0x2;
		pub_write(dxngcr2_id, dxngcr7.word);
	}
}

static void ddr_phy_pre_buffer_addressability_restore(void)
{
	for (int byte = 0; byte < BYTELANE_NUM; byte++) {
		PUB_DXnGCR2_t dxngcr2;
		PUB_DXnGCR3_t dxngcr3;
		PUB_DXnGCR7_t dxngcr7;
		int dxngcr2_id;
		int dxngcr3_id;
		int dxngcr7_id;

		dxngcr2_id = PUB_DX0GCR2 + byte * (PUB_DX1GCR2 - PUB_DX0GCR2);
		dxngcr3_id = PUB_DX0GCR3 + byte * (PUB_DX1GCR3 - PUB_DX0GCR3);
		dxngcr7_id = PUB_DX0GCR7 + byte * (PUB_DX1GCR7 - PUB_DX0GCR7);

		dxngcr2.word = pub_read(dxngcr2_id);
		dxngcr2.dxoemode = 0x0000;
		pub_write(dxngcr2_id, dxngcr2.word);

		dxngcr3.word = pub_read(dxngcr3_id);
		dxngcr3.dsoemode = 0x0;
		pub_write(dxngcr2_id, dxngcr3.word);

		dxngcr7.word = pub_read(dxngcr7_id);
		dxngcr7.x4dsoemode = 0x0;
		pub_write(dxngcr2_id, dxngcr7.word);
	}
}

static void ddr_phy_pre_buffer_addressability_bytes_enable_all(void)
{
	for (int byte = 0; byte < BYTELANE_NUM; byte++)
		emc_write(EMC_IND_DATA__FIRST_WORD +
			  byte * EMC_IND_DATA__STRIDE, 0x00000000);
	emc_write(EMC_IND_CMD, 0x00000701);

	EMC_MC_DDR_IF_t emdi = { .word = emc_read(EMC_MC_DDR_IF) };
	emdi.ppr_en = 1;
	emc_write(EMC_MC_DDR_IF, emdi.word);

	pub_indirect_write(MC_SPECIAL_CTRL, 0x80000000);
}

static void ddr_phy_pre_buffer_addressability_bytes_enable_single(int en_byte)
{
	for (int byte = 0; byte < BYTELANE_NUM; byte++)
		emc_write(EMC_IND_DATA__FIRST_WORD +
			  byte * EMC_IND_DATA__STRIDE,
			  !(byte == en_byte) * 0xffffffff);

	emc_write(EMC_IND_CMD, 0x00000701);
	SET_MEM_REG_FIELD(emc, EMC_MC_DDR_IF, ppr_en, 1);
	pub_indirect_write(MC_SPECIAL_CTRL, 0x80000000);
}

/* Internal sequence taken from mrep training function for less indenting. */
static void lrdimm_db_mrep_training_internal_loop(int dimm, int rank, int *rval)
{
	int dimm_db_rank = 2 * dimm;
	int dram_rank = 2 * dimm + rank;
	uint8_t mrep_result[BYTELANE_NUM][64] = {0};
	uint8_t nib[BYTELANE_NUM][2];

	/*
	 * Modify the SDRAM Devices MR3 to MPR mode and the MR4 to Read
	 * Preamble Training.
	 */
	mrs_write(dram_rank, 0x3, MR(3, dram_rank) + 0x0004, 0, 0x10, 1);
	mrs_write(dram_rank, 0x4, MR(4, dram_rank) + 0x0400, 0, 0x10, 1);
	mrs_write_inv(dram_rank, 0x3, MR(3, dram_rank) + 0x0004, 0, 0x10, 1,
		      RC(00, dimm));
	mrs_write_inv(dram_rank, 0x4, MR(4, dram_rank) + 0x0400, 0, 0x10, 1,
		      RC(00, dimm));

	/* Set the DB Rank Presence value. */
	mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x107f - (1 << rank), 0, 0x10, 1);

	/*
	 * Select the DB Function Register according to the DIMM’s current
	 * trained rank.
	 */
	mrs_write(dimm_db_rank, 0x7, 0x1700 + rank, 0, 0x10, 1);

	for (int i = 0; i < 64; i++) {
		/* Set the DB BC2x & BC3x registers to value of i. */
		mrs_write(dimm_db_rank, 0x7, 0x1200 + i, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1300 + i, 0, 0x10, 1);

		/* Start EMEM_MI and EMEM_MC Endless Read operation. */
		emi_write(EMI_IND_ADDR, 0x00000000);
		emi_write(EMI_IND_CMD, 0xfe000fe + 0x4000 * dram_rank);
		mem_config_ndelay(1 * NS_PER_US);

		/*
		 * Read all PHY Byte Lanes static data located at the DBDQ field
		 * of the DXnGSR2 PUB register.
		 */
		lrdimm_get_dxngsr2_dbdq(64, &mrep_result[0][i]);


		/* Stop EMEM_MI and EMEM_MC Endless Read operation. */
		emi_write(EMI_IND_CMD, 0xfe000ff + 0x4000 * dram_rank);
		mem_config_ndelay(1 * NS_PER_US);
	}

	/* Search where the transition from 0xf to 0x0 happens. */
	lrdimm_find_nibble_change(mrep_result, nib, 0, "MREP", dimm, rank,
				  rval);

	/* Save the MREP results at the DB Control Words BC2x and BC3x. */
	mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x10c0, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x1101, 0, 0x10, 1);

	ddr_phy_pre_buffer_addressability_setup();
	ddr_phy_pre_buffer_addressability_bytes_enable_all();

	mrs_write(dimm_db_rank, 0x7, 0x1700 + rank, 0, 0x10, 1);

	for (int byte = 0; byte < BYTELANE_NUM; byte++) {
		ddr_phy_pre_buffer_addressability_bytes_enable_single(byte);
		mrs_write(dimm_db_rank, 0x7, 0x1200 + nib[byte][0], 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1300 + nib[byte][1], 0, 0x10, 1);
	}
	ddr_phy_pre_buffer_addressability_bytes_enable_all();

	mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x1100, 0, 0x10, 1);

	ddr_phy_pre_buffer_addressability_restore();

	mrs_write(dimm_db_rank, 0x7, 0x10c1, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x1700 + rank, 0, 0x10, 1);

	/* Restore the SDRAM Devices MR3 and MR4 values. */
	mrs_write(dram_rank, 0x3, MR(3, dram_rank), 0, 0x10, 1);
	mrs_write(dram_rank, 0x4, MR(4, dram_rank), 0, 0x10, 1);
	mrs_write_inv(dram_rank, 0x3, MR(3, dram_rank), 0, 0x10, 1,
		      RC(00, dram_rank));
	mrs_write_inv(dram_rank, 0x4, MR(4, dram_rank), 0, 0x10, 1,
		      RC(00, dram_rank));
}

static int lrdimm_db_mrep_training(void)
{
	int rval = 0;

	lrdimm_db_training_pre_condition(0);

	/* Set the EMEM_MC to MPR operational mode. */
	EMC_WRD_LVL_MODES_t ewlm = { .word = emc_read(EMC_WRD_LVL_MODES) };
	ewlm.mpr_mode = (4 * ((1 << dp->dimm[1].ranks) - 1) +
			(1 << dp->dimm[0].ranks) - 1);
	emc_write(EMC_WRD_LVL_MODES, ewlm.word);

	/* Set the EMEM_MI general setup for Endless Read. */
	emi_write(EMI_REQ_SEQUENCE, 0x0003033f);
	emi_write(EMI_RD_RES, 0x00000020);

	/* Enable the EMEM_MC for Write/Read operations only. */
	EMC_MC_DDR_IF_t emdi = { .mc_en = 1 };
	emc_write(EMC_MC_DDR_IF, emdi.word);

	for (int dimm = 0; dimm < MAX_DIMM_PER_MEM_CTRL; dimm++) {

		if (dp->dimm[dimm].ranks == 0)
			continue;

		int dimm_db_rank = 2 * dimm;

		/* Enable the DB MREP mode. */
		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x10c1, 0, 0x10, 1);

		for (int rank = 0; rank < dp->dimm[dimm].ranks; rank++)
			lrdimm_db_mrep_training_internal_loop(dimm, rank,
							      &rval);

		/*
		 * Disable the DIMM DB MREP mode and Restore the DIMM DB Rank
		 * Presence value.
		 */
		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x10c0, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x107f + F0BC(07, dimm), 0, 0x10,
			  1);
	}

	emdi.word = emc_read(EMC_MC_DDR_IF);
	emdi.mc_en = 0;
	emc_write(EMC_MC_DDR_IF, emdi.word);

	ewlm.word = emc_read(EMC_WRD_LVL_MODES);
	ewlm.mpr_mode = 0;
	emc_write(EMC_WRD_LVL_MODES, ewlm.word);

	lrdimm_db_training_post_restore(0);

	return rval;
}

/* Internal sequence taken from mrd training function for less indenting. */
static void lrdimm_db_mrd_training_internal_loop(int dimm, int rank, int *rval)
{
	const int bccx_calc[5] = {0x6, 0x5, 0x0, 0x1, 0x2};

	int dimm_db_rank = 2 * dimm;
	int dram_rank = 2 * dimm + rank;
	uint8_t mrd_result[BYTELANE_NUM][5][31] = {0};
	uint8_t mrd_nib[BYTELANE_NUM][2];
	uint8_t mrd_byte[BYTELANE_NUM] = {0};

	/* Modify the SDRAM Devices MR3 to MPR mode. */
	mrs_write(dram_rank, 0x3, MR(3, dram_rank) + 0x0004, 0, 0x10, 1);
	mrs_write_inv(dram_rank, 0x3, MR(3, dram_rank) + 0x0004, 0, 0x10, 1,
		      RC(00, dimm));

	/* Modify the SDRAM Devices MR5 to RTT_PARK mode enabled. */
	mrs_write(dram_rank, 0x5, MR(5, dram_rank) + 0x0040, 0, 0x10, 1);
	mrs_write_inv(dram_rank, 0x5, MR(5, dram_rank) + 0x0040, 0, 0x10, 1,
		      RC(00, dimm));

	/* Set the DB Rank Presence value. */
	mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x107f - (1 << rank), 0, 0x10, 1);

	for (int bccx = 0; bccx < 5; bccx++) {
		/*
		 * Select the DB Function Register according to the DIMM’s
		 * current trained rank.
		 */
		mrs_write(dimm_db_rank, 0x7, 0x1700 + rank % 2, 0, 0x10, 1);

		/* Set the DB BCCx or BCEx registers to the bccx_val value. */
		mrs_write(dimm_db_rank, 0x7, 0x1C00 + 0x200 * (rank > 1) +
			  0x11 * bccx_calc[bccx], 0, 0x10, 1);

		/*
		 * Select the DB Function Register according to the DIMM’s
		 * current trained rank.
		 */
		mrs_write(dimm_db_rank, 0x7, 0x1700 + rank, 0, 0x10, 1);

		for (int i = 0; i < 31; i++) {
			int bc45 = i < 15 ? 0x1f - i : i - 0xf;

			/* Set the DB BC4x & BC5x registers to the bc45. */
			mrs_write(dimm_db_rank, 0x7, 0x1400 + bc45, 0, 0x10, 1);
			mrs_write(dimm_db_rank, 0x7, 0x1500 + bc45, 0, 0x10, 1);

			/* Perform EMEM_MI and EMEM_MC Single Read operation. */
			emi_write(EMI_IND_ADDR, 0x40 * dram_rank);
			emi_write(EMI_IND_CMD, 0xff00001 + 0x4000 * dram_rank);
			mem_config_ndelay(1 * NS_PER_US);

			/*
			 * Read all PHY Byte Lanes static data located at the
			 * DBDQ field of the DXnGSR2 PUB register.
			 */
			lrdimm_get_dxngsr2_dbdq(5 * 31,
						&mrd_result[0][bccx][i]);
		}
	}

	/* Find the middle index for each byte lane. */
	lrdimm_find_center_index(mrd_result, mrd_nib, mrd_byte, "MRD", dimm,
				 rank, rval);

	/*
	 * Save the MRD results at the DB Control Words BCCx or BCEx and BC4x
	 * and BC5x.
	 */
	mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x10c0, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x1101, 0, 0x10, 1);

	ddr_phy_pre_buffer_addressability_setup();
	ddr_phy_pre_buffer_addressability_bytes_enable_all();

	for (int byte = 0; byte < BYTELANE_NUM; byte++) {
		mrs_write(dimm_db_rank, 0x7, 0x1700 + rank % 2, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1C00 + 0x200 * (rank > 1) +
			  mrd_byte[byte], 0, 0, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1700 + rank, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1400 + mrd_nib[byte][0], 0,
			  0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1500 + mrd_nib[byte][1], 0,
			  0x10, 1);
	}
	ddr_phy_pre_buffer_addressability_bytes_enable_all();

	mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x1100, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x10c6, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x1700 + rank, 0, 0x10, 1);

	/* Restore the SDRAM Devices MR3 and MR5 values. */
	mrs_write(dram_rank, 0x3, MR(3, dram_rank), 0, 0x10, 1);
	mrs_write_inv(dram_rank, 0x3, MR(3, dram_rank), 0, 0x10, 1,
		      RC(00, dimm));
	mrs_write(dram_rank, 0x5, MR(5, dram_rank), 0, 0x10, 1);
	mrs_write_inv(dram_rank, 0x5, MR(5, dram_rank), 0, 0x10, 1,
		      RC(00, dimm));
}

static int lrdimm_db_mrd_training(void)
{
	int rval = 1;

	lrdimm_db_training_pre_condition(0);

	/* Set the EMEM_MC to MPR operational mode. */
	EMC_WRD_LVL_MODES_t ewlm = { .word = emc_read(EMC_WRD_LVL_MODES) };
	ewlm.mpr_mode = (4 * ((1 << dp->dimm[1].ranks) - 1) +
			(1 << dp->dimm[0].ranks) - 1);
	emc_write(EMC_WRD_LVL_MODES, ewlm.word);

	/* Enable the EMEM_MC for Write/Read operations only. */
	EMC_MC_DDR_IF_t emdi = { .mc_en = 1 };
	emc_write(EMC_MC_DDR_IF, emdi.word);

	for (int dimm = 0; dimm < MAX_DIMM_PER_MEM_CTRL; dimm++) {

		if (dp->dimm[dimm].ranks == 0)
			continue;

		int dimm_db_rank = 2 * dimm;

		/* Set the DIMM DB MPR data for a proper MRD training. */
		mrs_write(dimm_db_rank, 0x7, 0x1705, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1000, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x11ff, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1200, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x13ff, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1706, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1000, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x11ff, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1200, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x13ff, 0, 0x10, 1);

		/* Configure the DIMM DB for per lane failure indicator mode. */
		mrs_write(dimm_db_rank, 0x7, 0x1706, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1401, 0, 0x10, 1);

		/* Put the RCD into Side B None Inversion mode. */
		RC(00, dimm) |= 0x1;
		mrs_write(dimm_db_rank, 0x7, RC00 + RC(00, dimm), 0, 0x10, 1);

		/* Enable the DB MRD mode. */
		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x10c6, 0, 0x10, 1);

		for (int rank = 0; rank < dp->dimm[dimm].ranks; rank++)
			lrdimm_db_mrd_training_internal_loop(dimm, rank, &rval);

		/* Restore the RCD Side B Inversion mode. */
		RC(00, dimm) &= ~0x1;
		mrs_write(dimm_db_rank, 0x7, RC00 + RC(00, dimm), 0, 0x10, 1);

		/* Restore the DIMM DB Training Configuration to normal mode. */
		mrs_write(dimm_db_rank, 0x7, 0x1706, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1400, 0, 0x10, 1);

		/*
		 * Disable the DIMM DB MRD mode and Restore the DIMM DB Rank
		 * Presence value.
		 */
		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x10c0, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1070 + F0BC(07, dimm), 0, 0x10,
			  1);
	}

	emdi.word = emc_read(EMC_MC_DDR_IF);
	emdi.mc_en = 0;
	emc_write(EMC_MC_DDR_IF, emdi.word);

	ewlm.word = emc_read(EMC_WRD_LVL_MODES);
	ewlm.mpr_mode = 0;
	emc_write(EMC_WRD_LVL_MODES, ewlm.word);

	lrdimm_db_training_post_restore(0);

	return rval;
}

/* Internal sequence taken from dwl training function for less indenting. */
static void lrdimm_db_dwl_training_internal_loop(int dimm, int rank, int *rval)
{
	int dimm_db_rank = 2 * dimm;
	int dram_rank = 2 * dimm + rank;
	uint8_t dwl_result[BYTELANE_NUM][64] = {0};
	uint8_t nib[BYTELANE_NUM][2];

	/* Modify the SDRAM Devices MR1 to Write Leveling mode. */
	mrs_write(dram_rank, 0x1, MR(1, dram_rank) + 0x0080, 0, 0x10, 1);
	mrs_write_inv(dram_rank, 0x1, MR(1, dram_rank) + 0x0080, 0, 0x10, 1,
		      RC(00, dimm));

	/* Set the DB Rank Presence value. */
	mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x107f - (1 << rank), 0, 0x10, 1);

	/*
	 * Set the DB Rank Number Selection Control Word value according to the
	 * DIMM’s current trained rank.
	 */
	mrs_write(dimm_db_rank, 0x7, 0x1080 + rank, 0, 0x10, 1);

	/* Enter the DB DWL mode. */
	mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x10c4, 0, 0x10, 1);

	/*
	 * Select the DB Function Register according to the DIMM’s current
	 * trained rank.
	 */
	mrs_write(dimm_db_rank, 0x7, 0x1700 + rank, 0, 0x10, 1);

	for (int i = 0; i < 64; i++) {
		/* Set the DB BCAx & BCBx registers to value of i. */
		mrs_write(dimm_db_rank, 0x7, 0x1a00 + i, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1b00 + i, 0, 0x10, 1);
		mem_config_ndelay(1 * NS_PER_US);

		/*
		 * Read all PHY Byte Lanes static data located at the DBDQ field
		 * of the DXnGSR2 PUB register.
		 */
		lrdimm_get_dxngsr2_dbdq(64, &dwl_result[0][i]);
	}

	/* Search where the transition from 0x0 to 0xf happens. */
	lrdimm_find_nibble_change(dwl_result, nib, 1, "DWL", dimm, rank, rval);

	/* Save the DWL results at the DB Control Words BCAx and BCBx. */
	mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x10c0, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x1101, 0, 0x10, 1);

	ddr_phy_pre_buffer_addressability_setup();
	ddr_phy_pre_buffer_addressability_bytes_enable_all();

	mrs_write(dimm_db_rank, 0x7, 0x1700 + rank, 0, 0x10, 1);

	for (int byte = 0; byte < BYTELANE_NUM; byte++) {
		ddr_phy_pre_buffer_addressability_bytes_enable_single(byte);
		mrs_write(dimm_db_rank, 0x7, 0x1a00 + nib[byte][0], 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1b00 + nib[byte][1], 0, 0x10, 1);
	}
	ddr_phy_pre_buffer_addressability_bytes_enable_all();

	mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x1100, 0, 0x10, 1);

	ddr_phy_pre_buffer_addressability_restore();

	mrs_write(dimm_db_rank, 0x7, 0x1700 + rank, 0, 0x10, 1);

	/* Restore the SDRAM Devices MR1 value. */
	mrs_write(dram_rank, 0x1, MR(1, dram_rank), 0, 0x10, 1);
	mrs_write_inv(dram_rank, 0x1, MR(1, dram_rank), 0, 0x10, 1,
		      RC(00, dimm));
}

static int lrdimm_db_dwl_training(void)
{
	int rval = 1;

	lrdimm_db_training_pre_condition(0);

	for (int dimm = 0; dimm < MAX_DIMM_PER_MEM_CTRL; dimm++) {

		if (dp->dimm[dimm].ranks == 0)
			continue;

		int dimm_db_rank = 2 * dimm;

		for (int rank = 0; rank < dp->dimm[dimm].ranks; rank++)
			lrdimm_db_dwl_training_internal_loop(dimm, rank, &rval);

		/*
		 * Restore the DIMM DB Rank Presence value and the DB Rank
		 * Number Selectin Control Word value.
		 */
		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1070 + F0BC(07, dimm), 0, 0x10,
			  1);
		mrs_write(dimm_db_rank, 0x7, 0x1080, 0, 0x10, 1);
	}

	lrdimm_db_training_post_restore(0);

	return rval;
}

/* Internal sequence taken from mwd training function for less indenting. */
static void lrdimm_db_mwd_training_internal_loop(int dimm, int rank, int *rval)
{
	const int bcdx_calc[5] = {0x6, 0x5, 0x0, 0x1, 0x2};

	int dimm_db_rank = 2 * dimm;
	int dram_rank = 2 * dimm + rank;
	uint8_t mwd_result[BYTELANE_NUM][5][31] = {0};
	uint8_t mwd_nib[BYTELANE_NUM][2];
	uint8_t mwd_byte[BYTELANE_NUM] = {0};
	uint64_t rand_state = 3;

	/* Modify the SDRAM Devices MR5 to RTT_PARK mode enabled. */
	mrs_write(dram_rank, 0x5, MR(5, dram_rank) + 0x0040, 0, 0x10, 1);
	mrs_write_inv(dram_rank, 0x5, MR(5, dram_rank) + 0x0040, 0, 0x10, 1,
		      RC(00, dimm));

	/* Set the DB Rank Presence value. */
	mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x107f - (1 << rank), 0, 0x10, 1);

	for (int bcdx = 0; bcdx < 5; bcdx++) {
		/*
		 * Select the DB Function Register according to the DIMM’s
		 * current trained rank.
		 */
		mrs_write(dimm_db_rank, 0x7, 0x1700 + rank % 2, 0, 0x10, 1);

		/* Set the DB BCDx or BCFx registers to the bcdx_val value. */
		mrs_write(dimm_db_rank, 0x7, 0x1D00 + 0x200 * (rank > 1) +
			  0x11 * bcdx_calc[bcdx], 0, 0x10, 1);

		/*
		 * Select the DB Function Register according to the DIMM’s
		 * current trained rank.
		 */
		mrs_write(dimm_db_rank, 0x7, 0x1700 + rank, 0, 0x10, 1);

		for (int i = 0; i < 31; i++) {
			int bc89 = i < 15 ? 0x1f - i : i - 0xf;

			/* Set the DB BC8x & BC9x registers to the bc89. */
			mrs_write(dimm_db_rank, 0x7, 0x1800 + bc89, 0, 0x10, 1);
			mrs_write(dimm_db_rank, 0x7, 0x1900 + bc89, 0, 0x10, 1);

			/*
			 * Perform EMEM_MI and EMEM_MC Single Write operation
			 * with random Address and Bank.
			 */
			int rand_bank = rand_gen(&rand_state) & 0x3;
			int rand_addr = rand_gen(&rand_state) & ((1 << 24) - 1);
			emi_write(EMI_IND_ADDR, rand_addr);
			emi_write(EMI_IND_CMD, 0xff00002 + 0x4000 * dram_rank +
						0x8 * rand_bank);
			mem_config_ndelay(1 * NS_PER_US);

			/*
			 * Perform EMEM_MI and EMEM_MC Single Read operation to
			 * the same random Address and Bank.
			 */
			emi_write(EMI_IND_ADDR, rand_addr);
			emi_write(EMI_IND_CMD, 0xff00001 + 0x4000 * dram_rank +
						0x8 * rand_bank);
			mem_config_ndelay(1 * NS_PER_US);

			/*
			 * Read all PHY Byte Lanes static data located at the
			 * DBDQ field of the DXnGSR2 PUB register.
			 */
			lrdimm_get_dxngsr2_dbdq(5 * 31,
						&mwd_result[0][bcdx][i]);

			/*
			 * Update the DIMM DB MPR data with new Random Data
			 * values.
			 */
			mrs_write(dimm_db_rank, 0x7, 0x1705, 0, 0x10, 1);
			mrs_write(dimm_db_rank, 0x7, 0x1000, 0, 0x10, 1);
			mrs_write(dimm_db_rank, 0x7, 0x1100 +
				  (rand_gen(&rand_state) & 0xff), 0, 0x10, 1);
			mrs_write(dimm_db_rank, 0x7, 0x1200 +
				  (rand_gen(&rand_state) & 0xff), 0, 0x10, 1);
			mrs_write(dimm_db_rank, 0x7, 0x1300 +
				  (rand_gen(&rand_state) & 0xff), 0, 0x10, 1);
			mrs_write(dimm_db_rank, 0x7, 0x1706, 0, 0x10, 1);
			mrs_write(dimm_db_rank, 0x7, 0x1000 +
				  (rand_gen(&rand_state) & 0xff), 0, 0x10, 1);
			mrs_write(dimm_db_rank, 0x7, 0x1100 +
				  (rand_gen(&rand_state) & 0xff), 0, 0x10, 1);
			mrs_write(dimm_db_rank, 0x7, 0x1200 +
				  (rand_gen(&rand_state) & 0xff), 0, 0x10, 1);
			mrs_write(dimm_db_rank, 0x7, 0x1300, 0, 0x10, 1);

			/*
			 * Re-Select the DB Function Register according to the
			 * DIMM’s current trained rank.
			 */
			mrs_write(dimm_db_rank, 0x7, 0x1700 + rank, 0, 0x10, 1);
		}
	}

	/* Find the middle index for each byte lane. */
	lrdimm_find_center_index(mwd_result, mwd_nib, mwd_byte, "MWD", dimm,
				 rank, rval);

	/*
	 * Save the MRD results at the DB Control Words BCDx or BCFx and BC8x
	 * and BC9x.
	 */
	mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x10c0, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x1101, 0, 0x10, 1);

	ddr_phy_pre_buffer_addressability_setup();
	ddr_phy_pre_buffer_addressability_bytes_enable_all();

	for (int byte = 0; byte < BYTELANE_NUM; byte++) {
		mrs_write(dimm_db_rank, 0x7, 0x1700 + rank % 2, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1D00 + 0x200 * (rank > 1) +
			  mwd_byte[byte], 0, 0, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1700 + rank, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1800 + mwd_nib[byte][0], 0,
			  0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1900 + mwd_nib[byte][1], 0,
			  0x10, 1);
	}
	ddr_phy_pre_buffer_addressability_bytes_enable_all();

	mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x1100, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x10c7, 0, 0x10, 1);
	mrs_write(dimm_db_rank, 0x7, 0x1700 + rank, 0, 0x10, 1);

	/* Restore the SDRAM Device MR5 value. */
	mrs_write(dram_rank, 0x5, MR(5, dram_rank), 0, 0x10, 1);
	mrs_write_inv(dram_rank, 0x5, MR(5, dram_rank), 0, 0x10, 1,
		      RC(00, dimm));
}

static int lrdimm_db_mwd_training(void)
{
	int rval = 1;
	uint64_t rand_state = 4;

	lrdimm_db_training_pre_condition(1);

	/* Enable the EMEM_MC for Write/Read operations only. */
	EMC_MC_DDR_IF_t emdi = { .mc_en = 1 };
	emc_write(EMC_MC_DDR_IF, emdi.word);

	for (int dimm = 0; dimm < MAX_DIMM_PER_MEM_CTRL; dimm++) {

		if (dp->dimm[dimm].ranks == 0)
			continue;

		int dimm_db_rank = 2 * dimm;

		/*
		 * Set the DIMM DB MPR data with Random Data values to be used
		 * in the MWD training.
		 */
		mrs_write(dimm_db_rank, 0x7, 0x1705, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1000, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1100 +
			  (rand_gen(&rand_state) & 0xff), 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1200 +
			  (rand_gen(&rand_state) & 0xff),
			  0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1300 +
			  (rand_gen(&rand_state) & 0xff),
			  0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1706, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1000 +
			  (rand_gen(&rand_state) & 0xff),
			  0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1100 +
			  (rand_gen(&rand_state) & 0xff),
			  0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1200 +
			  (rand_gen(&rand_state) & 0xff),
			  0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1300, 0, 0x10, 1);

		/* Configure the DIMM DB for per lane failure indicator mode. */
		mrs_write(dimm_db_rank, 0x7, 0x1706, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1401, 0, 0x10, 1);

		/* Enable the DB MWD mode. */
		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x10c7, 0, 0x10, 1);

		for (int rank = 0; rank < dp->dimm[dimm].ranks; rank++)
			lrdimm_db_mwd_training_internal_loop(dimm, rank, &rval);

		/* Restore the DIMM DB Training Configuration to normal mode. */
		mrs_write(dimm_db_rank, 0x7, 0x1706, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1400, 0, 0x10, 1);

		/*
		 * Disable the DIMM DB MWD mode and Restore the DIMM DB Rank
		 * Presence value.
		 */
		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x10c0, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x107f + F0BC(07, dimm), 0, 0x10,
			  1);
	}

	emdi.word = emc_read(EMC_MC_DDR_IF);
	emdi.mc_en = 0;
	emc_write(EMC_MC_DDR_IF, emdi.word);

	lrdimm_db_training_post_restore(1);

	return rval;
}

/*
 * Do LRDIMM data buffer training sequence, return 1 if successful or
 * 0 if any of the training did not yield a result.
 */
static int lrdimm_db_data_training(void)
{
	int rval = 1;

	/*
	 * If we fail in any one of them, we continue until all is done.
	 * Thus in the error logs we have more than just the first encountered
	 * error.
	 */
	rval &= lrdimm_db_mrep_training();

	rval &= lrdimm_db_mrd_training();

	rval &= lrdimm_db_dwl_training();

	rval &= lrdimm_db_mwd_training();

	/* Post LRDIMM DB Training Memory Controller Recovery flow. */
	MC_PUP_CTRL_t mpc = { .mc_sw_rd_rst = 1 };
	pub_indirect_write(MC_PUP_CTRL, mpc.word);
	mem_config_ndelay(1000);
	mpc.mc_sw_rd_rst = 0;
	pub_indirect_write(MC_PUP_CTRL, mpc.word);
	mem_config_ndelay(1000);

	return rval;
}

/* Also called the LRDIMM DDR PHY HWL Training flow. */
static int lrdimm_standalone_write_leveling(void)
{
	int rval = 1;
	PUB_DTCR1_t dtcr1 = { .word = pub_read(PUB_DTCR1) };
	PUB_DTCR1_t dtcr1_temp = dtcr1;

	for (int dimm = 0; dimm < MAX_DIMM_PER_MEM_CTRL; dimm++) {

		if (dp->dimm[dimm].ranks == 0)
			continue;

		int dimm_db_rank = 2 * dimm;

		/* Disable all DIMM's RTT_PARK. */
		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1000 | F0BC02, 0, 0x10, 1);
	}

	PUB_PGCR1_t pgcr1 = { .word = pub_read(PUB_PGCR1) };
	pgcr1.lrdimmst = 1;
	pub_write(PUB_PGCR1, pgcr1.word);

	for (int dimm = 0; dimm < MAX_DIMM_PER_MEM_CTRL; dimm++) {

		if (dp->dimm[dimm].ranks == 0)
			continue;

		int dimm_db_rank = 2 * dimm;
		int ddr_phy_rank = 256 << dimm;

		dtcr1_temp.ranken = ddr_phy_rank;
		pub_write(PUB_DTCR1, dtcr1_temp.word);

		/* Set the DB Rank Presence value to 0. */
		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x107e, 0, 0x10, 1);

		/*
		 * Set the DB Rank Number Selection Control Word value
		 * to rank 0.
		 */
		mrs_write(dimm_db_rank, 0x7, 0x1081, 0, 0x10, 1);

		/* Enable DB DWL mode. */
		mrs_write(dimm_db_rank, 0x7, 0x10c5, 0, 0x10, 1);

		/* Perform regular Standalone Write Leveling. */
		if (!ddr_phy_write_leveling()) {
			MEM_ERR(1, "HWL failed for %d dimm.\n", dimm);
			rval = 0;
		}

		/* Disable the DB DWL mode. */
		mrs_write(dimm_db_rank, 0x7, 0x10c0, 0, 0x10, 1);

		/* Restore the DB Rank Number Selection Control Word value. */
		mrs_write(dimm_db_rank, 0x7, 0x1000 | F0BC08 | F0BC(08, dimm),
			  0, 0x10, 1);

		/* Restore the DB Rank Presence Control Word value. */
		mrs_write(dimm_db_rank, 0x7, 0x1000 | F0BC07 | F0BC(07, dimm),
			  0, 0x10, 1);
	}

	/* Restore the DTCR1.ranken value. */
	pub_write(PUB_DTCR1, dtcr1.word);
	pgcr1.lrdimmst = 0;
	pub_write(PUB_PGCR1, pgcr1.word);

	for (int dimm = 0; dimm < MAX_DIMM_PER_MEM_CTRL; dimm++) {

		if (dp->dimm[dimm].ranks == 0)
			continue;

		int dimm_db_rank = 2 * dimm;

		/* Restore all DIMM's RTT_PARK. */
		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1000 | F0BC02 | F0BC(02, dimm),
			  0, 0x10, 1);
	}

	return rval;
}

/* Also called the LRDIMM DDR PHY HIR Training flow. */
static int lrdimm_standalone_dqs_gate_leveling(void)
{
	int rval = 1;
	PUB_DTCR1_t dtcr1_orig = { .word = pub_read(PUB_DTCR1) };

	PUB_DTCR0_t dtcr0 = { .word = pub_read(PUB_DTCR0) };
	PUB_DTCR0_t dtcr0_orig = dtcr0;
	dtcr0.rfshdt = 0;
	pub_write(PUB_DTCR0, dtcr0.word);

	for (int dimm = 0; dimm < MAX_DIMM_PER_MEM_CTRL; dimm++) {

		if (dp->dimm[dimm].ranks == 0)
			continue;

		int dimm_db_rank = 2 * dimm;

		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1101, 0, 0x10, 1);

		ddr_phy_pre_buffer_addressability_setup();
		ddr_phy_pre_buffer_addressability_bytes_enable_all();

		/*
		 * Set the Byte Lanes drive data for Per DB Addressability
		 * operation on bytes 0, 1, 2, 3 and 8.
		 */
		for (int i = 0; i < 4; i++)
			emc_write(EMC_IND_DATA__FIRST_WORD +
				  i * EMC_IND_DATA__STRIDE, 0x00000000);
		emc_write(EMC_IND_DATA__FIRST_WORD + 8 * EMC_IND_DATA__STRIDE,
			  0x00000000);
		for (int i = 4; i < 8; i++)
			emc_write(EMC_IND_DATA__FIRST_WORD +
				  i * EMC_IND_DATA__STRIDE, 0xffffffff);
		emc_write(EMC_IND_CMD, 0x00000701);
		pub_indirect_write(MC_SPECIAL_CTRL, 0x80000000);

		/* Write DB MPR required Data to Byte Lanes 0, 1, 2, 3 and 8. */
		mrs_write(dimm_db_rank, 0x7, 0x1705, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1055, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1133, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x120f, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1300, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1706, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1055, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1133, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x120f, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1300, 0, 0x10, 1);

		/*
		 * Set the Byte Lanes drive data for Per DB Addressability
		 * operation on bytes 4, 5, 6, and 7.
		 */
		for (int i = 0; i < 4; i++)
			emc_write(EMC_IND_DATA__FIRST_WORD +
				  i * EMC_IND_DATA__STRIDE, 0xffffffff);
		emc_write(EMC_IND_DATA__FIRST_WORD + 8 * EMC_IND_DATA__STRIDE,
			  0xffffffff);
		for (int i = 4; i < 8; i++)
			emc_write(EMC_IND_DATA__FIRST_WORD +
				  i * EMC_IND_DATA__STRIDE, 0x00000000);
		emc_write(EMC_IND_CMD, 0x00000701);
		pub_indirect_write(MC_SPECIAL_CTRL, 0x80000000);

		/* Write DB MPR required Data to Byte Lanes 4, 5, 6 and 7. */
		mrs_write(dimm_db_rank, 0x7, 0x1705, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1000, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x110f, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1233, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1355, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1706, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1000, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x110f, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1233, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1355, 0, 0x10, 1);

		/*
		 * Exit Per Buffer Addressability mode and return DDR PHY to
		 * operational mode.
		 */
		ddr_phy_pre_buffer_addressability_bytes_enable_all();
		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1100, 0, 0x10, 1);
		ddr_phy_pre_buffer_addressability_restore();

		/* Set the DB to MPR Override mode. */
		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1102, 0, 0x10, 1);

		/* Perform per dimm PHY standalone DQS Gate Training. */
		PUB_DTCR1_t dtcr1 = dtcr1_orig;
		dtcr1.ranken = 256 << dimm;
		pub_write(PUB_DTCR1, dtcr1.word);

		if (!ddr_phy_dqs_gate_training()) {
			MEM_ERR(1, "HIR failed for %d dimm.\n", dimm);
			rval = 0;
		}

		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1100, 0, 0x10, 1);
	}

	/* Restore DTCR0.rfshdt */
	pub_write(PUB_DTCR0, dtcr0_orig.word);

	/* Restore DTCR1.ranken */
	pub_write(PUB_DTCR1, dtcr1_orig.word);

	return rval;
}

/*
 * Initialize the VREF Training Setup for the DDR PHY BIST and the VREF Training
 * Control registers.
 */
static void lrdimm_phy_bist_and_ctrl_init(void)
{
	SET_MEM_REG_FIELD(pub, PUB_DTCR0, rfshdt, 0x0);
	SET_MEM_REG_FIELD(pub, PUB_BISTRR, bdpat, 0xe);
	SET_MEM_REG_FIELD(pub, PUB_BISTUDPR, word, 0xa5a5a5a5);
	SET_MEM_REG_FIELDS(pub, PUB_BISTAR1, reg.bainc = 0x8; reg.bmrank = 0xf);
	SET_MEM_REG_FIELDS(pub, PUB_BISTAR2, reg.bmbank = dp->ddr_max_bank;
					     reg.bmcol = dp->ddr_max_col);
	SET_MEM_REG_FIELD(pub, PUB_BISTAR4, bmrow, dp->ddr_max_row);
}

/*
 * Perform VREF Training on the DDR PHY HOST only
 * Return 1 on success or 0 if failed.
 */
static int lrdimm_ddr_phy_host_vref_training(void)
{
	uint32_t ranken_orig;

	if (!dp->vref_train_host_en)
		return 1;

	SET_MEM_REG_FIELDS(pub, PUB_DTCR1,
		ranken_orig = reg.ranken;
		reg.ranken &= 0x0101);

	SET_MEM_REG_FIELD(pub, PUB_VTCR0, dven, 0x0);
	SET_MEM_REG_FIELDS(pub, PUB_VTCR1, reg.enum0 = 1;
					   reg.tvrefio = 0x4;
					   reg.vwcr = 0xf;
					   reg.hvss = 0x0;
					   reg.hven = 0x1);
	SET_MEM_REG_FIELD(pub, PUB_BISTRR, bdxsel, 0xf);

	/* Start the PUB control operation of the DDR PHY HOST Vref training */
	if (!ddr_phy_vref_training()) {
		MEM_ERR(1, "LRDIMM DDR PHY HOST VREF Training failed!\n");
		return 0;
	}

	for (int dimm = 0; dimm < MAX_DIMM_PER_MEM_CTRL; dimm++) {
		if (dp->dimm[dimm].ranks == 0)
			continue;

		/* LRDIMM dimm index is usually referred as a (physical) rank */
		int rank = dimm;

		for (int byte = 0; byte < BYTELANE_NUM; byte++) {
			PUB_DXnGCR5_t dxngcr5 = {
				.word = pub_read(ADDR_DXn(GCR5, byte))
			};
			dp->res_dxngcr5[rank][byte] =
				GET_RANK_FIELD(dxngcr5, dxrefiselr, rank);

			PUB_DXnGCR8_t dxngcr8 = {
				.word = pub_read(ADDR_DXn(GCR8, byte))
			};
			dp->res_dxngcr8[rank][byte] =
				GET_RANK_FIELD(dxngcr8, x4dxrefiselr, rank);
		}
	}
	SET_MEM_REG_FIELD(pub, PUB_VTCR1, hven, 0x0);
	SET_MEM_REG_FIELD(pub, PUB_DTCR1, ranken, ranken_orig);

	return 1;
}

static void lrdimm_dimm_db_vref_training_internal(int dimm, int byte)
{
	uint32_t wdqd_tr_val = GET_PUB_REG_FIELD_DXn(LCDLR1, byte, wdqd);
	uint32_t x4wdqd_tr_val = GET_PUB_REG_FIELD_DXn(LCDLR1, byte, x4wdqd);
	uint32_t bl_iprd = GET_PUB_REG_FIELD_DXn(MDLR0, byte, iprd);

	uint32_t vref_eye_max = 0;
	int vref_eye_max_idx = -1;
	int dimm_db_rank = 2 * dimm;

	SET_MEM_REG_FIELD(pub, PUB_BISTRR, bdxsel, byte);

	for (int vref_index = 50; vref_index >= 0; vref_index--) {
		uint32_t vref_max_lcdl = 0;
		uint32_t vref_min_lcdl = bl_iprd + 20;
		uint32_t vref_diff_lcdl;
		uint32_t phy_bist_fail;

		/* Write vref_index to DB VREF register */
		mrs_write(dimm_db_rank, 0x7, 0x1705, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1500 + vref_index, 0, 0x10, 1);
		mem_config_ndelay(1000);

		while (!phy_bist_run(&phy_bist_fail) && !phy_bist_fail) {
			SET_PUB_REG_FIELDS_DXn(LCDLR1, byte,
				vref_max_lcdl = reg.wdqd;
				reg.wdqd++;
				reg.x4wdqd++);
			if (MAX(vref_max_lcdl + 1, vref_max_lcdl + 1 +
				(x4wdqd_tr_val - wdqd_tr_val)) >= bl_iprd + 20)
				break;
		}
		SET_PUB_REG_FIELDS_DXn(LCDLR1, byte,
			reg.wdqd = wdqd_tr_val;
			reg.x4wdqd = x4wdqd_tr_val);

		while (!phy_bist_run(&phy_bist_fail) && !phy_bist_fail) {
			SET_PUB_REG_FIELDS_DXn(LCDLR1, byte,
				vref_min_lcdl = reg.wdqd;
				reg.wdqd--;
				reg.x4wdqd--);
			if (MIN(vref_min_lcdl - 1, vref_min_lcdl - 1 +
				(x4wdqd_tr_val - wdqd_tr_val)) == 0)
				break;
		}
		SET_PUB_REG_FIELDS_DXn(LCDLR1, byte,
			reg.wdqd = wdqd_tr_val;
			reg.x4wdqd = x4wdqd_tr_val);

		vref_diff_lcdl = vref_max_lcdl - vref_min_lcdl;

		if (vref_diff_lcdl > vref_eye_max) {
			vref_eye_max = vref_diff_lcdl;
			vref_eye_max_idx = vref_index;
		} else if (vref_diff_lcdl == vref_eye_max) {
			if (ABS((FyBC(5, 5x, dimm) & 0x3f) - vref_index) <
			    ABS((FyBC(5, 5x, dimm) & 0x3f) - vref_eye_max_idx))
				vref_eye_max_idx = vref_index;
		}
	}
	dp->res_db_hdq_vref[dimm][byte] = vref_eye_max_idx;
}

static void lrdimm_dimm_db_vref_training(void)
{
	SET_MEM_REG_FIELDS(pub, PUB_BISTRR, reg.bmode = 0x1;
					    reg.bdxen = 0x1;
					    reg.bsonf = 0x1;
					    reg.bdxsel = 0x0);
	SET_MEM_REG_FIELDS(pub, PUB_BISTAR0, reg.bbank = 0x0;
					     reg.bcol = 0x0);
	SET_MEM_REG_FIELD(pub, PUB_BISTAR3, brow, 0x0);

	for (int dimm = 0; dimm < MAX_DIMM_PER_MEM_CTRL; dimm++) {
		if (dp->dimm[dimm].ranks == 0)
			continue;

		SET_MEM_REG_FIELDS(pub, PUB_BISTAR1,
			reg.brank = 8 * dimm;
			reg.bmrank = 8 * dimm);

		pub_switch_rankidr(dimm, 1, 1);

		for (int byte = 0; byte < BYTELANE_NUM; byte++) {
			lrdimm_dimm_db_vref_training_internal(dimm, byte);
		}
	}
}

static void lrdimm_dimm_db_host_vref_setup(void)
{

	for (int dimm = 0; dimm < MAX_DIMM_PER_MEM_CTRL; dimm++) {
		if (dp->dimm[dimm].ranks == 0)
			continue;

		int dimm_db_rank = 2 * dimm;

		/* Set the Initial DB HOST Side VREF Value to all DB's */
		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1500 + FyBC(5, 5x, dimm), 0,
			  0x10, 1);
		mem_config_ndelay(1000);

		/* Set the DB to Per-Buffer-Addressability mode */
		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1101, 0, 0x10, 1);
		mem_config_ndelay(1000);

		ddr_phy_pre_buffer_addressability_setup();
		ddr_phy_pre_buffer_addressability_bytes_enable_all();

		for (int byte = 0; byte < BYTELANE_NUM; byte++) {
			ddr_phy_pre_buffer_addressability_bytes_enable_single(
									byte);

			int vref_optimal = dp->res_db_hdq_vref[dimm][byte];

			/* Write the VREF to the DB HOST Side Control Reg */
			mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
			mrs_write(dimm_db_rank, 0x7, 0x1500 + vref_optimal, 0,
				  0x10, 1);
			mem_config_ndelay(1000);
		}
		ddr_phy_pre_buffer_addressability_bytes_enable_all();

		/*Set the DB to Normal mode, clear the Per-Buffer-Addr mode */
		mrs_write(dimm_db_rank, 0x7, 0x1700, 0, 0x10, 1);
		mrs_write(dimm_db_rank, 0x7, 0x1100, 0, 0x10, 1);
		mem_config_ndelay(1000);

		ddr_phy_pre_buffer_addressability_restore();
	}
}

/*
 * LRDIMM DDR PHY and DB Interface VREF Training Flow
 * Return 1 on success or 0 if failed.
 */
static int lrdimm_ddr_phy_db_interface_vref_training(void)
{
	if (dp->vref_train_bypass)
		return 1;

	lrdimm_phy_bist_and_ctrl_init();

	if (!lrdimm_ddr_phy_host_vref_training()) {
		MEM_ERR(1, "LRDIMM phy host vref training failed\n");
		return 0;
	}

	if (!dp->vref_train_mem_en)
		return 1;

	lrdimm_dimm_db_vref_training();

	lrdimm_dimm_db_host_vref_setup();

	return 1;
}

/*
 * DDR PHY data training (Write/Read leveling and per bit skew).
 * Return 1 on success or 0 if failed.
 */
int ddr_phy_data_training(void)
{
	if (dp->type == LRDIMM) {
		if (!lrdimm_db_data_training()) {
			MEM_ERR(1, "LRDIMM data buffer training failed.\n");
			return 0;
		}
		MEM_VERB(2, "Hardware DDR PHY DQS gate training finished.\n");

		if (!lrdimm_standalone_write_leveling())
			return 0;

		/*
		 * Workaround for Synopsys STAR #9001364265
		 * PIR IDONE may not be set when RFSHDT is set to zero during
		 * training.
		 */
		//mem_config_ndelay(1 * NS_PER_MS);

		if (!lrdimm_standalone_dqs_gate_leveling())
			return 0;
	} else {
		/*
		 * Start the PUB control operation of the DDR PHY write
		 * leveling.
		 */
		if (!ddr_phy_write_leveling())
			return 0;

		/*
		 * Workaround for Synopsys STAR #9001364265
		 * PIR IDONE may not be set when RFSHDT is set to zero during
		 * training.
		 */
		//mem_config_ndelay(1 * NS_PER_MS);

		/* Perform DDR PHY Standalone DQS Gate Training. */
		if (!ddr_phy_dqs_gate_training())
			return 0;
	}

	/*
	 * Perform DDR PHY data training without write leveling and DQS gate
	 * training.
	 */
	PUB_PGSR0_t pgsr0_done = {
		.wladone = 1,
		.rddone = 1,
		.wddone = 1,
		.redone = 1,
		.wedone = 1,
	};
	PUB_PGSR0_t pgsr0_err = {
		.qsgerr = 1,
		.wlaerr = 1,
		.rderr = 1,
		.wderr = 1,
		.reerr = 1,
		.weerr = 1,
	};

	if (!pir_cmd(0x0004f801, 1 * NS_PER_MS,
	    "DDR PHY data training w/o write leveling/DQS gate training",
	    pgsr0_done.word, pgsr0_err.word)) {
		ERR_SHOW_PUB_DXn(GSR2);
		ERR_SHOW_PUB_DXn(RSR2);
		ERR_SHOW_PUB_DXn(RSR3);
		if (dp->package == PACKAGE_x4)
			ERR_SHOW_PUB_DXn(GSR5);
		return 0;
	}

	return 1;
}

/*
 * Checks the VREF host training min/max values with the setup value.
 * Section 3.1 10) b) 10-20 of the document.
 */
static void vref_host_training_min_max_check(int rank, int byte)
{
	const int hvref_init_max_step_limit = 15; /* 8.8mV x 15 = 132mV */

	PUB_DXnGCR5_t dxngcr5 = { .word = pub_read(ADDR_DXn(GCR5, byte)) };
	dp->res_dxngcr5[rank][byte] = GET_RANK_FIELD(dxngcr5, dxrefiselr, rank);

	MEM_VERB(2, "res_dxngcr5[rank%d][byte%d]=%d", rank, byte,
		 dp->res_dxngcr5[rank][byte]);

	if (dp->vref_train_hres2) {
		SET_MEM_REG_FIELDS(pub, PUB_DTCR0,
			reg.dtdrs = rank;
			reg.dtdbs = byte);

		PUB_VTDR_t vtdr = { .word = pub_read(PUB_VTDR) };
		dp->res_hvmin[rank][byte] = vtdr.hvrefmn;
		dp->res_hvmax[rank][byte] = vtdr.hvrefmx;

		MEM_VERB(2, "(hvmin/max %d/%d)", vtdr.hvrefmn, vtdr.hvrefmx);

		if ((vtdr.hvrefmx == 0x3f && vtdr.hvrefmn == 0) ||
		    (vtdr.hvrefmx == 0x3f && vtdr.hvrefmx != 0 &&
		     dp->vrefdq_isel_val > dp->res_dxngcr5[rank][byte]) ||
		    (vtdr.hvrefmx != 0x3f && vtdr.hvrefmn == 0 &&
		     dp->vrefdq_isel_val < dp->res_dxngcr5[rank][byte])) {

			MEM_VERB(2, "-->%d", dp->vrefdq_isel_val);

			dp->res_dxngcr5[rank][byte] = dp->vrefdq_isel_val;
			SET_RANK_FIELD(dxngcr5, dxrefiselr, rank,
				       dp->vrefdq_isel_val);
			pub_write(ADDR_DXn(GCR5, byte), dxngcr5.word);
		}
	}
	MEM_VERB(2, "\n");

	if (dp->res_dxngcr5[rank][byte] >
			dp->vrefdq_isel_val + hvref_init_max_step_limit ||
	    dp->res_dxngcr5[rank][byte] + hvref_init_max_step_limit <
			dp->vrefdq_isel_val) {
		WARN("res_dxngcr5[rank%d][byte%d]=%d not within vrefdq_sel_val"
		     " limit of %d +/- %d\n",
		     rank, byte, dp->res_dxngcr5[rank][byte],
		     dp->vrefdq_isel_val, hvref_init_max_step_limit);
	}

	PUB_DXnGCR8_t dxngcr8 = { .word = pub_read(ADDR_DXn(GCR8, byte)) };
	dp->res_dxngcr8[rank][byte] =
		GET_RANK_FIELD(dxngcr8, x4dxrefiselr, rank);

	MEM_VERB(2, "res_dxngcr8[rank%d][byte%d]=%d", rank, byte,
		 dp->res_dxngcr8[rank][byte]);

	if (dp->vref_train_hres2) {

		SET_MEM_REG_FIELD(pub, PUB_DTCR0, dtdbs4, 1);

		PUB_VTDR_t vtdr = { .word = pub_read(PUB_VTDR) };
		dp->res_hvmin_x4[rank][byte] = vtdr.hvrefmn;
		dp->res_hvmax_x4[rank][byte] = vtdr.hvrefmx;

		MEM_VERB(2, "(hvmin/max_x4 %d/%d)", vtdr.hvrefmn, vtdr.hvrefmx);

		if ((vtdr.hvrefmx == 0x3f && vtdr.hvrefmn == 0) ||
		    (vtdr.hvrefmx == 0x3f && vtdr.hvrefmx != 0 &&
		     dp->vrefdq_isel_val > dp->res_dxngcr8[rank][byte]) ||
		    (vtdr.hvrefmx != 0x3f && vtdr.hvrefmn == 0 &&
		     dp->vrefdq_isel_val < dp->res_dxngcr8[rank][byte])) {

			MEM_VERB(2, "-->%d", dp->vrefdq_isel_val);

			dp->res_dxngcr8[rank][byte] = dp->vrefdq_isel_val;
			SET_RANK_FIELD(dxngcr8, x4dxrefiselr,
				       rank,dp->vrefdq_isel_val);
			pub_write(ADDR_DXn(GCR8, byte), dxngcr8.word);
		}

		SET_MEM_REG_FIELD(pub, PUB_DTCR0, dtdbs4, 0);
	}
	MEM_VERB(2, "\n");

	if (dp->res_dxngcr8[rank][byte] >
			dp->vrefdq_isel_val + hvref_init_max_step_limit ||
	    dp->res_dxngcr8[rank][byte] + hvref_init_max_step_limit <
			dp->vrefdq_isel_val) {
		WARN("res_dxngcr8[rank%d][byte%d]=%d not within vrefdq_sel_val"
		     " limit of %d +/- %d\n",
		     rank, byte, dp->res_dxngcr8[rank][byte],
		     dp->vrefdq_isel_val, hvref_init_max_step_limit);
	}
}

/*
 * The VREF SDRAM training steps when dqres2 is disabled.
 * Section 3.1 10) c) 5-18 of the document.
 * Return 1 if successful or 0 if failed.
 */
static int vref_sdram_training_dqres2_disabled(void)
{
	uint32_t vrefdq_tr_val;

	mr6_get_vrefdq(dp->mem_vref, &vrefdq_tr_val, NULL);
	SET_MEM_REG_FIELD(pub, PUB_VTCR0, dvinit, vrefdq_tr_val);

	/* Perform the DDR PHY SDRAM VREF Training. */
	if (!ddr_phy_vref_training()) {
		MEM_ERR(1, "DDR PHY SDRAM VREF Training failed!\n");
		return 0;
	}

	for (int rank = 0; rank < MAX_ACTIVE_RANKS; rank++) {
		if ((dp->active_ranks & (1 << rank)) == 0)
			continue;
		for (int byte = 0; byte < BYTELANE_NUM; byte++) {
			PUB_DXnGCR6_t dxngcr6 = {
				.word = pub_read(ADDR_DXn(GCR6, byte)),
			};
			dp->res_dxngcr6[rank][byte] =
				GET_RANK_FIELD(dxngcr6, dxdqvrefr, rank);
			dp->res_mr6_vref[rank][byte] =
			  (MR(6, 0) & 0x00C0) + dp->res_dxngcr6[rank][byte];

			MEM_VERB(2, "dxngcr6/mr6_vref[rank%d][byte%d]=%d/%d\n",
				rank, byte, dp->res_dxngcr6[rank][byte],
				dp->res_mr6_vref[rank][byte]);

			if (dp->package != PACKAGE_x4)
				continue;

			PUB_DXnGCR9_t dxngcr9 = {
				.word = pub_read(ADDR_DXn(GCR9, byte)),
			};
			dp->res_dxngcr9[rank][byte] =
				GET_RANK_FIELD(dxngcr9, x4dxdqvrefr, rank);
			dp->res_mr6_vref_x4[rank][byte] =
			  (MR(6, 0) & 0x00C0) + dp->res_dxngcr9[rank][byte];

			MEM_VERB(2, "dxngcr9/mr6_vref_x4[rank%d][byte%d]=%d/%d\n",
				rank, byte, dp->res_dxngcr9[rank][byte],
				dp->res_mr6_vref_x4[rank][byte]);
		}
	}

	return 1;
}

static uint32_t get_pub_vtdr_dvrefmn(void)
{
	return GET_MEM_REG_FIELD(pub, PUB_VTDR, dvrefmn);
}

static uint32_t get_pub_vtdr_dvrefmx(void)
{
	return GET_MEM_REG_FIELD(pub, PUB_VTDR, dvrefmx);
}

/*
 * Substep which initiates the hardware to do the min or max VREF training.
 * Return 1 if successful of 0 if failed.
 */
static int vref_sdram_training_dqres2_enabled_min_max(int is_min)
{
	uint32_t mr6_temp;
	uint32_t vrefdq_tr_val;
	/*
	 * Variables which are different based on if we are doing the min or the
	 * max VREF training.
	 */
	uint32_t (*mr6_get_vrefdq)(uint32_t);
	uint32_t mr6_tr_en;
	char *step;
	uint32_t (*res_dv)[BYTELANE_NUM];
	uint32_t (*res_dv_x4)[BYTELANE_NUM];
	uint32_t (*get_pub_vtdr_dvref)(void);

	if (is_min) {
		mr6_get_vrefdq = mr6_get_vrefdq_r2;
		mr6_tr_en = 0x40;
		step = "min";
		res_dv = dp->res_dvmin;
		res_dv_x4 = dp->res_dvmin_x4;
		get_pub_vtdr_dvref = get_pub_vtdr_dvrefmn;
	} else {
		mr6_get_vrefdq = mr6_get_vrefdq_r1;
		mr6_tr_en = 0x00;
		step = "max";
		res_dv = dp->res_dvmax;
		res_dv_x4 = dp->res_dvmax_x4;
		get_pub_vtdr_dvref = get_pub_vtdr_dvrefmx;
	}

	vrefdq_tr_val = mr6_get_vrefdq(dp->mem_vref);
	SET_MEM_REG_FIELD(pub, PUB_VTCR0, dvinit, vrefdq_tr_val);

	mr6_temp = (MR(6, 0) & 0xff00) | mr6_tr_en | vrefdq_tr_val;

	MEM_VERB(2, "Writing 0x%x to all MR6 PUB registers.\n", mr6_temp);
	/* Write mr6_temp to the MR6 PUB register of all populated ranks. */
	for (int rank = 0; rank < MAX_ACTIVE_RANKS; rank++) {
		if ((dp->active_ranks & (1 << rank)) == 0)
			continue;
		pub_switch_rankidr(rank, 1, 1);
		pub_write(PUB_MR6, mr6_temp);
	}

	/* Perform the DDR PHY SDRAM VREF Training. */
	if (!ddr_phy_vref_training()) {
		MEM_ERR(1, "DDR PHY SDRAM VREF Training failed (for dv%s)!\n",
			step);
		return 0;
	}

	for (int rank = 0; rank < MAX_ACTIVE_RANKS; rank++) {
		if ((dp->active_ranks & (1 << rank)) == 0)
			continue;
		for (int byte = 0; byte < BYTELANE_NUM; byte++) {
			SET_MEM_REG_FIELDS(pub, PUB_DTCR0,
				reg.dtdrs = rank;
				reg.dtdbs = byte);

			res_dv[rank][byte] = get_pub_vtdr_dvref();

			MEM_VERB(2, "res_dv%s[rank%d][byte%d]=%d\n", step, rank,
				 byte, res_dv[rank][byte]);

			if (dp->package != PACKAGE_x4)
				continue;

			SET_MEM_REG_FIELD(pub, PUB_DTCR0, dtdbs4, 1);

			res_dv_x4[rank][byte] = get_pub_vtdr_dvref();

			MEM_VERB(2, "res_dv%s_x4[rank%d][byte%d]=%d\n", step, rank,
				byte, res_dv_x4[rank][byte]);

			SET_MEM_REG_FIELD(pub, PUB_DTCR0, dtdbs4, 0);
		}
	}

	return 1;
}

/* Calculate the middle VREF index based on the given min and max index. */
static void vref_min_max_2_mean(uint32_t min_idx, uint32_t max_idx, int rank,
				int byte, uint32_t *vref_range,
				uint32_t *vref_index)
{
	uint32_t sdram_vref = (mr6_get_vrefdq_r2_inv(min_idx) +
			       mr6_get_vrefdq_r1_inv(max_idx)) / 2;

	mr6_get_vrefdq(sdram_vref, vref_index, vref_range);

	MEM_VERB(2, "(sdram_vref=%d)vref_index/range[rank%d][byte%d]=%d/%d",
		sdram_vref, rank, byte, *vref_index, *vref_range);
	if ((max_idx == 0x32 && min_idx == 0) ||
	    (max_idx == 0x32 && min_idx != 0 && dp->mem_vref > sdram_vref) ||
	    (max_idx < 0x32 && min_idx == 0 && dp->mem_vref < sdram_vref)) {
		*vref_range = dp->res_mr6_vref[rank][byte] >> 6;
		*vref_index = dp->res_mr6_vref[rank][byte] & 0x3f;
		MEM_VERB(2, "-->%d/%d", *vref_index, *vref_range);
	}
	MEM_VERB(2, "\n");
}
/*
 * The VREF SDRAM training steps when dqres2 is enabled.
 * Section 3.1 10) c) 19-79 of the document.
 * Return 1 if successful or 0 if failed.
 */
static int vref_sdram_training_dqres2_enabled(void)
{
	/* Seek out the vref sdram min values. */
	if (!vref_sdram_training_dqres2_enabled_min_max(1))
		return 0;

	/* Seek out the vref sdram max values. */
	if (!vref_sdram_training_dqres2_enabled_min_max(0))
		return 0;

	/* Calculate the actual VREF value based on min/max. */
	for (int rank = 0; rank < MAX_ACTIVE_RANKS; rank++) {
		if ((dp->active_ranks & (1 << rank)) == 0)
			continue;
		for (int byte = 0; byte < BYTELANE_NUM; byte++) {
			uint32_t vref_range, vref_index;

			vref_min_max_2_mean(dp->res_dvmin[rank][byte],
					    dp->res_dvmax[rank][byte],
					    rank, byte, &vref_range,
					    &vref_index);

			PUB_DXnGCR6_t dxngcr6 = {
				.word = pub_read(ADDR_DXn(GCR6, byte)),
			};
			SET_RANK_FIELD(dxngcr6, dxdqvrefr, rank, vref_index);
			pub_write(ADDR_DXn(GCR6, byte), dxngcr6.word);

			dp->res_mr6_vref[rank][byte] =
				(vref_range << 6) | vref_index;

			if (dp->package != PACKAGE_x4)
				continue;

			vref_min_max_2_mean(dp->res_dvmin_x4[rank][byte],
					    dp->res_dvmax_x4[rank][byte],
					    rank, byte, &vref_range,
					    &vref_index);

			PUB_DXnGCR9_t dxngcr9 = {
				.word = pub_read(ADDR_DXn(GCR9, byte)),
			};
			SET_RANK_FIELD(dxngcr9, x4dxdqvrefr, rank, vref_index);
			pub_write(ADDR_DXn(GCR9, byte), dxngcr9.word);
		}
	}

	return 1;
}

static int per_dram_addr_vref_setup_internal(int phy_rank, int byte,
					     uint32_t pda_dq,
					     uint32_t new_mr6_vref)
{
	uint32_t mr6_vref_set, mr6_vref_unset, schcr0, schcr1;

	mr6_vref_set = (MR(6, 0) & 0xff00) | 0x80 | new_mr6_vref;

	for (int i = 0; i < 2; i++) {
		schcr1 = (phy_rank << 28) | (mr6_vref_set << 8) | 0x60;
		if (dp->type == RDIMM && byte > 4)
			schcr1 |= 0x80;
		schcr0 = (pda_dq << 14) | 0x10;

		if (!ddr_phy_scheduler_single_op(schcr0, schcr1)) {
			MEM_ERR(1, "DDR PHY Scheduler Single op timed out "
				"(byte %d)w mr6_vref_set.\n", byte);
			return 0;
		}
		if (!ddr_phy_scheduler_single_nop(schcr0, schcr1)) {
			MEM_ERR(1, "DDR PHY Scheduler Single nop timed out "
				"(byte %d)w mr6_vref_set.\n", byte);
			return 0;
		}
	}

	mr6_vref_unset = (MR(6, 0) & 0xff00) | new_mr6_vref;
	schcr1 = (phy_rank << 28) | (mr6_vref_unset << 8) | 0x60;
	if (dp->type == RDIMM && byte > 4)
		schcr1 |= 0x80;
	schcr0 = (pda_dq << 14) | 0x10;

	if (!ddr_phy_scheduler_single_op(schcr0, schcr1)) {
		MEM_ERR(1, "DDR PHY Scheduler Single op timed out (byte %d)"
			"w mr6_vref_unset.\n", byte);
		return 0;
	}
	if (!ddr_phy_scheduler_single_nop(schcr0, schcr1)) {
		MEM_ERR(1, "DDR PHY Scheduler Single nop timed out (byte %d)"
			"w mr6_vref_set.\n", byte);
		return 0;
	}

	return 1;
}

/*
 * Perform Per-DRAM_Addressability VREF Setup on the SDRAM.
 * Returns 1 if successful or 0 if failed.
 */
int per_dram_addressability_vref_setup(int rank)
{
	int phy_rank = 4 * rank;
	uint32_t tmp_mr3_data = MR(3, 0) | 0x0010;
	uint32_t schcr1 = (phy_rank << 28) | (tmp_mr3_data << 8) | 0x30;
	uint32_t schcr0 = 0x10;

	vref_static_setup_rank(rank, NULL, NULL);

	if (!ddr_phy_scheduler_single_op(schcr0, schcr1)) {
		MEM_ERR(1, "DDR PHY Scheduler Single op timed out"
			" (step d)7.).\n");
		return 0;
	}

	if (!ddr_phy_scheduler_single_nop(schcr0, schcr1)) {
		MEM_ERR(1, "DDR PHY Scheduler Single nop timed out"
			" (step d)8.).\n");
		return 0;
	}

	if (dp->type == RDIMM) {
		schcr1 = (phy_rank << 28) | (tmp_mr3_data << 8) | 0xb0;
		schcr0 = 0x10;
		if (!ddr_phy_scheduler_single_op(schcr0, schcr1)) {
			MEM_ERR(1, "DDR PHY Scheduler Single op timed out"
				" (step d)9c.).\n");
			return 0;
		}
		if (!ddr_phy_scheduler_single_nop(schcr0, schcr1)) {
			MEM_ERR(1, "DDR PHY Scheduler Single nop timed out"
				" (step d)9d.).\n");
			return 0;
		}
	}

	for (int byte = 0; byte < BYTELANE_NUM; byte++) {

		uint32_t pda_dq = dp->package == PACKAGE_x4 ?
				  0x3ffff ^ (0x1 << (byte * 2)) :
				  0x3ffff ^ (0x3 << (byte * 2));
		uint32_t new_mr6_vref = dp->res_mr6_vref[rank][byte];

		if (!per_dram_addr_vref_setup_internal(phy_rank, byte,
						pda_dq, new_mr6_vref))
			return 0;

		if (dp->package != PACKAGE_x4)
			continue;

		pda_dq = 0x3ffff ^ (0x2 << (byte * 2));
		new_mr6_vref = dp->res_mr6_vref_x4[rank][byte];

		if (!per_dram_addr_vref_setup_internal(phy_rank, byte,
						pda_dq, new_mr6_vref)) {
			MEM_ERR(1, "Failed in x4 additional steps.\n");
			return 0;
		}
	}

	schcr1 = (phy_rank << 28) | (MR(3, 0) << 8) | 0x30;
	schcr0 = 0x10;

	if (!ddr_phy_scheduler_single_op(schcr0, schcr1)) {
		MEM_ERR(1, "DDR PHY Scheduler Single op timed out"
			" (step d)44.).\n");
		return 0;
	}
	if (!ddr_phy_scheduler_single_nop(schcr0, schcr1)) {
		MEM_ERR(1, "DDR PHY Scheduler Single nop timed out"
			" (step d)45.).\n");
		return 0;
	}

	if (dp->type != RDIMM)
		return 1;

	schcr1 = (phy_rank << 28) | (MR(3, 0) << 8) | 0xb0;
	schcr0 = 0x10;

	if (!ddr_phy_scheduler_single_op(schcr0, schcr1)) {
		MEM_ERR(1, "DDR PHY Scheduler Single op timed out"
			" (step d)46.c.).\n");
		return 0;
	}
	if (!ddr_phy_scheduler_single_nop(schcr0, schcr1)) {
		MEM_ERR(1, "DDR PHY Scheduler Single nop timed out"
			" (step d)46.d).\n");
		return 0;
	}

	return 1;
}

/*
 * DDR PHY and DIMM's SDRAM Device VREF Training when VREF_TRAIN_BYPASS is
 * disabled. Returns 1 on success or 0 if failed.
 */
int ddr_phy_dimm_sdram_vref_training(void)
{
	/*
	 * Initialize the VREF Training Setup for the DDR PHY BIST and the VREF
	 * Training Control registers.
	 */
	SET_MEM_REG_FIELD(pub, PUB_DTCR0, rfshdt, 0x0);
	SET_MEM_REG_FIELD(pub, PUB_BISTRR, bdpat, 0xe);
	SET_MEM_REG_FIELD(pub, PUB_BISTUDPR, word, 0xa5a5a5a5);
	SET_MEM_REG_FIELDS(pub, PUB_BISTAR1, reg.bainc = 0x8; reg.bmrank = 0xf);
	SET_MEM_REG_FIELDS(pub, PUB_BISTAR2, reg.bmbank = dp->ddr_max_bank;
					     reg.bmcol = dp->ddr_max_col);
	SET_MEM_REG_FIELD(pub, PUB_BISTAR4, bmrow, dp->ddr_max_row);
	SET_MEM_REG_FIELDS(pub, PUB_VTCR1, reg.enum0 = 1;
					   reg.tvrefio = 0x4;
					   reg.vwcr = 0xf;
					   reg.hvss = 0x0);
	SET_MEM_REG_FIELDS(pub, PUB_VTCR0, reg.dvss = 0x0;
					   reg.pdaen = dp->vref_train_pdaen);

	/* Perform VREF Training on the DDR PHY HOST only. */
	SET_MEM_REG_FIELD(pub, PUB_VTCR0, dven, 0x0);
	SET_MEM_REG_FIELD(pub, PUB_VTCR1, hven, 0x1);
	SET_MEM_REG_FIELD(pub, PUB_BISTRR, bdxsel, 0xf);

	/* Perform the DDR PHY HOST VREF Training. */
	if (dp->vref_train_host_en) {
		MEM_VERB(2, "Performing DDR PHY HOST VREF Training.\n");
		if (!ddr_phy_vref_training()) {
			MEM_ERR(1, "DDR PHY HOST VREF Training failed!\n");
			goto ddr_phy_host_vref_train_fail;
		}

		for (int rank = 0; rank < MAX_ACTIVE_RANKS; rank++) {
			if ((dp->active_ranks & (1 << rank)) == 0)
				continue;
			for (int byte = 0; byte < BYTELANE_NUM; byte++)
				vref_host_training_min_max_check(rank, byte);
		}
	}

	/* Perform VFRE Training on the SDRAM only. */
	if (!dp->vref_train_mem_en) {
		MEM_VERB(2, "Skip further VREF training (vref_train_mem_en=0).\n");
		return 1;
	}

	MEM_VERB(2, "Performing VREF Training on the SDRAM only.\n");
	SET_MEM_REG_FIELD(pub, PUB_VTCR0, dven, 0x1);
	SET_MEM_REG_FIELD(pub, PUB_VTCR1, hven, 0x0);

	if (dp->vref_train_dqres2) {
		if (!vref_sdram_training_dqres2_enabled()) {
			MEM_ERR(1, "VREF SDRAM training (dqres2=1) failed.\n");
			goto sdram_vref_train_fail;
		}
	} else {
		if (!vref_sdram_training_dqres2_disabled()) {
			MEM_ERR(1, "VREF SDRAM training (dqres2=0) failed.\n");
			goto sdram_vref_train_fail;
		}
	}

	/* c)80 Write the MR6_DATA value to MR6 of all populated ranks. */
	for (int rank = 0; rank < MAX_ACTIVE_RANKS; rank++) {

		if ((dp->active_ranks & (1 << rank)) == 0)
			continue;

		pub_switch_rankidr(rank, 1, 1);
		pub_write(PUB_MR6, MR(6, rank));
	}

	/* Check the results of the DRAM VREF Training */
	int dvref_init_max_step_limit = 20; /* 20 x 7.8mV = 156mV */

	for (int rank = 0; rank < MAX_ACTIVE_RANKS; rank++) {
		if ((dp->active_ranks & (1 << rank)) == 0)
			continue;
		for (int byte = 0; byte < BYTELANE_NUM; byte++) {
			if ((dp->res_mr6_vref[rank][byte] & (1 << 6)) !=
			     (MR(6, rank) & (1 << 6)) ||
			    (dp->res_mr6_vref[rank][byte] & 0x1f) +
			    dvref_init_max_step_limit < (MR(6, rank) & 0x1f)||
			    (dp->res_mr6_vref[rank][byte] & 0x1f) >
			     (MR(6, rank) & 0x1f) + dvref_init_max_step_limit) {
				WARN("res_mr6_vref[rank%d][byte%d] = 0x%x "
				     "not in range of MR6_DATA 0x%x +/- %d\n",
				     rank, byte, dp->res_mr6_vref[rank][byte],
				     MR(6, rank), dvref_init_max_step_limit);
			}

			if (dp->package != PACKAGE_x4)
				continue;

			if ((dp->res_mr6_vref_x4[rank][byte] & (1 << 6)) !=
			     (MR(6, rank) & (1 << 6)) ||
			    (dp->res_mr6_vref_x4[rank][byte] & 0x1f) +
			     dvref_init_max_step_limit < (MR(6, rank) & 0x1f) ||
			    (dp->res_mr6_vref_x4[rank][byte] & 0x1f) >
			     (MR(6, rank) & 0x1f) + dvref_init_max_step_limit) {
				WARN("res_mr6_vref_x4[rank%d][byte%d] = 0x%x "
				     "not in range of MR6_DATA 0x%x +/- %d\n",
				    rank, byte, dp->res_mr6_vref_x4[rank][byte],
				    MR(6, rank), dvref_init_max_step_limit);
			}
		}
	}


	/* Skip the follow steps if both pdaen and dqres2 are disabled. */
	if (!dp->vref_train_pdaen && !dp->vref_train_dqres2)
		return 1;

	for (int rank = 0; rank < MAX_ACTIVE_RANKS; rank++) {
		if ((dp->active_ranks & (1 << rank)) == 0)
			continue;
		if (!per_dram_addressability_vref_setup(rank)) {
			MEM_ERR(1, "Per DRAM Addressability VREF setup failed.\n");
			return 0;
		}
	}
	pub_write(PUB_SCHCR0, 0);
	pub_write(PUB_SCHCR1, 0);
	return 1;

ddr_phy_host_vref_train_fail:
	ERR_SHOW_PUB_DXn(GSR3);
	ERR_SHOW_PUB_DXn(GCR5);
	if (dp->package == PACKAGE_x4) {
		ERR_SHOW_PUB_DXn(GSR6);
		ERR_SHOW_PUB_DXn(GCR8);
	}
	return 0;

sdram_vref_train_fail:
	ERR_SHOW_PUB_DXn(GSR3);
	ERR_SHOW_PUB_DXn(GCR6);
	if (dp->package == PACKAGE_x4) {
		ERR_SHOW_PUB_DXn(GSR6);
		ERR_SHOW_PUB_DXn(GCR9);
	}
	return 0;
}

/*
 * The internal per rank per byte steps of the Post DDR PHY Data
 * Training required Configurations changes.
 * returns 1 if there was an error or 0 if successful.
 */
int pdpdtc_internal_loop(int rank, int byte)
{
	int byte_nom_err = 0;
	uint32_t b_iprd, b_wlsl, b_wld, b_dgsl, b_dqsgd;
	PUB_DXnMDLR0_t dxnmdlr0;
	PUB_DXnGTR0_t dxngtr0;
	PUB_DXnLCDLR0_t dxnlcdlr0;
	PUB_DXnLCDLR2_t dxnlcdlr2;

	dxnmdlr0.word = pub_read(ADDR_DXn(MDLR0, byte));
	dxngtr0.word = pub_read(ADDR_DXn(GTR0, byte));
	dxnlcdlr0.word = pub_read(ADDR_DXn(LCDLR0, byte));
	dxnlcdlr2.word = pub_read(ADDR_DXn(LCDLR2, byte));

	b_iprd = dxnmdlr0.iprd;

	/* Steps for write leveling normalization. */
	for (int x4 = 0; x4 < (1 + (dp->package == PACKAGE_x4)); x4++) {

		if (x4) {
			b_wlsl = dxngtr0.x4wlsl;
			b_wld = dxnlcdlr0.x4wld;
		} else {
			b_wlsl = dxngtr0.wlsl;
			b_wld = dxnlcdlr0.wld;
		}

		if (b_wlsl == 0 && b_wld <= b_iprd) {
			byte_nom_err = 1;
			MEM_ERR(1, "rank%d byte%d%s, "
				"byte_wlsl=0 and byte_wld(%d)<=byte_iprd(%d)\n",
				rank, byte, x4 ? " x4" : "", b_wld, b_iprd);
		} else if (b_wld > b_iprd) {
			MEM_VERB(2, "rank%d byte%d%s, byte_wlsl(%d) "
				"byte_wld(%d) > byte_iprd(%d)\n",
				rank, byte, x4 ? " x4" : "",
				b_wlsl, b_wld, b_iprd);
		} else {
			b_wlsl--;
			b_wld += b_iprd;

			if (x4) {
				dxngtr0.x4wlsl = b_wlsl;
				dxnlcdlr0.x4wld = b_wld;
			} else {
				dxngtr0.wlsl = b_wlsl;
				dxnlcdlr0.wld = b_wld;
			}
			MEM_VERB(2, "(Norm)  rank%d byte%d %s, "
				"byte_wlsl(%d) byte_wld(%d) byte_iprd(%d)\n",
				rank, byte, x4 ? " x4" : "",
				b_wlsl, b_wld, b_iprd);

			pub_write(ADDR_DXn(GTR0, byte), dxngtr0.word);
			pub_write(ADDR_DXn(LCDLR0, byte), dxnlcdlr0.word);
		}
	}

	/* Steps for Read DQS Gate Normalization */
	for (int x4 = 0; x4 < (1 + (dp->package == PACKAGE_x4)); x4++) {

		if (x4) {
			b_dgsl = dxngtr0.x4dgsl;
			b_dqsgd = dxnlcdlr2.x4dqsgd;
		} else {
			b_dgsl = dxngtr0.dgsl;
			b_dqsgd = dxnlcdlr2.dqsgd;
		}

		if (b_dgsl == 0 && b_dqsgd <= b_iprd) {
			byte_nom_err = 1;
			MEM_ERR(1, "rank%d byte%d%s, byte_dqsl=0 "
				"and byte_dqsgd(%d) <= byte_iprd(%d)\n",
				rank, byte, x4 ? " x4" : "",
				b_dqsgd, b_iprd);
		} else if (b_dqsgd > b_iprd) {
			MEM_VERB(2, "rank%d byte%d%s, byte_dqsl(%d) "
				"byte_dqsgd(%d) > byte_iprd(%d)\n",
				rank, byte, x4 ? " x4" : "",
				b_dgsl, b_dqsgd, b_iprd);
		} else {
			b_dgsl--;
			b_dqsgd += b_iprd;

			if (x4) {
				dxngtr0.x4dgsl = b_dgsl;
				dxnlcdlr2.x4dqsgd = b_dqsgd;
			} else {
				dxngtr0.dgsl = b_dgsl;
				dxnlcdlr2.dqsgd = b_dqsgd;
			}
			MEM_VERB(2, "(Norm)  rank%d byte%d %s, "
				"byte_dqsl(%d) byte_dqsgd(%d) byte_iprd(%d)\n",
				rank, byte, x4 ? " x4" : "",
				b_dgsl, b_dqsgd, b_iprd);

			pub_write(ADDR_DXn(GTR0, byte), dxngtr0.word);
			pub_write(ADDR_DXn(LCDLR2, byte), dxnlcdlr2.word);
		}
	}

	return byte_nom_err;
}

/*
 * Do the Post DDR PHY Data Training required Configurations changes
 * Returns 1 if successful or 0 if failed.
 */
int post_ddr_phy_data_training_configs(void)
{
	int byte_nom_err = 0;

	if (dp->wlrdqsg_lcdl_norm == 0) {
		MEM_VERB(2, "Operatin skipped (wlrdqsg_lcdl_norm = 0).\n");
		return 1;
	}

	SET_MEM_REG_FIELD(pub, PUB_PGCR1, pubmode, 0x1);
	//SET_MEM_REG_FIELD(pub, PUB_PGCR6, inhvt, 0x1);

	for (int rank = 0; rank < MAX_ACTIVE_RANKS; rank++) {
		if ((dp->active_ranks & (1 << rank)) == 0)
			continue;

		pub_switch_rankidr(rank, 1, 1);
		mem_config_ndelay(10 * NS_PER_US);

		for (int byte = 0; byte < BYTELANE_NUM; byte++) {
			int err = pdpdtc_internal_loop(rank, byte);

			byte_nom_err |= err;
			if (err) {
				MEM_ERR(2, "On rank %d byte %d", rank, byte);
				ERR_SHOW_PUB_DX(byte, MDLR0);
				ERR_SHOW_PUB_DX(byte, GTR0);
				ERR_SHOW_PUB_DX(byte, LCDLR0);
				ERR_SHOW_PUB_DX(byte, LCDLR2);
			}
		}
	}
	pub_switch_rankidr(0, 1, 1);
	//SET_MEM_REG_FIELD(pub, PUB_PGCR6, inhvt, 0);
	SET_MEM_REG_FIELD(pub, PUB_PGCR1, pubmode, 0);
	mem_config_ndelay(10 * NS_PER_US);

	return byte_nom_err == 0;
}

/*
 * Do a write and readback using the direct address host interface.
 * Returns 1 if the data read is the same as the data written, else return 0.
 */
int test_write_readback(uint64_t *rand_state, uint32_t rank)
{
	uint32_t read_data[EMI_IND_DATA__LENGTH];
	uint32_t write_data[EMI_IND_DATA__LENGTH];

	uint32_t read_raw_ecc_data[EMI_ADD_IND_DATA__LENGTH];
	uint32_t write_raw_ecc_data[EMI_ADD_IND_DATA__LENGTH];

	uint32_t bank = rand_gen(rand_state) % dp->ddr_max_bank;
	uint32_t col = rand_gen(rand_state) % (dp->ddr_max_col >> 3);
	uint32_t row = rand_gen(rand_state) % dp->ddr_max_row;

	/* Generate the random data to write. */
	for (int i = 0; i < EMI_IND_DATA__LENGTH; i++)
		write_data[i] = (uint32_t)(rand_gen(rand_state) & 0xffffffff);

	/*
	 * We are just testing if the readback is working thus no need to
	 * calculate the correct ecc data.
	 */
	for (int i = 0; i < EMI_ADD_IND_DATA__LENGTH; i++)
		write_raw_ecc_data[i] = (uint32_t)(rand_gen(rand_state) &
						   0xffffffff);

	/* Do write operation to write the generated data. */
	if (!direct_addr_host_op(DIRECT_ADDR_HOST_OP__WRITE, 1, write_data,
		write_raw_ecc_data, rank, 0, bank >> 2, bank & 3, col, row)) {
		MEM_ERR(1, "Direct host write operation failed.\n");
		return 0;
	}

	/* Readback the written data. */
	if (!direct_addr_host_op(DIRECT_ADDR_HOST_OP__READ, 1, read_data,
		read_raw_ecc_data, rank, 0, bank >> 2, bank & 3, col, row)) {
		MEM_ERR(1, "Direct host read operation failed.\n");
		return 0;
	}
	/* Validate that the readback data is same as the written data. */
	if (memcmp(read_data, write_data,
		   EMI_IND_DATA__LENGTH * sizeof(read_data[0])))
		return 0;

	if (memcmp(read_raw_ecc_data, write_raw_ecc_data,
		   EMI_ADD_IND_DATA__LENGTH * sizeof(read_raw_ecc_data[0])))
		return 0;

	return 1;
}

/*
 * Memory Controller Latency Post DDR PHY Data Training.
 * Return 1 on success or 0 if failed.
 */
int mc_latency_post_ddr_phy_data_training(void)
{
	uint64_t rand_state = 3;
	uint32_t latency = 10;
	EMC_EXT_MC_LATENCY_t eeml = {
		.ext_mc_rl = latency,
	};
	emc_write(EMC_EXT_MC_LATENCY, eeml.word);

	EMC_MC_DDR_IF_t emdi = { .word = emc_read(EMC_MC_DDR_IF) };
	emdi.mc_en = 1;
	emc_write(EMC_MC_DDR_IF, emdi.word);

	MEM_VERB(2, "Do MC latency post DDR PHY data training.\n");

	for (int rank = 0; rank < MAX_ACTIVE_RANKS; rank++) {
		if ((dp->active_ranks & (1 << rank)) == 0)
			continue;
		/* Clear the Memory Controller Read Data FIFO path. */
		MC_PUP_CTRL_t mpc = { .mc_sw_rd_rst = 1 };
		pub_indirect_write(MC_PUP_CTRL, mpc.word);
		mem_config_ndelay(1000);
		mpc.mc_sw_rd_rst = 0;
		pub_indirect_write(MC_PUP_CTRL, mpc.word);
		mem_config_ndelay(1000);

		int success = 0;

		while (success < 10 && latency < 0x3f) {
			MEM_VERB(2, "Testing with ext_mc_rl=%d on rank %d...",
				 latency, rank);
			if (test_write_readback(&rand_state, rank)) {
				success++;
				MEM_VERB(2, "success!\n");
			} else {
				success = 0;
				latency++;
				eeml.ext_mc_rl = latency;
				emc_write(EMC_EXT_MC_LATENCY, eeml.word);
				MEM_VERB(2, "failed!\n");
			}
		}
	}

	if (latency < 0x3f) {
		eeml.ext_mc_rl = latency + 1;
		emc_write(EMC_EXT_MC_LATENCY, eeml.word);
		MEM_VERB(2, "Using latency value of %d\n", latency + 1);
	}
	emdi.mc_en = 0;
	emc_write(EMC_MC_DDR_IF, emdi.word);

	return latency < 0x3f;
}

/* This step should be performed only when CRC_EN option is enabled. */
static void enable_crc(void)
{
	EMC_DDR_DEBUG_t edd = { .word = emc_read(EMC_DDR_DEBUG) };
	edd.crc_en = dp->crc_en;
	emc_write(EMC_DDR_DEBUG, edd.word);

	for (int i = 0; i < MAX_ACTIVE_RANKS; i++) {
		if ((dp->active_ranks & (1 << i)) == 0)
			continue;
		uint32_t  mr = MR_RANK(i);
		int dimm_idx = dp->dimm[1].ranks > 0 && i > 1;

		MR(2, i) |= (dp->crc_en << 12); /* Set the WR_CRC bit. */

		MEM_VERB(2, "Enabling CRC for rank%d (MR2=0x%x).\n", i, MR(2, i));

		mrs_write(mr, 2, MR(2, i), 0, 0x4, 1);
		mem_config_ndelay(500);

		if (dp->type != LRDIMM && dp->type != RDIMM)
			continue;

		/* For RDIMM or LRDIMM, repeat for side B access. */
		mrs_write_inv(mr, 2, MR(2, i), 0, 0x4, 1,
			      RC(00, dimm_idx));
		mem_config_ndelay(500);
	}
}

/* Enable DDR Memory Controller. */
static void ddr_mc_enable(void)
{
	EMC_MC_DDR_IF_t emdi = { .word = emc_read(EMC_MC_DDR_IF) };
	emdi.mc_en = 1;
	emdi.calib_en = 1;
	emdi.ref_en = 1;
	emc_write(EMC_MC_DDR_IF, emdi.word);

	MEM_VERB(2, "Enabled DDR memory controller\n");
}

/*
 * The actual steps for setup.
 * Return 1 on success or 0 if failed.
 */
int ddr_do_actual_setup(void)
{
	if (dp->dimm_num == 0)
		return 0;

	/* Also initialize some global variables here. */
	dp->active_ranks = ((1 << dp->dimm[0].ranks) - 1) |
			   (((1 << dp->dimm[1].ranks) - 1) << 2);
	rcd_reg_val_setup();
	if (dp->type == LRDIMM) {
		db_reg_val_setup();
	}
	if (!mr_reg_val_setup()) {
		MEM_ERR(1, "MR compute fail, abort memory controller %d setup\n",
			dp->mss_index);
		return 0;
	}

	MEM_LOG("# Step 1: Set DDR Interface Frequency.\n");
	ddr_interface_freq_setup();

	MEM_LOG("# Step 2: Write Memory Controller Configuration registers.\n");
	mem_ctrl_config();

	MEM_LOG("# Step 3: APB Interface setup and clock & reset release.\n");
	if (!apb_setup()) {
		MEM_ERR(1, "EMC APB interface not setup correctly!\n");
		return 0;
	}
	MEM_LOG("# Step 4: DDR PHY SDRAM System Specific registers config.\n");
	ddr_phy_config();

	MEM_LOG("# Step 5: DDR PHY Initialization.\n");
	if (!ddr_phy_init()) {
		MEM_ERR(1, "DDR PHY initialization failed!\n");
		if (ddr_error_level >= 3)
			pub_dump();
		return 0;
	}
	MEM_LOG("# Step 6: DDR PHY CK deferential pair signals delay setup.\n");
	ddr_phy_ck_setup();

	MEM_LOG("# Step 7: DIMM's or SDRAM System Reset Release.\n");
	dimm_system_release();

	MEM_LOG("# Step 8: DIMM's or SDRAM Initialization.\n");
	dimm_init();

	MEM_LOG("# Step 9: DDR PHY Data Training.\n");
	if (!ddr_phy_data_training()) {
		MEM_ERR(1, "DDR PHY data training failed!\n");
		if (ddr_error_level >= 3)
			pub_dump();
		return 0;
	}

	if (!dp->vref_train_bypass && dp->type != LRDIMM) {
		MEM_LOG("# Step 10: DDR PHY and DIMM's SDRAM VREF training.\n");
		if (!ddr_phy_dimm_sdram_vref_training()) {
			MEM_ERR(1, "DDR PHY & DIMM's SDRAM VREF training fail\n");
			if (ddr_error_level >= 3)
				pub_dump();
			return 0;
		}
	} else if (dp->type == LRDIMM) {
		MEM_LOG("# Step 10: LRDIMM DDR PHY & DB Int VREF Training.\n");
		if (!lrdimm_ddr_phy_db_interface_vref_training()) {
			MEM_ERR(1, "LRDIMM VREF training fail\n");
			if (ddr_error_level >= 3)
				pub_dump();
			return 0;
		}
	}

	MEM_LOG("# Step 11: Post DDR PHY Data Training required configs.\n");
	if (!post_ddr_phy_data_training_configs()) {
		MEM_ERR(1, "Post DDR PHY Data Training required config fail!\n");
		if (ddr_error_level >= 3)
			pub_dump();
		return 0;
	}

	MEM_LOG("# Step 12: Memory Controller Post DDR PHY Data Training.\n");
	if (!mc_latency_post_ddr_phy_data_training()) {
		MEM_ERR(1, "Memory controller post DDR PHY data training fail!\n");
		return 0;
	}
	if (dp->crc_en) {
		MEM_LOG("# Step 13: Enable CRC operation.\n");
		enable_crc();
	}

	MEM_LOG("# Step 14: Enable DDR Memory Controller operation.\n");
	ddr_mc_enable();

	dp->trained = 1;
	dp->zeroed = 0;

	return 1;
}

static void ddr_idle_interface_freq_setup(void)
{
	uint32_t tyu_mss_rst;
	uint32_t tyu_mss_rst_orig;
	uint32_t rst_pin_umx;
	uint32_t rst_pin_s;
	uint32_t rst_pin_g;

	if (dp->mss_index == 0) {
		rst_pin_umx = MSS0_UMX_MC_DDR_RESET_EN;
		rst_pin_s = MSS0_DDR_MC_SRST_EN;
		rst_pin_g = MSS0_DDR_MC_GRST_EN;
	} else if (dp->mss_index == 1) {
		rst_pin_umx = MSS1_UMX_MC_DDR_RESET_EN;
		rst_pin_s = MSS1_DDR_MC_SRST_EN;
		rst_pin_g = MSS1_DDR_MC_GRST_EN;
	} else {
		MEM_ERR(1, "Unsupported MSS index %d.\n", dp->mss_index);
		return;
	}
	/* a) Set the MSS EMI and DDR PHY Reset. */
	tyu_mss_rst = mmio_read_32(TYU_BASE_ADDRESS + TYU_MSS_RESET);
	tyu_mss_rst_orig = tyu_mss_rst;
	tyu_mss_rst |= (rst_pin_s | rst_pin_umx);
	mmio_write_32(TYU_BASE_ADDRESS + TYU_MSS_RESET, tyu_mss_rst);
	tyu_mss_rst |= rst_pin_g;
	mmio_write_32(TYU_BASE_ADDRESS + TYU_MSS_RESET, tyu_mss_rst);

	MEM_VERB(2, "Setting the MSS EMI and DDR PHY Reset.\n");

	mem_config_ndelay(500);

	/* b) Set the PLL frequency parameters. */
	ddr_set_dll_frequency();

	/* c) Release MSS EMI and DDR PHY Reset. */
	tyu_mss_rst = (tyu_mss_rst & ~rst_pin_umx) |
			(tyu_mss_rst_orig & rst_pin_umx);
	mmio_write_32(TYU_BASE_ADDRESS + TYU_MSS_RESET, tyu_mss_rst);
	tyu_mss_rst &= ~(rst_pin_g | rst_pin_umx);
	mmio_write_32(TYU_BASE_ADDRESS + TYU_MSS_RESET, tyu_mss_rst);
	MEM_VERB(2, "Release the MSS EMI and DDR PHY Reset.\n");
	mem_config_ndelay(500);
}

/*
 * The actual steps for setup.
 * Return 1 on success or 0 if failed.
 */
int ddr_do_idle_setup(void)
{
	NOTICE("Doing MSS idle operations on MSS %d\n", dp->mss_index);

	dp->tck = 1250000; /* Setup the PLL using lowest support frequency. */

	/* Step 1, Set DDR Interface Frequency. */
	ddr_idle_interface_freq_setup();

	/* Step 2, Release APB Interface clock and Reset. */
	apb_setup();

	/* Step 3, DDR PHY Initialization (Initialze DDR PHY PLL/DLL/ZCAL) */
	ddr_phy_init();

	/*
	 * Step 4, Set Address/Control I/O's to OE Disable mode, and keep the
	 * I/O's Receive at Power Down mode.
	 */
	SET_MEM_REG_FIELD(pub, PUB_RIOCR2, coemode, 0xA);
	SET_MEM_REG_FIELD(pub, PUB_RIOCR2, csoemode, 0xAA);
	SET_MEM_REG_FIELD(pub, PUB_RIOCR4, ckeoemode, 0xAA);
	SET_MEM_REG_FIELD(pub, PUB_RIOCR5, odtoemode, 0xAA);
	SET_MEM_REG_FIELD(pub, PUB_ACIOCR1, aoemode, 0xAAAAAAAA);
	SET_MEM_REG_FIELD(pub, PUB_ACIOCR3, ckoemode, 0xAA);
	SET_MEM_REG_FIELDS(pub, PUB_ACIOCR3,
		reg.actoemode = 0x2;
		reg.a16oemode = 0x2;
		reg.a17oemode = 0x2;
		reg.paroemode = 0x2;
	);
	SET_MEM_REG_FIELDS(pub, PUB_ACIOCR3,
		reg.baoemode = 0xA;
		reg.bgoemode = 0xA;
	);

	/*
	 * Step 5, Set All Data Bytes I/O's to OE Disable, Termination Disable
	 * and Receive Power Down modes.
	 */
	for (int byte = 0; byte < BYTELANE_NUM; byte++) {
		SET_PUB_REG_FIELD_DXn(GCR1, byte, dxpdrmode, 0xAAAA);
		SET_PUB_REG_FIELDS_DXn(GCR2, byte,
			reg.dxtemode = 0xAAAA;
			reg.dxoemode = 0xAAAA;
		);
		SET_PUB_REG_FIELDS_DXn(GCR3, byte,
			reg.dspdrmode = 0x2;
			reg.dstemode = 0x2;
			reg.dsoemode = 0x2;
			reg.dmpdrmode = 0x2;
			reg.dmtemode = 0x2;
			reg.dmoemode = 0x2;
		);
		SET_PUB_REG_FIELDS_DXn(GCR7, byte,
			reg.x4dspdrmode = 0x2;
			reg.x4dstemode = 0x2;
			reg.x4dsoemode = 0x2;
			reg.x4dspdrmode = 0x2;
			reg.x4dstemode = 0x2;
			reg.x4dsoemode = 0x2;
		);
	}

	/* Step 6, Set the DDR PHY to DLL Test mode Setup. */
	SET_MEM_REG_FIELD(pub, PUB_PGCR6, inhvt, 0x1);
	SET_MEM_REG_FIELD(pub, PUB_PGCR6, dldlmt, 0x00);
	SET_MEM_REG_FIELD(pub, PUB_PGCR1, mdlen, 0x0);
	SET_MEM_REG_FIELD(pub, PUB_DXCCR, mdlen, 0x0);
	SET_MEM_REG_FIELD(pub, PUB_PGCR4, acdccbyp, 0x1);
	SET_MEM_REG_FIELD(pub, PUB_DXCCR, dxdccbyp, 0x1);
	SET_MEM_REG_FIELD(pub, PUB_PGCR0, dltmode, 0x1);

	/* Step 7, Set Address/Control LCDL's and BDL's to full delay setup. */
	pub_write(PUB_ACBDLR0, 0x3F3F3F3F);
	pub_write(PUB_ACBDLR1, 0x3F3F3F3F);
	pub_write(PUB_ACBDLR2, 0x3F3F3F3F);
	pub_write(PUB_ACBDLR3, 0x3F3F3F3F);
	pub_write(PUB_ACBDLR4, 0x3F3F3F3F);
	pub_write(PUB_ACBDLR5, 0x3F3F3F3F);
	pub_write(PUB_ACBDLR6, 0x3F3F3F3F);
	pub_write(PUB_ACBDLR7, 0x3F3F3F3F);
	pub_write(PUB_ACBDLR8, 0x3F3F3F3F);
	pub_write(PUB_ACBDLR9, 0x3F3F3F3F);
	pub_write(PUB_ACBDLR10, 0x3F3F3F3F);
	SET_MEM_REG_FIELD(pub, PUB_ACLCDLR, acd, 0x1FF);
	SET_MEM_REG_FIELD(pub, PUB_ACMDLR1, mdld, 0x1FF);

	/* Step 8, Set All Data Bytes LCDL's and BDL's to full delay setup. */
	for (int byte = 0; byte < BYTELANE_NUM; byte++) {
		pub_write(ADDR_DXn(BDLR0, byte), 0x3F3F3F3F);
		pub_write(ADDR_DXn(BDLR1, byte), 0x3F3F3F3F);
		pub_write(ADDR_DXn(BDLR2, byte), 0x003F3F3F);
		pub_write(ADDR_DXn(BDLR3, byte), 0x3F3F3F3F);
		pub_write(ADDR_DXn(BDLR4, byte), 0x3F3F3F3F);
		pub_write(ADDR_DXn(BDLR5, byte), 0x003F3F3F);
		pub_write(ADDR_DXn(BDLR7, byte), 0x003F3F3F);
		pub_write(ADDR_DXn(BDLR8, byte), 0x003F3F3F);
		pub_write(ADDR_DXn(LCDLR0, byte), 0x01FF01FF);
		pub_write(ADDR_DXn(LCDLR1, byte), 0x01FF01FF);
		pub_write(ADDR_DXn(LCDLR2, byte), 0x01FF01FF);
		pub_write(ADDR_DXn(LCDLR3, byte), 0x01FF01FF);
		pub_write(ADDR_DXn(LCDLR4, byte), 0x01FF01FF);
		pub_write(ADDR_DXn(LCDLR5, byte), 0x01FF01FF);
		SET_PUB_REG_FIELD_DXn(MDLR1, byte, mdld, 0x1FF);
	}

	/* Step 9, Set a value of 0x1 to the OSCEN field of the PGCR0 reg. */
	SET_MEM_REG_FIELD(pub, PUB_PGCR0, oscen, 1);

	return 1;
}

struct ddr_params *bluefield_setup_mss(uintptr_t mss_addr, int mem_ctrl_num)
{
	NOTICE("Initializing DDR at mss[%d]=0x%lx\n", mem_ctrl_num, mss_addr);
	dp = &(dps[mem_ctrl_num]);
	if (!ddr_get_info(dp, mss_addr, mem_ctrl_num)) {
		MEM_ERR(1, "DDR Values not valid!\n");
		return NULL;
	}

	if (dp->dimm_num == 0) {
		NOTICE("No memory present on MSS %d\n", mem_ctrl_num);
		ddr_do_idle_setup();
		return NULL;
	}

	if (!ddr_do_actual_setup()) {
		MEM_ERR(1, "Initializing DDR on MSS %d failed!\n", mem_ctrl_num);
		return NULL;
	}

	NOTICE("Finished initializing DDR on MSS %d!\n", mem_ctrl_num);

	return dp;
}
