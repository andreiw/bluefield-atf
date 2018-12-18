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
#ifndef _BIST_H_
#define _BIST_H_

#include "bluefield_ddr.h"

#define BIST_INC_ADDR_1_BYTE		0

#define BIST_SCAN_MODE__LOGICAL_RANK	0
#define BIST_SCAN_MODE__PHYSICAL_RANK	1
#define BIST_SCAN_MODE__ADDRESS		2
#define BIST_SCAN_MODE__RANDOM		3

/* The DIMM interface is 64DQ + 8CB = 72 bits. */
#define DQ_NUM				64
#define CB_NUM				8
#define DATA_QUEUES			(DQ_NUM + CB_NUM)
/* The BIST block size in bytes where BIST error data is stored. */
#define MAX_BIST_BLOCK_SIZE		(int)(sizeof(EMC_IFC_BIST_ERR_DATA_t) *\
					 EMC_IFC_BIST_ERR_DATA__LENGTH)
/* The line width of the SRAM used to store the BIST pattern in bytes. */
#define PATTERN_LINE_LENGTH		36
/* The number of lines we have in the memory controller's SRAM. */
#define MAX_BIST_LINES			256
/* The max size of a BIST pattern that can fit in the SRAM. */
#define BIST_MAX_PATTERN_SIZE		(PATTERN_LINE_LENGTH * MAX_BIST_LINES)
/* The size of the DQ masks. */
#define DQ_MASK_SIZE			(int)(sizeof(EMC_IFC_BIST_MASK_t) *\
					 EMC_IFC_BIST_MASK__LENGTH)
/*
 * How many bits in the EMI interface which are in the same group which have
 * adjacent bit data.
 * */
#define BF_MOD_SIZE			8
/* How many bits are sent each line for a burst when writing one SRAM line. */
#define BF_PER_DQ_BURST_BITS		(BYTES_TO_BITS(PATTERN_LINE_LENGTH) / \
					DATA_QUEUES)
/* The bit index difference in corresponding DQ lines in adjacent groups. */
#define BF_DATA_BUS_WIDTH		(BF_MOD_SIZE * BF_PER_DQ_BURST_BITS)
/* What is the minimum bit when writing a SRAM line which uses the CB lines. */
#define BF_CB_MIN			(DQ_NUM * BF_PER_DQ_BURST_BITS)

/* Macros used to figure out where and what a bit is on the EMI. */
#define BF_BIST_IS_CB(bit_ind)		((bit_ind) >= BF_CB_MIN)

#define BF_BIST_GET_BLOCK_NUM(bit_ind)	((bit_ind) / BF_DATA_BUS_WIDTH)

#define BF_BIST_GET_DQ(bit_ind)		(((bit_ind) % BF_MOD_SIZE) +	\
				(BF_BIST_GET_BLOCK_NUM(bit_ind) * BF_MOD_SIZE))

#define BF_BIST_GET_CB(bit_ind)		(((bit_ind) - BF_CB_MIN) % BF_MOD_SIZE)

#define BF_BIST_NORM_IND(bit_ind)	((bit_ind) % BF_DATA_BUS_WIDTH)

#define GET_BIT(x, pos)			(((x) >> (pos)) & 1)

#define BYTES_TO_BITS(Bytes)		((Bytes) << 3)

#define swap(a, b) \
	do { typeof(a) tmp = (a); (a) = (b); (b) = tmp; } while (0)

#ifdef ATF_CONSOLE
extern int g_debug_level;

 #define dbg_printf(debug_level, ...)			\
	do {						\
		if (g_debug_level > debug_level)	\
			tf_printf(__VA_ARGS__);		\
	} while (0)
#else
 #define dbg_printf(...)
#endif


/*
 * These are the BIST patterns we support; we are using the killer pattern and
 * the init 00 pattern during POST in the regular code flow. All the rest are
 * only used if we are entering BIST commands from the ATF console.
 */
