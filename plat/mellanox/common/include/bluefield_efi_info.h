/*
 * Copyright (c) 2016-2018, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __BLUEFIELD_EFI_INFO_H__
#define __BLUEFIELD_EFI_INFO_H__

#include "bluefield_def.h"

#define MAX_DIMM_NUM			(MAX_MEM_CTRL * MAX_DIMM_PER_MEM_CTRL)
#define MAX_NVDIMM_NUM			(MAX_DIMM_NUM - 1)
#define EFI_INFO_SIZE			0x1000
#define NVDIMM_ARS_REC_NUM		32
#define NVDIMM_ARS_CACHE_LINE_SIZE	64

/*
 * Set the block size to be 64 cache lines for two reasons.
 * - It's 4K bytes (a page).
 * - If needed, the status of the cache lines could be saved in a uint64_t
 *   bitmask.
 * However, this size could be adjusted according to the ARS performance on
 * real HW.
 */
#define NVDIMM_ARS_BLOCK_SIZE		(NVDIMM_ARS_CACHE_LINE_SIZE * 64)

/*
 * Used to set the current value for the down counter. Decremented by HW
 * based on ref-clock setup in control register (RSH_DOWN_COUNT_CONTROL).
 * When counter is 0 and timer tick occurs, an interrupt is signaled.
 */
#define NVDIMM_ARS_COUNT_DOWN		4096

/* Structure for type BF_EFI_INFO_TYPE_MEM_REGION. */
struct bf_efi_mem_region {
	/* CPU socket ID. Set to 0 if no appilcable. */
	uint32_t socket_id : 4;

	/* Memory controller ID within the socket. */
	uint32_t memory_controller_id : 4;

	/* Memory channel number within the memory controller.
	 * Set to 1 if there is only one channel per controller.
	 */
	uint32_t memory_channel_number : 4;

	/* A flag to indicate NVDIMM or not. */
	uint32_t is_nvdimm : 1;

	/* DIMM Identifier DIMM starting from 0. */
	uint32_t dimm : 4;

	/* Number of ranks this DIMM have */
	uint32_t ranks : 3;

	/* Power of 2 number of data bits (e.g. x4, x8) */
	uint32_t data_bits : 4;

	/* Type of DIMM (RDIMM/LRDIMM) */
	uint32_t is_registered : 1;
	uint32_t is_load_reduced : 1;

	/* Physical address. */
	uint64_t phy_addr;

	/* Total length. */
	uint64_t length;
};

/* Structure for type BF_EFI_TYPE_NVDIMM. */
struct bf_efi_nvdimm {
	/* Data from SPD. */
	uint8_t vendor_id[2];
	uint8_t device_id[2];
	uint8_t revision_id[2];
	uint8_t subsystem_vendor_id[2];
	uint8_t subsystem_device_id[2];
	uint8_t subsystem_revision_id[2];
	uint8_t manufacturing_location[1];
	uint8_t manufacturing_data[2];
	uint8_t serial_number[4];
	uint8_t region_format_interface_code[2];

	/* DIMM ID starting from 0. */
	uint8_t dimm;
};

/* ARS functions. */
enum {
	ARS_FUNC_QUERY_CAP = 1,		/* Query ARS Capabilities */
	ARS_FUNC_START_ARS = 2,		/* Start ARS */
	ARS_FUNC_QUERY_STATUS = 3,	/* Query ARS Status */
	ARS_FUNC_CLEAR_ERROR = 4,	/* Clear Uncorrectable Error */
	ARS_FUNC_TRANS_SPA = 5		/* Translate SPA */
};

/* ARS extended status (ACPI 6.2 Table 9-302 Query ARS Status). */
enum {
	ARS_EXT_STATUS_COMPLETE = 0,
	ARS_EXT_STATUS_INPROGRESS = 1,
	ARS_EXT_STATUS_NO_ARS = 2,
	ARS_EXT_STATUS_PRE_STOP = 3	/* Stopped Prematurely. */
};

/*
 * ARS (address range scrub) structure.
 * It is defined within the bf_efi strcuture, which is 4K bytes
 * in total currently. This structure will start at offset
 * NVDIMM_ARS_OFF (2048). For now we limit the size of this structure
 * to 1K byte, and save the last 1K bytes of the bf_efi structure for
 * other uses.
 *
 * Note: This structure is also referenced by ACPI ARS method. Need to
 *       make them consistent when changing data structure.
 */
