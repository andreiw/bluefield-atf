/*
 * Copyright (c) 2017, Mellanox Technologies, Ltd. All rights reserved.
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

#include <bluefield_svc.h>
#include <mmio.h>
#include <rsh.h>
#include <runtime_svc.h>
#include <spinlock.h>
#include <swap_boot.h>
#include <uuid.h>

static spinlock_t breadcrumb_lock;

/* BlueField Service UUID */
DEFINE_SVC_UUID2(bf_svc_uid,
		 0xb436c089, 0xd7e7, 0xe611, 0x87, 0x97,
		 0x00, 0x1a, 0xca, 0x00, 0xbf, 0xc4);

static uintptr_t set_post_reset_wdog(void *handle, u_register_t interval)
{
	uint64_t breadcrumb;

	if (interval > BREADCRUMB_WDOG_MASK)
		SMC_RET1(handle, SMCCC_INVALID_PARAMETERS);

	spin_lock(&breadcrumb_lock);
	breadcrumb = mmio_read_64(RSHIM_BASE + RSH_BREADCRUMB0);
	breadcrumb &= ~BREADCRUMB_WDOG_MASK;
	breadcrumb |= interval;
	mmio_write_64(RSHIM_BASE + RSH_BREADCRUMB0, breadcrumb);
	spin_unlock(&breadcrumb_lock);

	SMC_RET1(handle, 0);
}

static uintptr_t set_reset_action(void *handle, u_register_t reset_action)
{
	RSH_BOOT_CONTROL_t ctl;

	switch (reset_action) {
	case MLNX_BOOT_EXTERNAL:
	case MLNX_BOOT_EMMC:
	case MLNX_BOOT_SWAP_EMMC:
	case MLNX_BOOT_EMMC_LEGACY:
		break;
	default:
		SMC_RET1(handle, SMCCC_INVALID_PARAMETERS);
	}

	ctl = (RSH_BOOT_CONTROL_t) { .boot_mode = reset_action };
	mmio_write_64(RSHIM_BASE + RSH_BOOT_CONTROL, ctl.word);

	SMC_RET1(handle, 0);
}

static uintptr_t set_second_reset_action(void *handle,
					 u_register_t reset_action)
{
	uint64_t breadcrumb;

	switch (reset_action) {
	case MLNX_BOOT_EXTERNAL:
	case MLNX_BOOT_EMMC:
	case MLNX_BOOT_SWAP_EMMC:
	case MLNX_BOOT_EMMC_LEGACY:
	case MLNX_BOOT_NONE:
		break;
	default:
		SMC_RET1(handle, SMCCC_INVALID_PARAMETERS);
	}

	spin_lock(&breadcrumb_lock);
	breadcrumb = mmio_read_64(RSHIM_BASE + RSH_BREADCRUMB0);
	breadcrumb &= ~(BREADCRUMB_NEW_BOOT_MASK << BREADCRUMB_NEW_BOOT_SHIFT);

	if (reset_action == MLNX_BOOT_NONE)
		breadcrumb &= ~BREADCRUMB_SET_NEW_BOOT_MODE;
	else
		breadcrumb |= BREADCRUMB_SET_NEW_BOOT_MODE |
			(reset_action << BREADCRUMB_NEW_BOOT_SHIFT);

	mmio_write_64(RSHIM_BASE + RSH_BREADCRUMB0, breadcrumb);
	spin_unlock(&breadcrumb_lock);

	SMC_RET1(handle, 0);
}

static uintptr_t get_second_reset_action(void *handle)
{
	uint64_t breadcrumb = mmio_read_64(RSHIM_BASE + RSH_BREADCRUMB0);
	u_register_t result;

	if (breadcrumb & BREADCRUMB_SET_NEW_BOOT_MODE)
		result = (breadcrumb >> BREADCRUMB_NEW_BOOT_SHIFT) &
			BREADCRUMB_NEW_BOOT_MASK;
	else
		result = MLNX_BOOT_NONE;

	SMC_RET1(handle, result);
}

static uintptr_t get_tbb_fuse_status(void *handle, u_register_t fuse_status)
{
	RSH_SB_KEY_VLD_t key_vld;
	uint64_t sb_mode;
	u_register_t result;

	switch (fuse_status) {

	case MLNX_FUSE_STATUS_LIFECYCLE:

		sb_mode = mmio_read_64(RSHIM_BASE + RSH_SB_MODE);

		/*
		 * Setting the test field of a non secure chip and soft
		 * resetting the Arm cores would also make the chip behave as
		 * if it is in secure state. So also return this field so that
		 * the caller can be aware of this.
		 */
		result = sb_mode & (RSH_SB_MODE__SECURE_MASK |
				    RSH_SB_MODE__TEST_MASK);
		break;

	case MLNX_FUSE_STATUS_KEYS:

		key_vld.word = mmio_read_64(RSHIM_BASE + RSH_SB_KEY_VLD);

		result = key_vld.BF_KEY_VLD;
		break;

	default:
		result = SMCCC_INVALID_PARAMETERS;
	}

	SMC_RET1(handle, result);
}

static uintptr_t bluefield_smc_handler(uint32_t smc_fid, u_register_t x1,
				       u_register_t x2, u_register_t x3,
				       u_register_t x4, void *cookie,
				       void *handle, u_register_t flags)
{
	switch (smc_fid) {

	case MLNX_SET_POST_RESET_WDOG:
		return set_post_reset_wdog(handle, x1);

	case MLNX_GET_POST_RESET_WDOG:
		SMC_RET1(handle,
			 (mmio_read_64(RSHIM_BASE + RSH_BREADCRUMB0) &
			  BREADCRUMB_WDOG_MASK));

	case MLNX_SET_RESET_ACTION:
		return set_reset_action(handle, x1);

	case MLNX_GET_RESET_ACTION:
		SMC_RET1(handle,
			 ((mmio_read_64(RSHIM_BASE + RSH_BOOT_CONTROL) >>
			   RSH_BOOT_CONTROL__BOOT_MODE_SHIFT) &
			  RSH_BOOT_CONTROL__BOOT_MODE_MASK));

	case MLNX_SET_SECOND_RESET_ACTION:
		return set_second_reset_action(handle, x1);

	case MLNX_GET_SECOND_RESET_ACTION:
		return get_second_reset_action(handle);

	case MLNX_GET_TBB_FUSE_STATUS:
		return get_tbb_fuse_status(handle, x1);

	case MLNX_SIP_SVC_CALL_COUNT:
		/* Return the number of Mellanox SiP Service Calls */
		SMC_RET1(handle, MLNX_NUM_SVC_CALLS);

	case MLNX_SIP_SVC_UID:
		/* Return UID to the caller */
		SMC_UUID_RET(handle, bf_svc_uid);

	case MLNX_SIP_SVC_VERSION:
		/* Return the version of current implementation */
		SMC_RET2(handle, MLNX_SVC_VERSION_MAJOR,
			 MLNX_SVC_VERSION_MINOR);

	default:
		SMC_RET1(handle, SMC_UNK);
	}
}

/* Define a runtime service descriptor for fast SMC calls */
DECLARE_RT_SVC(
	bluefield_svc,
	OEN_SIP_START,
	OEN_SIP_END,
	SMC_TYPE_FAST,
	NULL,
	bluefield_smc_handler
);
