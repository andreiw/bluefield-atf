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

#ifndef __BLUEFIELD_DDR_H__
#define __BLUEFIELD_DDR_H__

#include <arch.h>
#include <assert.h>
#include <cassert.h>
#include <common_def.h>
#include <debug.h>
#include <mmio.h>
#include <platform_def.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "bluefield_ddr_regs.h"
#include "bluefield_private.h"

/* DDR SPD offset definition. */
#define SPD_NUM_BYTES_OFF			0
#define SPD_REVISION_OFF			1
#define SPD_DRAM_DEVICE_TYPE_OFF		2
#define SPD_MODULE_TYPE_OFF			3
#define SPD_SDRAM_DENSITY_BANKS_OFF		4
#define SPD_VENDOR_ID_OFF			320
#define SPD_DEVICE_ID_OFF			192
#define SPD_REVISION_ID_OFF			349
#define SPD_SUBSYSTEM_VENDOR_ID_OFF		194
#define SPD_SUBSYSTEM_DEVICE_ID_OFF		196
#define SPD_SUBSYSTEM_REVISION_ID_OFF		198
#define SPD_MANUFACTURING_LOCATION_OFF		322
#define SPD_MANUFACTURING_DATA_OFF		323
#define SPD_SERIAL_NUMBER_OFF			325
#define SPD_REGION_FORMAT_INTERFACE_CODE_OFF	204

/* NVDIMM register offset and fields. */

#define NVDIMM_CMD_STATUS0_OFF				0x61
#define NVDIMM_CMD_STATUS0_OPER_IN_PROGRESS		(1 << 0)
#define NVDIMM_CMD_STATUS0_FACTORY_DEFAULT_IN_PROGRESS	(1 << 1)
#define NVDIMM_CMD_STATUS0_CSAVE_IN_PROGRESS		(1 << 2)
#define NVDIMM_CMD_STATUS0_RESTORE_IN_PROGRESS		(1 << 3)
#define NVDIMM_CMD_STATUS0_ERASE_IN_PROGRESS		(1 << 4)
#define NVDIMM_CMD_STATUS0_ABORT_IN_PROGRESS		(1 << 5)
#define NVDIMM_CMD_STATUS0_ARM_IN_PROGRESS		(1 << 6)
#define NVDIMM_CMD_STATUS0_FIRMWARE_OPS_IN_PROGRESS	(1 << 7)

#define NVDIMM_FUNC_CMD_OFF				0x43
#define NVDIMM_FUNC_CMD_START_FACTORY_DEFAULT		(1 << 0)
#define NVDIMM_FUNC_CMD_START_CSAVE			(1 << 1)
#define NVDIMM_FUNC_CMD_START_RESTORE			(1 << 2)
#define NVDIMM_FUNC_CMD_START_ERASE			(1 << 3)
#define NVDIMM_FUNC_CMD_ABORT_CURRENT_OP		(1 << 5)

#define NVDIMM_RESTORE_TIMEOUT0_OFF			0x1C
#define NVDIMM_RESTORE_TIMEOUT1_OFF			0x1D
#define NVDIMM_RESTORE_TIMEOUT1_TIME_UNIT_IN_SECONDS	(1 << 7)

#define NVDIMM_RESTORE_STATUS_OFF			0x66
#define NVDIMM_RESTORE_STATUS_RESTORE_SUCCESS		(1 << 0)
#define NVDIMM_RESTORE_STATUS_RESTORE_ERROR		(1 << 1)
#define NVDIMM_RESTORE_STATUS_ABORT_SUCCESS		(1 << 4)
#define NVDIMM_RESTORE_STATUS_ABORT_ERROR		(1 << 5)

#define NVDIMM_HOST_MAX_OPERATION_RETRY_OFF		0x15
#define NVDIMM_HOST_MAX_OPERATION_RETRY_SAVE		(0x3 << 0)
#define NVDIMM_HOST_MAX_OPERATION_RETRY_RESTORE		(0x3 << 2)
#define NVDIMM_HOST_MAX_OPERATION_RETRY_ERASE		(0x3 << 4)

