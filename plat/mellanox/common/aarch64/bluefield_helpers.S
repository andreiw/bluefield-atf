/*
 * Copyright (c) 2013-2016, ARM Limited and Contributors. All rights reserved.
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

#include <arch.h>
#include <asm_macros.S>
#include <cortex_a72.h>
#include <gicv2.h>
#include <gicv3.h>
#include <platform_def.h>
#include "../bluefield_def.h"
#include "rsh_def.h"
#include "tyu_def.h"

	.globl	bluefield_calc_core_pos
	.globl	bluefield_get_baudrate
	.globl	plat_my_core_pos
	.globl	plat_crash_console_init
	.globl	plat_crash_console_putc
	.globl	platform_mem_init
	.globl	plat_secondary_cold_boot_setup
	.globl	plat_get_my_entrypoint
	.globl	plat_is_my_cpu_primary
	.globl	plat_reset_handler


	/* -----------------------------------------------------
	 *  unsigned int plat_my_core_pos(void)
	 *  This function uses the bluefield_calc_core_pos()
	 *  definition to get the index of the calling CPU.
	 * -----------------------------------------------------
	 */
func plat_my_core_pos
	mrs	x0, mpidr_el1
	b	bluefield_calc_core_pos
endfunc plat_my_core_pos

	/* -----------------------------------------------------
	 *  unsigned int bluefield_calc_core_pos(u_register_t mpidr)
	 *  Helper function to calculate the core position.
	 *  With this function:
         *  CorePos = (ClusterId * BF_MAX_CPUS_PER_CLUSTER) + CoreId
	 * -----------------------------------------------------
	 */
func bluefield_calc_core_pos
	and	x1, x0, #MPIDR_CPU_MASK
	and	x0, x0, #MPIDR_CLUSTER_MASK
	add	x0, x1, x0, LSR #BF_CLUSTER_SHIFT
	ret
endfunc bluefield_calc_core_pos

	/* -----------------------------------------------------
	 * unsigned int bluefield_get_baudrate(void)
	 * Helper function to calculate the required baud rate.
	 * Clobber list : x0, x1
	 * Note: if the clobber list changes, adjust the use of
	 * x4 in plat_crash_console_init if necessary.
	 * -----------------------------------------------------
	 */
func bluefield_get_baudrate
	/* Get the console_speed value. */
	mov_imm	x0, TYU_BASE_ADDRESS + TYU_CONSOLE_SPEED_ADDR
	ldr	w0, [x0]
	ubfx	x0, x0, #TYU_CONSOLE_SPEED_SHIFT, #TYU_CONSOLE_SPEED_WIDTH

	/*
	 * See if the console_speed value is 0, in which we use the
	 * default baudrate rather than left shifting 150.
	 */
	cmp	x0, #0
	b.eq	default_baudrate

	/*
	 * We calculate the baudrate by left shifting 150 with
	 * the console_speed value.
	 */
	mov_imm	x1, 150
	lsl	x0, x1, x0
	ret

	/* Use BF_DEFAULT_BAUDRATE as the baud rate. */
default_baudrate:
	mov_imm x0, BF_DEFAULT_BAUDRATE
	ret

endfunc bluefield_get_baudrate

	/* ---------------------------------------------
	 * int plat_crash_console_init(void)
	 * Function to initialize the crash console
	 * without a C Runtime to print crash report.
	 * Clobber list : x0 - x4
	 * ---------------------------------------------
	 */
func plat_crash_console_init
	mov	x4, x30   /* save LR to a register not clobbered by the call */
	bl	bluefield_get_baudrate
	mov	x30, x4
	mov	x2, x0
	mov_imm	x0, CRASH_UART_BASE
	mov_imm	x1, CRASH_UART_CLK_IN_HZ
	b	console_pl011_core_init
endfunc plat_crash_console_init

	/* ---------------------------------------------
	 * int plat_crash_console_putc(int c)
	 * Function to print a character on the crash
	 * console without a C Runtime.
	 * Clobber list : x1, x2
	 * ---------------------------------------------
	 */
func plat_crash_console_putc
	mov_imm	x1, CRASH_UART_BASE
	b	console_pl011_core_putc
