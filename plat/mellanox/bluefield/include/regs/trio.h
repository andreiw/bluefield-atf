#ifndef __REGS_TRIO_H__
#define __REGS_TRIO_H__

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif
#include "trio_def.h"

#ifndef __ASSEMBLER__


#ifndef __DOXYGEN__

/*
 * MMIO Address Space.
 * The MMIO physical address space for the TRIO is described below.  This is
 * a general description of the MMIO space as opposed to a register
 * description
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * This field of the address provides an offset into the
		 * region being accessed.
		 */
		uint64_t offset     : 20;
		/* Reserved. */
		uint64_t __reserved : 44;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 44;
		uint64_t offset     : 20;
#endif
	};

	uint64_t word;
} TRIO_MMIO_ADDRESS_SPACE_t;


/*
 * MMIO Region - Configuration Space Data Description.
 * Provides access to config registers.  See CFG_REGION_ADDR for address
 * definition.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Configuration read/write data */
		uint64_t data : 64;
#else   /* __BIG_ENDIAN__ */
		uint64_t data : 64;
#endif
	};

	uint64_t word;
} TRIO_CFG_REGION_VAL_t;


/*
 * Configuration Space Address Definition.
 * This is a general description of the MMIO space as opposed to a register
 * description.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Configuration register to be accessed.  Note that TRIO and
		 * ROUTER registers are always aligned on 8-byte boundaries
		 * (bits [2:0] must be zero) and access is always 8-bytes at
		 * a time.  CR_SPACE registers are 4-byte oriented and bits
		 * [1:0] must be zero.
		 */
		uint64_t reg        : 20;
		/* Interface being accessed. */
		uint64_t intfc      : 2;
		/* Reserved. */
		uint64_t __reserved : 42;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 42;
		uint64_t intfc      : 2;
		uint64_t reg        : 20;
#endif
	};

	uint64_t word;
} TRIO_CFG_REGION_ADDR_t;


/*
 * Device Info.
 * This register provides general information about the device attached to
 * this port and channel.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Encoded device Type - 0x14 to indicate TRIO */
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
} TRIO_DEV_INFO_t;


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
} TRIO_DEV_CTL_t;


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
} TRIO_SCRATCHPAD_t;


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
} TRIO_SEMAPHORE0_t;


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
} TRIO_DMA_STATUS_t;


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
} TRIO_CLOCK_COUNT_t;


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
} TRIO_INT_SETUP_t;


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
} TRIO_FEATURE_CTL_t;


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
} TRIO_CRED_CTL_t;


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
} TRIO_SAM_CTL_t;


/*
 * TX Credits - Posted.
 * Controls credits between TRIO and PXTL/H
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Each credit represents 256-bytes of data including header.
		 * When written, this signed integer will be added to the
		 * credit count. When read, returns the current value.
		 */
		uint64_t val        : 8;
		/* Reserved. */
		uint64_t __reserved : 56;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 56;
		uint64_t val        : 8;
#endif
	};

	uint64_t word;
} TRIO_TX_CREDIT_P_t;


/*
 * TX Credits - Non-Posted.
 * Controls credits between TRIO and PXTL/H
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Each credit represents 256-bytes of data including header.
		 * When written, this signed integer will be added to the
		 * credit count. When read, returns the current value.
		 */
		uint64_t val        : 8;
		/* Reserved. */
		uint64_t __reserved : 56;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 56;
		uint64_t val        : 8;
#endif
	};

	uint64_t word;
} TRIO_TX_CREDIT_NP_t;


/*
 * TX Credits - Completion.
 * Controls credits between TRIO and PXTL/H
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Each credit represents 256-bytes of data including header.
		 * When written, this signed integer will be added to the
		 * credit count. When read, returns the current value.
		 */
		uint64_t val        : 8;
		/* Reserved. */
		uint64_t __reserved : 56;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 56;
		uint64_t val        : 8;
#endif
	};

	uint64_t word;
} TRIO_TX_CREDIT_CPL_t;


/*
 * TX Credits - Completion.
 * Controls credits between TRIO and PXTL/H
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Each credit represents 256-bytes of data including header.
		 * When written, this signed integer will be added to the
		 * credit count. When read, returns the current value.
		 */
		uint64_t val        : 8;
		/* Reserved. */
		uint64_t __reserved : 56;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 56;
		uint64_t val        : 8;
#endif
	};

	uint64_t word;
} TRIO_TX_CREDIT_CPL_1_t;


/*
 * TX Credits - Completion.
 * Controls credits between TRIO and PXTL/H
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Each credit represents 256-bytes of data including header.
		 * When written, this signed integer will be added to the
		 * credit count. When read, returns the current value.
		 */
		uint64_t val        : 8;
		/* Reserved. */
		uint64_t __reserved : 56;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 56;
		uint64_t val        : 8;
#endif
	};

	uint64_t word;
} TRIO_TX_CREDIT_CPL_2_t;


/*
 * TX Credits - Completion.
 * Controls credits between TRIO and PXTL/H
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Each credit represents 256-bytes of data including header.
		 * When written, this signed integer will be added to the
		 * credit count. When read, returns the current value.
		 */
		uint64_t val        : 8;
		/* Reserved. */
		uint64_t __reserved : 56;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 56;
		uint64_t val        : 8;
#endif
	};

	uint64_t word;
} TRIO_TX_CREDIT_CPL_3_t;


/*
 * TX Credits - Completion.
 * Controls credits between TRIO and PXTL/H
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Each credit represents 256-bytes of data including header.
		 * When written, this signed integer will be added to the
		 * credit count. When read, returns the current value.
		 */
		uint64_t val        : 8;
		/* Reserved. */
		uint64_t __reserved : 56;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 56;
		uint64_t val        : 8;
#endif
	};

	uint64_t word;
} TRIO_TX_CREDIT_CPL_4_t;


/*
 * TX Credits - Completion.
 * Controls credits between TRIO and PXTL/H
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Each credit represents 256-bytes of data including header.
		 * When written, this signed integer will be added to the
		 * credit count. When read, returns the current value.
		 */
		uint64_t val        : 8;
		/* Reserved. */
		uint64_t __reserved : 56;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 56;
		uint64_t val        : 8;
#endif
	};

	uint64_t word;
} TRIO_TX_CREDIT_CPL_5_t;


/*
 * TX Credits - Completion.
 * Controls credits between TRIO and PXTL/H
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Each credit represents 256-bytes of data including header.
		 * When written, this signed integer will be added to the
		 * credit count. When read, returns the current value.
		 */
		uint64_t val        : 8;
		/* Reserved. */
		uint64_t __reserved : 56;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 56;
		uint64_t val        : 8;
#endif
	};

	uint64_t word;
} TRIO_TX_CREDIT_CPL_6_t;


/*
 * TX Credits - Completion.
 * Controls credits between TRIO and PXTL/H
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Each credit represents 256-bytes of data including header.
		 * When written, this signed integer will be added to the
		 * credit count. When read, returns the current value.
		 */
		uint64_t val        : 8;
		/* Reserved. */
		uint64_t __reserved : 56;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 56;
		uint64_t val        : 8;
#endif
	};

	uint64_t word;
} TRIO_TX_CREDIT_CPL_7_t;


/*
 * Message TX Data.
 * Software-defined packet to send. Must be a posted packet type.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Written into a RAM at the entry pointed to by
		 * MSG_TX_PTR.RAM_PTR . RAM values can be read by setting the
		 * RD_RAM bit in MSG_TX_PTR register.
		 */
		uint64_t data : 64;
#else   /* __BIG_ENDIAN__ */
		uint64_t data : 64;
#endif
	};

	uint64_t word;
} TRIO_MSG_TX_PKT_t;


/*
 * Message TX Pointer.
 * Pointer into RAM holding Tx Msg header and data.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Pointer into RAM holding Tx Msg header and data. RAM can
		 * hold 16B header and up to 128B data payload. To write the
		 * RAM, write RAM_PTR to desired value and then write
		 * MSG_TX_PKT data.
		 */
		uint64_t ram_ptr    : 5;
		/*
		 * Set RD_RAM bit to 1 to read out MSG_TX_PKT DATA at RAM_PTR
		 * entry. Hardware will clear this bit once RAM is read. Do
		 * not attempt to write MSG_TX_CTL SEND bit while read is in
		 * progress.
		 */
		uint64_t rd_ram     : 1;
		/* Reserved. */
		uint64_t __reserved : 58;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 58;
		uint64_t rd_ram     : 1;
		uint64_t ram_ptr    : 5;
#endif
	};

	uint64_t word;
} TRIO_MSG_TX_PTR_t;


