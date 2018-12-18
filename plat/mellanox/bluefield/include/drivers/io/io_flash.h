/*
 * Copyright (c) 2018, Mellanox Technologies and Contributors.
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

#ifndef __IO_FLASH_H__
#define __IO_FLASH_H__

#ifndef __IO_FLASH_HELPER_ASM__
#include <assert.h>
#include <debug.h>
#include <mmio.h>
#include <platform_def.h>
#include <stdint.h>
#include "rsh.h"

#include "bluefield_def.h"

enum flash_io_ret_value {
	FLASH_IO_SUCCESS = 0,
	FLASH_IO_ERR_SCRATCH_SETUP,
	FLASH_IO_ERR_INTEGRITY_CHECK,
	FLASH_IO_ERR_HASH_GET,
	FLASH_IO_ERR_DATA_GET,
	FLASH_IO_ERR_SIG_MISMATCH,
	FLASH_IO_ERR_NO_DATA,
	FLASH_IO_ERR_UNKNOWN
};

struct flash_dtoc_entry {
	uint32_t size : 24;	/* Directly count size in bytes */
	uint32_t type : 8;
	uint32_t param0;
	uint32_t param1;
	uint32_t __reserved_1[2];
	uint32_t flash_addr : 31; /* Directly count address in bytes */
	uint32_t is_rel	: 1;
	uint32_t section_crc : 16;
	uint32_t no_crc : 1;
	uint32_t dev_data : 1;
	uint32_t in_section : 1;
	uint32_t __reserved_2 : 12;
	uint32_t entry_crc : 16;
	uint32_t __reserved_3 : 16;
};

struct flash_dtoc_centent {
	uint32_t signature[4];
	uint32_t version : 8;
	uint32_t __reserved_0 : 24;
	uint32_t __reserved_1[2];
	uint32_t crc : 16;
	uint32_t __reserved_2 : 16;
	struct flash_dtoc_entry entries[0];
};

#endif /* __IO_FLASH_HELPER_ASM__ */

//#define FLASH_ENGINE_ENABLED

/*
 * SPI Flash Image Information
 */

#define FLASH_IMAGE_SIZE            (16 * 1024 * 1024)      /* 16MB */
#define FLASH_BOOT_IMAGE_SIZE       (4 * 1024 * 1024)       /* 4MB */
#define FLASH_PAGE_SIZE             (64 * 1024)             /* 64KB */

#define FLASH_IMAGE_SIGNATURE_SIZE  16
#define FLASH_IMAGE_SIG_WORD_MAX    (FLASH_IMAGE_SIGNATURE_SIZE >> 2)

#define FLASH_IMAGE_1_ADDR  0x0
#define FLASH_IMAGE_2_ADDR  (FLASH_IMAGE_1_ADDR + FLASH_BOOT_IMAGE_SIZE)
#define FLASH_IMAGE_3_ADDR  (FLASH_IMAGE_2_ADDR + FLASH_BOOT_IMAGE_SIZE)

#define FLASH_SECTION_IMAGE_INFO    0x10

#define IMAGE_INFO_SECURE_FW_IS_SET(val) \
	((((val) >> 15) & 0x1) && (((val) >> 14) & 0x1) && (((val) >> 8) & 0x1))

#define FLASH_IMAGE_HMAC_SIGNATURE_SIZE     (512  >> 3)     /* 512  bits */
#define FLASH_IMAGE_HMAC_DATA_BLOCK_SIZE    (1024 >> 3)     /* 1024 bits */
#define FLASH_IMAGE_BLOCK_SIZE              \
	(FLASH_IMAGE_HMAC_DATA_BLOCK_SIZE << 5)  /* 4K or may be 64K? */
#define FLASH_IMAGE_BLOCK_MASK              (FLASH_IMAGE_BLOCK_SIZE - 1)

/* Secure NIC firmware image inludes the raw data and the digest. */
#define FLASH_IMAGE_DATA_SIZE \
	(FLASH_BOOT_IMAGE_SIZE - FLASH_IMAGE_HMAC_SIGNATURE_SIZE)

/* iTOC Information */
#define FLASH_SECTION_ITOC_SHIFT            9
#define FLASH_SECTION_ITOC_PAD              (4 * 1024)      /* 4KB */
#define FLASH_SECTION_ITOC_PAD_MAX          \
	(FLASH_SECTION_ITOC_PAD << FLASH_SECTION_ITOC_SHIFT)
