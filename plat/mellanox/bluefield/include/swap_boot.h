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

#ifndef __BOOT_SWAP_H__
#define __BOOT_SWAP_H__

/*
 * Pass information to the next boot via the breadcrumb register.
 * The comments are phrased from the reader's point of view, i.e.
 * how to interpret the bits at boot time; the writer should understand
 * the bits as reflecting the requested action after the next reset.
 */

/* Set watchdog timer to this value in seconds, if non-zero. */
#define BREADCRUMB_WDOG_MASK 0xfffUL

/* Should we update the boot mode for the next reset? */
#define BREADCRUMB_SET_NEW_BOOT_MODE 0x1000UL

/* If set_new_boot_action is set, set the boot mode to this value. */
#define BREADCRUMB_NEW_BOOT_MASK 0x3UL
#define BREADCRUMB_NEW_BOOT_SHIFT 13

/* Mask of all bits defined in this header. */
#define BREADCRUMB_ALL_BITS \
	(BREADCRUMB_WDOG_MASK | BREADCRUMB_SET_NEW_BOOT_MODE \
	 | (BREADCRUMB_NEW_BOOT_MASK << BREADCRUMB_NEW_BOOT_SHIFT))

#endif /* __BOOT_SWAP_H__ */
