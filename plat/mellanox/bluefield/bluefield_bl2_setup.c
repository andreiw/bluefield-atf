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

#include <arch_helpers.h>
#include <bl_common.h>
#include <console.h>
#include <debug.h>
#include <desc_image_load.h>
#include <i2c_smbus.h>
#include <mmio.h>
#include <platform_def.h>
#include <string.h>
#include "bluefield_def.h"
#include "bluefield_private.h"
#include "bluefield_system.h"
#include "bluefield_ddr.h"
#include "rsh.h"
#include "tyu_def.h"
#if !defined(ALT_BL2) && TRUSTED_BOARD_BOOT
#include <bluefield_auth_mod.h>
#include <platform.h>
#endif

#ifdef ALT_BL2
extern void bluefield_mod_fuses(void);
#endif

#ifdef ATF_CONSOLE
extern void bluefield_console(void);
#else
static inline void bluefield_console(void) {}
#endif

uint8_t boot_on_spi_flash_recovery;

/*
 * The next 2 constants identify the extents of the coherent memory region.
 * These addresses are used by the MMU setup code and therefore they must be
 * page-aligned.  It is the responsibility of the linker script to ensure that
 * __COHERENT_RAM_START__ and __COHERENT_RAM_END__ linker symbols refer to
 * page-aligned addresses.
 */
#define BL2_COHERENT_RAM_BASE (unsigned long)(&__COHERENT_RAM_START__)
#define BL2_COHERENT_RAM_LIMIT (unsigned long)(&__COHERENT_RAM_END__)

/* Data structure which holds the extents of the trusted SRAM for BL2 */
static meminfo_t bl2_tzram_layout __aligned(CACHE_WRITEBACK_GRANULE);
/* Data structure which describes the memory layout. */
struct bf_mem_info bf_memory_layout;

/*******************************************************************************
 * BL1 has passed the extents of the trusted SRAM that should be visible to BL2
 * in x0. This memory layout is sitting at the base of the free trusted SRAM.
 * Copy it to a safe location before its reclaimed by later BL2 functionality.
 ******************************************************************************/
void bl2_early_platform_setup(meminfo_t *mem_layout)
{
	/* Initialize the console to provide early debug support */
	bluefield_console_init();

	/* Setup the BL2 memory layout */
	bl2_tzram_layout = *mem_layout;

	/* Initialize the platform config for future decision making */
	bf_sys_config_setup();
	/* Initialize the IO layer and register platform IO devices */
	bluefield_io_setup();
}

/*
 * The efuse spec requires that when programming a fuse bit, the strobe signal
 * will be asserted to 12usec +/-1usec. The fuse wrapper has a crspace register
 * which should be updated with how many i1clk cycles the strobe should be
 * asserted prior to executing a fuse burn command. We are programming this
 * register here so that if somebody ever decides to program the fuses elsewhere
 * the hardware still works accordingly.
 */
static void bluefield_pgm_strobe(void)
{
	uint32_t yu_core_pll_cfg;
	uint32_t core_f, core_r, core_od;
	uint32_t strobe;

	yu_core_pll_cfg = mmio_read_32(TYU_BASE_ADDRESS + TYU_YU_CORE_PLL_CFG);
	core_f = GET_CORE_PLL_FIELD(CORE_F, yu_core_pll_cfg);
	core_r = GET_CORE_PLL_FIELD(CORE_R, yu_core_pll_cfg);
	core_od = GET_CORE_PLL_FIELD(CORE_OD, yu_core_pll_cfg);

	strobe = 12000000000000ull * (uint64_t)((core_r + 1) * (core_od + 1)) /
		(BF_REF_CLK_IN_HZ * (uint64_t)(core_f + 1));

	mmio_write_32(TYU_BASE_ADDRESS + TYU_PG_STROBE_LENGTH, strobe);
}

