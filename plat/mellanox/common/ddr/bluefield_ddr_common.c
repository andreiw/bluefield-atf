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
#include <console.h>
#include <debug.h>
#include <delay_timer.h>
#include <mmio.h>
#include <string.h>
#include "bluefield_ddr.h"
#include "emc.h"
#include "emi.h"

#ifdef ATF_CONSOLE
int ddr_reg_flag = 0;
#endif
/* Flags to decide if we print verbosely and the logs. */
int ddr_verbose_level = DDR_VERBOSE;
int ddr_error_level = 1;
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
		MEM_ERR(1, "PUB write with reg_id=0x%x failed.\n", reg_id);
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
		MEM_ERR(1, "PUB read with reg_id=0x%x failed.\n", reg_id);

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
		MEM_ERR(1, "PUB indirect write with reg_id=0x%x failed.\n",
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
		MEM_ERR(1, "PUB indirect read with reg_id=0x%x failed.\n", reg_id);

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
		MEM_ERR(1, "Direct host operation timed out\n");
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
		MEM_ERR(1, "mrs write 0x%llx failed\n",
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
/*
 * Function used to fill out the bf_mem_ctrl_info struct based on the
 * information from the ddr_params struct.
 */
void bluefield_fill_mem_ctrl_info(struct ddr_params *dp_struct,
				  struct bf_mem_ctrl_info *mem_ctrl_info)
{
	if (dp_struct == NULL)
		return;

	for (int i = 0; i < MAX_DIMM_PER_MEM_CTRL; i++) {
		if (dp_struct->dimm[i].ranks == 0)
			continue;
		mem_ctrl_info->dimm_info[i].size_in_gb =
					dp_struct->dimm[i].capacity_gbit;
		mem_ctrl_info->dimm_info[i].is_nvdimm = dp->dimm[i].is_nvdimm;
		mem_ctrl_info->dimm_info[i].is_single_DRAM =
					dp_struct->type == ONBOARD;
		mem_ctrl_info->dimm_info[i].ranks_num =
					dp_struct->dimm[i].ranks;
		mem_ctrl_info->dimm_info[i].ddr_3ds_num =
					dp_struct->dimm[i].ddr_3ds;
		mem_ctrl_info->dimm_info[i].data_bits = dp->package + 2;
		mem_ctrl_info->dimm_info[i].is_registered =
				dp->type == RDIMM || dp->type == LRDIMM;
		mem_ctrl_info->dimm_info[i].is_load_reduced =
						     dp->type == LRDIMM;
	}
}

/*
 * tf_printf doesn't have the ability to do formatted prints, so we have
 * to implement the format that we want here.
 */
void print_reg(const char *name, uint32_t address, uint32_t val)
{
	int len = 0;

	while (name[len]) {
		console_putc(name[len]);
		len++;
	}

	while (len < 34) {
		console_putc(' ');
		len++;
	}

	for (int i = 0; i < 2; i++) {
		uint32_t data = i ? val : address;
		uint32_t data_alt = data;

		tf_printf(" 0x");

		if (data_alt == 0) {
			tf_printf("00000000");
			continue;
		}

		while (data_alt < 0x10000000) {
			console_putc('0');
			data_alt = data_alt << 4;
		}

		tf_printf("%x", data);
	}

	tf_printf("\n");
}
