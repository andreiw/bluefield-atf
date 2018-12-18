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
#include <context.h>
#include <cortex_a72.h>
#include <gicv2.h>
#include <gicv3.h>
#include <platform_def.h>
#include "../bluefield_def.h"

	.global efi_info
	.globl	nvdimm_ars_irq_handler_begin
	.globl	nvdimm_ars_irq_handler_end
	.globl	nvdimm_dcnt0_irq_handler_begin
	.globl	nvdimm_dcnt0_irq_handler_end

func serror_handler64
	/*
	 * Currently serror is only enabled for ARS (address range scrub),
	 * which is triggered from interrupt handler. In such case, the stack
	 * register (sp) points to the runtime stack (CTX_RUNTIME_SP) which
	 * could be used to push new variables.
	 *
	 * In this implementation, x4~x7 are saved on the stack and restore
	 * them on return. The code needs to be revised if serror is reused
	 * for other purposes.
	 */
	sub	sp, sp, #32
	stp	x4, x5, [sp, #0]
	stp	x6, x7, [sp, #16]

	/*
	 * NVDIMM ARS (address range scrub) check.
	 *
	 * Check if the return address is from nvdimm_ars_irq_handler() or
	 * nvdimm_dcnt0_irq_handler().
	 */
	mrs	x4, elr_el3
	adr	x5, nvdimm_ars_irq_handler_begin
	adr	x6, nvdimm_ars_irq_handler_end
	cmp	x4, x5
	b.ls	serror_source_check_cont
	cmp	x4, x6
	b.ls	serror_source_check_done

serror_source_check_cont:
	adr	x5, nvdimm_dcnt0_irq_handler_begin
	adr	x6, nvdimm_dcnt0_irq_handler_end
	cmp	x4, x5
	b.ls	serror_check_done
	cmp	x4, x6
	b.hi	serror_check_done

serror_source_check_done:
	/* Found serror triggered by ARS. */

	/* Set ars.serror = 1. */
	adr	x5, efi_info
	ldr	x4, [x5]
	add	x4, x4, #NVDIMM_ARS_OFF
	mov	w5, #1
	strb	w5, [x4, #2]

	/* Ignore the error. Restore the registers/stack and return. */
	ldp	x4, x5, [sp, #0]
	ldp	x6, x7, [sp, #16]
	add	sp, sp, #32

	eret

serror_check_done:
	/* Restore the registers and stack. */
	ldp	x4, x5, [sp, #0]
	ldp	x6, x7, [sp, #16]
	add	sp, sp, #32

	b	report_unhandled_exception
endfunc serror_handler64
