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

#include <assert.h>
#include <bluefield_auth_mod.h>
#include <debug.h>
#include <mmio.h>
#include "bluefield_def.h"
#include "rsh.h"

/* Mellanox Device Key buffer */
static unsigned char mellanox_device_key[MDK_BUF_SIZE];

/* Mellanox Device Key is valid, if set */
static unsigned char mellanox_device_key_is_valid;

/*
 * Variable exported by the BlueField crypto HMAC library through
 * REGISTER_BLUEFILED_AUTH_LIB()
 */
extern const bluefield_auth_lib_desc_t bluefield_auth_lib_desc;

#ifdef BLUEFIELD_AUTH_MOD_DEBUG
#define mdkey_dump()							  \
	do {								  \
		tf_printf("Mellanox Device Key:\n");			  \
		for (int i = 0; i < (MDK_BUF_SIZE & ~0x3); i += 4) {	  \
			tf_printf(" 0x%x 0x%x 0x%x 0x%x\n",		  \
				  ((uint8_t *) mellanox_device_key)[i+0], \
				  ((uint8_t *) mellanox_device_key)[i+1], \
				  ((uint8_t *) mellanox_device_key)[i+2], \
				  ((uint8_t *) mellanox_device_key)[i+3]  \
				);					  \
		}							  \
	} while (0)
#else
#define mdkey_dump() do {} while (0)
#endif

#define return_if_error(rc)				\
	do {						\
		if (rc != BLUEFIELD_AUTH_SUCCESS) {	\
			return rc;			\
		}					\
	} while (0)

/*
 * Verify a HMAC by comparison
 *
 * Parameters:
 *
 *   data_ptr, data_len: data to be hashed
 *   digest_info_ptr, digest_info_len: hash to be compared
 *   sk_ptr, sk_len: secret key pointer and size.
 */
int bluefield_auth_mod_verify_hmac(void         *data_ptr,
				   unsigned int  data_len,
				   void         *digest_info_ptr,
				   unsigned int  digest_info_len,
				   void         *sk_ptr,
				   unsigned int  sk_len,
				   unsigned int  flags)
{
	assert(data_ptr != NULL);
	assert(data_len != 0);
	assert(digest_info_ptr != NULL);
	assert(digest_info_len != 0);
	assert(sk_ptr != NULL);
	assert(sk_len != 0);

	return bluefield_auth_lib_desc.verify_hmac(data_ptr,
						   data_len,
						   digest_info_ptr,
						   digest_info_len,
						   sk_ptr,
						   sk_len,
						   flags);
}

/*
 * Returns a pointer to the Mellanox Device Key (MDK)
 *
 * Return:
 *   0 = success, Otherwise = error
 */
static int bluefield_auth_get_key(void **sk_ptr, unsigned int *sk_len)
{
	unsigned char *ptr;

	/* Check whether the MDK is valid. */
	if (!mellanox_device_key_is_valid) {
		ERROR("Mellanox Device Key is invalid!\n");
		return BLUEFIELD_AUTH_ERR_KEY;
	}

	ptr  = &mellanox_device_key[0];

	*sk_len = MDK_BUF_SIZE;
	*sk_ptr = (void *) ptr;

	return BLUEFIELD_AUTH_SUCCESS;
}

/*
 * Authenticate the NIC Firmware image
 *
 * This function authenticates an image using HMAC algorithm, the image must
 * contain:
 *
 *   - The data to calculate the HMAC from
 *   - The HMAC to be matched with
 *
 * For a successful authentication, the two hashes MUST match each other. The
 * function calls the crypto HMAC library to check this matching.
 *
 * Parameters:
 *   img: pointer to image in memory
 *   img_len: length of image (in bytes)
 *
 * Return: 0 = success, Otherwise = error
 */
int bluefield_auth_mod_verify(void)
{
	void *data_ptr, *hash_ptr, *sk_ptr;
	unsigned int data_len, hash_len, sk_len, img_len, img_auth_len;
	unsigned int flags, data_idx;
	int rc = 0;

	/* First, get the secret key to be used to hash the current image */
	rc = bluefield_auth_get_key(&sk_ptr, &sk_len);
	return_if_error(rc);

	/* Setup the scratchpad area to process data from SPI flash */
	rc = flash_io_setup_scratchpad();
	return_if_error(rc);

	/*
	 * Check the image integrity and retrieve image length.
	 */
	rc = flash_io_check_integrity(&img_len);
	return_if_error(rc);

	/* Get the hash from the image and its length */
	rc = flash_io_get_hash_param(&hash_ptr, &hash_len);
	return_if_error(rc);

	data_idx     = 0;
	img_auth_len = img_len;
	flags        = AUTH_F_HMAC_START;

	while (img_auth_len != 0) {

		/* Get the data to be hashed from the current image */
		rc = flash_io_get_data_param(data_idx++, &data_ptr, &data_len);
		return_if_error(rc);

		/*
		 * Adjust the data length, if needed. Also update the remaining
		 * image bytes left to authenticate.
		 */
		data_len = (img_auth_len > data_len) ? data_len : img_auth_len;
		img_auth_len -= data_len;

		/*
		 * Authenticate the image using the crypto HMAC library and the
		 * hash indicated in the image descriptor.
		 */
		rc = bluefield_auth_mod_verify_hmac(data_ptr, data_len,
						    hash_ptr, hash_len,
						    sk_ptr, sk_len, flags);
		return_if_error(rc);

		/* Update flags for hmac verification */
		flags = AUTH_F_HMAC_UPDATE;
	}

	/* Final round; get the authentication result */
	rc = bluefield_auth_mod_verify_hmac(data_ptr, data_len,
					    hash_ptr, hash_len,
					    sk_ptr, sk_len,
					    AUTH_F_HMAC_FINISH);
	return_if_error(rc);

	return BLUEFIELD_AUTH_SUCCESS;
}

/*
 * Read the Mellanox Device Key (MDK).
 */
static void bluefield_auth_mdkey_read(void)
{
	uint64_t  key_reg;
	uint32_t  offset, fw_access_enable;
	uint32_t *buf;
	uint8_t   word, len;

	buf = (uint32_t *) &mellanox_device_key[0];
	len = MDK_BUF_SIZE;

	/*
	 * Check whether the key bytes are valid, i.e., register bytes are
	 * visible to the ARM side.
	 */
	fw_access_enable =
		mmio_read_32(TYU_BASE_ADDRESS + MDK_FUSE_FW_ACCESS_EN);
	if (fw_access_enable != 0)
		return;

	key_reg = TYU_BASE_ADDRESS + MDK_FUSE_KEY_ADDR;

	/* Bytes are stored from the LSB to the MSB into mellanox_device_key. */
	for (word = 0, offset = 0; offset < (len & ~0x3); offset += 4)
		buf[word++] = mmio_read_32(key_reg + offset);

	mellanox_device_key_is_valid = 1;
	mdkey_dump();
}

/*
 * Perform some static checking and call the library initialization function
 */
void bluefield_auth_mod_init(void)
{
	assert(bluefield_auth_lib_desc.name != NULL);
	assert(bluefield_auth_lib_desc.init != NULL);
	assert(bluefield_auth_lib_desc.verify_hmac != NULL);

	/* Initialize the cryptographic library */
	bluefield_auth_lib_desc.init();
	INFO("Using crypto library '%s'\n", bluefield_auth_lib_desc.name);

	/* Read Mellanox Device Key */
	bluefield_auth_mdkey_read();
}
