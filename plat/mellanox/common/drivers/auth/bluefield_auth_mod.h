/*
 * Copyright (c) 2018, Mellanox Technologies. All rights reserved.
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

#ifndef __BLUEFIELD_AUTH_MOD_H__
#define __BLUEFIELD_AUTH_MOD_H__

#include <io_flash.h>
#include <stdint.h>

/* Mellanox Key address; read key bytes from TYU block in 'tyu.mdk_fuse'. */
#define MDK_FUSE_KEY_ADDR       0x1640
#define MDK_FUSE_FW_ACCESS_EN   0x1600

/* Mellanox Device Key size */
#define MDK_BUF_SIZE            64

/* Return values */
enum bluefield_auth_ret_value {
	BLUEFIELD_AUTH_SUCCESS = 0,
	BLUEFIELD_AUTH_ERR_SETUP,
	BLUEFIELD_AUTH_ERR_VERIFY,
	BLUEFIELD_AUTH_ERR_KEY,
	BLUEFIELD_AUTH_ERR_HMAC,
	BLUEFIELD_AUTH_ERR_UNKNOWN
};

/* Flags issued during HMAC verification */
#define AUTH_F_HMAC_START   0x8
#define AUTH_F_HMAC_UPDATE  0x4
#define AUTH_F_HMAC_FINISH  0x2

/*
 * Cryptographic HMAC library descriptor
 */
typedef struct bluefield_auth_lib_desc_s {
	const char *name;

	/*
	 * Initialize library. This function is not expected to fail. All errors
	 * must be handled inside the function, asserting or panicing in case of
	 * a non-recoverable error
	 */
	void (*init)(void);

	/*
	 * Verify a HMAC. Return one of the 'enum bluefield_auth_ret_value'
	 * options
	 */
	int (*verify_hmac)(void *data_ptr, unsigned int data_len,
			   void *digest_info_ptr, unsigned int digest_info_len,
			   void *sk_ptr, unsigned int sk_len,
			   unsigned int flags);
} bluefield_auth_lib_desc_t;

/* Public functions */
void bluefield_auth_mod_init(void);
int bluefield_auth_mod_verify(void);

/* Macro to register a cryptographic library */
#define REGISTER_BLUEFIELD_AUTH_LIB(_name, _init, _verify_hmac)		\
	const bluefield_auth_lib_desc_t bluefield_auth_lib_desc = {	\
		.name = _name,						\
		.init = _init,						\
		.verify_hmac = _verify_hmac				\
	}

#endif /* __BLUEFIELD_AUTH_MOD_H__ */
