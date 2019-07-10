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

#include <assert.h>
#include <debug.h>
#include <mmio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <delay_timer.h>

#include "bluefield_ddr.h"
#include "bluefield_def.h"
#include "bluefield_efi_info.h"
#include "bluefield_platform.h"
#include "bluefield_private.h"
#include "bluefield_system.h"
#include "hnf.h"
#include "l3c.h"
#include "rsh.h"
#include "i2c_smbus.h"

/* DIMM ID to I2C address mapping. */
const uint8_t csr_i2c_addr[MAX_DIMM_NUM] = {
	I2C_DIMM_0_ADDR, I2C_DIMM_1_ADDR, I2C_DIMM_2_ADDR, I2C_DIMM_3_ADDR };

const uint8_t bf_efi_magic_number[] = { 'U', 'E', 'F', 'I' };

/*
 * API to read register (one byte) at specific offset.
 * This API is used to read the NVDIMM control registers documented in
 * JEDEC Spec (JESD245B). It's called to control the NVDIMM Save/Restore.
 *
 * Return:
 * 	The result of i2c_smbus_transfer(), which is 0(success), others(fail).
 */
int bluefield_ddr_read_reg(uint32_t dimm, uint8_t *value, uint32_t offset)
{
	return i2c_smbus_transfer(csr_i2c_addr[dimm], offset, 1, value,
				  I2C_SMBUS_READ);
}

/*
 * API to write register (one byte) at specific offset.
 * This API is used to write the NVDIMM control registers documented in
 * JEDEC Spec (JESD245B). It's called to control the NVDIMM Save/Restore.
 *
 * Return:
 * 	The result of i2c_smbus_transfer(), which is 0(success), others(fail).
 */
int bluefield_ddr_write_reg(uint32_t dimm, uint8_t value, uint32_t offset)
{
	return i2c_smbus_transfer(csr_i2c_addr[dimm], offset, 1, &value,
				  I2C_SMBUS_WRITE);
}

#define NVDIMM_READ_REG(id, value, off) \
{ \
  if (bluefield_ddr_read_reg(id, value, off) != 0) \
    return -1; \
}

#define NVDIMM_WRITE_REG(id, value, off) \
{ \
  if (bluefield_ddr_write_reg(id, value, off) != 0) \
    return -1; \
}

/*
 * Check NVDIMM register value until some bits are cleared or timeout.
 * Return:
 *     0 (bits are cleared), -1 (access failed)
 */
static int nvdimm_cmd_status_check(uint32_t dimm, uint32_t offset,
				   uint8_t *value, uint8_t bitmask,
				   uint32_t timeout)
{
	while (timeout-- > 0) {
		NVDIMM_READ_REG(dimm, value, offset);
		if (!(*value & bitmask))
			return 0;
		mdelay(1);
	}

	return -1;
}

/*
 * Read NVDIMM timeout value from two offsets.
 *
 * Input:
 *     offset_0:  offset of the low-bits timeout
 *     offset_1:  offset of the high-bits timeout
 *     max_ms:    maximum allowed timeout value
 *
 * Return:
 *     Timeout value in milliseconds. In case of register access failures,
 *     the 'max_ms' will be returned.
 */
static uint32_t nvdimm_get_timeout(uint32_t dimm, uint32_t offset_0,
				   uint32_t offset_1, uint32_t max_ms)
{
	uint32_t timeout = max_ms;
	uint8_t lo_value, hi_value;

	if (bluefield_ddr_read_reg(dimm, &lo_value, offset_0) == 0 &&
	    bluefield_ddr_read_reg(dimm, &hi_value, offset_1) == 0) {
		timeout = (hi_value << 8) + lo_value;

		/* Check whether the time unit is in seconds. */
		if (hi_value & 0x80)
			timeout = (timeout & 0x7FFF) * 1000;

		if (timeout > max_ms)
			timeout = max_ms;
	}

	return timeout;
}

#ifdef IMAGE_BL2

/* The struct we got which discribes the memory layout. */
extern struct bf_mem_info bf_memory_layout;

