/*
 * Copyright (c) 2014-2015, ARM Limited and Contributors. All rights reserved.
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

#ifndef __BLUEFIELD_DEF_H__
#define __BLUEFIELD_DEF_H__

#if !USE_COHERENT_MEM
# error Must define USE_COHERENT_MEM=1 for BlueField
#endif

#define BF_CLUSTER_COUNT		8
#define BF_MAX_CPUS_PER_CLUSTER		2
#define BF_MAX_CPUS_PER_CLUSTER_LOG2    1
#define BF_CLUSTER_SHIFT \
	(MPIDR_AFF1_SHIFT-BF_MAX_CPUS_PER_CLUSTER_LOG2)
#define BF_PRIMARY_CPU			0x0

/*******************************************************************************
 * Memory configurations related constants
 ******************************************************************************/

/* Maximum number of memory controllers */
#define MAX_MEM_CTRL			2
/* Maximum number of dimms inserted per memory controller */
#define MAX_DIMM_PER_MEM_CTRL		2
/* Maximum number of physical ranks inserted per memory controller */
#define MAX_RANKS_PER_MEM_CTRL		4
/* Number of ports per memory controller */
#define NUM_PORTS_PER_MEM_CTRL		2
/* Number of physical memory regions the system has */
#define NUM_PMRS			8
/* Number of entries each LMR-to-MSS table has */
#define NUM_LMRS			8

/*******************************************************************************
 * Additional "files" that BlueField may use to boot
 ******************************************************************************/

/* Optional file containing information of the current system */
#define BF_SYS_FILE			29

/*******************************************************************************
 * BlueField memory map and layout related constants
 ******************************************************************************/

/* The first 4KB of Trusted SRAM are used as shared memory */
#define TRUSTED_SRAM_SIZE		0x00080000	/* 512 KB */
#define TRUSTED_SRAM_BASE		0x00400000
#define SHARED_RAM_BASE			TRUSTED_SRAM_BASE
#define SHARED_RAM_SIZE			0x00001000	/* 4 KB */

/* The remaining Trusted SRAM is used to load the BL images */
#define BL_RAM_BASE			(SHARED_RAM_BASE + SHARED_RAM_SIZE)
#define BL_RAM_SIZE			(TRUSTED_SRAM_SIZE - SHARED_RAM_SIZE)

#define TRUSTED_ROM_BASE		0x00000000
#define TRUSTED_ROM_SIZE		0x00020000	/* 128 KB */

#define DRAM1_BASE			ULL(0x80000000)
#define DRAM1_SIZE			ULL(0x80000000)
#define DRAM1_END			(DRAM1_BASE + DRAM1_SIZE - 1)

#define NS_DRAM1_BASE			DRAM1_BASE
#define NS_DRAM1_SIZE			DRAM1_SIZE
#define NS_DRAM1_END			(NS_DRAM1_BASE + NS_DRAM1_SIZE - 1)

/*
 * Mapping for all the devices in the lowest 2 GB of address space, including
 * the rshim (with UARTs), simulator devices, GIC and interconnect, but
 * excluding the boot ROM and boot SRAM.
 */
#define DEVICES_BASE			0x00800000
#define DEVICES_SIZE			0x7f800000ull

/* Base for rshim channel 0. */
#define RSHIM_BASE			0x00800000

/*
 * TYU base address including Cause, GPIO and SMBus memory space.
 * Note that memory regions accessed by the I2C driver are Cause region: from
 * 0x2801200 to 0x2801220, and SMBus1 region: from 0x2804800 to 0x2804fff.
 */
#define TYU_BASE_ADDRESS		0x2800000

/* The physical address of the first byte of memory. */
#define MEMORY_BASE			0x0000000080000000UL

/* EMMC device */
#define EMMC_BASE			0x06000000

/* Offset of the controller addresses from the base of device address region. */
#define EMMC_ADDR_OFFSET		0x8000

/* Memory mapped Generic timer interfaces  */
#define SYS_CNTCTL_BASE			0x04800000
#define SYS_CNTREAD_BASE		0x04810000

/* Generic ARM watchdog control (secure) base */
#define ARM_WDOG_BASE			0x07800000

/*
 * PL011 related constants
 */
#define BF_UART0_BASE			0x01000000
#define BF_UART1_BASE			0x01800000
/*******************************************************************************
 * Clock related constants
 ******************************************************************************/
