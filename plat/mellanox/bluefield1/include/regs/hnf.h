#ifndef __REGS_HNF_H__
#define __REGS_HNF_H__

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif
#include "hnf_def.h"

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
} HNF_DEV_INFO_t;


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
} HNF_DEV_CTL_t;


/* Scratchpad. */

__extension__
typedef union {
	struct {
		/* Scratchpad. */
		uint64_t scratchpad : 64;
	};

	uint64_t word;
} HNF_SCRATCHPAD_t;


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
} HNF_SEMAPHORE0_t;


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
} HNF_DMA_STATUS_t;


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
} HNF_CLOCK_COUNT_t;


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
} HNF_INT_SETUP_t;


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
} HNF_FEATURE_CTL_t;


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
} HNF_CRED_CTL_t;


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
} HNF_SAM_CTL_t;


/*
 * Scratchpad register.
 * General purpose read/write register.
 */

__extension__
typedef union {
	struct {
		/*
		 * Scratchpad register.
		 * General purpose read/write register.
		 */
		uint64_t hnf_scratchpad : 64;
	};

	uint64_t word;
} HNF_HNF_SCRATCHPAD_t;


/*
 * Array Dimensions.
 * Provides the dimensions of the array of clusters.
 */

__extension__
typedef union {
	struct {
		/* Full width of the array. */
		uint64_t x          : 8;
		/* Full height of the array. */
		uint64_t y          : 8;
		/*
		 * Number of clusters per row in the array, less than or
		 * equal to X.
		 */
		uint64_t xc         : 8;
		/*
		 * Number of clusters per column in the array, less than or
		 * equal to Y.
		 */
		uint64_t yc         : 8;
		/* Number of cores in the cluster. */
		uint64_t z          : 4;
		/* Reserved. */
		uint64_t __reserved : 28;
	};

	uint64_t word;
} HNF_HNF_DIM_t;


/*
 * Directory Configurtaion.
 * Configuration information for Directory.
 */

__extension__
typedef union {
	struct {
		/*
		 * Enable Instruction reads to use Forward State in the
		 * Directory. Used for ReadShared requests.
		 */
		uint64_t fwd_i_ctl       : 2;
		/*
		 * Enable Data reads to use Forward State in the Directory.
		 * Used for ReadShared requests.
		 */
		uint64_t fwd_d_ctl       : 2;
		/*
		 * Controls when F state lines that are evicted from L2 cache
		 * are written to memory, to be
		 *         put into L3 cache. Note that the number of sharers
		 * in the settings defined for this field is the number
		 *         left without counting the evicting L2 cache.
		 */
		uint64_t fwd_write_ctl   : 2;
		/*
		 * Enables L3 cache to pass dirty data on ReadUnique requests.
		 */
		uint64_t l3_pd_en        : 1;
		/*
		 * Enable scanning ahead in the Request Buffer when an
		 * incoming Request is blocked due to an address index match.
		 * Note: this bit is deprecated and has no effect.
		 */
		uint64_t req_sa_en       : 1;
		/*
		 * HN-F Bypass Enable. When set reads from RN can have data
		 * sent directly from SN_F to RN, bypassing the HN-F.
		 */
		uint64_t hbe             : 1;
		/* Reserved. */
		uint64_t __reserved_0    : 1;
		/*
		 * Selects how the directory index is taken from the Physical
		 * Address. Do not change the value of this field once
		 * cacheing has been enabled.
		 */
		uint64_t index           : 1;
		/*
		 * Enable address scrambling in LMR to MSS selection. Do not
		 * change the value of this field once external memory
		 * accesses have been enabled.
		 */
		uint64_t lmr_scramble_en : 1;
		/*
		 * Limit the number of concurrent snoops to each RN-F. Values
		 * of 0 and 1 will limit to 2 snoops; values greater than 8
		 * will limit to 8, other values are used as that value.
		 */
		uint64_t snp_limit       : 4;
		/*
		 * Control the prescale counter used to timeout exclusive
		 * transactions. The tracker for each exclusive transaction
		 * is protected for 8x the
		 *       prescale amount. Once that interval expires, the
		 * tracker is eligible to be replaced by a different
		 * exculsive transaction address if all trackers are
		 *       currently in use.
		 */
		uint64_t excl_ps         : 2;
		/*
		 * Controls the timeout policy on exclusive transaction
		 * trackers. 0 = initialize the timer only when the tracker
		 * is allocated,
		 *       1 = re-initialize the timer when an exclusive load
		 * or exclusive store hits an already allocated tracker (as
		 * well as when the tracker is allocated).
		 */
		uint64_t excl_to         : 1;
		/*
		 * SW can write a one to this bit to generate an interrupt.
		 * The main use of this is for testing. When this bit is 1
		 *         HNF_ERR_STATUS SW_INT bit will be set.
		 */
		uint64_t sw_int          : 1;
		/*
		 * When set, TSO writes do not wait to get data from TSO
		 * device before sending request to MSS. This bit is for
		 * Mellanox use only.
		 */
		uint64_t tso_nowait      : 1;
		/*
		 * Controls how many index collisions will happen before TSO
		 * anti-starvation arbitration is used.
		 */
		uint64_t tso_arb_limit   : 2;
		/*
		 * Controls the anti-starvation timeout for TSO writes. When
		 * the timer reaches this value TSO anti-starvation is
		 * stopped; writes are allowed to arb even if a non-TSO
		 * request did not get accepted.
		 */
		uint64_t tso_noarb_to    : 3;
		/*
		 * NOTE: This bit must be written to 1 before enabling DMA
		 * writes from PCIe devices. When set, a TSO write request to
		 * a cacheline already being written from same TSO device
		 * will wait for completion of the first write. This bit is
		 * for Mellanox use only.
		 */
		uint64_t tso_cl_wait     : 1;
		/*
		 * When set, a WriteUniquePtl, which is normally from PCIe,
		 * to a cacheline in F state will invalidate the line. When
		 * clear the line will be snooped as normal.
		 */
		uint64_t io_write_f      : 1;
		/* Reserved. */
		uint64_t __reserved_1    : 36;
	};

	uint64_t word;
} HNF_HNF_CFG_t;


