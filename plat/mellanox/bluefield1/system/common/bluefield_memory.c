/*
 * Copyright (c) 2018, Mellanox Technologies and Contributors.
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

#include <debug.h>
#include <mmio.h>
#include "bluefield_ddr.h"
#include "bluefield_ddr_bist.h"
#include "bluefield_private.h"
#include "i2c_smbus.h"
#include "tyu_def.h"
#include "rsh.h"

#pragma weak ddr_fixed_configs
const struct ddr_config *const ddr_fixed_configs[] = {
	NULL,
};

/*
 * Normally we panic when BIST fails, but when we have the ATF_CONSOLE, we
 * return so that we can enter the console.
 */
#ifdef ATF_CONSOLE
# define BIST_FAIL_ACTION()	return
#else
# define BIST_FAIL_ACTION()	panic()
#endif

#pragma weak bf_sys_mem_config
/*
 * Configure particular memory controller indicated by given base address and
 * using real setup code. Also fill out the struct describing the DIMMs
 * corresponding to the controller. Returns 1 if there is memory on this
 * memory controller or 0 if the memory controller has no memory or is
 * disabled.
 */
int bf_sys_mem_config(uintptr_t mem_base,
		      struct bf_mem_ctrl_info *mem_ctrl_info,
		      int mem_ctrl_num)
{
	struct ddr_params *dp = bluefield_setup_mss(mem_base, mem_ctrl_num);

	if (dp == NULL) {
		INFO("No memory on MSS %d\n", mem_ctrl_num);
		return 0;
	}

	bluefield_fill_mem_ctrl_info(dp, mem_ctrl_info);

	return dp->dimm_num > 0;
}

/*
 * Only the "generic" system uses this to try and retrain the memory
 * configuration.
 */
#pragma weak bf_sys_mem_try_cfg
int bf_sys_mem_try_cfg(uintptr_t mem_base,
		       struct bf_mem_ctrl_info *mem_ctrl_info,
		       int mem_ctrl_num)
{
	return 0;
}

#pragma weak bf_sys_post_test
void bf_sys_post_test(struct bf_mem_info *bmi)
{
	int skip_default_bist;
	uint32_t mc_mask = 0;
	uint32_t mc_pr_mask[MAX_MEM_CTRL];
	uint64_t hrst_tcks, srst_tcks;

	RSH_BOOT_CONTROL_t boot_ctrl;

	/*
	 * The hard reset tck values starts ticking prior to the soft one.
	 * The amount of tck the hard one have before a soft one is roughly
	 * around 0x8000000. So if the difference is more than 0x10000000
	 * we know it's a soft reset.
	 */
	srst_tcks = mmio_read_64(RSHIM_BASE + RSH_UPTIME);
	hrst_tcks = mmio_read_64(RSHIM_BASE + RSH_UPTIME_POR);
	boot_ctrl.word = mmio_read_64(RSHIM_BASE + RSH_BOOT_CONTROL);

	/*
	 * Skip the default BIST if we are just doing a soft reset and booting
	 * from the eMMC. As the memory configuration shouldn't be changed
	 * between soft resets, and also when loading the ATF image from the
	 * eMMC, it should have already run once when pushing the install.bfb
	 * to update the ATF image. It should be safe to assume that BIST won't
	 * fail when doing a soft reset and booting off the eMMC. So disable it
	 * to save warm reboot time.
	 */
	skip_default_bist = (hrst_tcks - srst_tcks > 0x10000000) &&
		(boot_ctrl.boot_mode != RSH_BOOT_CONTROL__BOOT_MODE_VAL_NONE);

	/* Calculate all present ranks (and corresponding MSS). */
	for (int i = 0; i < MAX_MEM_CTRL; i++) {
		struct bf_mem_dimm_info *di =
			bmi->mem_ctrl_info[i].dimm_info;

		if (di[0].size_in_gb == 0 && di[1].size_in_gb == 0) {
			mc_pr_mask[i] = 0;
			continue;
		}

		mc_mask |= 1 << i;
		mc_pr_mask[i] = (((1 << di[1].ranks_num) - 1) << 2) |
				 ((1 << di[0].ranks_num) - 1);
	}

	if (mc_mask == 0) {
		ERROR("No working memory brought up.\n");
		BIST_FAIL_ACTION();
	}

	if (!skip_default_bist) {
		if (run_default_bist(mc_mask, mc_pr_mask)) {
			ERROR("DDR BIST POST failed!\n");
			BIST_FAIL_ACTION();
		}
	}

	/* Memory stil need to be zeroed everytime to not trigger ECC errors. */
	if (init_ddr_by_bist(mc_mask, mc_pr_mask)) {
		ERROR("Zeroing DDR memory failed!\n");
		BIST_FAIL_ACTION();
	}
	NOTICE("DDR POST passed.\n");
}

#pragma weak bf_sys_get_spd
/*
 * Read the SPD of the specified DIMM on the specified MSS.
 * Return the size used for the SPD, if there was no SPD there, return 0.
 */
int bf_sys_get_spd(uint8_t *spd, int offset, int len, int mss, int dimm)
{
	return bf_sys_get_spd_default(spd, offset, len, mss, dimm);
}

/* By default we don't overwrite any parameters and rely on the SPD. */
#pragma weak bf_sys_ddr_get_info_user
int bf_sys_ddr_get_info_user(struct ddr_params *dp)
{
	return 1;
}

/*
 * This function should be populated for every system which does actual memory
 * training.
 */
#pragma weak bf_sys_ddr_get_info_board
int bf_sys_ddr_get_info_board(struct ddr_params *dp)
{
	return bf_sys_ddr_get_info_board_default(dp);
}
