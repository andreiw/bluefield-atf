#
# Copyright (c) 2015-2017, ARM Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

ifdef BUILD_BASE
BUILDDIR = ${BUILD_BASE}/tools/cert_create
else
BUILDDIR = .
endif

PROJECT		:= $(BUILDDIR)/cert_create
PLAT		:= none
V		?= 0
DEBUG		:= 0
BINARY		:= ${PROJECT}${BIN_EXT}
OPENSSL_DIR	:= /usr
USE_TBBR_DEFS   := 1

OBJECTS := $(BUILDDIR)/src/cert.o \
           $(BUILDDIR)/src/cmd_opt.o \
           $(BUILDDIR)/src/ext.o \
           $(BUILDDIR)/src/key.o \
           $(BUILDDIR)/src/main.o \
           $(BUILDDIR)/src/sha.o \
           $(BUILDDIR)/src/tbbr/tbb_cert.o \
           $(BUILDDIR)/src/tbbr/tbb_ext.o \
           $(BUILDDIR)/src/tbbr/tbb_key.o

CFLAGS := -Wall -std=c99

MAKE_HELPERS_DIRECTORY := ../../make_helpers/
include ${MAKE_HELPERS_DIRECTORY}build_macros.mk
include ${MAKE_HELPERS_DIRECTORY}build_env.mk

ifeq (${USE_TBBR_DEFS},1)
# In this case, cert_tool is platform-independent
PLAT_MSG		:=	TBBR Generic
PLAT_INCLUDE		:=	../../include/tools_share
else
PLAT_MSG		:=	${PLAT}

TF_PLATFORM_ROOT		:=	../../plat/
include ${MAKE_HELPERS_DIRECTORY}plat_helpers.mk

PLAT_INCLUDE		:=	$(wildcard ${PLAT_DIR}include)

ifeq ($(PLAT_INCLUDE),)
  $(error "Error: Invalid platform '${PLAT}' has no include directory.")
endif
endif

ifeq (${DEBUG},1)
  CFLAGS += -g -O0 -DDEBUG -DLOG_LEVEL=40
else
  CFLAGS += -O2 -DLOG_LEVEL=20
endif
ifeq (${V},0)
  Q := @
else
  Q :=
endif

$(eval $(call add_define,USE_TBBR_DEFS))
CFLAGS += ${DEFINES}

# Make soft links and include from local directory otherwise wrong headers
# could get pulled in from firmware tree.
INC_DIR := -I ./include -I ${PLAT_INCLUDE} -I ${OPENSSL_DIR}/include
LIB_DIR := -L ${OPENSSL_DIR}/lib
LIB := -lssl -lcrypto

HOSTCC ?= gcc

.PHONY: all clean realclean

all: ${BINARY}

${BINARY}: ${OBJECTS} Makefile
	@echo "  LD      $@"
	@echo 'const char build_msg[] = "Built : "__TIME__", "__DATE__; \
                const char platform_msg[] = "${PLAT_MSG}";' | \
                ${CC} -c ${CFLAGS} -xc - -o src/build_msg.o
	${Q}${HOSTCC} src/build_msg.o ${OBJECTS} ${LIB_DIR} ${LIB} -o $@

$(BUILDDIR)/%.o: %.c
	@echo "  CC      $<"
	@mkdir -p $(@D)
	${Q}${HOSTCC} -c ${CFLAGS} ${INC_DIR} $< -o $@

clean:
	$(call SHELL_DELETE_ALL, src/build_msg.o ${OBJECTS})

realclean: clean
	$(call SHELL_DELETE,${BINARY})