/*
 * Directory Error Status.
 * Logs information about errors. All of the error bits are write-1-to-clear.
 * The fields giving information about the errors are only
 *     valid if an error bit is set. All write-1-to-clear error bits will
 * assert an interrupt.
 */

__extension__
typedef union {
	struct {
		/* ECC error detected on directory state information. */
		uint64_t state_ecc       : 1;
		/* ECC error detected on directory share information. */
		uint64_t share_ecc       : 1;
		/* ECC error detected on directory tag. */
		uint64_t tag_ecc         : 1;
		/*
		 * Indicates that another ECC error(s) occured after an error
		 * had already occurred (e.g while one or more of STATE_ECC,
		 *         SHARE_ECC, or TAG_ECC bits was already set). The
		 * information about the error in ECC_TYPE and ECC_INDEX
		 * depends on error type;
		 *         for multiple correctable errors it applies to the
		 * most recent error, for an uncorrectable error it applies
		 * to the first
		 *         uncorrectable error (e.g. a an uncorrectable error
		 * will overwrite a correctable error, but not the other way).
		 */
		uint64_t ecc_multiple    : 1;
		/*
		 * The type of ECC error detected, 0 = correctable, 1 =
		 * uncorrectable.
		 */
		uint64_t ecc_type        : 1;
		/* Reserved. */
		uint64_t __reserved_0    : 3;
		/* Directory index of ECC error. */
		uint64_t ecc_index       : 10;
		/* Reserved. */
		uint64_t __reserved_1    : 2;
		/*
		 * DAT flit was received with Data Error Response.
		 * RESP_ERR_NODEID has the NodeID of the RN that sent the flit
		 */
		uint64_t rxdat_derr      : 1;
		/*
		 * DAT flit was received with Non-data Error Response.
		 * RESP_ERR_NODEID has the NodeID of the RN that sent the
		 * flit.
		 */
		uint64_t rxdat_nderr     : 1;
		/*
		 * RSP flit was received with Data Error Response.
		 * RESP_ERR_NODEID has the NodeID of the RN that sent the
		 * flit.
		 */
		uint64_t rxrsp_derr      : 1;
		/*
		 * RSP flit was received with Non-data Error Response.
		 * RESP_ERR_NODEID has the NodeID of the RN that sent the
		 * flit.
		 */
		uint64_t rxrsp_nderr     : 1;
		/*
		 * A DAT or RSP flit with Data Error or Non-data Error was
		 * received when any of RXDAT_DERR, RXDAT_NDERR, RXRSP_DERR,
		 * or RXRSP_NDERR
		 *         bits were already set.
		 */
		uint64_t rx_err_multiple : 1;
		/* Reserved. */
		uint64_t __reserved_2    : 7;
		/*
		 * NodeID of the RN that sent DAT or RSP flit with RESPERR of
		 * Data Error or Non-data Error. In event of multiple errors,
		 * indicated
		 *       by RX_ERR_MULITPLE, this field indicates the first
		 * error.
		 */
		uint64_t resp_err_nodeid : 7;
		/* Reserved. */
		uint64_t __reserved_3    : 1;
		/* Software has written a 1 into HNF_CFG SW_INT bit. */
		uint64_t sw_int          : 1;
		/*
		 * A Request with a Cache Maintainance Opcode was received
		 * for an address in MMIO range. CMO_ERR_ID has the {linear
		 *         Tile number, LPID} of the core that sent the
		 * request.
		 */
		uint64_t cmo_err         : 1;
		/*
		 * Linear Tile number and LPID of the core that a CMO request
		 * that set CMO_ERR bit. In event of multiple errors, this
		 * field indicates the first error.
		 */
		uint64_t cmo_err_id      : 7;
		/*
		 * A Request for a cacheable read was received for an address
		 * in MMIO range. MMIO_RD_ERR_ID has the {linear
		 *         Tile number, LPID} of the core that sent the
		 * request.
		 */
		uint64_t mmio_rd_err     : 1;
		/*
		 * Linear Tile number and LPID of the core that a request
		 * that set MMIO_RD_ERR bit. In event of multiple errors,
		 * this field indicates the first error.
		 */
		uint64_t mmio_rd_err_id  : 7;
		/* Reserved. */
		uint64_t __reserved_4    : 7;
	};

	uint64_t word;
} HNF_HNF_ERR_STATUS_t;


/*
 * Directory Performance Monitor Control.
 * Controls use of perfromance monitoring in Directory.
 */

__extension__
typedef union {
	struct {
		/* Reserved. */
		uint64_t __reserved_0 : 8;
		/*
		 * Selects the MAF, for types of events that are MAF-related.
		 */
		uint64_t maf_sel      : 7;
		/* Reserved. */
		uint64_t __reserved_1 : 1;
		/*
		 * Selects the RN-F, for types of events that are
		 * RN-F-related.
		 */
		uint64_t rnf_sel      : 7;
		/* Reserved. */
		uint64_t __reserved_2 : 1;
		/*
		 * Selects the REQ Opcode, for types of events that are
		 * opcode-related.
		 */
		uint64_t opcode_sel   : 5;
		/* Reserved. */
		uint64_t __reserved_3 : 35;
	};

	uint64_t word;
} HNF_HNF_PERF_CTL_t;


/*
 * Memory Subsystem Node IDs.
 * Specifies the locations (e.g. the Node IDs) of the Memory Controllers.
 */

__extension__
typedef union {
	struct {
		/* Node ID for MSS 0. */
		uint64_t mss_0        : 7;
		/* Reserved. */
		uint64_t __reserved_0 : 1;
		/* Node ID for MSS 1. */
		uint64_t mss_1        : 7;
		/* Reserved. */
		uint64_t __reserved_1 : 1;
		/* Node ID for MSS 2. */
		uint64_t mss_2        : 7;
		/* Reserved. */
		uint64_t __reserved_2 : 1;
		/* Node ID for MSS 3. */
		uint64_t mss_3        : 7;
		/* Reserved. */
		uint64_t __reserved_3 : 33;
	};

	uint64_t word;
} HNF_HNF_MSS_NODEID_t;


/*
 * Memory Subsystem DIMM Type.
 * Specifies the type of DIMM for each of the Memory Controllers. 0 = not
 * NVDIMM, 1 = NVDIMM.
 */

