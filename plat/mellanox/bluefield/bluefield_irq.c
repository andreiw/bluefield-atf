/*
 * Copyright (c) 2015, ARM Limited and Contributors. All rights reserved.
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

#include <errno.h>
#include <gicv3.h>
#include <platform.h>
#include <platform_def.h>
#include <interrupt_mgmt.h>
#include <smcc_helpers.h>
#include <debug.h>
#include "bluefield_private.h"

#define BLUEFIELD_IRQ_MAX	64

static struct irq_handler {
	void *arg;
	bluefield_irq_handler handler;
} irq_tbl[BLUEFIELD_IRQ_MAX];

extern void gicd_set_isenabler(uintptr_t base, unsigned int id);
extern void gicd_set_icenabler(uintptr_t base, unsigned int id);

void bluefield_irq_enable(unsigned int id, int enable)
{
	if (enable)
		gicd_set_isenabler(BASE_GICD_BASE, id);
	else
		gicd_set_icenabler(BASE_GICD_BASE, id);
}

static uint64_t irq_handler(uint32_t irq, uint32_t flags,
			    void *handle, void *cookie)
{
	uint64_t ret = 0;

	/* Get the interrupt id of the signaled Group 1 interrupt. */
	irq = read_icc_iar1_el1();

	if (irq_tbl[irq].handler != NULL) {
		ret = irq_tbl[irq].handler(irq, irq_tbl[irq].arg);
		write_icc_eoir1_el1(irq);
	}

	return ret;
}

int bluefield_irq_register(int irq, void *arg, bluefield_irq_handler handler)
{
	if (irq >= BLUEFIELD_IRQ_MAX)
		return -EINVAL;
	if (irq_tbl[irq].handler != NULL)
		return -EEXIST;
	irq_tbl[irq].arg = arg;
	dsb();
	irq_tbl[irq].handler = handler;
	return 0;
}

int bluefield_irq_unregister(int irq, bluefield_irq_handler handler)
{
	if (irq >= BLUEFIELD_IRQ_MAX)
		return -EINVAL;
	if (irq_tbl[irq].handler != handler)
		return -EEXIST;
	irq_tbl[irq].handler = NULL;
	return 0;
}

void bluefield_irq_init(void)
{
	int rc;
	uint32_t flags = 0;

	set_interrupt_rm_flag(flags, NON_SECURE);
	rc = register_interrupt_type_handler(INTR_TYPE_S_EL1,
					     irq_handler,
					     flags);
	if (rc)
		panic();
}