#define NVDIMM_READ_SPD(id, field, off) \
{ \
	if (bf_sys_get_spd(field, off, sizeof(field), \
	    (id) / MAX_DIMM_PER_MEM_CTRL, (id) % MAX_DIMM_PER_MEM_CTRL) == 0) \
		return -1; \
}
static int nvdimm_get_spd_info(uint32_t id, struct bf_efi_nvdimm *v)
{
	NVDIMM_READ_SPD(id, v->vendor_id, SPD_VENDOR_ID_OFF);
	NVDIMM_READ_SPD(id, v->device_id, SPD_DEVICE_ID_OFF);
	NVDIMM_READ_SPD(id, v->revision_id, SPD_REVISION_ID_OFF);
	NVDIMM_READ_SPD(id, v->subsystem_vendor_id,
			SPD_SUBSYSTEM_VENDOR_ID_OFF);
	NVDIMM_READ_SPD(id, v->subsystem_device_id,
			SPD_SUBSYSTEM_DEVICE_ID_OFF);
	NVDIMM_READ_SPD(id, v->subsystem_revision_id,
			SPD_SUBSYSTEM_REVISION_ID_OFF);
	NVDIMM_READ_SPD(id, v->manufacturing_location,
			SPD_MANUFACTURING_LOCATION_OFF);
	NVDIMM_READ_SPD(id, v->manufacturing_data, SPD_MANUFACTURING_DATA_OFF);
	NVDIMM_READ_SPD(id, v->serial_number, SPD_SERIAL_NUMBER_OFF);
	NVDIMM_READ_SPD(id, v->region_format_interface_code,
			SPD_REGION_FORMAT_INTERFACE_CODE_OFF);
	return 0;
}

/* Get memory region information for a DIMM.
 *
 * For now a stub NVDIMM entry (128MB) is added if running with the
 * simulator for testing purpose.
 * This function needs to be rewritten once PMR & I2C code are all
 * integrated.
 */
static int dimm_get_mem_region(uint32_t dimm,
			       struct bf_efi_mem_region *region)
{
#if MAX_DIMM_PER_MEM_CTRL != 2
#error Code assumes MAX_DIMM_PER_MEM_CTRL == 2, must fix
#endif
#if MAX_MEM_CTRL != 2
#error Code assumes MAX_MEM_CTRL == 2, must fix
#endif
	int mss_index = dimm / MAX_DIMM_PER_MEM_CTRL;
	int dimm_slot_index = dimm % MAX_DIMM_PER_MEM_CTRL;
	struct bf_mem_dimm_info current_bmdi;

	memset(region, 0, sizeof(*region));

	current_bmdi = bf_memory_layout.mem_ctrl_info[mss_index]
			.dimm_info[dimm_slot_index];

	if (current_bmdi.size_in_gb > 0) {
		region->socket_id = 0;
		region->memory_controller_id = mss_index;
		region->memory_channel_number = 0;
		region->is_nvdimm = current_bmdi.is_nvdimm;
		region->dimm = dimm;
		region->ranks = current_bmdi.ranks_num *
				current_bmdi.ddr_3ds_num;
		region->data_bits = current_bmdi.data_bits;
		region->is_registered = current_bmdi.is_registered;
		region->is_load_reduced = current_bmdi.is_load_reduced;
		region->phy_addr = region->is_nvdimm ?
		    ((uint64_t)current_bmdi.pa_start_GB << 30) : MEMORY_BASE;
		/* Change unit from gigabits (not gigabytes) to bytes. */
		region->length = (uint64_t)current_bmdi.size_in_gb * 0x8000000L;
		return 0;
	}
	return -1;
}

/* Set up the EFI information at the specified addr. */
void bluefield_common_setup_efi_info(uintptr_t addr)
{
	RSH_BOOT_CONTROL_t boot_control;
	struct bf_efi *efi_info = (struct bf_efi *)addr;
	const struct sys_info *info;
	const char unknown_sys[] = "unknown";
	int i;

	/* Verify the magic number. */
	if(memcmp(bf_efi_magic_number, efi_info->magic,
		  sizeof(bf_efi_magic_number))) {
		ERROR("Unexpected EFI magic number.\n");
		return;
	}

	/*
	 * Zero out the shared area, but skip over the two fields
	 * set at compile time.
	 */
	memset(&(efi_info->region_num), 0,
	       EFI_INFO_SIZE - offsetof(struct bf_efi, region_num));

	/* Get the memory region for each DIMM. */
	efi_info->region_num = 0;
	for (i = 0; i < MAX_DIMM_NUM; i++)
		if (dimm_get_mem_region(i, &efi_info->region[i]) == 0)
			efi_info->region_num++;

	assert(efi_info->region_num > 0);

	/* Get NVDIMM information. */
	efi_info->nvdimm_num = 0;
	for (i = 0; i < MAX_DIMM_NUM; i++) {
		if (!efi_info->region[i].length)
			continue;

		if (!efi_info->region[i].is_nvdimm)
			continue;

		efi_info->nvdimm[efi_info->nvdimm_num].dimm = i;
		if (nvdimm_get_spd_info(efi_info->region[i].dimm,
				&efi_info->nvdimm[efi_info->nvdimm_num]) != 0)
			break;
		efi_info->nvdimm_num++;
		if (efi_info->nvdimm_num >= MAX_NVDIMM_NUM)
			break;
	}

	efi_info->bf_is_pal = bf_sys_is_pal();
	efi_info->snic_model = bf_sys_snic_model();
	info = get_current_sys();
	if (info)
		memcpy(efi_info->sys_name, info->sys_name,
			ARRAYSIZE(efi_info->sys_name));
	else
		memcpy(efi_info->sys_name, unknown_sys,
			ARRAYSIZE(efi_info->sys_name));

	/*
	 * Set the flag to indicate whether it's external boot.
	 * Restore it to default eMMC boot mode in case the external host
	 * crashes and not able to change it back.
	 */
	boot_control.word = mmio_read_64(RSHIM_BASE + RSH_BOOT_CONTROL);
	efi_info->ext_boot = (boot_control.boot_mode ==
		RSH_BOOT_CONTROL__BOOT_MODE_VAL_NONE) ? 1 : 0;
	boot_control.boot_mode = RSH_BOOT_CONTROL__BOOT_MODE_VAL_EMMC;
	mmio_write_64(RSHIM_BASE + RSH_BOOT_CONTROL, boot_control.word);

	/* Set the flag to indicate whether the FW recovery is needed. */
	efi_info->fw_recovery = (boot_on_spi_flash_recovery != 0);

	efi_info->dev_id = mmio_read_32(TYU_BASE_ADDRESS + TYU_BOOT_DEVID);

	/* Make sure it's not out-of-boundary. */
	assert(sizeof(*efi_info) <= EFI_INFO_SIZE);
	assert((uintptr_t)&((struct bf_efi *)0)->ars == NVDIMM_ARS_OFF);

	/* Flush content to memory. */
	flush_dcache_range(addr, sizeof(*efi_info));
}

