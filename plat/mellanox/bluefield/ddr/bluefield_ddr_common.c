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

#include <assert.h>
#include <debug.h>
#include <delay_timer.h>
#include <mmio.h>
#include <string.h>
#include "bluefield_ddr.h"
#include "emc.h"
#include "emi.h"
#include "pub.h"

#ifdef ATF_CONSOLE
int ddr_reg_flag = 0;
#endif
/* Flags to decide if we print verbosely and the logs. */
int ddr_verbose_flag = DDR_VERBOSE;
int ddr_error_flag = 1;
int ddr_log_flag = LOG_MEM_TRANSACTION;

void mem_config_ndelay(uint32_t wait)
{
	ndelay(wait);
}

/*
 * Any access to the memory controller register (and device register in general)
 * for BlueField would occur sequentially in program order. We are enforcing
 * transactions in sequence in our mesh network, and for the non-cacheable
 * device memory access, the ARM cores waits for the acknowledgement of the
 * previous transaction before issuing the next one. This means for register
 * indirect accesses after we write the data/address registers, we do not need
 * to do a memory barrier before writing to the command register to issue the
 * indirect transaction.
 */

/* Write data <data> to a 32-bit register at <base>:<id>:<addr>. */
void mem_config_write(uintptr_t base, uint32_t id, uint32_t addr,
		      uint32_t data)
{
	uintptr_t reg_addr = base + (1 << 22) + ((uintptr_t)id << 14) +
			     ((uintptr_t)addr << 2);

	PCI_DUMP("%5d         %c %11.3x %10.3x %15.8x\n", 1, 'W',
		 id, addr, data);
	mmio_write_32(reg_addr, data);
}

/* Read data from a 32-bit register at <base>:<id>:<addr> to return. */
uint32_t mem_config_read(uintptr_t base, uint32_t id, uint32_t addr)
{
	uintptr_t reg_addr = base + (1 << 22) + ((uintptr_t)id << 14) +
			     ((uintptr_t)addr << 2);
	uint32_t data = mmio_read_32(reg_addr);

	PCI_DUMP("%5d         %c %11.3x %10.3x %15.8x\n", 1, 'R',
		 id, addr, data);
	return data;
}

/*
 * Do a read loop using read_func(reg_id), exiting the loop when the return
 * value matches the given exp_val after anding the mask value.
 * read_func	The function to use to do the read.
 * reg_id	The id of the register to read, to be passed to read_func.
 * mask		The mask to be used to AND to the read value before comparing.
 * exp_val	The expected value to be read after masked.
 * delay_max	The max time to delay, a value of 0 means no limit.
 * delay_intvl	The delay time in ns for each time the read doesn't match.
 * read_val	If not null, the place to write the final read value.
 * Returns 1 if the read value matches the given exp_val, else returns 0 if
 * timed out and the value still doesn't matches.
 */
int read_loop(uint32_t (*read_func)(uint32_t), uint32_t reg_id, uint32_t mask,
	      uint32_t exp_val, uint32_t delay_max, uint32_t delay_intvl,
	      uint32_t *read_val)
{
	PCI_DUMP("# Loop w reg %.3x, expect: (val & %.8x) == %.8x\n",
		reg_id, mask, exp_val);

	uint32_t delay = 0;
	uint32_t data = read_func(reg_id);

	while (((data ^ exp_val) & mask) != 0 ) {
		if (delay_max && delay >= delay_max)
			return 0;
		mem_config_ndelay(delay_intvl);
		delay += delay_intvl;
		data = read_func(reg_id);
	}
	if (read_val != NULL)
		*read_val = data;
	return 1;
}

#ifdef ATF_CONSOLE
static const char *get_reg_name(int reg_type, int reg_id)
{
	const char *result = "NO_DEFINITION";
	int idx;

	idx = find_record(reg_type, 1, reg_id, NULL);

	if (idx != -1) {
		result = reg_info[reg_type].reg_records[idx].name;
	} else if (reg_type == PUB_REG) {
		reg_type = PUB_RANK_REG;
		idx = find_record(reg_type, 1, reg_id, NULL);
		if (idx != -1)
			result = reg_info[reg_type].reg_records[idx].name;
	}

	return result;
}
#endif

void emc_write(uint32_t reg_id, uint32_t data)
{
#ifdef ATF_CONSOLE
	if (ddr_reg_flag)
		printf("SET EMC_%s = 0x%.8x\n",
			get_reg_name(EMC_REG, reg_id), data);
#endif
	mem_config_write(dp->mss_addr, EMC_BLOCK_ID, reg_id, data);
}

