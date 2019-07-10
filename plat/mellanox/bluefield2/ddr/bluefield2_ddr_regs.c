/*
 * Copyright (c) 2019, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bluefield_ddr.h"
#include "bluefield_private.h"
#include "emc.h"
#include "emi.h"

#define ADD_RECORD(type, reg)	{#reg, type##_##reg}
#define ADD_RECORD_ARRAY(type, reg, index)	\
			{#reg"["#index"]", type##_##reg##__FIRST_WORD + index}

static const struct reg_record pub_indirect_record[] = {
	{"MC_PUP_CTRL", MC_PUP_CTRL},
};

static const struct reg_record emc_record[] = {
	ADD_RECORD_ARRAY(EMC, IND_DATA, 0),
};

static const struct reg_record emi_record[] = {
	ADD_RECORD_ARRAY(EMI, IND_DATA, 0),
};

#define PUB_INDIRECT_REG_NUM	ARRAYSIZE(pub_indirect_record)
#define EMC_REG_NUM		ARRAYSIZE(emc_record)
#define EMI_REG_NUM		ARRAYSIZE(emi_record)

#define PUB_RANK_REGS		(MAX_RANKS_PER_MEM_CTRL * PUB_RANK_REG_NUM)

static uint32_t pub_indirect_data[MAX_MEM_CTRL][PUB_INDIRECT_REG_NUM];
static uint32_t emc_data[MAX_MEM_CTRL][EMC_REG_NUM];
static uint32_t emi_data[MAX_MEM_CTRL][EMI_REG_NUM];

#define ADD_REG_TYPE(TYPE, type)				\
	[TYPE##_REG] = {					\
		.prefix = #TYPE,				\
		.reg_read = type##_read,			\
		.reg_write = type##_write,			\
		.reg_num = TYPE##_REG_NUM,			\
		.reg_records = type##_record,			\
		.reg_data = {type##_data[0], type##_data[1]},	\
}

const struct reg_type_record reg_info[NUM_DDR_REGS_TYPE] = {
	ADD_REG_TYPE(PUB_INDIRECT, pub_indirect),
	ADD_REG_TYPE(EMC, emc),
	ADD_REG_TYPE(EMI, emi),
};

