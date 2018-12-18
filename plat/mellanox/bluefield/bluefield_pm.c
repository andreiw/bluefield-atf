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
#include <assert.h>
#include <debug.h>
#include <errno.h>
#include <mmio.h>
#include <platform.h>
#include <psci.h>
#include "bluefield_def.h"
#include "bluefield_private.h"
#include "bluefield_system.h"
#include "platform_def.h"
#include "rsh_def.h"


/*
 * Macros used to parse state information from State-ID if it is using the
 * recommended encoding for State-ID.
 */
#define BF_LOCAL_PSTATE_WIDTH		4
#define BF_LOCAL_PSTATE_MASK		((1 << BF_LOCAL_PSTATE_WIDTH) - 1)

/* Macros to construct the composite power state */

/* Make composite power state parameter till power level 0 */
#if PSCI_EXTENDED_STATE_ID

#define bluefield_make_pwrstate_lvl0(lvl0_state, pwr_lvl, type) \
		(((lvl0_state) << PSTATE_ID_SHIFT) | \
		 ((type) << PSTATE_TYPE_SHIFT))
#else
#define bluefield_make_pwrstate_lvl0(lvl0_state, pwr_lvl, type) \
		(((lvl0_state) << PSTATE_ID_SHIFT) | \
		((pwr_lvl) << PSTATE_PWR_LVL_SHIFT) | \
		((type) << PSTATE_TYPE_SHIFT))
#endif /* __PSCI_EXTENDED_STATE_ID__ */

/* Make composite power state parameter till power level 1 */
#define bluefield_make_pwrstate_lvl1(lvl1_state, lvl0_state, pwr_lvl, type) \
		(((lvl1_state) << BF_LOCAL_PSTATE_WIDTH) | \
		bluefield_make_pwrstate_lvl0(lvl0_state, pwr_lvl, type))
/*
 *  The table storing the valid idle power states. Ensure that the
 *  array entries are populated in ascending order of state-id to
 *  enable us to use binary search during power state validation.
 *  The table must be terminated by a NULL entry.
 */
const unsigned int bluefield_pm_idle_states[] = {
	/* State-id - 0x01 */
	bluefield_make_pwrstate_lvl1(BF_LOCAL_STATE_RUN, BF_LOCAL_STATE_RET,
			MPIDR_AFFLVL0, PSTATE_TYPE_STANDBY),
	/* State-id - 0x02 */
	bluefield_make_pwrstate_lvl1(BF_LOCAL_STATE_RUN, BF_LOCAL_STATE_OFF,
			MPIDR_AFFLVL0, PSTATE_TYPE_POWERDOWN),
	/* State-id - 0x22 */
	bluefield_make_pwrstate_lvl1(BF_LOCAL_STATE_OFF, BF_LOCAL_STATE_OFF,
			MPIDR_AFFLVL1, PSTATE_TYPE_POWERDOWN),
	0,
};

/* Global flag indicating if the call to power down the CPU is for suspend. */
static int is_suspend;

/*******************************************************************************
 * BlueField handler called when a CPU is about to enter standby.
 ******************************************************************************/
void bluefield_cpu_standby(plat_local_state_t cpu_state)
{

	assert(cpu_state == BF_LOCAL_STATE_RET);

	/*
	 * Enter standby state
	 * dsb is good practice before using wfi to enter low power states
	 */
	dsb();
	wfi();
}

/*******************************************************************************
 * BlueField handler called when a power domain is about to be turned on. The
 * mpidr determines the CPU to be turned on.
 ******************************************************************************/
int bluefield_pwr_domain_on(u_register_t mpidr)
{
	int rc = PSCI_E_SUCCESS;
	int cpu_idx = plat_core_pos_by_mpidr(mpidr);
	uintptr_t *mailbox = (void *) MBOX_BASE;
	uint64_t *cpu_bitmap = (uint64_t *)&mailbox[1];

	/* Set the target CPU's bit in the bitmap. */
	cpu_bitmap[cpu_idx >> 6] |= (1ULL << (cpu_idx & 0x3F));

	/* Make sure everyone can see the bitmap. */
	flush_dcache_range((uintptr_t) &cpu_bitmap[cpu_idx >> 6],
			   sizeof(*cpu_bitmap));

	/* Wake up anyone waiting. */
	dsbsy();
	sev();

	return rc;
}

/*******************************************************************************
 * BlueField handler called when a power domain is about to be turned off. The
 * target_state encodes the power state that each level should transition to.
 ******************************************************************************/