#pragma weak bluefield_setup_efi_info
void bluefield_setup_efi_info(uintptr_t addr)
{
	bluefield_common_setup_efi_info(addr);
}

/*
 * NVDIMM Spec (JESD245B Draft 8):
 *   Section 9.6: Abort Running Operation workflow.
 */
static int nvdimm_abort_operation(uint32_t dimm)
{
	uint32_t timeout;
	uint8_t value;

	/* Read the timeout value. If bit 1 in the CAPABILITIES1 register
	 * is set, the timeout is in seconds.
	 */
	NVDIMM_READ_REG(dimm, &value, NVDIMM_ABORT_CMD_TIMEOUT_OFF);
	timeout = value;
	NVDIMM_READ_REG(dimm, &value, NVDIMM_CAPABILITIES1_OFF);
	if (value & NVDIMM_CAPABILITIES1_ABORT_TIMEOUT_IN_SECONDS)
		timeout *= 1000;
	if (timeout > NVDIMM_MAX_RESTORE_DELAY_MS)
		timeout = NVDIMM_MAX_RESTORE_DELAY_MS;

	/* Set the ABORT_CURRENT_OP bit in NVDIMM_FUNC_CMD. */
	NVDIMM_WRITE_REG(dimm, NVDIMM_FUNC_CMD_ABORT_CURRENT_OP,
			 NVDIMM_FUNC_CMD_OFF);

	/* Check the Abort In Progress bit in NVDIMM_CMD_STATUS0. */
	if (nvdimm_cmd_status_check(dimm, NVDIMM_CMD_STATUS0_OFF,
		&value, NVDIMM_CMD_STATUS0_ABORT_IN_PROGRESS, timeout) < 0) {
		ERROR("NVDIMM Restore abort timeout.\n");
		return -1;
	}

	if (value & NVDIMM_ARM_STATUS_ABORT_SUCCESS) {
		INFO("NVDIMM Restore aborted.\n");
		return 0;
	}
	else {
		ERROR("NVDIMM Restore abort failed.\n");
		return -1;
	}
}

/*
 * NVDIMM Spec (JESD245B Draft 8):
 *   Section 9.3: Restore workflow.
 */