__extension__
typedef union {
	struct {
		/* DIMM Type for MSS 7 through 0. */
		uint64_t type       : 8;
		/* Reserved. */
		uint64_t __reserved : 56;
	};

	uint64_t word;
} HNF_HNF_PMR_TYPE_t;


/*
 * Physical Memory Region Thresholds.
 * Physical Address thresholds used to select 8 Physical Address Regions.
 * Region n is selected for a PA when PMR_THRESHOLD_n <= PA <
 * PMR_THRESHOLD_n+1. Note that if PMR_THRESHOLD_n == PMR_THRESHOLD_n+1 then
 * region n is not used. PMRs 0-3 are used for defining up to 4 different DDR
 * memory regions, and PMRs 4-7 are used for defining up to 4 different.
 */

__extension__
typedef union {
	struct {
		/* Reserved. */
		uint64_t __reserved_0 : 30;
		/*
		 * Start PA of Physical Memory Region n. All regions are
		 * aligned to 1 GB.
		 */
		uint64_t pa           : 10;
		/* Reserved. */
		uint64_t __reserved_1 : 24;
	};

	uint64_t word;
} HNF_HNF_PMR_THRESHOLD_t;


/*
 * Physical Memory Region Thresholds.
 * Physical Address thresholds used to select 8 Physical Address Regions.
 * Region n is selected for a PA when PMR_THRESHOLD_n <= PA <
 * PMR_THRESHOLD_n+1. Note that if PMR_THRESHOLD_n == PMR_THRESHOLD_n+1 then
 * region n is not used. PMRs 0-3 are used for defining up to 4 different DDR
 * memory regions, and PMRs 4-7 are used for defining up to 4 different.
 */

__extension__
typedef union {
	struct {
		/* Reserved. */
		uint64_t __reserved_0 : 30;
		/*
		 * Start PA of Physical Memory Region n. All regions are
		 * aligned to 1 GB.
		 */
		uint64_t pa           : 10;
		/* Reserved. */
		uint64_t __reserved_1 : 24;
	};

	uint64_t word;
} HNF_HNF_PMR_THRESHOLD_1_t;


/*
 * Physical Memory Region Thresholds.
 * Physical Address thresholds used to select 8 Physical Address Regions.
 * Region n is selected for a PA when PMR_THRESHOLD_n <= PA <
 * PMR_THRESHOLD_n+1. Note that if PMR_THRESHOLD_n == PMR_THRESHOLD_n+1 then
 * region n is not used. PMRs 0-3 are used for defining up to 4 different DDR
 * memory regions, and PMRs 4-7 are used for defining up to 4 different.
 */

__extension__
typedef union {
	struct {
		/* Reserved. */
		uint64_t __reserved_0 : 30;
		/*
		 * Start PA of Physical Memory Region n. All regions are
		 * aligned to 1 GB.
		 */
		uint64_t pa           : 10;
		/* Reserved. */
		uint64_t __reserved_1 : 24;
	};

	uint64_t word;
} HNF_HNF_PMR_THRESHOLD_2_t;


/*
 * Physical Memory Region Thresholds.
 * Physical Address thresholds used to select 8 Physical Address Regions.
 * Region n is selected for a PA when PMR_THRESHOLD_n <= PA <
 * PMR_THRESHOLD_n+1. Note that if PMR_THRESHOLD_n == PMR_THRESHOLD_n+1 then
 * region n is not used. PMRs 0-3 are used for defining up to 4 different DDR
 * memory regions, and PMRs 4-7 are used for defining up to 4 different.
 */

__extension__
typedef union {
	struct {
		/* Reserved. */
		uint64_t __reserved_0 : 30;
		/*
		 * Start PA of Physical Memory Region n. All regions are
		 * aligned to 1 GB.
		 */
		uint64_t pa           : 10;
		/* Reserved. */
		uint64_t __reserved_1 : 24;
	};

	uint64_t word;
} HNF_HNF_PMR_THRESHOLD_3_t;


/*
 * Physical Memory Region Thresholds.
 * Physical Address thresholds used to select 8 Physical Address Regions.
 * Region n is selected for a PA when PMR_THRESHOLD_n <= PA <
 * PMR_THRESHOLD_n+1. Note that if PMR_THRESHOLD_n == PMR_THRESHOLD_n+1 then
 * region n is not used. PMRs 0-3 are used for defining up to 4 different DDR
 * memory regions, and PMRs 4-7 are used for defining up to 4 different.
 */

__extension__
typedef union {
	struct {
		/* Reserved. */
		uint64_t __reserved_0 : 30;
		/*
		 * Start PA of Physical Memory Region n. All regions are
		 * aligned to 1 GB.
		 */
		uint64_t pa           : 10;
		/* Reserved. */
		uint64_t __reserved_1 : 24;
	};

	uint64_t word;
} HNF_HNF_PMR_THRESHOLD_4_t;


/*
 * Physical Memory Region Thresholds.
 * Physical Address thresholds used to select 8 Physical Address Regions.
 * Region n is selected for a PA when PMR_THRESHOLD_n <= PA <
 * PMR_THRESHOLD_n+1. Note that if PMR_THRESHOLD_n == PMR_THRESHOLD_n+1 then
 * region n is not used. PMRs 0-3 are used for defining up to 4 different DDR
 * memory regions, and PMRs 4-7 are used for defining up to 4 different.
 */

__extension__
typedef union {
	struct {
		/* Reserved. */
		uint64_t __reserved_0 : 30;
		/*
		 * Start PA of Physical Memory Region n. All regions are
		 * aligned to 1 GB.
		 */
		uint64_t pa           : 10;
		/* Reserved. */
		uint64_t __reserved_1 : 24;
	};

	uint64_t word;
} HNF_HNF_PMR_THRESHOLD_5_t;


/*
 * Physical Memory Region Thresholds.
 * Physical Address thresholds used to select 8 Physical Address Regions.
 * Region n is selected for a PA when PMR_THRESHOLD_n <= PA <
 * PMR_THRESHOLD_n+1. Note that if PMR_THRESHOLD_n == PMR_THRESHOLD_n+1 then
 * region n is not used. PMRs 0-3 are used for defining up to 4 different DDR
 * memory regions, and PMRs 4-7 are used for defining up to 4 different.
 */

