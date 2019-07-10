#
# Copyright (c) 2013-2016, ARM Limited and Contributors. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# Neither the name of ARM nor the names of its contributors may be used
# to endorse or promote products derived from this software without specific
# prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

# Ensure our platform works using no deprecated features
ERROR_DEPRECATED	:=	1

# We only support GICv3.
ARM_GIC_ARCH		:=	3

# We prefer not to use DMA for the DesignWare EMMC driver.
DEFINES			+=	-DDWMMC_NO_DMA

# Macro to enable/disable the 2nd eMMC card. The 2nd eMMC card shares the same
# gpio PIN with diag UART. Once enabled, the diag UART will be disabled.
DEFINES		+=	-DENABLE_2ND_EMMC

BF_COM			:=      plat/mellanox/common

PLAT_INCLUDES		:=	-I${BF_COM}/include				\
				-I${BF_COM}/include/drivers/io			\
				-I${BF_COM}/include/ddr				\
				-I${BF_COM}/drivers/i2c				\
				-I${BF_COM}/drivers/tmfifo			\
				-Iinclude/drivers/synopsys			\
				-Iinclude/common/tbbr				\
				-Iinclude/plat/arm/common/aarch64

PLAT_BL_COMMON_SOURCES	:=	${BF_COM}/bluefield_common.c			\
				${BF_COM}/aarch64/bluefield_helpers.S		\
				${BF_COM}/lib/lib.c				\
				${BF_COM}/drivers/tmfifo/tmfifo_console.c	\
				lib/xlat_tables/xlat_tables_common.c		\
				lib/xlat_tables/aarch64/xlat_tables.c		\
				drivers/arm/pl011/aarch64/pl011_console.S


BL1_SOURCES		+=	lib/cpus/aarch64/cortex_a72.S			\
				${BF_COM}/bluefield_bl1_setup.c		\
				${BF_COM}/bluefield_io_storage.c		\
				${BF_COM}/swap_boot.c				\
				${BF_COM}/drivers/io/io_bluefield.c		\
				drivers/arm/ccn/ccn.c				\
				drivers/io/io_storage.c				\
				drivers/emmc/emmc.c				\
				drivers/synopsys/emmc/dw_mmc.c			\
				drivers/delay_timer/delay_timer.c		\
				drivers/delay_timer/generic_delay_timer.c

# Common source files used for all BL2 configurations
BL2_SOURCES		+=	${BF_COM}/bluefield_bl2_setup.c			\
				${BF_COM}/bluefield_bl2_mem_params_desc.c	\
				${BF_COM}/bluefield_io_storage.c		\
				${BF_COM}/drivers/io/io_bluefield.c		\
				${BF_COM}/drivers/io/io_flash.c			\
				${BF_COM}/lib/strtol.c				\
				common/desc_image_load.c			\
				drivers/io/io_storage.c

BL31_SOURCES		+=	lib/cpus/aarch64/cortex_a72.S			\
				${BF_COM}/bluefield_bl31_setup.c		\
				${BF_COM}/ddr/bluefield_ddr_nvdimm.c		\
				${BF_COM}/aarch64/bluefield_serror.S		\
				${BF_COM}/bluefield_pm.c			\
				${BF_COM}/bluefield_topology.c			\
				${BF_COM}/bluefield_gicv3.c			\
				${BF_COM}/bluefield_irq.c			\
				${BF_COM}/bluefield_svc.c			\
				${BF_COM}/drivers/i2c/i2c_smbus.c		\
				drivers/arm/gic/common/gic_common.c		\
				drivers/arm/gic/v3/gicv3_main.c			\
				drivers/arm/gic/v3/gicv3_helpers.c		\
				drivers/delay_timer/delay_timer.c		\
				drivers/delay_timer/generic_delay_timer.c	\
				plat/common/plat_gicv3.c			\
				drivers/arm/ccn/ccn.c				\
				plat/common/plat_psci_common.c

ifndef ALT_BL2

    BL2_SOURCES		+=	drivers/delay_timer/delay_timer.c		\
				drivers/delay_timer/generic_delay_timer.c	\
				${BF_COM}/ddr/bluefield_ddr_nvdimm.c		\
				${BF_COM}/ddr/bluefield_ddr_common.c		\
				${BF_COM}/ddr/bluefield_ddr_params.c		\
				${BF_COM}/ddr/bluefield_sbin_data.c		\
				${BF_COM}/drivers/i2c/i2c_smbus.c		\
				${BF_COM}/ddr/bluefield_ddr_bist.c		\
				${BF_COM}/ddr/bluefield_bist_pattern.c		\

    ifeq (${ATF_CONSOLE},1)

        $(eval $(call add_define,ATF_CONSOLE))

        BL2_SOURCES	+=	${BF_COM}/bluefield_console.c			\
				${BF_COM}/lib/random.c

    endif