/*
 * Message TX Control.
 * Controls transmission of TX message.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * When written with a 1, packet in MSG_TX_PKT will be sent.
		 * Hardware will clear this bit once packet has been sent. Do
		 * not attempt to read the RAM using the RAM_PTR_RD_RAM bit
		 * while SEND bit is on.
		 */
		uint64_t send       : 1;
		/* Target link number for message. */
		uint64_t link_sel   : 3;
		/* Reserved. */
		uint64_t __reserved : 60;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 60;
		uint64_t link_sel   : 3;
		uint64_t send       : 1;
#endif
	};

	uint64_t word;
} TRIO_MSG_TX_CTL_t;


/*
 * Level Interrupt Set.
 * INTx messages normally control the 4 LEVEL_INT interrupts from TRIO.
 * However, software can use this register to manually override.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * When written with a 1, the associated interupt will be
		 * asserted.
		 */
		uint64_t val        : 4;
		/* Reserved. */
		uint64_t __reserved : 60;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 60;
		uint64_t val        : 4;
#endif
	};

	uint64_t word;
} TRIO_LEVEL_INT_SET_t;


/*
 * Level Interrupt Clear.
 * INTx messages normally control the 4 LEVEL_INT interrupts from TRIO.
 * However, software can use this register to manually override.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * When written with a 1, the associated interupt will be
		 * deasserted.
		 */
		uint64_t val        : 4;
		/* Reserved. */
		uint64_t __reserved : 60;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 60;
		uint64_t val        : 4;
#endif
	};

	uint64_t word;
} TRIO_LEVEL_INT_CLR_t;


/*
 * Error Status.
 * Indicators for various fatal and non-fatal TRIO error conditions
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Illegal opcode received on MMIO interface */
		uint64_t mmio_ill_opc : 1;
		/* Reserved. */
		uint64_t __reserved   : 63;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved   : 63;
		uint64_t mmio_ill_opc : 1;
#endif
	};

	uint64_t word;
} TRIO_ERROR_STATUS_t;


/*
 * MMIO Error Information.
 * Provides diagnostics information when an MMIO error occurs.  Captured
 * whenever the MMIO_ERR interrupt condition which is typically due to a size
 * error.  This does not update on a CFG_PROT interrupt.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Source Tile in {x[3:0],y[3:0]} format. */
		uint64_t src        : 8;
		/*
		 * Request size.  0=1B, 1=2B, 2=4B, 3=8B, 4=16B, 5=32B,
		 * 6=48B, 7=64B.
		 */
		uint64_t size       : 4;
		/* Full PA from request. */
		uint64_t pa         : 40;
		/*
		 * Opcode of request.  MMIO supports only MMIO_READ (0x0e)
		 * and MMIO_WRITE (0x0f).  All others are reserved and will
		 * only occur on a misconfigured TLB.
		 */
		uint64_t opc        : 5;
		/* Reserved. */
		uint64_t __reserved : 7;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 7;
		uint64_t opc        : 5;
		uint64_t pa         : 40;
		uint64_t size       : 4;
		uint64_t src        : 8;
#endif
	};

	uint64_t word;
} TRIO_MMIO_ERROR_INFO_t;


/*
 * MMIO Error Information.
 * Provides diagnostics information when an MMIO error occurs.  Captured
 * whenever the MMIO_ERR interrupt condition which is typically due to a size
 * error.  This does not update on a CFG_PROT interrupt.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Source Tile in {x[3:0],y[3:0]} format. */
		uint64_t src        : 8;
		/*
		 * Request size.  0=1B, 1=2B, 2=4B, 3=8B, 4=16B, 5=32B,
		 * 6=48B, 7=64B.
		 */
		uint64_t size       : 4;
		/* Full PA from request. */
		uint64_t pa         : 40;
		/*
		 * Opcode of request.  MMIO supports only MMIO_READ (0x0e)
		 * and MMIO_WRITE (0x0f).  All others are reserved and will
		 * only occur on a misconfigured TLB.
		 */
		uint64_t opc        : 5;
		/* Reserved. */
		uint64_t __reserved : 7;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 7;
		uint64_t opc        : 5;
		uint64_t pa         : 40;
		uint64_t size       : 4;
		uint64_t src        : 8;
#endif
	};

	uint64_t word;
} TRIO_MMIO_ERROR_INFO_HX_t;


/*
 * Panic Mode Control.
 * Controls for panic mode which is used for ingress transactions that do not
 * match any of the MAP regions.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * When a panic occurs, reads will be directed to PANIC_VA
		 * and writes will be directed to PANIC_VA+64.  The PANIC_VA
		 * is always cacheline aligned, hence this field represents
		 * the upper 43 bits of the VA and the low 6 bits of the VA
		 * are taken from the incoming transaction.
		 */
		uint64_t panic_va   : 43;
		/* Reserved. */
		uint64_t __reserved : 5;
		/* StreamID to use for panic accesses. */
		uint64_t stream_id  : 16;
#else   /* __BIG_ENDIAN__ */
		uint64_t stream_id  : 16;
		uint64_t __reserved : 5;
		uint64_t panic_va   : 43;
#endif
	};

	uint64_t word;
} TRIO_PANIC_MODE_CTL_t;


/*
 * Tile PIO Region - CFG Address Format.
 * This register describes the address format for PIO accesses when address
 * falls into a configuration region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Register Address (full byte address). */
		uint64_t reg_addr   : 12;
		/* Function Number */
		uint64_t fn         : 3;
		/* Device Number */
		uint64_t dev        : 5;
		/* BUS Number */
		uint64_t bus        : 8;
		/* Reserved. */
		uint64_t __reserved : 36;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 36;
		uint64_t bus        : 8;
		uint64_t dev        : 5;
		uint64_t fn         : 3;
		uint64_t reg_addr   : 12;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_CFG_ADDR_FMT_t;


/*
 * Tile PIO Prefetchable Base/Limit.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* 1MB aligned base address for prefetchable memory space. */
		uint64_t base_addr    : 17;
		/* Reserved. */
		uint64_t __reserved_0 : 15;
		/* 1MB aligned limit address for prefetchable memory space. */
		uint64_t lim_addr     : 17;
		/* Reserved. */
		uint64_t __reserved_1 : 15;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_1 : 15;
		uint64_t lim_addr     : 17;
		uint64_t __reserved_0 : 15;
		uint64_t base_addr    : 17;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_PF_t;


/*
 * Tile PIO Prefetchable Base/Limit.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* 1MB aligned base address for prefetchable memory space. */
		uint64_t base_addr    : 17;
		/* Reserved. */
		uint64_t __reserved_0 : 15;
		/* 1MB aligned limit address for prefetchable memory space. */
		uint64_t lim_addr     : 17;
		/* Reserved. */
		uint64_t __reserved_1 : 15;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_1 : 15;
		uint64_t lim_addr     : 17;
		uint64_t __reserved_0 : 15;
		uint64_t base_addr    : 17;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_PF_1_t;


/*
 * Tile PIO Prefetchable Base/Limit.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* 1MB aligned base address for prefetchable memory space. */
		uint64_t base_addr    : 17;
		/* Reserved. */
		uint64_t __reserved_0 : 15;
		/* 1MB aligned limit address for prefetchable memory space. */
		uint64_t lim_addr     : 17;
		/* Reserved. */
		uint64_t __reserved_1 : 15;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_1 : 15;
		uint64_t lim_addr     : 17;
		uint64_t __reserved_0 : 15;
		uint64_t base_addr    : 17;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_PF_2_t;


/*
 * Tile PIO Prefetchable Base/Limit.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* 1MB aligned base address for prefetchable memory space. */
		uint64_t base_addr    : 17;
		/* Reserved. */
		uint64_t __reserved_0 : 15;
		/* 1MB aligned limit address for prefetchable memory space. */
		uint64_t lim_addr     : 17;
		/* Reserved. */
		uint64_t __reserved_1 : 15;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_1 : 15;
		uint64_t lim_addr     : 17;
		uint64_t __reserved_0 : 15;
		uint64_t base_addr    : 17;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_PF_3_t;


/*
 * Tile PIO Prefetchable Base/Limit.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* 1MB aligned base address for prefetchable memory space. */
		uint64_t base_addr    : 17;
		/* Reserved. */
		uint64_t __reserved_0 : 15;
		/* 1MB aligned limit address for prefetchable memory space. */
		uint64_t lim_addr     : 17;
		/* Reserved. */
		uint64_t __reserved_1 : 15;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_1 : 15;
		uint64_t lim_addr     : 17;
		uint64_t __reserved_0 : 15;
		uint64_t base_addr    : 17;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_PF_4_t;


