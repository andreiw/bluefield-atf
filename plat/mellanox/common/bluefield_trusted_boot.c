/*
 * Copyright (c) 2015, ARM Limited and Contributors. All rights reserved.
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
#include <mmio.h>
#include <platform.h>
#include <platform_def.h>
#include <stdint.h>
#include <string.h>

#include "bluefield_def.h"
#include "rsh.h"

/* SHA256 algorithm */
#define SHA256_BYTES			32

/* We are using the last SHA256_BYTES bytes in the key to store the hash. */
#define BF_TBB_SHA256_START		(BF_TBB_KEY_REG_START + \
					BF_KEY_SIZE_BYTES - SHA256_BYTES)

static const unsigned char rotpk_hash_hdr[] =
		"\x30\x31\x30\x0D\x06\x09\x60\x86\x48"
		"\x01\x65\x03\x04\x02\x01\x05\x00\x04\x20";
static const unsigned int rotpk_hash_hdr_len = sizeof(rotpk_hash_hdr) - 1;
static unsigned char rotpk_hash_der[sizeof(rotpk_hash_hdr) - 1 + SHA256_BYTES];
static unsigned char bluefield_rotpk_hash[SHA256_BYTES];
#ifdef DUMP_SBKEY_REGS
/*
 * Function which dumps out registers related to the efuses for debugging
 * purposes.
 */
static void dump_sbkey_regs(void)
{
	RSH_SB_MODE_t rsb;
	RSH_SB_KEY_VLD_t rskv;
	RSH_SB_KEY_ACCESS_t rska;
	uint64_t rsk[4 * (512 / (8 * sizeof(uint64_t)))];

	rsb.word = mmio_read_64(RSHIM_BASE + RSH_SB_MODE);
	rskv.word = mmio_read_64(RSHIM_BASE + RSH_SB_KEY_VLD);
	rska.word = mmio_read_64(RSHIM_BASE + RSH_SB_KEY_ACCESS);
	for (int i = 0; i < sizeof(rsk) / sizeof(rsk[0]); i++)
		rsk[i] = mmio_read_64(RSHIM_BASE + RSH_SB_KEY_0__FIRST_WORD +
					sizeof(rsk[0]) * i);

	tf_printf("rsh_sb_mode = {\n");
	tf_printf("\t.boot_status = 0x%x\n", rsb.boot_status);
	tf_printf("\t.init_done = 0x%x\n", rsb.init_done);
	tf_printf("\t.test = 0x%x\n", rsb.test);
	tf_printf("\t.secure = 0x%x\n",	rsb.secure);
	tf_printf("}\n");
	tf_printf("rsh_sb_key_access = {\n");
	tf_printf("\t.write_disable = 0x%x\n", rska.write_disable);
	tf_printf("\t.efuse_reread_enable = 0x%x\n", rska.efuse_reread_enable);
	tf_printf("}\n");
	tf_printf("rsh_sb_key_valid = {\n");
	tf_printf("\t.key_0 = 0x%x\n", rskv.key_0);
	tf_printf("\t.key_1 = 0x%x\n", rskv.key_1);
	tf_printf("\t.key_2 = 0x%x\n", rskv.key_2);
	tf_printf("\t.key_3 = 0x%x\n", rskv.key_3);
	tf_printf("}\n");
	for (int i = 0; i < 4; i++) {
		tf_printf("rsh_sb_key_%d[] = {\n",i);
		for (int j = 0; j < 8; j++)
			tf_printf("\t0x%llx,\n", rsk[i * 8 + j]);
		tf_printf("}\n");
	}
}
#endif

/*
 * Check if we are in secure boot mode, if yes load the keys and return 1,
 * else return 0.
 */
static int bluefield_tbb_init(void)
{
	RSH_SB_MODE_t rsb;

	do {
		rsb.word = mmio_read_64(RSHIM_BASE + RSH_SB_MODE);
	} while (!rsb.init_done);

#ifdef DUMP_SBKEY_REGS
	dump_sbkey_regs();
#endif

	/*
	 * There is also an RMA mode which we treat as non-secure
	 * so we have to explicitly see if it is in secure state.
	 */
	if (rsb.secure == BF_CHIP_STATE_SECURE) {
		uint64_t *sha256 = (uint64_t *)bluefield_rotpk_hash;
		for (int i = 0; i < SHA256_BYTES / sizeof(uint64_t); i++)
			sha256[i] = mmio_read_64(BF_TBB_SHA256_START +
						 i * sizeof(uint64_t));
	}

	return rsb.secure == BF_CHIP_STATE_SECURE;
}

/*
 * Return 1 if we are executing trusted board boot or 0 if not executing.
 */
int bluefield_enable_tbb(void)
{
	static int initialized;
	static int enable_tbb;

	if (!initialized) {
		enable_tbb = bluefield_tbb_init();
		initialized = 1;
	}
	return enable_tbb;
}

/*
 * Return the ROTPK hash in the following ASN.1 structure in DER format:
 *
 * AlgorithmIdentifier  ::=  SEQUENCE  {
 *     algorithm         OBJECT IDENTIFIER,
 *     parameters        ANY DEFINED BY algorithm OPTIONAL
 * }
 *
 * DigestInfo ::= SEQUENCE {
 *     digestAlgorithm   AlgorithmIdentifier,
 *     digest            OCTET STRING
 * }
 */
int plat_get_rotpk_info(void *cookie, void **key_ptr, unsigned int *key_len,
			unsigned int *flags)
{
	uint8_t *dst;

	assert(key_ptr != NULL);
	assert(key_len != NULL);
	assert(flags != NULL);

	/* Copy the DER header */
	memcpy(rotpk_hash_der, rotpk_hash_hdr, rotpk_hash_hdr_len);
	dst = (uint8_t *)&rotpk_hash_der[rotpk_hash_hdr_len];

	/* Copy the actual hash */
	memcpy(dst, bluefield_rotpk_hash, SHA256_BYTES);

	*key_ptr = (void *)rotpk_hash_der;
	*key_len = (unsigned int)sizeof(rotpk_hash_der);
	*flags = ROTPK_IS_HASH;
	return 0;
}

/*
 * Return the non-volatile counter value stored in the platform. The cookie
 * will contain the OID of the counter in the certificate.
 *
 * Return: 0 = success, Otherwise = error
 */
int plat_get_nv_ctr(void *cookie, unsigned int *nv_ctr)
{

	*nv_ctr = 0;

	return 0;
}

/*
 * Store a new non-volatile counter value. Right now BlueField doesn't support
 * this capability so it returns error when called.
 *
 * Return: 0 = success, Otherwise = error
 */
int plat_set_nv_ctr(void *cookie, unsigned int nv_ctr)
{
	return 1;
}