else
    ifeq (${ALT_BL2},init_sbkey)
        $(eval $(call add_define,INIT_SBKEY))
    else ifeq (${ALT_BL2},update_sbkey)
        $(eval $(call add_define,UPDATE_SBKEY))
    else ifeq (${ALT_BL2},set_rma)
        $(eval $(call add_define,SET_RMA))
    else
        $(error "Unsupported ALT_BL2 value")
    endif

    $(eval $(call add_define,ALT_BL2))

    ifndef ALT_BL2_DATA_FILE
        $(error "Additional data file for alternate BL2 not provided!")
    endif

    ifeq (${SKIP_FUSE_WARNING},1)
        DEFINES			+=	-DSKIP_FUSE_WARNING
    endif

    BL2_SOURCES		+=	${BF_COM}/bluefield_mod_fuses.c		\
				$(ALT_BL2_DATA_FILE)
endif

# Disable the PSCI platform compatibility layer
ENABLE_PLAT_COMPAT	:= 	0

# We know Bf has the optional CRC instructions, so we use them to validate
# the boot stream.
CFLAGS += -march=armv8-a+crc
ASFLAGS += -march=armv8-a+crc

# Enable PSCI_STAT_COUNT/RESIDENCY APIs on BlueField
ENABLE_PSCI_STAT               :=      1

# Enable Performance Measurement Framework on BlueField
ENABLE_PMF	               :=      1

# Use multi console API
MULTI_CONSOLE_API		:= 1

# Enable using the second UART on BlueField
DEFINES			+=	-DENABLE_SEC_UART

# We only support LOAD_IMAGE_V2 in our platform
LOAD_IMAGE_V2		:=	1

# We rely on this for booting non-secure systems
DEFINES			+=	-DDYN_DISABLE_AUTH

# On BlueField, separate the code and read-only data sections to allow
# mapping the former as executable and the latter as execute-never.
SEPARATE_CODE_AND_RODATA       :=      1

ifneq (${TRUSTED_BOARD_BOOT},0)

    # BlueField BL1 was taped out before ATF tbbr was upgraded to use the new
    # PKCS#1 RSA 2.1 scheme. So in order for Trusted Board Boot to work, we have
    # to use the legacy PKCS#1 RSA 1.5 algorithm.
    KEY_ALG		:=	rsa_1_5

    # Include common TBB sources
    AUTH_SOURCES	:=	drivers/auth/auth_mod.c				\
				drivers/auth/crypto_mod.c			\
				drivers/auth/img_parser_mod.c			\
				drivers/auth/tbbr/tbbr_cot.c			\
				${BF_COM}/bluefield_trusted_boot.c

    PLAT_INCLUDES	+=	-Iinclude/bl1/tbbr				\
				-I${BF_COM}/drivers/auth

    BL1_SOURCES		+=	${AUTH_SOURCES}

    BL2_SOURCES		+=	${AUTH_SOURCES}					\
				${BF_COM}/drivers/io/io_flash_helpers.S	\
				${BF_COM}/drivers/auth/bluefield_auth_mod.c

    # We expect to locate the *.mk files under the directories specified below
    CRYPTO_LIB_MK	:=	drivers/auth/mbedtls/mbedtls_crypto.mk
    CRYPTO_HMAC_LIB_MK	:=	${BF_COM}/drivers/auth/mbedtls/mbedtls_crypto_hmac.mk
    IMG_PARSER_LIB_MK	:=	drivers/auth/mbedtls/mbedtls_x509.mk

    $(info Including ${CRYPTO_LIB_MK})
    include ${CRYPTO_LIB_MK}

    $(info Including ${CRYPTO_HMAC_LIB_MK})
    include ${CRYPTO_HMAC_LIB_MK}

    $(info Including ${IMG_PARSER_LIB_MK})
    include ${IMG_PARSER_LIB_MK}

    # Enable using the second core as a flash DMA engine
    DEFINES		+=	-DFLASH_ENGINE_ENABLED

endif
VERSION_STRING := ${VERSION_STRING}-8cdb15a
