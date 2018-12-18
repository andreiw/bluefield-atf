/*
 * Copyright (c) 2016, ARM Limited and Contributors. All rights reserved.
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

#include <stdint.h>

#include "bluefield_private.h"

/* System Address Map information for various topologies. */

struct bf_dev_tbl bf16_dev_tbl[] = {
	/* tile0 */
	{
		.nodeid = 0x0,	/* (0,0) */
		.devtbl_idx = -1,
		.has_sam = 1,
		.dev_type = DEV_TILE,
		.sec_nodeid = 0x0,
		.base_addr = 0x0058000000ULL,
	},
	/* tile1 */
	{
		.nodeid = 0x10,	/* (1,0) */
		.devtbl_idx = -1,
		.has_sam = 1,
		.dev_type = DEV_TILE,
		.sec_nodeid = 0x0,
		.base_addr = 0x0058400000ULL,
	},
	/* tile2 */
	{
		.nodeid = 0x20,	/* (2,0) */
		.devtbl_idx = -1,
		.has_sam = 1,
		.dev_type = DEV_TILE,
		.sec_nodeid = 0x0,
		.base_addr = 0x0058800000ULL,
	},
	/* tile3 */
	{
		.nodeid = 0x30,	/* (3,0) */
		.devtbl_idx = -1,
		.has_sam = 1,
		.dev_type = DEV_TILE,
		.sec_nodeid = 0x0,
		.base_addr = 0x0058c00000ULL,
	},
	/* tile4 */
	{
		.nodeid = 0x1,	/* (0,1) */
		.devtbl_idx = -1,
		.has_sam = 1,
		.dev_type = DEV_TILE,
		.sec_nodeid = 0x0,
		.base_addr = 0x0059000000ULL,
	},
	/* tile5 */
	{
		.nodeid = 0x11,	/* (1,1) */
		.devtbl_idx = -1,
		.has_sam = 1,
		.dev_type = DEV_TILE,
		.sec_nodeid = 0x0,
		.base_addr = 0x0059400000ULL,
	},
	/* tile6 */
	{
		.nodeid = 0x21,	/* (2,1) */
		.devtbl_idx = -1,
		.has_sam = 1,
		.dev_type = DEV_TILE,
		.sec_nodeid = 0x0,
		.base_addr = 0x0059800000ULL,
	},
	/* tile7 */
	{
		.nodeid = 0x31,	/* (3,1) */
		.devtbl_idx = -1,
		.has_sam = 1,
		.dev_type = DEV_TILE,
		.sec_nodeid = 0x0,
		.base_addr = 0x0059c00000ULL,
	},
	/* rsh0 */
	{
		.nodeid = 0x73,	/* (3,-1) */
		.devtbl_idx = 16,
		.has_sam = 1,
		.dev_type = DEV_RSH,
		.sec_nodeid = 0x0,
		.base_addr = 0x0000800000ULL,
	},
	/* smmu0 */
	{
		.nodeid = 0x78,	/* (0,2) */
		.devtbl_idx = 17,
		.has_sam = 1,
		.dev_type = DEV_SMMU,
		.sec_nodeid = 0x0,
		.base_addr = 0x0008000000ULL,
	},
	/* gic0 */
	{
		.nodeid = 0x70,	/* (0,-1) */
		.devtbl_idx = 18,
		.has_sam = 1,
		.dev_type = DEV_GIC,
		.sec_nodeid = 0x0,
		.base_addr = 0x0010000000ULL,
	},
	/* mss0 */
	{
		.nodeid = 0x60,	/* (-1,0) */
		.devtbl_idx = 19,
		.has_sam = 0,
		.dev_type = DEV_MSS,
		.sec_nodeid = 0x61,
		.base_addr = 0x0018000000ULL,
	},
	/* mss1 */
	{
		.nodeid = 0x68,	/* (4,0) */
		.devtbl_idx = 20,
		.has_sam = 0,
		.dev_type = DEV_MSS,
		.sec_nodeid = 0x69,
		.base_addr = 0x0020000000ULL,
	},
	/* crypto0 */
	{
		.nodeid = 0x70,	/* (0,-1) */
		.devtbl_idx = 24,
		.has_sam = 0,
		.dev_type = DEV_CRYPTO,
		.sec_nodeid = 0x0,
		.base_addr = 0x0040000000ULL,
	},
	/* crypto1 */
	{
		.nodeid = 0x79,	/* (1,2) */
		.devtbl_idx = 25,
		.has_sam = 0,
		.dev_type = DEV_CRYPTO,
		.sec_nodeid = 0x0,
		.base_addr = 0x0048000000ULL,
	},
	/* tp0 */
	{
		.nodeid = 0x71,	/* (1,-1) */
		.devtbl_idx = 4,
		.has_sam = 1,
		.dev_type = DEV_TP,
		.sec_nodeid = 0x0,
		.base_addr = 0xdd00000000ULL,
	},
	/* tp1 */
	{
		.nodeid = 0x72,	/* (2,-1) */
		.devtbl_idx = 5,
		.has_sam = 1,
		.dev_type = DEV_TP,
		.sec_nodeid = 0x0,
		.base_addr = 0xdd40000000ULL,
	},
	/* hca0 */
	{
		.nodeid = 0x7a,	/* (2,2) */
		.devtbl_idx = 6,
		.has_sam = 1,
		.dev_type = DEV_HCA,
		.sec_nodeid = 0x0,
		.base_addr = 0xdd80000000ULL,
	},
	/* End of table */
	{
		.base_addr = ~0ULL,
	}
};

struct bf_dev_tbl pdm_dev_tbl[] = {
	/* tile0 */
	{
		.nodeid = 0x0,	/* (0,0) */
		.devtbl_idx = -1,
		.has_sam = 1,
		.dev_type = DEV_TILE,
		.sec_nodeid = 0x0,
		.base_addr = 0x0058000000ULL,
	},
	/* tile1 */
	{
		.nodeid = 0x10,	/* (1,0) */
		.devtbl_idx = -1,
		.has_sam = 1,
		.dev_type = DEV_TILE,
		.sec_nodeid = 0x0,
		.base_addr = 0x0058400000ULL,
	},
	/* rsh0 */
	{
		.nodeid = 0x71,	/* (1,-1) */
		.devtbl_idx = 16,
		.has_sam = 1,
		.dev_type = DEV_RSH,
		.sec_nodeid = 0x0,
		.base_addr = 0x0000800000ULL,
	},
	/* smmu0 */
	{
		.nodeid = 0x71,	/* (1,-1) */
		.devtbl_idx = 17,
		.has_sam = 1,
		.dev_type = DEV_SMMU,
		.sec_nodeid = 0x0,
		.base_addr = 0x0008000000ULL,
	},
	/* gic0 */
	{
		.nodeid = 0x60,	/* (-1,0) */
		.devtbl_idx = 18,
		.has_sam = 1,
		.dev_type = DEV_GIC,
		.sec_nodeid = 0x0,
		.base_addr = 0x0010000000ULL,
	},
	/* mss0 */
	{
		.nodeid = 0x79,	/* (1,1) */
		.devtbl_idx = 19,
		.has_sam = 0,
		.dev_type = DEV_MSS,
		.sec_nodeid = 0x78,
		.base_addr = 0x0018000000ULL,
	},
	/* tp0 */
	{
		.nodeid = 0x68,	/* (2,0) */
		.devtbl_idx = 4,
		.has_sam = 1,
		.dev_type = DEV_TP,
		.sec_nodeid = 0x0,
		.base_addr = 0xdd00000000ULL,
	},
	/* End of table */
	{
		.base_addr = ~0ULL,
	}
};
