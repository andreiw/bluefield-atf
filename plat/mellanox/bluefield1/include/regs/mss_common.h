#ifndef __REGS_MSS_COMMON_H__
#define __REGS_MSS_COMMON_H__

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif
#include "mss_common_def.h"

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
} MSS_COMMON_DEV_INFO_t;


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
} MSS_COMMON_DEV_CTL_t;


/* Scratchpad. */

__extension__
typedef union {
	struct {
		/* Scratchpad. */
		uint64_t scratchpad : 64;
	};

	uint64_t word;
} MSS_COMMON_SCRATCHPAD_t;


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
} MSS_COMMON_SEMAPHORE0_t;


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
} MSS_COMMON_DMA_STATUS_t;


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
} MSS_COMMON_CLOCK_COUNT_t;


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
} MSS_COMMON_INT_SETUP_t;


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
} MSS_COMMON_FEATURE_CTL_t;


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
} MSS_COMMON_CRED_CTL_t;


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
} MSS_COMMON_SAM_CTL_t;


/*
 * DMA Read Pacer.
 * Control the behavior of dma read pacer
 */

__extension__
typedef union {
	struct {
		/* When set, enables the dma read pacer */
		uint64_t en               : 1;
		/* Average extra delay for each read request */
		uint64_t per_read_penalty : 3;
		/* The max value of counter in the read pacer */
		uint64_t max_count        : 9;
		/*
		 * Control the ratio of reads with extra read penalty. The
		 * penalty ratio is equal to PENALTY_FRACATION/256
		 */
		uint64_t penalty_fraction : 8;
		/* Reserved. */
		uint64_t __reserved       : 43;
	};

	uint64_t word;
} MSS_COMMON_DMA_READ_PACE_t;


/*
 * DMA Write Latency.
 * Provides random sample and record of DMA AXI4 write latency
 */

__extension__
typedef union {
	struct {
		/* Contains minimum latency since last clear. */
		uint64_t min_lat       : 15;
		/* Reserved. */
		uint64_t __reserved_0  : 1;
		/* Contains maximum latency since last clear. */
		uint64_t max_lat       : 15;
		/* Reserved. */
		uint64_t __reserved_1  : 1;
		/*
		 * Contains latency of the most recently sampled transaction.
		 */
		uint64_t curr_lat      : 15;
		/* Reserved. */
		uint64_t __reserved_2  : 1;
		/*
		 * When written with a 1, sets MAX_LAT to zero and MIN_LAT to
		 * all 1's.
		 */
		uint64_t clear         : 1;
		/* Contains maximum outstanding requests. */
		uint64_t max_stnd_reqs : 12;
		/* Reserved. */
		uint64_t __reserved_3  : 3;
	};

	uint64_t word;
} MSS_COMMON_DMA_WR_LAT_t;


/*
 * DMA Read Latency.
 * Provides random sample and record of DMA AXI4 read latency
 */

__extension__
typedef union {
	struct {
		/* Contains minimum latency since last clear. */
		uint64_t min_lat       : 15;
		/* Reserved. */
		uint64_t __reserved_0  : 1;
		/* Contains maximum latency since last clear. */
		uint64_t max_lat       : 15;
		/* Reserved. */
		uint64_t __reserved_1  : 1;
		/*
		 * Contains latency of the most recently sampled transaction.
		 */
		uint64_t curr_lat      : 15;
		/* Reserved. */
		uint64_t __reserved_2  : 1;
		/*
		 * When written with a 1, sets MAX_LAT to zero and MIN_LAT to
		 * all 1's.
		 */
		uint64_t clear         : 1;
		/* Contains maximum outstanding requests. */
		uint64_t max_stnd_reqs : 12;
		/* Reserved. */
		uint64_t __reserved_3  : 3;
	};

	uint64_t word;
} MSS_COMMON_DMA_RD_LAT_t;


/*
 * Bandwidth_Control.
 * Limit the bandwidth between the node and the mesh. There are four
 * bandwidth control registers, which respectively control WRITE-CDN,
 * WRITE-DDN, READ-CDN and READ-DDN channel
 */