#define FLASH_SECTION_ITOC_ENTRY_SIZE       32
#define FLASH_SECTION_ITOC_ENTRY_WORD_MAX   \
	(FLASH_SECTION_ITOC_ENTRY_SIZE >> 2)

#define ITOC_ENTRY_INFO_IDX         0
#define ITOC_ENTRY_FLASH_ADDR_IDX   5

#define ITOC_ENTRY_GET_TYPE(val)        (((val) >> 24) & 0x000000ff)
#define ITOC_ENTRY_GET_SIZE(val)        (((val) >>  2) & 0x003fffff)
#define ITOC_ENTRY_GET_FLASH_ADDR(val)  (((val) >>  2) & 0x1fffffff)

#define ITOC_ENTRY_TYPE_HMAC        0xa5
#define DTOC_ENTRY_TYPE_MFG_INFO    0xe0
#define DTOC_ENTRY_TYPE_VPD_RO      0xe3

/* Must be consistent with entry size 2^5 = 32 bytes */
#define ITOC_ENTRY_MAX              (FLASH_SECTION_ITOC_PAD >> 5)

/* dTOC should be located at the last section of the flash */
#define FLASH_DTOC_ADDR             (FLASH_IMAGE_SIZE - FLASH_SECTION_ITOC_PAD)

/*
 * Flash Scratchpad Configuration
 */

/*
 * Use the space reserved for BL31 to load the data to authenticate and
 * its corresponding signature.
 */
#define FLASH_SCRATCHPAD_BASE   BL31_BASE
#define FLASH_SCRATCHPAD_SIZE   MAX_BL31_SIZE

#define FLASH_ALIGN(x, align)   (((x) + (align) - 1) & ~((align) - 1))

#define FLASH_SCRATCHPAD_DATA_SIZE  \
	FLASH_ALIGN(FLASH_IMAGE_BLOCK_SIZE, 4)

/*
 * Partition the flash scratchpad:
 *
 * Allocate the bottom low addresses to store the image signature and the
 * top high addresses to store the chunks. That way, if by accident we run
 * out of space we would never overlap the signature section.
 */
#define FLASH_SCRATCHPAD_SIG_BASE   FLASH_SCRATCHPAD_BASE
#define FLASH_SCRATCHPAD_SIG_SIZE      \
	FLASH_ALIGN(FLASH_IMAGE_HMAC_SIGNATURE_SIZE, 4)

#ifndef FLASH_ENGINE_ENABLED
#define FLASH_SCRATCHPAD_DATA_BASE     \
	(FLASH_SCRATCHPAD_SIG_BASE + FLASH_SCRATCHPAD_SIG_SIZE)

#define FLASH_SCRATCHPAD_DATA_SIZE_MAX \
	(FLASH_SCRATCHPAD_SIZE - FLASH_SCRATCHPAD_SIG_SIZE)

#define FLASH_SCRATCHPAD_DATA_CNT      \
	(FLASH_SCRATCHPAD_DATA_SIZE_MAX / FLASH_SCRATCHPAD_DATA_SIZE)
#else
/*
 * In order to parallelize the hmac calculation and the copy of the FW
 * image, we split the area where to store chunks into two segements;
 * when one segment is being read, the other segment is being written.
 * So allocate the bottom low addresses to store the segment header and
 * the segment data. The upper high addresses would contain the stack of
 * the secondary core, i.e. flash engine.
 */
#define FLASH_SCRATCHPAD_SEGMENT_CNT    2

#define FLASH_ENGINE_STACK_SIZE    (32 * 1024) /* 32KB */

#define FLASH_SCRATCHPAD_DATA_HDR_BASE      \
	FLASH_ALIGN(FLASH_SCRATCHPAD_SIG_BASE + FLASH_SCRATCHPAD_SIG_SIZE, 256)

#define FLASH_SCRATCHPAD_DATA_HDR_SIZE      \
	(FLASH_SCRATCHPAD_SEGMENT_CNT * 64) /* Cache line aligned */

#define FLASH_SCRATCHPAD_DATA_BASE          FLASH_ALIGN( \
	FLASH_SCRATCHPAD_DATA_HDR_BASE + FLASH_SCRATCHPAD_DATA_HDR_SIZE, 4096)

