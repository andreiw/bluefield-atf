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

const union mem_config_cmd bf1_mem_config_array[] = {
	{
		.enop = {
			.cmd_type = ENOP_CMD,
			.wait_cycle = 0,
			.reg_data = 0x2b
		}
	},
	{
		.enop = {
			.cmd_type = ENOP_CMD,
			.wait_cycle = 13,
			.reg_data = 0x2b
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x100,
			.reg_data = 0x20312f4b
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x101,
			.reg_data = 0x90e0b
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x102,
			.reg_data = 0x1e213e06
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x103,
			.reg_data = 0x41210116
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x104,
			.reg_data = 0x90906
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x105,
			.reg_data = 0x4
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x106,
			.reg_data = 0x1571450
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x107,
			.reg_data = 0x1358a28
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x108,
			.reg_data = 0x1250514
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x109,
			.reg_data = 0x6c4a3c
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
			.reg_data = 0x1
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x10d,
			.reg_data = 0x101
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
			.reg_addr = 0x11b,
			.reg_data = 0x7
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x121,
			.reg_data = 0x88801
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x160,
			.reg_data = 0x14500a28
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x161,
			.reg_data = 0x28a01e78
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x162,
			.reg_data = 0x1e781450
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 1,
			.block_id = 0x3,
			.reg_addr = 0x163,
			.reg_data = 0x32c828a0
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
		.enop = {
			.cmd_type = ENOP_CMD,
			.wait_cycle = 13,
			.reg_data = 0x20
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
			.reg_data = 0x3
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
			.reg_data = 0x1c1c00a4
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
			.reg_data = 0x1c1c0347
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
			.reg_data = 0x1c180c5f
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
			.reg_data = 0x1c080020
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
			.reg_data = 0x1c000b70
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
			.reg_data = 0x1c2627a7
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
			.reg_data = 0x1c362bd8
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
			.reg_data = 0x1c3e2088
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
			.reg_data = 0x1a180c5f
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
			.reg_data = 0x1a080020
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
			.reg_data = 0x1a000b70
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
			.reg_data = 0x1a2627a7
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
			.reg_data = 0x1a362bd8
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
			.reg_data = 0x1a3e2088
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
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 14
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd9
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x1e0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd81c
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x1e2
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x20002
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x1f0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd9
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x220
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd81c
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x222
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x20002
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x230
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd9
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x260
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd81c
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x262
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x20002
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x270
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd9
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x2a0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd81c
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x2a2
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x20002
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x2b0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd9
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x2e0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd81c
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x2e2
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x20002
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x2f0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd9
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x320
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd81c
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x322
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x20002
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x330
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd9
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x360
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd81c
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x362
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x20002
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x370
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd9
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x3a0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd81c
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x3a2
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x20002
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x3b0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd9
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x3e0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0xd81c
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x3e2
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x0,
			.reg_data = 0x20002
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x14,
			.reg_data = 0x3f0
		}
	},
	{
		.write = {
			.cmd_type = WRITE_CMD,
			.wait_cycle = 0,
			.block_id = 0x3,
			.reg_addr = 0x10,
			.reg_data = 0x101
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 6
		}
	},
	{
		.xnop = {
			.cmd_type = XNOP_CMD,
			.block_id = 0x3,
			.wait_cycle = 13
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