struct bf_ars {
        uint8_t func;		/* ARS function (byte 0). */
	uint8_t flags;		/* ARS flags (byte 1). */
	uint8_t serror;		/* SError flag (byte 2). */
	int8_t handle;		/* NVDIMM NFIT handle. */
        uint8_t unuse1[4];	/* Unused. */
	uint64_t start_pa;	/* ARS start address (PA). */
	uint64_t start_len;	/* ARS start length. */
	uint64_t unuse2[5];	/* Unused. */

	union {
		/* Query ARS Status output (ACPI 6.2 Table 9-303). */
		struct {
			uint16_t status;
			uint16_t ext_status;
			uint32_t size;
			uint64_t start_pa;
			uint64_t start_len;
			uint64_t restart_pa;
			uint64_t restart_len;
			uint16_t type;
			uint16_t flags;
			uint32_t num;
			struct ars_err_rec {
				uint32_t handle;
				uint32_t reserved;
				uint64_t pa;
				uint64_t len;
			} rec[NVDIMM_ARS_REC_NUM];
		} __attribute__((packed)) query;

		/*  Clear Uncorrectable Error (ACPI 6.2 Table 9-305). */
		struct {
			uint16_t status;
			uint16_t ext_status;
			uint32_t reserved;
			uint64_t len;		/* Error Range Length */
		} __attribute__((packed)) clear;

		/* Translate SPA (ACPI 6.2 Table 9-308). */
		struct {
			uint16_t status;
			uint16_t ext_status;
			uint8_t flags;
			uint8_t reserved[3];
			uint64_t len;		/* Translated Length */
			uint32_t num;		/* Number of NVDIMMs */
			struct {		/* Device List */
				uint32_t handle;/* NFIT Device Handle */
				uint32_t rsvd;	/* Reserved */
				uint64_t dpa;	/* DIMM Physical Address */
			} device[1];
		} __attribute__((packed)) translate;
	} output;
};

/*
 * Structure passed to UEFI. It contains sub structs to describe the
 * memory we have as well as any particular NVDIMM info that we want
 * to pass on. The total size of this structure is 4K bytes.
 */
struct bf_efi {
	uint32_t reserved;	/* Reserved field. Don't change it */
	uint8_t magic[4];	/* Magic Number */

	/*
	 * DIMM / region information.
	 */
	uint8_t region_num;	/* Number of valid bf_efi_mem_region structs */
	uint8_t nvdimm_num;	/* Number of valud bf_efi_nvdimm structs */

	uint16_t sec_emmc : 1;	/* Flag to enable second eMMC */
	uint16_t ext_boot : 1;	/* Flag to indicate external boot */
	uint16_t tmfifo_init : 1; /* Tmfifo init flag. Used solely in UEFI. */
	uint16_t bf_is_pal : 1; /* Flag to indicate running on Palladium. */
	uint16_t snic_model : 2; /* Flag to indicate SmartNIC model. */
	uint16_t fw_recovery : 1; /* FW recovery flag. Used solely in UEFI. */

	/*
	 * Struct array to store the memory region information.
	 * The index to this array is calculated as:
	 * index = mss_num * MAX_DIMM_PER_MEM_CTRL + dimm_slot_num
	 * So there may be cases where a low index doesn't have a valid
	 * entry but a higher index does.
	 */
	struct bf_efi_mem_region region[MAX_DIMM_NUM];
	/*
	 * Struct array to store the NVDIMM specific information.
	 * This array is always filled up from index 0 to the number of
	 * NVDIMMs we have.
	 */
	struct bf_efi_nvdimm nvdimm[MAX_NVDIMM_NUM];

	/* Private data for the TmFifo driver */
	void *tmfifo;

	/* Reserved for the UEFI SystemTable pointer */
	void *efi_sys_tbl;

	/* BlueField system name */
	char sys_name[24];

	/* BlueField dev_id */
	uint32_t dev_id;

	/*
	 * ARS (address range scrub) structure.
	 * Make it aligned so it could start from offset NVDIMM_ARS_OFF in
	 * this structure, where it'll be accessed by the ACPI code.
	 */
	struct bf_ars ars __attribute__((aligned(NVDIMM_ARS_OFF)));
};

#endif /* __BLUEFIELD_EFI_INFO_H__ */
