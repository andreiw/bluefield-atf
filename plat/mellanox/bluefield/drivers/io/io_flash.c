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

#include <arch_helpers.h>
#include <io_flash.h>
#include <string.h>

/*
 * Note: all of the code that processes the NIC firmware image from the SPI
 * flash assumes that we are running in little-endian mode, and will need
 * minor tweaks if that is ever not true.
 */

/*
 * SPI Flash read functions
 *
 * Note that ATF is single threaded, BL2 code is executed in Core0,
 * which is referred to as the main core. This latter operates in EL1
 * with MMU enabled (identity mapping). In such mode of operation the
 * CRspace registers should be accessed using the rshim widget.
 * On the other hand, when the secondary core is enabled (Core1, also
 * referred to as the flash engine), the engine will operate in EL3 and
 * will access CRspace registers using mmio calls.
 */

/*
 * This function reads data chunks from the SPI flash. This is done through
 * the controller access gateway. The controller allows to read up to
 * 16 bytes per command, i.e. it has 4 registers of 32-bit word.
 */
static void flash_read_chunk(uint32_t  addr,
			     uint32_t *chunk,
			     uint32_t  chunk_size)
{
	uint32_t data[4];
	uint32_t idx;
	uint8_t  count;

	flash_gw_acquire_lock(rsh_mem_acc);

	for (idx = 0; idx < (chunk_size & ~0x3); idx += 4)
		flash_indirect_read((idx == 0), addr + (idx << 2), chunk + idx);

	count = chunk_size & 0x3;
	if (count > 0) {
		flash_indirect_read((idx == 0), addr + (idx << 2), data);
		switch (count) {
		case 3:
			chunk[idx + 2] = data[2];
			/* fall-through */
		case 2:
			chunk[idx + 1] = data[1];
			/* fall-through */
		case 1:
			chunk[idx + 0] = data[0];
		}
	}

	flash_gw_release(rsh_mem_acc);
}

#ifndef FLASH_ENGINE_ENABLED
/*
 * This function uses the rshim widget to read chunks of data from the SPI
 * flash. Address of the data chunks MUST be 16-bytes aligned. This function
 * is called when processing the flash content (i.e: signature verification)
 */
static void flash_read_chunk_noswap(uint32_t  addr,
				    uint32_t *chunk,
				    uint32_t  chunk_size)
{
	uint32_t idx;

	assert((chunk_size & 0x3) == 0);

	flash_gw_acquire_lock(rsh_mem_acc);
	for (idx = 0; idx < (chunk_size & ~0x3); idx += 4)
		flash_indirect_read_noswap((idx == 0), addr + (idx << 2),
					   chunk + idx);
	flash_gw_release(rsh_mem_acc);
}
#else
/*
 * This function uses the default mode to read chunks of data from the SPI
 * flash. Address of the data chunks MUST be 16-bytes aligned. This function
 * is called when processing the flash content (i.e: signature verification)
 */
static void flash_read_chunk_noswap(uint32_t  addr,
				    uint32_t *chunk,
				    uint32_t  chunk_size)
{
	uint32_t idx;

	assert((chunk_size & 0x3) == 0);

	flash_gw_acquire_lock(default_acc);
	for (idx = 0; idx < (chunk_size & ~0x3); idx += 4)
		flash_mmio_read_noswap((idx == 0), addr + (idx << 2),
				       chunk + idx);
	flash_gw_release(default_acc);
}
#endif

#ifdef FLASH_IO_DEBUG
void flash_print_data(uint32_t *data, uint32_t data_len)
{
	uint32_t data_idx;

	tf_printf("Data addr:0x%lx, size:0x%x", (uintptr_t) data, data_len);
	for (data_idx = 0; data_idx < data_len; data_idx++) {
		if (!(data_idx & 0x7))
			tf_printf("\n%u:", data_idx);
		tf_printf(" 0x%x", data[data_idx]);
	}
	tf_printf("\n");
}
#endif  /* FLASH_IO_DEBUG */

/*
 * Scratchpad functions. In order to authenticate the NIC firmware image,
 * we use the SRAM to contain the image raw data and its corresponding HMAC
 * signature. The area within the SRAM is referred to as flash_scratchpad.
 */

typedef struct {
	volatile uint64_t is_empty;     /* If set, the segment has no data */
	uint64_t          pad[7];       /* To align to cache line size */
} flash_scratchpad_seg_status_t;

/*
 * This function initializes the flash scratchpad data header; when the
 * flash engine is enabled, the data section of the scratchpad might be
 * divided among multiple data segments. These segments would contain
 * cached data that might be processed by multiple cores. The segment
 * data header informs about the state of each data segment, whether it
 * is empty or full.
 */
static void flash_setup_scratchpad_segment_data_hdr(void)
{
#ifdef FLASH_ENGINE_ENABLED
	flash_scratchpad_seg_status_t *scratch_hdr;
	uint8_t seg_idx;

	scratch_hdr = (flash_scratchpad_seg_status_t *)
		FLASH_SCRATCHPAD_DATA_HDR_BASE;
	memset(scratch_hdr, 0, FLASH_SCRATCHPAD_DATA_HDR_SIZE);
	for (seg_idx = 0; seg_idx < FLASH_SCRATCHPAD_SEGMENT_CNT; seg_idx++)
		scratch_hdr[seg_idx].is_empty = 1;

	flush_dcache_range((uintptr_t) scratch_hdr,
			   FLASH_SCRATCHPAD_DATA_HDR_SIZE);
#endif
}