__extension__
typedef union {
	struct {
		/* Reserved. */
		uint64_t __reserved_0 : 30;
		/*
		 * Start PA of Physical Memory Region n. All regions are
		 * aligned to 1 GB.
		 */
		uint64_t pa           : 10;
		/* Reserved. */
		uint64_t __reserved_1 : 24;
	};

	uint64_t word;
} HNF_HNF_PMR_THRESHOLD_6_t;


/*
 * Physical Memory Region Thresholds.
 * Physical Address thresholds used to select 8 Physical Address Regions.
 * Region n is selected for a PA when PMR_THRESHOLD_n <= PA <
 * PMR_THRESHOLD_n+1. Note that if PMR_THRESHOLD_n == PMR_THRESHOLD_n+1 then
 * region n is not used. PMRs 0-3 are used for defining up to 4 different DDR
 * memory regions, and PMRs 4-7 are used for defining up to 4 different.
 */

__extension__
typedef union {
	struct {
		/* Reserved. */
		uint64_t __reserved_0 : 30;
		/*
		 * Start PA of Physical Memory Region n. All regions are
		 * aligned to 1 GB.
		 */
		uint64_t pa           : 10;
		/* Reserved. */
		uint64_t __reserved_1 : 24;
	};

	uint64_t word;
} HNF_HNF_PMR_THRESHOLD_7_t;


/*
 * Logical Memory Region to MSS ID.
 * Each Register is a table for one of the Physical Memory Regions (register
 * n for PMR n). The values stored in the table are the MSS ID for each
 * Logical Memory Region. The LMR is selected by the memory striping function.
 */

__extension__
typedef union {
	struct {
		/* MSS_ID for LMR_0. */
		uint64_t mss_0        : 2;
		/* Reserved. */
		uint64_t __reserved_0 : 2;
		/* MSS_ID for LMR_1. */
		uint64_t mss_1        : 2;
		/* Reserved. */
		uint64_t __reserved_1 : 2;
		/* MSS_ID for LMR_2. */
		uint64_t mss_2        : 2;
		/* Reserved. */
		uint64_t __reserved_2 : 2;
		/* MSS_ID for LMR_3. */
		uint64_t mss_3        : 2;
		/* Reserved. */
		uint64_t __reserved_3 : 2;
		/* MSS_ID for LMR_4. */
		uint64_t mss_4        : 2;
		/* Reserved. */
		uint64_t __reserved_4 : 2;
		/* MSS_ID for LMR_5. */
		uint64_t mss_5        : 2;
		/* Reserved. */
		uint64_t __reserved_5 : 2;
		/* MSS_ID for LMR_6. */
		uint64_t mss_6        : 2;
		/* Reserved. */
		uint64_t __reserved_6 : 2;
		/* MSS_ID for LMR_7. */
		uint64_t mss_7        : 2;
		/* Reserved. */
		uint64_t __reserved_7 : 34;
	};

	uint64_t word;
} HNF_HNF_MSS_TABLE_t;


/*
 * Logical Memory Region to MSS ID.
 * Each Register is a table for one of the Physical Memory Regions (register
 * n for PMR n). The values stored in the table are the MSS ID for each
 * Logical Memory Region. The LMR is selected by the memory striping function.
 */

__extension__
typedef union {
	struct {
		/* MSS_ID for LMR_0. */
		uint64_t mss_0        : 2;
		/* Reserved. */
		uint64_t __reserved_0 : 2;
		/* MSS_ID for LMR_1. */
		uint64_t mss_1        : 2;
		/* Reserved. */
		uint64_t __reserved_1 : 2;
		/* MSS_ID for LMR_2. */
		uint64_t mss_2        : 2;
		/* Reserved. */
		uint64_t __reserved_2 : 2;
		/* MSS_ID for LMR_3. */
		uint64_t mss_3        : 2;
		/* Reserved. */
		uint64_t __reserved_3 : 2;
		/* MSS_ID for LMR_4. */
		uint64_t mss_4        : 2;
		/* Reserved. */
		uint64_t __reserved_4 : 2;
		/* MSS_ID for LMR_5. */
		uint64_t mss_5        : 2;
		/* Reserved. */
		uint64_t __reserved_5 : 2;
		/* MSS_ID for LMR_6. */
		uint64_t mss_6        : 2;
		/* Reserved. */
		uint64_t __reserved_6 : 2;
		/* MSS_ID for LMR_7. */
		uint64_t mss_7        : 2;
		/* Reserved. */
		uint64_t __reserved_7 : 34;
	};

	uint64_t word;
} HNF_HNF_MSS_TABLE_1_t;


/*
 * Logical Memory Region to MSS ID.
 * Each Register is a table for one of the Physical Memory Regions (register
 * n for PMR n). The values stored in the table are the MSS ID for each
 * Logical Memory Region. The LMR is selected by the memory striping function.
 */

__extension__
typedef union {
	struct {
		/* MSS_ID for LMR_0. */
		uint64_t mss_0        : 2;
		/* Reserved. */
		uint64_t __reserved_0 : 2;
		/* MSS_ID for LMR_1. */
		uint64_t mss_1        : 2;
		/* Reserved. */
		uint64_t __reserved_1 : 2;
		/* MSS_ID for LMR_2. */
		uint64_t mss_2        : 2;
		/* Reserved. */
		uint64_t __reserved_2 : 2;
		/* MSS_ID for LMR_3. */
		uint64_t mss_3        : 2;
		/* Reserved. */
		uint64_t __reserved_3 : 2;
		/* MSS_ID for LMR_4. */
		uint64_t mss_4        : 2;
		/* Reserved. */
		uint64_t __reserved_4 : 2;
		/* MSS_ID for LMR_5. */
		uint64_t mss_5        : 2;
		/* Reserved. */
		uint64_t __reserved_5 : 2;
		/* MSS_ID for LMR_6. */
		uint64_t mss_6        : 2;
		/* Reserved. */
		uint64_t __reserved_6 : 2;
		/* MSS_ID for LMR_7. */
		uint64_t mss_7        : 2;
		/* Reserved. */
		uint64_t __reserved_7 : 34;
	};

	uint64_t word;
} HNF_HNF_MSS_TABLE_2_t;


/*
 * Logical Memory Region to MSS ID.
 * Each Register is a table for one of the Physical Memory Regions (register
 * n for PMR n). The values stored in the table are the MSS ID for each
 * Logical Memory Region. The LMR is selected by the memory striping function.
 */

