#ifndef __REGS_EMI_H__
#define __REGS_EMI_H__

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif
#include "emi_def.h"

#ifndef __ASSEMBLER__


#ifndef __DOXYGEN__

/*
 * Device Info.
 * This register provides general information about the device attached to
 * this port and channel.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Encoded device Type. */
		uint64_t type         : 12;
		/* Reserved. */
		uint64_t __reserved_0 : 4;
		/* Device revision ID. */
		uint64_t device_rev   : 8;
		/* Register format architectural revision. */
		uint64_t register_rev : 4;
		/* Reserved. */
		uint64_t __reserved_1 : 4;
		/* Instance ID for multi-instantiated devices. */
		uint64_t instance     : 4;
		/* Reserved. */
		uint64_t __reserved_2 : 28;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_2 : 28;
		uint64_t instance     : 4;
		uint64_t __reserved_1 : 4;
		uint64_t register_rev : 4;
		uint64_t device_rev   : 8;
		uint64_t __reserved_0 : 4;
		uint64_t type         : 12;
#endif
	};

	uint64_t word;
} EMI_DEV_INFO_t;


/*
 * Device Control.
 * This register provides general device control.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * When 1, packets sent on the NDN will be routed x-first.
		 * When 0, packets will be routed y-first.  This setting must
		 * match the setting in the Tiles.  Devices may have
		 * additional interfaces with customized route-order settings
		 * used in addition to or instead of this field.
		 */
		uint64_t ndn_route_order : 1;
		/*
		 * When 1, packets sent on the CDN will be routed x-first.
		 * When 0, packets will be routed y-first.  This setting must
		 * match the setting in the Tiles.  Devices may have
		 * additional interfaces with customized route-order settings
		 * used in addition to or instead of this field.
		 */
		uint64_t cdn_route_order : 1;
		/*
		 * When 1, packets sent on the DDN will be routed x-first.
		 * When 0, packets will be routed y-first.  This setting must
		 * match the setting in the Tiles.  Devices may have
		 * additional interfaces with customized route-order settings
		 * used in addition to or instead of this field.
		 */
		uint64_t ddn_route_order : 1;
		/*
		 * When 1, the ExpCompAck flow will be used on DMA reads
		 * which allows read-data-bypass for lower latency. Must only
		 * be changed if no DMA read traffic is inflight.
		 */
		uint64_t dma_rd_ca_ena   : 1;
		/*
		 * For devices with DMA. When 1, the L3 cache profile will be
		 * forced to L3_PROFILE_VAL. When 0, the L3 profile is
		 * selected by the device.
		 */
		uint64_t l3_profile_ovd  : 1;
		/*
		 * For devices with DMA. L3 cache profile to be used when
		 * L3_PROFILE_OVD is 1.
		 */
		uint64_t l3_profile_val  : 4;
		/* Write response mapping for MMIO slave errors */
		uint64_t wr_slverr_map   : 2;
		/* Write response mapping for MMIO decode errors */
		uint64_t wr_decerr_map   : 2;
		/* Read response mapping for MMIO slave errors */
		uint64_t rd_slverr_map   : 2;
		/* Read response mapping for MMIO decode errors */
		uint64_t rd_decerr_map   : 2;
		/*
		 * When 1, the CDN sync FIFO is allowed to back pressure
		 * until full to avoid retries and improve performance
		 */
		uint64_t cdn_req_buf_ena : 1;
		/* Reserved. */
		uint64_t __reserved_0    : 2;
		/*
		 * For diagnostics only. Block new traffic when WRQ_INFL
		 * count exceeds this threshold
		 */
		uint64_t dma_wrq_hwm     : 8;
		/* For diagnostics only. Adjust packet gather delay on RNF */
		uint64_t gthr_delay_adj  : 4;
		/* Reserved. */
		uint64_t __reserved_1    : 32;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_1    : 32;
		uint64_t gthr_delay_adj  : 4;
		uint64_t dma_wrq_hwm     : 8;
		uint64_t __reserved_0    : 2;
		uint64_t cdn_req_buf_ena : 1;
		uint64_t rd_decerr_map   : 2;
		uint64_t rd_slverr_map   : 2;
		uint64_t wr_decerr_map   : 2;
		uint64_t wr_slverr_map   : 2;
		uint64_t l3_profile_val  : 4;
		uint64_t l3_profile_ovd  : 1;
		uint64_t dma_rd_ca_ena   : 1;
		uint64_t ddn_route_order : 1;
		uint64_t cdn_route_order : 1;
		uint64_t ndn_route_order : 1;
#endif
	};

	uint64_t word;
} EMI_DEV_CTL_t;


/* Scratchpad. */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Scratchpad. */
		uint64_t scratchpad : 64;
#else   /* __BIG_ENDIAN__ */
		uint64_t scratchpad : 64;
#endif
	};

	uint64_t word;
} EMI_SCRATCHPAD_t;


/*
 * Semaphore0.
 * Semaphore
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * When read, the current semaphore value is returned and the
		 * semaphore is set to 1.  Bit can also be written to 1 or 0.
		 */
		uint64_t val        : 1;
		/* Reserved. */
		uint64_t __reserved : 63;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 63;
		uint64_t val        : 1;
#endif
	};

	uint64_t word;
} EMI_SEMAPHORE0_t;


/*
 * DMA Status.
 * DMA status information for debug. Unused for devices that do not have DMA.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		uint64_t rdq_infl_count : 9;
		uint64_t wrq_infl_count : 9;
		/* Reserved. */
		uint64_t __reserved     : 7;
		/* Internal diagnostics status */
		uint64_t wrq_diag_vec   : 39;
#else   /* __BIG_ENDIAN__ */
		uint64_t wrq_diag_vec   : 39;
		uint64_t __reserved     : 7;
		uint64_t wrq_infl_count : 9;
		uint64_t rdq_infl_count : 9;
#endif
	};

	uint64_t word;
} EMI_DMA_STATUS_t;


/* Clock Count. */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * When 1, the counter is running.  Cleared by HW once count
		 * is complete.  When written with a 1, the count sequence is
		 * restarted.  Counter runs automatically after reset.
		 * Software must poll until this bit is zero, then read the
		 * CLOCK_COUNT register again to get the final COUNT value.
		 */
		uint64_t run        : 1;
		/*
		 * Indicates the number of core clocks that were counted
		 * during 1000 device clock periods.  Result is accurate to
		 * within +/-1 core clock periods.
		 */
		uint64_t count      : 15;
		/* Reserved. */
		uint64_t __reserved : 48;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 48;
		uint64_t count      : 15;
		uint64_t run        : 1;
#endif
	};

	uint64_t word;
} EMI_CLOCK_COUNT_t;


/*
 * Interrupt Setup.
 * Configuration for device interrupts.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Global enable. Must be 1 for any interrupts to be sent. */
		uint64_t gbl_ena      : 1;
		/* Reserved. */
		uint64_t __reserved   : 47;
		/*
		 * Base interrupt number. The interrupt number within the
		 * device will added to this number to form the interrupt
		 * number sent to the GIC.
		 */
		uint64_t base_int_num : 16;
#else   /* __BIG_ENDIAN__ */
		uint64_t base_int_num : 16;
		uint64_t __reserved   : 47;
		uint64_t gbl_ena      : 1;
#endif
	};

	uint64_t word;
} EMI_INT_SETUP_t;


/*
 * Feature Control.
 * Device-specific feature controls.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Feature Control.
		 * Device-specific feature controls.
		 */
		uint64_t feature_ctl : 64;
#else   /* __BIG_ENDIAN__ */
		uint64_t feature_ctl : 64;
#endif
	};

	uint64_t word;
} EMI_FEATURE_CTL_t;


/*
 * Credit Control.
 * Provides access to the request-credit counters that control end-to-end
 * flow control between the device and other nodes in the system.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * When written with a 1, the associated credit counter will
		 * be updated and the resulting value will be placed in VAL.
		 * This register clears once the access has occurred.
		 * Software must poll UPDATE until is is clear before VAL
		 * will be valid. A counter can be read without modifying it
		 * by setting VAL=0.
		 */
		uint64_t update     : 1;
		/*
		 * When UPDATE is written with a 1, this register determines
		 * which credit counter is accessed. For ring targets, this
		 * is {DIR,Link[3:0],CHANNEL[3:0]}
		 */
		uint64_t nodeid_sel : 11;
		/* Select target block. Not all nodes support all blocks. */
		uint64_t tgt_sel    : 2;
		/*
		 * For nodes with multiple DMA clients, this selects the
		 * client being accessed.
		 */
		uint64_t client_sel : 2;
		/*
		 * When UPDATE is written with a 1, this signed number will
		 * be added to the selected credit counter. The resulting
		 * value is provided in this register.
		 */
		uint64_t val        : 16;
		/* Reserved. */
		uint64_t __reserved : 32;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 32;
		uint64_t val        : 16;
		uint64_t client_sel : 2;
		uint64_t tgt_sel    : 2;
		uint64_t nodeid_sel : 11;
		uint64_t update     : 1;
#endif
	};

	uint64_t word;
} EMI_CRED_CTL_t;


/*
 * SAM Control.
 * Provides access to SAM initialization.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * When written with a 1, the associated table will be
		 * accessed. This register clears once the access has
		 * occurred. Software must poll UPDATE until is is clear
		 * before VAL will be valid.
		 */
		uint64_t update     : 1;
		/*
		 * When UPDATE is written with a 1, this register determines
		 * which entry is accessed.
		 */
		uint64_t idx        : 12;
		/* Structure to be accessed. */
		uint64_t tbl_sel    : 3;
		/*
		 * For nodes with multiple DMA clients, this selects the
		 * client being accessed.
		 */
		uint64_t client_sel : 1;
		/*
		 * When 1, the entry will not be updated and VAL will return
		 * the table data.
		 */
		uint64_t read       : 1;
		/*
		 * When UPDATE is written with a 1, this value will be
		 * written to the selected table entry.
		 */
		uint64_t val        : 34;
		/* Reserved. */
		uint64_t __reserved : 12;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 12;
		uint64_t val        : 34;
		uint64_t read       : 1;
		uint64_t client_sel : 1;
		uint64_t tbl_sel    : 3;
		uint64_t idx        : 12;
		uint64_t update     : 1;