/*
 * Tile PIO Prefetchable Base/Limit.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* 1MB aligned base address for prefetchable memory space. */
		uint64_t base_addr    : 17;
		/* Reserved. */
		uint64_t __reserved_0 : 15;
		/* 1MB aligned limit address for prefetchable memory space. */
		uint64_t lim_addr     : 17;
		/* Reserved. */
		uint64_t __reserved_1 : 15;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_1 : 15;
		uint64_t lim_addr     : 17;
		uint64_t __reserved_0 : 15;
		uint64_t base_addr    : 17;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_PF_5_t;


/*
 * Tile PIO Prefetchable Base/Limit.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* 1MB aligned base address for prefetchable memory space. */
		uint64_t base_addr    : 17;
		/* Reserved. */
		uint64_t __reserved_0 : 15;
		/* 1MB aligned limit address for prefetchable memory space. */
		uint64_t lim_addr     : 17;
		/* Reserved. */
		uint64_t __reserved_1 : 15;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_1 : 15;
		uint64_t lim_addr     : 17;
		uint64_t __reserved_0 : 15;
		uint64_t base_addr    : 17;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_PF_6_t;


/*
 * Tile PIO Prefetchable Base/Limit.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* 1MB aligned base address for prefetchable memory space. */
		uint64_t base_addr    : 17;
		/* Reserved. */
		uint64_t __reserved_0 : 15;
		/* 1MB aligned limit address for prefetchable memory space. */
		uint64_t lim_addr     : 17;
		/* Reserved. */
		uint64_t __reserved_1 : 15;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_1 : 15;
		uint64_t lim_addr     : 17;
		uint64_t __reserved_0 : 15;
		uint64_t base_addr    : 17;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_PF_7_t;


/*
 * Tile PIO Non-Prefetchable Base/Limit.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * 1MB aligned base address for non-prefetchable (32-bit)
		 * memory space.
		 */
		uint64_t base_addr       : 12;
		/*
		 * 1MB aligned limit address for non-prefetchable (32-bit)
		 * memory space.
		 */
		uint64_t lim_addr        : 12;
		/* Reserved. */
		uint64_t __reserved      : 8;
		/*
		 * Bus address MSBs. These bits are used in the MSBs of the
		 * PCI bus address. These are set to zero in root complex
		 * applications in order to generate 32-bit addresses. But
		 * they can be set to the external host base address when
		 * used from an endpoint (e.g. SmartNIC).
		 */
		uint64_t bus_addr_offset : 32;
#else   /* __BIG_ENDIAN__ */
		uint64_t bus_addr_offset : 32;
		uint64_t __reserved      : 8;
		uint64_t lim_addr        : 12;
		uint64_t base_addr       : 12;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_NON_PF_t;


/*
 * Tile PIO Non-Prefetchable Base/Limit.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * 1MB aligned base address for non-prefetchable (32-bit)
		 * memory space.
		 */
		uint64_t base_addr       : 12;
		/*
		 * 1MB aligned limit address for non-prefetchable (32-bit)
		 * memory space.
		 */
		uint64_t lim_addr        : 12;
		/* Reserved. */
		uint64_t __reserved      : 8;
		/*
		 * Bus address MSBs. These bits are used in the MSBs of the
		 * PCI bus address. These are set to zero in root complex
		 * applications in order to generate 32-bit addresses. But
		 * they can be set to the external host base address when
		 * used from an endpoint (e.g. SmartNIC).
		 */
		uint64_t bus_addr_offset : 32;
#else   /* __BIG_ENDIAN__ */
		uint64_t bus_addr_offset : 32;
		uint64_t __reserved      : 8;
		uint64_t lim_addr        : 12;
		uint64_t base_addr       : 12;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_NON_PF_1_t;


/*
 * Tile PIO Non-Prefetchable Base/Limit.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * 1MB aligned base address for non-prefetchable (32-bit)
		 * memory space.
		 */
		uint64_t base_addr       : 12;
		/*
		 * 1MB aligned limit address for non-prefetchable (32-bit)
		 * memory space.
		 */
		uint64_t lim_addr        : 12;
		/* Reserved. */
		uint64_t __reserved      : 8;
		/*
		 * Bus address MSBs. These bits are used in the MSBs of the
		 * PCI bus address. These are set to zero in root complex
		 * applications in order to generate 32-bit addresses. But
		 * they can be set to the external host base address when
		 * used from an endpoint (e.g. SmartNIC).
		 */
		uint64_t bus_addr_offset : 32;
#else   /* __BIG_ENDIAN__ */
		uint64_t bus_addr_offset : 32;
		uint64_t __reserved      : 8;
		uint64_t lim_addr        : 12;
		uint64_t base_addr       : 12;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_NON_PF_2_t;


/*
 * Tile PIO Non-Prefetchable Base/Limit.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * 1MB aligned base address for non-prefetchable (32-bit)
		 * memory space.
		 */
		uint64_t base_addr       : 12;
		/*
		 * 1MB aligned limit address for non-prefetchable (32-bit)
		 * memory space.
		 */
		uint64_t lim_addr        : 12;
		/* Reserved. */
		uint64_t __reserved      : 8;
		/*
		 * Bus address MSBs. These bits are used in the MSBs of the
		 * PCI bus address. These are set to zero in root complex
		 * applications in order to generate 32-bit addresses. But
		 * they can be set to the external host base address when
		 * used from an endpoint (e.g. SmartNIC).
		 */
		uint64_t bus_addr_offset : 32;
#else   /* __BIG_ENDIAN__ */
		uint64_t bus_addr_offset : 32;
		uint64_t __reserved      : 8;
		uint64_t lim_addr        : 12;
		uint64_t base_addr       : 12;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_NON_PF_3_t;


/*
 * Tile PIO Non-Prefetchable Base/Limit.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * 1MB aligned base address for non-prefetchable (32-bit)
		 * memory space.
		 */
		uint64_t base_addr       : 12;
		/*
		 * 1MB aligned limit address for non-prefetchable (32-bit)
		 * memory space.
		 */
		uint64_t lim_addr        : 12;
		/* Reserved. */
		uint64_t __reserved      : 8;
		/*
		 * Bus address MSBs. These bits are used in the MSBs of the
		 * PCI bus address. These are set to zero in root complex
		 * applications in order to generate 32-bit addresses. But
		 * they can be set to the external host base address when
		 * used from an endpoint (e.g. SmartNIC).
		 */
		uint64_t bus_addr_offset : 32;
#else   /* __BIG_ENDIAN__ */
		uint64_t bus_addr_offset : 32;
		uint64_t __reserved      : 8;
		uint64_t lim_addr        : 12;
		uint64_t base_addr       : 12;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_NON_PF_4_t;


/*
 * Tile PIO Non-Prefetchable Base/Limit.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * 1MB aligned base address for non-prefetchable (32-bit)
		 * memory space.
		 */
		uint64_t base_addr       : 12;
		/*
		 * 1MB aligned limit address for non-prefetchable (32-bit)
		 * memory space.
		 */
		uint64_t lim_addr        : 12;
		/* Reserved. */
		uint64_t __reserved      : 8;
		/*
		 * Bus address MSBs. These bits are used in the MSBs of the
		 * PCI bus address. These are set to zero in root complex
		 * applications in order to generate 32-bit addresses. But
		 * they can be set to the external host base address when
		 * used from an endpoint (e.g. SmartNIC).
		 */
		uint64_t bus_addr_offset : 32;
#else   /* __BIG_ENDIAN__ */
		uint64_t bus_addr_offset : 32;
		uint64_t __reserved      : 8;
		uint64_t lim_addr        : 12;
		uint64_t base_addr       : 12;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_NON_PF_5_t;


/*
 * Tile PIO Non-Prefetchable Base/Limit.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * 1MB aligned base address for non-prefetchable (32-bit)
		 * memory space.
		 */
		uint64_t base_addr       : 12;
		/*
		 * 1MB aligned limit address for non-prefetchable (32-bit)
		 * memory space.
		 */
		uint64_t lim_addr        : 12;
		/* Reserved. */
		uint64_t __reserved      : 8;
		/*
		 * Bus address MSBs. These bits are used in the MSBs of the
		 * PCI bus address. These are set to zero in root complex
		 * applications in order to generate 32-bit addresses. But
		 * they can be set to the external host base address when
		 * used from an endpoint (e.g. SmartNIC).
		 */
		uint64_t bus_addr_offset : 32;
#else   /* __BIG_ENDIAN__ */
		uint64_t bus_addr_offset : 32;
		uint64_t __reserved      : 8;
		uint64_t lim_addr        : 12;
		uint64_t base_addr       : 12;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_NON_PF_6_t;