/* Encapsulates scratchpad descriptor information */
typedef struct {
	/* address of the first scratchpad data chunk in SPI flash */
	uint32_t  data_addr_start;
	/* address of the last scratchpad data chunk in SPI flash */
	uint32_t  data_addr_max;
	/* Index of the data segment within the data block of the scratchpad */
	uint32_t  data_seg_idx;
	/* Index of the data block to read from the scratchpad */
	uint32_t  data_rd_idx;
	/* Index of the available scratchpad slot to write to */
	uint32_t  data_wr_idx;
	/* Whether the scratchpad has been initialized and ready to use */
	uint8_t   is_valid;
} flash_scratchpad_desc_t;

static flash_scratchpad_desc_t gbl_flash_scratchpad;

CASSERT(FLASH_SCRATCHPAD_DATA_CNT > 0, assert_flash_scratchpad_data_cnt);

int flash_io_setup_scratchpad(void)
{
	flash_scratchpad_desc_t *scratchpad = &gbl_flash_scratchpad;

	/* Initialize the scratchpad */
	scratchpad->data_addr_max = scratchpad->data_addr_start = 0;
	scratchpad->data_seg_idx  = 0;
	scratchpad->data_wr_idx   = scratchpad->data_rd_idx     = 0;
	scratchpad->is_valid      = 1;

	flush_dcache_range((uintptr_t) scratchpad,
			   sizeof(flash_scratchpad_desc_t));

	flash_setup_scratchpad_segment_data_hdr();

	return FLASH_IO_SUCCESS;
}

/*
 * Scratchpad helper functions.
 */

/* Returns a pointer to the signature section of the scratchpad */
static inline uint32_t *flash_scratchpad_get_sig_ptr(void)
{
	uint32_t *sig_ptr;

	assert(gbl_flash_scratchpad.is_valid == 1);

	sig_ptr = (uint32_t *) FLASH_SCRATCHPAD_SIG_BASE;
	return sig_ptr;
}

/* Returns a pointer to the block within the data section of the scratchpad */
static inline uint32_t *flash_scratchpad_get_data_ptr(uint32_t  data_idx,
						      uint32_t *data_size)
{
#ifndef FLASH_ENGINE_ENABLED
	uint32_t  *data_ptr;
	uintptr_t  data_addr;

	assert(gbl_flash_scratchpad.is_valid == 1);

	data_ptr   = NULL;
	*data_size = 0;

	if (data_idx > FLASH_SCRATCHPAD_DATA_CNT)
		return NULL;

	data_addr  = FLASH_SCRATCHPAD_DATA_BASE;
	data_addr += (data_idx * FLASH_SCRATCHPAD_DATA_SIZE);

	data_ptr   = (uint32_t *) data_addr;
	*data_size = FLASH_SCRATCHPAD_DATA_SIZE;

	return data_ptr;
#else
	flash_scratchpad_seg_status_t *scratch_hdr;
	flash_scratchpad_desc_t *scratchpad = &gbl_flash_scratchpad;
	uint32_t  seg_addr, data_addr;
	uint32_t *data_ptr;
	uint8_t   seg_idx;

	assert(gbl_flash_scratchpad.is_valid == 1);

	data_ptr   = NULL;
	*data_size = 0;

	scratch_hdr = (flash_scratchpad_seg_status_t *)
		FLASH_SCRATCHPAD_DATA_HDR_BASE;
	seg_idx     = scratchpad->data_seg_idx;

	/*
	 * Segment read should be done in incremental fashion. Wait
	 * until the following segment becomes full.
	 */
	while (scratch_hdr[seg_idx].is_empty == 1)
		;

	/* Get a pointer to the available data in the SRAM. */
	seg_addr   = FLASH_SCRATCHPAD_DATA_BASE +
		(seg_idx * FLASH_SCRATCHPAD_SEGMENT_DATA_SIZE);
	data_addr = seg_addr +
		(scratchpad->data_rd_idx * FLASH_SCRATCHPAD_DATA_SIZE);

	/* Increment the read index and wrap it up, if needed. */
	scratchpad->data_rd_idx += 1;

	clean_dcache_range((uintptr_t) data_addr,
			   FLASH_SCRATCHPAD_DATA_SIZE);

	if (scratchpad->data_rd_idx ==
	    FLASH_SCRATCHPAD_SEGMENT_DATA_CNT) {
		scratchpad->data_rd_idx       = 0;
		/*
		 * Need to re-fill the segment with data from the
		 * flash
		 */
		scratch_hdr[seg_idx].is_empty = 1;
		flush_dcache_range((uintptr_t) (scratch_hdr + seg_idx),
				   sizeof(flash_scratchpad_seg_status_t));
		/* Update next segment information. */
		seg_idx += 1;
		seg_idx %= FLASH_SCRATCHPAD_SEGMENT_CNT;
		scratchpad->data_seg_idx = seg_idx;
	}

	data_ptr   = (uint32_t *) ((uintptr_t) data_addr);
	*data_size = FLASH_SCRATCHPAD_DATA_SIZE;

	return data_ptr;
#endif
}

/*
 * Load the HMAC signature from the SPI flash to the scratchpad and return
 * a pointer to it.
 */
static uint32_t *flash_scratchpad_load_signature(uint32_t sig_addr,
						 uint32_t sig_size)
{
	uint32_t *sig_ptr;
	uint32_t  sig_word;

	if (sig_size > FLASH_SCRATCHPAD_SIG_SIZE)
		return NULL;

	sig_word = (sig_size + 3) / 4;
	sig_ptr  = flash_scratchpad_get_sig_ptr();
	if (!sig_ptr)
		return NULL;

	/* Copy the signature to the scratchpad from the flash */
	flash_read_chunk(sig_addr, sig_ptr, sig_word);

	return sig_ptr;
}

