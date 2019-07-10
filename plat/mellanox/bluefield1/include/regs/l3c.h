#ifndef __REGS_L3C_H__
#define __REGS_L3C_H__

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif
#include "l3c_def.h"

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
	};

	uint64_t word;
} L3C_DEV_INFO_t;


/*
 * Device Control.
 * This register provides general device control.
 */

__extension__
typedef union {
	struct {
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
	};

	uint64_t word;
} L3C_DEV_CTL_t;


/* Scratchpad. */

__extension__
typedef union {
	struct {
		/* Scratchpad. */
		uint64_t scratchpad : 64;
	};

	uint64_t word;
} L3C_SCRATCHPAD_t;


/*
 * Semaphore0.
 * Semaphore
 */

__extension__
typedef union {
	struct {
		/*
		 * When read, the current semaphore value is returned and the
		 * semaphore is set to 1.  Bit can also be written to 1 or 0.
		 */
		uint64_t val        : 1;
		/* Reserved. */
		uint64_t __reserved : 63;
	};

	uint64_t word;
} L3C_SEMAPHORE0_t;


/*
 * DMA Status.
 * DMA status information for debug. Unused for devices that do not have DMA.
 */

__extension__
typedef union {
	struct {
		uint64_t rdq_infl_count : 9;
		uint64_t wrq_infl_count : 9;
		/* Reserved. */
		uint64_t __reserved     : 7;
		/* Internal diagnostics status */
		uint64_t wrq_diag_vec   : 39;
	};

	uint64_t word;
} L3C_DMA_STATUS_t;


/* Clock Count. */

__extension__
typedef union {
	struct {
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
	};

	uint64_t word;
} L3C_CLOCK_COUNT_t;


/*
 * Interrupt Setup.
 * Configuration for device interrupts.
 */

__extension__
typedef union {
	struct {
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
	};

	uint64_t word;
} L3C_INT_SETUP_t;


/*
 * Feature Control.
 * Device-specific feature controls.
 */

__extension__
typedef union {
	struct {
		/*
		 * Feature Control.
		 * Device-specific feature controls.
		 */
		uint64_t feature_ctl : 64;
	};

	uint64_t word;
} L3C_FEATURE_CTL_t;


/*
 * Credit Control.
 * Provides access to the request-credit counters that control end-to-end
 * flow control between the device and other nodes in the system.
 */

__extension__
typedef union {
	struct {
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
	};

	uint64_t word;
} L3C_CRED_CTL_t;


/*
 * SAM Control.
 * Provides access to SAM initialization.
 */

__extension__
typedef union {
	struct {
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
	};

	uint64_t word;
} L3C_SAM_CTL_t;


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
	};

	uint32_t word;
} L3C_IND_DATA_t;


/*
 * IND_CMD.
 * Indirect Command: Indirect register/SRAM command. The status is in
 * register IND_STS.
 */

__extension__
typedef union {
	struct {
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
	};

	uint32_t word;
} L3C_IND_CMD_t;


/*
 * IND_CMD_RANGE.
 * Indirect Command Range: The SRAM end-line address for SRAM write/modify
 * range.
 */

__extension__
typedef union {
	struct {
		/*
		 * Memory End Address: The SRAM end-line address for SRAM
		 * write/modify range
		 */
		uint32_t mem_end_addr : 32;
	};

	uint32_t word;
} L3C_IND_CMD_RANGE_t;


/*
 * IND_ADDR.
 * Indirect Address. Specifies the indirect register address or SRAM
 * line/start-line
 */

__extension__
typedef union {
	struct {
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
	};

	uint32_t word;
} L3C_IND_ADDR_t;


/*
 * IND_STS.
 * Indirect Status: Indicates the status of the IND_CMD
 */

__extension__
typedef union {
	struct {
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
	};

	uint32_t word;
} L3C_IND_STS_t;


/*
 * BLOCK_ID.
 * Block Identity: Block and sub-block ID number for internal debugging
 * purposes.
 */

__extension__
typedef union {
	struct {
		/* Block ID number. */
		uint32_t block     : 16;
		/* Sub-block ID number if relevant. */
		uint32_t sub_block : 16;
	};

	uint32_t word;
} L3C_BLOCK_ID_t;


/*
 * SPARE.
 * Spare register
 */

__extension__
typedef union {
	struct {
		/* Spare */
		uint32_t val : 32;
	};

	uint32_t word;
} L3C_SPARE_t;


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
		/* Value: ECC single errors counter. */
		uint32_t val        : 4;
		/* Reserved. */
		uint32_t __reserved : 28;
	};

	uint32_t word;
} L3C_ECC_SINGLE_ERROR_CNT_t;


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
		/* Value: ECC double errors counter. */
		uint32_t val        : 4;
		/* Reserved. */
		uint32_t __reserved : 28;
	};

	uint32_t word;
} L3C_ECC_DOUBLE_ERROR_CNT_t;


/*
 * ECC_ADDR_ERROR_SELECT.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * ECC Address Error Select: Specifies the ECC ID of the SRAM (ECCID)
 * according to register ECC_SINGLE_ERROR / ECC_DOUBLE_ERROR for register
 * ECC_ADDR_ERROR_DATA.
 */

__extension__
typedef union {
	struct {
		/* ECC ID Write Data: ECC ID of the SRAM */
		uint32_t eccid_wdata : 5;
		/* Reserved. */
		uint32_t __reserved  : 23;
		/*
		 * ECC split number (ECC Split) (ECC may cover sections of an
		 * SRAM line).
		 */
		uint32_t index       : 4;
	};

	uint32_t word;
} L3C_ECC_ADDR_ERROR_SELECT_t;


/*
 * ECC_ADDR_ERROR_DATA_LOW.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
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
		/* SRAM line number. */
		uint32_t address    : 16;
		/* Reserved. */
		uint32_t __reserved : 16;
	};

	uint32_t word;
} L3C_ECC_ADDR_ERROR_DATA_LOW_t;


/*
 * ECC_ADDR_ERROR_DATA_HIGH.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * ECC Address Error Data High: Provides ECC error additional information for
 * the SRAM specified in register ECC_ADDR_ERROR_SELECT.
 */

__extension__
typedef union {
	struct {
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
	};

	uint32_t word;
} L3C_ECC_ADDR_ERROR_DATA_HIGH_t;


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
		/* ECC ID of the SRAM (ECCID). */
		uint32_t id         : 5;
		/* Reserved. */
		uint32_t __reserved : 27;
	};

	uint32_t word;
} L3C_SERR_INJ_t;


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
		/* ECC ID of the SRAM (ECCID). */
		uint32_t id         : 5;
		/* Reserved. */
		uint32_t __reserved : 27;
	};

	uint32_t word;
} L3C_DERR_INJ_t;


/*
 * ECC_ID_FIRST_LAST_SERR.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * ECC ID First Last Single Error: Indicates the first and last ECC ID of the
 * SRAM (ECCID) for the block that had a single error (e.g. the block had
 * three ECC errors in three different SRAMs). This may be useful
 * understanding multi ECC error events in different memories
 */

__extension__
typedef union {
	struct {
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
	};

	uint32_t word;
} L3C_ECC_ID_FIRST_LAST_SERR_t;


/*
 * ECC_ID_FIRST_LAST_DERR.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * ECC ID First Last Double Error: Indicates the first and last ECC ID of the
 * SRAM (ECCID) for the block that had a double error (e.g. the block had
 * three ECC errors in three different SRAMs).
 */

__extension__
typedef union {
	struct {
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
	};

	uint32_t word;
} L3C_ECC_ID_FIRST_LAST_DERR_t;


/*
 * ECC_SINGLE_ERROR_0.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
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
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_0  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_1  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_2  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_3  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_4  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_5  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_0 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_1 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_2 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_3 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_4 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_5 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_ctrls_0     : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_ctrls_1     : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cob_ctrls_0       : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cob_ctrls_1       : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t trb_cmds_0        : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t trb_cmds_1        : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t efdb_data_0       : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t efdb_data_1       : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wrdb_data_low_0   : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wrdb_data_low_1   : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wrdb_data_high_0  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wrdb_data_high_1  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t emem_tag_attrs_0  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t emem_tag_attrs_1  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t emem_wb_fifo_0    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t emem_wb_fifo_1    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t chi_dbid_attrs    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cdn_absrb_fifo    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t ddn_absrb_fifo    : 1;
		/* Reserved. */
		uint32_t __reserved        : 1;
	};

	uint32_t word;
} L3C_ECC_SINGLE_ERROR_0_t;


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
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_0  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_1  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_2  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_3  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_4  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_5  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_0 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_1 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_2 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_3 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_4 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_5 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_ctrls_0     : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_ctrls_1     : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cob_ctrls_0       : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cob_ctrls_1       : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t trb_cmds_0        : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t trb_cmds_1        : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t efdb_data_0       : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t efdb_data_1       : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wrdb_data_low_0   : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wrdb_data_low_1   : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wrdb_data_high_0  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wrdb_data_high_1  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t emem_tag_attrs_0  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t emem_tag_attrs_1  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t emem_wb_fifo_0    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t emem_wb_fifo_1    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t chi_dbid_attrs    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cdn_absrb_fifo    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t ddn_absrb_fifo    : 1;
		/* Reserved. */
		uint32_t __reserved        : 1;
	};

	uint32_t word;
} L3C_ECC_SINGLE_ERROR_MASK_0_t;


