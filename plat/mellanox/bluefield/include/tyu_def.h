/*
 * Copyright (c) 2017, Mellanox Technologies Ltd. All rights reserved.
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

#ifndef __TYU_DEF_H__
#define __TYU_DEF_H__


/*******************************************************************************
 * Auxiliary macros
 ******************************************************************************/
/* Auxiliary macro to get the corresponding field of the given word. */
#define GET_REG_FIELD(reg_field, reg_val)	 \
			(((reg_val) >> reg_field ##_SHIFT) & \
			reg_field ##_RMASK)
/* Auxiliary macro to get the corresponding word with given field updated. */
#define SET_REG_FIELD(reg_field, reg_val, field_val)	(((reg_val) & \
		~((reg_field ##_RMASK) << reg_field ##_SHIFT)) | \
		(((field_val) & (reg_field ##_RMASK)) << \
		reg_field ##_SHIFT))

/*******************************************************************************
 * Boot section
 ******************************************************************************/
#define TYU_LIVEFISH_MODE_ADDR		0x0
#define TYU_LIVEFISH_MODE_RMASK		1
#define TYU_LIVEFISH_MODE_SHIFT		0

/* The offset of the chip devid / revid in the TYU. */
#define TYU_BOOT_DEVID			0x14

#define TYU_BOOT_DEVID_ID_SHIFT		0
#define TYU_BOOT_DEVID_ID_RMASK		0xFFFF
#define TYU_BOOT_DEVID_REV_SHIFT	16
#define TYU_BOOT_DEVID_REV_RMASK	0xF

#define TYU_BOOT_DEVID_ID_BF1		0x211

/* DDR reset related definitions. */
/* The offset of the mss reset register in the TYU. */
#define TYU_MSS_RESET			0x48

#define MSS0_DDR_MC_SRST_EN		(1 << 0)
#define MSS0_DDR_MC_GRST_EN		(1 << 1)
#define MSS0_UMX_MC_DDR_RESET_EN	(1 << 2)
#define MSS1_DDR_MC_SRST_EN		(1 << 3)
#define MSS1_DDR_MC_GRST_EN		(1 << 4)
#define MSS1_UMX_MC_DDR_RESET_EN	(1 << 5)

/*******************************************************************************
 * Bootrecord section
 ******************************************************************************/
#define TYU_OSC_STRAP_EN_ADDR		0x200
#define TYU_OSC_STRAP_EN_RMASK		1
#define TYU_OSC_STRAP_EN_SHIFT		5

/*
 * Baud rate for the console (UART0). The baud rate used will be 150 baud
 * shifted left by the value in this field (so a value of 1 means 300 baud, a
 * value of 6 means 9600 baud, etc.). 0 means no requested speed; in that case
 * BF_DEFAULT_BAUDRATE will be used. Note that other communication
 * parameters for the console are not adjustable; it will always use 8 data
 * bits, no parity, and 1 stop bit.
 */
#define TYU_CONSOLE_SPEED_ADDR		0x320
#define TYU_CONSOLE_SPEED_WIDTH		4
#define TYU_CONSOLE_SPEED_SHIFT		0

/* Place where the user defined memory speed would be stored. */
#define TYU_MSS_SPEED_ADDR		0x320
#define TYU_MSS_SPEED_RMASK		0x7ff
#define TYU_MSS0_SPEED_SHIFT		4
#define TYU_MSS1_SPEED_SHIFT		15

/*
 * The core PLL control for BlueField. There are in total 9 core PLL in
 * BlueField. They control the various clock frequencies used in various
 * components in BlueField.
 */
/* Address in TYU of the register which have the core_bwadj field. */
#define TYU_CORE_PLL_BWADJ(pll_num)		(0x0380 + ((pll_num) << 3))

#define TYU_CORE_PLL_CORE_BWADJ_SHIFT		0
#define TYU_CORE_PLL_CORE_BWADJ_RMASK		0xfff
#define TYU_CORE_PLL_CORE_BWADJ_VAL_REQUIRED	0xf

/*
 * Address in TYU of the register which have the core_{r,od,f,test,bypass,pd}
 * fields.
 */
#define TYU_YU_CORE_PLL_CFG                     0x035c
#define TYU_CORE_PLL_CFG(pll_num)		(0x0384 + ((pll_num) << 3))

#define TYU_CORE_PLL_CORE_R_SHIFT		20
#define TYU_CORE_PLL_CORE_R_RMASK		0x3f

#define TYU_CORE_PLL_CORE_OD_SHIFT		16
#define TYU_CORE_PLL_CORE_OD_RMASK		0xf

#define TYU_CORE_PLL_CORE_F_SHIFT		3
#define TYU_CORE_PLL_CORE_F_RMASK		0x1fff

#define TYU_CORE_PLL_CORE_TEST_SHIFT		2
#define TYU_CORE_PLL_CORE_TEST_RMASK		0x1

#define TYU_CORE_PLL_CORE_BYPASS_SHIFT		1
#define TYU_CORE_PLL_CORE_BYPASS_RMASK		0x1

#define TYU_CORE_PLL_CORE_PD_SHIFT		0
#define TYU_CORE_PLL_CORE_PD_RMASK		0x1

/* Helper function to get the corresponding field of the given word. */
#define GET_CORE_PLL_FIELD(field, reg_val)	 \
			GET_REG_FIELD(TYU_CORE_PLL_ ##field, reg_val)
/* Helper function to get the corresponding word with given field updated. */
#define SET_CORE_PLL_FIELD(field, reg_val, field_val)	 \
			SET_REG_FIELD(TYU_CORE_PLL_ ##field, reg_val, field_val)

/*******************************************************************************
 * Top level section
 ******************************************************************************/

#define TYU_FORCE_CRS_ADDR		0x5a4
#define TYU_FORCE_CRS_RMASK		1
#define TYU_FORCE_CRS_SHIFT		0

/*******************************************************************************
 * Cause section
 ******************************************************************************/
#define TYU_SMBUS_MST_CAUSE_OR_BULK(num)	(0x1218 + ((num) << 5))

#define TYU_SMBUS_MST_CAUSE_ARBITER_BITS(num)	(0x121c + ((num) << 5))

/* These bits are specific to SMBus Master CAUSE_ARBITER_BITS registers. */
#define TYU_CAUSE_ARBITER_TRANSACTION_ENDED     0x001
#define TYU_CAUSE_ARBITER_ARBITRATION_LOST      0x002
#define TYU_CAUSE_ARBITER_UNEXPECTED_START      0x004
#define TYU_CAUSE_ARBITER_UNEXPECTED_STOP       0x008
#define TYU_CAUSE_ARBITER_WAIT_FOR_FW_DATA      0x010
#define TYU_CAUSE_ARBITER_PUT_STOP_FAILED       0x020
#define TYU_CAUSE_ARBITER_PUT_START_FAILED      0x040
#define TYU_CAUSE_ARBITER_CLK_TOGGLE_DONE       0x080
#define TYU_CAUSE_ARBITER_FW_TIMEOUT            0x100
#define TYU_CAUSE_ARBITER_GW_BUSY_FALL          0x200

#define TYU_CAUSE_ARBITER_BITS_MASK             0x000003ff /* 10 bits */

/*******************************************************************************
 * GPIO section
 ******************************************************************************/
#define TYU_GPIO_0_FUNC_EN_0    0x2028  /* Functional enable register */
#define TYU_GPIO_0_FORCE_OE_EN  0x2030  /* Force OE enable register   */

#define TYU_GPIO_LOCK					0x1004
#define TYU_GPIO_LOCK_SET_LOCK				0
#define TYU_GPIO_LOCK_LOCK_ACTIVE			16

#define TYU_GPIO_GPIO0					0x2000
#define TYU_GPIO_GPIO1					0x2100
#define TYU_GPIO_GPIOX_MODE0_OFFSET			0xc
#define TYU_GPIO_GPIOX_MODE1_OFFSET			0x8
#define TYU_GPIO_GPIOX_MODE0_SET_OFFSET			0x54
#define TYU_GPIO_GPIOX_MODE1_CLEAR_OFFSET		0x50
#define TYU_GPIO_GPIOX_FUNCTIONAL_ENABLE0_OFFSET	0x28
#define TYU_GPIO_GPIOX_FUNCTIONAL_ENABLE1_OFFSET	0x24
#define TYU_GPIO_GPIOX_DATA_OUT_OFFSET			0x0

#define TYU_GPIO_INTERNAL_DATAIN		        0x2204

/*******************************************************************************
 * SMbus section
 ******************************************************************************/
/*
 * The details of the SMBus definitions can be found in i2c_smbus.h found in the
 * i2c driver section.
 */
#define TYU_SMBUS_BASE(num)		(0x4000 + ((num) << 11))

/*******************************************************************************
 * Fuse section
 ******************************************************************************/
#define TYU_PG_STROBE_LENGTH		0x6820


#endif /* __TYU_DEF_H__ */