#endif
	};

	uint64_t word;
} EMI_SAM_CTL_t;


/*
 * IND_DATA.
 * Indirect Data: Data to write to the indirect register/SRAM or data read
 * from the indirect data register/SRAM
 *
 * IND_DATA_0 - First part of data.
 *
 * Summary:
 * The indirect register mechanism is used for accessing SRAMs and indirect
 * registers. Indirect registers are 32-bits wide, internally divided into
 * multiple fields. However, they do not have an address directly mapped to
 * the host configuration memory space. Instead, they are accessed indirectly
 * through the indirect access interface registers. SRAMs are memory
 * structures consisting of several addressable SRAM lines. Each SRAM line
 * can be made up of several fields.
 *
 * EZcp APIs for indirect register.
 * -       EZapiPrm_WriteReg
 * -       EZapiPrm_ReadReg
 * -       EZapiPrm_RMWReg
 *
 * EZcp APIs for SRAMs.
 * -       EZapiPrm_WriteSRAM
 * -       EZapiPrm_ReadSRAM
 * -       EZapiPrm_RMWSRAM
 *
 * Read Sequence Indirect Registers:
 * 1.      Specify the indirect register address to read - IND_ADDR.
 * 2.      Specify the indirect register command - IND_CMD.
 * 3.      Check the IND_CMD status - IND_STS.
 * 4.      Read the requested data - IND_DATA.
 *
 * Write Sequence Indirect Registers:
 * 1.      Specify the data to write - IND_DATA.
 * 2.      Specify the indirect register address to write - IND_ADDR.
 * 3.      Specify the indirect register command - IND_CMD.
 * 4.      Check the IND_CMD status - IND_STS.
 *
 * Read Sequence SRAMs:
 * 1.      Specify the SRAM line to read - IND_ADDR.
 * 2.      Specify the indirect SRAM command and SRAM ID - IND_CMD.
 * 3.      Check the IND_CMD status - IND_STS.
 * 4.      Read the requested data - IND_DATA.
 *
 * Write Sequence SRAMs:
 * 1.      Specify the data to write - IND_DATA.
 * 2.      Specify the first SRAM line to write - IND_ADDR.
 * 3.      Specify the end SRAM line if relevant - IND_CMD_RANGE if field OP
 * = 0x4/0x5 in register IND_CMD.
 * 4.      Specify the SRAM fields to modify if relevant - IND_CMD_BITMAP if
 * field OP = 0x3/0x5 in register IND_CMD.
 * 5.      Specify the indirect register command and SRAM ID - IND_CMD.
 * 6.      Check the IND_CMD status - IND_STS.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Value:
		 * - SRAM Self-init: Bitmap of SRAMs to initialize (line (OP
		 * = 0x0 in register IND_CMD). E.g. if the b0 = 1 in the SRAM
		 * bitmap this indicates the first SRAM in the block.
		 * - SRAM: Data to write to SRAM or data read from the SRAM
		 * (OP = 0x1/2/3/4/5 in register IND_CMD).
		 * - Indirect Register: Data to write to the indirect
		 * register or data read from the indirect data register (OP
		 * = 0x8/0x9 in register IND_CMD).
		 */
		uint32_t val : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t val : 32;
#endif
	};

	uint32_t word;
} EMI_IND_DATA_t;


/*
 * IND_CMD.
 * Indirect Command: Indirect register/SRAM command. The status is in
 * register IND_STS.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Operation:
		 * 0x0 - SRAM self-init sets to RegDB Cfg Val (configuration
		 * value) for SRAMs specified in IND_DATA registers.
		 * 0x1 - SRAM read
		 * 0x2 - SRAM write
		 * 0x3 - SRAM modify for selected SRAM fields in register
		 * IND_CMD_BITMAP.
		 * 0x4 - SRAM write range. SRAM start-line in register
		 * IND_ADDR and end-line in register IND_CMD_RANGE.
		 * 0x5 - SRAM modify range for selected SRAM fields in
		 * register IND_CMD_BITMAP. SRAM start-line in register
		 * IND_ADDR and end-line in register IND_CMD_RANGE.
		 * 0x6 - 0x7 - RESERVED.
		 * 0x8 - Indirect register read.
		 * 0x9 - Indirect register write.
		 * 0xA - Indirect command as defined in RegDB Ind Cmds
		 * (EZchip internal use only).
		 * 0xB - 0xFF - RESERVED.
		 */
		uint32_t op       : 8;
		/*
		 * The indirect register or SRAM's group as specified in
		 * RegDB.
		 */
		uint32_t group    : 8;
		/*
		 * The indirect register or SRAM's GroupInstances as
		 * specified in RegDB
		 */
		uint32_t instance : 8;
		/* The SRAM's Id as specified in RegDB */
		uint32_t id       : 8;
#else   /* __BIG_ENDIAN__ */
		uint32_t id       : 8;
		uint32_t instance : 8;
		uint32_t group    : 8;
		uint32_t op       : 8;
#endif
	};

	uint32_t word;
} EMI_IND_CMD_t;


/*
 * IND_CMD_RANGE.
 * Indirect Command Range: The SRAM end-line address for SRAM write/modify
 * range.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Memory End Address: The SRAM end-line address for SRAM
		 * write/modify range
		 */
		uint32_t mem_end_addr : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t mem_end_addr : 32;
#endif
	};

	uint32_t word;
} EMI_IND_CMD_RANGE_t;


/*
 * IND_ADDR.
 * Indirect Address. Specifies the indirect register address or SRAM
 * line/start-line
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Value:
		 * -       For SRAM read/write/modify single operations -
		 * line to access.
		 * -       For SRAM write/modify range operations - SRAM
		 * start-line of operation. The SRAM end-line is defined in
		 * register IND_CMD_RANGE.
		 * -       For Indirect registers - Address as specified in
		 * RegDB.
		 */
		uint32_t val : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t val : 32;
#endif
	};

	uint32_t word;
} EMI_IND_ADDR_t;


/*
 * IND_STS.
 * Indirect Status: Indicates the status of the IND_CMD
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Ready: Indicates that the command completed and a new
		 * command can be generated.
		 * 0 - Busy.
		 * 1 - Done.
		 * This bit is updated on every write (register IND_CMD) to
		 * the command register.
		 */
		uint32_t rdy        : 1;
		/*
		 * Indicates if the last command succeeded or failed.
		 * 0 - Error.
		 * 1 - Success.
		 * SUCCESS bit should be referred only when bit RDY = 1.
		 */
		uint32_t success    : 1;
		/* Reserved. */
		uint32_t __reserved : 30;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 30;
		uint32_t success    : 1;
		uint32_t rdy        : 1;
#endif
	};

	uint32_t word;
} EMI_IND_STS_t;


/*
 * SPARE.
 * Spare register
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Spare */
		uint32_t val : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t val : 32;
#endif
	};

	uint32_t word;
} EMI_SPARE_t;


/*
 * ECC_SINGLE_ERROR_CNT.
 * EZcp API: EZapiPrm_readReg
 *
 * ECC Single Error Counter: Provides a total single error counter for all
 * SRAM memories that have ECC. Clear on read. No wrap around.
 *
 * Soft Errors
 *
 * Soft errors are a transient random "bit-flip" of memory cells. It is an
 * event that corrupts the value stored in a memory cell without damaging the
 * cell itself.
 *
 * Soft errors are caused by the following events:
 *
 * -       Alpha particles (helium nucleus of 2 protons +2 neutrons)
 * emissions from chip packaging. This has been largely eliminated by chip
 * manufacturers.
 * -       Cosmic rays (high-energy particle originating in outer space). At
 * ground level, cosmic radiation is about 95% neutrons and 5% protons. These
 * particles can cause soft errors directly; and they can also interact with
 * atomic nuclei to produce troublesome short-range heavy ions.
 *
 * SRAM and DRAM Soft Error Information Description
 *
 * -       All SRAMs and DRAMs have ECC SECDED (Single Error Correction
 * Double Error Detection). SRAMs can also have parity instead of ECC. The
 * ECC is using the Hamming code algorithm with one additional parity bit for
 * double error detection.
 * -       All SRAMs and DRAMs with ECC protection have single and double
 * error indication via interrupt.
 * -       All SRAMs and DRAMs with ECC protection have double and single
 * error injection.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Value: ECC single errors counter. */
		uint32_t val        : 4;
		/* Reserved. */
		uint32_t __reserved : 28;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 28;
		uint32_t val        : 4;
#endif
	};

	uint32_t word;
} EMI_ECC_SINGLE_ERROR_CNT_t;


/*
 * ECC_DOUBLE_ERROR_CNT.
 * EZcp API: EZapiPrm_readReg
 *
 * ECC Double Error Counter: Provides a total double error counter for all
 * SRAM memories that have ECC. Clear on read. No wrap around.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Value: ECC double errors counter. */
		uint32_t val        : 4;
		/* Reserved. */
		uint32_t __reserved : 28;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 28;
		uint32_t val        : 4;
#endif
	};

	uint32_t word;
} EMI_ECC_DOUBLE_ERROR_CNT_t;


/*
 * ECC_ADDR_ERROR_SELECT.
 * EZcp API: EZapiPrm_SERCheckError
 *
 * ECC Address Error Select: Specifies the ECC ID of the SRAM (ECCID)
 * according to register ECC_SINGLE_ERROR / ECC_DOUBLE_ERROR for register
 * ECC_ADDR_ERROR_DATA.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* ECC ID Write Data: ECC ID of the SRAM */
		uint32_t eccid_wdata : 2;
		/* Reserved. */
		uint32_t __reserved  : 26;
		/*
		 * ECC split number (ECC Split) (ECC may cover sections of an
		 * SRAM line).
		 */
		uint32_t index       : 4;
#else   /* __BIG_ENDIAN__ */
		uint32_t index       : 4;
		uint32_t __reserved  : 26;
		uint32_t eccid_wdata : 2;
#endif
	};

	uint32_t word;
} EMI_ECC_ADDR_ERROR_SELECT_t;