static inline void flash_scratchpad_mask_data(uint32_t *ptr,
					      uint32_t  off,
					      uint8_t   size)
{
	VERBOSE("SCRATCH mask bytes off:0x%lx at 0x%x\n",
		(uintptr_t) (ptr + (off >> 2)), off);
	memset(ptr + (off >> 2), 0xff, size);
}

/*
 * Load the NIC firmware image raw data from the SPI flash to the scratchpad
 * and return a pointer to the given data block.
 */
static uint32_t *flash_scratchpad_load_data(uint32_t  img_addr,
					    uint32_t  img_size,
					    uint32_t  itoc_entry_addr,
					    uint32_t  data_idx,
					    uint32_t *data_size)
{
#ifndef FLASH_ENGINE_ENABLED
	flash_scratchpad_desc_t *scratchpad = &gbl_flash_scratchpad;
	uint32_t                *chunk_ptr;
	uint32_t                 chunk_addr, chunk_word, chunk_mask_addr;
	uint32_t                 data_rd_idx, data_wr_idx, data_mask_addr;
	uint32_t                 img_addr_end;
	uint8_t                  data_in_scratchpad;

	assert(scratchpad->is_valid == 1);

	*data_size         = 0;
	data_in_scratchpad = 1;

	chunk_addr   = img_addr + (data_idx * FLASH_SCRATCHPAD_DATA_SIZE);
	chunk_word   = (FLASH_SCRATCHPAD_DATA_SIZE + 3) / 4;
	img_addr_end = img_addr + img_size;
	if (chunk_addr > img_addr_end)
		return NULL;

	/* Check whether the data chunk is available in the scratchpad */
	if (scratchpad->data_wr_idx != scratchpad->data_rd_idx &&
	    (chunk_addr > scratchpad->data_addr_max ||
	     chunk_addr < scratchpad->data_addr_start))
		data_in_scratchpad = 0;

	/*
	 * If the scratchpad is empty, then re-fill it and keep track
	 * of the mapping of the inital data in the SPI flash.
	 */
	if (scratchpad->data_wr_idx < FLASH_SCRATCHPAD_DATA_CNT) {
		scratchpad->data_addr_start = chunk_addr;
		scratchpad->data_addr_max   = chunk_addr;
		scratchpad->data_addr_max  += FLASH_SCRATCHPAD_DATA_SIZE_MAX;
		data_in_scratchpad = 0;
	}

	/*
	 * In order to be consistent with secure firmware update procedure,
	 * the iTOC entry of the HMAC digest embedded in the NIC FW image
	 * MUST be masked, and won't be used in HMAC calculation.
	 */
	data_mask_addr  = itoc_entry_addr &  FLASH_IMAGE_BLOCK_MASK;
	chunk_mask_addr = itoc_entry_addr & ~FLASH_IMAGE_BLOCK_MASK;

	/*
	 * If the data section in the scratchpad is empty, then load image
	 * from the flash.
	 */
	while (!data_in_scratchpad && chunk_addr < img_addr_end &&
	       scratchpad->data_wr_idx < FLASH_SCRATCHPAD_DATA_CNT) {
		/* Get a pointer to the next available slot in scratchpad */
		chunk_ptr =
			flash_scratchpad_get_data_ptr(scratchpad->data_wr_idx,
						      NULL);
		if (!chunk_ptr)
			return NULL;
		/* Read data bytes from the flash */
		flash_read_chunk_noswap(chunk_addr, chunk_ptr, chunk_word);
		/*
		 * Check whether data should be masked; Note that only image
		 * magic pattern and iTOC entry of the signature section must
		 * be masked.
		 */
		if (chunk_addr == img_addr)
			flash_scratchpad_mask_data(chunk_ptr, 0,
						   FLASH_IMAGE_SIGNATURE_SIZE);
		else if (chunk_addr == chunk_mask_addr)
			flash_scratchpad_mask_data(chunk_ptr,
				data_mask_addr, FLASH_SECTION_ITOC_ENTRY_SIZE);
		chunk_addr += FLASH_SCRATCHPAD_DATA_SIZE;
		scratchpad->data_wr_idx++;
	}

	/* Read the avaialable chunk */
	chunk_ptr = flash_scratchpad_get_data_ptr(scratchpad->data_rd_idx,
						  data_size);
	scratchpad->data_rd_idx++;

	data_rd_idx = scratchpad->data_rd_idx;
	data_wr_idx = scratchpad->data_wr_idx;

	/* Reset read/write indexes */
	if (data_rd_idx == data_wr_idx) {
		scratchpad->data_rd_idx = 0;
		scratchpad->data_wr_idx = 0;
	}

	return chunk_ptr;
#else
	/*
	 * The flash engine handles data copies, so just retrieve
	 * a pointer to the next data to proceess.
	 */
	return flash_scratchpad_get_data_ptr(data_idx, data_size);
#endif
}


/*
 * Integrity check functions.
 */

/* Encapsulates iTOC entry parameters */
typedef struct {
	uint32_t flash_addr; /* Relative address of the section in the flash */
	uint32_t size;       /* Size of the section in bytes */
	uint32_t type;       /* Type of the section */
} itoc_entry_t;

/* Encapsulates iTOC array parameters */
typedef struct {
	itoc_entry_t entries_tbl[ITOC_ENTRY_MAX]; /* iTOC entries */
	uint32_t     entry_start_addr; /* iTOC entry start address */
	uint32_t     entry_sig_addr;   /* address of the HMAC entry */
	uint8_t      entry_sig_idx;    /* index of the HMAC entry */
	uint8_t      entry_cnt;        /* number of entries */
} itoc_array_t;