#define FLASH_SCRATCHPAD_DATA_SIZE_MAX      \
	(FLASH_SCRATCHPAD_SIZE - FLASH_SCRATCHPAD_SIG_SIZE -	\
	 FLASH_SCRATCHPAD_DATA_HDR_SIZE - FLASH_ENGINE_STACK_SIZE)

#define FLASH_SCRATCHPAD_DATA_CNT           \
	(FLASH_SCRATCHPAD_DATA_SIZE_MAX / FLASH_SCRATCHPAD_DATA_SIZE)

#define FLASH_SCRATCHPAD_SEGMENT_DATA_CNT   (FLASH_SCRATCHPAD_DATA_CNT >> 1)

#define FLASH_SCRATCHPAD_SEGMENT_DATA_SIZE  \
	(FLASH_SCRATCHPAD_SEGMENT_DATA_CNT * FLASH_SCRATCHPAD_DATA_SIZE)
#endif

/*
 * YU CRspace Information.
 */

/* YU base address */
#define YU_BASE_ADDR                    0xdd802f0000ULL

/* Boot Image Information registers in YU */
#define YU_BOOT_IMAGE_ERR_OFF           0x0e8
#define YU_BOOT_IMAGE_START_OFF         0x0c0
#define YU_BOOT_IMAGE_ITOC_OFF          0x0d8

#define YU_BOOT_IMAGE_ERR_MASK          0x3 /* Boot image error bitmask */

/*
 * SPI Flash Controller registers in YU. The SPI Flash is read and written
 * via an SPI Flash gateway accessible via the device’s CR space.
 */
#define YU_FLASH_CONTROLLER_GW_OFF              0x400
#define YU_FLASH_CONTROLLER_ADDR_OFF            0x404
#define YU_FLASH_CONTROLLER_DATA_OFF            0x410
#define YU_FLASH_CONTROLLER_REP_EN_OFF          0x420
#define YU_FLASH_CONTROLLER_SHADOW_DATA_OFF     0x430

#define YU_FLASH_CONTROLLER_DATA_SIZE           16 /* in bytes */

/* SPI Flash controller gateway command bits */
#define YU_FLASH_CONTROLLER_GW_LOCK         0x80000000
#define YU_FLASH_CONTROLLER_GW_BUSY         0x40000000
#define YU_FLASH_CONTROLLER_GW_TURN_AR_SIZE 0x30000000 /* 8 cycles */
#define YU_FLASH_CONTROLLER_GW_ADDR_BW      0x04000000
#define YU_FLASH_CONTROLLER_GW_IO_BW        0x02000000 /* quad mode */
#define YU_FLASH_CONTROLLER_GW_TRANS_SIZE   0x00000400 /* read 16 bytes */
#define YU_FLASH_CONTROLLER_GW_CS_HOLD      0x00000020
#define YU_FLASH_CONTROLLER_GW_DATA_PHASE   0x00000010
#define YU_FLASH_CONTROLLER_GW_ADDR_PHASE   0x00000008
#define YU_FLASH_CONTROLLER_GW_INSTR_PHASE  0x00000004
#define YU_FLASH_CONTROLLER_GW_READ         0x00000001

#define YU_FLASH_CONTROLLER_GW_CMD_BIT      16 /* access GW command bit */
/* SPI FLash controller supported read commands */
#define YU_FLASH_FC_RD                      0x03
#define YU_FLASH_FC_FAST_RD_QUAD_IO         0xeb
#define YU_FLASH_FC_FAST_RD_QUAD_OUTPUT     0x6b

/*
 * Macros to read and write crspace registers. In this implementation,
 * crspace registers are read/written using two mechanisms:
 *  (1) default_acc mode: mmio space access,
 *  (2) rsh_mem_acc mode: request data through the rshim widget,
 */

#define default_acc     MMIO
#define rsh_mem_acc     INDIRECT

#define IOSWAP32(x)     (__builtin_bswap32((x)))

/* Macros to read/write YU's 32-bit registers using the rshim widget. */
#define YU_INDIRECT_RD_DATA(off) \
	read_reg_indirect(YU_BASE_ADDR + (off), 2)  /* 2: 4 bytes */
#define YU_INDIRECT_WR_DATA(off, val) \
	write_reg_indirect((YU_BASE_ADDR + (off)), 2, (val)) /* 2: 4 bytes */