__extension__
typedef union {
	struct {
		/* When set, bandwidth control module is enabled */
		uint64_t en               : 1;
		/* The max value of buckets in the bandwidth control module */
		uint64_t max_count        : 9;
		/* Reserved. */
		uint64_t __reserved_0     : 10;
		/*
		 * Token is expressed as fraction format, token per cycle is
		 * equal to TOKEN_NUM/1023
		 */
		uint64_t token_num        : 10;
		/* Extra penalty added for each write/read transcation */
		uint64_t penalty_num      : 3;
		/*
		 * Control the ratio of reads with extra write/read penalty.
		 * The penalty ratio is equal to PENALTY_FRACATION/256
		 */
		uint64_t penalty_fraction : 8;
		/* Reserved. */
		uint64_t __reserved_1     : 23;
	};

	uint64_t word;
} MSS_COMMON_BND_CTL_t;


/*
 * Bandwidth_Control.
 * Limit the bandwidth between the node and the mesh. There are four
 * bandwidth control registers, which respectively control WRITE-CDN,
 * WRITE-DDN, READ-CDN and READ-DDN channel
 */

__extension__
typedef union {
	struct {
		/* When set, bandwidth control module is enabled */
		uint64_t en               : 1;
		/* The max value of buckets in the bandwidth control module */
		uint64_t max_count        : 9;
		/* Reserved. */
		uint64_t __reserved_0     : 10;
		/*
		 * Token is expressed as fraction format, token per cycle is
		 * equal to TOKEN_NUM/1023
		 */
		uint64_t token_num        : 10;
		/* Extra penalty added for each write/read transcation */
		uint64_t penalty_num      : 3;
		/*
		 * Control the ratio of reads with extra write/read penalty.
		 * The penalty ratio is equal to PENALTY_FRACATION/256
		 */
		uint64_t penalty_fraction : 8;
		/* Reserved. */
		uint64_t __reserved_1     : 23;
	};

	uint64_t word;
} MSS_COMMON_BND_CTL_1_t;


/*
 * Bandwidth_Control.
 * Limit the bandwidth between the node and the mesh. There are four
 * bandwidth control registers, which respectively control WRITE-CDN,
 * WRITE-DDN, READ-CDN and READ-DDN channel
 */

__extension__
typedef union {
	struct {
		/* When set, bandwidth control module is enabled */
		uint64_t en               : 1;
		/* The max value of buckets in the bandwidth control module */
		uint64_t max_count        : 9;
		/* Reserved. */
		uint64_t __reserved_0     : 10;
		/*
		 * Token is expressed as fraction format, token per cycle is
		 * equal to TOKEN_NUM/1023
		 */
		uint64_t token_num        : 10;
		/* Extra penalty added for each write/read transcation */
		uint64_t penalty_num      : 3;
		/*
		 * Control the ratio of reads with extra write/read penalty.
		 * The penalty ratio is equal to PENALTY_FRACATION/256
		 */
		uint64_t penalty_fraction : 8;
		/* Reserved. */
		uint64_t __reserved_1     : 23;
	};

	uint64_t word;
} MSS_COMMON_BND_CTL_2_t;


/*
 * Bandwidth_Control.
 * Limit the bandwidth between the node and the mesh. There are four
 * bandwidth control registers, which respectively control WRITE-CDN,
 * WRITE-DDN, READ-CDN and READ-DDN channel
 */

__extension__
typedef union {
	struct {
		/* When set, bandwidth control module is enabled */
		uint64_t en               : 1;
		/* The max value of buckets in the bandwidth control module */
		uint64_t max_count        : 9;
		/* Reserved. */
		uint64_t __reserved_0     : 10;
		/*
		 * Token is expressed as fraction format, token per cycle is
		 * equal to TOKEN_NUM/1023
		 */
		uint64_t token_num        : 10;
		/* Extra penalty added for each write/read transcation */
		uint64_t penalty_num      : 3;
		/*
		 * Control the ratio of reads with extra write/read penalty.
		 * The penalty ratio is equal to PENALTY_FRACATION/256
		 */
		uint64_t penalty_fraction : 8;
		/* Reserved. */
		uint64_t __reserved_1     : 23;
	};

	uint64_t word;
} MSS_COMMON_BND_CTL_3_t;


