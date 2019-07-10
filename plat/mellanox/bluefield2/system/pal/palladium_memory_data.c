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

#include "bluefield_private.h"

/* File storing the memory setup sequence of commands */

const union mem_config_cmd bf2_mem_config_array[] = {
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x100,
			.reg_data = 0x1c252a43
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x101,
			.reg_data = 0x70d0a
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x102,
			.reg_data = 0x1a1d3800
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x103,
			.reg_data = 0x189081a5
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x104,
			.reg_data = 0x8070206
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x105,
			.reg_data = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x106,
			.reg_data = 0x14e1248
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x107,
			.reg_data = 0x1300924
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x108,
			.reg_data = 0x1210492
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x109,
			.reg_data = 0x604236
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x10a,
			.reg_data = 0xc0400021
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x10b,
			.reg_data = 0x2
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x10d,
			.reg_data = 0x1010101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x116,
			.reg_data = 0x0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x117,
			.reg_data = 0xf0008134
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x11a,
			.reg_data = 0x560000
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x11b,
			.reg_data = 0x7
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x160,
			.reg_data = 0x9240492
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x161,
			.reg_data = 0x12480db6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x162,
			.reg_data = 0xdb60924
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x163,
			.reg_data = 0x16da1248
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x164,
			.reg_data = 0x12480db6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x165,
			.reg_data = 0x1b6c16da
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x166,
			.reg_data = 0x16da1248
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x167,
			.reg_data = 0x1ffe1b6c
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x1a3,
			.reg_data = 0x20400
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x0,
			.wait_cycle = 10
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x10
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x301
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x100,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1ff,
			.reg_data = 0x1100
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 9
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 12,
			.block_id = 0x3,
			.reg_addr = 0x1ff,
			.reg_data = 0x1000
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 9
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1ff,
			.reg_data = 0x0
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 9
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 9
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x120,
			.reg_data = 0xf
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 12
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c1c00d4
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000401
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c1c0000
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c1c0010
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c1c0020
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c1c0030
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c1c0040
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c1c0050
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c1c0080
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c1c0090
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c1c00a3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c1c00b0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c1c00e0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c1c00f4
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000401
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c1c033a
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x42000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x161c00d4
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000401
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x161c0000
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x161c0010
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x161c0020
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x161c0030
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x161c0040
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x161c0050
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x161c0080
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x161c0090
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x161c00a3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x161c00b0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x161c00e0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x161c00f4
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000401
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x161c033a
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x42000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c0c0000
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c18085f
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c140000
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c100000
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c080018
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c040109
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c000964
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x42000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c322bf8
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c2623a7
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c2a2bf8
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c2e2bf8
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c362be0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c3a2af1
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c3e229c
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x42000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1a0c0000
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1a18085f
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1a140000
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1a100000
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1a080018
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1a040109
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1a000964
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x42000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1a322bf8
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1a2623a7
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1a2a2bf8
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1a2e2bf8
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1a362be0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1a3a2af1
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1a3e229c
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x42000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1c1c0001
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x42000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x160c0000
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x1618085f
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x16140000
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x16100000
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x16080018
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x16040109
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x16000964
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x42000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x16322bf8
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x162623a7
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x162a2bf8
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x162e2bf8
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x16362be0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x163a2af1
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x163e229c
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x42000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xe0c0000
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xe18085f
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xe140000
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xe100000
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xe080018
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xe040109
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xe000964
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x42000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xe322bf8
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xe2623a7
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xe2a2bf8
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xe2e2bf8
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xe362be0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xe3a2af1
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x2000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xe3e229c
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x42000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x161c0001
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x42000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 9
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x7c000400
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x42000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x7a000400
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x42000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x76000400
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x42000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x6e000400
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x1,
			.reg_data = 0x42000101
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x1
		}
	},
	{
		.read = {
			.cmd_type = READ_CMD,
			.wait_cycle = 4,
			.block_id = 0x3,
			.reg_addr = 0x15,
			.loop_reg_msk = 0x3
		}
	},
	{
		.loop = {
			.cmd_type = LOOP_CMD,
			.max_cnt = 0x1000,
			.exp_val = 0x3
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 14
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 8
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x116,
			.reg_data = 0x10003
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 9
		}
	},
	{
		.enop = {
			.cmd_type = ENOP_CMD,
			.wait_cycle = 13,
			.reg_data = 0x80000000
		}
	},
	{
		.raw_val = 0
	}
};