/*
 * Tile PIO Non-Prefetchable Base/Limit.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * 1MB aligned base address for non-prefetchable (32-bit)
		 * memory space.
		 */
		uint64_t base_addr       : 12;
		/*
		 * 1MB aligned limit address for non-prefetchable (32-bit)
		 * memory space.
		 */
		uint64_t lim_addr        : 12;
		/* Reserved. */
		uint64_t __reserved      : 8;
		/*
		 * Bus address MSBs. These bits are used in the MSBs of the
		 * PCI bus address. These are set to zero in root complex
		 * applications in order to generate 32-bit addresses. But
		 * they can be set to the external host base address when
		 * used from an endpoint (e.g. SmartNIC).
		 */
		uint64_t bus_addr_offset : 32;
#else   /* __BIG_ENDIAN__ */
		uint64_t bus_addr_offset : 32;
		uint64_t __reserved      : 8;
		uint64_t lim_addr        : 12;
		uint64_t base_addr       : 12;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_NON_PF_7_t;


/*
 * Tile PIO Config-Space Setup.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Transactions matching this bus number will be sent as
		 * Type0. All others will be Type1.
		 */
		uint64_t secondary_bus   : 8;
		uint64_t subordinate_bus : 8;
		/* Reserved. */
		uint64_t __reserved      : 48;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved      : 48;
		uint64_t subordinate_bus : 8;
		uint64_t secondary_bus   : 8;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_BUS_CFG_t;


/*
 * Tile PIO Config-Space Setup.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Transactions matching this bus number will be sent as
		 * Type0. All others will be Type1.
		 */
		uint64_t secondary_bus   : 8;
		uint64_t subordinate_bus : 8;
		/* Reserved. */
		uint64_t __reserved      : 48;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved      : 48;
		uint64_t subordinate_bus : 8;
		uint64_t secondary_bus   : 8;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_BUS_CFG_1_t;


/*
 * Tile PIO Config-Space Setup.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Transactions matching this bus number will be sent as
		 * Type0. All others will be Type1.
		 */
		uint64_t secondary_bus   : 8;
		uint64_t subordinate_bus : 8;
		/* Reserved. */
		uint64_t __reserved      : 48;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved      : 48;
		uint64_t subordinate_bus : 8;
		uint64_t secondary_bus   : 8;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_BUS_CFG_2_t;


/*
 * Tile PIO Config-Space Setup.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Transactions matching this bus number will be sent as
		 * Type0. All others will be Type1.
		 */
		uint64_t secondary_bus   : 8;
		uint64_t subordinate_bus : 8;
		/* Reserved. */
		uint64_t __reserved      : 48;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved      : 48;
		uint64_t subordinate_bus : 8;
		uint64_t secondary_bus   : 8;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_BUS_CFG_3_t;


/*
 * Tile PIO Config-Space Setup.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Transactions matching this bus number will be sent as
		 * Type0. All others will be Type1.
		 */
		uint64_t secondary_bus   : 8;
		uint64_t subordinate_bus : 8;
		/* Reserved. */
		uint64_t __reserved      : 48;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved      : 48;
		uint64_t subordinate_bus : 8;
		uint64_t secondary_bus   : 8;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_BUS_CFG_4_t;


/*
 * Tile PIO Config-Space Setup.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Transactions matching this bus number will be sent as
		 * Type0. All others will be Type1.
		 */
		uint64_t secondary_bus   : 8;
		uint64_t subordinate_bus : 8;
		/* Reserved. */
		uint64_t __reserved      : 48;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved      : 48;
		uint64_t subordinate_bus : 8;
		uint64_t secondary_bus   : 8;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_BUS_CFG_5_t;


/*
 * Tile PIO Config-Space Setup.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Transactions matching this bus number will be sent as
		 * Type0. All others will be Type1.
		 */
		uint64_t secondary_bus   : 8;
		uint64_t subordinate_bus : 8;
		/* Reserved. */
		uint64_t __reserved      : 48;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved      : 48;
		uint64_t subordinate_bus : 8;
		uint64_t secondary_bus   : 8;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_BUS_CFG_6_t;


/*
 * Tile PIO Config-Space Setup.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Transactions matching this bus number will be sent as
		 * Type0. All others will be Type1.
		 */
		uint64_t secondary_bus   : 8;
		uint64_t subordinate_bus : 8;
		/* Reserved. */
		uint64_t __reserved      : 48;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved      : 48;
		uint64_t subordinate_bus : 8;
		uint64_t secondary_bus   : 8;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_BUS_CFG_7_t;


/*
 * Root Complex Control.
 * Matches format and functionality of PCIe Root Control Register.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved_0 : 4;
		/*
		 * When 1, CRS (retry) on PCIe config read access to the
		 * Vendor ID register will result in the special 0x0001
		 * return value. For reads to other registers, or for all
		 * writes, or when SW_VIS=0, the hardware will automatically
		 * retry the request.
		 */
		uint64_t crs_sw_vis   : 1;
		/* Reserved. */
		uint64_t __reserved_1 : 59;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_1 : 59;
		uint64_t crs_sw_vis   : 1;
		uint64_t __reserved_0 : 4;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_RC_CTL_t;


/*
 * Tile PIO Completion Error Status.
 * Contains information for the most recent Tile PIO completion error
 * (response from MAC flagged an error).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Contains the TileID for the request that encountered an
		 * error.
		 */
		uint64_t src_tile     : 8;
		/* Reserved. */
		uint64_t __reserved_0 : 8;
		/* Region that encountered an error. */
		uint64_t region       : 3;
		/* Reserved. */
		uint64_t __reserved_1 : 5;
		/* Indicates error type that was received. */
		uint64_t err_sts      : 2;
		/* Reserved. */
		uint64_t __reserved_2 : 6;
		/* Clears error status fields when written with a 1. */
		uint64_t clr_sts      : 1;
		/* Reserved. */
		uint64_t __reserved_3 : 31;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_3 : 31;
		uint64_t clr_sts      : 1;
		uint64_t __reserved_2 : 6;
		uint64_t err_sts      : 2;
		uint64_t __reserved_1 : 5;
		uint64_t region       : 3;
		uint64_t __reserved_0 : 8;
		uint64_t src_tile     : 8;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_CPL_ERR_STS_t;


/*
 * Tile PIO Controls.
 * Controls for Tile PIO Transactions
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Indicates the timeout value for PIO requests (both loads
		 * and stores).  After this time expires, the request is
		 * assumed to be lost and an MMIO error will be signaled back
		 * to the requesting Tile and a PIO completion timeout
		 * interrupt will be signaled.  The timeout value is
		 * 2^(CPL_TIMER+18) cycles.  Thus if trio is operating at
		 * 950MHz, a value of 0 corresponds to approximately 0.3ms.
		 * And a value of 6 corresponds to approximately 17.7ms.
		 * Setting to all 1's disables the completion timer thus the
		 * max timer value at 950MHz is 4521.0ms.  The timer is
		 * accurate within +0/-6.6%.
		 */
		uint64_t cpl_timer                : 4;
		/*
		 * Used for software testing.  When set to 1, the PIO
		 * completion timer will be set to 1024 cycles.
		 */
		uint64_t short_timer              : 1;
		/*
		 * Disable tracking of NP TPIO credits for retries. Must be
		 * zero for normal operation.
		 */
		uint64_t np_xact_cred_hwm_disable : 1;
		/* Reserved. */
		uint64_t __reserved_0             : 2;
		/*
		 * This setting controls the number of tags available for PIO
		 * reads and configuration requests.  This must not exceed
		 * the capability of any of the attached MACs. The maximum
		 * supported value is 5.
		 */
		uint64_t tag_size                 : 3;
		/* Reserved. */
		uint64_t __reserved_1             : 1;
		/*
		 * Default link for configuration traffic if all transaction
		 * falls outside of range of all downstream bridges.
		 */
		uint64_t default_link_cfg         : 3;
		/* Reserved. */
		uint64_t __reserved_2             : 1;
		/*
		 * Default link for nonprefetchable-region traffic if all
		 * transaction falls outside of range of all downstream
		 * bridges.
		 */
		uint64_t default_link_npf         : 3;
		/* Reserved. */
		uint64_t __reserved_3             : 1;
		/*
		 * Default link for prefetchable-region traffic if all
		 * transaction falls outside of range of all downstream
		 * bridges.
		 */
		uint64_t default_link_pf          : 3;
		/* Reserved. */
		uint64_t __reserved_4             : 41;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_4             : 41;
		uint64_t default_link_pf          : 3;
		uint64_t __reserved_3             : 1;
		uint64_t default_link_npf         : 3;
		uint64_t __reserved_2             : 1;
		uint64_t default_link_cfg         : 3;
		uint64_t __reserved_1             : 1;
		uint64_t tag_size                 : 3;
		uint64_t __reserved_0             : 2;
		uint64_t np_xact_cred_hwm_disable : 1;
		uint64_t short_timer              : 1;
		uint64_t cpl_timer                : 4;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_CTL_t;


