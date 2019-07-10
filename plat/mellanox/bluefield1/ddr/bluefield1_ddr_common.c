/*
 * Copyright (c) 2019, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <console.h>
#include <debug.h>
#include <delay_timer.h>
#include <mmio.h>
#include <string.h>
#include "bluefield_ddr.h"
#include "emc.h"
#include "emi.h"
#include "pub.h"


/*
 * Modify the RANKIDR PUB register to indicate which rank (or DIMM if we are
 * accessing the RDIMMCRx registers) we are going to access for read/write.
 */
void pub_switch_rankidr(unsigned int rank_num, int is_write, int is_read)
{
	if (!is_write && !is_read)
		return;

	assert(rank_num < 16); /* We only support max of 16 ranks (with 3DS). */
	PUB_RANKIDR_t rankidr = { .word = pub_read(PUB_RANKIDR) };

	if (is_write)
		rankidr.rankwid = rank_num;
	if (is_read)
		rankidr.rankrid = rank_num;
	pub_write(PUB_RANKIDR, rankidr.word);
}


static const char *find_pgsr0_field(uint32_t val)
{
#define ADD_VAL(field)			\
	if (val == ((PUB_PGSR0_t){.field=1}).word)	\
		return #field;

	ADD_VAL(aplock)
	ADD_VAL(srderr)
	ADD_VAL(cawrn)
	ADD_VAL(caerr)
	ADD_VAL(weerr)
	ADD_VAL(reerr)
	ADD_VAL(wderr)
	ADD_VAL(rderr)
	ADD_VAL(wlaerr)
	ADD_VAL(qsgerr)
	ADD_VAL(wlerr)
	ADD_VAL(zcerr)
	ADD_VAL(verr)
	ADD_VAL(vdone)
	ADD_VAL(srddone)
	ADD_VAL(cadone)
	ADD_VAL(wedone)
	ADD_VAL(redone)
	ADD_VAL(wddone)
	ADD_VAL(rddone)
	ADD_VAL(wladone)
	ADD_VAL(qsgdone)
	ADD_VAL(wldone)
	ADD_VAL(didone)
	ADD_VAL(zcdone)
	ADD_VAL(dcdone)
	ADD_VAL(pldone)
	ADD_VAL(idone)

	return "??";
}

/*
 * Issue a PHY DDR training command and wait until it completes.
 * Return error if timed out, the expected done bits are not set
 * or the error bits are set from the PGSR0 register.
 * pir_val	The value to set to PIR to initiate the operation
 * timeout	The maximum time to wait in nanoseconds before timeout
 * operation	Description of the operation, used when printing error/verbose
 * pgsr0_done	Bitmap of bits expected to be set when idone is set
 * pgsr0_err	Bitmap of bits expected to be unset when idone is set
 * Return 1 on success or 0 if failed.
 */
int pir_cmd(uint32_t pir_val, uint32_t timeout, const char *operation,
	    uint32_t pgsr0_done, uint32_t pgsr0_err)
{
	uint32_t pgsr0_val;
	/* Wait on idone = 1 for the operation to finish. */
	PUB_PGSR0_t pgsr0 = { .idone = 1 };

	/* Start by issuing the command */
	pub_write(PUB_PIR, pir_val);

	/* Wait until the command finished and read back the pgsr0 value. */
	if (!read_loop(pub_read, PUB_PGSR0, pgsr0.word, pgsr0.word,
		       timeout, MAX(1u, timeout / 100), &pgsr0_val)) {
		MEM_ERR(1, "%s timed out.\n", operation);
		return 0;
	}

	if (((pgsr0_val & pgsr0_done) == pgsr0_done) &&
	     (pgsr0_val & pgsr0_err) == 0) {
		MEM_VERB(2, "%s finished!\n", operation);
		return 1;
	}

	MEM_ERR(1, "%s ended with pgsr0 = 0x%x.\n", operation, pgsr0_val);

	/* Find and print out all the done bits which are not set. */
	while ((pgsr0_val & pgsr0_done) != pgsr0_done) {
		uint32_t diff_bit = (pgsr0_val & pgsr0_done) ^ pgsr0_done;

		diff_bit = (diff_bit & (diff_bit - 1)) ^ diff_bit;
		MEM_ERR(1, "pgsr0.%s not set\n", find_pgsr0_field(diff_bit));
		pgsr0_val |= diff_bit;
	}

	/* Find and print out all the error bits which are set. */
	while ((pgsr0_val & pgsr0_err) != 0) {
		uint32_t diff_bit = pgsr0_val & pgsr0_err;

		diff_bit = (diff_bit & (diff_bit - 1)) ^ diff_bit;
		MEM_ERR(1, "pgsr0.%s set\n", find_pgsr0_field(diff_bit));
		pgsr0_val &= ~diff_bit;
	}

	return 0;
}

/*
 * Perform DDR PHY Scheduler Single operation.
 * Returns 1 if successful or 0 if the wait for schtrg to be cleared timed out.
 */