/* Wait for the rshim mem_acc interface to read/write data bytes. */
static inline void reg_indirect_wait(uint64_t resp_count)
{
	int retries = 1000;

	while (mmio_read_64(RSHIM_BASE + RSH_MEM_ACC_RSP_CNT) == resp_count) {
		if (--retries < 0) {
			ERROR("RSH_MEM_ACC timeout\n");
			panic();
		}
	}
}

/* Read shim registers indirectly, using the rshim mem_acc widget. */
static inline uint64_t read_reg_indirect(uintptr_t pa, uint8_t size)
{
	uint64_t  data64;
	uint64_t  resp_count = mmio_read_64(RSHIM_BASE + RSH_MEM_ACC_RSP_CNT);

	RSH_MEM_ACC_CTL_t rmac = {
		.address = pa,
		.size = size,
		.write = 0,
		.send = 1,
	};

	mmio_write_64(RSHIM_BASE + RSH_MEM_ACC_CTL, rmac.word);

	reg_indirect_wait(resp_count);

	data64 = mmio_read_64(RSHIM_BASE + RSH_MEM_ACC_DATA__FIRST_WORD);

	return data64;
}

/* Write shim registers indirectly, using the rshim mem_acc widget. */
static inline void write_reg_indirect(uintptr_t pa, uint8_t size, uint64_t data)
{
	uint64_t resp_count = mmio_read_64(RSHIM_BASE + RSH_MEM_ACC_RSP_CNT);

	mmio_write_64(RSHIM_BASE + RSH_MEM_ACC_DATA__FIRST_WORD, data);

	RSH_MEM_ACC_CTL_t rmac = {
		.address = pa,
		.size = size,
		.write = 1,
		.send = 1,
	};

	mmio_write_64(RSHIM_BASE + RSH_MEM_ACC_CTL, rmac.word);

	reg_indirect_wait(resp_count);
}

/* Macros to read/write YU's 32-bit registers using mmio space. */
#define YU_MMIO_RD_DATA(off)      mmio_read_32(YU_BASE_ADDR + (off))
#define YU_MMIO_WR_DATA(off, val) mmio_write_32((YU_BASE_ADDR + (off)), (val))

/*
 * Read/Write the YU registers. Accesses are 4-bytes long and the data
 * is byte swapped.
 */
#define YU_INDIRECT_RD(off)      IOSWAP32(YU_INDIRECT_RD_DATA((off)))
#define YU_INDIRECT_WR(off, val) YU_INDIRECT_WR_DATA((off), IOSWAP32((val)))

#define YU_MMIO_RD(off)          IOSWAP32(YU_MMIO_RD_DATA((off)))
#define YU_MMIO_WR(off, val)     YU_MMIO_WR_DATA((off), IOSWAP32((val)))

/*
 * Helper to operate and access registers through different modes.
 * 'mode' refers to either mechanisms to access crspace registers,
 * 'op' specifies whether it is a read or write.
 */
#define YU_IO_HELPER(mode, op)          YU_ ##mode ##_ ##op

/*
 * Coalesce the different modes and operate using common functions.
 * These macros MUST be used for all crspace accesses.
 */
#define YU_READ(mode, off)              YU_IO_HELPER(mode, RD) (off)
#define YU_WRITE(mode, off, val)        YU_IO_HELPER(mode, WR) (off, val)

#define YU_READ_DATA(mode, off)         YU_IO_HELPER(mode, RD_DATA) (off)
#define YU_WRITE_DATA(mode, off, val)   YU_IO_HELPER(mode, WR_DATA) (off, val)

/*
 * SPI Flash access function macros
 */

/* This function macro waits until the command is completed. */
#define flash_gw_wait(mode)						\
	do {								\
		uint32_t status;					\
		do {							\
			status = YU_READ(mode, YU_FLASH_CONTROLLER_GW_OFF); \
		} while (status & YU_FLASH_CONTROLLER_GW_BUSY);         \
	} while (0)

/* This functions issues a gateway command. */
#define flash_gw_exec_cmd(mode, cmd)					\
	do {								\
		cmd |= YU_FLASH_CONTROLLER_GW_LOCK;			\
		cmd |= YU_FLASH_CONTROLLER_GW_BUSY;			\
		YU_WRITE(mode, YU_FLASH_CONTROLLER_GW_OFF, cmd);	\
	} while (0)

