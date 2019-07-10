/*
 * Copyright (c) 2019, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <mmio.h>
#include "bluefield_common.h"
#include "bluefield_private.h"
#include "rsh.h"
#include "tyu_def.h"

static void bluefield2_access_setup(void)
{
	RSH_DEVICE_MSTR_PRIV_LVL_t priv_lvl = {
		.dev_lvl_diag_uart =
			RSH_DEVICE_MSTR_PRIV_LVL__DEV_LVL_DIAG_UART_VAL_HIGH,
		.dev_lvl_mmc =
			RSH_DEVICE_MSTR_PRIV_LVL__DEV_LVL_MMC_VAL_HIGH,
		.rci_lvl_tp1 =
			RSH_DEVICE_MSTR_PRIV_LVL__RCI_LVL_TP1_VAL_HIGH,
		.rci_lvl_tp0 =
			RSH_DEVICE_MSTR_PRIV_LVL__RCI_LVL_TP0_VAL_HIGH,
		.rci_lvl_hca =
			RSH_DEVICE_MSTR_PRIV_LVL__RCI_LVL_HCA_VAL_HIGH,
		.rci_lvl_usb =
			RSH_DEVICE_MSTR_PRIV_LVL__RCI_LVL_USB_VAL_HIGH,
		.mem_acc_lvl =
			RSH_DEVICE_MSTR_PRIV_LVL__MEM_ACC_LVL_RESET_VAL,
		.oob_ns_bit =
			RSH_DEVICE_MSTR_PRIV_LVL__OOB_NS_BIT_RESET_VAL
	};

	/*
	 * Default values got from the non-secure booting, which is used
	 * as initial configuration for secure booting as well.
	 */
	mmio_write_64(RSHIM_BASE + RSH_DEVICE_DISABLE, 0);
	mmio_write_64(RSHIM_BASE + RSH_DEVICE_MSTR_PRIV_LVL, priv_lvl.word);
	mmio_write_64(RSHIM_BASE + RSH_DEVICE_SLV_SEC_LVL, 0);
}

void bl1_early_platform_setup(void)
{
	/* Initialise access permissions. */
	bluefield2_access_setup();

	bluefield_bl1_early_platform_setup();
}