/*
 * Tile PIO status (diagnostics).
 * Diagnostics information for Tile PIO
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Contains the state of the PIO dispatch engines. */
		uint64_t pio_state    : 6;
		/* Reserved. */
		uint64_t __reserved_0 : 2;
		/*
		 * Contains the counter value for the FIFO selected by
		 * CTR_SEL.
		 */
		uint64_t ctr_val      : 5;
		/* Reserved. */
		uint64_t __reserved_1 : 3;
		/*
		 * Selects the PIO region for the CTR_VAL field and the
		 * PKT_CNT field.
		 */
		uint64_t ctr_sel      : 3;
		/* Reserved. */
		uint64_t __reserved_2 : 13;
		/*
		 * Contains a count of the packets sent for the region
		 * selected by CTR_SEL.
		 */
		uint64_t pkt_cnt      : 16;
		/* Reserved. */
		uint64_t __reserved_3 : 16;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_3 : 16;
		uint64_t pkt_cnt      : 16;
		uint64_t __reserved_2 : 13;
		uint64_t ctr_sel      : 3;
		uint64_t __reserved_1 : 3;
		uint64_t ctr_val      : 5;
		uint64_t __reserved_0 : 2;
		uint64_t pio_state    : 6;
#endif
	};

	uint64_t word;
} TRIO_TILE_PIO_DIAG_STS_t;


/*
 * Map Error Status.
 * Captured information from packet with mapping error.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* MAC from which the request arrived. */
		uint64_t src_mac      : 3;
		/* Reserved. */
		uint64_t __reserved_0 : 5;
		/* When 1, request was a write.  When 0, request was a read */
		uint64_t write        : 1;
		/* Reserved. */
		uint64_t __reserved_1 : 3;
		/*
		 * IO Address of packet that was not handled by any mapping
		 * region.
		 */
		uint64_t io_addr      : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t io_addr      : 52;
		uint64_t __reserved_1 : 3;
		uint64_t write        : 1;
		uint64_t __reserved_0 : 5;
		uint64_t src_mac      : 3;
#endif
	};

	uint64_t word;
} TRIO_MAP_ERR_STS_t;


/*
 * RX Completion Queue Queue Status.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Number of entries in CPL Queue. */
		uint64_t buf_cnt    : 6;
		/* Reserved. */
		uint64_t __reserved : 58;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 58;
		uint64_t buf_cnt    : 6;
#endif
	};

	uint64_t word;
} TRIO_RX_CPL_STS_t;


/*
 * Map Memory Write Queue Status.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Number of entries in WRQ. */
		uint64_t buf_cnt    : 6;
		/* Reserved. */
		uint64_t __reserved : 58;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 58;
		uint64_t buf_cnt    : 6;
#endif
	};

	uint64_t word;
} TRIO_MAP_WRQ_STS_t;


/*
 * Map Memory Read Queue Status.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Number of entries in each RDQ. There are 6 bits for each
		 * of the 8 links
		 */
		uint64_t buf_cnt_vec : 48;
		/* Reserved. */
		uint64_t __reserved  : 16;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved  : 16;
		uint64_t buf_cnt_vec : 48;
#endif
	};

	uint64_t word;
} TRIO_MAP_RDQ_STS_t;


/*
 * Map Memory Region Controls.
 * Global controls for map-memory regions.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * When asserted, the reads will pace their Tile memory read
		 * requests to avoid oversubscribing the response network
		 * bandwidth.
		 */
		uint64_t read_pace         : 1;
		/*
		 * Controls the read pace bandwidth.  Larger numbers result
		 * in lower bandwidth provisioned for Tile memory reads.
		 * Utilization is approximately 5/(5+N) so Setting to 3
		 * corresponds to approximately 63% utilization.  Setting to
		 * 4 corresponds to approximately 55% utilization.  This can
		 * typically be left at 3 unless the system simultaneously
		 * has high bandwidth push DMA in which case 9 is typically a
		 * more appropriate setting.
		 */
		uint64_t read_pace_bw      : 4;
		/* Reserved. */
		uint64_t __reserved_0      : 3;
		/*
		 * When written with a 1, a coherence fence is initiated.
		 * Hardware will clear the bit once all older MAP-MEM writes
		 * are visible to Tile software and that all reads have
		 * returned data to the PCIe interface.
		 */
		uint64_t fence             : 1;
		/* Reserved. */
		uint64_t __reserved_1      : 7;
		/*
		 * Indicates which map event counter should be selected (one
		 * counter is shared amongst all MAP_MEM_CTL.EVT_CTR_SEL
		 * events).  The counter is MAP_STAT_CTR
		 */
		uint64_t evt_ctr_sel       : 3;
		/* Reserved. */
		uint64_t __reserved_2      : 1;
		/*
		 * For internal diagnostics only. Disables ingress sequence
		 * number checking. Will cause reads/writes to become out of
		 * order.
		 */
		uint64_t disable_sqn_check : 1;
		/* For internal diagnostics only. */
		uint64_t current_sqn       : 11;
		/* Reserved. */
		uint64_t __reserved_3      : 32;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_3      : 32;
		uint64_t current_sqn       : 11;
		uint64_t disable_sqn_check : 1;
		uint64_t __reserved_2      : 1;
		uint64_t evt_ctr_sel       : 3;
		uint64_t __reserved_1      : 7;
		uint64_t fence             : 1;
		uint64_t __reserved_0      : 3;
		uint64_t read_pace_bw      : 4;
		uint64_t read_pace         : 1;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_CTL_t;


/*
 * Map Memory Region Read-Queue Assignment.
 * Mapping of read-queue FIFOs to links. For optimal load balancing across
 * links (to minimize burstiness), the FIFOs should be allocated to the
 * active links in an interleaved pattern. For example, if 4 links are
 * active, link-0 should be assigned FIFO0 and FIFO4, link-1 should be
 * assigned FIFO1 and FIFO5 etc. This is not required for fairness but will
 * maintain more even service across the links.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Link number associated with this FIFO */
		uint64_t fifo0_sel    : 3;
		/* Link number associated with this FIFO */
		uint64_t fifo1_sel    : 3;
		/* Link number associated with this FIFO */
		uint64_t fifo2_sel    : 3;
		/* Link number associated with this FIFO */
		uint64_t fifo3_sel    : 3;
		/* Link number associated with this FIFO */
		uint64_t fifo4_sel    : 3;
		/* Link number associated with this FIFO */
		uint64_t fifo5_sel    : 3;
		/* Link number associated with this FIFO */
		uint64_t fifo6_sel    : 3;
		/* Link number associated with this FIFO */
		uint64_t fifo7_sel    : 3;
		/* Reserved. */
		uint64_t __reserved_0 : 8;
		/*
		 * When set, the associated FIFO will not accept new incoming
		 * packets. This can be used to reassign FIFOs. Software
		 * first sets the FIFO_DISABLE bit, then issues a
		 * MAP_MEM_CTL.FENCE, then reassigns the FIFO to a different
		 * link.
		 */
		uint64_t fifo_disable : 8;
		/* Reserved. */
		uint64_t __reserved_1 : 24;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_1 : 24;
		uint64_t fifo_disable : 8;
		uint64_t __reserved_0 : 8;
		uint64_t fifo7_sel    : 3;
		uint64_t fifo6_sel    : 3;
		uint64_t fifo5_sel    : 3;
		uint64_t fifo4_sel    : 3;
		uint64_t fifo3_sel    : 3;
		uint64_t fifo2_sel    : 3;
		uint64_t fifo1_sel    : 3;
		uint64_t fifo0_sel    : 3;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_RDQ_ASSIGN_t;


/*
 * Message Queue Setup.
 * Provides configuration of the message queue which is used to capture
 * incoming messages and writes.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Enable reception of messages. */
		uint64_t ena          : 1;
		/*
		 * Number of 4KB blocks of memory to be used for ring.
		 * Encoded as log2(NUM_BLOCKS). Thus 0=4KB of storage,
		 * 15=2^^15 * 4KB=128MB
		 */
		uint64_t ring_size    : 4;
		/* Reserved. */
		uint64_t __reserved_0 : 3;
		/* Base VA of ring (bits 49:12). */
		uint64_t va           : 37;
		/* Reserved. */
		uint64_t __reserved_1 : 3;
		/* StreamID to use for ring VA translation. */
		uint64_t stream_id    : 16;
