/*
 * Copyright (c) 2019, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __BLUEFIELD_PLATFORM_H__
#define __BLUEFIELD_PLATFORM_H__

#include <stdint.h>
#include "bluefield_private.h"

/*
 * All functions/variables declared in the header file should be implemented
 * in all specific platform code.
 */

/*
 * A register block is defined by the highest and lowest registers
 * accessible in the block. End is inclusive.
 */
struct reg_block {
	uint64_t start;
	uint64_t end;
};

#define REG_BLOCK(_block_base, _start_reg, _end_reg) \
	{ .start = _block_base + _start_reg,             \
	  .end   = _block_base + _end_reg}

#define PERF_L3_BLOCK(_mss, _l3half)                    \
	REG_BLOCK(MSS_BASE(_mss) + L3_HALF_OFFSET(_l3half), \
		L3C_PERF_CNT_CFG*4,                             \
		L3C_PERF_CNT_HIGH__LAST_WORD*4)

#define PERF_TRIO_BLOCKS(_trio)                   \
	REG_BLOCK(TRIO_BASE(_trio),                   \
		0x00c0,                                   \
		0x00f8),                                  \
	REG_BLOCK(TRIO_BASE(_trio),                   \
		TRIO_GEN_TRIO_PERFMON_CONFIG,             \
		TRIO_GEN_TRIO_PERFMVAL),                  \
	REG_BLOCK(TRIO_BASE(_trio) + PCIE_TLR_OFFSET, \
		0x8000,                                   \
		0x8060)

#define PERF_TILE_BLOCK(_tile)                  \
	REG_BLOCK(TILE_BASE(_tile),                 \
		HNF_GEN_HNF_PERFMON_CONFIG__FIRST_WORD, \
		HNF_GEN_HNF_PERFMVAL__LAST_WORD)

#define L3C_PROF_BLOCK(_mss, _l3half)				\
	REG_BLOCK(MSS_BASE(_mss) + L3_HALF_OFFSET(_l3half),	\
		L3C_PROF_RD_MISS__FIRST_WORD*4,			\
		L3C_PROF_WR_HIT1__LAST_WORD*4)

#define BLOCK_TABLE_END {0, 0}

/* Reg tables for verify. */
struct reg_block **register_tables;

/* Number of entries in register_tables */
const unsigned int NUM_REG_TABLES;


/* Get the device specific table for the platform */
void bluefield_get_dev_tbl(struct bf_dev_tbl **dev_tbl,
			   uint32_t *disabled_devs);

uintptr_t bluefield_plat_smc_handler(uint32_t smc_fid, u_register_t x1,
				     u_register_t x2, u_register_t x3,
				     u_register_t x4, void *cookie,
				     void *handle, u_register_t flags);

#define RSH_PWR_WDOG_STATUS_CHECK(field, max_ms, err_msg) do { \
	uint32_t ms = 0; \
	RSH_PWR_WDOG_STATUS_t status; \
	while (1) { \
		status.word = mmio_read_64(RSHIM_BASE + RSH_PWR_WDOG_STATUS); \
		if (status.field) \
			break; \
		if (ms++ > max_ms) { \
			ERROR(err_msg); \
			return; \
		} \
		mdelay(1); \
	} \
} while(0)

void bluefield_nvdimm_save_cstate(int mss_idx);

#endif /* __BLUEFIELD_PLATFORM_H__ */
