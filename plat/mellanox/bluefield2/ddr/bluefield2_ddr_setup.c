/*
 * Copyright (c) 2019, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <debug.h>

#include "bluefield_ddr.h"

/* The place where we store the DDR parameters for the current setup. */
struct ddr_params *dp;
/* Struct storing the ddr parameters. */
struct ddr_params dps[MAX_MEM_CTRL];

int ddr_do_actual_setup(void)
{
	ERROR("BlueField 2 memory setup not implemented yet.\n");
	return -1;
}

struct ddr_params *bluefield_setup_mss(uintptr_t mss_addr, int mem_ctrl_num)
{
	ERROR("BlueField 2 memory configuration not implemented yet.\n");
	return NULL;
}
