/*
 * Copyright (c) 2014, ARM Limited and Contributors. All rights reserved.
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

#include <assert.h>
#include <debug.h>
#include <io_driver.h>
#include <io_storage.h>
#include <mmio.h>

#include <bluefield_def.h>

#include "bluefield_boot.h"
#include "rsh_def.h"


/*
 * Note: all of the code that processes the boot stream assumes that we are
 * running in little-endian mode, and will need minor tweaks if that is
 * ever not true.
 */

/* Last header read from boot data stream */
static union boot_image_header header;

/* Have we read a header from the data stream yet? */
static uint8_t header_valid;

/* What's the partial CRC of the data read so far? */
static uint32_t partial_crc;

/*
 * How many bytes have been read from the stream but not used?  Note that
 * this may include padding bytes at the end of an image which are not
 * counted in the image's length.
 */
static int residue_bytes;

/*
 * What are those bytes?  They're the low-order bytes of this word,
 * starting with the lowest-order.
 */
static uint64_t residue;

/* How many bytes have we read or skipped from the current image? */
static int cur_offset;


/*
 * Read bytes from the current image into a buffer, or throw them away if
 * the buffer is NULL.  We also keep track of the CRC of the data seen so
 * that we can check it at the end of the file.
 */
static void read_bytes(uint8_t *buf, int bytes)
{
	cur_offset += bytes;

	while (bytes && residue_bytes) {
		bytes--;
		residue_bytes--;
		if (buf)
			*buf++ = residue & 0xFF;
		residue >>= 8;
	}

	uint64_t *buf64 = (uint64_t *)buf;
	uint64_t c = mmio_read_64(RSHIM_BASE + RSH_BOOT_FIFO_COUNT);

	for (; bytes >= 8; bytes -= 8) {
		if (c <= 0)
			while ((c = mmio_read_64(RSHIM_BASE +
						 RSH_BOOT_FIFO_COUNT)) <= 0)
				;

		uint64_t d = mmio_read_64(RSHIM_BASE + RSH_BOOT_FIFO_DATA);

		/* FIXME use a compiler instrinsic here, once we have one */
		__asm__("crc32x %w0, %w0, %x1" : "+r" (partial_crc) : "r" (d));
		if (buf64)
			*buf64++ = d;
		c--;
	}

	if (bytes) {
		buf = (uint8_t *)buf64;

		if (c <= 0)
			while ((c = mmio_read_64(RSHIM_BASE +
						 RSH_BOOT_FIFO_COUNT)) <= 0)
				;

		uint64_t d = mmio_read_64(RSHIM_BASE + RSH_BOOT_FIFO_DATA);

		/* FIXME use a compiler instrinsic here, once we have one */
		__asm__("crc32x %w0, %w0, %x1" : "+r" (partial_crc) : "r" (d));
		for (int i = 0; i < bytes; i++) {
			if (buf)
				*buf++ = d & 0xFF;
			d >>= 8;
		}

		residue = d;
		residue_bytes = 8 - bytes;
	}
}

/*
 * Read the next header from the data stream, skipping any unread bytes in
 * the current image first.
 */
static void get_next_header(void)
{
	if (header_valid)
		read_bytes(NULL, header.data.image_len - cur_offset);

	while (mmio_read_64(RSHIM_BASE + RSH_BOOT_FIFO_COUNT) < 3)
		;

	header.words[0] = mmio_read_64(RSHIM_BASE + RSH_BOOT_FIFO_DATA);
	header.words[1] = mmio_read_64(RSHIM_BASE + RSH_BOOT_FIFO_DATA);
	header.words[2] = mmio_read_64(RSHIM_BASE + RSH_BOOT_FIFO_DATA);

	if (header.data.magic != BFB_IMGHDR_MAGIC) {
		ERROR("BlueField boot: bad magic number 0x%x\n",
		      header.data.magic);
		panic();
	}

	if (header.data.major != BFB_IMGHDR_MAJOR) {
		ERROR("BlueField boot: bad major version %d\n",
		      header.data.major);
		panic();
	}

	/* Discard any extra header words. */
	if (header.data.hdr_len > 3)
		for (int i = 0; i < header.data.hdr_len - 3; i++)
			(void) mmio_read_64(RSHIM_BASE + RSH_BOOT_FIFO_DATA);

	INFO(
	"BlueField ImgHdr V%d.%d Len %d ID %d ImLen %d HdCRC 0x%x FolIm 0x%lx\n",
	     header.data.major, header.data.minor,
	     header.data.hdr_len,
	     header.data.image_id, header.data.image_len,
	     header.data.image_crc,
	     header.data.following_images);

	partial_crc = ~0;
	residue_bytes = 0;
	cur_offset = 0;
	header_valid = 1;
}