static int nvdimm_restore(uint32_t dimm)
{
	uint32_t delay, timeout;
	uint8_t value;

	/* Wait for the Operation In Progress bit in the NVDIMM_CMD_STATUS0
	 * register to be clear.
	 */
	NVDIMM_READ_REG(dimm, &value, NVDIMM_CMD_STATUS0_OFF);
	delay = 0;
	while (value & NVDIMM_CMD_STATUS0_OPER_IN_PROGRESS) {
		mdelay(1);
		if (delay++ >= NVDIMM_MAX_RESTORE_DELAY_MS)
			return -1;
		NVDIMM_READ_REG(dimm, &value, NVDIMM_CMD_STATUS0_OFF);
	}

	/* Set bit 3 (Clear the RESTORE_STATUS register) in NVDIMM_MGT_CMD0. */
	NVDIMM_WRITE_REG(dimm, NVDIMM_MGT_CMD0_CLEAR_RESTORE_STATUS,
			 NVDIMM_MGT_CMD0_OFF);

	timeout = nvdimm_get_timeout(dimm, NVDIMM_RESTORE_TIMEOUT0_OFF,
				NVDIMM_RESTORE_TIMEOUT1_OFF,
				NVDIMM_MAX_RESTORE_TIMEOUT_MS);

	/* Set the START_RESTORE bit in the NVDIMM_FUNC_CMD register. */
	NVDIMM_WRITE_REG(dimm, NVDIMM_FUNC_CMD_START_RESTORE,
			 NVDIMM_FUNC_CMD_OFF);

	/* Check the Restore In Progress bit in NVDIMM_CMD_STATUS0 */
	NVDIMM_READ_REG(dimm, &value, NVDIMM_CMD_STATUS0_OFF);
	while (value & NVDIMM_CMD_STATUS0_RESTORE_IN_PROGRESS) {
		if (timeout-- == 0) {
			/* Abort the operation. */
			nvdimm_abort_operation(dimm);
			break;
		} else {
			/* Retry. */
			NVDIMM_WRITE_REG(dimm, NVDIMM_FUNC_CMD_START_RESTORE,
					 NVDIMM_FUNC_CMD_OFF);
			mdelay(1);
			NVDIMM_READ_REG(dimm, &value, NVDIMM_CMD_STATUS0_OFF);
		}
	}

	NVDIMM_READ_REG(dimm, &value, NVDIMM_RESTORE_STATUS_OFF);
	if (!(value & NVDIMM_RESTORE_STATUS_RESTORE_SUCCESS)) {
		ERROR("NVDIMM Restore failed.\n");
		return -1;
	} else {
		NOTICE("NVDIMM Restored.\n");
		return 0;
	}
}

/*
 * NVDIMM Spec (JESD245B Draft 8):
 *   Section 9.4: Arm workflow.
 */
static int nvdimm_arm(uint32_t dimm)
{
	uint32_t delay, timeout;
	uint8_t value;

	/* Wait for the Operation In Progress bit in the NVDIMM_CMD_STATUS0
	 * register to be clear.
	 */
	NVDIMM_READ_REG(dimm, &value, NVDIMM_CMD_STATUS0_OFF);
	delay = 0;
	while (value & NVDIMM_CMD_STATUS0_OPER_IN_PROGRESS) {
		mdelay(1);
		if (delay++ >= NVDIMM_MAX_ARM_DELAY_MS)
			return -1;
		NVDIMM_READ_REG(dimm, &value, NVDIMM_CMD_STATUS0_OFF);
	}

	/* Set bit 5 (Clear the ARM_STATUS register) in NVDIMM_MGT_CMD0. */
	NVDIMM_WRITE_REG(dimm, NVDIMM_MGT_CMD0_CLEAR_ARM_STATUS,
			 NVDIMM_MGT_CMD0_OFF);

	/* Get timeout. */
	timeout = nvdimm_get_timeout(dimm, NVDIMM_ARM_TIMEOUT0_OFF,
				NVDIMM_ARM_TIMEOUT1_OFF,
				NVDIMM_MAX_ARM_TIMEOUT_MS);

	/* Enable the Catastrophic Save operation. */
	NVDIMM_WRITE_REG(dimm, NVDIMM_ARM_CMD_CSAVE_ON_SAVE_N,
			 NVDIMM_ARM_CMD_OFF);

	/* Check the Restore In Progress bit in NVDIMM_CMD_STATUS0 */
	NVDIMM_READ_REG(dimm, &value, NVDIMM_CMD_STATUS0_OFF);
	while (value & NVDIMM_CMD_STATUS0_ARM_IN_PROGRESS) {
		if (timeout-- == 0) {
			/* Abort the operation. */
			nvdimm_abort_operation(dimm);
			break;
		} else {
			/* Retry. */
			NVDIMM_WRITE_REG(dimm, NVDIMM_ARM_CMD_CSAVE_ON_SAVE_N,
					 NVDIMM_ARM_CMD_OFF);
			mdelay(1);
			NVDIMM_READ_REG(dimm, &value, NVDIMM_CMD_STATUS0_OFF);
		}
	}

	NVDIMM_READ_REG(dimm, &value, NVDIMM_ARM_STATUS_OFF);
	if (!(value & NVDIMM_ARM_STATUS_SAVE_N_ARMED)) {
		ERROR("NVDIMM Arm failed.\n");
		return -1;
	} else {
		INFO("NVDIMM Armed.\n");
		return 0;
	}
}