__extension__
typedef union {
	struct {
		/* MSS_ID for LMR_0. */
		uint64_t mss_0        : 2;
		/* Reserved. */
		uint64_t __reserved_0 : 2;
		/* MSS_ID for LMR_1. */
		uint64_t mss_1        : 2;
		/* Reserved. */
		uint64_t __reserved_1 : 2;
		/* MSS_ID for LMR_2. */
		uint64_t mss_2        : 2;
		/* Reserved. */
		uint64_t __reserved_2 : 2;
		/* MSS_ID for LMR_3. */
		uint64_t mss_3        : 2;
		/* Reserved. */
		uint64_t __reserved_3 : 2;
		/* MSS_ID for LMR_4. */
		uint64_t mss_4        : 2;
		/* Reserved. */
		uint64_t __reserved_4 : 2;
		/* MSS_ID for LMR_5. */
		uint64_t mss_5        : 2;
		/* Reserved. */
		uint64_t __reserved_5 : 2;
		/* MSS_ID for LMR_6. */
		uint64_t mss_6        : 2;
		/* Reserved. */
		uint64_t __reserved_6 : 2;
		/* MSS_ID for LMR_7. */
		uint64_t mss_7        : 2;
		/* Reserved. */
		uint64_t __reserved_7 : 34;
	};

	uint64_t word;
} HNF_HNF_MSS_TABLE_3_t;


/*
 * Logical Memory Region to MSS ID.
 * Each Register is a table for one of the Physical Memory Regions (register
 * n for PMR n). The values stored in the table are the MSS ID for each
 * Logical Memory Region. The LMR is selected by the memory striping function.
 */

__extension__
typedef union {
	struct {
		/* MSS_ID for LMR_0. */
		uint64_t mss_0        : 2;
		/* Reserved. */
		uint64_t __reserved_0 : 2;
		/* MSS_ID for LMR_1. */
		uint64_t mss_1        : 2;
		/* Reserved. */
		uint64_t __reserved_1 : 2;
		/* MSS_ID for LMR_2. */
		uint64_t mss_2        : 2;
		/* Reserved. */
		uint64_t __reserved_2 : 2;
		/* MSS_ID for LMR_3. */
		uint64_t mss_3        : 2;
		/* Reserved. */
		uint64_t __reserved_3 : 2;
		/* MSS_ID for LMR_4. */
		uint64_t mss_4        : 2;
		/* Reserved. */
		uint64_t __reserved_4 : 2;
		/* MSS_ID for LMR_5. */
		uint64_t mss_5        : 2;
		/* Reserved. */
		uint64_t __reserved_5 : 2;
		/* MSS_ID for LMR_6. */
		uint64_t mss_6        : 2;
		/* Reserved. */
		uint64_t __reserved_6 : 2;
		/* MSS_ID for LMR_7. */
		uint64_t mss_7        : 2;
		/* Reserved. */
		uint64_t __reserved_7 : 34;
	};

	uint64_t word;
} HNF_HNF_MSS_TABLE_4_t;


/*
 * Logical Memory Region to MSS ID.
 * Each Register is a table for one of the Physical Memory Regions (register
 * n for PMR n). The values stored in the table are the MSS ID for each
 * Logical Memory Region. The LMR is selected by the memory striping function.
 */

__extension__
typedef union {
	struct {
		/* MSS_ID for LMR_0. */
		uint64_t mss_0        : 2;
		/* Reserved. */
		uint64_t __reserved_0 : 2;
		/* MSS_ID for LMR_1. */
		uint64_t mss_1        : 2;
		/* Reserved. */
		uint64_t __reserved_1 : 2;
		/* MSS_ID for LMR_2. */
		uint64_t mss_2        : 2;
		/* Reserved. */
		uint64_t __reserved_2 : 2;
		/* MSS_ID for LMR_3. */
		uint64_t mss_3        : 2;
		/* Reserved. */
		uint64_t __reserved_3 : 2;
		/* MSS_ID for LMR_4. */
		uint64_t mss_4        : 2;
		/* Reserved. */
		uint64_t __reserved_4 : 2;
		/* MSS_ID for LMR_5. */
		uint64_t mss_5        : 2;
		/* Reserved. */
		uint64_t __reserved_5 : 2;
		/* MSS_ID for LMR_6. */
		uint64_t mss_6        : 2;
		/* Reserved. */
		uint64_t __reserved_6 : 2;
		/* MSS_ID for LMR_7. */
		uint64_t mss_7        : 2;
		/* Reserved. */
		uint64_t __reserved_7 : 34;
	};

	uint64_t word;
} HNF_HNF_MSS_TABLE_5_t;


/*
 * Logical Memory Region to MSS ID.
 * Each Register is a table for one of the Physical Memory Regions (register
 * n for PMR n). The values stored in the table are the MSS ID for each
 * Logical Memory Region. The LMR is selected by the memory striping function.
 */

__extension__
typedef union {
	struct {
		/* MSS_ID for LMR_0. */
		uint64_t mss_0        : 2;
		/* Reserved. */
		uint64_t __reserved_0 : 2;
		/* MSS_ID for LMR_1. */
		uint64_t mss_1        : 2;
		/* Reserved. */
		uint64_t __reserved_1 : 2;
		/* MSS_ID for LMR_2. */
		uint64_t mss_2        : 2;
		/* Reserved. */
		uint64_t __reserved_2 : 2;
		/* MSS_ID for LMR_3. */
		uint64_t mss_3        : 2;
		/* Reserved. */
		uint64_t __reserved_3 : 2;
		/* MSS_ID for LMR_4. */
		uint64_t mss_4        : 2;
		/* Reserved. */
		uint64_t __reserved_4 : 2;
		/* MSS_ID for LMR_5. */
		uint64_t mss_5        : 2;
		/* Reserved. */
		uint64_t __reserved_5 : 2;
		/* MSS_ID for LMR_6. */
		uint64_t mss_6        : 2;
		/* Reserved. */
		uint64_t __reserved_6 : 2;
		/* MSS_ID for LMR_7. */
		uint64_t mss_7        : 2;
		/* Reserved. */
		uint64_t __reserved_7 : 34;
	};

	uint64_t word;
} HNF_HNF_MSS_TABLE_6_t;