#else   /* __BIG_ENDIAN__ */
		uint64_t stream_id    : 16;
		uint64_t __reserved_1 : 3;
		uint64_t va           : 37;
		uint64_t __reserved_0 : 3;
		uint64_t ring_size    : 4;
		uint64_t ena          : 1;
#endif
	};

	uint64_t word;
} TRIO_MSG_QUEUE_SETUP_t;


/*
 * Message Queue Write Pointer.
 * Incremented by hardware whenever a new message word is written into the
 * queue. This register is not masked based on size. Software must mask the
 * value based on the MSG_QUEUE_SETUP.RING_SIZE.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved_0 : 8;
		uint64_t val          : 19;
		/* Reserved. */
		uint64_t __reserved_1 : 37;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_1 : 37;
		uint64_t val          : 19;
		uint64_t __reserved_0 : 8;
#endif
	};

	uint64_t word;
} TRIO_MSG_QUEUE_WRITE_PTR_t;


/*
 * Message Queue Read Pointer.
 * Incremented by software whenever a message word is read from the queue.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved_0 : 8;
		uint64_t val          : 19;
		/* Reserved. */
		uint64_t __reserved_1 : 37;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_1 : 37;
		uint64_t val          : 19;
		uint64_t __reserved_0 : 8;
#endif
	};

	uint64_t word;
} TRIO_MSG_QUEUE_READ_PTR_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_1_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_2_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_3_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_4_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_5_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_6_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_7_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_8_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_9_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_10_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_11_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_12_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_13_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_14_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_15_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_16_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_17_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_18_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_19_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_20_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_21_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_22_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_23_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_24_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_25_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_26_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_27_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_28_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_29_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_30_t;


/*
 * Map Memory Region Setup.
 * Configuration of the associated memory region.  There is one set of
 * SETUP/BASE/LIM registers for each of the map regions with each register
 * set consuming 32-bytes of register space (the 4th location in each block
 * of 4 registers is unused).
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the region.  When 0, accesses from the associated
		 * MAC will not be mapped to the associated region.  Multiple
		 * MACs may be assigned to the same region if they share a
		 * common global IO address space.
		 */
		uint64_t mac_ena        : 8;
		/* Ordering type for this region. */
		uint64_t order_mode     : 2;
		/* Reserved. */
		uint64_t __reserved_0   : 2;
		/* Base VA associated with this region. */
		int64_t va             : 37;
		/* Reserved. */
		uint64_t __reserved_1   : 9;
		/*
		 * When set, use CACHE_PROF for L3C cache profile selection.
		 * When clear, use the LSBs of the steering tag from the
		 * incoming request.
		 */
		uint64_t cache_prof_ovd : 1;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t cache_prof     : 4;
		/*
		 * When set, incoming writes will be sent to the message
		 * queue. Reads will target the associated VA.
		 */
		uint64_t msg_q          : 1;
#else   /* __BIG_ENDIAN__ */
		uint64_t msg_q          : 1;
		uint64_t cache_prof     : 4;
		uint64_t cache_prof_ovd : 1;
		uint64_t __reserved_1   : 9;
		int64_t va             : 37;
		uint64_t __reserved_0   : 2;
		uint64_t order_mode     : 2;
		uint64_t mac_ena        : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_SETUP_31_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_1_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_2_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_3_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_4_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_5_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_6_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_7_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_8_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_9_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_10_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_11_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_12_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_13_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_14_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_15_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_16_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_17_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_18_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_19_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_20_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_21_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_22_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_23_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_24_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_25_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_26_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_27_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_28_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_29_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_30_t;


/*
 * Map Memory Base Address.
 * Base address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Slicing is used to partition a MAP_MEM region into windows
		 * and only match within a part of each window. This leaves
		 * holes in the MAP_MEM region that may be filled by other
		 * sliced MAP_MEM regions. The SLICE_STRIDE field indicates
		 * the number of slices in a window measured in log2. For
		 * example, setting SLICE_SIZE to 4KB and SLICE_STRIDE to 2
		 * would indicate one 4KB slice for every 16KB. Likewise, if
		 * SLICE_SIZE was 16KB and SLICE_STRIDE was 3, that would
		 * indicate one 16KB slice per every 8*16KB=128KB of IO
		 * address space. In this example, the first 16KB at
		 * MAP_MEM_BASE.ADDR would match the map region, and the next
		 * seven 112KB would not match. 128KB-156KB would match again
		 * and so on.
		 *
		 * Setting SLICE_STRIDE to zero disables slicing.
		 *
		 * As an example usage, slicing may be used to capture an
		 * aperature from BAR0 within each virtual function. In this
		 * case, STRIDE and SIZE would be set such that the size of
		 * the VF is 2^(12+SLICE_STRIDE + SLICE_SIZE). A
		 * 2^(12+SLICE_SIZE) region within each VF would then match
		 * the MAP_MEM region.
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR field must
		 * point to the base of the first slice.
		 *
		 * Holes in the IO address space created by slicing are not
		 * reflected in the Tile-side address offset. Thus the 1st
		 * slice starts at MAP_MEM_SETUP.VA and the 2nd slice starts
		 * at MAP_MEM_SETUP.VA+2^(12+SLICE_SIZE).
		 *
		 * When slicing is used, the MAP_MEM_BASE.ADDR must be
		 * naturally aligned to the SLICE_SIZE. The MAP_MEM_LIM.ADDR
		 * must point to the end of the last valid slice.
		 */
		uint64_t slice_stride : 5;
		/*
		 * Indicates size of a single slice, calculated as
		 * 2^(SLICE_SIZE+12). 0 represents a 4KB slice. 1 represents
		 * an 8KB slice. 15 represents a 128MB slice.
		 */
		uint64_t slice_size   : 4;
		/* Reserved. */
		uint64_t __reserved   : 3;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address to be accepted in the region.  The
		 * Tile-side virtual address is computed as (MAP_MEM_SETUP.VA
		 * + IO_ADDRESS - MAP_MEM_BASE.ADDR).
		 */
		uint64_t addr         : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr         : 52;
		uint64_t __reserved   : 3;
		uint64_t slice_size   : 4;
		uint64_t slice_stride : 5;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_BASE_31_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_1_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_2_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_3_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_4_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_5_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_6_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_7_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_8_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_9_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_10_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_11_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_12_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_13_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_14_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_15_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_16_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_17_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_18_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_19_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_20_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_21_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_22_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_23_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_24_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_25_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_26_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_27_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_28_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_29_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_30_t;


/*
 * Map Memory Limit Address.
 * Limit address of the associated memory region.  There is one register for
 * each region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Limit page address.  An IO address must be less than or
		 * equal to this address to be accepted in the region.  The
		 * low 12 bits of the IO address are ignored for the limit
		 * check (i.e. the low 12 bits of MAP_MEM_LIM.ADDR are
		 * treated as 1's.)
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_MEM_LIM_31_t;


/*
 * MAP Stats Counter.
 * Provides count of event selected by MAP_MEM_CTL.EVT_CTR_SEL
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Contains the value of the counter being accessed by
		 * MAP_MEM_CTL.EVT_CTR_SEL.  The MAP_EVT_CTR Interrupt is
		 * asserted when value wraps.
		 */
		uint64_t val        : 32;
		/* Reserved. */
		uint64_t __reserved : 32;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 32;
		uint64_t val        : 32;
#endif
	};

	uint64_t word;
} TRIO_MAP_STAT_CTR_t;


/*
 * MAP Diag State.
 * MAP diagnostics state
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Write queue state.  Zero when idle. */
		uint64_t wrq        : 3;
		/* Read queue state.  Zero when idle. */
		uint64_t rdq        : 4;
		/* RSHIM interface state.  Zero when idle. */
		uint64_t rsh        : 4;
		/* Reserved. */
		uint64_t __reserved : 53;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 53;
		uint64_t rsh        : 4;
		uint64_t rdq        : 4;
		uint64_t wrq        : 3;
#endif
	};

	uint64_t word;
} TRIO_MAP_DIAG_FSM_STATE_t;


/*
 * Map Rshim Region Setup.
 * Configuration of the rshim region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Enable the rshim region.  When 0, accesses from the
		 * associated MAC will not be mapped to the rshim region.
		 */
		uint64_t rsh_ena      : 8;
		/* Reserved. */
		uint64_t __reserved_0 : 24;
		/*
		 * All transactions are claimed (ignore MAP_RSH_BASE). This
		 * bit is ignored if any of the normal MAP_MEM_SETUP.MAC_ENA
		 * bits are set.
		 */
		uint64_t claim_all    : 1;
		/* Reserved. */
		uint64_t __reserved_1 : 31;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_1 : 31;
		uint64_t claim_all    : 1;
		uint64_t __reserved_0 : 24;
		uint64_t rsh_ena      : 8;