/*
 * BandWidth Throttle Control.
 * Control Bandwidth throttle algorithm behavior.There are four bandwidth
 * throttle control registers, which respectively control WRITE-CDN,
 * WRITE-DDN, READ-CDN and READ-DDN channel
 */

__extension__
typedef union {
	struct {
		/* When set, bandwidth throttle algorithm is enabled */
		uint64_t en         : 1;
		/*
		 * The threshold used for determing whether the mesh is
		 * congested or not
		 */
		uint64_t cong_thres : 15;
		/*
		 * Maximum possible value of Bandwith throttle module output
		 */
		uint64_t max        : 10;
		/*
		 * Minimum possible value of Bandwith throttle module output
		 */
		uint64_t min        : 10;
		/*
		 * The ratio between decrement amout and maximum value is
		 * 1/(1 << DEC_PER)
		 */
		uint64_t dec_per    : 5;
		/*
		 * The ratio between increment amout and maximum value is
		 * 1/(1 << INC_PER)
		 */
		uint64_t inc_per    : 5;
		/*
		 * The number of cycles in one monitor window of the algorithm
		 */
		uint64_t win_len    : 18;
	};

	uint64_t word;
} MSS_COMMON_BND_THRT_CTL_t;


/*
 * BandWidth Throttle Control.
 * Control Bandwidth throttle algorithm behavior.There are four bandwidth
 * throttle control registers, which respectively control WRITE-CDN,
 * WRITE-DDN, READ-CDN and READ-DDN channel
 */

__extension__
typedef union {
	struct {
		/* When set, bandwidth throttle algorithm is enabled */
		uint64_t en         : 1;
		/*
		 * The threshold used for determing whether the mesh is
		 * congested or not
		 */
		uint64_t cong_thres : 15;
		/*
		 * Maximum possible value of Bandwith throttle module output
		 */
		uint64_t max        : 10;
		/*
		 * Minimum possible value of Bandwith throttle module output
		 */
		uint64_t min        : 10;
		/*
		 * The ratio between decrement amout and maximum value is
		 * 1/(1 << DEC_PER)
		 */
		uint64_t dec_per    : 5;
		/*
		 * The ratio between increment amout and maximum value is
		 * 1/(1 << INC_PER)
		 */
		uint64_t inc_per    : 5;
		/*
		 * The number of cycles in one monitor window of the algorithm
		 */
		uint64_t win_len    : 18;
	};

	uint64_t word;
} MSS_COMMON_BND_THRT_CTL_1_t;


/*
 * BandWidth Throttle Control.
 * Control Bandwidth throttle algorithm behavior.There are four bandwidth
 * throttle control registers, which respectively control WRITE-CDN,
 * WRITE-DDN, READ-CDN and READ-DDN channel
 */

__extension__
typedef union {
	struct {
		/* When set, bandwidth throttle algorithm is enabled */
		uint64_t en         : 1;
		/*
		 * The threshold used for determing whether the mesh is
		 * congested or not
		 */
		uint64_t cong_thres : 15;
		/*
		 * Maximum possible value of Bandwith throttle module output
		 */
		uint64_t max        : 10;
		/*
		 * Minimum possible value of Bandwith throttle module output
		 */
		uint64_t min        : 10;
		/*
		 * The ratio between decrement amout and maximum value is
		 * 1/(1 << DEC_PER)
		 */
		uint64_t dec_per    : 5;
		/*
		 * The ratio between increment amout and maximum value is
		 * 1/(1 << INC_PER)
		 */
		uint64_t inc_per    : 5;
		/*
		 * The number of cycles in one monitor window of the algorithm
		 */
		uint64_t win_len    : 18;
	};

	uint64_t word;
} MSS_COMMON_BND_THRT_CTL_2_t;