/*
 * Logical Memory Region to MSS ID.
 * Each Register is a table for one of the Physical Memory Regions (register
 * n for PMR n). The values stored in the table are the MSS ID for each
 * Logical Memory Region. The LMR is selected by the memory striping function.
 */

__extension__
typedef union {
	struct {
		/* MSS_ID for LMR_0. */
		uint64_t mss_0        : 2;
		/* Reserved. */
		uint64_t __reserved_0 : 2;
		/* MSS_ID for LMR_1. */
		uint64_t mss_1        : 2;
		/* Reserved. */
		uint64_t __reserved_1 : 2;
		/* MSS_ID for LMR_2. */
		uint64_t mss_2        : 2;
		/* Reserved. */
		uint64_t __reserved_2 : 2;
		/* MSS_ID for LMR_3. */
		uint64_t mss_3        : 2;
		/* Reserved. */
		uint64_t __reserved_3 : 2;
		/* MSS_ID for LMR_4. */
		uint64_t mss_4        : 2;
		/* Reserved. */
		uint64_t __reserved_4 : 2;
		/* MSS_ID for LMR_5. */
		uint64_t mss_5        : 2;
		/* Reserved. */
		uint64_t __reserved_5 : 2;
		/* MSS_ID for LMR_6. */
		uint64_t mss_6        : 2;
		/* Reserved. */
		uint64_t __reserved_6 : 2;
		/* MSS_ID for LMR_7. */
		uint64_t mss_7        : 2;
		/* Reserved. */
		uint64_t __reserved_7 : 34;
	};

	uint64_t word;
} HNF_HNF_MSS_TABLE_7_t;


/*
 * Credit Control.
 * Provides access to the request-credit counters that control end-to-end
 * flow control between the
 *       device and other nodes in the system.
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
} HNF_HNF_CRED_CTL_t;


/*
 * L3 Cache Read Profile.
 * Select the L3 cache profile for A72 reads. The profile used is selected by
 *       the concatenation the following information. 1) PMR region: NVDIMM
 * (1) or not NVDIMM (0),
 *       2) Read type: Istream (1) or Dstream (0). The contatenation selects
 * one of four policies.
 */

__extension__
typedef union {
	struct {
		/* L3 Profile. */
		uint64_t p0         : 4;
		/* L3 Profile. */
		uint64_t p1         : 4;
		/* L3 Profile. */
		uint64_t p2         : 4;
		/* L3 Profile. */
		uint64_t p3         : 4;
		/* Reserved. */
		uint64_t __reserved : 48;
	};

	uint64_t word;
} HNF_HNF_L3_READ_PROFILE_t;


/*
 * L3 Cache Write Profile.
 * Select the L3 cache profile for A72 writes to regular DRAM (not NVDIMM.
 * NVDIMM write profile is configured in HNF_L3_WRITE_PROFILE_B).
 *       The profile used is selected by the concatenation the following
 * information. 1) Write type CopyBack (00), WriteUnique (01), CMO (10),
 * Directory_Victim (11),
 *       2) Number of Tiles sharing the data: None (00), One (01), Two (10),
 * More than Two (11).
 */

__extension__
typedef union {
	struct {
		/* L3 Profile. */
		uint64_t p0  : 4;
		/* L3 Profile. */
		uint64_t p1  : 4;
		/* L3 Profile. */
		uint64_t p2  : 4;
		/* L3 Profile. */
		uint64_t p3  : 4;
		/* L3 Profile. */
		uint64_t p4  : 4;
		/* L3 Profile. */
		uint64_t p5  : 4;
		/* L3 Profile. */
		uint64_t p6  : 4;
		/* L3 Profile. */
		uint64_t p7  : 4;
		/* L3 Profile. */
		uint64_t p8  : 4;
		/* L3 Profile. */
		uint64_t p9  : 4;
		/* L3 Profile. */
		uint64_t p10 : 4;
		/* L3 Profile. */
		uint64_t p11 : 4;
		/* L3 Profile. */
		uint64_t p12 : 4;
		/* L3 Profile. */
		uint64_t p13 : 4;
		/* L3 Profile. */
		uint64_t p14 : 4;
		/* L3 Profile. */
		uint64_t p15 : 4;
	};

	uint64_t word;
} HNF_HNF_L3_WRITE_PROFILE_A_t;


/*
 * L3 Cache Write Profile.
 * Select the L3 cache profile for A72 writes to NVDIMM DRAM (regular DRAM
 * write profile is configured in HNF_L3_WRITE_PROFILE_A).
 *       The profile used is selected by the concatenation of the same
 * information used for HNF_L3_WRITE_PROFILE_A.
 */

__extension__
typedef union {
	struct {
		/* L3 Profile. */
		uint64_t p0  : 4;
		/* L3 Profile. */
		uint64_t p1  : 4;
		/* L3 Profile. */
		uint64_t p2  : 4;
		/* L3 Profile. */
		uint64_t p3  : 4;
		/* L3 Profile. */
		uint64_t p4  : 4;
		/* L3 Profile. */
		uint64_t p5  : 4;
		/* L3 Profile. */
		uint64_t p6  : 4;
		/* L3 Profile. */
		uint64_t p7  : 4;
		/* L3 Profile. */
		uint64_t p8  : 4;
		/* L3 Profile. */
		uint64_t p9  : 4;
		/* L3 Profile. */
		uint64_t p10 : 4;
		/* L3 Profile. */
		uint64_t p11 : 4;
		/* L3 Profile. */
		uint64_t p12 : 4;
		/* L3 Profile. */
		uint64_t p13 : 4;
		/* L3 Profile. */
		uint64_t p14 : 4;
		/* L3 Profile. */
		uint64_t p15 : 4;
	};

	uint64_t word;
} HNF_HNF_L3_WRITE_PROFILE_B_t;


/*
 * Directory Debug Control.
 * Control for MAF Debug
 */

__extension__
typedef union {
	struct {
		/*
		 * Selects which MAF information is selected in MAF_DBG
		 * Registers.
		 */
		uint64_t maf        : 7;
		/*
		 * Selects which fields of Request information is read in
		 * HNF_DBG_3 Register.
		 */
		uint64_t sel        : 2;
		/* Enables debug information. */
		uint64_t enable     : 1;
		/* Reserved. */
		uint64_t __reserved : 54;
	};

	uint64_t word;
} HNF_HNF_DBG_CTL_t;