/*
 * ECC_ADDR_ERROR_DATA_LOW.
 * EZcp API: EZapiPrm_SERCheckError
 *
 * ECC Address Error Data Low: Provides the ECC error address (line number)
 * for the SRAM ECC ID specified in register ECC_ADDR_ERROR_SELECT (even it
 * does not match field LAST_ERR in registers ECC_ID_FIRST_LAST_SERR/
 * ECC_ID_FIRST_LAST_DERR). Register ECC_LATCH_MODE_0 controls if first or
 * last ECC error is stored.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* SRAM line number. */
		uint32_t address    : 16;
		/* Reserved. */
		uint32_t __reserved : 16;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 16;
		uint32_t address    : 16;
#endif
	};

	uint32_t word;
} EMI_ECC_ADDR_ERROR_DATA_LOW_t;


/*
 * ECC_ADDR_ERROR_DATA_HIGH.
 * EZcp API: EZapiPrm_SERCheckError
 *
 * ECC Address Error Data High: Provides ECC error additional information for
 * the SRAM specified in register ECC_ADDR_ERROR_SELECT.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* ECC syndrome (error bit according to the Hamming code). */
		uint32_t syndrom         : 16;
		/* Reserved. */
		uint32_t __reserved_0    : 8;
		/*
		 * ECC split number (ECC Split) (ECC may cover sections of an
		 * SRAM line).
		 */
		uint32_t index           : 4;
		/* Reserved. */
		uint32_t __reserved_1    : 1;
		/*
		 * 0 - No multiple errors.
		 * 1 - Multiple errors.
		 */
		uint32_t multiple_errors : 1;
		/*
		 * Single Double Bit.
		 * 0 - Single bit error.
		 * 1 - Double bit error.
		 */
		uint32_t s_d_bit         : 1;
		/*
		 * 1 - The data in registers ECC_ADDR_ERROR_DATA_LOW and
		 * ECC_ADDR_ERROR_DATA_HIGH have been updated from the SRAM
		 * selected by register ECC_ADDR_ERROR_SELECT.
		 */
		uint32_t data_ready      : 1;
#else   /* __BIG_ENDIAN__ */
		uint32_t data_ready      : 1;
		uint32_t s_d_bit         : 1;
		uint32_t multiple_errors : 1;
		uint32_t __reserved_1    : 1;
		uint32_t index           : 4;
		uint32_t __reserved_0    : 8;
		uint32_t syndrom         : 16;
#endif
	};

	uint32_t word;
} EMI_ECC_ADDR_ERROR_DATA_HIGH_t;


/*
 * SERR_INJ.
 * EZcp API: EZapiPrm_writeReg
 *
 * Single Error Injection: Injects a single error (flips one data bit) for
 * specified SRAM ECCID during a single write. ECCID is defined in
 * ECC_SINGLE_ERROR_0.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* ECC ID of the SRAM (ECCID). */
		uint32_t id         : 2;
		/* Reserved. */
		uint32_t __reserved : 30;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 30;
		uint32_t id         : 2;
#endif
	};

	uint32_t word;
} EMI_SERR_INJ_t;


/*
 * DERR_INJ.
 * EZcp API: EZapiPrm_writeReg
 *
 * Double Error Injection: Injects a double error (flips two data bits) for
 * specified memory ID during a single write. ID is defined in
 * ECC_DOUBLE_ERROR_0
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* ECC ID of the SRAM (ECCID). */
		uint32_t id         : 2;
		/* Reserved. */
		uint32_t __reserved : 30;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 30;
		uint32_t id         : 2;
#endif
	};

	uint32_t word;
} EMI_DERR_INJ_t;


/*
 * ECC_ID_FIRST_LAST_SERR.
 * EZcp API: EZapiPrm_SERCheckError
 *
 * ECC ID First Last Single Error: Indicates the first and last ECC ID of the
 * SRAM (ECCID) for the block that had a single error (e.g. the block had
 * three ECC errors in three different SRAMs). This may be useful
 * understanding multi ECC error events in different memories
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Indicates first ECC ID of the SRAM (ECCID) with a single
		 * error.
		 */
		uint32_t first_err    : 8;
		/* Reserved. */
		uint32_t __reserved_0 : 8;
		/*
		 * Indicates last ECC ID of the SRAM (ECCID) with a single
		 * error.
		 */
		uint32_t last_err     : 8;
		/* Reserved. */
		uint32_t __reserved_1 : 6;
		/*
		 * 1 - More than one ECC error occurred on the first ECC ID
		 * SRAM.
		 */
		uint32_t multi_first  : 1;
		/*
		 * 1 - More than one ECC error occurred on the last ECC ID
		 * SRAM.
		 */
		uint32_t multi_last   : 1;
#else   /* __BIG_ENDIAN__ */
		uint32_t multi_last   : 1;
		uint32_t multi_first  : 1;
		uint32_t __reserved_1 : 6;
		uint32_t last_err     : 8;
		uint32_t __reserved_0 : 8;
		uint32_t first_err    : 8;
#endif
	};

	uint32_t word;
} EMI_ECC_ID_FIRST_LAST_SERR_t;


/*
 * ECC_ID_FIRST_LAST_DERR.
 * EZcp API: EZapiPrm_SERCheckError
 *
 * ECC ID First Last Double Error: Indicates the first and last ECC ID of the
 * SRAM (ECCID) for the block that had a double error (e.g. the block had
 * three ECC errors in three different SRAMs).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Indicates first ECC ID of the SRAM (ECCID) with a double
		 * error.
		 */
		uint32_t first_err    : 8;
		/* Reserved. */
		uint32_t __reserved_0 : 8;
		/*
		 * Indicates last ECC ID of the SRAM (ECCID) with a double
		 * error.
		 */
		uint32_t last_err     : 8;
		/* Reserved. */
		uint32_t __reserved_1 : 6;
		/*
		 * 1 - More than one ECC error occurred on the first ECC ID
		 * SRAM.
		 */
		uint32_t multi_first  : 1;
		/*
		 * 1 - More than one ECC error occurred on the last ECC ID
		 * SRAM.
		 */
		uint32_t multi_last   : 1;
#else   /* __BIG_ENDIAN__ */
		uint32_t multi_last   : 1;
		uint32_t multi_first  : 1;
		uint32_t __reserved_1 : 6;
		uint32_t last_err     : 8;
		uint32_t __reserved_0 : 8;
		uint32_t first_err    : 8;
#endif
	};

	uint32_t word;
} EMI_ECC_ID_FIRST_LAST_DERR_t;


/*
 * ECC_SINGLE_ERROR_0.
 * EZcp API: EZapiPrm_SERCheckError
 *
 * ECC Single Error 0: Provides a single error host interrupt cause for all
 * SRAMs that have ECC or parity.
 * Bit Value:
 * 0 - SRAM has no error.
 * 1 - SRAM has an error.
 * Bit Description:
 * b0 - Name of ECCID 0 with ECC.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t ctrl_info  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wr_data    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t rdata      : 1;
		/* Reserved. */
		uint32_t __reserved : 29;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 29;
		uint32_t rdata      : 1;
		uint32_t wr_data    : 1;
		uint32_t ctrl_info  : 1;
#endif
	};

	uint32_t word;
} EMI_ECC_SINGLE_ERROR_0_t;


/*
 * ECC_SINGLE_ERROR_MASK_0.
 * EZcp API: EZapiPrm_writeReg
 *
 * ECC Single Error Mask 0: Provides a single error interrupt mask for all
 * SRAMs that have ECC or parity. The interrupt will be sent to external host
 * if in register ECC_DISABLE_STATUS the status indication is enabled.
 * Bit Value:
 * 0 - Enable ECC single error interrupt for this SRAM.
 * 1 - Disable ECC single error interrupt to external host for this SRAM.
 * Bit Description:
 * b0 - Name of first SRAM with ECC.
 * ...
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t ctrl_info  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wr_data    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t rdata      : 1;
		/* Reserved. */
		uint32_t __reserved : 29;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 29;
		uint32_t rdata      : 1;
		uint32_t wr_data    : 1;
		uint32_t ctrl_info  : 1;
#endif
	};

	uint32_t word;
} EMI_ECC_SINGLE_ERROR_MASK_0_t;


/*
 * ECC_DOUBLE_ERROR_0.
 * EZcp API: EZapiPrm_SERCheckError
 *
 * ECC Double Error 0: Provides a host interrupt cause for all SRAMs that
 * have ECC or parity.
 * Bit Value:
 * 0 - SRAM has no error.
 * 1 - SRAM has an error.
 * Bit Description:
 * b0 - Name of ECCID 0 with ECC.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t ctrl_info  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wr_data    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t rdata      : 1;
		/* Reserved. */
		uint32_t __reserved : 29;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 29;
		uint32_t rdata      : 1;
		uint32_t wr_data    : 1;
		uint32_t ctrl_info  : 1;
#endif
	};

	uint32_t word;
} EMI_ECC_DOUBLE_ERROR_0_t;


/*
 * ECC_DOUBLE_ERROR_MASK_0.
 * EZcp API: EZapiPrm_writeReg
 *
 * ECC Double Error Mask 0: Provides a double error interrupt mask for all
 * SRAMs that have ECC or parity. The interrupt will be sent to external host
 * if in register ECC_DISABLE_STATUS the status indication is enabled.
 * Bit Value:
 * 0 - Enable ECC double error interrupt to external host for this SRAM.
 * 1 - Disable ECC double error interrupt for this SRAM.
 * Bit Description:
 * b0 - Name of first SRAM with ECC.
 * ...
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t ctrl_info  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wr_data    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t rdata      : 1;
		/* Reserved. */
		uint32_t __reserved : 29;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 29;
		uint32_t rdata      : 1;
		uint32_t wr_data    : 1;
		uint32_t ctrl_info  : 1;
#endif
	};

	uint32_t word;
} EMI_ECC_DOUBLE_ERROR_MASK_0_t;


