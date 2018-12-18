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
#include "rsh.h"
#include "i2c_smbus.h"

/*
 * In the simulator we are configuring memory on all MSS present to verify that
 * the code works. But we only return as if the first memory controller is
 * populated with memory.
 */
int bf_sys_mem_config(uintptr_t mem_base,
		      struct bf_mem_ctrl_info *mem_ctrl_info,
		      int mem_ctrl_num)
{
	bluefield_setup_mss(mem_base, mem_ctrl_num);

	if (mem_ctrl_num != 0)
		return 0;
	/*
	 * In the simulator, we overload the reserved fields in the rshim
	 * DEV_INFO register to convey how much memory we have.
	 */
	RSH_DEV_INFO_t rdi = {
		.word = mmio_read_64(RSHIM_BASE + RSH_DEV_INFO),
	};
	/* We are storing in gigabits, so we need to multiply by 8 here. */
	mem_ctrl_info->dimm_info[0].size_in_gb = rdi.__reserved_2 * 8;
	mem_ctrl_info->dimm_info[0].is_nvdimm = 0;
	mem_ctrl_info->dimm_info[0].is_single_DRAM = 0;
	mem_ctrl_info->dimm_info[0].ranks_num = 2;
	mem_ctrl_info->dimm_info[0].ddr_3ds_num = 1;
	/* We also simulate an NVDIMM on slot one. */
	mem_ctrl_info->dimm_info[1].size_in_gb = 1;
	mem_ctrl_info->dimm_info[1].is_nvdimm = 1;
	mem_ctrl_info->dimm_info[1].is_single_DRAM = 0;
	mem_ctrl_info->dimm_info[1].ranks_num = 1;
	mem_ctrl_info->dimm_info[1].ddr_3ds_num = 1;
	mem_ctrl_info->dimm_info[1].pa_start_GB = (MEMORY_BASE >> 30) +
						  rdi.__reserved_2;

	return 1;
}

/*
 * We don't do the BIST POST test in the simulator as we haven't implemented
 * the hardware to do the test.
 */
void bf_sys_post_test(struct bf_mem_info *bmi)
{
}

/*
 * The fast models I2C model is modeled on our understanding of the hardware,
 * and it turned out to be not that accurate. We updated our driver code to
 * reflect the real hardware but haven't had a time to update the simulation
 * model. As a result, reading the SPD in the simulator would fail currently,
 * thus we are not reading the SPD currently in the simulator.
 * @TODO This code should be updated to call the I2C API (or removed to use
 * the common implementation) once the simulator is updated to reflect the
 * real hardware.
 */
int bf_sys_get_spd(uint8_t *spd, int offset, int len, int mss, int dimm)
{
	/*
	 * We return something here when not reading the whole SPD so that
	 * NVDIMMs can be initialized.
	 */
	if (len < 32) {
		spd[0] = 0;
		return 1;
	}

	return 0;
}

/* This is the memory setup on the simulator that we normally test out. */
int bf_sys_ddr_get_info_user(struct ddr_params *dp)
{
	dp->tck = 833333;
	dp->type = RDIMM;
	dp->dimm_num = 1;
	dp->dimm[0].ranks = 2;
	dp->dimm[0].is_nvdimm = 0;
	dp->dimm[0].density = DENSITY_4Gbit;
	dp->speed_bin = DDR4_2666U;
	dp->package = PACKAGE_x4;
	dp->ddr_3ds = 1;

	/*
	 * We disable signal normalization in simulation as our simulation
	 * memory model isn't sophisticated enough to support simulating
	 * real signal levels, and thus the normalization stage would cause
	 * lots of error messages.
	 */
	dp->wlrdqsg_lcdl_norm = 0;

	return 1;
}