/*
 * Debug Information, Part 1.
 * Debug Information
 */

__extension__
typedef union {
	struct {
		/* Debug Information. */
		uint64_t data       : 58;
		/* Reserved. */
		uint64_t __reserved : 6;
	};

	uint64_t word;
} HNF_HNF_DBG_1_t;


/*
 * Debug Information, Part 2.
 * Debug Information
 */

__extension__
typedef union {
	struct {
		/* Debug Information. */
		uint64_t data       : 52;
		/* Reserved. */
		uint64_t __reserved : 12;
	};

	uint64_t word;
} HNF_HNF_DBG_2_t;


/*
 * Debug Information, Part 3.
 * Debug Information, selected by SEL field
 */

__extension__
typedef union {
	struct {
		/* Debug Information. */
		uint64_t data : 64;
	};

	uint64_t word;
} HNF_HNF_DBG_3_t;


/*
 * Debug Information, Part 4.
 * Debug Information
 */

__extension__
typedef union {
	struct {
		/* Debug Information. */
		uint64_t data : 64;
	};

	uint64_t word;
} HNF_HNF_DBG_4_t;


/*
 * Debug Information, Part 5.
 * Debug Information
 */

__extension__
typedef union {
	struct {
		/* Debug Information. */
		uint64_t data : 64;
	};

	uint64_t word;
} HNF_HNF_DBG_5_t;


/*
 * Debug Information, Part 6.
 * Debug Information
 */

__extension__
typedef union {
	struct {
		/* Debug Information. */
		uint64_t data : 64;
	};

	uint64_t word;
} HNF_HNF_DBG_6_t;


/*
 * Generic performance module configuration register in hnf.
 * Set up the configuration of generic performance module in hnf
 */

__extension__
typedef union {
	struct {
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
	};

	uint64_t word;
} HNF_GEN_HNF_PERFMON_CONFIG_t;


/*
 * Generic performance module configuration register in hnf.
 * Set up the configuration of generic performance module in hnf
 */

__extension__
typedef union {
	struct {
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
	};

	uint64_t word;
} HNF_GEN_HNF_PERFMON_CONFIG_1_t;


/*
 * Generic performance module configuration register in hnf.
 * Set up the configuration of generic performance module in hnf
 */

__extension__
typedef union {
	struct {
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
	};

	uint64_t word;
} HNF_GEN_HNF_PERFMON_CONFIG_2_t;


/*
 * Generic performance module configuration register in hnf.
 * Set up the configuration of generic performance module in hnf
 */

__extension__
typedef union {
	struct {
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
	};

	uint64_t word;
} HNF_GEN_HNF_PERFMON_CONFIG_3_t;


/*
 * Generic performance value register in hnf.
 * The register is used to provide interface where software can read the
 * value of generic performance counter in hnf
 */

__extension__
typedef union {
	struct {
		/* Selected generic performance register's value */
		uint64_t val : 64;
	};

	uint64_t word;
} HNF_GEN_HNF_PERFMVAL_t;


/*
 * Generic performance value register in hnf.
 * The register is used to provide interface where software can read the
 * value of generic performance counter in hnf
 */

__extension__
typedef union {
	struct {
		/* Selected generic performance register's value */
		uint64_t val : 64;
	};

	uint64_t word;
} HNF_GEN_HNF_PERFMVAL_1_t;


/*
 * Generic performance value register in hnf.
 * The register is used to provide interface where software can read the
 * value of generic performance counter in hnf
 */

__extension__
typedef union {
	struct {
		/* Selected generic performance register's value */
		uint64_t val : 64;
	};

	uint64_t word;
} HNF_GEN_HNF_PERFMVAL_2_t;


/*
 * Generic performance value register in hnf.
 * The register is used to provide interface where software can read the
 * value of generic performance counter in hnf
 */

__extension__
typedef union {
	struct {
		/* Selected generic performance register's value */
		uint64_t val : 64;
	};

	uint64_t word;
} HNF_GEN_HNF_PERFMVAL_3_t;


/*
 * Generic performance module configuration register in core clock domain.
 * Set up the configuration of generic performance module in core clock domain
 */

__extension__
typedef union {
	struct {
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
	};

	uint64_t word;
} HNF_GEN_CCLK_PERFMON_CONFIG_t;


/*
 * Generic performance module configuration register in core clock domain.
 * Set up the configuration of generic performance module in core clock domain
 */

__extension__
typedef union {
	struct {
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
	};

	uint64_t word;
} HNF_GEN_CCLK_PERFMON_CONFIG_1_t;


/*
 * Generic performance module configuration register in core clock domain.
 * Set up the configuration of generic performance module in core clock domain
 */

__extension__
typedef union {
	struct {
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
	};

	uint64_t word;
} HNF_GEN_CCLK_PERFMON_CONFIG_2_t;


/*
 * Generic performance module configuration register in core clock domain.
 * Set up the configuration of generic performance module in core clock domain
 */

__extension__
typedef union {
	struct {
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
	};

	uint64_t word;
} HNF_GEN_CCLK_PERFMON_CONFIG_3_t;


/*
 * Generic performance value register in core clock domain.
 * The register is used to provide interface where software can read the
 * value of generic performance counter in core clock domain
 */

__extension__
typedef union {
	struct {
		/* Selected generic performance register's value */
		uint64_t val : 64;
	};

	uint64_t word;
} HNF_GEN_CCLK_PERFMVAL_t;


/*
 * Generic performance value register in core clock domain.
 * The register is used to provide interface where software can read the
 * value of generic performance counter in core clock domain
 */

__extension__
typedef union {
	struct {
		/* Selected generic performance register's value */
		uint64_t val : 64;
	};

	uint64_t word;
} HNF_GEN_CCLK_PERFMVAL_1_t;


/*
 * Generic performance value register in core clock domain.
 * The register is used to provide interface where software can read the
 * value of generic performance counter in core clock domain
 */

__extension__
typedef union {
	struct {
		/* Selected generic performance register's value */
		uint64_t val : 64;
	};

	uint64_t word;
} HNF_GEN_CCLK_PERFMVAL_2_t;