void bluefield_pwr_domain_off(const psci_power_state_t *target_state)
{
	int cpu_idx;
	u_register_t mpidr;
	uintptr_t *mailbox = (void *) MBOX_BASE;
	uint64_t *cpu_bitmap = (uint64_t *)&mailbox[1];
	assert(target_state->pwr_domain_state[BF_PWR_LVL0] ==
					BF_LOCAL_STATE_OFF);

	/*
	 * If execution reaches this stage then this power domain will be
	 * suspended. Perform at least the cpu specific actions followed
	 * by the cluster specific operations if applicable.
	 */
	/* Prevent interrupts from spuriously waking up this cpu */
	bluefield_gic_cpuif_disable();

	/* Turn redistributor off */
	bluefield_gic_redistif_off();

	/* Get the mpidr for this cpu */
	mpidr = read_mpidr_el1();
	cpu_idx = plat_core_pos_by_mpidr(mpidr);
	/* Clear the target CPU's bit in the bitmap. */
	cpu_bitmap[cpu_idx >> 6] &= ~(1ULL << (cpu_idx & 0x3F));

	/* Make sure everyone can see the bitmap. */
	flush_dcache_range((uintptr_t) &cpu_bitmap[cpu_idx >> 6],
			   sizeof(*cpu_bitmap));
}

/*******************************************************************************
 * BlueField handler called to enter the wfi() which would trigger the
 * core to be completely powered off. But as we don't have such a hardware
 * mechanism, we mimic what we do before a core is released and does the wfe().
 ******************************************************************************/
void __dead2 bluefield_pwr_domain_pwr_down_wfi(const psci_power_state_t *target)
{
	int cpu_idx;
	u_register_t mpidr;
	uint32_t sctlr;
	uintptr_t *mailbox = (void *) MBOX_BASE;
	uint64_t *cpu_bitmap = (uint64_t *)&mailbox[1];

	/*
	 * Turn off address translation. When the core boots up again
	 * the initialization code would assert that address translation
	 * is turned off so that it can set it up. But as we aren't really
	 * resetting the core, registers won't be reset to their default
	 * values, and thus we have to turn it off here manually.
	 */
	sctlr = read_sctlr_el3();
	sctlr &= ~SCTLR_M_BIT;
	write_sctlr_el3(sctlr);
	isb();

	/* Get the mpidr for this cpu */
	mpidr = read_mpidr_el1();
	cpu_idx = plat_core_pos_by_mpidr(mpidr);

	if (!is_suspend) {
		/* Wait until we are released again. */
		while((cpu_bitmap[cpu_idx >> 6] &
		       (1ULL << (cpu_idx & 0x3F))) == 0)
			wfe();
	} else {
		/* Wait for a interrupt to unsuspend */
		wfi();
	}

	/* Get address of entrypoint from mailbox and jump to it. */
	(*((void (**)(void))MBOX_BASE))();

	/* We shouldn't ever return. */
	panic();
}

/*******************************************************************************
 * BlueField handler called when a power domain has just been powered on after
 * being turned off earlier. The target_state encodes the low power state that
 * each level has woken up from.
 ******************************************************************************/
void bluefield_pwr_domain_on_finish(const psci_power_state_t *target_state)
{
	assert(target_state->pwr_domain_state[MPIDR_AFFLVL0] ==
					BF_LOCAL_STATE_OFF);

	/* Perform the common cluster specific operations */
	if (target_state->pwr_domain_state[MPIDR_AFFLVL1] ==
					BF_LOCAL_STATE_OFF) {
		/* Enable the simulated interconnect if this cluster was off */
		bf_sys_sim_ic_enable();
	}

	/* Enable the gic cpu interface */
	bluefield_gic_pcpu_init();

	/* Program the gic per-cpu distributor or re-distributor interface */
	bluefield_gic_cpuif_enable();
}

/*******************************************************************************
 * BlueField handlers to shutdown/reboot the system
 ******************************************************************************/
static void __dead2 bluefield_system_off(void)
{
	/* Setup the NVDIMM Save operation. */
	bluefield_setup_nvdimm_save();

	/*
	 * FIXME: once we have a programmatic way to do a power-off,
	 * we should do that here.
	 */
	ERROR("System Off: operation not handled.\n");
	panic();
}

static void __dead2 bluefield_system_reset(void)
{
	/* Setup the NVDIMM Save operation. */
	bluefield_setup_nvdimm_save();

	mmio_write_64(RSHIM_BASE + RSH_RESET_CONTROL,
		      RSH_RESET_CONTROL__RESET_CHIP_VAL_KEY);
	/* Wait for reset to occur. */
	while (1)
		wfi();
}

/*******************************************************************************
 * BlueField platform handler called to check the validity of the power
 * state parameter. The power state parameter has to be a composite power
 * state.
 ******************************************************************************/