enum bist_pattern_type {
	KILLER_PATTERN = 0,
#ifdef ATF_CONSOLE
	WALKING_1_PATTERN,
	WALKING_0_PATTERN,
	SSO_PATTERN,
	AA_55_PATTERN,
	ADDRESS_PATTERN,
#endif
	INIT_00_PATTERN,
#ifdef ATF_CONSOLE
	INIT_FF_PATTERN,
	RANDOM_PATTERN,
	DQ0_1_010,
	DQ0_8_010,
	DQ0_1_101,
	DQ0_8_101,
	DQ28_1_010,
	DQ28_8_010,
	DQ28_1_101,
	DQ28_8_101,
	DQ53_1_010,
	DQ53_8_010,
	DQ53_1_101,
	DQ53_8_101,
#endif
	BIST_PATTERN_NUM,
};

/*
 * The user is expected to enter 11 for random pattern to be compatible
 * with previous NPS code.
 */
#define BIST_RANDOM_MODE	11
#define PATT_SHIFT		(BIST_RANDOM_MODE - RANDOM_PATTERN)


enum bist_dqmask_state {
	BIST_MASK_UNINITIALIZED = 0,
	BIST_MASK_DEFAULT,
	BIST_MASK_MODIFIED,
};

enum bist_operation_mode {
	COMPARE_ONLY = 0,
	WRITE_ONLY,
	WRITE_COMPARE,
	OP_MODE_NUM
};

/* The value here should match the REP_NUM field of IFC_BIST_CFG register. */
enum bist_repetition_mode {
	ENDLESS = 0,
	SINGLE,
	UNTIL_ERROR,
	REP_MODE_NUM
};

/* Struct used to store the BIST parameters. */
struct bist_parameters {
	/* Mask of the MSS to do the BIST. */
	uint32_t			mc_mask;
	/* Mask of the physical ranks in each MSS to do the BIST. */
	uint32_t			mc_pr_mask[MAX_MEM_CTRL];
	/* Length of the BIST pattern. */
	uint32_t			pattern_length;
	/* Pointer to where the BIST pattern is. */
	uint8_t				*pattern;
	/* Function which can generate the pattern. */
	int				(*get_pattern)(uint8_t *);
	/* Bist repetition mode. */
	enum bist_repetition_mode	rep_mode;
	/* Bist operation mode. */
	enum bist_operation_mode	op_mode;
};

/* Struct used to indicate a pattern with a lot of repetative lines. */
struct bist_line_pattern {
	/* A bitmap of SRAM lines which uses this pattern. */
	uint64_t	use_map[(MAX_BIST_LINES + 8 * sizeof(uint64_t) - 1) /
				(8 * sizeof(uint64_t))];
	uint8_t		line_pattern[PATTERN_LINE_LENGTH];
};

/* Struct used to store a dynamically generated pattern. */
struct bist_genereated_pattern {
	/* Pointer pointing to the pointer which uses this pattern */
	uint8_t **			pattern_ppointer;
	/* Place where the generated pattern is stored. */
	uint8_t				pattern[BIST_MAX_PATTERN_SIZE];
};

#ifdef ATF_CONSOLE
/* The debug verbosity level, default is set to 1, 0 is no output at all. */
extern int g_debug_level;
/* BIST scan mode, only logical/random are currently supported. */
extern int g_scan_mode;
/* BIST random scan mode seed. */
extern int g_bist_rand_scan_seed;
/* Non-zero to limit running BIST up to this address. */
extern int g_bist_high_addr;
/* Non-zero to limit running BIST starting from this address. */
extern int g_bist_low_addr;
#endif

/* Default configuration for all the BIST patterns. */
extern struct bist_parameters bist_configs[BIST_PATTERN_NUM];

/* Function which fills the pattern using the generation function. */
int fill_pattern(enum bist_pattern_type);

int run_default_bist(uint32_t skip_mc_mask, uint32_t mc_pr_mask[MAX_MEM_CTRL]);
int init_ddr_by_bist(uint32_t skip_mc_mask, uint32_t mc_pr_mask[MAX_MEM_CTRL]);
#endif /* _BIST_H_ */
