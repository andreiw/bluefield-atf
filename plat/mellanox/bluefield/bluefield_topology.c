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
#include <platform_def.h>
#include "bluefield_private.h"

/* The BlueField power domain tree descriptor */
unsigned char bluefield_power_domain_tree_desc[BF_CLUSTER_COUNT + 1];


CASSERT(BF_CLUSTER_COUNT && BF_CLUSTER_COUNT <= 256,
	assert_invalid_bluefield_cluster_count);

/*******************************************************************************
 * This function dynamically constructs the topology according to
 * BF_CLUSTER_COUNT and returns it.
 ******************************************************************************/
const unsigned char *plat_get_power_domain_tree_desc(void)
{
	int i;

	/*
	 * The BlueField power domain tree does not have a single system level
	 * power domain i.e. a single root node. The first entry in the power
	 * domain descriptor specifies the number of power domains at the
	 * highest power level. For the BlueField this is the number of cluster
	 * power domains.
	 */
	bluefield_power_domain_tree_desc[0] = BF_CLUSTER_COUNT;

	for (i = 0; i < BF_CLUSTER_COUNT; i++)
		bluefield_power_domain_tree_desc[i + 1] =
					BF_MAX_CPUS_PER_CLUSTER;

	return bluefield_power_domain_tree_desc;
}

/*******************************************************************************
 * This function returns the core count within the cluster corresponding to
 * `mpidr`.
 ******************************************************************************/
unsigned int bluefield_get_cluster_core_count(u_register_t mpidr)
{
	return BF_MAX_CPUS_PER_CLUSTER;
}

/*******************************************************************************
 * This function validates an MPIDR by checking whether it falls within the
 * acceptable bounds. An error code (-1) is returned if an incorrect mpidr
 * is passed.
 ******************************************************************************/
int bluefield_check_mpidr(u_register_t mpidr)
{
	unsigned int cluster_id, cpu_id;

	mpidr &= MPIDR_AFFINITY_MASK;

	if (mpidr & ~(MPIDR_CLUSTER_MASK | MPIDR_CPU_MASK))
		return -1;

	cluster_id = (mpidr >> MPIDR_AFF1_SHIFT) & MPIDR_AFFLVL_MASK;
	cpu_id = (mpidr >> MPIDR_AFF0_SHIFT) & MPIDR_AFFLVL_MASK;

	if (cluster_id >= BF_CLUSTER_COUNT)
		return -1;

	if (cpu_id >= bluefield_get_cluster_core_count(mpidr))
		return -1;

	return 0;
}

/*******************************************************************************
 * This function implements a part of the critical interface between the psci
 * generic layer and the platform that allows the former to query the platform
 * to convert an MPIDR to a unique linear index. An error code (-1) is returned
 * in case the MPIDR is invalid.
 ******************************************************************************/
int plat_core_pos_by_mpidr(u_register_t mpidr)
{
	if (bluefield_check_mpidr(mpidr) == -1)
		return -1;

	/* FIXME: eventually we should figure out the total number of
	 * processors from the rshim registers as part of the cold boot
	 * flow, store it somewhere, and then return -1 here if the given
	 * mpidr isn't legal.
	 */

	return bluefield_calc_core_pos(mpidr);
}
