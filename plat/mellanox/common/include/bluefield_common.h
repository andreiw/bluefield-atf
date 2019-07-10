/*
 * Copyright (c) 2019, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __BLUEFIELD_COMMON_H__
#define __BLUEFIELD_COMMON_H__

#include <stdint.h>

void bluefield_bl1_early_platform_setup(void);

void bluefield_common_setup_efi_info(uintptr_t addr);

#endif /* __BLUEFIELD_COMMON_H__ */
