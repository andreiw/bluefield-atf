/*
 * Copyright (c) 2018, Mellanox Technologies and Contributors.
 * All rights reserved.
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

#include "bluefield_private.h"

/*
 * We can't configure the actual hardware in the simulator as the model is not
 * implemented in fast models.
 */

void bf_sys_setup_sam(struct bf_dev_tbl *dev_tbl, uint32_t disabled_devs)
{
}

void bf_sys_setup_interrupts(struct bf_dev_tbl *dev_tbl, uint32_t disabled_devs)
{
}

void bf_sys_setup_pmr(struct bf_dev_tbl *dev_tbl, uint32_t disabled_devs,
		      struct bf_mem_info *bmi, uint8_t options)
{
}

void bf_sys_setup_hnf_errata(struct bf_dev_tbl *dev_tbl, uint32_t disabled_devs)
{
}

void bf_sys_setup_trio(struct bf_dev_tbl *dev_tbl, uint32_t disabled_devs)
{
}

#if TRUSTED_BOARD_BOOT
void bluefield_auth_hca_firmware(void)
{
}
#endif