/*
 * ECC_DISABLE_FIX_0.
 * EZcp API: EZapiPrm_writeReg
 *
 * ECC Disable Fix: Disables ECC correction that corrects single-error in
 * SRAMs that have ECC protection.
 * Bit Value:
 * 0 - Enable ECC correction (only single errors can be corrected).
 * 1 - Disable ECC correction.
 * Bit Description:
 * b0 - First ECC ID of the SRAM (ECCID)
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t ctrl_info  : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t wr_data    : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t rdata      : 1;
		/* Reserved. */
		uint32_t __reserved : 29;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 29;
		uint32_t rdata      : 1;
		uint32_t wr_data    : 1;
		uint32_t ctrl_info  : 1;
#endif
	};

	uint32_t word;
} EMI_ECC_DISABLE_FIX_0_t;


/*
 * ECC_DISABLE_STATUS_0.
 * EZcp API: EZapiPrm_writeReg
 *
 * ECC Disable Status 0: Disables SRAM ECC status indication for memories to
 * host block. An interrupt will be sent to external host if in register
 * ECC_SINGLE_ERROR_MASK or ECC_DOUBLE_ERROR_MASK the interrupt bit is
 * enabled.
 * Bit Value:
 * 0 - Enable ECC status indication to host block.
 * 1 - Disable ECC status indication.
 * Bit Description:
 * b0 - First ECC ID of the SRAM (ECCID)
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t ctrl_info  : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t wr_data    : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t rdata      : 1;
		/* Reserved. */
		uint32_t __reserved : 29;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 29;
		uint32_t rdata      : 1;
		uint32_t wr_data    : 1;
		uint32_t ctrl_info  : 1;
#endif
	};

	uint32_t word;
} EMI_ECC_DISABLE_STATUS_0_t;


/*
 * ECC_LATCH_MODE_0.
 * EZcp API: EZapiPrm_SERCheckError
 *
 * ECC Latch Mode 0: Indicates if the first or last ECC error (faulty line
 * and syndrome bit) will be stored by the SRAM. This may be useful
 * understanding multi ECC error events. When reading the registers
 * ECC_ADDR_ERROR_DATA_LOW/HIGH the results will be according to register
 * ECC_LATCH_MODE.
 * Bit Value:
 * 1 - First ECC error will be stored by the SRAM.
 * 0 - Last ECC error will be stored by the SRAM.
 * Bit Description:
 * b0 - First ECC ID of the SRAM (ECCID)
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t ctrl_info  : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t wr_data    : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t rdata      : 1;
		/* Reserved. */
		uint32_t __reserved : 29;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 29;
		uint32_t rdata      : 1;
		uint32_t wr_data    : 1;
		uint32_t ctrl_info  : 1;
#endif
	};

	uint32_t word;
} EMI_ECC_LATCH_MODE_0_t;


/*
 * DRAM_ECC_COUNT.
 * EZcp API: EZapiPrm_readReg
 *
 * DRAM ECC Counter: Provides a total single and double error counter for all
 * DRAMs that have ECC. Clear on read. No wrap around.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* ECC single errors counter */
		uint32_t single_error_count : 16;
		/* ECC double errors counter */
		uint32_t double_error_count : 16;
#else   /* __BIG_ENDIAN__ */
		uint32_t double_error_count : 16;
		uint32_t single_error_count : 16;
#endif
	};

	uint32_t word;
} EMI_DRAM_ECC_COUNT_t;


/*
 * DRAM_ECC_INJECT.
 * EZcp API: EZapiPrm_writeReg
 *
 * DRAM ECC Injection: Injects a single error (flips one data bit) or double
 * error (flips two data bits) for specified DRAM wrapper during a single
 * write.
 * Write 1 another time for an additional error injection. It is not possible
 * e.g. to select both DRAM_ECC_0_SINGLE_INJECT and DRAM_ECC_0_DOUBLE_INJECT.
 *
 * Note: Generic register that is only valid for EMEM_MI, PSTAT, and ODSTAT
 * blocks.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * DRAM ECC Interface x Single Inject.
		 * Note: Double Inject for EMEM_MI block.
		 */
		uint32_t dram_ecc_single_inject : 1;
		/* Reserved. */
		uint32_t __reserved_0           : 15;
		/*
		 * DRAM ECC Interface x Double Inject.
		 * Note: Single Inject for EMEM_MI block.
		 */
		uint32_t dram_ecc_double_inject : 1;
		/* Reserved. */
		uint32_t __reserved_1           : 15;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1           : 15;
		uint32_t dram_ecc_double_inject : 1;
		uint32_t __reserved_0           : 15;
		uint32_t dram_ecc_single_inject : 1;
#endif
	};

	uint32_t word;
} EMI_DRAM_ECC_INJECT_t;


/*
 * DRAM_ECC_ERROR.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM ECC Error: Provides a host interrupt cause for all DRAMs that have
 * single or double ECC error.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* 1 - ECC single error from DRAM wrapper x. */
		uint32_t dram_ecc_single : 1;
		/* Reserved. */
		uint32_t __reserved_0    : 15;
		/* 1 - ECC double error from DRAM wrapper x. */
		uint32_t dram_ecc_double : 1;
		/* Reserved. */
		uint32_t __reserved_1    : 15;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1    : 15;
		uint32_t dram_ecc_double : 1;
		uint32_t __reserved_0    : 15;
		uint32_t dram_ecc_single : 1;
#endif
	};

	uint32_t word;
} EMI_DRAM_ECC_ERROR_t;


/*
 * DRAM_ECC_MASK_ERROR.
 * EZcp API: EZapiPrm_writeReg
 *
 * DRAM ECC Mask Error: Provides a single and double error interrupt mask for
 * all DRAMs that have ECC or parity.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * 0 - Enable ECC single error interrupt to host CPU from
		 * DRAM wrapper x.
		 */
		uint32_t dram_ecc_single : 1;
		/* Reserved. */
		uint32_t __reserved_0    : 15;
		/*
		 * 0 - Enable ECC double error interrupt to host CPU from
		 * DRAM wrapper x.
		 */
		uint32_t dram_ecc_double : 1;
		/* Reserved. */
		uint32_t __reserved_1    : 15;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1    : 15;
		uint32_t dram_ecc_double : 1;
		uint32_t __reserved_0    : 15;
		uint32_t dram_ecc_single : 1;
#endif
	};

	uint32_t word;
} EMI_DRAM_ECC_MASK_ERROR_t;


/*
 * DRAM_ECC_DISABLE.
 * EZcp API: EZapiPrm_writeReg
 *
 * DRAM ECC Disable: Disables ECC calculation and fix that corrects
 * single-error in memories that have ECC protection.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* 1- Disable ECC fix for DRAM wrapper x. */
		uint32_t dram_ecc_disable_fix    : 1;
		/* Reserved. */
		uint32_t __reserved_0            : 15;
		/*
		 * 1 - For DRAM wrapper x:
		 * a. The single and double ECC error counters in register
		 * DRAM_ECC_COUNT are frozen.
		 *
		 * b. The single and double error indications in register
		 * DRAM_ECC_ERROR are set to 0.
		 */
		uint32_t dram_ecc_disable_status : 1;
		/* Reserved. */
		uint32_t __reserved_1            : 15;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1            : 15;
		uint32_t dram_ecc_disable_status : 1;
		uint32_t __reserved_0            : 15;
		uint32_t dram_ecc_disable_fix    : 1;
#endif
	};

	uint32_t word;
} EMI_DRAM_ECC_DISABLE_t;


/*
 * DRAM_ECC_LATCH_SELECT.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM ECC Latch Select: Indicates if the first or last ECC error will be
 * stored by the DRAM
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * For DRAM wrapper x.
		 *
		 * 0 - Last ECC error will be stored.
		 *
		 * 1 - First ECC error will be stored.
		 */
		uint32_t dram_ecc_first : 1;
		/* Reserved. */
		uint32_t __reserved_0   : 15;
		/* Selects to Edge (one of 8) to get the ECC info. */
		uint32_t edge_sel       : 4;
		/* Reserved. */
		uint32_t __reserved_1   : 4;
		/* 1 - Start select DRAM_INFO */
		uint32_t start          : 1;
		/* Reserved. */
		uint32_t __reserved_2   : 7;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_2   : 7;
		uint32_t start          : 1;
		uint32_t __reserved_1   : 4;
		uint32_t edge_sel       : 4;
		uint32_t __reserved_0   : 15;
		uint32_t dram_ecc_first : 1;
#endif
	};

	uint32_t word;
} EMI_DRAM_ECC_LATCH_SELECT_t;


/*
 * DRAM_ERR_ADDR_0.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Error Address: Provides the ECC error address for the DRAM wrapper
 * selected by fields DRAM_ID_SEL and INDEX_SEL in register
 * DRAM_ECC_LATCH_SELECT. See register DRAM_SYNDROM for additional ECC error
 * information.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Provides the ECC error address */
		uint32_t error_addr : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t error_addr : 32;
#endif
	};

	uint32_t word;
} EMI_DRAM_ERR_ADDR_0_t;


/*
 * DRAM_SYNDROM.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Syndrome: Provides ECC error information. See register DRAM_ERR_ADDR
 * for ECC error address.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* 1 - Double error. */
		uint32_t derr         : 1;
		/* 1 - Single error. */
		uint32_t serr         : 1;
		/* Reserved. */
		uint32_t __reserved_0 : 14;
		/* ECC syndrome (error bit according to the Hamming code). */
		uint32_t syndrom      : 10;
		/* Reserved. */
		uint32_t __reserved_1 : 6;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1 : 6;
		uint32_t syndrom      : 10;
		uint32_t __reserved_0 : 14;
		uint32_t serr         : 1;
		uint32_t derr         : 1;
#endif
	};

	uint32_t word;
} EMI_DRAM_SYNDROM_t;


