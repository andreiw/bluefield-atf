/*
 * Copyright (c) 2019, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <mmio.h>
#include "bluefield_def.h"
#include "bluefield_platform.h"
#include "bluefield_private.h"
#include "bluefield_system.h"
#include "rsh.h"
#include "tyu_def.h"

void bluefield_get_dev_tbl(struct bf_dev_tbl **dev_tbl,
			   uint32_t *disabled_devs)
{
	/*
	 * Figure out which chip we're on, and point to the relevant
	 * device table.
	 */
	struct bf_dev_tbl *tbl;

	/*
	 * Declare the known device tables here. "bf2" is the device table
	 * for the real chip which have 8 cores. "pdm_bf2" means the Palladium
	 * Mini Model which is a stripped down version with only 4 cores and
	 * less components for easier testing. "pdm_bf2_full" is the Palladium
	 * full model which is still worked on and thus missing some hardware
	 * blocks from the real chip.
	 */
	extern struct bf_dev_tbl bf2_dev_tbl[];
	extern struct bf_dev_tbl pdm_bf2_dev_tbl[];
	extern struct bf_dev_tbl pdm_bf2_full_dev_tbl[];

	/*
	 * Right now we only have three tables; we pick the little one if we
	 * have two or fewer tiles, otherwise we we see if the devid indicates
	 * we are running on Pallaidum and pick the Pallaidum full model, or
	 * use the one for the real chip.
	 */
	RSH_FABRIC_DIM_t rfd = {
		.word = mmio_read_64(RSHIM_BASE + RSH_FABRIC_DIM)
	};

	if (rfd.dim_x * rfd.dim_y <= 2)
		tbl = pdm_bf2_dev_tbl; /* mini-model */
	else {
		if (bluefield_devid_rev() == TYU_BOOT_DEVID_REV_BF2_PDM)
			tbl = pdm_bf2_full_dev_tbl; /* full-model */
		else
			tbl = bf2_dev_tbl; /* real hw */
	}

	/*
	 * For now we only support the ability to disable tiles; other devices
	 * can be disabled in the future simply by setting the corresponding
	 * bit in ddevs relative to our chosen device table to 1. We are
	 * assuming that the tiles are at the beginning of the device table.
	 * This should be the case as we need to set up the SAM of the
	 * first tile first before it can access other devices in the mesh.
	 */
	RSH_TILE_STATUS_t rts = {
		.word = mmio_read_64(RSHIM_BASE + RSH_TILE_STATUS)
	};
	uint32_t max_tile_num = rfd.dim_x * rfd.dim_y;
	uint32_t disabled_tiles = ~rts.hnf_ena & ((1 << max_tile_num) - 1);
	/* The primary tile is always enabled. */
	uint32_t ddevs =  disabled_tiles & (~1);
	/* Return device information for later configuration stages. */
	*dev_tbl = tbl;
	*disabled_devs = ddevs;
}

