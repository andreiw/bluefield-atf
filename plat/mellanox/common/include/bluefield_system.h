/*
 * Copyright (c) 2017, Mellanox Technologies and Contributors.
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
#ifndef __BLUEFIELD_SYSTEM_H__
#define __BLUEFIELD_SYSTEM_H__

#include "bluefield_private.h"

/*
 * This file stores declarations of functions which may be implmented
 * differently on different system.
 */

/* List of all possible supported DDR configuration types, ends will NULL */
extern const struct ddr_config *const ddr_fixed_configs[];
/*
 * Configure particular memory controller indicated by given base address.
 * Also fill out the struct describing the DIMMs corresponding to the
 * controller. Returns 1 if there is memory on this memory controller or 0
 * if the memory controller has no memory or is disabled.
 */
int bf_sys_mem_config(uintptr_t mem_base,
		      struct bf_mem_ctrl_info *mem_ctrl_info,
		      int mem_ctrl_num);

int bf_sys_mem_try_cfg(uintptr_t mem_base,
		       struct bf_mem_ctrl_info *mem_ctrl_info,
		       int mem_ctrl_num);

/* Does the BIST POST test after memory is configured, should panic if fails. */
void bf_sys_post_test(struct bf_mem_info *bmi);

/* Read the SPD of the DIMM inserted. */
int bf_sys_get_spd(uint8_t *spd, int offset, int len, int mss, int dimm);

/* Function where the user overwrites the default DDR parameters. */
int bf_sys_ddr_get_info_user(struct ddr_params *dp);

/*
 * Function which populates information about the board, should be fixed for
 * each platform.
 */
int bf_sys_ddr_get_info_board(struct ddr_params *dp);

void bf_sys_config_setup(void);

/* Set up our simulated interconnect. */
void bf_sys_sim_ic_init(void);
void bf_sys_sim_ic_enable(void);
void bf_sys_sim_ic_disable(void);

/* Initialize the system address map. */
void bf_sys_setup_sam(struct bf_dev_tbl *dev_tbl, uint32_t disabled_devs);

/* Initialize interrupts. */
void bf_sys_setup_interrupts(struct bf_dev_tbl *dev_tbl,
			     uint32_t disabled_devs);

/* Set up memory mapping. */
void bf_sys_setup_pmr(struct bf_dev_tbl *dev_tbl, uint32_t disabled_devs,
		      struct bf_mem_info *bmi, uint8_t options);

/* Set up HNF-specific errata. */
void bf_sys_setup_hnf_errata(struct bf_dev_tbl *dev_tbl,
			     uint32_t disabled_devs);

/* Set up trio memory mapping. */
void bf_sys_setup_trio(struct bf_dev_tbl *dev_tbl, uint32_t disabled_devs);

/* Return 1 if we are the Palladium system. */
int bf_sys_is_pal(void);

/* Return 1 for MBF1M332A and 2 for MBF1L332A SmartNic models.
 * Return 0 for BlueWhale. */
int bf_sys_snic_model(void);

#endif /* __BLUEFIELD_SYSTEM_H__ */