/*
 * DRAM_FIRST_LAST.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * Stores the DRAM wrapper ID of first and last ECC error, and indicates if
 * more than one error occurred.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* First single error DRAM wrapper ID. */
		uint32_t first_dram_id_serr : 4;
		/* Reserved. */
		uint32_t __reserved_0       : 3;
		/*
		 * First single error occurred on more than one DRAM wrapper.
		 */
		uint32_t multi_first_serr   : 1;
		/* Last single error DRAM wrapper ID. */
		uint32_t last_dram_id_serr  : 4;
		/* Reserved. */
		uint32_t __reserved_1       : 3;
		/*
		 * Last single error occurred on more than one DRAM wrapper.
		 */
		uint32_t multi_last_serr    : 1;
		/* First double error DRAM wrapper ID. */
		uint32_t first_dram_id_derr : 4;
		/* Reserved. */
		uint32_t __reserved_2       : 3;
		/*
		 * First double error occurred on more than one DRAM wrapper.
		 */
		uint32_t multi_first_derr   : 1;
		/* Last double error DRAM wrapper ID. */
		uint32_t last_dram_id_derr  : 4;
		/* Reserved. */
		uint32_t __reserved_3       : 3;
		/*
		 * Last double error occurred on more than one DRAM wrapper.
		 */
		uint32_t multi_last_derr    : 1;
#else   /* __BIG_ENDIAN__ */
		uint32_t multi_last_derr    : 1;
		uint32_t __reserved_3       : 3;
		uint32_t last_dram_id_derr  : 4;
		uint32_t multi_first_derr   : 1;
		uint32_t __reserved_2       : 3;
		uint32_t first_dram_id_derr : 4;
		uint32_t multi_last_serr    : 1;
		uint32_t __reserved_1       : 3;
		uint32_t last_dram_id_serr  : 4;
		uint32_t multi_first_serr   : 1;
		uint32_t __reserved_0       : 3;
		uint32_t first_dram_id_serr : 4;
#endif
	};

	uint32_t word;
} EMI_DRAM_FIRST_LAST_t;


/*
 * DRAM_ADDITIONAL_INFO_0.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Additional Information: Specific DRAM information provided by the
 * block's DRAM wrappers. EZchip internal use. The information is valid when
 * bit READY = 1 in register DRAM_SYNDROM.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Physical Error Bank. */
		uint32_t err_bank     : 4;
		/* Physical Error Logical Rank. */
		uint32_t err_lrank    : 2;
		/* Reserved. */
		uint32_t __reserved_0 : 2;
		/* Physical Error Physical Rank. */
		uint32_t err_prank    : 2;
		/* Reserved. */
		uint32_t __reserved_1 : 6;
		/* Error Edge Bitmap. */
		uint32_t err_edge     : 8;
		/* Reserved. */
		uint32_t __reserved_2 : 8;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_2 : 8;
		uint32_t err_edge     : 8;
		uint32_t __reserved_1 : 6;
		uint32_t err_prank    : 2;
		uint32_t __reserved_0 : 2;
		uint32_t err_lrank    : 2;
		uint32_t err_bank     : 4;
#endif
	};

	uint32_t word;
} EMI_DRAM_ADDITIONAL_INFO_0_t;


/*
 * DRAM_ADDITIONAL_INFO_1.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Additional Information: Specific DRAM information provided by the
 * block's DRAM wrappers. EZchip internal use. The information is valid when
 * bit READY = 1 in register DRAM_SYNDROM.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Physical Error Address (Row + Col). */
		uint32_t err_addr   : 25;
		/* Reserved. */
		uint32_t __reserved : 7;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 7;
		uint32_t err_addr   : 25;
#endif
	};

	uint32_t word;
} EMI_DRAM_ADDITIONAL_INFO_1_t;


/*
 * DRAM_ADDITIONAL_INFO_2.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Additional Information: Specific DRAM information provided by the
 * block's DRAM wrappers. EZchip internal use. The information is valid when
 * bit READY = 1 in register DRAM_SYNDROM.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Error Transaction Stream ID. */
		uint32_t err_stream_id : 8;
		/* Reserved. */
		uint32_t __reserved_0  : 8;
		/* Host Access Indication */
		uint32_t host_access   : 1;
		/* Reserved. */
		uint32_t __reserved_1  : 15;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1  : 15;
		uint32_t host_access   : 1;
		uint32_t __reserved_0  : 8;
		uint32_t err_stream_id : 8;
#endif
	};

	uint32_t word;
} EMI_DRAM_ADDITIONAL_INFO_2_t;


/*
 * DRAM_ADDITIONAL_INFO_3.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Additional Information: Specific DRAM information provided by the
 * block's DRAM wrappers. EZchip internal use. The information is valid when
 * bit READY = 1 in register DRAM_SYNDROM.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Error Transaction User Free bits. */
		uint32_t err_usr_free : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t err_usr_free : 32;
#endif
	};

	uint32_t word;
} EMI_DRAM_ADDITIONAL_INFO_3_t;


/*
 * DRAM_ADDITIONAL_INFO_4.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Additional Information: Specific DRAM information provided by the
 * block's DRAM wrappers. EZchip internal use. The information is valid when
 * bit READY = 1 in register DRAM_SYNDROM.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Error Transaction Data. */
		uint32_t val : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t val : 32;
#endif
	};

	uint32_t word;
} EMI_DRAM_ADDITIONAL_INFO_4_t;


/*
 * DRAM_ADDITIONAL_INFO_5.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Additional Information: Specific DRAM information provided by the
 * block's DRAM wrappers. EZchip internal use. The information is valid when
 * bit READY = 1 in register DRAM_SYNDROM.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Error Transaction Data. */
		uint32_t val : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t val : 32;
#endif
	};

	uint32_t word;
} EMI_DRAM_ADDITIONAL_INFO_5_t;


/*
 * DRAM_ERR_ADDR_1.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Error Address: Provides the ECC error address for the DRAM wrapper
 * selected by fields DRAM_ID_SEL and INDEX_SEL in register
 * DRAM_ECC_LATCH_SELECT. See register DRAM_SYNDROM for additional ECC error
 * information.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Provides the ECC error address */
		uint32_t error_addr : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t error_addr : 32;
#endif
	};

	uint32_t word;
} EMI_DRAM_ERR_ADDR_1_t;


/*
 * REQ_SEQUENCE.
 * Request sequence number configurations
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * The global sequence number threshold for a request to be
		 * valid for arbitration.
		 */
		uint32_t num_open_req : 7;
		/* Reserved. */
		uint32_t __reserved_0 : 1;
		/*
		 * The read sequence number threshold for a request to be
		 * considered high priority.
		 */
		uint32_t rd_hp_th     : 7;
		/* Reserved. */
		uint32_t __reserved_1 : 1;
		/*
		 * The write sequence number threshold for a request to be
		 * considered high priority.
		 */
		uint32_t wr_hp_th     : 7;
		/* Reserved. */
		uint32_t __reserved_2 : 9;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_2 : 9;
		uint32_t wr_hp_th     : 7;
		uint32_t __reserved_1 : 1;
		uint32_t rd_hp_th     : 7;
		uint32_t __reserved_0 : 1;
		uint32_t num_open_req : 7;
#endif
	};

	uint32_t word;
} EMI_REQ_SEQUENCE_t;


/*
 * RD_LAT_EST.
 * Latency estimation configurations - read requests
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint32_t __reserved_0     : 8;
		/* Average latency granularity. */
		uint32_t avg_lat_gran     : 5;
		/* Reserved. */
		uint32_t __reserved_1     : 3;
		/* latency delta divider factor (power of 2) */
		uint32_t lat_delta_factor : 3;
		/* Reserved. */
		uint32_t __reserved_2     : 1;
		/* deviation delta divider factor (power of 2) */
		uint32_t dev_delta_factor : 3;
		/* Reserved. */
		uint32_t __reserved_3     : 1;
		/* deviation divider factor (power of 2) */
		uint32_t dev_factor       : 4;
		/* Reserved. */
		uint32_t __reserved_4     : 4;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_4     : 4;
		uint32_t dev_factor       : 4;
		uint32_t __reserved_3     : 1;
		uint32_t dev_delta_factor : 3;
		uint32_t __reserved_2     : 1;
		uint32_t lat_delta_factor : 3;
		uint32_t __reserved_1     : 3;
		uint32_t avg_lat_gran     : 5;
		uint32_t __reserved_0     : 8;
#endif
	};

	uint32_t word;
} EMI_RD_LAT_EST_t;


/*
 * WR_LAT_EST.
 * Latency estimation configurations - write requests
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint32_t __reserved_0     : 8;
		/* Average latency granularity. */
		uint32_t avg_lat_gran     : 5;
		/* Reserved. */
		uint32_t __reserved_1     : 3;
		/* latency delta divider factor (power of 2) */
		uint32_t lat_delta_factor : 3;
		/* Reserved. */
		uint32_t __reserved_2     : 1;
		/* deviation delta divider factor (power of 2) */
		uint32_t dev_delta_factor : 3;
		/* Reserved. */
		uint32_t __reserved_3     : 1;
		/* deviation divider factor (power of 2) */
		uint32_t dev_factor       : 4;
		/* Reserved. */
		uint32_t __reserved_4     : 4;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_4     : 4;
		uint32_t dev_factor       : 4;
		uint32_t __reserved_3     : 1;
		uint32_t dev_delta_factor : 3;
		uint32_t __reserved_2     : 1;
		uint32_t lat_delta_factor : 3;
		uint32_t __reserved_1     : 3;
		uint32_t avg_lat_gran     : 5;
		uint32_t __reserved_0     : 8;
#endif
	};

	uint32_t word;
} EMI_WR_LAT_EST_t;


/*
 * RD_RES.
 * Read response path configurations
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* read response fifo credits (used by the bank arbiter) */
		uint32_t fifo_credits : 6;
		/* Reserved. */
		uint32_t __reserved   : 26;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved   : 26;
		uint32_t fifo_credits : 6;
#endif
	};

	uint32_t word;
} EMI_RD_RES_t;


