/*
 * Copyright (c) 2018, Mellanox Technologies. All rights reserved.
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

#ifndef __IO_FLASH_HELPER_ASM__
#define __IO_FLASH_HELPER_ASM__

#include <arch.h>
#include <asm_macros.S>
#include <platform_def.h>
#include "../bluefield_def.h"
#include "rsh_def.h"
#include <io_flash.h>

#ifdef FLASH_ENGINE_ENABLED

	.globl	flash_io_engine_ep

    /*
     * void flash_io_engine_go_to_standby (void);
     *
     * This function must be called to restore the state of the
     * secondary cpu after using it to execute specific tasks.
     * This code is identical to BL1 code where CPUs are waiting
     * for events to jump to their entry point.
     */
func flash_io_engine_go_to_standby

sb_try:
	/*
	 * Load the rshim scratchpad.  If the low bit is off, it's still
	 * a cold boot, so sleep.
	 */
	mov_imm	x0, RSHIM_BASE
	ldr	x0, [x0, #RSH_SCRATCHPAD4]
	tbz	x0, #0, sb_sleep

	/* Set our CPU number into x0. */
	mov_imm	x0, FLASH_ENGINE_ID

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
	tbz	x2, #0, sb_sleep

	/*
	 * Time to go.  Load our destination address from the mailbox and jump
	 * there.
	 */
	ldr	x0, [x1]
	br	x0

sb_sleep:
	/*
	 * Not yet time to go, so wait for an event.  The power control code
	 * will signal one after it sets our bit in the bitmap.  Note that
	 * since signaling an event sets a state bit in the processor, this
	 * works even if the event is signaled after we check but before we
	 * execute the WFE.
	 */
	dsb	sy
	wfe
	b	sb_try

endfunc flash_io_engine_go_to_standby

	/*
	 * Flash engine enty point.
	 */
func flash_io_engine_ep
	/*
	 * For the current engine, first sets the stack
	 * pointer to a stack allocated in SRAM.
	 */
	mov	x9, x30 // lr
	mov_imm	x0, FLASH_ENGINE_STACK_START
	mov	sp, x0

	/*
	 * Do work; read data from the SPI flash. Data
	 * is copied to the SRAM.
	 */
	bl  flash_io_engine_do_read

	/*
	 * Go back to standby mode if there all work is
	 * done.
	 */
	b  flash_io_engine_go_to_standby
endfunc flash_io_engine_ep

#endif /* FLASH_ENGINE_ENABLED */
#endif /* __IO_FLASH_HELPER_ASM__ */