#endif
	};

	uint64_t word;
} TRIO_MAP_RSH_SETUP_t;


/*
 * Rshim Region Base Address.
 * Base address of the associated memory region.  The rshim region consumes
 * 1024kB of address space.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint64_t __reserved : 12;
		/*
		 * Base page address.  An IO address must be greater than or
		 * equal to this address and less than this address plus
		 * 0x100000 to be accepted into the region.
		 */
		uint64_t addr       : 52;
#else   /* __BIG_ENDIAN__ */
		uint64_t addr       : 52;
		uint64_t __reserved : 12;
#endif
	};

	uint64_t word;
} TRIO_MAP_RSH_BASE_t;


/*
 * Rshim Region Address Format.
 * This register describes the address format for requests that target the
 * rshim region.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Full byte address within rshim register space */
		uint64_t addr       : 16;
		/* Rshim channel specifier */
		uint64_t ch         : 4;
		/* Reserved. */
		uint64_t __reserved : 44;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 44;
		uint64_t ch         : 4;
		uint64_t addr       : 16;
#endif
	};

	uint64_t word;
} TRIO_MAP_RSH_ADDR_FMT_t;


/*
 * MAC Configuration.
 * Configuration parameters for each MAC.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Maximum packet size for the associated MAC.  Must not
		 * exceed the capabilities or configuration of the MAC. 0=64,
		 * 1=128, ..., 4=1024.  Encodings larger than 4 are reserved.
		 */
		uint64_t mps          : 3;
		/* Reserved. */
		uint64_t __reserved_0 : 5;
		/*
		 * Maximum request size for the associated MAC.  Must not
		 * exceed the capabilities or configuration of the MAC. 0=64,
		 * 1=128, ..., 6=4096.  Encodings larger than 6 are reserved.
		 */
		uint64_t mrs          : 3;
		/* Reserved. */
		uint64_t __reserved_1 : 5;
		/*
		 * Number of 256-byte completion buffers reserved for this
		 * port. In order to prevent blocking between ports, the sum
		 * of all ports' CPL_BUF_RSVD fields must be less than
		 * CPL_BUF_CTL.PBUF_ENTRIES-CPL_BUF_CTL.UD_BLOCKS.
		 */
		uint64_t cpl_buf_rsvd : 7;
		/* Reserved. */
		uint64_t __reserved_2 : 41;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_2 : 41;
		uint64_t cpl_buf_rsvd : 7;
		uint64_t __reserved_1 : 5;
		uint64_t mrs          : 3;
		uint64_t __reserved_0 : 5;
		uint64_t mps          : 3;
#endif
	};

	uint64_t word;
} TRIO_MAC_CONFIG_t;


/*
 * MAC Configuration.
 * Configuration parameters for each MAC.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Maximum packet size for the associated MAC.  Must not
		 * exceed the capabilities or configuration of the MAC. 0=64,
		 * 1=128, ..., 4=1024.  Encodings larger than 4 are reserved.
		 */
		uint64_t mps          : 3;
		/* Reserved. */
		uint64_t __reserved_0 : 5;
		/*
		 * Maximum request size for the associated MAC.  Must not
		 * exceed the capabilities or configuration of the MAC. 0=64,
		 * 1=128, ..., 6=4096.  Encodings larger than 6 are reserved.
		 */
		uint64_t mrs          : 3;
		/* Reserved. */
		uint64_t __reserved_1 : 5;
		/*
		 * Number of 256-byte completion buffers reserved for this
		 * port. In order to prevent blocking between ports, the sum
		 * of all ports' CPL_BUF_RSVD fields must be less than
		 * CPL_BUF_CTL.PBUF_ENTRIES-CPL_BUF_CTL.UD_BLOCKS.
		 */
		uint64_t cpl_buf_rsvd : 7;
		/* Reserved. */
		uint64_t __reserved_2 : 41;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_2 : 41;
		uint64_t cpl_buf_rsvd : 7;
		uint64_t __reserved_1 : 5;
		uint64_t mrs          : 3;
		uint64_t __reserved_0 : 5;
		uint64_t mps          : 3;
#endif
	};

	uint64_t word;
} TRIO_MAC_CONFIG_1_t;


/*
 * MAC Configuration.
 * Configuration parameters for each MAC.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Maximum packet size for the associated MAC.  Must not
		 * exceed the capabilities or configuration of the MAC. 0=64,
		 * 1=128, ..., 4=1024.  Encodings larger than 4 are reserved.
		 */
		uint64_t mps          : 3;
		/* Reserved. */
		uint64_t __reserved_0 : 5;
		/*
		 * Maximum request size for the associated MAC.  Must not
		 * exceed the capabilities or configuration of the MAC. 0=64,
		 * 1=128, ..., 6=4096.  Encodings larger than 6 are reserved.
		 */
		uint64_t mrs          : 3;
		/* Reserved. */
		uint64_t __reserved_1 : 5;
		/*
		 * Number of 256-byte completion buffers reserved for this
		 * port. In order to prevent blocking between ports, the sum
		 * of all ports' CPL_BUF_RSVD fields must be less than
		 * CPL_BUF_CTL.PBUF_ENTRIES-CPL_BUF_CTL.UD_BLOCKS.
		 */
		uint64_t cpl_buf_rsvd : 7;
		/* Reserved. */
		uint64_t __reserved_2 : 41;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_2 : 41;
		uint64_t cpl_buf_rsvd : 7;
		uint64_t __reserved_1 : 5;
		uint64_t mrs          : 3;
		uint64_t __reserved_0 : 5;
		uint64_t mps          : 3;
#endif
	};

	uint64_t word;
} TRIO_MAC_CONFIG_2_t;


/*
 * MAC Configuration.
 * Configuration parameters for each MAC.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Maximum packet size for the associated MAC.  Must not
		 * exceed the capabilities or configuration of the MAC. 0=64,
		 * 1=128, ..., 4=1024.  Encodings larger than 4 are reserved.
		 */
		uint64_t mps          : 3;
		/* Reserved. */
		uint64_t __reserved_0 : 5;
		/*
		 * Maximum request size for the associated MAC.  Must not
		 * exceed the capabilities or configuration of the MAC. 0=64,
		 * 1=128, ..., 6=4096.  Encodings larger than 6 are reserved.
		 */
		uint64_t mrs          : 3;
		/* Reserved. */
		uint64_t __reserved_1 : 5;
		/*
		 * Number of 256-byte completion buffers reserved for this
		 * port. In order to prevent blocking between ports, the sum
		 * of all ports' CPL_BUF_RSVD fields must be less than
		 * CPL_BUF_CTL.PBUF_ENTRIES-CPL_BUF_CTL.UD_BLOCKS.
		 */
		uint64_t cpl_buf_rsvd : 7;
		/* Reserved. */
		uint64_t __reserved_2 : 41;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_2 : 41;
		uint64_t cpl_buf_rsvd : 7;
		uint64_t __reserved_1 : 5;
		uint64_t mrs          : 3;
		uint64_t __reserved_0 : 5;
		uint64_t mps          : 3;
#endif
	};

	uint64_t word;
} TRIO_MAC_CONFIG_3_t;


/*
 * MAC Configuration.
 * Configuration parameters for each MAC.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Maximum packet size for the associated MAC.  Must not
		 * exceed the capabilities or configuration of the MAC. 0=64,
		 * 1=128, ..., 4=1024.  Encodings larger than 4 are reserved.
		 */
		uint64_t mps          : 3;
		/* Reserved. */
		uint64_t __reserved_0 : 5;
		/*
		 * Maximum request size for the associated MAC.  Must not
		 * exceed the capabilities or configuration of the MAC. 0=64,
		 * 1=128, ..., 6=4096.  Encodings larger than 6 are reserved.
		 */
		uint64_t mrs          : 3;
		/* Reserved. */
		uint64_t __reserved_1 : 5;
		/*
		 * Number of 256-byte completion buffers reserved for this
		 * port. In order to prevent blocking between ports, the sum
		 * of all ports' CPL_BUF_RSVD fields must be less than
		 * CPL_BUF_CTL.PBUF_ENTRIES-CPL_BUF_CTL.UD_BLOCKS.
		 */
		uint64_t cpl_buf_rsvd : 7;
		/* Reserved. */
		uint64_t __reserved_2 : 41;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_2 : 41;
		uint64_t cpl_buf_rsvd : 7;
		uint64_t __reserved_1 : 5;
		uint64_t mrs          : 3;
		uint64_t __reserved_0 : 5;
		uint64_t mps          : 3;
#endif
	};

	uint64_t word;
} TRIO_MAC_CONFIG_4_t;