/*
 * ECC_DOUBLE_ERROR_0.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
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
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_0  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_1  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_2  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_3  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_4  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_5  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_0 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_1 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_2 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_3 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_4 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_5 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_ctrls_0     : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_ctrls_1     : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cob_ctrls_0       : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cob_ctrls_1       : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t trb_cmds_0        : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t trb_cmds_1        : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t efdb_data_0       : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t efdb_data_1       : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wrdb_data_low_0   : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wrdb_data_low_1   : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wrdb_data_high_0  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wrdb_data_high_1  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t emem_tag_attrs_0  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t emem_tag_attrs_1  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t emem_wb_fifo_0    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t emem_wb_fifo_1    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t chi_dbid_attrs    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cdn_absrb_fifo    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t ddn_absrb_fifo    : 1;
		/* Reserved. */
		uint32_t __reserved        : 1;
	};

	uint32_t word;
} L3C_ECC_DOUBLE_ERROR_0_t;


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
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_0  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_1  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_2  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_3  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_4  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_low_5  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_0 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_1 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_2 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_3 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_4 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_data_high_5 : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_ctrls_0     : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cache_ctrls_1     : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cob_ctrls_0       : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cob_ctrls_1       : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t trb_cmds_0        : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t trb_cmds_1        : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t efdb_data_0       : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t efdb_data_1       : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wrdb_data_low_0   : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wrdb_data_low_1   : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wrdb_data_high_0  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t wrdb_data_high_1  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t emem_tag_attrs_0  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t emem_tag_attrs_1  : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t emem_wb_fifo_0    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t emem_wb_fifo_1    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t chi_dbid_attrs    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t cdn_absrb_fifo    : 1;
		/* Status bit for the "named" SRAM. EECID is bit number. */
		uint32_t ddn_absrb_fifo    : 1;
		/* Reserved. */
		uint32_t __reserved        : 1;
	};

	uint32_t word;
} L3C_ECC_DOUBLE_ERROR_MASK_0_t;


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
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cache_data_low_0  : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cache_data_low_1  : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cache_data_low_2  : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cache_data_low_3  : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cache_data_low_4  : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cache_data_low_5  : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cache_data_high_0 : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cache_data_high_1 : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cache_data_high_2 : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cache_data_high_3 : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cache_data_high_4 : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cache_data_high_5 : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cache_ctrls_0     : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cache_ctrls_1     : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cob_ctrls_0       : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cob_ctrls_1       : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t trb_cmds_0        : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t trb_cmds_1        : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t efdb_data_0       : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t efdb_data_1       : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t wrdb_data_low_0   : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t wrdb_data_low_1   : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t wrdb_data_high_0  : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t wrdb_data_high_1  : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t emem_tag_attrs_0  : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t emem_tag_attrs_1  : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t emem_wb_fifo_0    : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t emem_wb_fifo_1    : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t chi_dbid_attrs    : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t cdn_absrb_fifo    : 1;
		/* ECC correction control bit for the "named" SRAM. */
		uint32_t ddn_absrb_fifo    : 1;
		/* Reserved. */
		uint32_t __reserved        : 1;
	};

	uint32_t word;
} L3C_ECC_DISABLE_FIX_0_t;


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
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cache_data_low_0  : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cache_data_low_1  : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cache_data_low_2  : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cache_data_low_3  : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cache_data_low_4  : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cache_data_low_5  : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cache_data_high_0 : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cache_data_high_1 : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cache_data_high_2 : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cache_data_high_3 : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cache_data_high_4 : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cache_data_high_5 : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cache_ctrls_0     : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cache_ctrls_1     : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cob_ctrls_0       : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cob_ctrls_1       : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t trb_cmds_0        : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t trb_cmds_1        : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t efdb_data_0       : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t efdb_data_1       : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t wrdb_data_low_0   : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t wrdb_data_low_1   : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t wrdb_data_high_0  : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t wrdb_data_high_1  : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t emem_tag_attrs_0  : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t emem_tag_attrs_1  : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t emem_wb_fifo_0    : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t emem_wb_fifo_1    : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t chi_dbid_attrs    : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t cdn_absrb_fifo    : 1;
		/* ECC status indication control bit for the "named" SRAM. */
		uint32_t ddn_absrb_fifo    : 1;
		/* Reserved. */
		uint32_t __reserved        : 1;
	};

	uint32_t word;
} L3C_ECC_DISABLE_STATUS_0_t;


/*
 * ECC_LATCH_MODE_0.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
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
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cache_data_low_0  : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cache_data_low_1  : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cache_data_low_2  : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cache_data_low_3  : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cache_data_low_4  : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cache_data_low_5  : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cache_data_high_0 : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cache_data_high_1 : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cache_data_high_2 : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cache_data_high_3 : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cache_data_high_4 : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cache_data_high_5 : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cache_ctrls_0     : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cache_ctrls_1     : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cob_ctrls_0       : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cob_ctrls_1       : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t trb_cmds_0        : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t trb_cmds_1        : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t efdb_data_0       : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t efdb_data_1       : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t wrdb_data_low_0   : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t wrdb_data_low_1   : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t wrdb_data_high_0  : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t wrdb_data_high_1  : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t emem_tag_attrs_0  : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t emem_tag_attrs_1  : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t emem_wb_fifo_0    : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t emem_wb_fifo_1    : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t chi_dbid_attrs    : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t cdn_absrb_fifo    : 1;
		/* ECC latch mode control bit for the "named" SRAM. */
		uint32_t ddn_absrb_fifo    : 1;
		/* Reserved. */
		uint32_t __reserved        : 1;
	};

	uint32_t word;
} L3C_ECC_LATCH_MODE_0_t;


/*
 * CHI_CFG.
 * CHI related configuration
 * - TAG_MASK, FWD_SHIFT, PASS_DIRTY_SHIFT
 *   configurations to select stream using CHI hooks.
 *   stream = PMR_Base + (steering_tag_masked | wr and (
 * forward_indication,number of sharers)_shifted | rd and pass_dirty_shifted)
 * *redundant since profile is calculated in HNF, passed on the transaction*
 */

__extension__
typedef union {
	struct {
		/*
		 * steering tag mask bitmap. ANDed with steering tag received
		 * over CHI.
		 * *redundant*
		 */
		uint32_t tag_mask         : 8;
		/*
		 * forward state shift left.
		 * 0x0-0x7: shift left by 0-7 bits
		 * 0x8-0xf: disabled mode, forward state doesn't affect
		 * stream select.
		 * *redundant*
		 */
		uint32_t fwd_shift        : 4;
		/*
		 * pass dirty bit shift left.
		 * 0x0-0x7: shift left by 0-7 bits
		 * 0x8-0xf: disabled mode,
		 * pass dirty doesn't affect stream select.
		 * *redundant*
		 */
		uint32_t pass_dirty_shift : 4;
		/* Reserved. */
		uint32_t __reserved       : 16;
	};

	uint32_t word;
} L3C_CHI_CFG_t;


/*
 * PROF_PROP.
 * Specifies the behavior of the cache population mechanism, miscellaneous
 * properties
 */

__extension__
typedef union {
	struct {
		/*
		 * Write through profile. when set, write commands linked to
		 * this profile would update both cache and DRAM.
		 */
		uint32_t wr_through : 16;
		/* Reserved. */
		uint32_t __reserved : 16;
	};

	uint32_t word;
} L3C_PROF_PROP_t;


/*
 * PROF_RD_MISS.
 * Specifies the behavior of the cache population mechanism for the case
 * where a non-posted request misses in the cache.
 */