uint32_t emc_read(uint32_t reg_id)
{
	uint32_t data;

	data = mem_config_read(dp->mss_addr, EMC_BLOCK_ID, reg_id);

#ifdef ATF_CONSOLE
	if (ddr_reg_flag)
		printf("GET EMC_%s = 0x%.8x\n",
			get_reg_name(EMC_REG, reg_id), data);
#endif

	return data;
}

void emi_write(uint32_t reg_id, uint32_t data)
{
	/*
	 * The SPARE register was used as a last minute ECO fix, and since it
	 * was not RTL synthesis there were some limitations and we need to put
	 * the following restrictions on it.
	 */
	if (reg_id == EMI_SPARE && ((data & 0x1f0) >> 4) > 28) {
		ERROR("EMI write abort, writing 0x%x[8:4] > 28 to SPARE\n",
			data);
		return;
	}

#ifdef ATF_CONSOLE
	if (ddr_reg_flag)
		printf("SET EMI_%s = 0x%.8x\n",
			get_reg_name(EMI_REG, reg_id), data);
#endif

	mem_config_write(dp->mss_addr, EMI_BLOCK_ID, reg_id, data);
}

uint32_t emi_read(uint32_t reg_id)
{
	uint32_t data;

	data = mem_config_read(dp->mss_addr, EMI_BLOCK_ID, reg_id);
#ifdef ATF_CONSOLE
	if (ddr_reg_flag)
		printf("GET EMI_%s = 0x%.8x\n",
			get_reg_name(EMI_REG, reg_id), data);
#endif
	return data;
}

/*
 * Do a PHY Utility Block register access using EMEM_MC's indirect command
 * registers.
 * reg_id	The reg id of the PUB register to access.
 * data		Pointer to the place to store the data read./
 * 		Pointer to the place the data to write is stored.
 * mem_id	ID of the access interface to use, should be one of the
 * 		EMC_IND_CMD__MEM_ID_VAL_xxx values.
 * op		The access operation (read/write), should be
 * 		EMC_IND_CMD__OP_VAL_READ/WRITE.
 * Returns 1 if successful or 0 if failed (timeout).
 */
static int pub_access(uint32_t reg_id, uint32_t *data,
		      uint32_t mem_id, uint32_t op)
{
	EMC_IND_CMD_t cmd = {
		.mem_id = mem_id,
		.op = op,
	};

	if (op == EMC_IND_CMD__OP_VAL_WRITE)
		emc_write(EMC_IND_DATA__FIRST_WORD, *data);

	emc_write(EMC_IND_ADDR, reg_id);
	emc_write(EMC_IND_CMD, cmd.word);

	EMC_IND_STS_t sts = {
		.rdy = 1,
		.success = 1
	};
	if (!read_loop(emc_read, EMC_IND_STS, sts.word, sts.word,
		       100 * NS_PER_MS, 100, NULL))
		return 0;

	if (op == EMC_IND_CMD__OP_VAL_READ)
		*data = emc_read(EMC_IND_DATA__FIRST_WORD);

	return 1;
}

/* Write a PHY PUB register using the APB interface. */
void pub_write(uint32_t reg_id, uint32_t data)
{
#ifdef ATF_CONSOLE
	int reg_flag = ddr_reg_flag;

	if (ddr_reg_flag)
		printf("SET PUB_%s = 0x%.8x\n",
			get_reg_name(PUB_REG, reg_id), data);
	ddr_reg_flag = 0;
#endif
	if (!pub_access(reg_id, &data, EMC_IND_CMD__MEM_ID_VAL_APB,
			EMC_IND_CMD__OP_VAL_WRITE))
		MEM_ERR("PUB write with reg_id=0x%x failed.\n", reg_id);
#ifdef ATF_CONSOLE
	ddr_reg_flag = reg_flag;
#endif
}

/* Read a PHY PUB register using the APB interface. */
uint32_t pub_read(uint32_t reg_id)
{
	uint32_t data = 0;

#ifdef ATF_CONSOLE
	int reg_flag = ddr_reg_flag;

	ddr_reg_flag = 0;
#endif

	if (!pub_access(reg_id, &data, EMC_IND_CMD__MEM_ID_VAL_APB,
			EMC_IND_CMD__OP_VAL_READ))
		MEM_ERR("PUB read with reg_id=0x%x failed.\n", reg_id);

#ifdef ATF_CONSOLE
	if (reg_flag)
		printf("GET PUB_%s = 0x%.8x\n",
			get_reg_name(PUB_REG, reg_id), data);

	ddr_reg_flag = reg_flag;
#endif
	return data;
}