static int nvdimm_enable_rshim_power_watchdog(void)
{
	RSH_PWR_WDOG_TI_t ti;
	RSH_PWR_WDOG_TF_t tf;
	RSH_PWR_WDOG_CTL_t ctl;

	/* No NMI interrupt for now. */
	ti = (RSH_PWR_WDOG_TI_t) {
		.enable = 0,
		.thresh = 0
	};
	mmio_write_64(RSHIM_BASE + RSH_PWR_WDOG_TI, ti.word);

	/* Enable autonomous flush. */
	tf = (RSH_PWR_WDOG_TF_t) {
		.enable = 1,
		.thresh = 10
	};
	mmio_write_64(RSHIM_BASE + RSH_PWR_WDOG_TF, tf.word);

	/* Enable HW participation in the ADR sequence. */
	ctl = (RSH_PWR_WDOG_CTL_t) {
		.lock = 0,
		.sw_override = 0
	};
	mmio_write_64(RSHIM_BASE + RSH_PWR_WDOG_CTL, ctl.word);

	return 0;
}

void bluefield_setup_nvdimm_restore(void)
{
	int err, armed = 0;
	uint32_t dimm;
	struct bf_efi_mem_region region;

	for (dimm = 0; dimm < MAX_DIMM_NUM; dimm++) {
		if (dimm_get_mem_region(dimm, &region) != 0)
			continue;

		if (!region.is_nvdimm)
			continue;

		err = nvdimm_restore(dimm);
		if (!err) {
			err = nvdimm_arm(dimm);
			if (!err)
				armed = 1;
		}
	}

	if (armed)
		nvdimm_enable_rshim_power_watchdog();
}

#elif defined(IMAGE_BL31)

/* Pointer to the efi info structure. */
struct bf_efi *efi_info;

/*
 * Access a memory range to detect the errors.
 *
 * Read the first 8 bytes of each cacheline. Since error exception (SError) is
 * asynchronous, waiting for the exception for each cacheline seems inefficient.
 * As an optimization, this function initiates the read for the whole block(4KB)
 * and checks the result in the next down-counter interrupt which should have
 * given enough time for the SError to occur and finish. The down-counter is
 * programmed as 4096 (NVDIMM_ARS_COUNT_DOWN), which is around 4 microseconds
 * depending on the clock. If no error is detected, the 4KB memory read should
 * be done within 2us, and leave 2us to handle one serror interrupt. This
 * function will exit after one error is detected.
 */
static __always_inline inline
void nvdimm_ars_scrub(volatile struct bf_ars *ars, uint64_t pa, uint64_t len)
{
	unsigned int scr;

	/*
	 * Enable the scr_el3.EA bit, so External Aborts and SError Interrupts
	 * are taken to EL3 and handled by ATF.
	 */
	scr = read_scr_el3();
	if (!(scr & SCR_EA_BIT))
		write_scr_el3(scr | SCR_EA_BIT);

	/* Adjust the PA region (len & pa) to be cache aligned. */
	len += pa - (pa & ~((uint64_t)NVDIMM_ARS_CACHE_LINE_SIZE - 1));
	pa &= ~((uint64_t)NVDIMM_ARS_CACHE_LINE_SIZE - 1);

	/*
	 * Initiate read access to the memory block of 'len' bytes.
	 * This is trying to accelerate the scrub process instead of calling
	 * dsbsy() for each cacheline.
	 */
	for (;;) {
		/* Don't continue if error detected on this block. */
		if (ars->serror)
			break;

		*(volatile uint64_t *)pa;
		pa += NVDIMM_ARS_CACHE_LINE_SIZE;
		if (len <= NVDIMM_ARS_CACHE_LINE_SIZE)
			break;
		len -= NVDIMM_ARS_CACHE_LINE_SIZE;
	}
	dsbsy();

	/* Done with this block. Restore the scr_el3.EA bit. */
	if (!(scr & SCR_EA_BIT))
		write_scr_el3(scr);
}

/*
 * Adjust PA / Len to fall into a NVDIMM region.
 * Return NVDIMM handle (dimm_id + 1) to be consistent with ACPI
 * definition, or -1 (not found).
 */
static int nvdimm_adjust_ars_range(uint64_t *pa, uint64_t *len)
{
	int i;
	struct bf_efi_mem_region *region = efi_info->region;

	for (i = 0; i < efi_info->region_num; i++) {
		if (region[i].is_nvdimm &&
		    *pa + *len > region[i].phy_addr &&
		    *pa < region[i].phy_addr + region[i].length) {
			/* Found an overlapped NVDIMM region. */
			if (*pa <= region[i].phy_addr) {
				*len -= region[i].phy_addr - *pa;
				*pa = region[i].phy_addr;
			}
			return i + 1;
		}
	}

	*len = 0;
	return -1;
}