/* Encapsulates the firmware image descriptor information */
typedef struct {
	uint32_t     base;       /* Base address of the flash image */
	uint32_t     size;       /* Size of the flash image */
	uint32_t     img_addr;   /* Address of the NIC firmware image */
	uint32_t     img_size;   /* Size of the NIC firmware image */
	uint32_t     sig_addr;   /* Address of HMAC signature */
	uint32_t     sig_size;   /* Size of HMAC signature */
	itoc_array_t itoc_array; /* iTOC array */
	uint8_t      is_valid;   /* If set, the flash image is valid */
} flash_img_desc_t;

static flash_img_desc_t gbl_flash_img_desc;

/*
 * The magic pattern that marks the beginning of valid 5th generation
 * firmware image.
 */
static const uint32_t flash_img_magic_pattern[] = {
	0x4d544657, 0xabcdef00, 0xfade1234, 0x5678dead
};

/*
 * This function verifies whether the image signature matches the magic
 * pattern referred to as 'magic'.
 * It returns 1 if there is match, 0 otherwise.
 */
static uint8_t flash_signature_is_valid(uint32_t sig_addr, uint32_t *magic)
{
	uint32_t sig[FLASH_IMAGE_SIG_WORD_MAX] = { 0 };
	uint32_t byte_len;

	flash_read_chunk(sig_addr, sig, FLASH_IMAGE_SIG_WORD_MAX);

	byte_len = FLASH_IMAGE_SIG_WORD_MAX << 2;
	return (memcmp(sig, magic, byte_len) == 0) ? 1 : 0;
}

/*
 * This function searches for the magic pattern at differnt flash addresses
 * which can store the beginning of a firmware image. Meaning, it scans the
 * flash content at addresses FLASH_IMAGE_1_ADDR, FLASH_IMAGE_2_ADDR (and
 * exceptionally at address FLASH_IMAGE_3_ADDR). If the device recognizes
 * the magic pattern, the function returns 1; '*addr' parameter stores the
 * boot image start address. Otherwise, the function returns 0.
 */
static uint8_t flash_scan_content(uint32_t *addr, uint32_t *magic)
{
	const uint32_t scan_addr[] = {
		FLASH_IMAGE_1_ADDR,
		FLASH_IMAGE_2_ADDR,
		FLASH_IMAGE_3_ADDR
	};
	uint8_t  is_valid = 0, addr_idx = 0;

	while (is_valid == 0 &&
	       addr_idx < (sizeof(scan_addr) / sizeof(scan_addr[0])))
		is_valid =
			flash_signature_is_valid(scan_addr[addr_idx++], magic);

	*addr = (is_valid == 1) ? scan_addr[addr_idx - 1] : 0;
	return is_valid;
}

static uint8_t flash_has_valid_itoc(uint32_t  img_start_addr,
				    uint32_t *itoc_hdr_valid_addr,
				    uint32_t *magic)
{
	uint32_t hdr_addr, hdr_addr_max;
	uint8_t  is_valid;

	hdr_addr     = img_start_addr + FLASH_SECTION_ITOC_PAD;
	hdr_addr_max = img_start_addr + FLASH_SECTION_ITOC_PAD_MAX;
	is_valid     = 0;

	while (hdr_addr < hdr_addr_max) {
		if (flash_signature_is_valid(hdr_addr, magic)) {
			is_valid = 1;
			break;
		}
		hdr_addr += FLASH_SECTION_ITOC_PAD;
	}

	*itoc_hdr_valid_addr = (is_valid == 1) ? hdr_addr : 0;
	return is_valid;
}

/*
 * This function returns the last offset of the highest image section
 * in the flash. The returned offset byte is relative to the image start
 * address.
 */
static uint32_t flash_get_img_last_offset(uint32_t      itoc_hdr_addr,
					  itoc_array_t *itoc_array)
{
	itoc_entry_t *itoc_entry_ptr;
	uint32_t      last_off, prev_addr;
	uint32_t      entry_addr, entry_addr_max, entry_cnt;
	uint32_t      entry[FLASH_SECTION_ITOC_ENTRY_WORD_MAX];
	uint32_t      flash_addr, size, prev_size;
	uint8_t       type;

	/*
	 * Skip the iTOC header; both the iTOC header and the entries are
	 * of 32 bytes size.
	 */
	entry_addr     = itoc_hdr_addr + FLASH_SECTION_ITOC_ENTRY_SIZE;
	entry_addr_max = itoc_hdr_addr + FLASH_SECTION_ITOC_PAD;
	entry_cnt      = 0;

	/* Save the iTOC entry start address. */
	itoc_array->entry_start_addr = entry_addr;

	prev_addr = prev_size = 0;
	size      = 0;

	while (entry_addr < entry_addr_max) {
		flash_read_chunk(entry_addr, entry,
				 FLASH_SECTION_ITOC_ENTRY_WORD_MAX);
		type       = ITOC_ENTRY_GET_TYPE(entry[ITOC_ENTRY_INFO_IDX]);
		size       = ITOC_ENTRY_GET_SIZE(entry[ITOC_ENTRY_INFO_IDX]);
		flash_addr =
		   ITOC_ENTRY_GET_FLASH_ADDR(entry[ITOC_ENTRY_FLASH_ADDR_IDX]);

		/*
		 * *TBD* Stop at IMAGE_INFO section and read 'secure_fw' bit.
		 * If the image is non-secure, exit imediately?
		 *
		 *  uint32_t img_info;
		 *
		 *  if (type == FLASH_SECTION_IMAGE_INFO) {
		 *      flash_read_chunk(flash_addr, &img_info, 1);
		 *          if (!IMAGE_INFO_SECURE_FW_IS_SET(img_info))
		 *              return 0;
		 *  }
		 */

		/*
		 * Looks like the iTOC entries are kept in linked-list manner;
		 * A valid entry presents a known type different from 0xff, the
		 * flash address should be greater than the one of the previous
		 * entry. Thus, return as soon as entry with type 0xff is read.
		 */
		if (type == 0xff)
			break;

		VERBOSE("iTOC Entry %u type:0x%x size:0x%x flash_addr:0x%x\n",
			entry_cnt, type, size, flash_addr);

		if (itoc_array) {
			itoc_entry_ptr =
				&itoc_array->entries_tbl[entry_cnt++];
			itoc_entry_ptr->flash_addr = flash_addr << 2;
			itoc_entry_ptr->size       = size       << 2;
			itoc_entry_ptr->type       = type;
		}

		if (type != ITOC_ENTRY_TYPE_HMAC) {
			prev_addr = flash_addr;
			prev_size = size;
		}

		entry_addr += FLASH_SECTION_ITOC_ENTRY_SIZE;
	}

	if (itoc_array)
		itoc_array->entry_cnt = entry_cnt;

	last_off = prev_addr + prev_size;
	last_off = (last_off << 2) - 1;
	return last_off;
}