#ifndef ALT_BL2
static void bluefield_get_dev_tbl(struct bf_dev_tbl **dev_tbl,
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

/*
 * In LiveFish mode, the PLL clocks are not properly set up, instead the
 * osc_strap_en bit is turned on, which protects the PLL by making them
 * work in bypass mode. In order to use DRAM, we need to control the PLL
 * to set the correct frequency, thus need to turn this bit back off.
 * But this bit not only controls the PLL used for the DRAM, but also
 * controls the PLLs used in other parts of the system, so in order for
 * the PLL to not crash, we explicitly turn every PLL to bypass mode.
 * Then when the corresponding PLL needs to be configured, we then configure
 * the PLL and at the end turn off bypass for it.
 */
static void bluefield_livefish_pll_setup(void)
{
	uint32_t osc_strap_wd = mmio_read_32(TYU_BASE_ADDRESS +
					     TYU_OSC_STRAP_EN_ADDR);

	/*
	 * We only need to do this operation after the HCA portion of the SoC
	 * have done a reset when in LiveFish mode as the values are persistent
	 * after an ARM side soft reset.
	 */
	if (GET_REG_FIELD(TYU_OSC_STRAP_EN, osc_strap_wd) == 0)
		return;

	NOTICE("Setting up core PLL in bypass first time in LiveFish mode\n");

	/* Set all PLL cores in bypass mode. */
	for (int i = 0; i < 9; i++) {
		uint32_t core_pll = mmio_read_32(TYU_BASE_ADDRESS +
						 TYU_CORE_PLL_CFG(i));
		core_pll = SET_CORE_PLL_FIELD(CORE_BYPASS, core_pll, 1);
		mmio_write_32(TYU_BASE_ADDRESS + TYU_CORE_PLL_CFG(i), core_pll);
	}

	osc_strap_wd = SET_REG_FIELD(TYU_OSC_STRAP_EN, osc_strap_wd, 0);
	mmio_write_32(TYU_BASE_ADDRESS + TYU_OSC_STRAP_EN_ADDR, osc_strap_wd);
}

#if TRUSTED_BOARD_BOOT
/*
 * This function authenticates the HCA firmware.
 *
 * Note that a base requirement is that the platform firmware shall
 * authenticate the NIC firmware before enabling the PCIe interface to
 * exchange data. This must not violate the PCIe timings; the whole
 * process has to conclude within 1 second.
 * This function also handles the special signal 'Force_CRS' accordingly.
 * When the signal is set, it blocks the ability of the PCIe interface to
 * exchange data over the PCIe link, including blocking of responses to
 * configuration cycles.
 */
#pragma weak bluefield_auth_hca_firmware
void bluefield_auth_hca_firmware(void)
{
	uint32_t tyu_fcrs;
	int      rc = 0;

	/*
	 * If we're in secure boot mode, then we will be responsible for
	 * authenticating the HCA firmware. Until that happens, PCIe access
	 * from or to the HCA is disabled by the FORCE_CRS configuration bit,
	 * which is always set at hard reset time, but only has an effect if
	 * secure boot is enabled.
	 * FORCE_CRS is only enabled by hard reset, so if it's already been
	 * turned off by a previous boot, we can skip these checks.
	 * That said, this code should only verify the NIC FW following
	 * full-chip reset.
	 *
	 * Also set the MDK visiblility bit when the NIC FW has been already
	 * verified. User code shouldn't be able to access MDK until the next
	 * cold reset to the chip.
	 *
	 */

	if (!plat_enable_tbb())
		return;

	tyu_fcrs = mmio_read_32(TYU_BASE_ADDRESS + TYU_FORCE_CRS_ADDR);
	if (GET_REG_FIELD(TYU_FORCE_CRS, tyu_fcrs)) {
		bluefield_auth_mod_init();
		/* Initiate the FW verification */
		rc = bluefield_auth_mod_verify();
	}

	/*
	 * If the authentication succeeded, then enable the NIC access to
	 * the PCIe fabric by clearing force_crs bit in TYU. This bit is
	 * also set only following full-chip reset.
	 */

	if (rc != 0) {
		/* If we reach here, then system recovery is needed. */
		boot_on_spi_flash_recovery = 1;
		ERROR("ConnectX firmware authentication failed rc:%d\n", rc);
		return;
	}

	/*
	 * Enable the PCIe interface on the NIC side immediately.
	 * When a device is in Secure mode, the default state of
	 * 'Force_CRS' is set to 1. So, deassert Force_CRS to let
	 * NIC have access to PCIe. This also enables the access
	 * to CRSPACE on the NIC, when non-secure FW is running into
	 * the NIC.
	 */
	tyu_fcrs = SET_REG_FIELD(TYU_FORCE_CRS, tyu_fcrs, 0);
	mmio_write_32(TYU_BASE_ADDRESS + TYU_FORCE_CRS_ADDR, tyu_fcrs);

	/* Make the MDK visible to the NIC firmware. */
	mmio_write_32(TYU_BASE_ADDRESS + MDK_FUSE_FW_ACCESS_EN, 1);
}
#endif

/* Configure memory and pass on information about the memory configuration */
void bluefield_setup_memory(struct bf_dev_tbl *dev_tbl,
			    uint32_t disabled_devs,
			    struct bf_mem_info *bmi)
{
	int mem_ctrl_num = 0;
	int have_mem = 0;

	for (int idx = 0; dev_tbl[idx].base_addr != ~0ULL; idx++) {
		if (dev_tbl[idx].dev_type != DEV_MSS)
			continue;
		if ((disabled_devs >> idx) & 1) {
			mem_ctrl_num++;
			continue;
		}
		assert(mem_ctrl_num < MAX_MEM_CTRL);
		have_mem |= bf_sys_mem_config(dev_tbl[idx].base_addr,
				  &(bmi->mem_ctrl_info[mem_ctrl_num]),
				  mem_ctrl_num);
		mem_ctrl_num++;
	}

# ifndef ATF_CONSOLE
	/*
	 * If no memory was brought up, try a second time with the lowest
	 * frequency and all possible setups. It would be better to have
	 * low speed working memory rather than no memory at all.
	 */
	if (!have_mem) {
		mem_ctrl_num = 0;

		for (int idx = 0; dev_tbl[idx].base_addr != ~0ULL; idx++) {
			if (dev_tbl[idx].dev_type != DEV_MSS)
				continue;
			if ((disabled_devs >> idx) & 1) {
				mem_ctrl_num++;
				continue;
			}
			assert(mem_ctrl_num < MAX_MEM_CTRL);
			have_mem |= bf_sys_mem_try_cfg(dev_tbl[idx].base_addr,
					  &(bmi->mem_ctrl_info[mem_ctrl_num]),
					  mem_ctrl_num);
			mem_ctrl_num++;
		}
	}
# endif /* ATF_CONSOLE */

	/* The test should panic if it fails and we don't have the console. */
	bf_sys_post_test(bmi);
}
#endif

void bl2_platform_setup(void)
{
	NOTICE("BL2 built for %s\n", TARGET_SYSTEM);

	bluefield_pgm_strobe();

#ifndef ALT_BL2
	struct bf_dev_tbl *bdt = NULL;
	uint32_t disabled_devs = 0;

	bluefield_get_dev_tbl(&bdt, &disabled_devs);

	bf_sys_setup_sam(bdt, disabled_devs);

#if TRUSTED_BOARD_BOOT
	if (!plat_enable_tbb())
		dyn_disable_auth();

	bluefield_auth_hca_firmware();
#endif

	memset(&bf_memory_layout, 0, sizeof(bf_memory_layout));

	bluefield_delay_timer_init();

	i2c_smbus_initialize();

	bluefield_livefish_pll_setup();

	bf_sys_setup_interrupts(bdt, disabled_devs);

	bluefield_setup_memory(bdt, disabled_devs, &bf_memory_layout);

	bluefield_console();

	bf_sys_setup_pmr(bdt, disabled_devs, &bf_memory_layout, 0);

	bf_sys_setup_hnf_errata(bdt, disabled_devs);

	bf_sys_setup_trio(bdt, disabled_devs);

	bluefield_setup_nvdimm_restore();
#else /* ALT_BL2 */
	bluefield_mod_fuses();
#endif /* ALT_BL2 */
}

/*******************************************************************************
 * Perform the very early platform specific architectural setup here. At the
 * moment this is only initializes the mmu in a quick and dirty way.
 ******************************************************************************/
void bl2_plat_arch_setup(void)
{
	bluefield_setup_page_tables(bl2_tzram_layout.total_base,
				    bl2_tzram_layout.total_size,
				    BL_CODE_BASE,
				    BL_CODE_END,
				    BL_RO_DATA_BASE,
				    BL_RO_DATA_END,
				    BL2_COHERENT_RAM_BASE,
				    BL2_COHERENT_RAM_LIMIT
			      );

	enable_mmu_el1(0);
}

/*******************************************************************************
 * This function can be used by the platforms to update/use image
 * information for given `image_id`.
 ******************************************************************************/
int bl2_plat_handle_post_image_load(unsigned int image_id)
{
	int err = 0;
	bl_mem_params_node_t *bl_mem_params = get_bl_mem_params_node(image_id);
	assert(bl_mem_params);

	switch (image_id) {
	case BL33_IMAGE_ID:
		/* BL33 expects to receive the primary CPU MPID (through r0) */
		bl_mem_params->ep_info.args.arg0 = 0xffff & read_mpidr();
		bl_mem_params->ep_info.spsr = bluefield_get_spsr_for_bl33_entry();
#ifndef ALT_BL2
		/* Setup information for UEFI. */
		uintptr_t image_base = bl_mem_params->image_info.image_base;
		if (image_base)
			bluefield_setup_efi_info(image_base);
#endif
		break;
	default:
		/* Do nothing in default case */
		break;
	}

	return err;
}