static void nvdimm_ars_set_ext_status(struct bf_ars *ars, uint16_t status)
{
	/*
	 * ACPI will check the ext_status to tell whether the
	 * ARS process is completed or not. It could run on a diffrent
	 * core than the SWINT or DOWN-COUNTER interrupt handler does. So add
	 * a barrier to make sure previous writes are all done before setting
	 * the status.
	 */
	dsbsy();
	ars->output.query.ext_status = status;
}

/* NVDIMM ARS handler. */
uint64_t nvdimm_ars_irq_handler(int irq, void *arg)
{
	struct bf_ars *ars = &efi_info->ars;

	/*
	 * Add an asm statement here so the two labels won't be optimized out,
	 * which are used in bluefield_serror.S.
	 */
	__asm__ volatile goto("\n\t"
			 :::: nvdimm_ars_irq_handler_begin,
			      nvdimm_ars_irq_handler_end);

nvdimm_ars_irq_handler_begin:

	/* Disable the interrupt. */
	mmio_write_64(RSHIM_BASE + RSH_SWINT, 0);

	INFO("ARS start_pa=0x%llx, start_len=0x%llx, restart_pa=0x%llx, "
	      "restart_len=0x%llx, flags=0x%x status=%d, func=%d\n",
	      ars->start_pa, ars->start_len, ars->output.query.restart_pa,
	      ars->output.query.restart_len, ars->flags,
	      ars->output.query.ext_status, ars->func);

	/* Handle new START_ARS request. */
	if (ars->func == ARS_FUNC_START_ARS &&
	    ars->output.query.restart_pa == 0) {
		RSH_DOWN_COUNT_VALUE_t dcnt = {
			.count = NVDIMM_ARS_COUNT_DOWN
			};
		RSH_DOWN_COUNT_CONTROL_t dctl = {
			.ena = 1,
			/* .mode = 0, */
			};

		/* Set initial value of the output. */
		ars->output.query.ext_status = ARS_EXT_STATUS_INPROGRESS;
		ars->output.query.start_pa = ars->start_pa;
		ars->output.query.start_len = ars->start_len;
		ars->output.query.restart_pa = ars->start_pa;
		ars->output.query.restart_len = ars->start_len;
		/* type: bit 0 (volatile memory), bit 1 (Persistent Memory) */
		ars->output.query.type = 0x2;
		ars->output.query.flags = 0;
		ars->output.query.num = 0;
		ars->output.query.size = (uintptr_t)ars->output.query.rec -
					 (uintptr_t)&ars->output.query - 4;
		ars->output.query.rec[0].len = 0;

		/* Only scrub NVDIMM regions. */
		ars->handle = nvdimm_adjust_ars_range(
					&ars->output.query.restart_pa,
					&ars->output.query.restart_len);
		if (ars->handle < 0 || ars->output.query.restart_len == 0) {
			nvdimm_ars_set_ext_status(ars, ARS_EXT_STATUS_COMPLETE);
			return 0;
		}

		/* Initiate the scrub. */
		ars->serror = 0;
		nvdimm_ars_scrub(ars, ars->output.query.restart_pa,
				 NVDIMM_ARS_BLOCK_SIZE);

		/*
		 * Enable the down-counter interrupt.
		 * The down-counter cycle configured to be ~4us, which should
		 * be good enough for scrub of this block to complete. See
		 * more comments for function nvdimm_ars_scrub().
		 */
		mmio_write_64(RSHIM_BASE +
			RSH_DOWN_COUNT_VALUE__FIRST_WORD, dcnt.word);
		mmio_write_64(RSHIM_BASE +
			RSH_DOWN_COUNT_REFRESH_VALUE__FIRST_WORD, dcnt.word);
		mmio_write_64(RSHIM_BASE +
			RSH_DOWN_COUNT_CONTROL__FIRST_WORD, dctl.word);
	} else if (ars->func == ARS_FUNC_CLEAR_ERROR) {
		/* Handle the CLEAR_ERROR request. */
		uint64_t pa = ars->start_pa, len = ars->start_len;

		/*
		 * Zero out cache line with dc zva, then flush to memory
		 * with dc cvac.
		 */
		/* zero_normalmem() is an alias of zeromem_dczva(). */
		zero_normalmem((void *)pa, len);
		clean_dcache_range(pa, len);

		ars->output.clear.status = 0;
		ars->output.clear.len = ars->start_len;
		nvdimm_ars_set_ext_status(ars, ARS_EXT_STATUS_COMPLETE);
	}

nvdimm_ars_irq_handler_end:

	return 0;
}