/*
 * MAC Configuration.
 * Configuration parameters for each MAC.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Maximum packet size for the associated MAC.  Must not
		 * exceed the capabilities or configuration of the MAC. 0=64,
		 * 1=128, ..., 4=1024.  Encodings larger than 4 are reserved.
		 */
		uint64_t mps          : 3;
		/* Reserved. */
		uint64_t __reserved_0 : 5;
		/*
		 * Maximum request size for the associated MAC.  Must not
		 * exceed the capabilities or configuration of the MAC. 0=64,
		 * 1=128, ..., 6=4096.  Encodings larger than 6 are reserved.
		 */
		uint64_t mrs          : 3;
		/* Reserved. */
		uint64_t __reserved_1 : 5;
		/*
		 * Number of 256-byte completion buffers reserved for this
		 * port. In order to prevent blocking between ports, the sum
		 * of all ports' CPL_BUF_RSVD fields must be less than
		 * CPL_BUF_CTL.PBUF_ENTRIES-CPL_BUF_CTL.UD_BLOCKS.
		 */
		uint64_t cpl_buf_rsvd : 7;
		/* Reserved. */
		uint64_t __reserved_2 : 41;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_2 : 41;
		uint64_t cpl_buf_rsvd : 7;
		uint64_t __reserved_1 : 5;
		uint64_t mrs          : 3;
		uint64_t __reserved_0 : 5;
		uint64_t mps          : 3;
#endif
	};

	uint64_t word;
} TRIO_MAC_CONFIG_5_t;


/*
 * MAC Configuration.
 * Configuration parameters for each MAC.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Maximum packet size for the associated MAC.  Must not
		 * exceed the capabilities or configuration of the MAC. 0=64,
		 * 1=128, ..., 4=1024.  Encodings larger than 4 are reserved.
		 */
		uint64_t mps          : 3;
		/* Reserved. */
		uint64_t __reserved_0 : 5;
		/*
		 * Maximum request size for the associated MAC.  Must not
		 * exceed the capabilities or configuration of the MAC. 0=64,
		 * 1=128, ..., 6=4096.  Encodings larger than 6 are reserved.
		 */
		uint64_t mrs          : 3;
		/* Reserved. */
		uint64_t __reserved_1 : 5;
		/*
		 * Number of 256-byte completion buffers reserved for this
		 * port. In order to prevent blocking between ports, the sum
		 * of all ports' CPL_BUF_RSVD fields must be less than
		 * CPL_BUF_CTL.PBUF_ENTRIES-CPL_BUF_CTL.UD_BLOCKS.
		 */
		uint64_t cpl_buf_rsvd : 7;
		/* Reserved. */
		uint64_t __reserved_2 : 41;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_2 : 41;
		uint64_t cpl_buf_rsvd : 7;
		uint64_t __reserved_1 : 5;
		uint64_t mrs          : 3;
		uint64_t __reserved_0 : 5;
		uint64_t mps          : 3;
#endif
	};

	uint64_t word;
} TRIO_MAC_CONFIG_6_t;


/*
 * MAC Configuration.
 * Configuration parameters for each MAC.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Maximum packet size for the associated MAC.  Must not
		 * exceed the capabilities or configuration of the MAC. 0=64,
		 * 1=128, ..., 4=1024.  Encodings larger than 4 are reserved.
		 */
		uint64_t mps          : 3;
		/* Reserved. */
		uint64_t __reserved_0 : 5;
		/*
		 * Maximum request size for the associated MAC.  Must not
		 * exceed the capabilities or configuration of the MAC. 0=64,
		 * 1=128, ..., 6=4096.  Encodings larger than 6 are reserved.
		 */
		uint64_t mrs          : 3;
		/* Reserved. */
		uint64_t __reserved_1 : 5;
		/*
		 * Number of 256-byte completion buffers reserved for this
		 * port. In order to prevent blocking between ports, the sum
		 * of all ports' CPL_BUF_RSVD fields must be less than
		 * CPL_BUF_CTL.PBUF_ENTRIES-CPL_BUF_CTL.UD_BLOCKS.
		 */
		uint64_t cpl_buf_rsvd : 7;
		/* Reserved. */
		uint64_t __reserved_2 : 41;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_2 : 41;
		uint64_t cpl_buf_rsvd : 7;
		uint64_t __reserved_1 : 5;
		uint64_t mrs          : 3;
		uint64_t __reserved_0 : 5;
		uint64_t mps          : 3;
#endif
	};

	uint64_t word;
} TRIO_MAC_CONFIG_7_t;


/*
 * Completion Buffer Control.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Number of 256-byte completion buffers that can be used by
		 * any port. In order to prevent blocking between ports, the
		 * sum of all ports' CPL_BUF_RSVD fields must be less than
		 * CPL_BUF_CTL.PBUF_ENTRIES-CPL_BUF_CTL.UD_BLOCKS.
		 */
		uint64_t ud_blocks    : 7;
		/* Reserved. */
		uint64_t __reserved_0 : 9;
		/*
		 * Number of 256-byte blocks in the completion buffer that
		 * may be shared or dedicated to individual ports via the
		 * UD_BLOCKS and CPL_BUF_RSVD registers.
		 */
		uint64_t pbuf_blocks  : 7;
		/* Reserved. */
		uint64_t __reserved_1 : 41;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_1 : 41;
		uint64_t pbuf_blocks  : 7;
		uint64_t __reserved_0 : 9;
		uint64_t ud_blocks    : 7;
#endif
	};

	uint64_t word;
} TRIO_CPL_BUF_CTL_t;


/*
 * Completion Buffer Diagnostics.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Data associated with CTR_SEL/LINK_SEL */
		uint64_t val          : 20;
		/* Reserved. */
		uint64_t __reserved_0 : 4;
		/* Data associated with SEL */
		uint64_t ctr_sel      : 3;
		/* Reserved. */
		uint64_t __reserved_1 : 5;
		uint64_t link_sel     : 3;
		/* Reserved. */
		uint64_t __reserved_2 : 5;
		/*
		 * Reduce number of outstanding reads to ARM memory system by
		 * this number of cachelines. For diagnostics only.
		 */
		uint64_t rt_hwm       : 9;
		/* Reserved. */
		uint64_t __reserved_3 : 1;
		uint64_t rt_cnt       : 10;
		/* Reserved. */
		uint64_t __reserved_4 : 4;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved_4 : 4;
		uint64_t rt_cnt       : 10;
		uint64_t __reserved_3 : 1;
		uint64_t rt_hwm       : 9;
		uint64_t __reserved_2 : 5;
		uint64_t link_sel     : 3;
		uint64_t __reserved_1 : 5;
		uint64_t ctr_sel      : 3;
		uint64_t __reserved_0 : 4;
		uint64_t val          : 20;
#endif
	};

	uint64_t word;
} TRIO_COMP_BUF_DIAG_t;


/*
 * Generic performance module configuration register in trio module.
 * Set up the configuration of generic performance module in trio module
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Write(1) or read(0) control */
		uint64_t wr_rd_b    : 1;
		/* access strobe (pulse) */
		uint64_t strobe     : 1;
		/* Configuration access address offset */
		uint64_t addr       : 3;
		/* Configuration write data */
		uint64_t wdata      : 56;
		/* Reserved. */
		uint64_t __reserved : 3;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 3;
		uint64_t wdata      : 56;
		uint64_t addr       : 3;
		uint64_t strobe     : 1;
		uint64_t wr_rd_b    : 1;
#endif
	};

	uint64_t word;
} TRIO_GEN_TRIO_PERFMON_CONFIG_t;


/*
 * Generic performance value register in trio module.
 * The register is used to provide interface where software can read the
 * value of generic performance counter in trio module
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Selected generic performance register's value */
		uint64_t val : 64;
#else   /* __BIG_ENDIAN__ */
		uint64_t val : 64;
#endif
	};

	uint64_t word;
} TRIO_GEN_TRIO_PERFMVAL_t;


/*
 * Config Op Retry.
 * Controls behavior of automatic hardware retry for config reads and writes.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * Maximum number of times hardware will retry a config op
		 * after receiving a retry response. If zero, hardware will
		 * not attempt to resend config ops.
		 */
		uint64_t limit      : 8;
		/* Reserved. */
		uint64_t __reserved : 56;
#else   /* __BIG_ENDIAN__ */
		uint64_t __reserved : 56;
		uint64_t limit      : 8;
#endif
	};

	uint64_t word;
} TRIO_CONFIG_OP_RETRY_t;

#endif /* !defined(__DOXYGEN__) */

#endif /* !defined(__ASSEMBLER__) */

#endif /* !defined(__REGS_TRIO_H__) */