/* Write a PHY indirect PUB register using the indirect interface. */
void pub_indirect_write(uint32_t reg_id, uint32_t data)
{
#ifdef ATF_CONSOLE
	int reg_flag = ddr_reg_flag;

	if (ddr_reg_flag)
		printf("SET PUB_IND_%s = 0x%.8x\n",
			get_reg_name(PUB_INDIRECT_REG, reg_id), data);
	ddr_reg_flag = 0;
#endif
	if (!pub_access(reg_id, &data, EMC_IND_CMD__MEM_ID_VAL_PHY,
			EMC_IND_CMD__OP_VAL_WRITE))
		MEM_ERR("PUB indirect write with reg_id=0x%x failed.\n",
			reg_id);
#ifdef ATF_CONSOLE
	ddr_reg_flag = reg_flag;
#endif
}

/* Read a PHY indirect PUB register using the indirect interface. */
uint32_t pub_indirect_read(uint32_t reg_id)
{
	uint32_t data = 0;

#ifdef ATF_CONSOLE
	int reg_flag = ddr_reg_flag;

	ddr_reg_flag = 0;
#endif

	if (!pub_access(reg_id, &data, EMC_IND_CMD__MEM_ID_VAL_PHY,
			EMC_IND_CMD__OP_VAL_READ))
		MEM_ERR("PUB indirect read with reg_id=0x%x failed.\n", reg_id);

#ifdef ATF_CONSOLE
	if (reg_flag)
		printf("GET PUB_IND_%s = 0x%.8x\n",
			get_reg_name(PUB_INDIRECT_REG, reg_id), data);

	ddr_reg_flag = reg_flag;
#endif

	return data;
}

/*
 * op_cmd	Defines the type of command. (DIRECT_ADDR_HOST_OP__READ/WRITE)
 * op_ecc_en	Defines if write/read is with ECC raw data. (boolean)
 * data		Point to the EMI_IND_DATA__LENGTH words of data to write/to
 *		store result for read.
 * ecc_data	Point to the EMI_ADD_IND_DATA__LENGTH words of data used
 *		additionally when doing ECC.
 * op_pr	Defines the applied Physical Rank.
 * op_lr	Defines the applied Logical Rank.
 * op_bg	Defines the applied Bank Group.
 * op_ba	Defines the applied Bank Address.
 * op_col	Defines the applied SDRAM Column Address.
 * op_row	Defines the applied SDRAM Row Address.
 * Returns 1 on success (and write the *data if is a read operation) or
 * 0 if it fails (invalid parameters).
 */
int direct_addr_host_op(uint32_t op_cmd, uint32_t op_ecc_en,
			uint32_t *data, uint32_t *ecc_data,
			uint32_t op_pr, uint32_t op_lr,
			uint32_t op_bg, uint32_t op_ba,
			uint32_t op_col, uint32_t op_row)
{
	/* Before executing, first check the parameters are valid. */
	if ((op_cmd != DIRECT_ADDR_HOST_OP__READ &&
	     op_cmd != DIRECT_ADDR_HOST_OP__WRITE) ||
	    ((op_bg << 2) | op_ba) > dp->ddr_max_bank ||
	    ((op_col << 3) | 0x7) > dp->ddr_max_col ||
	    op_row > dp->ddr_max_row)
		return 0;

	/* Load data to write if command is a write operation. */
	if (op_cmd == 2) {
		for (int i = 0; i < EMI_IND_DATA__LENGTH; i++)
			emi_write(EMI_IND_DATA__FIRST_WORD + i, data[i]);
		if (op_ecc_en) {
			for (int i = 0; i < EMI_ADD_IND_DATA__LENGTH; i++)
				emi_write(EMI_ADD_IND_DATA__FIRST_WORD + i,
					  ecc_data[i]);
		}
	}

	/* Issue the operation. */
	uint32_t temp_addr = (op_row << 7) | op_col;
	uint32_t temp_cmd = op_cmd | (((op_bg << 2) | op_ba) << 8) |
			    (op_pr << 14) | (op_lr << 12) |
			    ((op_ecc_en ? 0xfe : 0xff) << 24);
	emi_write(EMI_IND_ADDR, temp_addr);
	emi_write(EMI_IND_CMD, temp_cmd);

	/* Wait until operation is successful/ */
	EMI_IND_STS_t eis = {
		.rdy = 1,
		.success = 1,
	};
	if (!read_loop(emi_read, EMI_IND_STS, eis.word, eis.word,
			100 * NS_PER_MS, 10, NULL)) {
		MEM_ERR("Direct host operation timed out\n");
		return 0;
	}

	/* Writeback the read data if command is a read operation. */
	if (op_cmd == 1) {
		for (int i = 0; i < EMI_IND_DATA__LENGTH; i++)
			data[i] = emi_read(EMI_IND_DATA__FIRST_WORD + i);
		if (op_ecc_en) {
			for (int i = 0; i < EMI_ADD_IND_DATA__LENGTH; i++)
				ecc_data[i] = emi_read(
					EMI_ADD_IND_DATA__FIRST_WORD + i);
		}
	}

	return 1;
}