/* Timer interrupt triggered by the RSHIM down-counter-0. */
static uint64_t nvdimm_dcnt0_irq_handler(int irq, void *arg)
{
	struct bf_ars *ars = &efi_info->ars;
	RSH_DOWN_COUNT_CONTROL_t dctl;
	RSH_DOWN_COUNT_VALUE_t dcnt = {
		.count = NVDIMM_ARS_COUNT_DOWN
		};

	/*
	 * Add an asm statement here so the two labels won't be optimized out,
	 * which are used in bluefield_serror.S.
	 */
	__asm__ volatile goto("\n\t"
			 :::: nvdimm_dcnt0_irq_handler_begin,
			      nvdimm_dcnt0_irq_handler_end);

nvdimm_dcnt0_irq_handler_begin:

	/* Disable the interrupt. */
	dctl.word = mmio_read_64(RSHIM_BASE +
				 RSH_DOWN_COUNT_CONTROL__FIRST_WORD);
	dctl.ena = 0;
	mmio_write_64(RSHIM_BASE +
		      RSH_DOWN_COUNT_CONTROL__FIRST_WORD, dctl.word);

	/* Don't continue if ARS is not in progress. */
	if (ars->output.query.ext_status != ARS_EXT_STATUS_INPROGRESS)
		return 0;

	/* Fill in the error record if serror is found. */
	if (ars->serror) {
		uint64_t pa = ars->output.query.restart_pa;
		struct ars_err_rec *rec = ars->output.query.rec;

		/* Walk through each cacheline to find the error. */
		for (pa = ars->output.query.restart_pa;
		     pa < ars->output.query.restart_pa + NVDIMM_ARS_BLOCK_SIZE;
		     pa += NVDIMM_ARS_CACHE_LINE_SIZE) {
			int num = ars->output.query.num;

			/* Stop if no more space for more error records. */
			if (num >= NVDIMM_ARS_REC_NUM) {
				nvdimm_ars_set_ext_status(
					ars, ARS_EXT_STATUS_PRE_STOP);
				return 0;
			}

			/*
			 * How to tell a specific line has error?
			 *
			 * Here we read it again and use a full memory fence
			 * to wait for it to finish. Needs to verify it on real
			 * HW to see whether the SError can actually finish.
			 * If not, other way should be investigated such as by
			 * checking the return result with the assumption that
			 * the Data Abort won't change the target register.
			 */
			ars->serror = 0;
			__asm__ volatile("dsb st\n\t");
			*(volatile uint64_t *)pa;
			__asm__ volatile("dsb sy\n\t");
			if (!ars->serror)
				continue;

			/* Merge into last record, or create a new one. */
			if (rec[num].len && pa == rec[num].pa + rec[num].len &&
			    rec[num].handle == ars->handle) {
				rec[num].len += NVDIMM_ARS_CACHE_LINE_SIZE;
			} else {
				NOTICE("Found memory error at address 0x%llx "
					"during address range scrub.\n", pa);
				rec[num].handle = ars->handle;
				rec[num].pa = pa;
				rec[num].len = NVDIMM_ARS_CACHE_LINE_SIZE;
				ars->output.query.num++;
			}
		}
	}

	if (ars->output.query.restart_len <= NVDIMM_ARS_BLOCK_SIZE) {
		/* All done. */
		INFO("ARS done: start_pa=0x%llx, start_len=0x%llx\n",
		     ars->start_pa, ars->start_len);
		nvdimm_ars_set_ext_status(ars, ARS_EXT_STATUS_COMPLETE);
		return 0;
	}

	/* Start the next block. */
	ars->serror = 0;
	ars->output.query.restart_pa += NVDIMM_ARS_BLOCK_SIZE;
	ars->output.query.restart_len -= NVDIMM_ARS_BLOCK_SIZE;

	/* Only scrub NVDIMM regions. */
	ars->handle = nvdimm_adjust_ars_range(&ars->output.query.restart_pa,
					      &ars->output.query.restart_len);
	if (ars->handle < 0 || ars->output.query.restart_len == 0) {
		INFO("ARS done: start_pa=0x%llx, start_len=0x%llx\n",
			ars->start_pa, ars->start_len);
		nvdimm_ars_set_ext_status(ars, ARS_EXT_STATUS_COMPLETE);
		return 0;
	}

	nvdimm_ars_scrub(ars, ars->output.query.restart_pa,
			 NVDIMM_ARS_BLOCK_SIZE);

	/* Re-enable the down-counter interrupt. */
	mmio_write_64(RSHIM_BASE + RSH_DOWN_COUNT_VALUE__FIRST_WORD, dcnt.word);
	dctl.ena = 1;
	mmio_write_64(RSHIM_BASE +
		      RSH_DOWN_COUNT_CONTROL__FIRST_WORD, dctl.word);

nvdimm_dcnt0_irq_handler_end:

	return 0;
}