#define NVDIMM_ARM_TIMEOUT0_OFF				0x20
#define NVDIMM_ARM_TIMEOUT1_OFF				0x21
#define NVDIMM_ARM_TIMEOUT1_TIME_UNIT_IN_SECONDS	(1 << 7)
#define NVDIMM_ARM_CMD_OFF				0x45
#define NVDIMM_ARM_CMD_CSAVE_ON_SAVE_N			(1 << 0)
#define NVDIMM_ARM_STATUS_OFF				0x6A
#define NVDIMM_ARM_STATUS_SUCCESS			(1 << 0)
#define NVDIMM_ARM_STATUS_ERROR				(1 << 1)
#define NVDIMM_ARM_STATUS_SAVE_N_ARMED			(1 << 2)
#define NVDIMM_ARM_STATUS_ABORT_SUCCESS			(1 << 4)
#define NVDIMM_ARM_STATUS_ABORT_ERROR			(1 << 5)

#define NVDIMM_CAPABILITIES1_OFF			0x11
#define NVDIMM_CAPABILITIES1_ATOMIC_ARM_ERASE		(0 << 1)
#define NVDIMM_CAPABILITIES1_ABORT_TIMEOUT_IN_SECONDS	(1 << 1)

#define NVDIMM_ABORT_CMD_TIMEOUT_OFF			0x24

#define NVDIMM_MGT_CMD0_OFF				0x40
#define NVDIMM_MGT_CMD0_CLEAR_CSAVE_STATUS		(1 << 2)
#define NVDIMM_MGT_CMD0_CLEAR_RESTORE_STATUS		(1 << 3)
#define NVDIMM_MGT_CMD0_CLEAR_ERASE_STATUS		(1 << 4)
#define NVDIMM_MGT_CMD0_CLEAR_ARM_STATUS		(1 << 5)

#define NVDIMM_CSAVE_TIMEOUT0				0x18
#define NVDIMM_CSAVE_TIMEOUT1				0x19
#define NVDIMM_CSAVE_TIMEOUT1_TIME_UNIT_IN_SECONDS	(1 << 7)

#define NVDIMM_MAX_RESTORE_DELAY_MS			10000
#define NVDIMM_MAX_RESTORE_TIMEOUT_MS			(5 * 60 * 1000)
#define NVDIMM_MAX_ARM_DELAY_MS				10000
#define NVDIMM_MAX_ARM_TIMEOUT_MS			(5 * 1000)
#define NVDIMM_MAX_SAVE_DELAY_MS			100
#define NVDIMM_MAX_SAVE_TIMEOUT_MS			(5 * 60 * 1000)

#define SPD_SIZE	256

/* We have 8 + 1(ecc) byte lanes. */
#define BYTELANE_NUM					9
#define BIT_ENTRIES_NUM					(BYTELANE_NUM * 8)

/* Given the address of a RCD address, convert it to its array's index. */
#define RCD_IDX(x)	((x < 0x100) ? (x >> 4) : ((x >> 8) + 0xF))

/* Given the address of a DB address, convert it to its array's index. */
#define DB_IDX(x)	((x < 0x100) ? (x >> 4) : ((x >> 8) + 0xF))

#define ARRAYSIZE(x)	(sizeof(x) / sizeof(x[0]))
#define CEIL_DIV(x, y)	(((x) + (y) - 1) / (y))

/*
 * Rounding algorithim for dealing with the limited minimum granularity of
 * timing parameters as specified in the JEDEC SPD SPEC: Add a 97.4%
 * inverse correction factor and then truncate down to the next integer.
 * This is usually used to calculate the cycle value from a timing parameter
 * and the clock period.
 */
#define DDR_CEIL_DIV(x, tck)	((1000 * (x) + 974 * (tck)) / (1000 * (tck)))
/* Delay for at least x ticks. */
#define DELAY_TCK(x, tck)	mem_config_ndelay(CEIL_DIV((x) * (tck), \
						 FS_PER_NS))
#define MAX_ACTIVE_RANKS	4
/* Reverse bitmask for selected rank. */
#define MR_RANK(x)	(((1 << MAX_ACTIVE_RANKS) - 1) & ~(1 << (x)))

#define FS_PER_PS		(1000)
#define FS_PER_NS		(1000 * FS_PER_PS)
#define NS_PER_US		(1000)
#define NS_PER_MS		(1000 * NS_PER_US)
#define FS_PER_MS		((uint64_t)FS_PER_NS * (uint64_t)NS_PER_MS)


/* Set this to 1 to allow verbose message printing. */
#define DDR_VERBOSE		0
/* Set this to 1 to print the pci log dumps. */
#define LOG_MEM_TRANSACTION	0

extern int ddr_verbose_flag;
extern int ddr_error_flag;
extern int ddr_log_flag;
#ifdef ATF_CONSOLE
extern int ddr_reg_flag;
#endif


#define PCI_DUMP(...)			do {	\
	if (ddr_log_flag)			\
		printf(__VA_ARGS__);		\
} while (0)

