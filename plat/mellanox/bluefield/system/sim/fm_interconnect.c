/*
 * Copyright (c) 2013-2015, ARM Limited and Contributors. All rights reserved.
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
#include <arch_helpers.h>
#include <ccn.h>
#include <mmio.h>
#include <platform_def.h>
#include "bluefield_def.h"

/*******************************************************************************
 * CCN512 related constants
 ******************************************************************************/
#define CCN_BASE			0x13000000
#define CCN_512_RNF_IDS			0, 11, 12, 14, 15, 17, \
					18, 29, 30, 32, 33, 35

#if DEBUG
#include "../../../../../drivers/arm/ccn/ccn_private.h"
#endif

static const unsigned char coreID_2_nodeID_map[] = {
	CCN_512_RNF_IDS
};

static ccn_desc_t ccn_info = {
	BF_CLUSTER_COUNT,
	coreID_2_nodeID_map,
	CCN_BASE
};

void bf_sys_sim_ic_init(void)
{
	/*
	 * Initialize CCN-512 driver
	 */
#if DEBUG
	/*
	 * Update the num_masters parameter in debug mode in order to pass
	 * an assertion that checks the cores connected, needed for the bf2
	 * and bf4 models to work
	 */
	ccn_info.num_masters = count_set_bits(
			mmio_read_64(CCN_BASE + MN_RNF_NODEID_OFFSET));
#endif
	ccn_init(&ccn_info);
}

void bf_sys_sim_ic_enable(void)
{
	ccn_enter_snoop_dvm_domain(1 << (MPIDR_AFFLVL1_VAL(read_mpidr())));
}

void bf_sys_sim_ic_disable(void)
{
	ccn_exit_snoop_dvm_domain(1 << (MPIDR_AFFLVL1_VAL(read_mpidr())));
}
