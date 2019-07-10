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

#ifndef __PLATFORM_DEF_H__
#define __PLATFORM_DEF_H__

#include <arch.h>
#include <common_def.h>
#include <interrupt_props.h>
#include <platform_def.h>
#include <tbbr_img_def.h>
#include <xlat_tables.h>
#include "bluefield_def.h"

/* Special value used to verify platform parameters from BL2 to BL31 */
#define BF_BL31_PLAT_PARAM_VAL		0x0f1e2d3c4b5a6978ULL

/*
 * Macros mapping the MPIDR Affinity levels to BlueField Platform Power levels.
 * The power levels have a 1:1 mapping with the MPIDR affinity levels.
 */
#define BF_PWR_LVL0		MPIDR_AFFLVL0
#define BF_PWR_LVL1		MPIDR_AFFLVL1
#define BF_PWR_LVL2		MPIDR_AFFLVL2

/*
 *  Macros for local power states encoded by State-ID field
 *  within the power-state parameter.
 */
/* Local power state for power domains in Run state. */
#define BF_LOCAL_STATE_RUN		0
/* Local power state for retention. Valid only for CPU power domains */
#define BF_LOCAL_STATE_RET		1
/* Local power state for OFF/power-down. Valid for CPU and cluster power
 * domains
 */
#define BF_LOCAL_STATE_OFF		2

/*
 * This macro defines the deepest retention state possible. A higher state
 * id will represent an invalid or a power down state.
 */
#define PLAT_MAX_RET_STATE		BF_LOCAL_STATE_RET

/*
 * This macro defines the deepest power down states possible. Any state ID
 * higher than this is invalid.
 */
#define PLAT_MAX_OFF_STATE		BF_LOCAL_STATE_OFF

/*
 * Some data must be aligned on the biggest cache line size in the platform.
 * This is known only to the platform as it might have a combination of
 * integrated and external caches.
 */

#define CACHE_WRITEBACK_SHIFT		6

#define CACHE_WRITEBACK_GRANULE		(1 << CACHE_WRITEBACK_SHIFT)
/*
 * One cache line needed for bakery locks on BlueField
 */
#define PLAT_PERCPU_BAKERY_LOCK_SIZE	(1 * CACHE_WRITEBACK_GRANULE)


#define BF_IRQ_SEC_PHY_TIMER		29

#define BF_IRQ_SEC_SGI_0		8
#define BF_IRQ_SEC_SGI_1		9
#define BF_IRQ_SEC_SGI_2		10
#define BF_IRQ_SEC_SGI_3		11
#define BF_IRQ_SEC_SGI_4		12
#define BF_IRQ_SEC_SGI_5		13
#define BF_IRQ_SEC_SGI_6		14
#define BF_IRQ_SEC_SGI_7		15

#define BF_IRQ_SEC_RSH_SWINT_0		32
#define BF_IRQ_SEC_RSH_DCNT_0		38

/*
 * Define a list of Group 1 Secure and Group 0 interrupts as per GICv3
 * terminology. On a GICv2 system or mode, the lists will be merged and treated
 * as Group 0 interrupts.
 */