#define MEM_LOG(...)			do {	\
	if (ddr_verbose_flag || ddr_log_flag)	\
		tf_printf(__VA_ARGS__);		\
} while (0)

#define MEM_ERR(...)			do {	\
	if (ddr_error_flag)			\
		ERROR(__VA_ARGS__);		\
} while (0)

#define MEM_VERB(...)			do {	\
	if (ddr_verbose_flag)			\
		tf_printf(__VA_ARGS__);		\
} while (0)

#ifdef ATF_CONSOLE
  #define ADD_GLOBAL_VAR(name, default_val) int name = default_val;
#else
  #define ADD_GLOBAL_VAR(name, val) static const int name = val;
#endif

/* These are the options for doing direct host access operations. */
#define DIRECT_ADDR_HOST_OP__READ	1
#define DIRECT_ADDR_HOST_OP__WRITE	2

/* Data values for the SDRAM commands using the MRS interface. */
#define	ZQCL_DATA	0x0400
#define ZQCS_DATA	0x0000
/* Issue a SDRAM command using the MRS interface. */
#define MRS_SDRAM_CMD(rank, data)	mrs_write(rank, 0, data, 0x3, 0x4, 1)

/* Retrieve a register's field. */
#define GET_MEM_REG_FIELD(type, name, field)			\
	(((name##_t){ .word = type##_read(name) }).field)
#define GET_MEM_REG_FIELDS(type, name, exp) do {		\
	name##_t reg = { .word = type##_read(name) };		\
	exp;							\
} while(0)
/* Do a read-modify-write of a memory register's field. */
#define SET_MEM_REG_FIELD(type, name, field, data) do {		\
	name##_t reg = { .word = type##_read(name) };		\
	reg.field = data;					\
	type##_write(name, reg.word);				\
} while(0)

#define SET_MEM_REG_FIELDS(type, name, exp) do {		\
	name##_t reg = { .word = type##_read(name) };		\
	exp;							\
	type##_write(name, reg.word);				\
} while(0)

/* Get the address of the DXnXXX reg with the nth byte lane. */
#define ADDR_DXn(reg, n) (PUB_DX0##reg + (n) * (PUB_DX1##reg - PUB_DX0##reg))