__extension__
typedef union {
	struct {
		/*
		 * The LRU structure index to which read transactions should
		 * allocate, in case their corresponding binary randomization
		 * results in 0. Negative indexes (i.e. when MSB is 0) would
		 * mean that the transactions will not be allocated.
		 */
		uint32_t low_order            : 5;
		/*
		 * The LRU structure index to which read transactions should
		 * allocate, in case their corresponding binary randomization
		 * results in 1. Negative indexes (i.e. when MSB is 1) would
		 * mean that the transactions will not be allocated.
		 */
		uint32_t high_order           : 5;
		/* Reserved. */
		uint32_t __reserved_0         : 2;
		/*
		 * Specifies the initial state of the cache entry's State
		 * Bit, after a read transaction gets allocated into it.
		 */
		uint32_t alloc_state          : 1;
		/*
		 * Block allocation of read transaction, if the State Bit of
		 * the evicted cache entry is high, in case their
		 * corresponding binary randomization results in 0.
		 */
		uint32_t low_state_blk_alloc  : 1;
		/*
		 * Block allocation of read transaction, if the State Bit of
		 * the evicted cache entry is high, in case their
		 * corresponding binary randomization results in 1.
		 */
		uint32_t high_state_blk_alloc : 1;
		/* Reserved. */
		uint32_t __reserved_1         : 1;
		/*
		 * The probability of a read transaction that get a miss in
		 * the cache, to be allocated to the Read Miss High Order
		 * rather than to the Read Miss Low Order.
		 */
		uint32_t prob                 : 16;
	};

	uint32_t word;
} L3C_PROF_RD_MISS_t;


/*
 * PROF_WR_MISS.
 * Specifies the behavior of the cache population mechanism for the case
 * where a posted request misses in the cache.
 */

__extension__
typedef union {
	struct {
		/*
		 * The LRU structure index to which write transactions should
		 * allocate, in case their corresponding binary randomization
		 * results in 0. Negative indexes (i.e. when MSB is 0) would
		 * mean that the transactions will not be allocated.
		 */
		uint32_t low_order            : 5;
		/*
		 * The LRU structure index to which write transactions should
		 * allocate, in case their corresponding binary randomization
		 * results in 1. Negative indexes (i.e. when MSB is 1) would
		 * mean that the transactions will not be allocated.
		 */
		uint32_t high_order           : 5;
		/* Reserved. */
		uint32_t __reserved_0         : 2;
		/*
		 * Specifies the initial state of the cache entry's State
		 * Bit, after a write transaction gets allocated into it.
		 */
		uint32_t alloc_state          : 1;
		/*
		 * Block allocation of write transaction, if the State Bit of
		 * the evicted cache entry is high, in case their
		 * corresponding binary randomization results in 0.
		 */
		uint32_t low_state_blk_alloc  : 1;
		/*
		 * Block allocation of write transaction, if the State Bit of
		 * the evicted cache entry is high, in case their
		 * corresponding binary randomization results in 1.
		 */
		uint32_t high_state_blk_alloc : 1;
		/* Reserved. */
		uint32_t __reserved_1         : 1;
		/*
		 * The probability of a write transaction that get a miss in
		 * the cache, to be allocated to the Write Miss High Order
		 * rather than to the Write Miss Low Order.
		 */
		uint32_t prob                 : 16;
	};

	uint32_t word;
} L3C_PROF_WR_MISS_t;


/*
 * PROF_RD_HIT0.
 * Specifies the behavior of the cache population mechanism for the case
 * where a non-posted request hits in the cache, in an entry in which the
 * State Bit is 0.
 */

__extension__
typedef union {
	struct {
		/*
		 * A signed addition to LRU structure index of a cache entry,
		 * that is used when a read transactions gets a hit with that
		 * entry, in case the State Bit of that entry was 0, and the
		 * binary randomization for the transaction has resulted in
		 * 0. The index resulting from adding the delta will saturate
		 * from both directions, either at 0 (if the delta is
		 * negative) or at 15 (is the delta is positive).
		 */
		uint32_t low_delta    : 5;
		/*
		 * A signed addition to LRU structure index of a cache entry,
		 * that is used when a read transactions gets a hit with that
		 * entry, in case the State Bit of that entry was 0, and the
		 * binary randomization for the transaction has resulted in
		 * 1. The index resulting from adding the delta will saturate
		 * from both directions, either at 0 (if the delta is
		 * negative) or at 15 (is the delta is positive).
		 */
		uint32_t high_delta   : 5;
		/* Reserved. */
		uint32_t __reserved_0 : 2;
		/*
		 * Specifies the new state of the cache entry's State Bit,
		 * after a read transaction hits it, while the current State
		 * Bit is 0.
		 */
		uint32_t new_state    : 1;
		/* Reserved. */
		uint32_t __reserved_1 : 3;
		/*
		 * The probability of a read transaction that get a hit in
		 * the cache with an entry who's State Bit is 0, to apply the
		 * Read Hit0 High Delta on the entry that it hits, rather
		 * than to the Read Hit0 Low Delta.
		 */
		uint32_t prob         : 16;
	};

	uint32_t word;
} L3C_PROF_RD_HIT0_t;


/*
 * PROF_WR_HIT0.
 * Specifies the behavior of the cache population mechanism for the case
 * where a posted request hits in the cache, in an entry in which the State
 * Bit is 0.
 */

__extension__
typedef union {
	struct {
		/*
		 * A signed addition to LRU structure index of a cache entry,
		 * that is used when a write transactions gets a hit with
		 * that entry, in case the State Bit of that entry was 0, and
		 * the binary randomization for the transaction has resulted
		 * in 0. The index resulting from adding the delta will
		 * saturate from both directions, either at 0 (if the delta
		 * is negative) or at 15 (is the delta is positive).
		 */
		uint32_t low_delta    : 5;
		/*
		 * A signed addition to LRU structure index of a cache entry,
		 * that is used when a write transactions gets a hit with
		 * that entry, in case the State Bit of that entry was 0, and
		 * the binary randomization for the transaction has resulted
		 * in 1. The index resulting from adding the delta will
		 * saturate from both directions, either at 0 (if the delta
		 * is negative) or at 15 (is the delta is positive).
		 */
		uint32_t high_delta   : 5;
		/* Reserved. */
		uint32_t __reserved_0 : 2;
		/*
		 * Specifies the new state of the cache entry's State Bit,
		 * after a write transaction hits it, while the current State
		 * Bit is 0.
		 */
		uint32_t new_state    : 1;
		/* Reserved. */
		uint32_t __reserved_1 : 3;
		/*
		 * The probability of a write transaction that get a hit in
		 * the cache with an entry who's State Bit is 0, to apply the
		 * Write Hit0 High Delta on the entry that it hits, rather
		 * than to the Write Hit0 Low Delta.
		 */
		uint32_t prob         : 16;
	};

	uint32_t word;
} L3C_PROF_WR_HIT0_t;


/*
 * PROF_RD_HIT1.
 * Specifies the behavior of the cache population mechanism for the case
 * where a non-posted request hits in the cache, in an entry in which the
 * State Bit is 1.
 */

__extension__
typedef union {
	struct {
		/*
		 * A signed addition to LRU structure index of a cache entry,
		 * that is used when a read transactions gets a hit with that
		 * entry, in case the State Bit of that entry was 1, and the
		 * binary randomization for the transaction has resulted in
		 * 0. The index resulting from adding the delta will saturate
		 * from both directions, either at 0 (if the delta is
		 * negative) or at 15 (is the delta is positive).
		 */
		uint32_t low_delta    : 5;
		/*
		 * A signed addition to LRU structure index of a cache entry,
		 * that is used when a read transactions gets a hit with that
		 * entry, in case the State Bit of that entry was 1, and the
		 * binary randomization for the transaction has resulted in
		 * 1. The index resulting from adding the delta will saturate
		 * from both directions, either at 0 (if the delta is
		 * negative) or at 15 (is the delta is positive).
		 */
		uint32_t high_delta   : 5;
		/* Reserved. */
		uint32_t __reserved_0 : 2;
		/*
		 * Specifies the new state of the cache entry's State Bit,
		 * after a read transaction hits it, while the current State
		 * Bit is 1.
		 */
		uint32_t new_state    : 1;
		/* Reserved. */
		uint32_t __reserved_1 : 3;
		/*
		 * The probability of a read transaction that get a hit in
		 * the cache with an entry who's State Bit is 1, to apply the
		 * Read Hit1 High Delta on the entry that it hits, rather
		 * than to the Read Hit1 Low Delta.
		 */
		uint32_t prob         : 16;
	};

	uint32_t word;
} L3C_PROF_RD_HIT1_t;


/*
 * PROF_WR_HIT1.
 * Specifies the behavior of the cache population mechanism for the case
 * where a posted request hits in the cache, in an entry in which the State
 * Bit is 1.
 */