endfunc plat_crash_console_putc

	/* ---------------------------------------------
	 * int plat_crash_console_flush()
	 * Function to force a write of all buffered
	 * data that hasn't been output.
	 * Out : return -1 on error else return 0.
	 * Clobber list : r0
	 * ---------------------------------------------
	 */
func plat_crash_console_flush
	mov_imm	x0, CRASH_UART_BASE
	b	console_pl011_core_flush
endfunc plat_crash_console_flush

	/* ---------------------------------------------------------------------
	 * We don't need to carry out any memory initialization on ARM
	 * platforms. The Secure RAM is accessible straight away.
	 * ---------------------------------------------------------------------
	 */
func platform_mem_init
	ret
endfunc platform_mem_init

	/*
	 * void plat_secondary_cold_boot_setup (void);
	 *
	 * This function performs any platform specific actions
	 * needed for a secondary cpu after a cold reset e.g
	 * mark the cpu's presence, mechanism to place it in a
	 * holding pen etc.
	 */
func plat_secondary_cold_boot_setup

	/*
	 * Deactivate the gic cpu interface as well.  (Not sure if we want
	 * or need to do this, but we'll leave it in for now.)
	 */
	/* Check for GICv3 system register access */
	mrs	x0, id_aa64pfr0_el1
	ubfx	x0, x0, #ID_AA64PFR0_GIC_SHIFT, #ID_AA64PFR0_GIC_WIDTH
	cmp	x0, #1
	b.ne	gicv2_bypass_disable

	/* Check for SRE enable */
	mrs	x1, ICC_SRE_EL3
	tst	x1, #ICC_SRE_SRE_BIT
	b.eq	gicv2_bypass_disable

	mrs	x2, ICC_SRE_EL3
	orr	x2, x2, #(ICC_SRE_DIB_BIT | ICC_SRE_DFB_BIT)
	msr	ICC_SRE_EL3, x2
	b	check_enabled