/* Use the MRS Interface to do a MRS command. */
void mrs_write(uint32_t mr_rank, uint32_t mr_num, uint32_t mr_data,
	       uint32_t mr_cmd, uint32_t mr_delay, uint32_t mr_last)
{
	uint32_t temp_data_0 = (mr_cmd << 29) | (mr_rank << 25) |
			       (mr_num << 18) | mr_data;
	uint32_t temp_data_1 = (mr_last << 30) | 0x2000000 |
			       (mr_delay << 6) | 1;
	uint32_t temp_cmd = 1;

	emc_write(EMC_IND_DATA__FIRST_WORD, temp_data_0);
	emc_write(EMC_IND_DATA__FIRST_WORD + 1, temp_data_1);
	emc_write(EMC_IND_CMD, temp_cmd);

	EMC_IND_STS_t sts = {
		.rdy = 1,
		.success = 1
	};
	if (!read_loop(emc_read, EMC_IND_STS, sts.word, sts.word,
		       100 * NS_PER_MS, 100, NULL))
		MEM_ERR("mrs write 0x%llx failed\n",
			((uint64_t)temp_data_1 << 32) | temp_data_0);
}

/* MRS operation for RCD side B if accessing a LRDIMM or RDIMM. */
void mrs_write_inv(uint32_t mr_rank, uint32_t mr_num, uint32_t mr_data,
		   uint32_t mr_cmd, uint32_t mr_delay, uint32_t mr_last,
		   uint8_t rc00)
{
	/*
	 * This is described in section 7.3.3 of the DDR initialization spec.
	 * When bit 0 of RC00 is set, bit inversion is enabled when accessing
	 * RCD side B.
	 */
	if ((rc00 & 1) == 0) {
		mr_data = mr_data ^ 0x22bf8;
		mr_num = mr_num ^ 0x7;
	}
	mr_num |= 0x8;
	mrs_write(mr_rank, mr_num, mr_data, mr_cmd, mr_delay, mr_last);
}
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

/*
 * Change dp to point to the current dp corresponding to the given mss_index.
 * Return 1 on success or 0 if failed (corresponding MSS not present).
 */
int ddr_switch_current_mss(int mss_index)
{
	if (mss_index < MAX_MEM_CTRL && dps[mss_index].dimm_num) {
		dp = &(dps[mss_index]);
		return 1;
	}
	ERROR("MSS%d not present.\n", mss_index);
	return 0;
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
		MEM_ERR("%s timed out.\n", operation);
		return 0;
	}

	if (((pgsr0_val & pgsr0_done) == pgsr0_done) &&
	     (pgsr0_val & pgsr0_err) == 0) {
		MEM_VERB("%s finished!\n", operation);
		return 1;
	}

	MEM_ERR("%s ended with pgsr0 = 0x%x.\n", operation, pgsr0_val);

	/* Find and print out all the done bits which are not set. */
	while ((pgsr0_val & pgsr0_done) != pgsr0_done) {
		uint32_t diff_bit = (pgsr0_val & pgsr0_done) ^ pgsr0_done;

		diff_bit = (diff_bit & (diff_bit - 1)) ^ diff_bit;
		MEM_ERR("pgsr0.%s not set\n", find_pgsr0_field(diff_bit));
		pgsr0_val |= diff_bit;
	}

	/* Find and print out all the error bits which are set. */
	while ((pgsr0_val & pgsr0_err) != 0) {
		uint32_t diff_bit = pgsr0_val & pgsr0_err;

		diff_bit = (diff_bit & (diff_bit - 1)) ^ diff_bit;
		MEM_ERR("pgsr0.%s set\n", find_pgsr0_field(diff_bit));
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
		MEM_ERR("Timed out waiting for schcr0.schtrig == 0\n");
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
