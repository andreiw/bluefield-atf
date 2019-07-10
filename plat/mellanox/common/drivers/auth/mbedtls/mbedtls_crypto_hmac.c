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


#include <bluefield_auth_mod.h>
#include <debug.h>
#include <mbedtls_common.h>
#include <stddef.h>
#include <string.h>
/* mbed TLS headers */
#include <mbedtls/md.h>

#define LIB_NAME		"Bluefield mbed TLS"

/*
 * The BlueField crypto HMAC library is responsible for verifying hashes.
 * It relies on mbed TLS library to perform the cryptographic operations.
 *
 * The BlueField crypto HMAC library itself does not impose any specific
 * format on hashes or data, but byte order should be considered in order
 * to compute the HMAC through the cryptographic libraries.
 * This might be updated when we get more information from NIC Firmware
 * folks.
 *
 */

typedef struct {
	mbedtls_md_context_t     ctx;       /* Generic message-digest context */
	mbedtls_md_type_t        md_alg;    /* Supported message digests */
	const mbedtls_md_info_t *md_info;   /* Message digest information */
	int                      ready;     /* Whether the context is ready */
} crypto_hmac_ctx_t;

static crypto_hmac_ctx_t gbl_crypto_hmac_ctx;

/*
 * Initialize the library and export the descriptor
 */
static void init(void)
{
	/* Initialize mbed TLS */
	mbedtls_init();
}

/*
 * This function initializes a message-digest context and binds it to
 * a particular message-digest algorithm.
 */
static int setup_hmac(crypto_hmac_ctx_t *hmac_ctx,
		      mbedtls_md_type_t  md_alg,
		      unsigned int       digest_len)
{
	int rc, ret;

	ret = BLUEFIELD_AUTH_SUCCESS;

	hmac_ctx = &gbl_crypto_hmac_ctx;

	/* Check whether the context is ready to use */
	if (hmac_ctx->ready)
		return ret;

	/*
	 * First, initialize an md_context without binding it to a particular
	 * message digest algorithm.
	 */
	mbedtls_md_init(&hmac_ctx->ctx);

	/*
	 * Read the message digest information associated with the given
	 * digest type.
	 */
	hmac_ctx->md_alg  = md_alg;
	hmac_ctx->md_info = mbedtls_md_info_from_type(md_alg);
	if (hmac_ctx->md_info == NULL) {
		ret = BLUEFIELD_AUTH_ERR_SETUP;
		goto err;
	}

	/* Length of hash must match the algorithm's size */
	if (digest_len != mbedtls_md_get_size(hmac_ctx->md_info)) {
		ret = BLUEFIELD_AUTH_ERR_SETUP;
		goto err;
	}

	/*
	 * Finally, bind the md_context to a message-digest algorithm;
	 * select algorithm to use and allocate internal structures.
	 */
	rc = mbedtls_md_setup(&hmac_ctx->ctx, hmac_ctx->md_info, 1);
	if (rc == 0) {
		hmac_ctx->ready = 1; /* Mark the context as ready */
		goto end;
	}

err:
	/* Free and clear the internal structures of ctx. */
	mbedtls_md_free(&hmac_ctx->ctx);
	hmac_ctx = NULL;
end:
	return ret;
}

/* Return an initialized message-digest context */
static inline
crypto_hmac_ctx_t *get_hmac_context(mbedtls_md_type_t md_alg,
				    unsigned int      digest_len)
{
	crypto_hmac_ctx_t *hmac_ctx = &gbl_crypto_hmac_ctx;
	int rc = BLUEFIELD_AUTH_SUCCESS;

	/* Return if the context is ready to use */
	if (!hmac_ctx->ready) {
		if (rc != setup_hmac(hmac_ctx, md_alg, digest_len))
			return NULL;
	}

	return hmac_ctx;
}

/* Release the message-digest context */
static inline void put_hmac_context(crypto_hmac_ctx_t *hmac_ctx)
{
	if (hmac_ctx) {
		/* Free and clear the internal structures of ctx. */
		mbedtls_md_free(&hmac_ctx->ctx);
		hmac_ctx->ready = 0;
	}
}

/*
 * Compute the HMAC signature of the given data and return whether
 * a match is found.
 */
static int verify_hmac(void         *data_ptr,
		       unsigned int  data_len,
		       void         *digest_ptr,
		       unsigned int  digest_len,
		       void         *sk_ptr,
		       unsigned int  sk_len,
		       unsigned int  flags)
{
	crypto_hmac_ctx_t *hmac_ctx;
	unsigned char data_hash[MBEDTLS_MD_MAX_SIZE];
	int rc, ret;

	ret = BLUEFIELD_AUTH_ERR_HMAC;
	rc  = 0;

	/* This should always be called first */
	hmac_ctx = get_hmac_context(MBEDTLS_MD_SHA512, digest_len);
	if (!hmac_ctx)
		return ret;

	switch (flags) {
	case AUTH_F_HMAC_START:
		/* Set HMAC key and prepare to authenticate the data. */
		rc  = mbedtls_md_hmac_starts(&hmac_ctx->ctx,
			      (const unsigned char *) sk_ptr, sk_len);
		/* fall-through */
	case AUTH_F_HMAC_UPDATE:
		/* Generic HMAC process buffer. */
		rc |= mbedtls_md_hmac_update(&hmac_ctx->ctx,
				 (const unsigned char *) data_ptr, data_len);
		break;
	case AUTH_F_HMAC_FINISH:
		/* Calculate the final HMAC, then compare digests. */
		rc  = mbedtls_md_hmac_finish(&hmac_ctx->ctx, data_hash);
		if (rc == 0)
			rc = memcmp(data_hash, digest_ptr,
				    mbedtls_md_get_size(hmac_ctx->md_info));
		break;
	}

	/*
	 * Verify whether authentication operations are completed without
	 * errors.
	 */
	if (rc == 0) {
		ret = BLUEFIELD_AUTH_SUCCESS;
	}

	if ((rc != 0) || (flags == AUTH_F_HMAC_FINISH)){
		/* Release the message-digest context and return. */
		put_hmac_context(hmac_ctx);
	}

	return ret;
}

/*
 * Register crypto library descriptor
 */
REGISTER_BLUEFIELD_AUTH_LIB(LIB_NAME, init, verify_hmac);