/* Identify the device type as platform-specific */
static io_type_t device_type_bf(void)
{
	return IO_TYPE_PLATFORM;
}


/* Bf functions, device info and handle */

static int bf_dev_open(const uintptr_t dev_spec, io_dev_info_t **dev_info);
static int bf_file_open(io_dev_info_t *dev_info, const uintptr_t spec,
		io_entity_t *entity);
static int bf_file_seek(io_entity_t *entity, int mode, ssize_t cur_offset);
static int bf_file_len(io_entity_t *entity, size_t *length);
static int bf_file_read(io_entity_t *entity, uintptr_t buffer, size_t length,
		size_t *length_read);
static int bf_file_close(io_entity_t *entity);

static const io_dev_connector_t bf_dev_connector = {
	.dev_open = bf_dev_open
};


static const io_dev_funcs_t bf_dev_funcs = {
	.type = device_type_bf,
	.open = bf_file_open,
	.seek = bf_file_seek,
	.size = bf_file_len,
	.read = bf_file_read,
	.write = NULL,
	.close = bf_file_close,
	.dev_init = NULL,
	.dev_close = NULL,
};


/* No state associated with this device so structure can be const */
static const io_dev_info_t bf_dev_info = {
	.funcs = &bf_dev_funcs,
	.info = (uintptr_t)NULL
};


/* Open a connection to the Bf boot stream device */
static int bf_dev_open(const uintptr_t dev_spec __unused,
		io_dev_info_t **dev_info)
{
	assert(dev_info != NULL);
	*dev_info = (io_dev_info_t *)&bf_dev_info; /* cast away const */
	return 0;
}


/* Open a file on the Bf boot stream device */
static int bf_file_open(io_dev_info_t *dev_info __attribute__((unused)),
		const uintptr_t spec, io_entity_t *entity)
{
	uint8_t id = spec;

	assert(entity != NULL);

	if (!header_valid)
		get_next_header();

	if (header.data.image_id != id &&
	    (header.data.following_images & (1ULL << id)) == 0)
		return -ENOENT;

	while (header.data.image_id != id)
		get_next_header();

	entity->info = (uintptr_t) id;

	return 0;
}


/* Seek to a particular file offset on the Bf boot stream device */
static int bf_file_seek(io_entity_t *entity, int mode, ssize_t offset)
{
	assert(entity != NULL);
	assert((int) entity->info == header.data.image_id);
	assert(header_valid);

	/*
	 * Adjust the input offset to the absolute byte position to seek
	 * to.  We can only seek forward, so some modes are invalid.
	 */
	switch (mode) {
	case IO_SEEK_SET:
		break;

	case IO_SEEK_CUR:
		offset += cur_offset;
		break;

	case IO_SEEK_END:
		offset += header.data.image_len;
		break;

	default:
		return -EINVAL;
	}

	if (offset < cur_offset || offset >= header.data.image_len)
		return -EINVAL;

	read_bytes(NULL, offset - cur_offset);

	return 0;
}


/* Return the size of a file on the Bf boot stream device */
static int bf_file_len(io_entity_t *entity, size_t *length)
{
	assert(entity != NULL);
	assert((int) entity->info == header.data.image_id);
	assert(header_valid);
	assert(length != NULL);

	*length = (size_t) header.data.image_len;

	return 0;
}


/* Read data from a file on the Bf boot stream device */
static int bf_file_read(io_entity_t *entity, uintptr_t buffer, size_t length,
		size_t *length_read)
{
	assert(entity != NULL);
	assert((int) entity->info == header.data.image_id);
	assert(header_valid);
	assert(buffer != (uintptr_t)NULL);
	assert(length_read != NULL);

	if (cur_offset + length > header.data.image_len)
		length = header.data.image_len - cur_offset;

	read_bytes((uint8_t *) buffer, length);

	/*
	 * If we've consumed the entire image, then the CRC should match
	 * what was in the header.
	 */
	if (cur_offset >= header.data.image_len &&
	    header.data.image_crc != ~partial_crc) {
		ERROR(
		"BlueField boot: image %d bad CRC: expected 0x%x actual 0x%x\n",
		      header.data.image_id,
		      header.data.image_crc, ~partial_crc);
		return -EIO;
	}

	*length_read = length;
	return 0;
}


/* Close a file on the Bf boot stream device */
static int bf_file_close(io_entity_t *entity)
{
	assert(entity != NULL);
	assert((int) entity->info == header.data.image_id);

	return 0;
}


/* Exported functions */

/* Register the Bf boot stream driver with the IO abstraction */
int register_io_dev_bf(const io_dev_connector_t **dev_con)
{
	int result;

	assert(dev_con != NULL);

	result = io_register_device(&bf_dev_info);
	if (result == 0)
		*dev_con = &bf_dev_connector;

	return result;
}