void bluefield_init_efi_info(uintptr_t addr)
{
	int i;
	mmap_region_t *map;

	/* Save the address. */
	efi_info = (struct bf_efi *)addr;

	/* Verify the magic number. */
	if(memcmp(bf_efi_magic_number, efi_info->magic,
		  sizeof(bf_efi_magic_number))) {
		ERROR("Unexpected EFI magic number.\n");
		return;
	}

	/* Don't continue if we don't have at least one valid entry for both. */
	if (!efi_info->region_num || !efi_info->nvdimm_num)
		return;

	/*
	 * Fix the size in MAP_NS_DRAM1, which is located at map[size - 2].
	 * The last one is an empty entry. See bluefield_mmap[] definition in
	 * bluefield_common.c
	 */
	map = (mmap_region_t *)(bluefield_get_mmap() + PLAT_MMAP_ENTRIES - 2);
	map->size = 0;
	for (i = 0; i < efi_info->region_num; i++)
		map->size += efi_info->region[i].length;

	/* Initialize the ARS area. */
	memset(&efi_info->ars, 0, sizeof(efi_info->ars));

	/* Register the SWINT0 interrupt to handle NVDIMM ARS request. */
	if (bluefield_irq_register(BF_IRQ_SEC_RSH_SWINT_0, NULL,
				   nvdimm_ars_irq_handler))
		ERROR("NVDIMM unable to register the ars irq handler.\n");

	/* Register the DCNT0 interrupt for asymchronous NVDIMM ARS handling. */
	if (bluefield_irq_register(BF_IRQ_SEC_RSH_DCNT_0, NULL,
				   nvdimm_dcnt0_irq_handler))
		ERROR("NVDIMM unable to register the dcnt0 irq handler.\n");
}

/*
 * Check NVDIMM state and start the Save operation.
 */
void bluefield_setup_nvdimm_save(void)
{
	int i, mc_bitmap = 0, mc_idx, dimm;
	uint8_t value;
	uint32_t max_timeout[MAX_NVDIMM_NUM], timeout, cnt;
	RSH_PWR_WDOG_CTL_t ctl;

	if (efi_info->nvdimm_num == 0)
		return;

	for (i = 0; i < efi_info->nvdimm_num; i++) {
		dimm = efi_info->nvdimm[i].dimm;

		/* Get the CSAVE timeout value for each one. */
		max_timeout[i] = nvdimm_get_timeout(
			dimm,
			NVDIMM_CSAVE_TIMEOUT0, NVDIMM_CSAVE_TIMEOUT1,
			NVDIMM_MAX_SAVE_TIMEOUT_MS);

		/* Track memory controller usage. */
		if (efi_info->region[dimm].memory_controller_id == 0)
		mc_bitmap |= 1 << efi_info->region[dimm].memory_controller_id;
	}

	/* Enable SW override in the ADR sequence. Once set, the power watchdog
	 * hardware will not participate in the ADR sequence
	 */
	ctl = (RSH_PWR_WDOG_CTL_t) {
		.lock = 0,
		.sw_override = 1
	};
	mmio_write_64(RSHIM_BASE + RSH_PWR_WDOG_CTL, ctl.word);

	mc_idx = 0;
	while(mc_bitmap != 0) {
		if (mc_bitmap & (1 << mc_idx)) {
			bluefield_nvdimm_save_cstate(mc_idx);
			mc_bitmap &= ~(1 << mc_idx);
		}
		mc_idx++;
	}

	cnt = 0;	/* the number of dimms which are done. */
	timeout = 0;	/* the elapsed time from begining. */

	/* Check all NVDIMMs until all are done or timeout. */
	for (i = 0; cnt != efi_info->nvdimm_num;
			i = (i + 1) % efi_info->nvdimm_num) {
		if (!max_timeout[i])
			continue;

		/* Check the Save In Progress bit in NVDIMM_CMD_STATUS0. */
		if (nvdimm_cmd_status_check(
			efi_info->nvdimm[i].dimm,
			NVDIMM_CMD_STATUS0_OFF,
			&value, NVDIMM_CMD_STATUS0_CSAVE_IN_PROGRESS,
			NVDIMM_MAX_SAVE_DELAY_MS) == 0) {
			/* Clear timeout for this dimm if done. */
			INFO("NVDIMM %d saved.\n", i);
			max_timeout[i] = 0;
			cnt++;
			continue;
		}

		/* Clear timeout for this dimm if expired. */
		timeout += NVDIMM_MAX_SAVE_DELAY_MS;
		if (timeout > max_timeout[i]) {
			ERROR("NVDIMM %d save timeout.\n", i);
			max_timeout[i] = 0;
			cnt++;
		}
	}
}

#endif
