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
#include <arch_helpers.h>
#include <arm_gic.h>
#include <assert.h>
#include <bl_common.h>
#include <ccn.h>
#include <debug.h>
#include <desc_image_load.h>
#include <mmio.h>
#include <pl011.h>
#include <platform.h>
#include <platform_def.h>
#include <xlat_tables.h>
#include "bluefield_def.h"
#include "bluefield_private.h"
#include "rsh_def.h"
#include "tmfifo_console.h"

/*
 * Table of regions for various BL stages to map using the MMU.
 * This doesn't include TZRAM as the 'mem_layout' argument passed to
 * bluefield_setup_page_tables() will give the available subset of that.
 *
 * IMPORTANT: MAP_NS_DRAM1 must go last! This map entry is corrected based
 * on the real size of DRAM later on, and is assumed to be the last
 * entry in the list.
 */
#if IMAGE_BL1
const mmap_region_t bluefield_mmap[] = {
	MAP_SHARED_RAM,
	MAP_DEVICES,
#if TRUSTED_BOARD_BOOT
	MAP_NS_DRAM1,
#endif
	{0}
};
#endif
#if IMAGE_BL2
const mmap_region_t bluefield_mmap[] = {
	MAP_SHARED_RAM,
	MAP_DEVICES,
	MAP_NS_DRAM1,
	{0}
};
#endif
#if IMAGE_BL31
const mmap_region_t bluefield_mmap[] = {
	MAP_TRIO_DEVICES,
	MAP_SHARED_RAM,
	MAP_DEVICES,
	MAP_NS_DRAM1,
	{0}
};
#endif

CASSERT((ARRAY_SIZE(bluefield_mmap) + BL_REGIONS) <= MAX_MMAP_REGIONS,
	assert_max_mmap_regions);


/*
 * Set up the page tables for the generic and platform-specific memory regions.
 * The extents of the generic memory regions are specified by the function
 * arguments and consist of:
 * - Trusted SRAM seen by the BL image;
 * - Code section;
 * - Read-only data section;
 * - Coherent memory region, if applicable.
 */
void bluefield_setup_page_tables(uintptr_t total_base,
				 size_t total_size,
				 uintptr_t code_start,
				 uintptr_t code_limit,
				 uintptr_t rodata_start,
				 uintptr_t rodata_limit,
				 uintptr_t coh_start,
				 uintptr_t coh_limit
			   )
{
	/*
	 * Map the Trusted SRAM with appropriate memory attributes.
	 * Subsequent mappings will adjust the attributes for specific regions.
	 */
	VERBOSE("Trusted SRAM seen by this BL image: %p - %p\n",
		(void *) total_base, (void *) (total_base + total_size));
	mmap_add_region(total_base, total_base,
			total_size,
			MT_MEMORY | MT_RW | MT_SECURE);

	/* Re-map the code section */
	VERBOSE("Code region: %p - %p\n",
		(void *) code_start, (void *) code_limit);
	mmap_add_region(code_start, code_start,
			code_limit - code_start,
			MT_CODE | MT_SECURE);

	/* Re-map the read-only data section */
	VERBOSE("Read-only data region: %p - %p\n",
		(void *) rodata_start, (void *) rodata_limit);
	mmap_add_region(rodata_start, rodata_start,
			rodata_limit - rodata_start,
			MT_RO_DATA | MT_SECURE);

	/* Re-map the coherent memory region */
	VERBOSE("Coherent region: %p - %p\n",
		(void *) coh_start, (void *) coh_limit);
	mmap_add_region(coh_start, coh_start,
			coh_limit - coh_start,
			MT_DEVICE | MT_RW | MT_SECURE);

	/* Now (re-)map the platform-specific memory regions */
	mmap_add(bluefield_get_mmap());

	/* Create the page tables to reflect the above mappings */
	init_xlat_tables();
}

uintptr_t plat_get_ns_image_entrypoint(void)
{
#ifdef PRELOADED_BL33_BASE
	return PRELOADED_BL33_BASE;
#else
	return NS_IMAGE_OFFSET;
#endif
}

