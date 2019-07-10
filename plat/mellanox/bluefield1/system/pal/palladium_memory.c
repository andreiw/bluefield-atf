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
 * Neither the name of ARM nor the names of its contributors may be used
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
#include "bluefield_private.h"
#include "tyu_def.h"

/* Skip reading the SPD. */
static int __bf_spd_skip(uint8_t *spd, int offset, int len, int mss, int dimm)
{
	return 0;
}

/* Get information solely from the SPD. */
static int __bf_user_noinfo(struct ddr_params *dp)
{
	return 1;
}

/* Use the default analog parameter table. */
static int __bf_board_default(struct ddr_params *dp)
{
	return bf_sys_ddr_get_info_board_default(dp);
}

/* Setup the memory by just reading the SPD. */
const struct ddr_config mem_spd = {
	.get_spd = __bf_spd_skip,
	.info_user = __bf_user_noinfo,
	.info_board = __bf_board_default,
	.config = "via SPD",
};

/* System info for palladium */
const struct sys_info palladium_system = {
	.mss0_config = &mem_spd,
	.mss1_config = &mem_spd,
	.sys_name = "palladium",
	.psid_vals = (uint16_t []){0}, /* Not applicable */
	.snic_model = 0,
};

/*
 * Function to reset/deassert sections of the memory controller based on the
 * given values.
 * Mask: A combination of the MSSx_XX_EN macros indicating which pins to modify.
 * Value: Should be 0 or 1, indicates the bits given in mask should be modified
 * to high or low.
 */
static void reset_mss(uint32_t mask, int value)
{
	uint32_t mss_reset_reg_data;

	if (value != 0 && value != 1) {
		ERROR("Wrong argument passed to reset_memory function.\n");
		return;
	}

	mss_reset_reg_data = mmio_read_32(TYU_BASE_ADDRESS + TYU_MSS_RESET);

	if (value)
		mss_reset_reg_data |= mask;
	else
		mss_reset_reg_data &= ~mask;

	mmio_write_32(TYU_BASE_ADDRESS + TYU_MSS_RESET, mss_reset_reg_data);
}

static void execute_write(struct mem_config_write_cmd entry, uintptr_t mem_base)
{
	uint32_t wait = GET_REAL_WAIT(entry.wait_cycle);

	if (wait > 0)
		mem_config_ndelay(wait);
	mem_config_write(mem_base, entry.block_id, entry.reg_addr,
			 entry.reg_data);
}

static uint32_t execute_read(struct mem_config_read_cmd entry,
			     struct mem_config_loop_cmd lp_entry,
			     uintptr_t mem_base)
{
	assert(lp_entry.cmd_type == LOOP_CMD);
	uint32_t wait = GET_REAL_WAIT(entry.wait_cycle);
	uint32_t data = 0;
	uint32_t lp_counter;

	for (lp_counter = 0; lp_counter <  lp_entry.max_cnt; lp_counter++) {
		if (wait > 0)
			mem_config_ndelay(wait);

		data = mem_config_read(mem_base, entry.block_id,
				       entry.reg_addr);

		if ((data & entry.loop_reg_msk) == lp_entry.exp_val)
			break;
	}
	if (lp_counter >= lp_entry.max_cnt)
		ERROR("Mem_config read failed, got data 0x%x\n", data);

	return data;
}

static void execute_enop(struct mem_config_enop_cmd entry, int mem_ctrl_num)
{
	static int call_count[MAX_MEM_CTRL];

	call_count[mem_ctrl_num]++;

	if (call_count[mem_ctrl_num] == 3) {
		if (mem_ctrl_num == 0)
			reset_mss(MSS0_DDR_MC_SRST_EN |
				  MSS0_UMX_MC_DDR_RESET_EN, 0);
		else
			reset_mss(MSS1_DDR_MC_SRST_EN |
				  MSS1_UMX_MC_DDR_RESET_EN, 0);
	}
	mem_config_ndelay(GET_REAL_WAIT(entry.wait_cycle));
}

static void execute_xnop(struct mem_config_xnop_cmd entry)
{
	mem_config_ndelay(GET_REAL_WAIT(entry.wait_cycle));
}

/*
 * Configure particular MSS on the Palladium indicated by given base
 * address and fixed sequence. Also fill out the struct describing the
 * DIMMs corresponding to the controller.
 */
int bf_sys_mem_config(uintptr_t mem_base,
		      struct bf_mem_ctrl_info *mem_ctrl_info,
		      int mem_ctrl_num)
{
	extern const union mem_config_cmd bf1_mem_config_array[];
	const union mem_config_cmd *mem_config_array;
	int index;

	if (mem_ctrl_num == 0)
		reset_mss(MSS0_DDR_MC_GRST_EN, 0);
	else
		reset_mss(MSS1_DDR_MC_GRST_EN, 0);

	mem_config_array = bf1_mem_config_array;

	for (index = 0; mem_config_array[index].raw_val != 0; index++) {
		union mem_config_cmd entry = mem_config_array[index];

		switch (entry.general.cmd_type) {
		case XNOP_CMD:
			execute_xnop(entry.xnop);
			break;
		case ENOP_CMD:
			execute_enop(entry.enop, mem_ctrl_num);
			break;
		case WRITE_CMD:
			execute_write(entry.write, mem_base);
			break;
		case READ_CMD: {
			union mem_config_cmd nx = mem_config_array[++index];

			execute_read(entry.read, nx.loop, mem_base);
			break;
		}
		default:
			ERROR("Unknown memory config opcode 0x%llx!\n",
			      entry.raw_val);
			break;
		}
	}

	/* Static memory configuration information for the Palladium. */
	mem_ctrl_info->dimm_info[0].size_in_gb = 16 * 8;
	mem_ctrl_info->dimm_info[0].is_nvdimm = 0;
	mem_ctrl_info->dimm_info[0].is_single_DRAM = 0;
	mem_ctrl_info->dimm_info[0].ranks_num = 2;

	/* We still populate the dp struct here so BIST can use it. */
	ddr_get_info(&(dps[mem_ctrl_num]), mem_base, mem_ctrl_num);

	return 1;
}

/*
 * We don't do the BIST POST test on the Palladium as it takes a very long time
 * to finish, and we trust that there shouldn't be any issues with the emulated
 * memory on the Palladium.
 */
void bf_sys_post_test(struct bf_mem_info *bmi)
{
}

/* There is no SPD to read on the Palladium. */
int bf_sys_get_spd(uint8_t *spd, int offset, int len, int mss, int dimm)
{
	return 0;
}

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

	return 1;
}

int bf_sys_is_pal(void)
{
	return 1;
}

/* Returns pointer to the system running, or NULL if can't be figured out. */
const struct sys_info *get_current_sys(void)
{
        return &palladium_system;
}