/*
 * Generic performance value register in core clock domain.
 * The register is used to provide interface where software can read the
 * value of generic performance counter in core clock domain
 */

__extension__
typedef union {
	struct {
		/* Selected generic performance register's value */
		uint64_t val : 64;
	};

	uint64_t word;
} HNF_GEN_CCLK_PERFMVAL_3_t;


/*
 * Performance Control Register.
 * The performance control register holds global and per accumulator control
 * fields defining the operation of the module on 0/1 data paths
 */

__extension__
typedef union {
	struct {
		/* Filter Match options for accumulator 1 */
		uint64_t fm1          : 3;
		/* Reserved. */
		uint64_t __reserved_0 : 1;
		/*
		 * PERFACC1 match selection for accumulation trigger. This
		 * field is ignored if ACCM1 selects free runing counter mode
		 */
		uint64_t ms1          : 2;
		/* Reserved. */
		uint64_t __reserved_1 : 2;
		/*
		 * Accumulator 1 Mode. This field controls accumulator 1
		 * operation
		 */
		uint64_t accm1        : 3;
		/* Auto-Disable Accumulator 1 */
		uint64_t ad1          : 1;
		/*
		 * Event trigger for PERFACC1 path. This field is ignored if
		 * ACCM1=1
		 */
		uint64_t etrig1       : 2;
		/* Enable Both for accumulator 1 */
		uint64_t eb1          : 1;
		/*
		 * Enable performance accumulator 1 (PERFACC1) or both
		 * accumulators (PERFACC0, PERFACC1), depending on EB1 value.
		 */
		uint64_t en1          : 1;
		/* Filter Match options for accumulator 0 */
		uint64_t fm0          : 3;
		/* Reserved. */
		uint64_t __reserved_2 : 1;
		/*
		 * PERFACC0 match selection for accumulation trigger. This
		 * field is ignored if ACCM0 selects free runing counter mode
		 */
		uint64_t ms0          : 2;
		/* Reserved. */
		uint64_t __reserved_3 : 2;
		/*
		 * Accumulator 0 Mode. This field control accumulator 0
		 * operation
		 */
		uint64_t accm0        : 3;
		/* Auto-Disable Accumulator 0 */
		uint64_t ad0          : 1;
		/*
		 * Event trigger for PERFACC0 path. This field is ignored if
		 * ACCM0=1
		 */
		uint64_t etrig0       : 2;
		/* Enable Both for accumulator 0 */
		uint64_t eb0          : 1;
		/*
		 * Enable performance accumulator 0(PERFACC0) or both
		 * accumulators(PERFACC0, PERFACC1), depending on EB0 value.
		 */
		uint64_t en0          : 1;
		/* Reserved. */
		uint64_t __reserved_4 : 32;
	};

	uint64_t word;
} HNF_PERFCTL_t;


/*
 * Performance Event Selection Register.
 * The performance event selection register controls multiplexing logic at
 * the block level
 */

__extension__
typedef union {
	struct {
		/* Reserved. */
		uint64_t __reserved_0 : 16;
		/*
		 * Performance value selection from a group associated with
		 * EVTSEL.
		 */
		uint64_t pvalsel      : 4;
		/*
		 * Module select. This optional field is used in case the
		 * performance module collects performance data from
		 * additional other blocks
		 */
		uint64_t modsel       : 4;
		/* Event select signal */
		uint64_t evtsel       : 8;
		/* Reserved. */
		uint64_t __reserved_1 : 32;
	};

	uint64_t word;
} HNF_PERFEVT_t;


/*
 * Performance Value Extraction Register.
 * The performance value extraction register extracts two 32-bit values out
 * of PVAL with bitwise offset and width control
 */

__extension__
typedef union {
	struct {
		/*
		 * Value 1 LS bit offset within PVAL, indicating the
		 * right-shift amount from PVAL.
		 */
		uint64_t vofs1        : 7;
		/* Reserved. */
		uint64_t __reserved_0 : 1;
		/* Value 1 length */
		uint64_t vlen1        : 6;
		/* Reserved. */
		uint64_t __reserved_1 : 2;
		/*
		 * Value 0 LS bit offset within PVAL, indicating the
		 * right-shift amount from PVAL.
		 */
		uint64_t vofs0        : 7;
		/* Reserved. */
		uint64_t __reserved_2 : 1;
		/* Value 0 length */
		uint64_t vlen0        : 6;
		/* Reserved. */
		uint64_t __reserved_3 : 34;
	};

	uint64_t word;
} HNF_PERFVALEXT_t;


/*
 * Performance Accumulator 0 Register.
 * This register enable both reading and updating the 32-bit value
 * accumulated by accumulator 0.
 */

__extension__
typedef union {
	struct {
		/* Accumulator 0 value */
		uint64_t acc        : 56;
		/* Reserved. */
		uint64_t __reserved : 8;
	};

	uint64_t word;
} HNF_PERFACC0_t;


/*
 * Performance Accumulator 1 Register.
 * This register enable both reading and updating the 32-bit value
 * accumulated by accumulator 1.
 */

__extension__
typedef union {
	struct {
		/* Accumulator 1 value */
		uint64_t acc        : 56;
		/* Reserved. */
		uint64_t __reserved : 8;
	};

	uint64_t word;
} HNF_PERFACC1_t;


/*
 * Performance Match Value 0 Register.
 * This register provides up to 32-bit match to be compared with extracted
 * value 0 according to PERFCTL register configurations.
 */

__extension__
typedef union {
	struct {
		/* Match value to be compared against extracted value 0 */
		uint64_t mval       : 32;
		/* Reserved. */
		uint64_t __reserved : 32;
	};

	uint64_t word;
} HNF_PERFMVAL0_t;


/*
 * Performance Match Value 1 Register.
 * This register provides up to 32-bit match to be compared with extracted
 * value 1 according to PERFCTL register configurations.
 */

__extension__
typedef union {
	struct {
		/* Match value to be compared against extracted value 1 */
		uint64_t mval       : 32;
		/* Reserved. */
		uint64_t __reserved : 32;
	};

	uint64_t word;
} HNF_PERFMVAL1_t;

#endif /* !defined(__DOXYGEN__) */

#endif /* !defined(__ASSEMBLER__) */

#endif /* !defined(__REGS_HNF_H__) */