#define BF_REF_CLK_IN_HZ		156250000

/*******************************************************************************
 * Console related constants
 ******************************************************************************/
#define BF_DEFAULT_BAUDRATE		115200

#define BF_UART_CLK_IN_HZ		BF_REF_CLK_IN_HZ

/*******************************************************************************
 * GIC-400 & interrupt handling related constants
 ******************************************************************************/

/* Base platform compatible GIC memory map */
#define BASE_GICD_BASE			0x12000000
#define BASE_GICR_BASE			0x12200000
#define BASE_GICC_BASE			0x11000000
#define BASE_GICH_BASE			0x11010000
#define BASE_GICV_BASE			0x11020000

/*******************************************************************************
 * Trusted board boot
 ******************************************************************************/
/*
 * There are 4 keys stored in BlueField's efuses; we are using the
 * zeroth one for ATF's trusted board boot. Note that this def is used in other
 * macro expansions and so it could only be the value of 0, 1, 2 or 3. Any
 * brackets or other macro expansions for this def would result in a build fail.
 */
#define BF_USE_KEY_NUM			0
#define BF_KEY_SIZE_BYTES		64
#define BF_KEY_SIZE_WORDS		(BF_KEY_SIZE_BYTES / sizeof(uint64_t))
/* The size of the chip id in bytes used when setting the chip into rma mode */
#define BF_CHIP_ID_SIZE_BYTES		8

#define BF_CHIP_STATE_NONSECURE		0
#define BF_CHIP_STATE_SECURE		1
#define BF_CHIP_STATE_RMA		3

/*
 * The following definitions are defined this way so that KEY_NUM gets expanded
 * first before pasting so that the expanded def provide a valid definition.
 * Also note that you also need to include rsh.h in order to use these
 * definitions.
 */
/* The offset of the first SBKEY register we are using. */
#define ____SB_KEY_OFFSET(KEY_NUM)	(RSH_SB_KEY_ ##KEY_NUM ##__FIRST_WORD)
#define __SB_KEY_OFFSET(KEY_NUM)	____SB_KEY_OFFSET(KEY_NUM)
#define SB_KEY_OFFSET			__SB_KEY_OFFSET(BF_USE_KEY_NUM)
/*
 * The field of RSH_SB_KEY_VLD that corresponds to the fuses we are using for
 * secure boot.
 */
#define ____BF_KEY_VLD(KEY_NUM)		key_ ##KEY_NUM
#define __BF_KEY_VLD(KEY_NUM)		____BF_KEY_VLD(KEY_NUM)
#define BF_KEY_VLD			__BF_KEY_VLD(BF_USE_KEY_NUM)

#define BF_TBB_KEY_REG_START		(RSHIM_BASE + SB_KEY_OFFSET)

/*******************************************************************************
 * EMMC related constants
 ******************************************************************************/

/* EMMC clock rate */
#define EMMC_CLK_RATE			(50 * 1000 * 1000)

/*******************************************************************************
 *  Shared Data
 ******************************************************************************/

/* Entrypoint mailbox.  The format of the data at this address is as
 * follows:
 *
 * 1. One 64-bit word containing the entry point to jump to when a CPU is
 *    asked to start up (via the PSCI cpu_on API).
 *
 * 2. A CPU bitmap, built out of 64-bit words, indexed by the CPU's linear
 *    index (the same as returned by plat_my_core_pos()).  Once a secondary
 *    CPU's bit in the bitmap is 1, it may proceed out of the holding pen
 *    and call the entry point.
 *
 * The processor that sets a bit in the bitmap will issue an SEV
 * instruction afterwards, so secondary CPUs can use WFE in their wait
 * loops.
 *
 * Note that none of this data has been properly configured until
 * plat_setup_psci_ops has been called; this is signified by bit 0 of the
 * rshim's SCRATCHPAD4 register being set.  Before that time, the mailbox
 * may contain random data, since the rshim boot RAM is not zeroed upon
 * reset.  Thus, all readers must first check that bit in SCRATCHPAD4; if
 * it's still zero, they must behave as though the entry point and CPU
 * bitmap are also zero.
 */
#define MBOX_BASE			SHARED_RAM_BASE

/* ARS (Address Range Scrub) structure offset within bf_efi structure. */
#define NVDIMM_ARS_OFF			0x800

#endif /* __BLUEFIELD_DEF_H__ */