/*
 * The magic pattern that marks the beginning of valid iTOC header.
 */
static const uint32_t flash_itoc_magic_pattern[] = {
	0x49544f43, 0x4081516, 0x2342cafa, 0xbacafe00
};

/*
 * This function returns the actual size of the image. This is be done
 * through parsing the iTOC table.
 *
 * The Image Table of Content (iTOC) is composed of a list of pointers to
 * the different sections in the firmware image. The iTOC section starts at
 * a 4KB aligned address. It contains a header followed by an array of iTOC
 * entries. The iTOC header signature determines the iTOC validity.
 * The data sections on the flash contains “raw” data only. The section
 * meta-data are set in the pointing iTOC entry. The size of the image is
 * defined as the last byte offset of the highest section pointed to by
 * an iTOC entry.
 */
static uint32_t flash_get_img_size_from_itoc_section(uint32_t      img_addr,
						     uint8_t       read_yu,
						     itoc_array_t *itoc_array)
{
	uint32_t  itoc_hdr_addr, last_off;
	uint32_t *itoc_magic;
	uint8_t   itoc_hdr_is_valid;
	uint32_t  img_size;

	itoc_magic        = (uint32_t *) &flash_itoc_magic_pattern[0];
	img_size          = 0;
	itoc_hdr_is_valid = 0;

	/* If 'read_yu' is set, then read the iTOC header pointer from YU */
	if (read_yu == 1) {
		itoc_hdr_addr     = YU_READ(rsh_mem_acc,
					    YU_BOOT_IMAGE_ITOC_OFF);
		/*
		 * TYU returns relative address, so add to the image start
		 * address.
		 */
		itoc_hdr_addr    += img_addr;
		itoc_hdr_is_valid =
			flash_signature_is_valid(itoc_hdr_addr, itoc_magic);
	}

	/* Lookup for a valid iTOC header in the flash. */
	if (itoc_hdr_is_valid == 0)
		itoc_hdr_is_valid = flash_has_valid_itoc(img_addr,
							 &itoc_hdr_addr,
							 itoc_magic);

	/*
	 * If the iTOC header is valid, then parse iTOC entries to determine
	 * the last offset of the highest image section.
	 */
	if (itoc_hdr_is_valid == 1) {
		last_off =
			flash_get_img_last_offset(itoc_hdr_addr, itoc_array);
		img_size = last_off + 1;
	}

	return img_size;
}

/*
 * This function searches for a "magic pattern" on the Flash.
 *
 * The image pattern is a 16-byte constant string that marks the beginning
 * of any valid 5th generation firmware image. The device failsafe flash
 * layout consists of up to two firmware images (Image 1 and Image 2) and
 * their expansion ROM images. The ConnectX-5 has a flash layout of a 16MB
 * flash that is logically divided into equally-sized chunks of 4MB, the
 * first chunk stores Image 1, and the second chunk stores Image 2.
 *
 * It returns 1 if the flash has a valid firmware image. Otherwise, it
 * returns 0 and the address.
 */
static uint8_t flash_has_valid_image(uint32_t     *img_addr,
				     uint32_t     *img_size,
				     itoc_array_t *itoc_array)
{
	uint32_t  img_start_addr, boot_img_err, actual_size;
	uint32_t *img_magic;
	uint8_t   img_is_valid, scan_enabled;

	img_magic    = (uint32_t *) &flash_img_magic_pattern[0];
	*img_addr    = 0;
	*img_size    = 0;
	img_is_valid = 0;
	scan_enabled = 0;

	/*
	 * First of all, retrieve the image start address.
	 */

	/*
	 * Information regarding the boot image should be found in YU Boot.
	 * So, read the start address, the iTOC and tools pointers, if the
	 * the YU Boot registers are valid.
	 */
	boot_img_err = YU_READ(rsh_mem_acc, YU_BOOT_IMAGE_ERR_OFF);
	if ((boot_img_err & YU_BOOT_IMAGE_ERR_MASK) == 0) {
		img_start_addr = YU_READ(rsh_mem_acc, YU_BOOT_IMAGE_START_OFF);
		/* Check whether the signature is valid */
		img_is_valid   =
			flash_signature_is_valid(img_start_addr, img_magic);
	}

	/* If the YU Boot information cannot be read, scan the flash. */
	if (img_is_valid == 0) {
		scan_enabled = 1;
		img_is_valid = flash_scan_content(&img_start_addr, img_magic);
	}

	/* Return if no valid image found. */
	if (img_is_valid == 0)
		return 0;

	/*
	 * Now determine the actual size of the image. Currently, the only way
	 * to determine the actual size of the image is done through the iTOC
	 * entries. The size is calculated based on the last byte of the highest
	 * image section excluding the HMAC signature section.
	 */
	actual_size = flash_get_img_size_from_itoc_section(img_start_addr,
				     (scan_enabled == 0) ? 1 : 0, itoc_array);
	/* The size MUST not exceed FLASH_IMAGE_DATA_SIZE. */
	*img_size    = (actual_size < FLASH_IMAGE_DATA_SIZE) ? actual_size : 0;
	*img_addr    = img_start_addr;
	img_is_valid = (*img_size > 0) ? 1 : 0;
	return img_is_valid;
}

