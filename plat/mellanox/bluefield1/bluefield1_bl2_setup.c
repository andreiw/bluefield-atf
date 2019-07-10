/*
 * Copyright (c) 2019, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <mmio.h>
#include "bluefield_common.h"
#include "bluefield_def.h"
#include "bluefield_efi_info.h"
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
	 * Declare the known device tables here. "bf16" is the device table
	 * for the real chip which have 16 cores. "pdm" means the Palladium
	 * Mini Model which is a stripped down version with only 4 cores and
	 * less components for easier testing.
	 */
	extern struct bf_dev_tbl bf16_dev_tbl[];
	extern struct bf_dev_tbl pdm_dev_tbl[];

	/*
	 * Right now we only have two tables; we pick the little one if we
	 * have two or fewer tiles, otherwise we pick the big one.
	 */
	RSH_FABRIC_DIM_t rfd = {
		.word = mmio_read_64(RSHIM_BASE + RSH_FABRIC_DIM)
	};

	if (rfd.dim_x * rfd.dim_y <= 2)
		tbl = pdm_dev_tbl;
	else
		tbl = bf16_dev_tbl;

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

void bluefield_setup_efi_info(uintptr_t addr)
{
#ifdef ENABLE_2ND_EMMC
	struct bf_efi *efi_info = (struct bf_efi *)addr;

	RSH_BF1_GPIO_MODE_t mode;
	RSH_DEVICE_PROTECTION_t protect;
#endif

	bluefield_common_setup_efi_info(addr);

#ifdef ENABLE_2ND_EMMC
	if (!efi_info->snic_model) {
		/*
		 * Enable the 2nd eMMC and set the flag in efi_info which will
		 * be used in UEFI. UEFI doesn't have access to register
		 * RSH_BF1_GPIO_MODE.
		 */
		mode.word = mmio_read_64(RSHIM_BASE + RSH_BF1_GPIO_MODE);
		mode.mmc_alt_card4 = 1;	/* Enables 4-bit alternate MMC card. */
		mode.mmc_alt_card = 1;	/* Enables alternate MMC card. */
		mmio_write_64(RSHIM_BASE + RSH_BF1_GPIO_MODE, mode.word);
		efi_info->sec_emmc = 1;

		/* Disable DIAG_UART. */
		protect.word = mmio_read_64(RSHIM_BASE + RSH_DEVICE_PROTECTION);
		protect.disable_dev_diag_uart = 1;
		mmio_write_64(RSHIM_BASE + RSH_DEVICE_PROTECTION, protect.word);
	}
#endif
}