__extension__
typedef union {
	struct {
		/*
		 * A signed addition to LRU structure index of a cache entry,
		 * that is used when a write transactions gets a hit with
		 * that entry, in case the State Bit of that entry was 1, and
		 * the binary randomization for the transaction has resulted
		 * in 0. The index resulting from adding the delta will
		 * saturate from both directions, either at 0 (if the delta
		 * is negative) or at 15 (is the delta is positive).
		 */
		uint32_t low_delta    : 5;
		/*
		 * A signed addition to LRU structure index of a cache entry,
		 * that is used when a write transactions gets a hit with
		 * that entry, in case the State Bit of that entry was 1, and
		 * the binary randomization for the transaction has resulted
		 * in 1. The index resulting from adding the delta will
		 * saturate from both directions, either at 0 (if the delta
		 * is negative) or at 15 (is the delta is positive).
		 */
		uint32_t high_delta   : 5;
		/* Reserved. */
		uint32_t __reserved_0 : 2;
		/*
		 * Specifies the new state of the cache entry's State Bit,
		 * after a write transaction hits it, while the current State
		 * Bit is 1.
		 */
		uint32_t new_state    : 1;
		/* Reserved. */
		uint32_t __reserved_1 : 3;
		/*
		 * The probability of a write transaction that get a hit in
		 * the cache with an entry who's State Bit is 1, to apply the
		 * Write Hit1 High Delta on the entry that it hits, rather
		 * than to the Write Hit1 Low Delta.
		 */
		uint32_t prob         : 16;
	};

	uint32_t word;
} L3C_PROF_WR_HIT1_t;


/*
 * LFSR_PROB_MISS.
 * seed load for miss probability LFSR
 */

__extension__
typedef union {
	struct {
		/* seed to load into LFSR */
		uint32_t seed       : 24;
		/* Reserved. */
		uint32_t __reserved : 8;
	};

	uint32_t word;
} L3C_LFSR_PROB_MISS_t;


/*
 * LFSR_PROB_HIT0.
 * seed load for hit0 probability LFSR
 */

__extension__
typedef union {
	struct {
		/* seed to load into LFSR */
		uint32_t seed       : 24;
		/* Reserved. */
		uint32_t __reserved : 8;
	};

	uint32_t word;
} L3C_LFSR_PROB_HIT0_t;


/*
 * LFSR_PROB_HIT1.
 * seed load for hit1 probability LFSR
 */

__extension__
typedef union {
	struct {
		/* seed to load into LFSR */
		uint32_t seed       : 24;
		/* Reserved. */
		uint32_t __reserved : 8;
	};

	uint32_t word;
} L3C_LFSR_PROB_HIT1_t;


/*
 * MAIN_CFG.
 * L3C main configurations, features enabling
 */

__extension__
typedef union {
	struct {
		/* 1 - incoming interface enable, CHI requests */
		uint32_t chi_en         : 1;
		/* 1 - outgoing interface  enable, EMI requests */
		uint32_t emi_en         : 1;
		/* Reserved. */
		uint32_t __reserved_0   : 14;
		/*
		 * 1 - enable. once a ReadUnique request arrives, clean
		 * relevant entry's dirty bit, and pass dirty indication back
		 * to HN/RN-F.
		 */
		uint32_t rduniq_en      : 1;
		/* Reserved. */
		uint32_t __reserved_1   : 3;
		/*
		 * 1 - feature enable (for both rshim trigger and FORCE_FLUSH
		 * configuration).  L3/EMEM flush mechanism, go over all
		 * cache and write-back to DRAM all dirty entries.
		 */
		uint32_t flush_en       : 1;
		/*
		 * 1 - enable. when flush is in progress blocking new CHI
		 * transactions. when flush is done this blocking is
		 * deasserted. (POR)
		 * 0 - when flush is in progress new CHI transactions are
		 * also served.
		 */
		uint32_t flush_halt_chi : 1;
		/*
		 * 1 - force flush feature from host (POR is from rsim).
		 * L3/EMEM flush mechanism, go over all cache and write-back
		 * to DRAM all dirty entries. should be set on quiet system
		 * (with no new requests arrival). flush status is shown on
		 * FLUSH_STS register.
		 */
		uint32_t force_flush    : 1;
		/* Reserved. */
		uint32_t __reserved_2   : 9;
	};

	uint32_t word;
} L3C_MAIN_CFG_t;


/*
 * PMR_DFN.
 * The properties of a PMR (physical memory region) that are relevant for L3C
 */

__extension__
typedef union {
	struct {
		/*
		 * The 1GB-granular threshold, below which a PA belongs to
		 * this PMR (as long as it is also greater than or equal to
		 * the thresholds associated with the lower PMRs).
		 */
		uint32_t threshold    : 10;
		/* Reserved. */
		uint32_t __reserved_0 : 2;
		/*
		 * 0 - PMR targets non NVDIMM type memory
		 * 1 - PMR targets NVDIMM type memory
		 * this configuration is used  only for physical rank
		 * selection, when working in pmr selects physical rank type
		 * mode (EMEM_CFG_PMR_TO_RANK_EN==1).
		 */
		uint32_t dimm_type    : 1;
		/* Reserved. */
		uint32_t __reserved_1 : 3;
		/*
		 * The power of 2 of number of MSS SkyMesh/Ring ports that
		 * this PMR can be mapped to:
		 * 00- 1 MSS port
		 * 01- 2 MSS ports
		 * 10- 4 MSS ports
		 * 11- 8 MSS ports
		 */
		uint32_t mss_port_num : 2;
		/* Reserved. */
		uint32_t __reserved_2 : 2;
		/*
		 * The power of 2 of number of DDR devices that this PMR can
		 * be mapped to:
		 * 00- 1 device
		 * 01- 2 devices
		 * 10- reserved
		 * 11- reserved
		 * This configuration specifies the number of DDR devices for
		 * PMRs 0-7.
		 */
		uint32_t emem_num     : 2;
		/* Reserved. */
		uint32_t __reserved_3 : 2;
		/*
		 * The base stream that the L3C will associate to all CHI
		 * requests that belong to this PMR (The final stream will
		 * also be affected by cacheability hints from the directory).
		 * *redundant*
		 */
		uint32_t chi_stream   : 8;
	};

	uint32_t word;
} L3C_PMR_DFN_t;


/*
 * PMR_DFN_1.
 * The properties of a PMR (physical memory region) that are relevant for L3C
 */

__extension__
typedef union {
	struct {
		/*
		 * 1GB-granular threshold, used for alignment of address
		 * ranges (contiguousness on the DRAM channel) while
		 * converting from PA to DRAM address.
		 */
		uint32_t emi_base_th  : 10;
		/* Reserved. */
		uint32_t __reserved_0 : 2;
		/*
		 * add/sub the EMI_BASE_TH to/from the PA
		 * 0 - subtract
		 * 1 - add
		 */
		uint32_t emi_base_add : 1;
		/* Reserved. */
		uint32_t __reserved_1 : 19;
	};

	uint32_t word;
} L3C_PMR_DFN_1_t;


/*
 * SCRMBL_EN.
 * Various Scrambling Enable Bits
 */

__extension__
typedef union {
	struct {
		/*
		 * Allows turning L3C bank scrambling (for a given MSS
		 * SkyMesh port) on or off. Without this scrambling, the L3C
		 * bank is just 2 bits taken directly from the PA.
		 */
		uint32_t cache_bank   : 1;
		/* Reserved. */
		uint32_t __reserved_0 : 7;
		/*
		 * Allows turning L3C set scrambling (inside a given L3 cache
		 * bank) on or off. Without this scrambling, the L3C set is
		 * just 7 bits taken directly from the PA.
		 */
		uint32_t cache_set    : 1;
		/* Reserved. */
		uint32_t __reserved_1 : 7;
		/*
		 * Allows turning EMEM address scrambling for DDR (inside a
		 * given MSS) on and off. Without this scrambling, the EMEM
		 * address is just the PA decreased by the relevant PMR
		 * threshold (or by 2GB in case it is in PMR 0).
		 */
		uint32_t ddr_addr     : 1;
		/* Reserved. */
		uint32_t __reserved_2 : 7;
		/*
		 * Allows turning EMEM address scrambling for HMC (inside a
		 * given MSS) on and off. Without this scrambling, the EMEM
		 * address is just the PA decreased by the relevant PMR
		 * threshold.
		 * *redundant*
		 */
		uint32_t hmc_addr     : 1;
		/* Reserved. */
		uint32_t __reserved_3 : 7;
	};

	uint32_t word;
} L3C_SCRMBL_EN_t;


/*
 * EMEM_CFG.
 * EMEM related configuration
 */