int ddr_phy_scheduler_single_op(uint32_t schcr0, uint32_t schcr1)
{
	SET_MEM_REG_FIELD(pub, PUB_PGCR1, pubmode, 0x1);
	mem_config_ndelay(100);
	pub_write(PUB_SCHCR1, schcr1);
	pub_write(PUB_SCHCR0, schcr0);
	mem_config_ndelay(100);
	SET_MEM_REG_FIELD(pub, PUB_SCHCR0, schtrig, 0x1);

	PUB_SCHCR0_t schcr0_msk = { .schtrig = ~0 };
	PUB_SCHCR0_t schcr0_exp = { .schtrig = 0 };

	if (!read_loop(pub_read, PUB_SCHCR0, schcr0_msk.word, schcr0_exp.word,
		       1 * NS_PER_US, 10, NULL)) {
		MEM_ERR(1, "Timed out waiting for schcr0.schtrig == 0\n");
		return 0;
	}
	SET_MEM_REG_FIELD(pub, PUB_PGCR1, pubmode, 0x0);
	mem_config_ndelay(100);

	return 1;
}

/*
 * Perform DDR PHY Scheduler Single NOP operation.
 * Returns 1 if successful or 0 if the wait for schtrg to be cleared timed out.
 */
int ddr_phy_scheduler_single_nop(uint32_t schcr0, uint32_t schcr1)
{
	schcr0 = 0;
	schcr1 = schcr1 & 0xf0000000;
	return ddr_phy_scheduler_single_op(schcr0, schcr1);
}


/*
 * Function to dump all the PUB register values. Essentially the same result as
 * the ATF "pub_dump" console command but can work without the console enabled.
 */
void pub_dump(void)
{
	uint32_t rankidr_orig = pub_read(PUB_RANKIDR);

	for (int j = 0; j < reg_info[PUB_REG].reg_num; j++)
		print_reg(reg_info[PUB_REG].reg_records[j].name,
			  reg_info[PUB_REG].reg_records[j].address,
			  pub_read(reg_info[PUB_REG].reg_records[j].address));

	for (int r = 0; r < MAX_ACTIVE_RANKS; r++) {
		if ((dp->active_ranks & (1 << r)) == 0)
			continue;
		tf_printf("Rank%d:\n", r);
		pub_switch_rankidr(r, 1, 1);
		for (int j = 0; j < reg_info[PUB_RANK_REG].reg_num; j++)
			print_reg(reg_info[PUB_RANK_REG].reg_records[j].name,
				  reg_info[PUB_RANK_REG].reg_records[j].address,
				  pub_read(reg_info[PUB_RANK_REG].
					   reg_records[j].address));
	}
	pub_write(PUB_RANKIDR, rankidr_orig);
}

/*
 * Do a PHY BIST, write 0 to results if the BIST pass or non-zero if the BIST
 * fails. Returns 0 if the BIST was executed successful or -1 if timed out
 * waiting for BIST to complete.
 */
int phy_bist_run(uint32_t *results)
{
	/*
	 * By default phy_bist_loop is set to one and we just try to do
	 * the PHY BIST once before deciding if it passes or fails.
	 * But in cases where we want to catch flakiness in the result,
	 * we can increase phy_bist_loop and repeat the PHY BIST run
	 * for several times before we declare that it actually passes.
	 */
	for (int i = 0; i < phy_bist_loop; i++) {
		const uint32_t seed = 0x1234ABCD;
		PUB_PGCR0_t pgcr0;
		PUB_BISTRR_t bistrr;
		PUB_BISTGSR_t bistgsr;

		bistrr.word = pub_read(PUB_BISTRR);
		bistrr.binst = 0x2;
		pub_write(PUB_BISTRR, bistrr.word);
		mem_config_ndelay(100);

		bistrr.binst = 0x3;
		pub_write(PUB_BISTRR, bistrr.word);
		mem_config_ndelay(100);

		pgcr0.word = pub_read(PUB_PGCR0);
		pgcr0.phyfrst = 0;
		pub_write(PUB_PGCR0, pgcr0.word);
		mem_config_ndelay(500);

		pgcr0.phyfrst = 1;
		pub_write(PUB_PGCR0, pgcr0.word);
		mem_config_ndelay(100);

		pub_write(PUB_BISTLSR, seed);
		mem_config_ndelay(100);

		bistrr.binst = 0x1;
		pub_write(PUB_BISTRR, bistrr.word);

		bistgsr = (PUB_BISTGSR_t){
			.bdone = 1,
		};
		if (!read_loop(pub_read, PUB_BISTGSR, bistgsr.word,
			       bistgsr.word, 10 * NS_PER_US, 1 * NS_PER_US,
			       &bistgsr.word)) {
			ERROR("phy_bist_run : retries timed out\n");
			return -1;
		}
		bistrr.binst = 0x2;
		pub_write(PUB_BISTRR, bistrr.word);

		bistgsr.word = pub_read(PUB_BISTGSR);
		*results = bistgsr.bdxerr;

		/* Break on failure. */
		if (*results)
			break;
	}
	return 0;
}
