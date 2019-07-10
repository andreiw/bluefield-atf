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
#include <delay_timer.h>
#include <firmware_image_package.h>
#include <io_driver.h>
#include <io_storage.h>
#include <io_bluefield.h>
#include <platform_def.h>
#include <string.h>
#include <utils.h>
#include <mmio.h>

#include "bluefield_private.h"
#include "rsh.h"

/* IO devices */
static const io_dev_connector_t *bf_dev_con;
static uintptr_t bf_dev_handle;


static int open_bf(const uintptr_t spec);
#ifndef ALT_BL2
static int open_bf_try(const uintptr_t spec);
#endif

struct plat_io_policy {
	uintptr_t *dev_handle;
	uintptr_t image_spec;
	int (*check)(const uintptr_t spec);
};

/*
 * BlueField only supports the Bf boot stream method for now, but we retain
 * the policy table in case we want to expand that in the future.
 */
static const struct plat_io_policy policies[] = {
	[BL2_IMAGE_ID] = {
		&bf_dev_handle,
		(uintptr_t)BL2_IMAGE_ID,
		open_bf
	},
	[SCP_BL2_IMAGE_ID] = {
		&bf_dev_handle,
		(uintptr_t)SCP_BL2_IMAGE_ID,
		open_bf
	},
	[BL31_IMAGE_ID] = {
		&bf_dev_handle,
		(uintptr_t)BL31_IMAGE_ID,
		open_bf
	},
	[BL32_IMAGE_ID] = {
		&bf_dev_handle,
		(uintptr_t)BL32_IMAGE_ID,
		open_bf
	},
	[BL33_IMAGE_ID] = {
		&bf_dev_handle,
		(uintptr_t)BL33_IMAGE_ID,
		open_bf
	},
#ifndef ALT_BL2
	[BF_SYS_FILE] = {
		&bf_dev_handle,
		(uintptr_t)BF_SYS_FILE,
		open_bf_try
	},
#endif
#if TRUSTED_BOARD_BOOT
	[TRUSTED_BOOT_FW_CERT_ID] = {
		&bf_dev_handle,
		(uintptr_t)TRUSTED_BOOT_FW_CERT_ID,
		open_bf
	},
	[TRUSTED_KEY_CERT_ID] = {
		&bf_dev_handle,
		(uintptr_t)TRUSTED_KEY_CERT_ID,
		open_bf
	},
	[SCP_FW_KEY_CERT_ID] = {
		&bf_dev_handle,
		(uintptr_t)SCP_FW_KEY_CERT_ID,
		open_bf
	},
	[SOC_FW_KEY_CERT_ID] = {
		&bf_dev_handle,
		(uintptr_t)SOC_FW_KEY_CERT_ID,
		open_bf
	},
	[TRUSTED_OS_FW_KEY_CERT_ID] = {
		&bf_dev_handle,
		(uintptr_t)TRUSTED_OS_FW_KEY_CERT_ID,
		open_bf
	},
	[NON_TRUSTED_FW_KEY_CERT_ID] = {
		&bf_dev_handle,
		(uintptr_t)NON_TRUSTED_FW_KEY_CERT_ID,
		open_bf
	},
	[SCP_FW_CONTENT_CERT_ID] = {
		&bf_dev_handle,
		(uintptr_t)SCP_FW_CONTENT_CERT_ID,
		open_bf
	},
	[SOC_FW_CONTENT_CERT_ID] = {
		&bf_dev_handle,
		(uintptr_t)SOC_FW_CONTENT_CERT_ID,
		open_bf
	},
	[TRUSTED_OS_FW_CONTENT_CERT_ID] = {
		&bf_dev_handle,
		(uintptr_t)TRUSTED_OS_FW_CONTENT_CERT_ID,
		open_bf
	},
	[NON_TRUSTED_FW_CONTENT_CERT_ID] = {
		&bf_dev_handle,
		(uintptr_t)NON_TRUSTED_FW_CONTENT_CERT_ID,
		open_bf
	},
#endif /* TRUSTED_BOARD_BOOT */
};

static int open_bf(const uintptr_t spec)
{
	int result;
	uintptr_t local_image_handle;

	/* See if the file exists in the boot stream. */
	result = io_dev_init(bf_dev_handle, (uintptr_t)NULL);
	if (result == 0) {
		result = io_open(bf_dev_handle, spec, &local_image_handle);
		if (result == 0) {
			VERBOSE("Using Bf boot stream\n");
			io_close(local_image_handle);
		}
	}
	return result;
}

#ifndef ALT_BL2
/* Do not wait forever if nothing shows up in boot fifo when waiting for file */
static int open_bf_try(const uintptr_t spec)
{
	for (int i = 0; i < 10; i++) {
		if (mmio_read_64(RSHIM_BASE + RSH_BOOT_FIFO_COUNT) >= 3)
			break;
		mdelay(100);
	}

	if (mmio_read_64(RSHIM_BASE + RSH_BOOT_FIFO_COUNT) < 3)
		return -ENOENT;

	return open_bf(spec);
}
#endif

void bluefield_io_setup(void)
{
	int io_result;

	/* Register the IO devices on this platform */
	io_result = register_io_dev_bf(&bf_dev_con);
	assert(io_result == 0);

	/* Open connections to devices and cache the handles */
	io_result = io_dev_open(bf_dev_con, (uintptr_t)NULL, &bf_dev_handle);
	assert(io_result == 0);

	/* Ignore improbable errors in release builds */
	(void)io_result;
}

int bluefield_get_alt_image_source(
	unsigned int image_id __unused,
	uintptr_t *dev_handle __unused,
	uintptr_t *image_spec __unused)
{
	/* By default do not try an alternative */
	return -ENOENT;
}

/*
 * Return an IO device handle and specification which can be used to access
 * an image.  Use this to enforce platform load policy.
 */
int plat_get_image_source(unsigned int image_id, uintptr_t *dev_handle,
			  uintptr_t *image_spec)
{
	int result;
	const struct plat_io_policy *policy;

	assert(image_id < ARRAY_SIZE(policies));

	policy = &policies[image_id];
	result = policy->check(policy->image_spec);
	if (result == 0) {
		*image_spec = policy->image_spec;
		*dev_handle = *(policy->dev_handle);
	} else {
		VERBOSE("Trying alternative IO\n");
		result = bluefield_get_alt_image_source(image_id, dev_handle,
						       image_spec);
	}

	return result;
}