__extension__
typedef union {
	struct {
		/*
		 * Represents the (power of 2 of the) number of PMRs sharing
		 * the same DDR device. The value 3 is illegal. The value of
		 * this field determines which PMR threshold is used as "the
		 * bottom of the region" for the purpose of calculating the
		 * EMEM address from the PA. using per PMR alignment in
		 * PMR_DFN_1 makes this configuration  *redundant*
		 */
		uint32_t pmr_share_ddr    : 2;
		/* Reserved. */
		uint32_t __reserved_0     : 6;
		/*
		 * Represents the (power of 2 of the) number of PMRs sharing
		 * the same HMC device. The value 3 is illegal. The value of
		 * this field determines which PMR threshold is used as "the
		 * bottom of the region" for the purpose of calculating the
		 * EMEM address from the PA. *redundant*
		 */
		uint32_t pmr_share_hmc    : 2;
		/* Reserved. */
		uint32_t __reserved_1     : 2;
		/*
		 * When PMR is shared over 2 or more DDR devices, this
		 * configuration selects the bit to peel.
		 * 0 - peel SM port LSB (PA[6]), POR.
		 * 1 - peel SM port MSB (PA[7]).
		 */
		uint32_t mss_is_msb       : 1;
		/* Reserved. */
		uint32_t __reserved_2     : 3;
		/*
		 * Physical Rank division enabled (on DDR devices). forward
		 * requests of non-NVDIMM PMR to lower physical rank/s and
		 * NVDIMM PMR to higher physical rank/s by replacing physical
		 * rank target msb using adjacent RANK_IS_QUAD and RANK_MAP
		 * configurations.
		 * i.e. DIMM / NVDIMM usage.
		 */
		uint32_t pmr_to_rank_en   : 1;
		/*
		 * when PMR_TO_RANK_EN is set.
		 * 0 - using 2 physical ranks, msb is located on EMEM_ADDR[0].
		 * 1 - using 4 physical ranks, msb is located on EMEM_ADDR[1]
		 */
		uint32_t rank_is_quad     : 1;
		/*
		 * when PMR_TO_RANK_EN is set. Physical Rank mapping
		 * 0 - map non-NVDIMM PMR to Rank 0 [Ranks 0-1 when quad] and
		 * NVDIMM  PMR to Rank 1 [Ranks 2-3 when quad].
		 * 1 - map non-NVDIMM PMR to Rank 1 [Ranks 2-3 when quad] and
		 * NVDIMM  PMR to Rank 0 [Ranks 0-1 when quad].
		 */
		uint32_t rank_map         : 1;
		/*
		 * when PMR_TO_RANK_EN and RANK_IS_QUAD are set, to use when
		 * non-NVDIMM type has 2 physical ranks and NVDIMM type has
		 * only one.
		 * 0 - NVDIMM type uses 2 physical ranks.
		 * 1 - NVDIMM type uses 1 physical rank (the lower index).
		 */
		uint32_t rank_uneven_quad : 1;
		/*
		 * when PMR_TO_RANK_EN is set. forcing zeroes on Logical Rank
		 * field for NVDIMM type. to use when non-NVDIMM/NVDIMM are
		 * 4H/2H respectively.
		 * 0 - no logical ranks.
		 * 1 - 1 bit logical rank.
		 * 2 - reserved.
		 * 3 - reserved.
		 * Note - 3DS and Non-3DS DIMMs on the same channel are not
		 * supported, i.e 4H and 1H
		 */
		uint32_t lrank_zero       : 2;
		/* Reserved. */
		uint32_t __reserved_3     : 2;
		/*
		 * single DRAM mode. using different address translation
		 * function (PA to emem_address) in order to prevent DRAM
		 * contention.
		 */
		uint32_t single_dram_mode : 1;
		/*
		 * single DRAM size.
		 * 0 -  2Gbit.
		 * 1 -  4Gbit.
		 * 2 -  8Gbit.
		 * 3 - 16Gbit
		 */
		uint32_t single_dram_size : 2;
		/* Reserved. */
		uint32_t __reserved_4     : 5;
	};

	uint32_t word;
} L3C_EMEM_CFG_t;


/*
 * SECURE_CFG.
 * CHI secure/non-secure check configurations
 */

__extension__
typedef union {
	struct {
		/* valid bitmap, bit per region */
		uint32_t val        : 8;
		/*
		 * permissions for non-region match.
		 * enable vector [3:0] as follows:
		 * 3: non-secure-write;
		 * 2: non-secure-read;
		 * 1: secure-write;
		 * 0: secure-read;
		 */
		uint32_t dflt_prm   : 4;
		/* Reserved. */
		uint32_t __reserved : 20;
	};

	uint32_t word;
} L3C_SECURE_CFG_t;


/*
 * SECURE_PRM.
 * permissions per region.
 * enable vector [3:0] as follows:
 * 3: non-secure-write;
 * 2: non-secure-read;
 * 1: secure-write;
 * 0: secure-read;
 * *Important*: Valid configuration per region should not contain secure and
 * non-secure permissions. e.g, 0x3 or 0xc are the values to be configured.
 */

__extension__
typedef union {
	struct {
		/* permissions per region. */
		uint32_t r0 : 4;
		/* permissions per region. */
		uint32_t r1 : 4;
		/* permissions per region. */
		uint32_t r2 : 4;
		/* permissions per region. */
		uint32_t r3 : 4;
		/* permissions per region. */
		uint32_t r4 : 4;
		/* permissions per region. */
		uint32_t r5 : 4;
		/* permissions per region. */
		uint32_t r6 : 4;
		/* permissions per region. */
		uint32_t r7 : 4;
	};

	uint32_t word;
} L3C_SECURE_PRM_t;


/*
 * SECURE_LOW_ADDR.
 * CHI secure/non-secure check configurations, lowest address of region
 */

__extension__
typedef union {
	struct {
		/*
		 * lowest Physical Address (PA) of secure region, included.
		 * 4k granularity (e.g. PA[39:12])
		 */
		uint32_t val        : 28;
		/* Reserved. */
		uint32_t __reserved : 4;
	};

	uint32_t word;
} L3C_SECURE_LOW_ADDR_t;


/*
 * SECURE_HIGH_ADDR.
 * CHI secure/non-secure check configurations, highest address of region
 */

__extension__
typedef union {
	struct {
		/*
		 * highest Physical Address (PA) of secure region, included.
		 * 4k granularity (e.g. PA[39:12])
		 */
		uint32_t val        : 28;
		/* Reserved. */
		uint32_t __reserved : 4;
	};

	uint32_t word;
} L3C_SECURE_HIGH_ADDR_t;


/*
 * REQ_ERR_SEL.
 * errored request is either:
 * - request with Physical Address out of PMR ranges.
 * - request with secure/non_secure permission violation
 * - request with Physical Address that match multiple overlapping secure
 * regions.
 */

__extension__
typedef union {
	struct {
		/* Enable capture for out of range request error. */
		uint32_t en_oor         : 1;
		/*
		 * Enable capture for secure/non_secure permission violation.
		 */
		uint32_t en_sec_err     : 1;
		/*
		 * Enable capture for secure/non_secure overlapping regions
		 * PA access.
		 */
		uint32_t en_sec_ovlp    : 1;
		/* Reserved. */
		uint32_t __reserved_0   : 1;
		/*
		 * 0 - capture last errored request.
		 * 1 - capture first errored request.
		 */
		uint32_t first_err_mode : 1;
		/* Reserved. */
		uint32_t __reserved_1   : 27;
	};

	uint32_t word;
} L3C_REQ_ERR_SEL_t;


/*
 * REQ_ERR_STS0.
 * errored request is either:
 * - request with Physical Address out of PMR ranges.
 * - request with secure/non_secure permission violation
 * - request with Physical Address that match multiple overlapping secure
 * regions.
 */

__extension__
typedef union {
	struct {
		/*
		 * Error capture fields are valid, out of range request was
		 * captured.
		 */
		uint32_t is_oor       : 1;
		/*
		 * Error capture fields are valid, secure/non_secure
		 * permission violation was captured.
		 */
		uint32_t is_sec_err   : 1;
		/*
		 * Error capture fields are valid, secure/non_secure
		 * overlapping regions PA accessed and captured.
		 */
		uint32_t is_sec_ovlp  : 1;
		/* Reserved. */
		uint32_t __reserved_0 : 1;
		/* Errored request SRCID. */
		uint32_t srcid        : 7;
		/* Reserved. */
		uint32_t __reserved_1 : 1;
		/* Errored request TXNID. */
		uint32_t txnid        : 8;
		/* Errored request PA[11:6] */
		uint32_t pa_lsb       : 6;
		/* Reserved. */
		uint32_t __reserved_2 : 6;
	};

	uint32_t word;
} L3C_REQ_ERR_STS0_t;


/*
 * REQ_ERR_STS1.
 * errored request is either:
 * - request with Physical Address out of PMR ranges.
 * - request with secure/non_secure permission violation
 * - request with Physical Address that match multiple overlapping secure
 * regions.
 */

