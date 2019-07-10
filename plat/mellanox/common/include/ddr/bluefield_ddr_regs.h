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

#ifndef __BLUEFIELD_DDR_REGS_H__
#define __BLUEFIELD_DDR_REGS_H__

/* EMEM MC Indirect registers. */
__extension__
typedef union {
	struct {
		uint32_t	mc_sw_rd_rst		: 1;
		uint32_t	__reserved_0		: 31;
	};
	uint32_t word;
} MC_PUP_CTRL_t;

__extension__
typedef union {
	struct {
		uint32_t	dft_init_start		: 1;
		uint32_t	__reserved_0		: 7;
		uint32_t	dfi_dram_clk_dis	: 2;
		uint32_t	__reserved_1		: 6;
		uint32_t	dfi_data_byte_dis	: 9;
		uint32_t	__reserved_2		: 7;
	};
	uint32_t word;
} PUB_CTRL_t;

__extension__
typedef union {
	struct {
		uint32_t	dfi_init_complete	: 1;
		uint32_t	__reserved_0		: 31;
	};
	uint32_t word;
} PUB_STATUS_t;

/*
 * IFC_BIST_ERR_ADDR.
 * recent address of Bist machine read error decoded by column/row
 */
__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* DRAM column and row of first read error. */
		uint32_t err_col : 7;
		uint32_t err_row : 25;
#else   /* __BIG_ENDIAN__ */
		uint32_t err_row : 25;
		uint32_t err_col : 7;
#endif
	};
	uint32_t word;
} BF_BIST_ERR_ROW_COL_t;

/* EMEM MC Indirect registers. */
#define MC_PUP_CTRL	0x0
#define PUB_CTRL	0x10
#define PUB_STATUS	0x11
#define MC_SPECIAL_CTRL	0xFF

/* Block id of EMEM MI */
#define EMI_BLOCK_ID 0x2
/* Block id of EMEM MC */
#define EMC_BLOCK_ID 0x3

/* Definitions of the RCD control registers. */
#define RC00	0x0000
#define RC01	0x0010
#define RC02	0x0020
#define RC03	0x0030
#define RC04	0x0040
#define RC05	0x0050
#define RC06	0x0060
#define RC07	0x0070
#define RC08	0x0080
#define RC09	0x0090
#define RC0A	0x00A0
#define RC0B	0x00B0
#define RC0C	0x00C0
#define RC0D	0x00D0
#define RC0E	0x00E0
#define RC0F	0x00F0
#define RC1x	0x0100
#define RC2x	0x0200
#define RC3x	0x0300
#define RC4x	0x0400
#define RC5x	0x0500
#define RC6x	0x0600
#define RC7x	0x0700
#define RC8x	0x0800
#define RC9x	0x0900
#define RCAx	0x0A00
#define RCBx	0x0B00
#define RCCx	0x0C00
#define RCDx	0x0D00
#define RCEx	0x0E00
#define RCFx	0x0F00

#define RCD_NUM	31

/* Definitions of the DB control registers. */
#define F0BC00	0x0000
#define F0BC01	0x0010
#define F0BC02	0x0020
#define F0BC03	0x0030
#define F0BC04	0x0040
#define F0BC05	0x0050
#define F0BC06	0x0060
#define F0BC07	0x0070
#define F0BC08	0x0080
#define F0BC09	0x0090
#define F0BC0A	0x00A0
#define F0BC0B	0x00B0
#define F0BC0C	0x00C0
#define F0BC0D	0x00D0
#define F0BC0E	0x00E0
#define F0BC0F	0x00F0
#define FyBC1x	0x0100
#define FyBC2x	0x0200
#define FyBC3x	0x0300
#define FyBC4x	0x0400
#define FyBC5x	0x0500
#define FyBC6x	0x0600
#define FyBC7x	0x0700
#define FyBC8x	0x0800
#define FyBC9x	0x0900
#define FyBCAx	0x0A00
#define FyBCBx	0x0B00
#define FyBCCx	0x0C00
#define FyBCDx	0x0D00
#define FyBCEx	0x0E00
#define FyBCFx	0x0F00

#define DB_NUM		31
#define DBF0_NUM	16
#define DBFx_NUM	15
/* How many function space we have. */
#define DBFS_NUM	7

/* Definitions of EMEM_MC indirect command fields. */
#define EMC_IND_CMD__MEM_ID_VAL_MC	0x0
#define EMC_IND_CMD__MEM_ID_VAL_APB	0x1
#define EMC_IND_CMD__MEM_ID_VAL_IFC	0x2
#define EMC_IND_CMD__MEM_ID_VAL_PHY	0x3
#define EMC_IND_CMD__MEM_ID_VAL_IDRL	0x7

#define EMC_IND_CMD__OP_VAL_READ	0x0
#define EMC_IND_CMD__OP_VAL_WRITE	0x1

/* Definitions of various fields of the mode registers. */
#define MR0_DLL_RESET		0x100
#endif /* __BLUEFIELD_DDR_REGS_H__ */