gicv2_bypass_disable:
	mov	w0, #(IRQ_BYP_DIS_GRP1 | FIQ_BYP_DIS_GRP1)
	orr	w0, w0, #(IRQ_BYP_DIS_GRP0 | FIQ_BYP_DIS_GRP0)
	ldr	x1, =BASE_GICC_BASE
	str	w0, [x1, #GICC_CTLR]

check_enabled:
	/* Get the cluster bit this core is on and store to x1. */
	mrs	x0, mpidr_el1
	and	x0, x0, #MPIDR_CLUSTER_MASK
	lsr	x0, x0, #MPIDR_AFFINITY_BITS
	mov_imm x1, 1
	lsl	x1, x1, x0

	/* Get the enabled cluster bit from the RSH_TILE_STATUS register. */
	mov_imm	x0, RSHIM_BASE
	ldr	x0, [x0, #RSH_TILE_STATUS]
	lsr	x0, x0, #RSH_TILE_STATUS__CLUSTER_ENA_SHIFT
	and	x0, x0, #RSH_TILE_STATUS__CLUSTER_ENA_RMASK

	/*
	 * See if the cluster this core is at is enabled; if not, core puts
	 * itself in a wfi loop. Since no interrupts are enabled at this point,
	 * this essentially makes the core disabled.
	 */
	and	x0, x0, x1
	cmp	x0, #0
	b.ne	cb_try

play_dead:
	wfi
	b	play_dead

cb_try:
	/*
	 * Load the rshim scratchpad.  If the low bit is off, it's still
	 * a cold boot, so sleep.
	 */
	mov_imm	x0, RSHIM_BASE
	ldr	x0, [x0, #RSH_SCRATCHPAD4]
	tbz	x0, #0, cb_sleep

	/*
	 * Get our CPU number into x0.  Note that since this routine never
	 * returns, we can call other routines without saving LR.
	 */
	bl	plat_my_core_pos

	/*
	 * The CPU bitmap is right after the 64-bit destination address;
	 * point x2 there, leaving x1 pointing at the destination address.
	 */
	mov_imm	x1, MBOX_BASE
	add	x2, x1, #8

	/*
	 * Take the high bits of our CPU number and use them to load the
	 * proper 64-bit word in the CPU bitmap.
	 */
	lsr	x3, x0, #6
	ldr	x2, [x2, x3, lsl #3]

	/*
	 * Use the low bits of our CPU number to find our bit in the bitmap;
	 * if it's not set yet, sleep.
	 */
	lsr	x2, x2, x0
	tbz	x2, #0, cb_sleep

	/*
	 * Time to go.  Load our destination address from the mailbox and jump
	 * there.
	 */
	ldr	x0, [x1]
	br	x0

cb_sleep:
	/*
	 * Not yet time to go, so wait for an event.  The power control code
	 * will signal one after it sets our bit in the bitmap.  Note that
	 * since signaling an event sets a state bit in the processor, this
	 * works even if the event is signaled after we check but before we
	 * execute the WFE.
	 */
	dsb	sy
	wfe
	b	cb_try

endfunc plat_secondary_cold_boot_setup

	/*
	 * uintptr_t plat_get_my_entrypoint (void);
	 *
	 * The main job of this routine is to distinguish between a cold and
	 * warm boot.  On BlueField, we use a bit in one of the rshim scratchpad
	 * registers for this.
	 *
	 * For a cold boot, return 0.
	 * For a warm boot, read the mailbox and return the address it
	 * contains.
	 */
func plat_get_my_entrypoint
	mov_imm	x0, RSHIM_BASE
	ldr	x0, [x0, #RSH_SCRATCHPAD4]
	tbnz	x0, #0, warm_reset

	/* Cold reset */
	mov	x0, #0
	ret

warm_reset:
	/*
	 * A mailbox is maintained in the trusted SRAM. It is flushed out of
	 * the caches after every update using normal memory so it is safe to
	 * read it here with SO attributes.
	 */
	mov_imm	x0, MBOX_BASE
	ldr	x0, [x0]
	cbz	x0, _panic_handler
	ret

	/*
	 * The rshim indicates this is a warm reset but the mailbox is empty.
	 * This should never happen!
	 */
_panic_handler:
	no_ret	plat_panic_handler
endfunc plat_get_my_entrypoint

	/*
	 * unsigned int plat_is_my_cpu_primary (void);
	 *
	 * Find out whether the current cpu is the primary cpu.
	 */
func plat_is_my_cpu_primary
	mrs	x0, mpidr_el1
	and	x0, x0, #(MPIDR_CLUSTER_MASK | MPIDR_CPU_MASK)
	cmp	x0, #BF_PRIMARY_CPU
	cset	w0, eq
	ret
endfunc plat_is_my_cpu_primary

	/* ---------------------------------------------------------------------
	 * We need to initialize the L2 cache configuration.
	 * ---------------------------------------------------------------------
	 */
func plat_reset_handler
	ldr     x0, =( \
		      (1 << \
		       CORTEX_A72_L2CTLR_ECC_ENABLE_SHIFT) | \
		      (CORTEX_A72_L2_TAG_RAM_SETUP_0_CYCLES << \
		       CORTEX_A72_L2CTLR_TAG_RAM_SETUP_SHIFT) | \
		      (CORTEX_A72_L2_TAG_RAM_LATENCY_2_CYCLES << \
		       CORTEX_A72_L2CTLR_TAG_RAM_LATENCY_SHIFT) | \
		      (CORTEX_A72_L2_DATA_RAM_SETUP_0_CYCLES << \
		       CORTEX_A72_L2CTLR_DATA_RAM_SETUP_SHIFT) | \
		      (CORTEX_A72_L2_DATA_RAM_LATENCY_2_CYCLES << \
		       CORTEX_A72_L2CTLR_DATA_RAM_LATENCY_SHIFT) \
		     )
	msr     CORTEX_A72_L2CTLR_EL1, x0

	/*
	 * Enable the external debug interface for this core as if this is a
	 * warm reboot, the interface was shut down before CPU power off.
	 */
	mov	x0, #0
	msr	osdlr_el1, x0
	isb
	dsb	sy

	ret
endfunc plat_reset_handler
