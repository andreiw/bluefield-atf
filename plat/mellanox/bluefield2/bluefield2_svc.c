/*
 * Copyright (c) 2019, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_helpers.h>
#include <bluefield_svc.h>
#include <mmio.h>
#include <platform_def.h>
#include <rsh.h>
#include <runtime_svc.h>
#include <spinlock.h>
#include <swap_boot.h>
#include <utils_def.h>
#include <uuid.h>

#include <emi_def.h>
#include <hnf_def.h>
#include <l3c_def.h>
#include <mmc_def.h>
#include <trio_def.h>

#include "bluefield_efi_info.h"
#include "bluefield_platform.h"


static struct reg_block PERF_BLOCKS[] = {
	PERF_L3_BLOCK(0, 0),
	PERF_L3_BLOCK(0, 1),
	REG_BLOCK(MSS0_BASE_ADDR, 0x00c0, 0x00f8),
	PERF_TRIO_BLOCKS(0),
	PERF_TRIO_BLOCKS(1),
	PERF_TILE_BLOCK(0),
	PERF_TILE_BLOCK(1),
	PERF_TILE_BLOCK(2),
	PERF_TILE_BLOCK(3),
	REG_BLOCK(MSS0_BASE_ADDR + EMEM_MI_OFFSET,
			EMI_ECC_SINGLE_ERROR_CNT * 4,
			EMI_DRAM_FIRST_LAST * 4),
	BLOCK_TABLE_END
};

static struct reg_block L3C_BLOCKS[] = {
	L3C_PROF_BLOCK(0, 0),
	L3C_PROF_BLOCK(0, 1)
};

static struct reg_block *regs_table[] = {
	[MLNX_REGS_PERF] = PERF_BLOCKS,
	[MLNX_REGS_L3C]  = L3C_BLOCKS
};

struct reg_block **register_tables = regs_table;

const unsigned int NUM_REG_TABLES = ARRAY_SIZE(regs_table);

static uintptr_t get_tbb_fuse_status(void *handle, u_register_t fuse_status)
{
	RSH_SB_KEY_VLD_t key_vld;
	uint64_t sb_mode;
	uint64_t sb_ctr_sts;
	u_register_t result;

	switch (fuse_status) {

	case MLNX_FUSE_STATUS_LIFECYCLE:

		sb_mode = mmio_read_64(RSHIM_BASE + RSH_SB_MODE);
		sb_ctr_sts = mmio_read_64(RSHIM_BASE + RSH_SB_CTL_STAT);

		/*
		 * Setting the test field of a non secure chip and soft
		 * resetting the Arm cores would also make the chip behave as
		 * if it is in secure state. So also return this field so that
		 * the caller can be aware of this.
		 */
		result = (sb_mode & RSH_SB_MODE__SECURE_MASK) |
			 (sb_ctr_sts & RSH_SB_CTL_STAT__TEST_MASK);
		break;

	case MLNX_FUSE_STATUS_KEYS:

		key_vld.word = mmio_read_64(RSHIM_BASE + RSH_SB_KEY_VLD);

		result = key_vld.BF_KEY_VLD;
		break;

	default:
		result = SMCCC_INVALID_PARAMETERS;
	}

	SMC_RET1(handle, result);
}

uintptr_t bluefield_plat_smc_handler(uint32_t smc_fid, u_register_t x1,
				     u_register_t x2, u_register_t x3,
				     u_register_t x4, void *cookie,
				     void *handle, u_register_t flags)
{
	switch (smc_fid) {

	case MLNX_GET_TBB_FUSE_STATUS:
		return get_tbb_fuse_status(handle, x1);

	default:
		SMC_RET1(handle, SMC_UNK);
	}
}