/*******************************************************************************
 * Function that sets up the console
 ******************************************************************************/
static console_pl011_t bf_console;
#ifdef ENABLE_SEC_UART
static console_pl011_t bf_console2;
#endif
console_tmfifo_t tm_console;

void bluefield_console_init(void)
{
	uint32_t bf_console_baudrate = bluefield_get_baudrate();

	if (!console_pl011_register(BOOT_UART_BASE, BOOT_UART_CLK_IN_HZ,
					bf_console_baudrate, &bf_console))
		panic();

	console_set_scope(&bf_console.console,
			  CONSOLE_FLAG_BOOT | CONSOLE_FLAG_RUNTIME);

#ifdef ENABLE_SEC_UART
	if (!console_pl011_register(BF_UART1_BASE, BOOT_UART_CLK_IN_HZ,
					bf_console_baudrate, &bf_console2))
		panic();

	console_set_scope(&bf_console2.console,
			  CONSOLE_FLAG_BOOT | CONSOLE_FLAG_RUNTIME);
#endif

	/*
	 * It is important that we register the tmfifo console last so it
	 * is at the head of the console list, thus our own non-blocking
	 * getc() implemenation works without hacking the common code.
	 */
	if (!console_tmfifo_register(&tm_console,
				     RSHIM_BASE + RSH_TM_TILE_TO_HOST_DATA,
				     RSHIM_BASE + RSH_TM_TILE_TO_HOST_STS,
				     RSHIM_BASE + RSH_TM_HOST_TO_TILE_DATA,
				     RSHIM_BASE + RSH_TM_HOST_TO_TILE_STS))
		panic();

	/*
	 * If we enable the tmfifo for runtime, it might corrupt messages sent
	 * by the Linux driver.
	 */
	console_set_scope(&tm_console.console, CONSOLE_FLAG_BOOT);
}

/*******************************************************************************
 * Gets SPSR for BL33 entry
 ******************************************************************************/
uint32_t bluefield_get_spsr_for_bl33_entry(void)
{
	unsigned long el_status;
	unsigned int mode;
	uint32_t spsr;

	/* Figure out what mode we enter the non-secure world in */
	el_status = read_id_aa64pfr0_el1() >> ID_AA64PFR0_EL2_SHIFT;
	el_status &= ID_AA64PFR0_ELX_MASK;

	mode = (el_status) ? MODE_EL2 : MODE_EL1;

	/*
	 * TODO: Consider the possibility of specifying the SPSR in
	 * the FIP ToC and allowing the platform to have a say as
	 * well.
	 */
	spsr = SPSR_64(mode, MODE_SP_ELX, DISABLE_ALL_EXCEPTIONS);
	return spsr;
}

/*******************************************************************************
 * Returns BlueField specific memory map regions.
 ******************************************************************************/
const mmap_region_t *bluefield_get_mmap(void)
{
	return bluefield_mmap;
}

unsigned int plat_get_syscnt_freq2(void)
{
	unsigned int counter_base_frequency;

	/* Read the frequency from Frequency modes table */
	counter_base_frequency = mmio_read_32(SYS_CNTCTL_BASE + CNTFID_OFF);

	/* The first entry of the frequency modes table must not be 0 */
	if (counter_base_frequency == 0)
		panic();

	return counter_base_frequency;
}

/*******************************************************************************
 * This function flushes the data structures so that they are visible
 * in memory for the next BL image.
 ******************************************************************************/
void plat_flush_next_bl_params(void)
{
	flush_bl_params_desc();
}

/*******************************************************************************
 * This function returns the list of loadable images.
 ******************************************************************************/
bl_load_info_t *plat_get_bl_image_load_info(void)
{
	return get_bl_load_info_from_mem_params_desc();
}

/*******************************************************************************
 * This function returns the list of executable images.
 ******************************************************************************/
bl_params_t *plat_get_next_bl_params(void)
{
	bl_params_t *next_bl_params = get_next_bl_params_from_mem_params_desc();

	populate_next_bl_params_config(next_bl_params);
	return next_bl_params;
}
