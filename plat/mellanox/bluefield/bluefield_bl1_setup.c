/*
 * Copyright (c) 2013-2015, ARM Limited and Contributors. All rights reserved.
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

#include <arch.h>
#include <assert.h>
#include <bl_common.h>
#include <bl1.h>
#include <console.h>
#include <debug.h>
#include <delay_timer.h>
#include <platform.h>
#include <platform_def.h>
#include <tbbr_img_def.h>
#include <utils.h>
#include <xlat_tables.h>
#include "../../bl1/bl1_private.h"
#include "bluefield_private.h"
#include "bluefield_system.h"
#include "rsh.h"
#include "tyu_def.h"

/*
 * The next 2 constants identify the extents of the coherent memory region.
 * These addresses are used by the MMU setup code and therefore they must be
 * page-aligned.  It is the responsibility of the linker script to ensure that
 * __COHERENT_RAM_START__ and __COHERENT_RAM_END__ linker symbols refer to
 * page-aligned addresses.
 */
#define BL1_COHERENT_RAM_BASE (unsigned long)(&__COHERENT_RAM_START__)
#define BL1_COHERENT_RAM_LIMIT (unsigned long)(&__COHERENT_RAM_END__)

/* Data structure which holds the extents of the trusted SRAM for BL1*/
static meminfo_t bl1_tzram_layout;

meminfo_t *bl1_plat_sec_mem_layout(void)
{
	return &bl1_tzram_layout;
}
/*******************************************************************************
 * Perform any BL1 specific platform actions.
 ******************************************************************************/
void bl1_early_platform_setup(void)
{
	/* Initialize the console to provide early debug support */
	bluefield_console_init();

	/* Allow BL1 to see the whole Trusted RAM */
	bl1_tzram_layout.total_base = BL_RAM_BASE;
	bl1_tzram_layout.total_size = BL_RAM_SIZE;

	/* Initialize the platform config for future decision making. */
	bf_sys_config_setup();
	/*
	 * Initialize the simulated interconnect for this cluster during
	 * cold boot.  No need for locks as no other CPU is active.
	 */
	bf_sys_sim_ic_init();

	/* Enable the simulated interconnect for the primary CPU's cluster. */
	bf_sys_sim_ic_enable();

	/* Announce that BL1 have booted */
	tf_printf("Mellanox BlueField (Rev %x) BL1 V1.1\n",
		  mmio_read_32(TYU_BASE_ADDRESS + TYU_BOOT_DEVID));

#if DEBUG == 0
	/* Suppress all non-error boot logs for a faster boot time. */
	tf_log_set_max_level(LOG_LEVEL_ERROR);
#endif
}

/*
 * Override the default implementation which uses the first free memory
 * for BL2 to store BL2's memory information. BL2's first address is it's
 * text section, and overriding it would corrupt BL2's execution path.
 */
int bl1_plat_handle_post_image_load(unsigned int image_id)
{
	static meminfo_t bl2_tzram_layout;
	image_desc_t *image_desc;
	entry_point_info_t *ep_info;

	if (image_id != BL2_IMAGE_ID)
		return 0;

	image_desc = bl1_plat_get_image_desc(BL2_IMAGE_ID);
	assert(image_desc != NULL);

	/* Get the entry point info */
	ep_info = &image_desc->ep_info;

	bl1_calc_bl2_mem_layout(&bl1_tzram_layout, &bl2_tzram_layout);

	ep_info->args.arg1 = (uintptr_t)&bl2_tzram_layout;

	VERBOSE("BL1: BL2 memory layout address = %p\n",
		(void *) &bl2_tzram_layout);
	return 0;
}
/******************************************************************************
 * Perform the very early platform specific architecture setup. This only does
 * basic initialization. Later architectural setup (bl1_arch_setup()) does not
 * do anything platform specific.
 *****************************************************************************/
void bl1_plat_arch_setup(void)
{
	bluefield_setup_page_tables(bl1_tzram_layout.total_base,
				    bl1_tzram_layout.total_size,
				    BL_CODE_BASE,
				    BL1_CODE_END,
				    BL1_RO_DATA_BASE,
				    BL1_RO_DATA_END,
				    BL1_COHERENT_RAM_BASE,
				    BL1_COHERENT_RAM_LIMIT
			     );
	enable_mmu_el3(0);
}

static void bluefield_allow_livefish_pcie(void)
{
#if TRUSTED_BOARD_BOOT
	uint32_t tyu_fcrs;

	if (!plat_enable_tbb())
		return;

	tyu_fcrs = mmio_read_32(TYU_BASE_ADDRESS + TYU_FORCE_CRS_ADDR);

	/*
	 * If we're in secure boot mode, then a later boot stage will be
	 * responsible for authenticating the HCA firmware. Until that happens,
	 * PCIe access from or to the HCA is disabled by the FORCE_CRS
	 * configuration bit, which is always set at hard reset time, but only
	 * has an effect if secure boot is enabled.
	 * However, if we are in Livefish mode, then we're running built-in HCA
	 * firmware, which is already trusted. In this case, we want to enable
	 * PCIe access, because our boot stream might be coming from a PCIe
	 * master.
	 * FORCE_CRS is only enabled by hard reset, so if it's already been
	 * turned off by a previous boot, we can skip these checks.
	 */
	if (GET_REG_FIELD(TYU_FORCE_CRS, tyu_fcrs) && plat_enable_tbb()) {
		/* 20ms worth of ticks. */
		uint64_t wait_val = (uint64_t)BF_REF_CLK_IN_HZ * 20 / 1000;
		uint64_t tick_val;

		/* Wait for at least 20ms after reset. */
		do {
			/* How many ticks since the last reset. */
			tick_val = mmio_read_64(RSHIM_BASE + RSH_UPTIME);
		} while (tick_val < wait_val);

		/* Check to see if we are in LiveFish mode. */
		uint32_t tyu_livefish = mmio_read_32(TYU_BASE_ADDRESS +
						TYU_LIVEFISH_MODE_ADDR);
		if (GET_REG_FIELD(TYU_LIVEFISH_MODE, tyu_livefish)) {
			/* Deassert force_crs to let NIC have access to PCIe. */
			tyu_fcrs = SET_REG_FIELD(TYU_FORCE_CRS, tyu_fcrs, 0);
			mmio_write_32(TYU_BASE_ADDRESS + TYU_FORCE_CRS_ADDR,
					tyu_fcrs);
		}
	}
#endif
}

void bl1_platform_setup(void)
{
	/* Initialise the IO layer and register platform IO devices */
	bluefield_io_setup();

	bluefield_delay_timer_init();

	/* Handle alternate boot partition management. */
	bluefield_handle_alternate_boot();

	NOTICE("BL1 compiled for %s\n", TARGET_SYSTEM);

	bluefield_allow_livefish_pcie();

	if (!plat_enable_tbb())
		dyn_disable_auth();
}
