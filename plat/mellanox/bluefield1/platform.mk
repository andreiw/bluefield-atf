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

include plat/mellanox/common/bf_common.mk

# Errata workarounds for Cortex-A72:
ERRATA_A72_859971	:=	1

# By default we build for the hardware system.
TARGET_SYSTEM		:=	hw

# BlueField 1 used version 0 of the images
DEFINES			+=	-DBF_IMG_VER=0

$(eval $(call add_define_val,TARGET_SYSTEM,\"$(TARGET_SYSTEM)\"))

BF_PLAT			:=      plat/mellanox/bluefield1
BF_SYS			:=      ${BF_PLAT}/system/${TARGET_SYSTEM}
BF_SYS_COMMON		:=      ${BF_PLAT}/system/common

PLAT_INCLUDES		+=	-I${BF_PLAT}/include 				\
				-I${BF_PLAT}/include/regs			\
				-I${BF_PLAT}/include/ddr			\

PLAT_BL_COMMON_SOURCES	+=	$(BF_SYS_COMMON)/bluefield_stub.c		\


#BL1_SOURCES		+=

# Common source files used for all BL2 configurations
BL2_SOURCES		+=	${BF_PLAT}/bluefield1_sam_data.c		\
				${BF_PLAT}/bluefield1_bl2_setup.c		\
				$(BF_SYS_COMMON)/bluefield_sam.c

BL31_SOURCES		+=	${BF_PLAT}/bluefield1_svc.c			\
				${BF_PLAT}/ddr/bluefield1_ddr_nvdimm.c		\

ifndef ALT_BL2

    BL2_SOURCES		+=	${BF_PLAT}/ddr/bluefield1_ddr_common.c		\
				${BF_PLAT}/ddr/bluefield1_ddr_nvdimm.c		\
				${BF_PLAT}/ddr/bluefield1_ddr_setup.c		\
				${BF_PLAT}/ddr/bluefield1_ddr_print.c		\
				${BF_PLAT}/ddr/bluefield1_ddr_regs.c		\
				${BF_SYS_COMMON}/bluefield_memory.c		\

    ifeq (${ATF_CONSOLE},1)

        $(eval $(call add_define,ATF_CONSOLE))

        BL2_SOURCES	+=	${BF_PLAT}/ddr/bluefield1_ddr_debug.c		\

    endif

endif

include ${BF_SYS}/system.mk

# Put the different kind of builds in different output directories
ifndef ALT_BL2
    BUILD_PLAT      :=      ${BUILD_BASE}/${PLAT}/${TARGET_SYSTEM}/${BUILD_TYPE}
else
    BUILD_PLAT      :=      ${BUILD_BASE}/${PLAT}/${ALT_BL2}/${BUILD_TYPE}
endif