/*
 * BandWidth Throttle Control.
 * Control Bandwidth throttle algorithm behavior.There are four bandwidth
 * throttle control registers, which respectively control WRITE-CDN,
 * WRITE-DDN, READ-CDN and READ-DDN channel
 */

__extension__
typedef union {
	struct {
		/* When set, bandwidth throttle algorithm is enabled */
		uint64_t en         : 1;
		/*
		 * The threshold used for determing whether the mesh is
		 * congested or not
		 */
		uint64_t cong_thres : 15;
		/*
		 * Maximum possible value of Bandwith throttle module output
		 */
		uint64_t max        : 10;
		/*
		 * Minimum possible value of Bandwith throttle module output
		 */
		uint64_t min        : 10;
		/*
		 * The ratio between decrement amout and maximum value is
		 * 1/(1 << DEC_PER)
		 */
		uint64_t dec_per    : 5;
		/*
		 * The ratio between increment amout and maximum value is
		 * 1/(1 << INC_PER)
		 */
		uint64_t inc_per    : 5;
		/*
		 * The number of cycles in one monitor window of the algorithm
		 */
		uint64_t win_len    : 18;
	};

	uint64_t word;
} MSS_COMMON_BND_THRT_CTL_3_t;


/*
 * Generic performance module configuration register.
 * Set up the configuration of generic performance module
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
} MSS_COMMON_GEN_PERFMON_CONFIG_t;


/*
 * Generic performance module configuration register.
 * Set up the configuration of generic performance module
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
} MSS_COMMON_GEN_PERFMON_CONFIG_1_t;


/*
 * Generic performance module configuration register.
 * Set up the configuration of generic performance module
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
} MSS_COMMON_GEN_PERFMON_CONFIG_2_t;


/*
 * Generic performance module configuration register.
 * Set up the configuration of generic performance module
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
} MSS_COMMON_GEN_PERFMON_CONFIG_3_t;


/*
 * Generic performance value register.
 * The register is used to provide interface where software can read the
 * value of generic performance counter
 */

__extension__
typedef union {
	struct {
		/* Selected generic performance register's value */
		uint64_t val : 64;
	};

	uint64_t word;
} MSS_COMMON_GEN_PERFMVAL_t;


/*
 * Generic performance value register.
 * The register is used to provide interface where software can read the
 * value of generic performance counter
 */

__extension__
typedef union {
	struct {
		/* Selected generic performance register's value */
		uint64_t val : 64;
	};

	uint64_t word;
} MSS_COMMON_GEN_PERFMVAL_1_t;


/*
 * Generic performance value register.
 * The register is used to provide interface where software can read the
 * value of generic performance counter
 */

__extension__
typedef union {
	struct {
		/* Selected generic performance register's value */
		uint64_t val : 64;
	};

	uint64_t word;
} MSS_COMMON_GEN_PERFMVAL_2_t;


/*
 * Generic performance value register.
 * The register is used to provide interface where software can read the
 * value of generic performance counter
 */

__extension__
typedef union {
	struct {
		/* Selected generic performance register's value */
		uint64_t val : 64;
	};

	uint64_t word;
} MSS_COMMON_GEN_PERFMVAL_3_t;


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
} MSS_COMMON_PERFCTL_t;


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
} MSS_COMMON_PERFEVT_t;


/*
 * Performance Value Extraction Register.
 * The performance value extraction register extracts two 56-bit values out
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
} MSS_COMMON_PERFVALEXT_t;


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
} MSS_COMMON_PERFACC0_t;


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
} MSS_COMMON_PERFACC1_t;


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
} MSS_COMMON_PERFMVAL0_t;


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
} MSS_COMMON_PERFMVAL1_t;

#endif /* !defined(__DOXYGEN__) */

#endif /* !defined(__ASSEMBLER__) */

#endif /* !defined(__REGS_MSS_COMMON_H__) */