/*
 * RANK_ARB.
 * Rank arbiter configurations
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* commit threshold (number of consecutive RASs) */
		uint32_t th1                 : 6;
		/* Reserved. */
		uint32_t __reserved_0        : 2;
		/*
		 * starvation prevention threshold (number of consecutive
		 * RASs)
		 */
		uint32_t th2                 : 6;
		/* Reserved. */
		uint32_t __reserved_1        : 2;
		/* poor bank distribution threshold */
		uint32_t poor_bank_distr     : 7;
		/* Reserved. */
		uint32_t __reserved_2        : 1;
		/* poor bank distribution threshold for 3ds devices */
		uint32_t poor_bank_distr_3ds : 7;
		/* Reserved. */
		uint32_t __reserved_3        : 1;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_3        : 1;
		uint32_t poor_bank_distr_3ds : 7;
		uint32_t __reserved_2        : 1;
		uint32_t poor_bank_distr     : 7;
		uint32_t __reserved_1        : 2;
		uint32_t th2                 : 6;
		uint32_t __reserved_0        : 2;
		uint32_t th1                 : 6;
#endif
	};

	uint32_t word;
} EMI_RANK_ARB_t;


/*
 * OLDEST_TIMER.
 * Oldest request service demand timer
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Oldest read request service demand timer */
		uint32_t rd_th      : 8;
		/* Oldest write request service demand timer */
		uint32_t wr_th      : 8;
		/* Reserved. */
		uint32_t __reserved : 16;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 16;
		uint32_t wr_th      : 8;
		uint32_t rd_th      : 8;
#endif
	};

	uint32_t word;
} EMI_OLDEST_TIMER_t;


/*
 * RD_LAT.
 * read latency thresholds
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		uint32_t low_th     : 9;
		uint32_t med_th     : 9;
		uint32_t high_th    : 9;
		/* Reserved. */
		uint32_t __reserved : 5;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 5;
		uint32_t high_th    : 9;
		uint32_t med_th     : 9;
		uint32_t low_th     : 9;
#endif
	};

	uint32_t word;
} EMI_RD_LAT_t;


/*
 * WR_LAT.
 * write latency thresholds
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		uint32_t low_th     : 9;
		uint32_t med_th     : 9;
		uint32_t high_th    : 9;
		/* Reserved. */
		uint32_t __reserved : 5;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 5;
		uint32_t high_th    : 9;
		uint32_t med_th     : 9;
		uint32_t low_th     : 9;
#endif
	};

	uint32_t word;
} EMI_WR_LAT_t;


/*
 * LRANK_ARB.
 * Logical Rank arbiter configurations
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* commit threshold (number of consecutive RASs) */
		uint32_t th1             : 5;
		/* Reserved. */
		uint32_t __reserved_0    : 3;
		/*
		 * starvation prevention threshold (number of consecutive
		 * RASs)
		 */
		uint32_t th2             : 5;
		/* Reserved. */
		uint32_t __reserved_1    : 3;
		/* poor bank distribution threshold */
		uint32_t poor_bank_distr : 5;
		/* Reserved. */
		uint32_t __reserved_2    : 3;
		/* Enable Logical Rank Arbitration */
		uint32_t en              : 1;
		/* Reserved. */
		uint32_t __reserved_3    : 7;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_3    : 7;
		uint32_t en              : 1;
		uint32_t __reserved_2    : 3;
		uint32_t poor_bank_distr : 5;
		uint32_t __reserved_1    : 3;
		uint32_t th2             : 5;
		uint32_t __reserved_0    : 3;
		uint32_t th1             : 5;
#endif
	};

	uint32_t word;
} EMI_LRANK_ARB_t;


/*
 * L3C_IF.
 * L3 Cache Interface configurations
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable/Disable the ingress interface between L3C and EMI:
		 * 0 - Disable
		 * 1 - Enable
		 */
		uint32_t en         : 1;
		/* Reserved. */
		uint32_t __reserved : 31;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 31;
		uint32_t en         : 1;
#endif
	};

	uint32_t word;
} EMI_L3C_IF_t;


/*
 * HOST_ENDLESS.
 * Host Endless Operation configuration register
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable/Disable the Host Endless Operation:
		 * 0 - Disable
		 * 1 - Enable
		 */
		uint32_t en         : 1;
		/* Reserved. */
		uint32_t __reserved : 31;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 31;
		uint32_t en         : 1;
#endif
	};

	uint32_t word;
} EMI_HOST_ENDLESS_t;


/*
 * STATUS.
 * reflects the statuses from the block
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Data is defined by register 0x2F */
		uint32_t val : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t val : 32;
#endif
	};

	uint32_t word;
} EMI_STATUS_t;


/*
 * STATUS_SEL.
 * controls the desired status information which is presented in the status
 * regs
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the counters which are displayed when DATA = 0x0
		 * ,0x7, or 0xF.
		 */
		uint32_t cntrs_en      : 1;
		/* Reserved. */
		uint32_t __reserved_0  : 3;
		/*
		 * Type of the selected command
		 * 0 - read
		 * 1 - write
		 * Relevant when DATA = 0xF.
		 */
		uint32_t cmd_type      : 1;
		/* Reserved. */
		uint32_t __reserved_1  : 3;
		/*
		 * 0x0 - Incoming Data Counters
		 * 0x1 - Incoming Control and Data Syncronizers
		 * 0x2 - SRAM valid (Control and Data)
		 * 0x3 - Data Manager
		 * 0x4 - Control Manager
		 * 0x5 - Control Manager Cont.
		 * 0x6 - Outgoing Control and Data Syncronizers
		 * 0x7 - Outgoing Data Counters
		 * 0xF - Performance Counters
		 */
		uint32_t data          : 4;
		/* Reserved. */
		uint32_t __reserved_2  : 4;
		/*
		 * lowest stream id to mesure statistics on Results displayd
		 * when DATA = 0x0 and 0x7.
		 * Link List ID, when DATA = 0x5. Values 0 to 255
		 */
		uint32_t low_streamid  : 8;
		/*
		 * highest stream id to mesure statistics on.
		 * Results displayed when DATA = 0x0 and 0x7.
		 * Command ID, when DATA = 0x5. Values 0 to 127
		 */
		uint32_t high_streamid : 8;
#else   /* __BIG_ENDIAN__ */
		uint32_t high_streamid : 8;
		uint32_t low_streamid  : 8;
		uint32_t __reserved_2  : 4;
		uint32_t data          : 4;
		uint32_t __reserved_1  : 3;
		uint32_t cmd_type      : 1;
		uint32_t __reserved_0  : 3;
		uint32_t cntrs_en      : 1;
#endif
	};

	uint32_t word;
} EMI_STATUS_SEL_t;


/*
 * AVG_LAT_CALC.
 * Average latency calculation mode register
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint32_t __reserved                  : 16;
		/* Seed for rounding LFSR */
		uint32_t lat_delta_and_dev_lfsr_seed : 16;
#else   /* __BIG_ENDIAN__ */
		uint32_t lat_delta_and_dev_lfsr_seed : 16;
		uint32_t __reserved                  : 16;
#endif
	};

	uint32_t word;
} EMI_AVG_LAT_CALC_t;


/*
 * HP_LATENCY.
 * High Priority Latency Configuration Register
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * High Priority Latency feature Enable/Disable
		 * 0 - Disable
		 * 1 - Enable
		 */
		uint32_t en         : 1;
		/* Reserved. */
		uint32_t __reserved : 7;
		/*
		 * Counter Resolution. A pulse is generated once the counter
		 * reaches zero.
		 */
		uint32_t res        : 8;
		/*
		 * Write Requests Threshold.
		 * In case the Latency Counter exceeds this threshold, the
		 * request is marked as high priority.
		 */
		uint32_t th_wr      : 8;
		/*
		 * Read Requests Threshold.
		 * In case the Latency Counter exceeds this threshold, the
		 * request is marked as high priority.
		 */
		uint32_t th_rd      : 8;
#else   /* __BIG_ENDIAN__ */
		uint32_t th_rd      : 8;
		uint32_t th_wr      : 8;
		uint32_t res        : 8;
		uint32_t __reserved : 7;
		uint32_t en         : 1;
#endif
	};

	uint32_t word;
} EMI_HP_LATENCY_t;


/*
 * SM_RD_WR_WIN.
 * TH1 and TH2 for the RD/WR SM
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * The read lowest threshold used in the RD/WR state machine.
		 */
		uint32_t rd_thr1 : 8;
		/*
		 * The second read threshold used in the RD/WR state machine.
		 */
		uint32_t rd_thr2 : 8;
		/*
		 * The write lowest threshold used in the RD/WR state machine.
		 */
		uint32_t wr_thr1 : 8;
		/*
		 * The second write threshold used in the RD/WR state machine.
		 */
		uint32_t wr_thr2 : 8;
#else   /* __BIG_ENDIAN__ */
		uint32_t wr_thr2 : 8;
		uint32_t wr_thr1 : 8;
		uint32_t rd_thr2 : 8;
		uint32_t rd_thr1 : 8;
#endif
	};

	uint32_t word;
} EMI_SM_RD_WR_WIN_t;


/*
 * WR_REQ_SM.
 * Low and MED thresholds used for translating latched fullness(accumulative
 * sizes of all wr requests) as part of the RD-WR profile selection.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Low threshold for total transaction sizes. */
		uint32_t low_load_thr    : 8;
		/* Med threshold for total transaction sizes. */
		uint32_t med_load_thr    : 8;
		/* Write banks distribution threshold for RD_WR_SM */
		uint32_t poor_bank_distr : 9;
		/* Reserved. */
		uint32_t __reserved      : 7;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved      : 7;
		uint32_t poor_bank_distr : 9;
		uint32_t med_load_thr    : 8;
		uint32_t low_load_thr    : 8;
#endif
	};

	uint32_t word;
} EMI_WR_REQ_SM_t;


/*
 * RD_REQ_SM.
 * Low and MED thresholds used for translating latched fullness(accumulative
 * sizes of all rd requests) as part of the RD-WR profile selection.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Low threshold for total transaction sizes. */
		uint32_t low_load_thr    : 8;
		/* Med threshold for total transaction sizes. */
		uint32_t med_load_thr    : 8;
		/* Threshold for rd-wr SM pending read banks num. */
		uint32_t poor_bank_distr : 9;
		/* Reserved. */
		uint32_t __reserved      : 7;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved      : 7;
		uint32_t poor_bank_distr : 9;
		uint32_t med_load_thr    : 8;
		uint32_t low_load_thr    : 8;
