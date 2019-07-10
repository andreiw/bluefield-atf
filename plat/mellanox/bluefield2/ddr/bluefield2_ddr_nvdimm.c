/*
 * Copyright (c) 2019, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <delay_timer.h>
#include <mmio.h>
#include <stdint.h>
#include <stdlib.h>

#include "bluefield_ddr.h"
#include "bluefield_def.h"
#include "bluefield_platform.h"
#include "bluefield_private.h"
#include "bluefield_system.h"
#include "rsh.h"

void bluefield_nvdimm_save_cstate(int mss_idx)
{
	RSH_SW_ADR_SEQ_MSS_t mss;

	if (mss_idx != 0) {
		ERROR("MSS%d not present in system\n", mss_idx);
		return;
	}

	/* Step-1: flush the cache. */
	mss = (RSH_SW_ADR_SEQ_MSS_t) {
		.save_n = 1,
		.flush_l3_emi = 1,
		.bkp_memc = 0
	};
	mmio_write_64(RSHIM_BASE + RSH_SW_ADR_SEQ_MSS__FIRST_WORD, mss.word);

	/* Check completion. */
	RSH_PWR_WDOG_STATUS_CHECK(l3_emi_flush_done,
				  NVDIMM_MAX_SAVE_DELAY_MS,
				  "NVDIMM: failed to flush cache.\n");

	/* Step-2: put DDR in Self-Refresh mode. */
	mss.flush_l3_emi = 0;
	mss.bkp_memc = 1;
	mmio_write_64(RSHIM_BASE + RSH_SW_ADR_SEQ_MSS__FIRST_WORD, mss.word);

	/* Check completion. */
	RSH_PWR_WDOG_STATUS_CHECK(memc_bkp_done,
			NVDIMM_MAX_SAVE_DELAY_MS,
			"NVDIMM: failed to set self-refresh.\n");

	/* Step-3: start SAVE_N (low trigger). */
	mss.bkp_memc = 0;
	mss.save_n = 0;
	mmio_write_64(RSHIM_BASE + RSH_SW_ADR_SEQ_MSS__FIRST_WORD, mss.word);
}