/*
 * This function retrieves the HMAC signature information. Note that
 * the returned address is relative to the start of the FW image.
 */
static void flash_get_img_signature_info(itoc_array_t *itoc_array,
					 uint32_t     *sig_addr,
					 uint16_t     *sig_size)
{
	itoc_entry_t *itoc_entries_ptr;
	uint32_t entry_sig_addr;
	uint8_t  entry_cnt, curr_entry_idx;

	/* Retrieve HMAC signature information from iTOC array */
	entry_cnt = itoc_array->entry_cnt;
	while (entry_cnt > 0) {
		/*
		 * Last entry should refer to the HMAC signature
		 * section. But, let's keep it safe in case FW
		 * folks wishes to add a new iTOC entry or they
		 * messes with iTOC entry order.
		 */
		curr_entry_idx   = entry_cnt - 1;
		itoc_entries_ptr =
			&itoc_array->entries_tbl[curr_entry_idx];
		if (itoc_entries_ptr->type == ITOC_ENTRY_TYPE_HMAC) {
			*sig_addr       = itoc_entries_ptr->flash_addr;
			*sig_size       = itoc_entries_ptr->size;
			entry_sig_addr  =
				curr_entry_idx * FLASH_SECTION_ITOC_ENTRY_SIZE;
			entry_sig_addr += itoc_array->entry_start_addr;
			itoc_array->entry_sig_idx  = curr_entry_idx;
			itoc_array->entry_sig_addr = entry_sig_addr;
			return;
		}
		entry_cnt--;
	}

	*sig_addr = 0;
	*sig_size = 0;
}

/*
 * New routines to improve scratchpad performance.
 */

#ifdef FLASH_ENGINE_ENABLED
/*
 * For performance purposes, a secondary core is used to copy data bytes
 * from the SPI flash to the SRAM scratchpad area. So set Core1 as secondary
 * core to help out with these tasks.
 */
#define FLASH_ENGINES_MASK      0x0000002

/* Flash engine flags */
#define FLASH_ENGINE_F_START          0x8
#define FLASH_ENGINE_F_STOP           0x1

#define FLASH_ENGINE_EP_INVALID       0

/*
 * This function initializes the mailbox according to the flash engine
 * setup, i.e., flag and entry point.
 */
static void flash_engine_setup(uint8_t flag, uintptr_t entry_pt)
{
	uintptr_t *mailbox;
	uint64_t  *cpu_bitmap;

	mailbox = (void *) MBOX_BASE;

	if (entry_pt != FLASH_ENGINE_EP_INVALID)
		*mailbox = entry_pt; /* Program the jump address. */

	cpu_bitmap    = (uint64_t *)&mailbox[1];
	cpu_bitmap[0] = (flag == FLASH_ENGINE_F_START) ?
		FLASH_ENGINES_MASK : 0;
	cpu_bitmap[1] = 0;

	flush_dcache_range((uintptr_t) mailbox,
			   (uintptr_t) &cpu_bitmap[2] - (uintptr_t) mailbox);

	dsbsy();

	mmio_write_64(RSHIM_BASE + RSH_SCRATCHPAD4, 1);

	sev();
}

/*
 * This function routine is executed by the secondary core (i.e. engine)
 * to transfer the firmware image blocks from the SPI flash into the SRAM.
 */