/* Retrieve a PUB_DXnXXXX register's field. */
#define GET_PUB_REG_FIELD_DXn(name, byte, field)		\
	(((PUB_DXnname##_t){ .word = pub_read(ADDR_DXn(name, byte) }).field)
/* Set a PUB_DXnXXXX register's field. */
#define SET_PUB_REG_FIELD_DXn(name, byte, field, data) do {	\
	PUB_DXn##name##_t reg = {.word = pub_read(ADDR_DXn(name, byte)) };\
	reg.field = data;					\
	pub_write(ADDR_DXn(name, byte), reg.word);		\
} while(0)
/* Set a PUB_DXnXXXX register's fields. */
#define SET_PUB_REG_FIELDS_DXn(name, byte, exp) do {	\
	PUB_DXn##name##_t reg = {.word = pub_read(ADDR_DXn(name, byte)) };\
	exp;					\
	pub_write(ADDR_DXn(name, byte), reg.word);		\
} while(0)

/* Access the corresponding numbered field of a DXnREG. */
#define GET_RANK_FIELD(var, field, rank)			\
	((rank) == 0 ? var.field##0 :				\
	 (rank) == 1 ? var.field##1 :				\
	 (rank) == 2 ? var.field##2 : var.field##3)
#define SET_RANK_FIELD(var, field, rank, value)			\
	switch (rank) {						\
	case 0:							\
		var.field##0 = value;				\
		break;						\
	case 1:							\
		var.field##1 = value;				\
		break;						\
	case 2:							\
		var.field##2 = value;				\
		break;						\
	default:						\
		var.field##3 = value;				\
		break;						\
	}

enum ddr_regs_type {
	/* External memory controller registers. */
	EMC_REG = 0,
	/* External memory interface registers. */
	EMI_REG,
	/* PHY utility block registers. */
	PUB_REG,
	/* PHY utility block registers that are per rank related. */
	PUB_RANK_REG,
	/* PHY utility block indirect registers. */
	PUB_INDIRECT_REG,
	NUM_DDR_REGS_TYPE
};

enum dimm_type {
	ONBOARD = 0, /* Defines a system with directly mounted SDRAM device. */
	UDIMM,     /* Defines a system with UDIMM DIMMs. */
	RDIMM,	   /* Defines a system with RDIMM DIMMs. */
	LRDIMM,	   /* Defines a system with LRDIMM DIMMs. */
};

/* All the tck speed we support. */
enum ddr_freq {
	DDR4_1600 = 0,
	DDR4_1866,
	DDR4_2133,
	DDR4_2400,
	DDR4_2666,
	NUM_OF_FREQ,
};

/*
 * All the speed bins we currently support, note that ones appended with _E are
 * the enhanced speed bins.
 */
enum ddr_speed_bin {
	DDR4_1600J = 0,
	DDR4_1600K,
	DDR4_1600L,
	DDR4_1866L,
	DDR4_1866M,
	DDR4_1866N,
	DDR4_2133N,
	DDR4_2133P,
	DDR4_2133R,
	DDR4_2400P,
	DDR4_2400R,
	DDR4_2400T,
	DDR4_2400U,
	DDR4_2666T,
	DDR4_2666U,
	DDR4_2666V,
	DDR4_2666W,
	DDR4_1600K_E,
	DDR4_1866M_E,
	DDR4_2133P_E,
	DDR4_2400T_E,
	NUM_OF_SPEED_BIN,
	GET_VAL_FROM_SPD,
};

enum ddr_density {
	DENSITY_2Gbit = 0,
	DENSITY_4Gbit,
	DENSITY_8Gbit,
	DENSITY_16Gbit,
	NUM_DENSITY,
};

enum ddr_package {
	PACKAGE_x4 = 0,
	PACKAGE_x8,
	PACKAGE_x16,
	NUM_PACKAGE,
};

enum ddr_rzq {
	DISABLED = 0,
	RZQ_DIV_1,
	RZQ_DIV_2,
	RZQ_DIV_3,
	RZQ_DIV_4,
	RZQ_DIV_5,
	RZQ_DIV_6,
	RZQ_DIV_7,
	RZQ_DIV_8,
	HIGH_Z,
	NUM_RZQ,
};

/* Basic information we keep for every memory register for debugging purpose. */
struct reg_record {
	/* Name of the register (without the common prefix). */
	const char *name;
	/*
	 * The address (id) of the register that can be used by the
	 * corresponding read/write functions.
	 */
	uint32_t address;
};

/* Struct with information about a type of DDR regs for debugging purpose. */
struct reg_type_record {
	/* Prefix for this type of register. */
	const char *prefix;
	/* Read function to read this kind of register. */
	uint32_t (*reg_read)(uint32_t id);
	/* Write function to write this kind of register. */
	void (*reg_write)(uint32_t id, uint32_t data);
	/* How many registers we have of this type. */
	unsigned int reg_num;
	/* Pointer to array of data about each register. */
	const struct reg_record *reg_records;
	/* Place used to recored/restore the values of the registers. */
	uint32_t *reg_data[MAX_MEM_CTRL];
};

/*
 * Fields which are relavent per DIMM. Yes, we do support DIMMs on the same
 * memory controller with different ranks and capacity.
 */
struct dimm_params {
	/* Number of ranks for this DIMM. */
	uint8_t ranks;

	/* Nonzero if this DIMM is a nonvolatile DIMM device. */
	uint8_t is_nvdimm;

	/*
	 * 1 if not 3DS DDR SDRAM device, how many dies on a 3DS DDR SDRAM
	 * device (2H, 4H or 8H (8H currently not supported)).
	 */
	uint8_t ddr_3ds;

	/* Module DRAM capacity in gigabits (not Gigabytes). */
	uint16_t capacity_gbit;

	/* DDR SDRAM component capacity. */
	enum ddr_density density;

	/* Values of the RCD control registers (populated during setup). */
	uint8_t rcd_regs[RCD_NUM];

	/* Values of the DB control registers (populated during setup). */
	uint8_t db_regs[DBF0_NUM + DBFx_NUM * DBFS_NUM];

	/* Raw SPD data. */
	uint8_t spd[SPD_SIZE];
};

/*
 * Fields which are relavent per MSS. Most of them are timing parameters
 * the memory controller is using which is valid for all the DIMMs attached
 * to it, but it also contains other information like board parameters and
 * debug options.
 */
struct ddr_params {
	/* The base address of the MSS this struct corresponds to. */
	uintptr_t mss_addr;

	/* Which MSS this struct is corresponds to. */
	int mss_index;

	/* What DIMM type the attached DIMM is (or directly mounted SDRAM). */
	enum dimm_type type;

	/* The number of dimms attached. */
	int dimm_num;

	/* Parameters for each specific DIMM. */
	struct dimm_params dimm[MAX_DIMM_PER_MEM_CTRL];

	/* The speed bin for the current dimm, should be GET_VAL_FROM_SPD if
	 * we are reading the SPD data instead for the values.
	 */
	enum ddr_speed_bin speed_bin;

	/* DDR SDRAM component package configuration. (x4, x8 or x16.) */
	enum ddr_package package;

	/*
	 * 1 if not 3DS DDR SDRAM device, how many dies on a 3DS DDR SDRAM
	 * device (2H, 4H or 8H (8H currently not supported)). Max of the two
	 * values for the two dimms.
	 */
	uint8_t ddr_3ds;

	/* DDR interface operational clock, femtoseconds. */
	uint64_t tck;

	/* Minimum allowed DDR interface operational clock, femtoseconds. */
	uint64_t tck_min;

	/* Maximum allowed DDR interface operational clock, femtoseconds. */
	uint64_t tck_max;

	/*
	 * Maximum allowed DDR frequency in kilohertz, the frequency limitation
	 * is due to the SoC's back-end timing closure at tape out and might be
	 * lower than the nominated frequency for the DIMM. In this case we need
	 * lower the running frequency to this allowed frequency.
	 */
	uint64_t ddr_max_freq_khz;

	/* Bitmask of which ranks are mirrored. */
	uint32_t system_address_mirror;

	/* DDR Debug & SI Functionality Enablers */
	/* DDR SDRAM Write DBI operation Enable. */
	uint8_t wr_dbi_en : 1;
	/* DDR SDRAM Read DBI operation Enable. */
	uint8_t rd_dbi_en : 1;
	/* DDR SDRAM Write DBI operation Enable. */
	uint8_t parity_en : 1;
	/* DDR SDRAM Write Data CRC operation Enable. */
	uint8_t crc_en : 1;

	/* Modules Analog Parameters. */
	enum ddr_rzq mem_rtt_nom;
	enum ddr_rzq mem_rtt_wr;
	enum ddr_rzq mem_rtt_park;
	enum ddr_rzq mem_odic;
	uint8_t asr_en;
	uint32_t mem_vref;

	/* DDR PHY Module Parameters. */
	enum ddr_rzq phy_wr_drv;
	enum ddr_rzq phy_rd_odt;
	uint32_t phy_rd_vref;
	uint64_t phy_rtd; //Stored in femtoseconds

	/* Board Specific Structure Parameters. */
	char *sys_name;
	uint32_t ddr_tcase; //Stored in Celsius
	uint64_t ddr_clk[2]; //Stored in femtoseconds
	uint64_t ddr_addr_ctrl; //Stored in femtoseconds
	uint32_t *sysrtd;
	/* Write Leveling and Read DQS gate LCDL Post Training Normalization. */
	uint8_t wlrdqsg_lcdl_norm;
	/* DDR PHY VREF Training Bypass control. */
	uint8_t vref_train_bypass;
	/* DDR PHY HOST Read VREF Training Enable control. */
	uint8_t vref_train_host_en;
	/* DDR PHY SDRAM Memory VREF Training Enable control. */
	uint8_t vref_train_mem_en;
	/* DDR PHY VREF Training using Per DRAM Addressability Model. */
	uint8_t vref_train_pdaen;
	/* DDR PHY VREF HOST Training results of Min/Max values v.s. Setup. */
	uint8_t vref_train_hres2;
	/* DDR PHY VREF DRAM Training results using both Ranges. */
	uint8_t vref_train_dqres2;
	/* Device reference clock that drives the DDR PLL. */
	uint32_t ddr_clk_ref;

	/*
	 * Speed bin values read directly from the SPD. All values
	 * are stored in femtoseconds.
	 */
	uint64_t trcd;
	uint64_t trp;
	uint64_t tras;
	uint64_t trc;
	uint64_t tccd_l;
	uint64_t trrd_s;
	uint64_t trrd_l;
	uint64_t tfaw;
	uint64_t trfc;
	uint64_t trfc1;
	uint64_t trfc2;
	uint64_t trfc4;
	uint64_t twtr_fs;
	uint64_t twtr_s_fs;
	uint64_t twtr_l_fs;
	uint64_t twr;
	uint64_t taa;
	/*
	 * Bitmap of supported CL, if bit n is set then it means CL value
	 * of n is supported.
	 */
	uint64_t cl_map;
	uint8_t cl;		/* Stored in number of TCKs. */

	/*
	 * Values calculated using the other values. All values unless
	 * otherwise indicated are stored in femtoseconds.
	 */
	enum ddr_freq freq;
	uint64_t twtr;		/* Stored in number of TCKs. */
	uint64_t twtr_s;	/* Stored in number of TCKs. */
	uint64_t twtr_l;	/* Stored in number of TCKs. */
	uint64_t pl;		/* Stored in number of TCKs. */
	uint64_t tccd_l_slr;	/* Stored in number of TCKs. */
	uint64_t tccd_dlr;	/* Stored in number of TCKs. */
	uint64_t tccd_l_slr_fs;
	uint64_t tccd_dlr_fs;
	uint64_t trrd_s_slr;
	uint64_t trrd_l_slr;
	uint64_t tfaw_slr;
	uint64_t tcke;
	uint64_t txp;
	uint64_t twlo;
	uint64_t tdqsck;
	uint64_t tdllk;		/* Stored in number of TCKs. */
	uint64_t trfc_dlr1;
	uint64_t trfc_dlr2;
	uint64_t trfc_dlr4;
	uint64_t trefi;
	uint64_t tccd_s;
	uint64_t tccd_s_slr;
	uint64_t trrd_dlr;
	uint64_t tfaw_dlr;
	uint64_t trtp;
	uint64_t tmrd;
	uint64_t tmod;
	uint64_t twlmrd;
	uint64_t txs;
	uint64_t trtodt;
	uint64_t trtw;
	uint64_t tzqcs;
	uint8_t cwl;		/* Stored in number of TCKs. */
	uint8_t al;		/* Stored in number of TCKs. */

	/*
	 * Original values that we stored backup for. This is used as these
	 * values were bounded by the tck value, as the tck value changes
	 * these values might change value. But if the tck value is changed
	 * again we want them to get their previous value and not bounded
	 * value.
	 */
	uint64_t bak_tfaw;
	uint64_t bak_tccd_l;
	uint64_t bak_tccd_l_slr_fs;
	uint64_t bak_tccd_dlr_fs;
	uint64_t bak_trrd_s;
	uint64_t bak_trrd_l;
	uint64_t bak_trrd_s_slr;
	uint64_t bak_trrd_l_slr;
	uint64_t bak_tcke;
	uint64_t bak_txp;

	uint32_t ddr_max_bank;
	uint32_t ddr_max_col;
	uint32_t ddr_max_row;
	uint8_t rcd_cs_mode;
	uint8_t rcd_dimm_type;
	uint8_t rcd_addr_lat;
	uint8_t rdimm_ca_latency;
	uint8_t rdimm_rd_wr_gap;
	uint8_t mc_3ds_al_add;
	uint8_t bank_rdy_pre_rff;

	/*
	 * This value should be set in the initial configuration process so
	 * subsequent configurations know that a configuration has been
	 * done previously.
	 */
	uint8_t configured;
	/* Additional values calculated and used when doing the memory setup. */

	/* Bit i of this word indicate that rank i is active. */
	uint32_t active_ranks;
	/* Initial VREF DQ ISEL value calculated when setting PUB registers. */
	uint32_t vrefdq_isel_val;
	/* Values calculated during the VREF stage. */
	uint32_t res_dxngcr5[MAX_ACTIVE_RANKS][BYTELANE_NUM];
	uint32_t res_dxngcr6[MAX_ACTIVE_RANKS][BYTELANE_NUM];
	uint32_t res_dxngcr8[MAX_ACTIVE_RANKS][BYTELANE_NUM];
	uint32_t res_dxngcr9[MAX_ACTIVE_RANKS][BYTELANE_NUM];
	uint32_t res_hvmin[MAX_ACTIVE_RANKS][BYTELANE_NUM];
	uint32_t res_hvmax[MAX_ACTIVE_RANKS][BYTELANE_NUM];
	uint32_t res_hvmin_x4[MAX_ACTIVE_RANKS][BYTELANE_NUM];
	uint32_t res_hvmax_x4[MAX_ACTIVE_RANKS][BYTELANE_NUM];
	uint32_t res_dvmin[MAX_ACTIVE_RANKS][BYTELANE_NUM];
	uint32_t res_dvmax[MAX_ACTIVE_RANKS][BYTELANE_NUM];
	uint32_t res_dvmin_x4[MAX_ACTIVE_RANKS][BYTELANE_NUM];
	uint32_t res_dvmax_x4[MAX_ACTIVE_RANKS][BYTELANE_NUM];
	uint32_t res_mr6_vref[MAX_ACTIVE_RANKS][BYTELANE_NUM];
	uint32_t res_mr6_vref_x4[MAX_ACTIVE_RANKS][BYTELANE_NUM];

	/* Values of the MRS mode registers. */
	uint32_t mr_regs[MAX_ACTIVE_RANKS][7];
};

/* The place where we store the DDR parameters for the current setup. */
extern struct ddr_params *dp;
/* Struct storing the ddr parameters. */
extern struct ddr_params dps[MAX_MEM_CTRL];
/*
 * Simple way to access the RCD register array with the number of the RCD reg.
 * For example RC(5x, 0) would be dp->dimm[0].rcd_regs[RCD_IDX(RC5x)].
 */
#define RC(x, y)	(dp->dimm[y].rcd_regs[RCD_IDX(RC ## x)])
/*
 * Simple way to access the DB register array with the number of the DB reg.
 * For example FyBC(6x) would be db_regs[DB_IDX(FyBC6x)].
 */
#define F0BC(x, y)	(dp->dimm[y].db_regs[DB_IDX(F0BC ## x)])
/* fs is for function space. */
#define FyBC(fs, x, y)	(dp->dimm[y].db_regs[fs * DBFx_NUM + DB_IDX(FyBC ## x)])
/* Simple way to access the MRS mode registers array. */
#define MR(x, y)	(dp->mr_regs[(y)][(x)])

/*
 * Lookup tables used to get the timing for a particular frequency/speed bin.
 * All values unless indicated are stored in femtoseconds.
 */
extern const uint64_t sbin_trcd[NUM_OF_SPEED_BIN];
extern const uint64_t sbin_trp[NUM_OF_SPEED_BIN];
extern const uint64_t sbin_tras[NUM_OF_FREQ];
extern const uint64_t sbin_trc[NUM_OF_SPEED_BIN];
extern const uint64_t sbin_tccd_l[NUM_OF_FREQ];
extern const uint64_t sbin_trrd_s_512b[NUM_OF_FREQ];
extern const uint64_t sbin_trrd_s_1kb[NUM_OF_FREQ];
extern const uint64_t sbin_trrd_s_2kb[NUM_OF_FREQ];
extern const uint64_t sbin_trrd_l_512b[NUM_OF_FREQ];
extern const uint64_t sbin_trrd_l_1kb[NUM_OF_FREQ];
extern const uint64_t sbin_trrd_l_2kb[NUM_OF_FREQ];
extern const uint64_t sbin_tfaw_512b[NUM_OF_FREQ];
extern const uint64_t sbin_tfaw_1kb[NUM_OF_FREQ];
extern const uint64_t sbin_tfaw_2kb[NUM_OF_FREQ];
/* PL stored in number of tcks. */
extern const uint64_t sbin_pl[NUM_OF_FREQ];
extern const uint64_t sbin_tccd_l_slr[NUM_OF_FREQ];
extern const uint64_t sbin_tccd_dlr[NUM_OF_FREQ];
extern const uint64_t sbin_trrd_s_slr[NUM_OF_FREQ];
extern const uint64_t sbin_trrd_l_slr[NUM_OF_FREQ];
extern const uint64_t sbin_tfaw_slr_512b[NUM_OF_FREQ];
extern const uint64_t sbin_tfaw_slr_1kb[NUM_OF_FREQ];
extern const uint64_t sbin_tfaw_slr_2kb[NUM_OF_FREQ];
extern const uint64_t sbin_tcke[NUM_OF_FREQ];
extern const uint64_t sbin_txp[NUM_OF_FREQ];
extern const uint64_t sbin_twlo[NUM_OF_FREQ];
extern const uint64_t sbin_tdqsck[NUM_OF_FREQ];
/* tdllk stored in number of tcks. */
extern const uint64_t sbin_tdllk[NUM_OF_FREQ];
extern const uint64_t sbin_trfc[NUM_DENSITY];
extern const uint64_t sbin_trfc1[NUM_DENSITY];
extern const uint64_t sbin_trfc2[NUM_DENSITY];
extern const uint64_t sbin_trfc4[NUM_DENSITY];
extern const uint64_t sbin_trfc_dlr1[NUM_DENSITY];
extern const uint64_t sbin_trfc_dlr2[NUM_DENSITY];
extern const uint64_t sbin_trfc_dlr4[NUM_DENSITY];
extern const uint64_t sbin_trefi[NUM_DENSITY];
/* All CL/CWL values are stored in number of tcks. */
extern const uint8_t sbin_cl_1_500[NUM_OF_SPEED_BIN];
extern const uint8_t sbin_cl_1_250[NUM_OF_SPEED_BIN];
extern const uint8_t sbin_cl_1_071[NUM_OF_SPEED_BIN];
extern const uint8_t sbin_cl_0_937[NUM_OF_SPEED_BIN];
extern const uint8_t sbin_cl_0_833[NUM_OF_SPEED_BIN];
extern const uint8_t sbin_cl_0_750[NUM_OF_SPEED_BIN];
extern const uint8_t sbin_cwl[NUM_OF_FREQ];
/* Stored in the max row size value. */
extern const uint32_t sbin_ddr_max_row[NUM_DENSITY][NUM_PACKAGE];

/* Things that are used only when the console is turned on. */
#ifdef ATF_CONSOLE
extern const struct reg_type_record reg_info[NUM_DDR_REGS_TYPE];

int find_record(int reg_type, int addr_mode, int address, char *name);
#endif

int ddr_get_info(struct ddr_params *dp, uintptr_t mss_addr, int mss_num);
void mem_config_ndelay(uint32_t wait);
void mem_config_write(uintptr_t base, uint32_t id, uint32_t addr,
		      uint32_t data);
uint32_t mem_config_read(uintptr_t base, uint32_t id, uint32_t addr);
int read_loop(uint32_t (*read_func)(uint32_t), uint32_t reg_id, uint32_t mask,
	      uint32_t exp_val, uint32_t delay_max, uint32_t delay_intvl,
	      uint32_t *read_val);
void emc_write(uint32_t reg_id, uint32_t data);
uint32_t emc_read(uint32_t reg_id);
void emi_write(uint32_t reg_id, uint32_t data);
uint32_t emi_read(uint32_t reg_id);
void pub_write(uint32_t reg_id, uint32_t data);
uint32_t pub_read(uint32_t reg_id);
void pub_indirect_write(uint32_t reg_id, uint32_t data);
uint32_t pub_indirect_read(uint32_t reg_id);
int direct_addr_host_op(uint32_t op_cmd, uint32_t op_ecc_en,
			uint32_t *data, uint32_t *ecc_data,
			uint32_t op_pr, uint32_t op_lr,
			uint32_t op_bg, uint32_t op_ba,
			uint32_t op_col, uint32_t op_row);
void mrs_write(uint32_t mr_rank, uint32_t mr_num, uint32_t mr_data,
	       uint32_t mr_cmd, uint32_t mr_delay, uint32_t mr_last);
void mrs_write_inv(uint32_t mr_rank, uint32_t mr_num, uint32_t mr_data,
		   uint32_t mr_cmd, uint32_t mr_delay, uint32_t mr_last,
		   uint8_t rc00);
void pub_switch_rankidr(unsigned int rank_num, int is_write, int is_read);
int ddr_switch_current_mss(int mss_index);
int pir_cmd(uint32_t pir_val, uint32_t timeout, const char *operation,
	    uint32_t pgsr0_done, uint32_t pgsr0_err);
void mr6_get_vrefdq(uint32_t vref, uint32_t *vrefdq_tr_val,
		    uint32_t *vrefdq_tr_rng);
int ddr_phy_scheduler_single_op(uint32_t schcr0, uint32_t schcr1);
int ddr_phy_scheduler_single_nop(uint32_t schcr0, uint32_t schcr1);
int get_host_vref_dq_val(int vrefdq_isel);
int get_host_vrefdq_isel(int phy_rd_vref);
int vref_rank_avg_restore(int rank_idx);
/*
 * Perform Per-DRAM_Addressability VREF Setup on the SDRAM.
 * Returns 1 if successful or 0 if failed.
 */
int per_dram_addressability_vref_setup(int rank);

/* Default DDR analog parameter setup function. */
int bf_sys_ddr_get_info_board_default(struct ddr_params *dp);

/* API to read SPD from EEPROM.
 * Return: 0 (success), others (fail)
 */
int bluefield_ddr_read_spd(uint32_t dimm, uint8_t *buf, uint32_t offset,
			   uint32_t len);

/* API to read register (one byte) at specific offset.
 * Return: 0 (success), others (fail)
 */
int bluefield_ddr_read_reg(uint32_t dimm, uint8_t *value, uint32_t offset);

/* API to write register (one byte) at specific offset.
 * Return: 0 (success), others (fail)
 */
int bluefield_ddr_write_reg(uint32_t dimm, uint8_t value, uint32_t offset);

#endif /* __BLUEFIELD_DDR_H__ */
