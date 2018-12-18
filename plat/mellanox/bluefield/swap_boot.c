/*
 * Copyright (c) 2017, Mellanox Technologies Ltd. All rights reserved.
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
 * Neither the name of Mellanox nor the names of its contributors may be used
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
#include <debug.h>
#include <dw_mmc.h>
#include <emmc.h>
#include <mmio.h>
#include <platform.h>
#include <rsh.h>
#include <rsh_wd_control.h>
#include <swap_boot.h>

/*
 * Used with RSH_BOOT_CONTROL; bit 0 is not set, so we don't boot from
 * eMMC on reset, but software detects that bit 1 is set and interprets
 * it to mean we should swap the eMMC boot partitions and boot from eMMC.
 * TBD: consider just adding this value to the hardware XML definition.
 */
#define RSH_BOOT_CONTROL__BOOT_MODE_VAL_EMMC_ALT 0x2


/* Swap the eMMC boot partition. */
static void swap_boot_partition(void)
{
	static uint8_t desc[EMMC_BLOCK_SIZE]
		__attribute__((aligned(EMMC_BLOCK_SIZE)));
	dw_mmc_params_t params = {
		.reg_base = EMMC_BASE + EMMC_ADDR_OFFSET,
		.desc_base = (uintptr_t) &desc[0],
		.desc_size = sizeof(desc),
		.clk_rate = EMMC_CLK_RATE,
		.bus_width = EMMC_BUS_WIDTH_8,
		.flags = 0
	};
	int part;

	NOTICE("Swapping eMMC boot partitions...\n");
	dw_mmc_init(&params);

	part = emmc_get_boot_partition();
	switch (part) {
	case 1:
	case 2:
		part = 3 - part;   /* swap 1 to 2, or 2 to 1 */
		NOTICE("Swapped boot partitions; rebooting from partition %d\n", part);
		break;
	default:
		ERROR("Invalid existing partition %d; resetting to partition 1 and rebooting\n", part);
		part = 1;
		break;
	}
	emmc_set_boot_partition(part);
}

/* Handle alternate boot flow based on RSH_BOOT_CONTROL and RSH_BREADCRUMB0. */
void bluefield_handle_alternate_boot(void)
{
	/*
	 * If alternate eMMC boot mode is requested, then reboot using
	 * the alternate boot partition.
	 */
	RSH_BOOT_CONTROL_t ctl = {
		.word = mmio_read_64(RSHIM_BASE + RSH_BOOT_CONTROL) };
	if (ctl.boot_mode == RSH_BOOT_CONTROL__BOOT_MODE_VAL_EMMC_ALT) {
		RSH_BOOT_CONTROL_t boot_emmc =
			{ .boot_mode = RSH_BOOT_CONTROL__BOOT_MODE_VAL_EMMC };

		swap_boot_partition();

		mmio_write_64(RSHIM_BASE + RSH_BOOT_CONTROL, boot_emmc.word);
		mmio_write_64(RSHIM_BASE + RSH_RESET_CONTROL,
			      RSH_RESET_CONTROL__RESET_CHIP_VAL_KEY);

		/* Wait for reset to occur. */
		while (1)
			wfi();
	}

	/* Arm the watchdog if requested. */
	uint64_t breadcrumb = mmio_read_64(RSHIM_BASE + RSH_BREADCRUMB0);
	uint64_t watchdog = (breadcrumb & BREADCRUMB_WDOG_MASK);
	if (watchdog) {
		uint64_t wdog_ticks = watchdog * plat_get_syscnt_freq2();
		RSH_WD_CONTROL_WCS_t enable = { .en = 1 };

		/*
		 * The watchdog offset register is only 32 bits wide,
		 * so it can only support a fairly small number of
		 * seconds.  We support larger values by programming
		 * the WOR to ~1U so we have the maximum WS0->WS1
		 * delay, enabling the watchdog, then reprogramming
		 * the HI/LO comparison registers with the right value
		 * explicitly so that WS0 will fire when requested.
		 * Note that we have to set the HI word first.
		 */
		if (wdog_ticks == (uint32_t) wdog_ticks) {
			mmio_write_32(ARM_WDOG_BASE + RSH_WD_CONTROL_WOR,
				      (uint32_t) (wdog_ticks));
			mmio_write_32(ARM_WDOG_BASE + RSH_WD_CONTROL_WCS,
				      enable.word);
		} else {
			mmio_write_32(ARM_WDOG_BASE + RSH_WD_CONTROL_WOR, ~0);
			mmio_write_32(ARM_WDOG_BASE + RSH_WD_CONTROL_WCS,
				      enable.word);
			wdog_ticks += read_cntpct_el0();
			mmio_write_32(ARM_WDOG_BASE + RSH_WD_CONTROL_WCV_HI,
				      (uint32_t) (wdog_ticks >> 32));
			mmio_write_32(ARM_WDOG_BASE + RSH_WD_CONTROL_WCV_LO,
				      (uint32_t) (wdog_ticks));
		}
		NOTICE("Enabled watchdog (%lld sec delay)\n", watchdog);
	}

	/* Reset the boot mode for the next boot if requested. */
	if (breadcrumb & BREADCRUMB_SET_NEW_BOOT_MODE) {
		static const char
			boot_mode_string[BREADCRUMB_NEW_BOOT_MASK + 1][12] =
			{ "external", "emmc", "swap_emmc", "emmc_legacy" };
		int boot_mode = (breadcrumb >> BREADCRUMB_NEW_BOOT_SHIFT) &
			BREADCRUMB_NEW_BOOT_MASK;
		RSH_BOOT_CONTROL_t ctl = { .boot_mode = boot_mode };
		mmio_write_64(RSHIM_BASE + RSH_BOOT_CONTROL, ctl.word);
		NOTICE("Next boot will be in %s mode\n",
		       boot_mode_string[boot_mode]);
	}

	/* Clear the bits we use from the breadcrumb register. */
	mmio_write_64(RSHIM_BASE + RSH_BREADCRUMB0,
		      breadcrumb & ~BREADCRUMB_ALL_BITS);
}