#endif
	};

	uint32_t word;
} EMI_RD_REQ_SM_t;


/*
 * WR_RD_REQ_SM.
 * High thresholds used for translating latched fullness(accumulative sizes
 * of all rd requests) as part of the RD-WR profile selection.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* High threshold for total transaction sizes. */
		uint32_t high_load_thr_rd  : 8;
		/* High threshold for total transaction sizes. */
		uint32_t high_load_thr_wr  : 8;
		/* Allow state change only when counter >= TH */
		uint32_t min_win_th        : 5;
		/* Reserved. */
		uint32_t __reserved_0      : 3;
		/* Enable the duration leftover exchange between states */
		uint32_t leftover_en       : 1;
		/* Reserved. */
		uint32_t __reserved_1      : 3;
		/*
		 * Enable low load consideration when determining bad
		 * utilization state
		 */
		uint32_t consider_low_load : 1;
		/* Reserved. */
		uint32_t __reserved_2      : 3;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_2      : 3;
		uint32_t consider_low_load : 1;
		uint32_t __reserved_1      : 3;
		uint32_t leftover_en       : 1;
		uint32_t __reserved_0      : 3;
		uint32_t min_win_th        : 5;
		uint32_t high_load_thr_wr  : 8;
		uint32_t high_load_thr_rd  : 8;
#endif
	};

	uint32_t word;
} EMI_WR_RD_REQ_SM_t;


/*
 * RD_WR_SM_4TO2.
 * Global (rd / wr) table used to translate the latched fullness and latency
 * status of all RD/WR sorters to 2 bits later used for RD-WR profile
 * selection.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Table of 16 entries (of 2 bits each) that translate the
		 * codes of the fullness and latency to 2 bits.
		 */
		uint32_t map : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t map : 32;
#endif
	};

	uint32_t word;
} EMI_RD_WR_SM_4TO2_t;


/*
 * RD_WR_SM_PROF_INDEX0.
 * Global (RD/WR) table used to translate the combined RD/WR priorities to 1
 * of 8 profile indexes.
 * In this register exists the first 10 entries of the table.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Table of 10 entries that translates the {rd_pri,wer_pri}
		 * to profile index (3 bits).
		 */
		uint32_t map        : 30;
		/* Reserved. */
		uint32_t __reserved : 2;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 2;
		uint32_t map        : 30;
#endif
	};

	uint32_t word;
} EMI_RD_WR_SM_PROF_INDEX0_t;


/*
 * RD_WR_SM_PROF_INDEX1.
 * Global (RD/WR) table used to translate the combined RD/WR priorities to 1
 * of 8 profile indexes.
 * In this register exists the last 6 entries of the table.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Table of 6 entries that translates the {rd_pri,wer_pri} to
		 * profile index (3 bits).
		 */
		uint32_t map        : 18;
		/* Reserved. */
		uint32_t __reserved : 14;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 14;
		uint32_t map        : 18;
#endif
	};

	uint32_t word;
} EMI_RD_WR_SM_PROF_INDEX1_t;


/*
 * RD_WR_SM_STARVATION_PREVENTION.
 * highest possible starvation duration
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* read state threshold number 4 */
		uint32_t rd_th4       : 13;
		/* Reserved. */
		uint32_t __reserved_0 : 3;
		/* write state threshold number 4 */
		uint32_t wr_th4       : 13;
		/* Reserved. */
		uint32_t __reserved_1 : 3;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1 : 3;
		uint32_t wr_th4       : 13;
		uint32_t __reserved_0 : 3;
		uint32_t rd_th4       : 13;
#endif
	};

	uint32_t word;
} EMI_RD_WR_SM_STARVATION_PREVENTION_t;


/*
 * UTIL_DEGRADATION.
 * utility degradation feature - aimed to reduce DDR power consumption
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * this bit enables the utility degredation feature
		 * 1 - enable
		 * 0 - disable
		 */
		uint32_t en         : 1;
		/*
		 * this bit enables long mode
		 * that changes the granularity of max tokens in the bucket
		 * and ,increment bucket countdown, and tokens added.
		 * 1 - enable
		 * 0 - disable
		 */
		uint32_t long_en    : 1;
		/* Reserved. */
		uint32_t __reserved : 11;
		/*
		 * defines the maximum amount of tokens allowed in the bucket.
		 * the final amount is MAX_TOKENS*2^8 (2^8 granularity).
		 * if long mode is enabled the final amount is
		 * MAX_TOKENS*2^12 (2^12 granularity)
		 */
		uint32_t max_tokens : 19;
#else   /* __BIG_ENDIAN__ */
		uint32_t max_tokens : 19;
		uint32_t __reserved : 11;
		uint32_t long_en    : 1;
		uint32_t en         : 1;
#endif
	};

	uint32_t word;
} EMI_UTIL_DEGRADATION_t;


/*
 * UTIL_DEGRADATION_BUCKET_INCREMENT.
 * utility degradation feature - bucket tokens incremetation related
 * configurations
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * the number of ddr_mc_clk cycles between the additions of
		 * tokens to the bucket.
		 * every COUNTDOWN ddr_mc_clk cycles the number of tokens is
		 * incremented. (2^0 granularity)
		 * if long mode is enabled, every COUNTDOWN*2^15 ddr_mc_clk
		 * cycles the number of tokens is incremented. (2^15
		 * granularity)
		 */
		uint32_t countdown : 15;
		/*
		 * the number of tokens that are added to the bucket at each
		 * incrementation.
		 * the final number to be added is TOKEN_NUM (2^0
		 * granularity).
		 * if long mode is enabled, the final number to be added is
		 * TOKEN_NUM*2^14  (2^14 granularity)
		 */
		uint32_t token_num : 17;
#else   /* __BIG_ENDIAN__ */
		uint32_t token_num : 17;
		uint32_t countdown : 15;
#endif
	};

	uint32_t word;
} EMI_UTIL_DEGRADATION_BUCKET_INCREMENT_t;


/*
 * INTR_CAUSE.
 * There are 3 interrupt causes:
 * 1. Flush Error Interrupt
 * 2. Row Hammer
 * 3. Page Boundary Exception (only relevant when working in Single SDRAM
 * mode).
 * 4. Rank Error Interrupt
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Flush Error Indication */
		uint32_t flush_err        : 1;
		/* Reserved. */
		uint32_t __reserved_0     : 3;
		/* Row Hammer Indication */
		uint32_t row_hammer       : 1;
		/* Reserved. */
		uint32_t __reserved_1     : 3;
		/*
		 * Page Boundary Exception (only relevant when working in
		 * Single SDRAM mode).
		 */
		uint32_t page_bound_excpt : 1;
		/* Reserved. */
		uint32_t __reserved_2     : 3;
		/*
		 * ALERT Indication (relevant when Parity/CRC are enabled in
		 * DRAM)
		 */
		uint32_t alert            : 1;
		/* Reserved. */
		uint32_t __reserved_3     : 3;
		/* Rank Error Indication */
		uint32_t rank_err         : 1;
		/* Reserved. */
		uint32_t __reserved_4     : 15;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_4     : 15;
		uint32_t rank_err         : 1;
		uint32_t __reserved_3     : 3;
		uint32_t alert            : 1;
		uint32_t __reserved_2     : 3;
		uint32_t page_bound_excpt : 1;
		uint32_t __reserved_1     : 3;
		uint32_t row_hammer       : 1;
		uint32_t __reserved_0     : 3;
		uint32_t flush_err        : 1;
#endif
	};

	uint32_t word;
} EMI_INTR_CAUSE_t;


/*
 * INTR_MASK.
 * There are 3 interrupt causes:
 * 1. Flush Error Interrupt
 * 2. Row Hammer
 * 3. Page Boundary Exception (only relevant when working in Single SDRAM
 * mode).
 * 4. Rank Error Interrupt
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Flush Error Indication */
		uint32_t flush_err        : 1;
		/* Reserved. */
		uint32_t __reserved_0     : 3;
		/* Row Hammer Indication */
		uint32_t row_hammer       : 1;
		/* Reserved. */
		uint32_t __reserved_1     : 3;
		/*
		 * Page Boundary Exception (only relevant when working in
		 * Single SDRAM mode).
		 */
		uint32_t page_bound_excpt : 1;
		/* Reserved. */
		uint32_t __reserved_2     : 3;
		/*
		 * ALERT Indication (relevant when Parity/CRC are enabled in
		 * DRAM)
		 */
		uint32_t alert            : 1;
		/* Reserved. */
		uint32_t __reserved_3     : 3;
		/* Rank Error Indication */
		uint32_t rank_err         : 1;
		/* Reserved. */
		uint32_t __reserved_4     : 15;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_4     : 15;
		uint32_t rank_err         : 1;
		uint32_t __reserved_3     : 3;
		uint32_t alert            : 1;
		uint32_t __reserved_2     : 3;
		uint32_t page_bound_excpt : 1;
		uint32_t __reserved_1     : 3;
		uint32_t row_hammer       : 1;
		uint32_t __reserved_0     : 3;
		uint32_t flush_err        : 1;
#endif
	};

	uint32_t word;
} EMI_INTR_MASK_t;


/*
 * DRAM_ERR_DATA.
 * These registers capture the data of the transaction in which an ECC error
 * has occurred.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* DRAM Error Data */
		uint32_t val : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t val : 32;
#endif
	};

	uint32_t word;
} EMI_DRAM_ERR_DATA_t;


/*
 * FLUSH_ERR_CNTR.
 * Counts the number of requests which arrived to the EMI during Flush
 * activity
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		uint32_t val : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t val : 32;
#endif
	};

	uint32_t word;
} EMI_FLUSH_ERR_CNTR_t;


/*
 * FLUSH_ERR_INFO_0.
 * This registers holds the controls for the first requests which arrived
 * during Flush activity
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Request's Address[31:0] */
		uint32_t addr : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t addr : 32;
#endif
	};

	uint32_t word;
} EMI_FLUSH_ERR_INFO_0_t;