__extension__
typedef union {
	struct {
		/* Errored request PA[39:12] - 4k granularity */
		uint32_t pa_msb     : 28;
		/* Reserved. */
		uint32_t __reserved : 4;
	};

	uint32_t word;
} L3C_REQ_ERR_STS1_t;


/*
 * REQ_ERR_CNT.
 * CHI errored request counter
 */

__extension__
typedef union {
	struct {
		/* counter */
		uint32_t val : 31;
		/* overflow */
		uint32_t ovf : 1;
	};

	uint32_t word;
} L3C_REQ_ERR_CNT_t;


/*
 * LTNCY_MON_CFG.
 * The average latency value is achieved by reading two different registers -
 * ACCUM_LTNCY and TRANS_CNT. In order to have atomic read, those counters
 * must stop counting before reading them.
 * For each Latency Monitor there is a stop bit and reset bit, for stopping
 * and resetting average latency counter and transaction counter.
 * Reading sequence should be as follows:
 * * Write 1 to the relevant stop bit
 * * Read accumulative latency counter
 * * Read transaction counter
 * * Write 1 to the relevant reset bit
 */

__extension__
typedef union {
	struct {
		/* 1 - enable continuous mode in all latency monitors. */
		uint32_t cont_mode    : 1;
		/* Reserved. */
		uint32_t __reserved_0 : 7;
		/*
		 * 1 - stop accumulative latency and transaction counters of
		 * CHI
		 */
		uint32_t stop_chi     : 1;
		/*
		 * 1 - stop accumulative latency and transaction counters of
		 * EMN
		 */
		uint32_t stop_emn     : 1;
		/* Reserved. */
		uint32_t __reserved_1 : 6;
		/*
		 * 1 - reset accumulative latency and transaction counters of
		 * CHI
		 */
		uint32_t reset_chi    : 1;
		/*
		 * 1 - reset accumulative latency and transaction counters of
		 * EMN
		 */
		uint32_t reset_emn    : 1;
		/* Reserved. */
		uint32_t __reserved_2 : 14;
	};

	uint32_t word;
} L3C_LTNCY_MON_CFG_t;


/*
 * CHI_LTNCY_MON_PRMS_0.
 * Controls whether or not CHI latency is being captured for monitoring, and
 * sets the properties for the captured transactions selection.
 */

__extension__
typedef union {
	struct {
		/*
		 * Only transactions with this PMR can be captured for
		 * latency monitoring (unless the field is masked)
		 */
		uint32_t pmr          : 3;
		/* Reserved. */
		uint32_t __reserved_0 : 1;
		/*
		 * PMR mask. Allows fully or partially ignoring the PMR
		 * field, when deciding whether or not a passing transaction
		 * should be capture. For each bit in the mask bitmap:
		 * 0 - the field's value in this bit will not be taken into
		 * account (don't care)
		 * 1 - the field's value in this bit will be taken into
		 * account
		 */
		uint32_t pmr_mask     : 3;
		/* Reserved. */
		uint32_t __reserved_1 : 1;
		/*
		 * Only transactions with this EXTTAG can be captured for
		 * latency monitoring (unless the field is masked)
		 */
		uint32_t exttag       : 4;
		/*
		 * EXTTAG mask. Allows fully or partially ignoring the EXTTAG
		 * field, when deciding whether or not a passing transaction
		 * should be capture. For each bit in the mask bitmap:
		 * 0 - the field's value in this bit will not be taken into
		 * account (don't care)
		 * 1 - the field's value in this bit will be taken into
		 * account
		 */
		uint32_t exttag_mask  : 4;
		/*
		 * Only transactions with this SRCID can be captured for
		 * latency monitoring (unless the field is masked)
		 */
		uint32_t srcid        : 7;
		/* Reserved. */
		uint32_t __reserved_2 : 1;
		/*
		 * SRCID mask. Allows fully or partially ignoring the SRCID
		 * field, when deciding whether or not a passing transaction
		 * should be capture. For each bit in the mask bitmap:
		 * 0 - the field's value in this bit will not be taken into
		 * account (don't care)
		 * 1 - the field's value in this bit will be taken into
		 * account
		 */
		uint32_t srcid_mask   : 7;
		/* Reserved. */
		uint32_t __reserved_3 : 1;
	};

	uint32_t word;
} L3C_CHI_LTNCY_MON_PRMS_0_t;


/*
 * CHI_LTNCY_MON_PRMS_1.
 * Controls whether or not CHI latency is being captured for monitoring, and
 * sets the properties for the captured transactions selection.
 */

__extension__
typedef union {
	struct {
		/*
		 * Only transactions with this OPCODE can be captured for
		 * latency monitoring (unless the field is masked)
		 */
		uint32_t opcode       : 5;
		/* Reserved. */
		uint32_t __reserved_0 : 3;
		/*
		 * OPCODE mask. Allows fully or partially ignoring the OPCODE
		 * field, when deciding whether or not a passing transaction
		 * should be capture. For each bit in the mask bitmap:
		 * 0 - the field's value in this bit will not be taken into
		 * account (don't care)
		 * 1 - the field's value in this bit will be taken into
		 * account
		 */
		uint32_t opcode_mask  : 5;
		/* Reserved. */
		uint32_t __reserved_1 : 19;
	};

	uint32_t word;
} L3C_CHI_LTNCY_MON_PRMS_1_t;


/*
 * CHI_LTNCY_MON.
 * Stores the latency of the captured CHI transaction, and bits indicating
 * the capture status.
 */

__extension__
typedef union {
	struct {
		/*
		 * 30 bits counter, measuring the latency of the captured CHI
		 * transaction.
		 */
		uint32_t cnt : 30;
		/*
		 * 00 - Awaits request
		 * 01 - Awaits response
		 * 10 - Idle, capture is valid
		 * 11 - Idle, no capture
		 */
		uint32_t sts : 2;
	};

	uint32_t word;
} L3C_CHI_LTNCY_MON_t;


/*
 * CHI_LTNCY_MON_MAX_LTNCY.
 * Stores the max latency of the captured CHI transaction.
 */

__extension__
typedef union {
	struct {
		/*
		 * 30 bits counter, measuring the max latency of the captured
		 * CHI transaction.
		 */
		uint32_t cnt        : 30;
		/* Reserved. */
		uint32_t __reserved : 2;
	};

	uint32_t word;
} L3C_CHI_LTNCY_MON_MAX_LTNCY_t;


/*
 * CHI_LTNCY_MON_TXN.
 * transactions counter of the captured CHI transaction for calculating
 * average latency.
 */

__extension__
typedef union {
	struct {
		/* transaction counter. */
		uint32_t cnt        : 28;
		/* Reserved. */
		uint32_t __reserved : 4;
	};

	uint32_t word;
} L3C_CHI_LTNCY_MON_TXN_t;


/*
 * CHI_LTNCY_MON_ACC.
 * Stores the accumulator latency  of the captured CHI transactions for
 * calculating average.
 */

__extension__
typedef union {
	struct {
		/* counter */
		uint32_t cnt : 31;
		/* overflow */
		uint32_t ovf : 1;
	};

	uint32_t word;
} L3C_CHI_LTNCY_MON_ACC_t;


/*
 * EMN_LTNCY_MON_PRMS.
 * Controls whether or not EMN latency is being captured for monitoring, and
 * sets the properties for the captured transactions selection.
 */

__extension__
typedef union {
	struct {
		/*
		 * Only transactions with this targets can be captured for
		 * latency monitoring (unless the field is masked). topology
		 * dependent:
		 * physical rank 0-2 bits
		 * logical rank 0-2 bits
		 * bank group 1-2 bits
		 * bank 2 bits
		 */
		uint32_t trgt       : 8;
		/*
		 * TRGT mask. Allows fully or partially ignoring the EXTTAG
		 * field, when deciding whether or not a passing transaction
		 * should be capture. For each bit in the mask bitmap:
		 * 0 - the field's value in this bit will not be taken into
		 * account (don't care)
		 * 1 - the field's value in this bit will be taken into
		 * account
		 */
		uint32_t trgt_mask  : 8;
		/*
		 * Only transactions of this type can be captured for latency
		 * monitoring (unless the field is masked).
		 * 0 - write transactions
		 * 1 - read transactions
		 */
		uint32_t cmd        : 1;
		/*
		 * CMD mask. Allows the CMD field, when deciding whether or
		 * not a passing transaction should be capture.
		 * 0 - the field's value will not be taken into account
		 * (don't care)
		 * 1 - the field's value will be taken into account
		 */
		uint32_t cmd_mask   : 1;
		/* Reserved. */
		uint32_t __reserved : 14;
	};

	uint32_t word;
} L3C_EMN_LTNCY_MON_PRMS_t;


/*
 * EMN_LTNCY_MON.
 * Stores the latency of the captured EMN transaction, and bits indicating
 * the capture status.
 */