int bluefield_validate_power_state(unsigned int power_state,
				psci_power_state_t *req_state)
{
	unsigned int state_id;
	int i;

	assert(req_state);

	/*
	 *  Currently we are using a linear search for finding the matching
	 *  entry in the idle power state array. This can be made a binary
	 *  search if the number of entries justify the additional complexity.
	 */
	for (i = 0; !!bluefield_pm_idle_states[i]; i++) {
		if (power_state == bluefield_pm_idle_states[i])
			break;
	}

	/* Return error if entry not found in the idle state array */
	if (!bluefield_pm_idle_states[i])
		return PSCI_E_INVALID_PARAMS;

	i = 0;
	state_id = psci_get_pstate_id(power_state);

	/* Parse the State ID and populate the state info parameter */
	while (state_id) {
		req_state->pwr_domain_state[i++] = state_id &
						BF_LOCAL_PSTATE_MASK;
		state_id >>= BF_LOCAL_PSTATE_WIDTH;
	}

	return PSCI_E_SUCCESS;
}

/*******************************************************************************
 * BlueField platform handler called to check the validity of the non secure
 * entrypoint.
 ******************************************************************************/
int bluefield_validate_ns_entrypoint(uintptr_t entrypoint)
{
	/*
	 * Check if the non secure entrypoint lies within the non
	 * secure DRAM.
	 */
	if (entrypoint >= NS_DRAM1_BASE)
		return PSCI_E_SUCCESS;

	return PSCI_E_INVALID_ADDRESS;
}

/*******************************************************************************
 * BlueField handler called when a power domain is about to be suspended.
 * The CPU interrupt is NOT turned off as there is no hardware support
 * for wakeup events, and the interrupt is needed to wake up the primary CPU.
 ******************************************************************************/
void bluefield_pwr_domain_suspend(const psci_power_state_t *target_state)
{
	/* Tell bluefield_pwr_domain_pwr_down_wfi that the CPU is suspending. */
	is_suspend = 1;
}

/*******************************************************************************
 * BlueField handler called when a power domain has just been powered on after
 * having been suspended earlier. As nothing was actually suspended, no
 * reinitialize flows needs to be done here.
 ******************************************************************************/
void bluefield_pwr_domain_suspend_finish(const psci_power_state_t *target_state)
{
	is_suspend = 0;
}

void bluefield_get_sys_suspend_power_state(psci_power_state_t *req_state)
{
	for (int i = MPIDR_AFFLVL0; i <= PLAT_MAX_PWR_LVL; i++)
		req_state->pwr_domain_state[i] = BF_LOCAL_STATE_OFF;
}

/*******************************************************************************
 * Export the platform handlers to enable psci to invoke them
 ******************************************************************************/
static const plat_psci_ops_t bluefield_plat_psci_ops = {
	.cpu_standby = bluefield_cpu_standby,
	.pwr_domain_on = bluefield_pwr_domain_on,
	.pwr_domain_off = bluefield_pwr_domain_off,
	.pwr_domain_suspend = bluefield_pwr_domain_suspend,
	.pwr_domain_on_finish = bluefield_pwr_domain_on_finish,
	.pwr_domain_suspend_finish = bluefield_pwr_domain_suspend_finish,
	.pwr_domain_pwr_down_wfi = bluefield_pwr_domain_pwr_down_wfi,
	.system_off = bluefield_system_off,
	.system_reset = bluefield_system_reset,
	.validate_power_state = bluefield_validate_power_state,
	.validate_ns_entrypoint = bluefield_validate_ns_entrypoint,
	.get_sys_suspend_power_state = bluefield_get_sys_suspend_power_state,
};

/*****************************************************************************
 * Export the platform specific psci ops & initialize the bluefield power
 * controller
 *****************************************************************************/
int plat_setup_psci_ops(uintptr_t sec_entrypoint,
				const plat_psci_ops_t **psci_ops)
{
	uintptr_t *mailbox;
	uint64_t *cpu_bitmap;

	*psci_ops = &bluefield_plat_psci_ops;

	/* Program the jump address. */
	mailbox = (void *) MBOX_BASE;
	*mailbox = sec_entrypoint;

	/*
	 * Zero out the CPU bitmap. This is big enough for 128 CPUs; once
	 * that becomes a problem we'll have plenty of other things to
	 * worry about.
	 */
	cpu_bitmap = (uint64_t *)&mailbox[1];
	cpu_bitmap[0] = 0;
	cpu_bitmap[1] = 0;

	flush_dcache_range((uintptr_t) mailbox,
			   (uintptr_t) &cpu_bitmap[2] - (uintptr_t) mailbox);

	/*
	 * Make sure the zeroed bitmap is visible before we say we're on the
	 * warm boot path.
	 */
	dsbsy();

	mmio_write_64(RSHIM_BASE + RSH_SCRATCHPAD4, 1);

	return 0;
}