#define BF_G1S_IRQ_PROPS(grp) \
	INTR_PROP_DESC(BF_IRQ_SEC_PHY_TIMER, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_LEVEL), \
	INTR_PROP_DESC(BF_IRQ_SEC_SGI_1, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(BF_IRQ_SEC_SGI_2, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(BF_IRQ_SEC_SGI_3, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(BF_IRQ_SEC_SGI_4, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(BF_IRQ_SEC_SGI_5, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(BF_IRQ_SEC_SGI_7, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(BF_IRQ_SEC_RSH_SWINT_0, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_LEVEL), \
	INTR_PROP_DESC(BF_IRQ_SEC_RSH_DCNT_0, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_LEVEL)

#define BF_G0_IRQ_PROPS(grp) \
	INTR_PROP_DESC(BF_IRQ_SEC_SGI_0, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(BF_IRQ_SEC_SGI_6, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE)

#define MAP_SHARED_RAM			MAP_REGION_FLAT(		\
						SHARED_RAM_BASE,	\
						SHARED_RAM_SIZE,	\
						MT_DEVICE | MT_RW | MT_SECURE)

#define MAP_DEVICES			MAP_REGION_FLAT(DEVICES_BASE,	\
						DEVICES_SIZE,		\
						MT_DEVICE | MT_RW | MT_SECURE)

#define MAP_NS_DRAM1			MAP_REGION_FLAT(		\
						NS_DRAM1_BASE,	\
						NS_DRAM1_SIZE,	\
						MT_MEMORY | MT_RW | MT_NS)

#define MAP_TRIO_DEVICES		MAP_REGION_FLAT(      \
						TRIO_DEVICES_BASE,            \
						TRIO_DEVICES_SIZE,            \
						MT_DEVICE | MT_RW | MT_SECURE)

/*
 * Size for the stack, mmap entries and xlat table are all calculated
 * with trusted board boot enabled.
 */

/* Size of cacheable stacks */
#if IMAGE_BL1
# define PLATFORM_STACK_SIZE		0x1000
#elif IMAGE_BL2
# define PLATFORM_STACK_SIZE		0x1800
#elif IMAGE_BL31
# define PLATFORM_STACK_SIZE		0x400
#endif

/*
 * PLAT_MMAP_ENTRIES depends on the number of entries in the
 * bluefield_mmap array defined for each BL stage.
 */
#if IMAGE_BL1
# define PLAT_MMAP_ENTRIES		4
#elif IMAGE_BL2
# define PLAT_MMAP_ENTRIES		5
#elif IMAGE_BL31
# define PLAT_MMAP_ENTRIES		5
#endif

/*
 * The number of regions like RO(code), coherent and data required by
 * different BL stages which need to be mapped in the MMU.
 */
#define BL_REGIONS			3
#define MAX_MMAP_REGIONS		(PLAT_MMAP_ENTRIES + BL_REGIONS)

/*
 * Platform specific page table and MMU setup constants
 */
#if IMAGE_BL1
# define MAX_XLAT_TABLES		4
#elif IMAGE_BL2
# define MAX_XLAT_TABLES		3
#elif IMAGE_BL31
# define MAX_XLAT_TABLES		5
#endif

/*
 * Support up to 1T memory space.
 */
#define PLAT_PHY_ADDR_SPACE_SIZE	(1ull << 40)
#define PLAT_VIRT_ADDR_SPACE_SIZE	(1ull << 40)

/*
 * Those max sizes are calculated using the current BL RW debug size
 * plus a little space for growth when trusted board boot is enabled.
 */
#define MAX_BL1_RW_SIZE			0x9000

#define MAX_BL2_SIZE			0x57000

#define MAX_BL31_SIZE			0x1E000

#define MAX_IO_DEVICES			3
#define MAX_IO_HANDLES			4

/* Required platform porting definitions */
#define PLAT_NUM_PWR_DOMAINS		(BF_CLUSTER_COUNT + PLATFORM_CORE_COUNT)
#define PLAT_MAX_PWR_LVL		MPIDR_AFFLVL1
#define PLATFORM_CORE_COUNT		(BF_CLUSTER_COUNT * \
					BF_MAX_CPUS_PER_CLUSTER)

#define BOOT_UART_BASE			BF_UART0_BASE
#define BOOT_UART_CLK_IN_HZ		BF_UART_CLK_IN_HZ

/*
 * Note that ARM platform ports use UART 1 here.  That doesn't make sense
 * at the moment, but we should reevaluate when we get closer to real
 * system configurations.
 */
#define BL31_RUN_UART_BASE		BF_UART0_BASE
#define BL31_RUN_UART_CLK_IN_HZ		BF_UART_CLK_IN_HZ
#define CRASH_UART_BASE			BF_UART0_BASE
#define CRASH_UART_CLK_IN_HZ		BF_UART_CLK_IN_HZ

/*******************************************************************************
 * BL1 specific defines.
 * BL1 RW data is relocated from ROM to RAM at runtime so we need 2 sets of
 * addresses.
 ******************************************************************************/
#define BL1_RO_BASE			TRUSTED_ROM_BASE
#define BL1_RO_LIMIT			(TRUSTED_ROM_BASE + TRUSTED_ROM_SIZE)
/*
 * Put BL1 RW at the top of the Trusted SRAM.
 */
#define BL1_RW_BASE			(BL_RAM_BASE + BL_RAM_SIZE -	\
					 MAX_BL1_RW_SIZE)
#define BL1_RW_LIMIT			(BL_RAM_BASE + BL_RAM_SIZE)

/*******************************************************************************
 * BL2 specific defines.
 ******************************************************************************/
/*
 * Put BL2 at the bottom just above the shared memory region.
 */
#define BL2_BASE			BL_RAM_BASE
#define BL2_LIMIT			(BL_RAM_BASE + MAX_BL2_SIZE)

/*******************************************************************************
 * BL31 specific defines.
 ******************************************************************************/
/*
 * Put BL31 just below BL1 R/W section, if we ever run out of space we can
 * overlap BL31's data (nobits) section with BL1 R/W section.
 */
#define BL31_BASE			(BL1_RW_BASE - MAX_BL31_SIZE)
#define BL31_LIMIT			BL1_RW_BASE

/*
 * Load address of BL33 for bluefield.
 */
#define NS_IMAGE_OFFSET			(DRAM1_BASE + 0x8000000)

#endif /* __PLATFORM_DEF_H__ */