#define flash_gw_exec_cmd_and_wait(mode, cmd)   \
	do {					\
		flash_gw_exec_cmd(mode, cmd);	\
		flash_gw_wait(mode);		\
	} while (0)

/* This function configures the gateway command registers. */
#define flash_gw_forge_cmd(mode, gw_cmd, rd_cmd, addr, is_first)    \
	do {							    \
		/*							\
		 * Prepare 'cs_hold' in 'gw_cmd'; this is useful when	\
		 * multiple GW transactions are needed.			\
		 */							\
		gw_cmd = YU_FLASH_CONTROLLER_GW_CS_HOLD;		\
		if (is_first) {						\
			/*						\
			 * Workaround HW bug: dummy command to set 'cs_hold' \
			 * on GW. This MUST be released at the end of the \
			 * transaction.					\
			 */						\
			flash_gw_exec_cmd_and_wait(mode, gw_cmd);	\
			/*						\
			 * Now prepare the first read command. Note that \
			 * 'cs_hold' bit is still asserted in 'gw_cmd'.	\
			 */						\
			gw_cmd |= YU_FLASH_CONTROLLER_GW_TURN_AR_SIZE;	\
			gw_cmd |= YU_FLASH_CONTROLLER_GW_INSTR_PHASE;	\
			gw_cmd |= YU_FLASH_CONTROLLER_GW_ADDR_PHASE;	\
			gw_cmd |= (rd_cmd << YU_FLASH_CONTROLLER_GW_CMD_BIT); \
			/* Write the start address to read from. */	\
			YU_WRITE(mode, YU_FLASH_CONTROLLER_ADDR_OFF, addr); \
		}							\
		gw_cmd |= YU_FLASH_CONTROLLER_GW_READ;			\
		gw_cmd |= YU_FLASH_CONTROLLER_GW_DATA_PHASE;		\
		gw_cmd |= YU_FLASH_CONTROLLER_GW_TRANS_SIZE;		\
		gw_cmd |= YU_FLASH_CONTROLLER_GW_IO_BW;			\
	} while (0)

/* This functions reads the gateway shadow data registers. */
#define flash_gw_read_shadow_data_noswap(mode, buf)                     \
	do {								\
		uint32_t *data32 = (uint32_t *) buf;			\
		uint8_t   byte   = 0;					\
		for (; byte < YU_FLASH_CONTROLLER_DATA_SIZE; byte += 4)	\
			data32[byte >> 2] = YU_READ_DATA(mode,		\
			   YU_FLASH_CONTROLLER_SHADOW_DATA_OFF + byte); \
	} while (0)

#define flash_gw_read_shadow_data(mode, buf)                            \
	do {								\
		uint32_t *data32 = (uint32_t *) buf;			\
		uint8_t   byte  = 0;					\
		for (; byte < YU_FLASH_CONTROLLER_DATA_SIZE; byte += 4) \
			data32[byte >> 2] = YU_READ(mode,		\
			   YU_FLASH_CONTROLLER_SHADOW_DATA_OFF + byte); \
	} while (0)

/*
 * This function macro issues an incremental read commands to the SPI flash
 * controller. This aims to read data with minimal overhead;
 * Indeed, the software won't wait until the gateway is busy, it will issue
 * the following command. The data of the previous command is loaded to the
 * shadow registers. Only the first command requires waiting until data is
 * ready.
 */
#define flash_gw_enable(mode, is_first, addr)				\
	do {								\
		uint32_t gw_cmd;					\
		uint8_t  rd_cmd = YU_FLASH_FC_FAST_RD_QUAD_OUTPUT;	\
		if (is_first) {						\
			flash_gw_forge_cmd(mode, gw_cmd, rd_cmd, addr,	\
					   is_first);			\
			flash_gw_exec_cmd_and_wait(mode, gw_cmd);	\
		}							\
		/* Fire the GW to prefetch the following data bytes. */	\
		flash_gw_forge_cmd(mode, gw_cmd, rd_cmd, addr, 0);	\
		flash_gw_exec_cmd(mode, gw_cmd);			\
	} while (0)

/*
 * This function macro tries to acquire the SPI flash controller
 * GW lock.
 */