/*
 * FLUSH_ERR_INFO_1.
 * This registers holds the controls for the first requests which arrived
 * during Flush activity
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Request's Address[33:32] */
		uint32_t addr         : 2;
		/* Reserved. */
		uint32_t __reserved_0 : 6;
		/*
		 * Request's Command:
		 * 0 - Read Command
		 * 1 - Write Command
		 */
		uint32_t cmd          : 1;
		/* Reserved. */
		uint32_t __reserved_1 : 23;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1 : 23;
		uint32_t cmd          : 1;
		uint32_t __reserved_0 : 6;
		uint32_t addr         : 2;
#endif
	};

	uint32_t word;
} EMI_FLUSH_ERR_INFO_1_t;


/*
 * FLUSH_ERR_INFO_2.
 * This registers holds the controls for the first requests which arrived
 * during Flush activity
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Request's Stream ID */
		uint32_t stream_id : 8;
		/* Request's User Free bits */
		uint32_t usr_free  : 24;
#else   /* __BIG_ENDIAN__ */
		uint32_t usr_free  : 24;
		uint32_t stream_id : 8;
#endif
	};

	uint32_t word;
} EMI_FLUSH_ERR_INFO_2_t;


/*
 * PAGE_BOUND_EXCPT_INFO_0.
 * This register holds the info of the exception boundary request
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Request Bank */
		uint32_t bank         : 4;
		/* Request Logical Rank */
		uint32_t lr           : 2;
		/* Reserved. */
		uint32_t __reserved_0 : 2;
		/*
		 * Request Command:
		 * 0 - Read
		 * 1 - Write
		 */
		uint32_t cmd          : 1;
		/* Reserved. */
		uint32_t __reserved_1 : 23;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1 : 23;
		uint32_t cmd          : 1;
		uint32_t __reserved_0 : 2;
		uint32_t lr           : 2;
		uint32_t bank         : 4;
#endif
	};

	uint32_t word;
} EMI_PAGE_BOUND_EXCPT_INFO_0_t;


/*
 * PAGE_BOUND_EXCPT_INFO_1.
 * This register holds the info of the exception boundary request
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Request Address (ROW and COL) */
		uint32_t addr       : 25;
		/* Reserved. */
		uint32_t __reserved : 7;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 7;
		uint32_t addr       : 25;
#endif
	};

	uint32_t word;
} EMI_PAGE_BOUND_EXCPT_INFO_1_t;


/*
 * RANK_ERR_INFO_0.
 * This registers holds the controls for the request which arrived to a non
 * active rank
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Request's Address[31:0] */
		uint32_t addr : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t addr : 32;
#endif
	};

	uint32_t word;
} EMI_RANK_ERR_INFO_0_t;


/*
 * RANK_ERR_INFO_1.
 * This registers holds the controls for the request which arrived to a non
 * active rank
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Request's Address[33:32] */
		uint32_t addr         : 2;
		/* Reserved. */
		uint32_t __reserved_0 : 6;
		/*
		 * Request's Command:
		 * 0 - Read Command
		 * 1 - Write Command
		 */
		uint32_t cmd          : 1;
		/* Reserved. */
		uint32_t __reserved_1 : 23;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1 : 23;
		uint32_t cmd          : 1;
		uint32_t __reserved_0 : 6;
		uint32_t addr         : 2;
#endif
	};

	uint32_t word;
} EMI_RANK_ERR_INFO_1_t;


/*
 * RANK_ERR_INFO_2.
 * This registers holds the controls for the request which arrived to a non
 * active rank
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Request's Stream ID */
		uint32_t stream_id : 8;
		/* Request's User Free bits */
		uint32_t usr_free  : 24;
#else   /* __BIG_ENDIAN__ */
		uint32_t usr_free  : 24;
		uint32_t stream_id : 8;
#endif
	};

	uint32_t word;
} EMI_RANK_ERR_INFO_2_t;


/*
 * RDWR_SM_PROFILE_0.
 * Parameters of the rdwr sm profiles
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* read state shift threshold */
		uint32_t rd_th3       : 9;
		/* Reserved. */
		uint32_t __reserved_0 : 7;
		/* write state shift threshold */
		uint32_t wr_th3       : 9;
		/* Reserved. */
		uint32_t __reserved_1 : 7;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1 : 7;
		uint32_t wr_th3       : 9;
		uint32_t __reserved_0 : 7;
		uint32_t rd_th3       : 9;
#endif
	};

	uint32_t word;
} EMI_RDWR_SM_PROFILE_0_t;


/*
 * RDWR_SM_PROFILE_1.
 * Parameters of the rdwr sm profiles
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* read state shift threshold */
		uint32_t rd_th3       : 9;
		/* Reserved. */
		uint32_t __reserved_0 : 7;
		/* write state shift threshold */
		uint32_t wr_th3       : 9;
		/* Reserved. */
		uint32_t __reserved_1 : 7;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1 : 7;
		uint32_t wr_th3       : 9;
		uint32_t __reserved_0 : 7;
		uint32_t rd_th3       : 9;
#endif
	};

	uint32_t word;
} EMI_RDWR_SM_PROFILE_1_t;


/*
 * RDWR_SM_PROFILE_2.
 * Parameters of the rdwr sm profiles
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* read state shift threshold */
		uint32_t rd_th3       : 9;
		/* Reserved. */
		uint32_t __reserved_0 : 7;
		/* write state shift threshold */
		uint32_t wr_th3       : 9;
		/* Reserved. */
		uint32_t __reserved_1 : 7;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1 : 7;
		uint32_t wr_th3       : 9;
		uint32_t __reserved_0 : 7;
		uint32_t rd_th3       : 9;
#endif
	};

	uint32_t word;
} EMI_RDWR_SM_PROFILE_2_t;


/*
 * RDWR_SM_PROFILE_3.
 * Parameters of the rdwr sm profiles
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* read state shift threshold */
		uint32_t rd_th3       : 9;
		/* Reserved. */
		uint32_t __reserved_0 : 7;
		/* write state shift threshold */
		uint32_t wr_th3       : 9;
		/* Reserved. */
		uint32_t __reserved_1 : 7;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1 : 7;
		uint32_t wr_th3       : 9;
		uint32_t __reserved_0 : 7;
		uint32_t rd_th3       : 9;
#endif
	};

	uint32_t word;
} EMI_RDWR_SM_PROFILE_3_t;


/*
 * RDWR_SM_PROFILE_4.
 * Parameters of the rdwr sm profiles
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* read state shift threshold */
		uint32_t rd_th3       : 9;
		/* Reserved. */
		uint32_t __reserved_0 : 7;
		/* write state shift threshold */
		uint32_t wr_th3       : 9;
		/* Reserved. */
		uint32_t __reserved_1 : 7;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1 : 7;
		uint32_t wr_th3       : 9;
		uint32_t __reserved_0 : 7;
		uint32_t rd_th3       : 9;
#endif
	};

	uint32_t word;
} EMI_RDWR_SM_PROFILE_4_t;


/*
 * RDWR_SM_PROFILE_5.
 * Parameters of the rdwr sm profiles
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* read state shift threshold */
		uint32_t rd_th3       : 9;
		/* Reserved. */
		uint32_t __reserved_0 : 7;
		/* write state shift threshold */
		uint32_t wr_th3       : 9;
		/* Reserved. */
		uint32_t __reserved_1 : 7;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1 : 7;
		uint32_t wr_th3       : 9;
		uint32_t __reserved_0 : 7;
		uint32_t rd_th3       : 9;
#endif
	};

	uint32_t word;
} EMI_RDWR_SM_PROFILE_5_t;


/*
 * RDWR_SM_PROFILE_6.
 * Parameters of the rdwr sm profiles
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* read state shift threshold */
		uint32_t rd_th3       : 9;
		/* Reserved. */
		uint32_t __reserved_0 : 7;
		/* write state shift threshold */
		uint32_t wr_th3       : 9;
		/* Reserved. */
		uint32_t __reserved_1 : 7;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1 : 7;
		uint32_t wr_th3       : 9;
		uint32_t __reserved_0 : 7;
		uint32_t rd_th3       : 9;
#endif
	};

	uint32_t word;
} EMI_RDWR_SM_PROFILE_6_t;


/*
 * RDWR_SM_PROFILE_7.
 * Parameters of the rdwr sm profiles
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* read state shift threshold */
		uint32_t rd_th3       : 9;
		/* Reserved. */
		uint32_t __reserved_0 : 7;
		/* write state shift threshold */
		uint32_t wr_th3       : 9;
		/* Reserved. */
		uint32_t __reserved_1 : 7;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1 : 7;
		uint32_t wr_th3       : 9;
		uint32_t __reserved_0 : 7;
		uint32_t rd_th3       : 9;
#endif
	};

	uint32_t word;
} EMI_RDWR_SM_PROFILE_7_t;


/*
 * ADD_IND_DATA.
 * These registers holds the ECC data for Host Write/Read with ECC Bypass
 * commands.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Value:
		 * - SRAM Self-init: Bitmap of SRAMs to initialize (line (OP
		 * = 0x0 in register IND_CMD). E.g. if the b0 = 1 in the SRAM
		 * bitmap this indicates the first SRAM in the block.
		 * - SRAM: Data to write to SRAM or data read from the SRAM
		 * (OP = 0x1/2/3/4/5 in register IND_CMD).
		 * - Indirect Register: Data to write to the indirect
		 * register or data read from the indirect data register (OP
		 * = 0x8/0x9 in register IND_CMD).
		 */
		uint32_t val : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t val : 32;
#endif
	};

	uint32_t word;
} EMI_ADD_IND_DATA_t;


/*
 * DEBUG_CTRL.
 * Chicken Bits Registers
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Disable the fix for PHY Update ACK
		 * 0 - Fix is enabled
		 * 1 - Fix is disabled
		 */
		uint32_t phy_update_ack_fix_dis : 1;
		/* Reserved. */
		uint32_t __reserved             : 31;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved             : 31;
		uint32_t phy_update_ack_fix_dis : 1;
#endif
	};

	uint32_t word;
} EMI_DEBUG_CTRL_t;

#endif /* !defined(__DOXYGEN__) */

#endif /* !defined(__ASSEMBLER__) */

#endif /* !defined(__REGS_EMI_H__) */