__extension__
typedef union {
	struct {
		/*
		 * 30 bits counter, measuring the latency of the captured EMN
		 * transaction.
		 */
		uint32_t cnt : 30;
		/*
		 * 00 - Awaits request
		 * 01 - Awaits response
		 * 10 - Idle, capture is valid
		 * 11 - Idle, no capture
		 */
		uint32_t sts : 2;
	};

	uint32_t word;
} L3C_EMN_LTNCY_MON_t;


/*
 * EMN_LTNCY_MON_MAX_LTNCY.
 * Stores the max latency of the captured EMN transaction.
 */

__extension__
typedef union {
	struct {
		/*
		 * 30 bits counter, measuring the max latency of the captured
		 * EMN transaction.
		 */
		uint32_t cnt        : 30;
		/* Reserved. */
		uint32_t __reserved : 2;
	};

	uint32_t word;
} L3C_EMN_LTNCY_MON_MAX_LTNCY_t;


/*
 * EMN_LTNCY_MON_TXN.
 * transactions counter of the captured EMN transaction for calculating
 * average latency.
 */

__extension__
typedef union {
	struct {
		/* transaction counter. */
		uint32_t cnt        : 28;
		/* Reserved. */
		uint32_t __reserved : 4;
	};

	uint32_t word;
} L3C_EMN_LTNCY_MON_TXN_t;


/*
 * EMN_LTNCY_MON_ACC.
 * Stores the accumulator latency  of the captured EMN transactions for
 * calculating average.
 */

__extension__
typedef union {
	struct {
		/* counter */
		uint32_t cnt : 31;
		/* overflow */
		uint32_t ovf : 1;
	};

	uint32_t word;
} L3C_EMN_LTNCY_MON_ACC_t;


/*
 * FLUSH_STS.
 * L3C/EMEM flush status
 */

__extension__
typedef union {
	struct {
		/*
		 * 00 - IDLE
		 * 01 - L3 flush in progress
		 * 10 - EMEM flush in progress
		 * 11 - flush done (MAIN_CFG_FLUSH_EN is set)
		 */
		uint32_t state      : 2;
		/* Reserved. */
		uint32_t __reserved : 30;
	};

	uint32_t word;
} L3C_FLUSH_STS_t;


/*
 * PERF_CNT_CFG.
 * performance counters configurations
 */

__extension__
typedef union {
	struct {
		/* 1 - enable performance counters. */
		uint32_t en           : 1;
		/* 1 - reset all performance counters. */
		uint32_t rst          : 1;
		/* Reserved. */
		uint32_t __reserved_0 : 6;
		/* source id select, for relevant counters */
		uint32_t srcid_sel    : 7;
		/* Reserved. */
		uint32_t __reserved_1 : 1;
		/*
		 * source id mask, wildcard selecting with srcid_sel
		 * 1 - compare (matching srcid_sel bit is valid).
		 * 0 - ignore matching srcid_sel bit.
		 */
		uint32_t srcid_mask   : 7;
		/* Reserved. */
		uint32_t __reserved_2 : 1;
		/* profile select, for relevant counters */
		uint32_t prf_sel      : 4;
		/*
		 * profile mask, wildcard selecting with prf_sel
		 * 1 - compare (matching prf_sel bit is valid).
		 * 0 - ignore matching prf_sel bit.
		 */
		uint32_t prf_mask     : 4;
	};

	uint32_t word;
} L3C_PERF_CNT_CFG_t;


/*
 * PERF_CNT_CFG_1.
 * performance counters configurations
 */

__extension__
typedef union {
	struct {
		/* set select, for relevant counters */
		uint32_t set_sel       : 11;
		/* Reserved. */
		uint32_t __reserved_0  : 1;
		/*
		 * set mask, wildcard selecting with set_sel
		 * 1 - compare (matching set_sel bit is valid).
		 * 0 - ignore matching set_sel bit.
		 */
		uint32_t set_mask      : 11;
		/* Reserved. */
		uint32_t __reserved_1  : 1;
		/*
		 * count relevant counters when the usage is greater/equal to
		 * this threshold
		 */
		uint32_t emem_usage_th : 7;
		/* Reserved. */
		uint32_t __reserved_2  : 1;
	};

	uint32_t word;
} L3C_PERF_CNT_CFG_1_t;


/*
 * PERF_CNT_CFG_2.
 * performance counters configurations
 */

__extension__
typedef union {
	struct {
		/*
		 * profile select in bits [3:0], for relevant counters
		 * [7:4] - must be "0".
		 */
		uint32_t strm_sel   : 8;
		/*
		 * stream mask, wildcard selecting with strm_sel
		 * 1 - compare (matching strm_sel bit is valid).
		 * 0 - ignore matching strm_sel bit.
		 */
		uint32_t strm_mask  : 8;
		/* Reserved. */
		uint32_t __reserved : 16;
	};

	uint32_t word;
} L3C_PERF_CNT_CFG_2_t;


/*
 * PERF_CNT_SEL.
 * performance counters configurations
 */

__extension__
typedef union {
	struct {
		/* counter select */
		uint32_t cnt_0        : 6;
		/* Reserved. */
		uint32_t __reserved_0 : 2;
		/* counter select */
		uint32_t cnt_1        : 6;
		/* Reserved. */
		uint32_t __reserved_1 : 2;
		/* counter select */
		uint32_t cnt_2        : 6;
		/* Reserved. */
		uint32_t __reserved_2 : 2;
		/* counter select */
		uint32_t cnt_3        : 6;
		/* Reserved. */
		uint32_t __reserved_3 : 2;
	};

	uint32_t word;
} L3C_PERF_CNT_SEL_t;


/*
 * PERF_CNT_SEL_1.
 * performance counters configurations
 */

__extension__
typedef union {
	struct {
		/* counter select */
		uint32_t cnt_4      : 6;
		/* Reserved. */
		uint32_t __reserved : 26;
	};

	uint32_t word;
} L3C_PERF_CNT_SEL_1_t;


/*
 * PERF_CNT_LOW.
 * performance counters, 32 lsb
 */

__extension__
typedef union {
	struct {
		/* counter */
		uint32_t val : 32;
	};

	uint32_t word;
} L3C_PERF_CNT_LOW_t;


/*
 * PERF_CNT_HIGH.
 * performance counters, 24 msb
 */

__extension__
typedef union {
	struct {
		/* counter */
		uint32_t val        : 24;
		/* Reserved. */
		uint32_t __reserved : 8;
	};

	uint32_t word;
} L3C_PERF_CNT_HIGH_t;


/*
 * SMTAPIF_STS.
 * debug statusses
 */

__extension__
typedef union {
	struct {
		/* cdn absorb buffer fullness */
		uint32_t cdn_fullness     : 10;
		/* Reserved. */
		uint32_t __reserved_0     : 2;
		/* ddn absorb buffer fullness */
		uint32_t ddn_fullness     : 9;
		/* Reserved. */
		uint32_t __reserved_1     : 3;
		/* has valid DBID */
		uint32_t dbid_rdy         : 1;
		/* {full, empty} indications */
		uint32_t dbid_attr_fifo   : 2;
		/* {full, empty} indications */
		uint32_t dbid_res_fifo    : 2;
		/* {full, empty} indications */
		uint32_t dbid_atwait_fifo : 2;
		/* Reserved. */
		uint32_t __reserved_2     : 1;
	};

	uint32_t word;
} L3C_SMTAPIF_STS_t;


/*
 * SMTAPIF_STS_1.
 * debug statusses
 */

__extension__
typedef union {
	struct {
		/* credits towards banks, 2 bits per bank */
		uint32_t banks_crdt_cdn : 4;
		/* credits towards banks, 2 bits per bank */
		uint32_t banks_crdt_ddn : 4;
		/* {full, empty} indications */
		uint32_t comp_res_fifo  : 2;
		/* Reserved. */
		uint32_t __reserved_0   : 2;
		/* ndn credit count */
		uint32_t ndn_crdt       : 4;
		/* ddn response credit count */
		uint32_t ddn_rs_crdt    : 4;
		/* Reserved. */
		uint32_t __reserved_1   : 12;
	};

	uint32_t word;
} L3C_SMTAPIF_STS_1_t;


/*
 * CLNTREQ_STS.
 * debug statusses
 */

__extension__
typedef union {
	struct {
		/* write data buffer fullness */
		uint32_t wrdb_fullness : 8;
		/* {full, empty} indications */
		uint32_t sngl_alloc_db : 2;
		/* {full, empty} indications */
		uint32_t dual_alloc_db : 2;
		/* {full, empty} indications */
		uint32_t clnt_ddn_db   : 2;
		/* {full, empty} indications */
		uint32_t clnt_cdn_db   : 2;
		/* {full, empty} indications */
		uint32_t clntres_db    : 2;
		/* ready indication */
		uint32_t ctrlr_rdy     : 1;
		/* Reserved. */
		uint32_t __reserved    : 13;
	};

	uint32_t word;
} L3C_CLNTREQ_STS_t;