#define flash_gw_acquire_lock(mode)					\
		do {							\
			uint32_t status, rep;				\
			do {						\
				status = YU_READ(mode,			\
						 YU_FLASH_CONTROLLER_GW_OFF); \
			} while (status & YU_FLASH_CONTROLLER_GW_LOCK);	\
			/* Clear the replacement enable bit. */		\
			rep = YU_READ(mode, YU_FLASH_CONTROLLER_REP_EN_OFF); \
			YU_WRITE(mode, YU_FLASH_CONTROLLER_REP_EN_OFF,	\
				 rep & ~0x1);				\
			/*						\
			 * Wait until 'busy' bit is clear, for cache	\
			 * replacement.					\
			 */						\
			do {						\
				status = YU_READ(mode,			\
						 YU_FLASH_CONTROLLER_GW_OFF); \
			} while (status & YU_FLASH_CONTROLLER_GW_BUSY);	\
		} while (0)

/*
 * This function macro releases the SPI flash controller GW since
 * this isn't done by HW.
 */
#define flash_gw_release(mode)						\
		do {							\
			uint32_t cmd, rep;				\
			cmd  = YU_FLASH_CONTROLLER_GW_LOCK;		\
			cmd |= YU_FLASH_CONTROLLER_GW_BUSY;		\
			/* Reset the GW to terminate the transaction. */ \
			flash_gw_exec_cmd_and_wait(mode, cmd);		\
			/* Release the lock. */				\
			rep = YU_READ(mode, YU_FLASH_CONTROLLER_REP_EN_OFF); \
			YU_WRITE(mode, YU_FLASH_CONTROLLER_REP_EN_OFF,	\
				 rep | 0x1);				\
			YU_WRITE(mode, YU_FLASH_CONTROLLER_GW_OFF, 0);	\
		} while (0)

/* This function macros read the SPI flash content. */
#define flash_indirect_read(is_first, addr, buf)			\
		do {							\
			flash_gw_enable(rsh_mem_acc, is_first, addr);	\
			flash_gw_read_shadow_data(rsh_mem_acc, buf);	\
		} while (0)

#define flash_mmio_read(rsh_mem_acc, addr, buf)				\
		do {							\
			flash_gw_enable(default_acc, is_first, addr);	\
			flash_gw_read_shadow_data(default_acc, buf);	\
		} while (0)

/*
 * This function macros read the SPI flash content without swapping
 * bytes.
 *
 * Note that the SPI flash contains the NIC firmware image; data words
 * are ordered in BE. when secure boot is enabled, the image is loaded
 * from the SPI flash. In order to minimize the overhead of the copy,
 * data is read without swapping bytes.
 */
#define flash_indirect_read_noswap(is_first, addr, buf)			\
		do {							\
			flash_gw_enable(rsh_mem_acc, is_first, addr);	\
			flash_gw_read_shadow_data_noswap(rsh_mem_acc, buf); \
		} while (0)

#define flash_mmio_read_noswap(is_first, addr, buf)			\
		do {							\
			flash_gw_enable(default_acc, is_first, addr);	\
			flash_gw_read_shadow_data_noswap(default_acc, buf); \
		} while (0)

/*
 * SPI FLash IO API
 */

/*
 * Setup the scratchpad. This should be called prior to any other flash io
 * call.
 */
int flash_io_setup_scratchpad(void);

/*
 * Check the image integrity and retrieves image length. This is intended
 * to check whether the "magic pattern" has been written, and the image has
 * been formatted properly.
 * Indeed, when secure boot is set, the NIC firmware image is copied to
 * the SPI flash, its corresponding digest value is stored at the end of
 * the image, and the "magic pattern" of the image is written to enable
 * it as active so the platform firmware start the authentication.
 */
int flash_io_check_integrity(unsigned int *img_len);

/* Get a pointer to the hash and its length from the current image. */
int flash_io_get_hash_param(void **hash_ptr, unsigned int *hash_len);

/* Get the data block to be hashed and its length from the current image */
int flash_io_get_data_param(unsigned int  data_idx,
			    void        **data_ptr,
			    unsigned int *data_len);

/*
 * Populates the device info data to the given address.
 * At most allowed to write <max_len> bytes to the given address.
 */
int flash_io_get_dev_info(void *data, int max_len);

/*
 * SPI Flash engine routines
 */
void flash_io_engine_do_read(void); /* transfer data from flash to SRAM. */
void flash_io_engine_ep(void);      /* engine entry point. */

#endif /* __IO_FLASH_H__ */