void flash_io_engine_do_read(void)
{
	flash_scratchpad_seg_status_t *scratch_hdr;
	flash_img_desc_t *flash_img_desc = &gbl_flash_img_desc;
	itoc_array_t     *itoc_array;
	uint32_t  seg_addr[FLASH_SCRATCHPAD_SEGMENT_CNT];
	uint32_t *chunk_ptr;
	uint32_t  chunk_word, chunk_addr, chunk_mask_addr;
	uint32_t  img_start_addr, img_size, img_off, img_addr_end;
	uint32_t  itoc_entry_addr, data_mask_addr;
	uint8_t   data_idx, seg_idx;

	img_start_addr = flash_img_desc->img_addr;
	img_size       = flash_img_desc->img_size;
	img_addr_end   = img_start_addr + img_size;

	scratch_hdr = (flash_scratchpad_seg_status_t *)
		FLASH_SCRATCHPAD_DATA_HDR_BASE;

	/* Set the addresses of each section */
	for (seg_idx = 0; seg_idx < FLASH_SCRATCHPAD_SEGMENT_CNT; seg_idx++) {
		seg_addr[seg_idx] = FLASH_SCRATCHPAD_DATA_BASE +
			(seg_idx * FLASH_SCRATCHPAD_SEGMENT_DATA_SIZE);
	}

	/*
	 * In order to be consistent with secure firmware update procedure,
	 * the iTOC entry of the HMAC digest embedded in the NIC FW image
	 * MUST be masked, and won't be used in HMAC calculation.
	 */
	itoc_array      = &flash_img_desc->itoc_array;
	itoc_entry_addr = itoc_array->entry_sig_addr;
	data_mask_addr  = itoc_entry_addr &  FLASH_IMAGE_BLOCK_MASK;
	chunk_mask_addr = itoc_entry_addr & ~FLASH_IMAGE_BLOCK_MASK;

	seg_idx    = 0;
	img_off    = img_start_addr;
	chunk_word = (FLASH_SCRATCHPAD_DATA_SIZE + 3) / 4;

	while (1) {

		if (img_off >= img_addr_end)
			break;

		seg_idx %= FLASH_SCRATCHPAD_SEGMENT_CNT;
		if (scratch_hdr[seg_idx].is_empty == 0) {
			seg_idx++;
			continue;
		}

		/* Get the start address of the data segment */
		chunk_addr = seg_addr[seg_idx];

		clean_dcache_range((uintptr_t) chunk_addr,
				   FLASH_SCRATCHPAD_SEGMENT_DATA_SIZE);

		/*
		 * If the data segment in the scratchpad is empty, then
		 * copy image from the flash.
		 */
		for (data_idx = 0; data_idx < FLASH_SCRATCHPAD_SEGMENT_DATA_CNT;
		     data_idx++) {
			chunk_ptr = (uint32_t *) ((uintptr_t)chunk_addr);
			flash_read_chunk_noswap(img_off, chunk_ptr, chunk_word);

			/*
			 * Check whether data should be masked; Note that
			 * only image magic pattern and iTOC entry of the
			 * signature section must be masked.
			 */
			if (img_off == img_start_addr)
				flash_scratchpad_mask_data(chunk_ptr, 0,
						   FLASH_IMAGE_SIGNATURE_SIZE);
			else if (img_off == chunk_mask_addr)
				flash_scratchpad_mask_data(chunk_ptr,
				 data_mask_addr, FLASH_SECTION_ITOC_ENTRY_SIZE);

			img_off    += FLASH_SCRATCHPAD_DATA_SIZE;
			chunk_addr += FLASH_SCRATCHPAD_DATA_SIZE;

			/*
			 * Flushing the dcache allows the main core to read
			 * the most recent data in the SRAM.
			 */
			flush_dcache_range((uintptr_t) chunk_ptr,
					   FLASH_SCRATCHPAD_DATA_SIZE);
		}

		/* Mark the segment as full */
		scratch_hdr[seg_idx].is_empty = 0;
		flush_dcache_range((uintptr_t) (scratch_hdr + seg_idx),
				   sizeof(flash_scratchpad_seg_status_t));
		seg_idx++;
	}

	/* All work done; reset the flash engine and exit */
	flash_engine_setup(FLASH_ENGINE_F_STOP, FLASH_ENGINE_EP_INVALID);

	return;
}
#endif

int flash_io_check_integrity(unsigned int *img_len)
{
	flash_img_desc_t *flash_img_desc = &gbl_flash_img_desc;
	itoc_array_t     *itoc_array;
	uint32_t          img_addr, img_size;
	uint32_t          sig_addr;
	uint16_t          sig_size;

	flash_img_desc->is_valid = 0;

	*img_len = 0;

	itoc_array            = &flash_img_desc->itoc_array;
	itoc_array->entry_cnt = 0;

	/*
	 * If the flash contains a valid image then the image base and size
	 * are set. The image base refers to the start of the image in the
	 * flash, i.e., address of the first byte of the magic pattern. The
	 * size refers to the actual size of the image to authenticate. This
	 * does not include the signature of the image signature.
	 */
	if (!flash_has_valid_image(&img_addr, &img_size, itoc_array))
		return FLASH_IO_ERR_INTEGRITY_CHECK;

	flash_get_img_signature_info(itoc_array, &sig_addr, &sig_size);
	if (sig_addr == 0 || sig_size == 0)
		return FLASH_IO_ERR_INTEGRITY_CHECK;

	flash_img_desc->img_addr = img_addr;
	flash_img_desc->img_size = img_size;
	flash_img_desc->sig_addr = img_addr + sig_addr;
	flash_img_desc->sig_size = sig_size;

	flash_img_desc->is_valid = 1;

	*img_len = img_size;

	VERBOSE("FLASH img addr:0x%x img size:0x%x\n", img_addr, img_size);
	VERBOSE("FLASH sig addr:0x%x sig size:0x%x\n", sig_addr, sig_size);

#ifdef FLASH_ENGINE_ENABLED
	flush_dcache_range((uintptr_t) flash_img_desc,
			   sizeof(flash_img_desc_t));

	flash_engine_setup(FLASH_ENGINE_F_START,
			   (uintptr_t) &flash_io_engine_ep);
#endif

	return FLASH_IO_SUCCESS;
}

/*
 * functions to retrieve information needed to authenticate the NIC firmware
 * image.
 */

int flash_io_get_hash_param(void **hash_ptr, unsigned int *hash_len)
{
	flash_img_desc_t *flash_img_desc = &gbl_flash_img_desc;
	uint32_t         *ptr;
	uint32_t          addr, size;

	if (!flash_img_desc->is_valid)
		return FLASH_IO_ERR_HASH_GET;

	addr = flash_img_desc->sig_addr;
	size = flash_img_desc->sig_size;

	ptr = flash_scratchpad_load_signature(addr, size);
	if (!ptr)
		return FLASH_IO_ERR_HASH_GET;

	*hash_len = (unsigned int) size;
	*hash_ptr = (void *) ptr;

	return FLASH_IO_SUCCESS;
}