/*
 * CTRLR_STS.
 * debug statusses
 */

__extension__
typedef union {
	struct {
		/* command queue valid(busy) vector */
		uint32_t cmdq_vld : 16;
		/* collision buffer sets valid vector */
		uint32_t cob_vld  : 16;
	};

	uint32_t word;
} L3C_CTRLR_STS_t;


/*
 * CTRLR_STS_1.
 * cmdq states:
 *   0 - IDLE
 *   1 - CTRLR
 *   2 - LIST_CTRLR
 *   3 - LIST
 *   4 - ELIG_TRAIL
 *   5 - ELIG_FIRST
 *   6 - AWAITS_ACK
 *   7 - AWAITS_TRAIL
 */

__extension__
typedef union {
	struct {
		/* command queue state */
		uint32_t cmdq_0_state : 3;
		/* Reserved. */
		uint32_t __reserved_0 : 1;
		/* command queue state */
		uint32_t cmdq_1_state : 3;
		/* Reserved. */
		uint32_t __reserved_1 : 1;
		/* command queue state */
		uint32_t cmdq_2_state : 3;
		/* Reserved. */
		uint32_t __reserved_2 : 1;
		/* command queue state */
		uint32_t cmdq_3_state : 3;
		/* Reserved. */
		uint32_t __reserved_3 : 1;
		/* command queue state */
		uint32_t cmdq_4_state : 3;
		/* Reserved. */
		uint32_t __reserved_4 : 1;
		/* command queue state */
		uint32_t cmdq_5_state : 3;
		/* Reserved. */
		uint32_t __reserved_5 : 1;
		/* command queue state */
		uint32_t cmdq_6_state : 3;
		/* Reserved. */
		uint32_t __reserved_6 : 1;
		/* command queue state */
		uint32_t cmdq_7_state : 3;
		/* Reserved. */
		uint32_t __reserved_7 : 1;
	};

	uint32_t word;
} L3C_CTRLR_STS_1_t;


/*
 * CTRLR_STS_2.
 * cmdq states:
 *   0 - IDLE
 *   1 - CTRLR
 *   2 - LIST_CTRLR
 *   3 - LIST
 *   4 - ELIG_TRAIL
 *   5 - ELIG_FIRST
 *   6 - AWAITS_ACK
 *   7 - AWAITS_TRAIL
 */

__extension__
typedef union {
	struct {
		/* command queue state */
		uint32_t cmdq_8_state  : 3;
		/* Reserved. */
		uint32_t __reserved_0  : 1;
		/* command queue state */
		uint32_t cmdq_9_state  : 3;
		/* Reserved. */
		uint32_t __reserved_1  : 1;
		/* command queue state */
		uint32_t cmdq_10_state : 3;
		/* Reserved. */
		uint32_t __reserved_2  : 1;
		/* command queue state */
		uint32_t cmdq_11_state : 3;
		/* Reserved. */
		uint32_t __reserved_3  : 1;
		/* command queue state */
		uint32_t cmdq_12_state : 3;
		/* Reserved. */
		uint32_t __reserved_4  : 1;
		/* command queue state */
		uint32_t cmdq_13_state : 3;
		/* Reserved. */
		uint32_t __reserved_5  : 1;
		/* command queue state */
		uint32_t cmdq_14_state : 3;
		/* Reserved. */
		uint32_t __reserved_6  : 1;
		/* command queue state */
		uint32_t cmdq_15_state : 3;
		/* Reserved. */
		uint32_t __reserved_7  : 1;
	};

	uint32_t word;
} L3C_CTRLR_STS_2_t;


/*
 * CTRLR_STS_3.
 * debug statusses
 */

__extension__
typedef union {
	struct {
		/* current client request set */
		uint32_t cob_set          : 4;
		/* collision buffer credits for current client request set */
		uint32_t cob_set_crdt     : 4;
		/* {full, empty} indications */
		uint32_t cmdq_cche_db     : 2;
		/* {full, empty} indications */
		uint32_t cmdq_emem_std_db : 2;
		/* {full, empty} indications */
		uint32_t cmdq_emem_pri_db : 2;
		/* ready indication */
		uint32_t trb_rdy          : 1;
		/* Reserved. */
		uint32_t __reserved       : 17;
	};

	uint32_t word;
} L3C_CTRLR_STS_3_t;


/*
 * CARRAY_STS.
 * debug statusses
 */

__extension__
typedef union {
	struct {
		/* {full, empty} indications */
		uint32_t pipe_out_fifo : 2;
		/* Reserved. */
		uint32_t __reserved_0  : 2;
		/* credits */
		uint32_t pipe_out_crdt : 4;
		/* Reserved. */
		uint32_t __reserved_1  : 24;
	};

	uint32_t word;
} L3C_CARRAY_STS_t;


/*
 * EMEMREQ_STS.
 * debug statusses
 */

__extension__
typedef union {
	struct {
		/* open tags usage */
		uint32_t tag_usage      : 7;
		/* Reserved. */
		uint32_t __reserved_0   : 1;
		/* {full, empty} indications */
		uint32_t tag_attr_wr_db : 2;
		/* {full, empty} indications */
		uint32_t assmbled_db    : 2;
		/* {full, empty} indications */
		uint32_t eviction_db    : 2;
		/* {full, empty} indications */
		uint32_t tgt_db         : 2;
		/* credits */
		uint32_t tgt_crdt       : 2;
		/* Reserved. */
		uint32_t __reserved_1   : 14;
	};

	uint32_t word;
} L3C_EMEMREQ_STS_t;


/*
 * EMEMRES_STS.
 * debug statusses
 */

__extension__
typedef union {
	struct {
		/* {full, empty} indications */
		uint32_t rs_db            : 2;
		/* {full, empty} indications */
		uint32_t fetch_wait_fifo  : 2;
		/* {full, empty} indications */
		uint32_t nofetch_fifo     : 2;
		/* {full, empty} indications */
		uint32_t fetch_fifo       : 2;
		/* fullness */
		uint32_t wb_fifo_fullness : 7;
		/* Reserved. */
		uint32_t __reserved_0     : 1;
		/* {full, empty} indications */
		uint32_t clntres_fifo     : 2;
		/* Reserved. */
		uint32_t __reserved_1     : 14;
	};

	uint32_t word;
} L3C_EMEMRES_STS_t;


/*
 * CLNTRES_STS.
 * debug statusses
 */

__extension__
typedef union {
	struct {
		/* credits */
		uint32_t smtap_rdres_crdt : 2;
		/* Reserved. */
		uint32_t __reserved_0     : 2;
		/* credits */
		uint32_t smtap_ndn_crdt   : 2;
		/* Reserved. */
		uint32_t __reserved_1     : 26;
	};

	uint32_t word;
} L3C_CLNTRES_STS_t;


/*
 * GLOBAL_STS.
 * debug statusses
 */

__extension__
typedef union {
	struct {
		/* emi isolation enabled (lbist mode) */
		uint32_t emi_isolation : 1;
		/* Reserved. */
		uint32_t __reserved    : 31;
	};

	uint32_t word;
} L3C_GLOBAL_STS_t;


/*
 * INTRPT_CAUSE.
 * interrupt cause register
 */

__extension__
typedef union {
	struct {
		/* CHI requested address is out of range of PMR thresholds */
		uint32_t chi_addr_oor         : 1;
		/* CHI requested address is violating secure permissions */
		uint32_t chi_addr_secure_err  : 1;
		/* CHI requested address is overlapping secure regions */
		uint32_t chi_addr_secure_ovlp : 1;
		/* Reserved. */
		uint32_t __reserved           : 29;
	};

	uint32_t word;
} L3C_INTRPT_CAUSE_t;


/*
 * INTRPT_MASK.
 * 0 - disable interrupt
 * 1 - enable interrupt
 */

__extension__
typedef union {
	struct {
		/* 1 - enable interrupt */
		uint32_t chi_addr_oor         : 1;
		/* 1 - enable interrupt */
		uint32_t chi_addr_secure_err  : 1;
		/* 1 - enable interrupt */
		uint32_t chi_addr_secure_ovlp : 1;
		/* Reserved. */
		uint32_t __reserved           : 29;
	};

	uint32_t word;
} L3C_INTRPT_MASK_t;

#endif /* !defined(__DOXYGEN__) */

#endif /* !defined(__ASSEMBLER__) */

#endif /* !defined(__REGS_L3C_H__) */
