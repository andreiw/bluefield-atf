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

#include <arch_helpers.h>
#include <cassert.h>
#include <bluefield_svc.h>
#include <debug.h>
#include <mmc_def.h>
#include <mmio.h>
#include <platform_def.h>
#include <runtime_svc.h>
#include <spinlock.h>
#include <swap_boot.h>
#include <rsh.h>
#include <tyu_def.h>
#include <uuid.h>

#include "bluefield_efi_info.h"
#include "bluefield_platform.h"

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

static uintptr_t set_emmc_rst_n(void *handle)
{
	uint64_t spsr_el3 = read_spsr_el3();

	/*
	 * spsr_el3[3:2] contains the exception level that was taken from.
	 * Ignore exception levels except level 2 (UEFI).
	 */
	if (GET_EL(spsr_el3) != 2)
		SMC_RET1(handle, SMCCC_INVALID_PARAMETERS);

	mmio_write_32(EMMC_BASE + MMC_RST_N, 0);
	SMC_RET1(handle, 0);
}

/* Format for the SMC response about the memory information */
union dimm_info_smc {
	struct {
		uint64_t size_GB : 16;		/* Dimm size in GB */
		uint64_t is_rdimm : 1;		/* If is RDIMM/LRDIMM */
		uint64_t is_lrdimm : 1;		/* If is LRDIMM */
		uint64_t is_nvdimm : 1;		/* If is NVDIMM */
		uint64_t __reserved0 : 2;
		uint64_t ranks : 3;		/* Rank num of DIMM */
		uint64_t package_X : 8;		/* Bits per memory package */
		uint64_t __reserved1 : 32;
	};
	uint64_t val;
};

static uintptr_t get_tbb_dimm_info(void *handle, u_register_t mss_dimm)
{
	int mss, dimm, dimm_idx;
	union dimm_info_smc dimm_info;
	struct bf_efi *ei = (struct bf_efi *)NS_IMAGE_OFFSET;

	mss = mss_dimm >> 16;
	dimm = mss_dimm & 0xffff;
	dimm_idx = mss * MAX_DIMM_PER_MEM_CTRL + dimm;
	dimm_info.val = 0;

	for (int i = 0; i < MAX_DIMM_NUM; i++) {
		if (ei->region[i].dimm != dimm_idx)
			continue;
		if (ei->region[i].length == 0)
			continue;

		dimm_info.size_GB = ei->region[i].length >> 30;
		dimm_info.is_rdimm = ei->region[i].is_registered;
		dimm_info.is_lrdimm = ei->region[i].is_load_reduced;
		dimm_info.is_nvdimm = ei->region[i].is_nvdimm;
		dimm_info.ranks = ei->region[i].ranks;
		dimm_info.package_X = 1 << ei->region[i].data_bits;
	}

	SMC_RET1(handle, dimm_info.val);
}

/* Parameter values for protected_read/write functions */
#define PROT_64 0
#define PROT_32 1

/*
 * Verifies whether addr is within any of the reg_blocks in table.
 * Returns 1 if the verification was successful, 0 if not.
 */
static int verify_reg(uint64_t addr, struct reg_block *table) {
	struct reg_block *idx = table;

	while (idx->start != 0) {
		if (addr >= idx->start && addr <= idx->end) {
			return 1;
		}

		idx++;
	}

	return 0; // got to end with no matches, invalid addr
}

static uintptr_t protected_write(
		void *handle,
		u_register_t tablenum,
		u_register_t value,
		u_register_t address,
		int size)
{
	if (tablenum > NUM_REG_TABLES) {
		SMC_RET1(handle, SMCCC_OUT_OF_RANGE);
	}

	struct reg_block *reg_table = register_tables[tablenum];

	if (verify_reg((uint64_t) address, reg_table)) {
		// do size dependent write
		switch (size) {
		case PROT_32:
			mmio_write_32(address, (uint32_t) value);
			break;
		case PROT_64:
		default:
			mmio_write_64(address, value);
			break;
		}

		SMC_RET1(handle, 0);
	} else {
		// bad access, return error
		SMC_RET1(handle, SMCCC_ACCESS_VIOLATION);
	}
}

static uintptr_t protected_read(
		void *handle,
		u_register_t tablenum,
		u_register_t address,
		int size)
{
	if (tablenum > NUM_REG_TABLES) {
		SMC_RET2(handle, SMCCC_OUT_OF_RANGE, 0x0);
	}

	struct reg_block *reg_table = register_tables[tablenum];

	if (verify_reg((uint64_t) address, reg_table)) {
		uint64_t result;

		switch (size) {
		case PROT_32:
			result = (uint64_t)mmio_read_32(address);
			break;
		case PROT_64:
		default:
			result = mmio_read_64(address);
			break;
		}

		SMC_RET2(handle, 0, result);
	} else {
		SMC_RET2(handle, SMCCC_ACCESS_VIOLATION, 0x0);
	}
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

	case MLNX_WRITE_REG_32:
		return protected_write(handle, x1, x2, x3, PROT_32);

	case MLNX_READ_REG_32:
		return protected_read(handle, x1, x2, PROT_32);

	case MLNX_WRITE_REG_64:
		return protected_write(handle, x1, x2, x3, PROT_64);

	case MLNX_READ_REG_64:
		return protected_read(handle, x1, x2, PROT_64);

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

	case MLNX_SET_EMMC_RST_N:
		return set_emmc_rst_n(handle);

	case MLNX_GET_DIMM_INFO:
		return get_tbb_dimm_info(handle, x1);

	default:
		return bluefield_plat_smc_handler(smc_fid, x1, x2, x3, x4,
						  cookie, handle, flags);
	}
}

/* Do nothing, but keep this stub in case we need to add operations. */
int32_t bluefield_smc_init(void)
{
	return 0;
}

/* Define a runtime service descriptor for fast SMC calls */
DECLARE_RT_SVC(
	bluefield_svc,
	OEN_SIP_START,
	OEN_SIP_END,
	SMC_TYPE_FAST,
	bluefield_smc_init,
	bluefield_smc_handler
);