int flash_io_get_data_param(unsigned int  data_idx,
			    void        **data_ptr,
			    unsigned int *data_len)
{
	uint32_t         *ptr;
	uint32_t          len;
	flash_img_desc_t *flash_img_desc = &gbl_flash_img_desc;
	itoc_array_t     *itoc_array;
	uint32_t          addr, size;
	uint32_t          entry_addr;

	if (!flash_img_desc->is_valid)
		return FLASH_IO_ERR_DATA_GET;

	addr       = flash_img_desc->img_addr;
	size       = flash_img_desc->img_size;
	itoc_array = &flash_img_desc->itoc_array;
	entry_addr = itoc_array->entry_sig_addr;

	ptr = flash_scratchpad_load_data(addr, size, entry_addr,
					 data_idx, &len);
	if (!ptr || len == 0)
		return FLASH_IO_ERR_DATA_GET;

	*data_len = (unsigned int) len;
	*data_ptr = (void *) ptr;

	return FLASH_IO_SUCCESS;
}

int flash_io_get_dev_info(void *data, int max_len)
{
#define ID_STR_ITEM	0x82
#define VPD_R_ITEM	0x90
	const char *dtoc_signature = (const char []){
		0x43, 0x4f, 0x54, 0x44, 0x16, 0x15, 0x08, 0x04,
		0xfa, 0xca, 0x42, 0x23, 0x00, 0xfe, 0xca, 0xba,
	};

	struct flash_dtoc_entry mfg_info = {.type = 0};
	struct flash_dtoc_entry vpd_ro = {.type = 0};

	assert(max_len >= FLASH_SECTION_ITOC_PAD);

	/* Load the dTOC from SPI flash, max size same as iTOC */
	flash_read_chunk(FLASH_DTOC_ADDR, data, FLASH_SECTION_ITOC_PAD);

	struct flash_dtoc_centent *dtoc = (struct flash_dtoc_centent *)data;

	/* Verify that it is indeed the dTOC section */
	if (memcmp(dtoc->signature, dtoc_signature, FLASH_IMAGE_SIGNATURE_SIZE))
		return FLASH_IO_ERR_SIG_MISMATCH;

	/* Extract the manufacture info(for PSID) and VPD section */
	for (int i = 0; i < ITOC_ENTRY_MAX; i++) {
		if (dtoc->entries[i].type == 0xff)
			break;

		if (dtoc->entries[i].type == DTOC_ENTRY_TYPE_MFG_INFO)
			mfg_info = dtoc->entries[i];

		if (dtoc->entries[i].type == DTOC_ENTRY_TYPE_VPD_RO)
			vpd_ro = dtoc->entries[i];
	}

	/*
	 * Extract the OPN from the VPD. This takes priority over the PSID
	 * as given a new variant of a board, the PSID wouldn't match any
	 * exisitng ones, but the OPN might be similar enough that BL2
	 * can do the right configuration.
	 */
	if (vpd_ro.type == DTOC_ENTRY_TYPE_VPD_RO) {
		int idx = 0;
		char *vpd = data;

		assert(max_len >= vpd_ro.size);

		flash_read_chunk(vpd_ro.flash_addr, data, vpd_ro.size);

		/*
		 * Swap the byte order since they are swapped when read
		 * through the gateway.
		 */
		for (int i = 0; i < (vpd_ro.size + 3) / 4; i++)
			((uint32_t *)data)[i] = IOSWAP32(((uint32_t *)data)[i]);

		/* Skip the Identifier String and get to the VPD-R section */
		if (vpd[idx] != ID_STR_ITEM)
			goto vpd_fail;

		idx += vpd[idx + 1] + (vpd[idx + 2] << 8) + 3;

		/*
		 * This is in VPD RO storage, so the only resource data type
		 * besides the Indentifier String and End Tag should be VPD-R.
		 */
		if (vpd[idx] != VPD_R_ITEM)
			goto vpd_fail;

		int vpd_r_end = idx + vpd[idx + 1] + (vpd[idx + 2] << 8) + 3;

		idx += 3;
		/* Go through each field in VPD-R to find part number */
		while (idx < vpd_r_end) {
			/* Checksum is the last field in VPD-R */
			if (!memcmp(&vpd[idx], "RV", 2))
				goto vpd_fail;

			/* We want the part number field */
			if (!memcmp(&vpd[idx], "PN", 2)) {
				/* Sanity check field starts with "MBE"/"MBF" */
				if (memcmp(&vpd[idx + 3], "MBE", 3) &&
				    memcmp(&vpd[idx + 3], "MBF", 3))
					goto vpd_fail;

				memmove(data, &vpd[idx + 3], vpd[idx + 2]);
				((char *)data)[(int)vpd[idx + 2]] = 0;
				return FLASH_IO_SUCCESS;
			}
			idx += vpd[idx + 2] + 3;
		}
	}

vpd_fail:

	/* Extract the PSID if no valid OPN found */
	if (mfg_info.type == DTOC_ENTRY_TYPE_MFG_INFO) {
		/* We only need to read the first 16 bytes for the PSID */
		flash_read_chunk(mfg_info.flash_addr, data, 16);

		/*
		 * Swap the byte order since they are swapped when read
		 * through the gateway.
		 */
		for (int i = 0; i < 4; i++)
			((uint32_t *)data)[i] = IOSWAP32(((uint32_t *)data)[i]);

		/* Do a sanity check to see the PSID string starts with "MT_" */
		if (!memcmp(data, "MT_", 3)) {
			/* Don't want returned string to continue forever */
			((char *)data)[16] = 0;
			return FLASH_IO_SUCCESS;
		}
	}


	return FLASH_IO_ERR_NO_DATA;
}
