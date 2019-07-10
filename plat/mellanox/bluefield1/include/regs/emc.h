#ifndef __REGS_EMC_H__
#define __REGS_EMC_H__

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif
#include "emc_def.h"

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
} EMC_DEV_INFO_t;


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
} EMC_DEV_CTL_t;


/* Scratchpad. */

__extension__
typedef union {
	struct {
		/* Scratchpad. */
		uint64_t scratchpad : 64;
	};

	uint64_t word;
} EMC_SCRATCHPAD_t;


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
} EMC_SEMAPHORE0_t;


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
} EMC_DMA_STATUS_t;


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
} EMC_CLOCK_COUNT_t;


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
} EMC_INT_SETUP_t;


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
} EMC_FEATURE_CTL_t;


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
} EMC_CRED_CTL_t;


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
} EMC_SAM_CTL_t;


/*
 * IND_DATA.
 * Indirect data registers
 */

__extension__
typedef union {
	struct {
		/* Data */
		uint32_t val : 32;
	};

	uint32_t word;
} EMC_IND_DATA_t;


/*
 * IND_CMD.
 * Indirect command registers
 */

__extension__
typedef union {
	struct {
		/*
		 * Operation type:
		 * 0 - Read
		 * 1 - write
		 */
		uint32_t op           : 1;
		/* Reserved. */
		uint32_t __reserved_0 : 7;
		/*
		 * 0x0 - Manage Command parameters
		 * 0x1 - APB Interface
		 * 0x2 - IFC Bist Pattern SRAM
		 * 0x3 - PHY Specific
		 * 0x7 - Indirect Data Registers Lock
		 */
		uint32_t mem_id       : 3;
		/* Reserved. */
		uint32_t __reserved_1 : 21;
	};

	uint32_t word;
} EMC_IND_CMD_t;


/*
 * IND_ADDR.
 * Address for host operation
 */

__extension__
typedef union {
	struct {
		/* indirect address */
		uint32_t val : 32;
	};

	uint32_t word;
} EMC_IND_ADDR_t;


/*
 * IND_STS.
 * Holds statuses on various processes in the memory controller
 */

__extension__
typedef union {
	struct {
		/* the menagment fifo is ready to accept next cmd */
		uint32_t rdy          : 1;
		/* the menagment fifo is ready to accept next cmd */
		uint32_t success      : 1;
		/* Reserved. */
		uint32_t __reserved_0 : 6;
		/* after reset the relevant arbiter is disabled */
		uint32_t arb_dis      : 1;
		/* Reserved. */
		uint32_t __reserved_1 : 7;
		/* APB interface transaction ended */
		uint32_t apb_ifc_stat : 1;
		/* PHY commands ready */
		uint32_t phy_cfg_rdy  : 1;
		/* Reserved. */
		uint32_t __reserved_2 : 6;
		/* management command burst done bit */
		uint32_t mng_cmd_done : 1;
		/* Reserved. */
		uint32_t __reserved_3 : 7;
	};

	uint32_t word;
} EMC_IND_STS_t;


/*
 * SPARE.
 * Spare Configuration Register
 */

__extension__
typedef union {
	struct {
		/* 32 bit Spare Configuration data */
		uint32_t val : 32;
	};

	uint32_t word;
} EMC_SPARE_t;


/*
 * TIMING1.
 * Holds time delay parameter of the DDR
 */

__extension__
typedef union {
	struct {
		/*
		 * Gap between Auto precharge signal on write cas and the
		 * time the bank is ready again.
		 * (CWL + AL + 4 + Twr + Trp)
		 * This Parameter appears in DDR clock
		 */
		uint32_t wra_trp_gap : 8;
		/*
		 * Gap between Auto precharge signal and the time the bank is
		 * ready again.
		 * (AL + Trtp + Trp)
		 * This Parameter appears in DDR clock
		 */
		uint32_t rda_trp_gap : 8;
		/*
		 * CL + AL
		 * AL can be either:
		 *          CL - 1
		 *             or
		 *          CL - 2
		 * This Parameter appears in DDR clock
		 */
		uint32_t rd_al       : 8;
		/*
		 * CWL + AL
		 * AL can be either:
		 * CL - 1
		 * or
		 * CL - 2
		 * This Parameter appears in DDR clock
		 */
		uint32_t wr_al       : 8;
	};

	uint32_t word;
} EMC_TIMING1_t;


/*
 * TIMING2.
 * Holds time delay parameter of a Same Package Rank DDR
 */

__extension__
typedef union {
	struct {
		/*
		 * write to read command switching penalty (clocks which no
		 * CAS command can be executed)
		 * ceil[(cwl + al + 4 + Twtr_s)/2]
		 */
		uint32_t wr_rd_gap_sh : 8;
		/*
		 * write to read command switching penalty (clocks which no
		 * CAS command can be executed)
		 * ceil[(cwl + al + 4 + Twtr_l)/2]
		 */
		uint32_t wr_rd_gap_lo : 8;
		/*
		 * read to write command switching penalty (clocks which no
		 * CAS command can be executed)
		 * ceil((rd_al - wr_al + 2)/2)
		 */
		uint32_t rd_wr_gap    : 8;
		/* Reserved. */
		uint32_t __reserved   : 8;
	};

	uint32_t word;
} EMC_TIMING2_t;


/*
 * TIMING3.
 * Holds time delay parameter of the DDR
 */

__extension__
typedef union {
	struct {
		/*
		 * Number of active ODT cycles (DDR clock)
		 * activation starts with CAS WR command.
		 */
		uint32_t odt_duration : 4;
		/* Reserved. */
		uint32_t __reserved   : 4;
		/*
		 * ACT to ACT or REF command period
		 * This Parameter appears in DDR clock
		 */
		uint32_t trc          : 8;
		/*
		 * DQS read enable latency.
		 * Enables using DQS as clock input for DQ data strobe.
		 * Should be related to memory read latency + PHY parameters
		 * if available.
		 * Should appear in DDR clocks.
		 */
		uint32_t dqs_rd_en    : 8;
		/*
		 * DQS write enable latency.
		 * Enables using OE DQ wr data strobe.
		 * Should be related to memory write latency
		 * Should appear in DDR clocks.
		 */
		uint32_t dqs_wr_en    : 8;
	};

	uint32_t word;
} EMC_TIMING3_t;


/*
 * TIMING4.
 * Holds time delay parameter of the DDR
 */

__extension__
typedef union {
	struct {
		/*
		 * ACT to ACT delay different logical ranks
		 * MAX(trrd_dlr,tccd_dlr) (DDR cycles)
		 */
		uint32_t act2act_gap_dlr     : 3;
		/*
		 * ACT to ACT delay same logical rank different bank group
		 * ceil[MAX(trrd_s_slr,tccd_s_slr)/2] (MC cycles)
		 */
		uint32_t act2act_gap_s_slr   : 3;
		/*
		 * ACT to ACT delay same logical rank same bank group
		 * ceil[MAX(trrd_l_slr,tccd_l_slr)/2] (MC cycles)
		 */
		uint32_t act2act_gap_l_slr   : 3;
		/* Reserved. */
		uint32_t __reserved          : 4;
		/* Four activate window different logical ranks (MC cycles) */
		uint32_t tfaw_dlr            : 5;
		/* Four activate window same logical rank (MC cycles) */
		uint32_t tfaw_slr            : 5;
		/* Reduced TFAW_SLR for usage on Keep Phase conditions. */
		uint32_t tfaw_keep_phase     : 5;
		/*
		 * time slot during which ras phase cannot be moved from even
		 * to odd in the same MC clock
		 * ceil[MAX(trrd_l_slr,tccd_l_slr, trrd_dlr, tccd_dlr)/2] (MC
		 * cycles)
		 */
		uint32_t keep_phase_duration : 4;
	};

	uint32_t word;
} EMC_TIMING4_t;


/*
 * TIMING5.
 * Holds time delay parameter of the Multiple Package Ranks Gaps
 */

__extension__
typedef union {
	struct {
		/*
		 * Same command type rank switching penalty
		 * Parameter defined in DDR clocks.
		 * ceil[package rank switch penalty + BL/2]
		 * (number of ddr quiet cycles plus 4)
		 */
		uint32_t prank_switch_gap : 4;
		/* Reserved. */
		uint32_t __reserved_0     : 4;
		/*
		 * Write to Read command rank switching penalty
		 * Parameter defined in MC clocks
		 * ceil[(cwl + 4 + Rank switch penalty)/2]
		 */
		uint32_t prank_wr_rd_gap  : 4;
		/* Reserved. */
		uint32_t __reserved_1     : 4;
		/*
		 * Read to Write command rank switching penalty
		 * Parameter defined in MC clocks
		 * ceil[(rd_al - wr_al + 2)/2]
		 */
		uint32_t prank_rd_wr_gap  : 4;
		/* Reserved. */
		uint32_t __reserved_2     : 12;
	};

	uint32_t word;
} EMC_TIMING5_t;


/*
 * TIMING6.
 * Holds timeing parameters of the DDR4 Component
 */

__extension__
typedef union {
	struct {
		/*
		 * tCCD_l DDR4 component Timing parameter.
		 * Same Bank Group CAS to CAS operations delay. (parameter in
		 * MC Clocks)
		 */
		uint32_t tccd_l     : 4;
		/* Reserved. */
		uint32_t __reserved : 28;
	};

	uint32_t word;
} EMC_TIMING6_t;


/*
 * RFRSH_PARAMS_1X.
 * Holds parameters refers to the refresh process
 */

__extension__
typedef union {
	struct {
		/*
		 * Average periodic refresh interval
		 * (in number of clocks)
		 */
		uint32_t trefi       : 15;
		/* REF command to ACT or REF command time */
		uint32_t trfc        : 9;
		/* number of refresh back to back commnads (up to 9) */
		uint32_t rfrsh_burst : 4;
		/*
		 * 0x0 - fixed 1x
		 * 0x1 - fixed 2x
		 * 0x2 - fixed 4x
		 * 0x3 - Reserved
		 * 0x4 - Reserved
		 * 0x5 - On The Fly 2x
		 * 0x6 - On The Fly 4x
		 * 0x7 - Reserved
		 */
		uint32_t mode        : 3;
		/* Reserved. */
		uint32_t __reserved  : 1;
	};

	uint32_t word;
} EMC_RFRSH_PARAMS_1X_t;


/*
 * RFRSH_PARAMS_2X.
 * Holds parameters refers to the refresh process
 */

__extension__
typedef union {
	struct {
		/*
		 * Average periodic refresh interval
		 * (in number of clocks)
		 */
		uint32_t trefi       : 15;
		/* REF command to ACT or REF command time */
		uint32_t trfc        : 9;
		/* number of refresh back to back commnads (up to 17) */
		uint32_t rfrsh_burst : 5;
		/* Reserved. */
		uint32_t __reserved  : 3;
	};

	uint32_t word;
} EMC_RFRSH_PARAMS_2X_t;


/*
 * RFRSH_PARAMS_4X.
 * Holds parameters refers to the refresh process
 */

__extension__
typedef union {
	struct {
		/*
		 * Average periodic refresh interval
		 * (in number of clocks)
		 */
		uint32_t trefi       : 15;
		/* REF command to ACT or REF command time */
		uint32_t trfc        : 9;
		/* number of refresh back to back commnads (up to 33) */
		uint32_t rfrsh_burst : 6;
		/* Reserved. */
		uint32_t __reserved  : 2;
	};

	uint32_t word;
} EMC_RFRSH_PARAMS_4X_t;


/*
 * RFRSH_PARAMS_DLR.
 * Holds refresh parameters to different logical ranks
 */

__extension__
typedef union {
	struct {
		/* REF to REF different logical ranks */
		uint32_t trfc_1x    : 9;
		/* REF to REF different logical ranks */
		uint32_t trfc_2x    : 9;
		/* REF to REF different logical ranks */
		uint32_t trfc_4x    : 9;
		/* Reserved. */
		uint32_t __reserved : 5;
	};

	uint32_t word;
} EMC_RFRSH_PARAMS_DLR_t;


/*
 * CALIB_PARAMS.
 * short calibration parameters
 */

__extension__
typedef union {
	struct {
		/*
		 * calibration threshold.
		 * 10 msb of 32 bits counter.
		 */
		uint32_t th              : 10;
		/* Reserved. */
		uint32_t __reserved_0    : 6;
		/*
		 * ZQCS command to ACT interval time.
		 * counted in ddr_mc_clk
		 */
		uint32_t tzqcs           : 10;
		/* Reserved. */
		uint32_t __reserved_1    : 4;
		/*
		 * 0 - activate short calibration in a staggered manner
		 * 1 - activate short calibration in all ranks together
		 */
		uint32_t rank_overlap_en : 1;
		/*
		 * when debug mode - the calibration happens in resolution of
		 * 1k clocks.
		 */
		uint32_t debug_mode      : 1;
	};

	uint32_t word;
} EMC_CALIB_PARAMS_t;


/*
 * DDR_PROPERTIES.
 * SDRAM device and DIMM phisical parameters
 */

__extension__
typedef union {
	struct {
		/*
		 * The number of package ranks connected to this memory
		 * channel.
		 * 00 - single rank
		 * 01 - dual rank
		 * 10 - quad rank
		 * 11 - reserved
		 */
		uint32_t package_rank_num : 2;
		/* Reserved. */
		uint32_t __reserved_0     : 2;
		/*
		 * number of logical ranks per package rank (3DS)
		 * 00 - one logical rank (monolithic)
		 * 01 - two logical ranks (2H TSV)
		 * 10 - four logical ranks (4H TSV)
		 * 11 - reserved
		 */
		uint32_t logic_rank_num   : 2;
		/* Reserved. */
		uint32_t __reserved_1     : 2;
		/*
		 * 00 - x4
		 * 01 - x8
		 * 10 - x16
		 * 11 - reserved
		 */
		uint32_t dq_pins          : 2;
		/* Reserved. */
		uint32_t __reserved_2     : 2;
		/*
		 * Indicates that two single rank DIMMs are connected to this
		 * memory channel.
		 */
		uint32_t two_single_rank  : 1;
		/* Reserved. */
		uint32_t __reserved_3     : 3;
		/*
		 * Indicates that SDRAM System has a single Byte SDRAM
		 * connected to Byte Lane 0 and No DIMMs are connected to
		 * this memory channel.
		 */
		uint32_t single_sdram     : 1;
		/*
		 * Send only one CAS command (instead of 8 or 9) when working
		 * in SINGLE_SDRAM mode.
		 * Relevant only when SINGLE_SDRAM is set to $1$.
		 */
		uint32_t single_cas       : 1;
		/* Reserved. */
		uint32_t __reserved_4     : 2;
		/* Controls 3DS AL = CL -3 operational mode. */
		uint32_t al_3ds_mode      : 1;
		/* Reserved. */
		uint32_t __reserved_5     : 3;
		/*
		 * Write DBI Mode Enable configuration.
		 * 0 - Write DBI mode disabled
		 * 1 - Write DBI mode enabled
		 */
		uint32_t wr_dbi_en        : 1;
		/*
		 * Read DBI Mode Enable configuration.
		 * 0 - Read DBI mode disabled
		 * 1 - Read DBI mode enabled
		 */
		uint32_t rd_dbi_en        : 1;
		/* Reserved. */
		uint32_t __reserved_6     : 6;
	};

	uint32_t word;
} EMC_DDR_PROPERTIES_t;


/*
 * DDR_DEBUG.
 * SDRAM device and DIMM Debug Modes Control
 */

__extension__
typedef union {
	struct {
		/* Enables Command Address Parity Mode. */
		uint32_t parity_en    : 1;
		/* Enables Data CRC Mode operation */
		uint32_t crc_en       : 1;
		/* Reserved. */
		uint32_t __reserved_0 : 6;
		/* Enables adding 1 DDR Clock delay to the Parity Signal */
		uint32_t parity_delay : 1;
		/* Reserved. */
		uint32_t __reserved_1 : 7;
		/*
		 * Clears the Alert Counter and releases the Alert Logic for
		 * the next Alert Event.
		 */
		uint32_t alert_clear  : 1;
		/* Reserved. */
		uint32_t __reserved_2 : 15;
	};

	uint32_t word;
} EMC_DDR_DEBUG_t;


/*
 * ACTIVE_RANKS.
 * Physical and Logical Rank active bitmaps
 */

__extension__
typedef union {
	struct {
		/*
		 * Package Rank 0 Logical Rank Active Bitmap:
		 * 0 - Non active
		 * 1 - Active.
		 * In case Package Rank is not active, BM must be set to
		 * zeros.
		 */
		uint32_t pr_bm_0 : 8;
		/*
		 * Package Rank 1 Logical Rank Active Bitmap:
		 * 0 - Non active
		 * 1 - Active.
		 * In case Package Rank is not active, BM must be set to
		 * zeros.
		 */
		uint32_t pr_bm_1 : 8;
		/*
		 * Package Rank 1 Logical Rank Active Bitmap:
		 * 0 - Non active
		 * 1 - Active.
		 * In case Package Rank is not active, BM must be set to
		 * zeros.
		 */
		uint32_t pr_bm_2 : 8;
		/*
		 * Package Rank 1 Logical Rank Active Bitmap:
		 * 0 - Non active
		 * 1 - Active.
		 * In case Package Rank is not active, BM must be set to
		 * zeros.
		 */
		uint32_t pr_bm_3 : 8;
	};

	uint32_t word;
} EMC_ACTIVE_RANKS_t;


/*
 * ALERT_COUNT.
 * Alert Signal width Indication
 */

__extension__
typedef union {
	struct {
		/* Alert Input Width */
		uint32_t val : 32;
	};

	uint32_t word;
} EMC_ALERT_COUNT_t;


/*
 * APB_IFC.
 * APB Interface control and configuration
 */

__extension__
typedef union {
	struct {
		/* APB Interface Clock Enable Control */
		uint32_t cont_clk_en     : 1;
		/* APB Interface Reset Control */
		uint32_t cont_rst        : 1;
		/* Reserved. */
		uint32_t __reserved_0    : 2;
		/*
		 * Enable the APB ready functionality.
		 * 0 - ignores the ready input.
		 * 1 - enables the ready input.
		 */
		uint32_t cont_ready_en   : 1;
		/* Reserved. */
		uint32_t __reserved_1    : 19;
		/* APB Interface Read Latency Configuration */
		uint32_t cont_rl_cfg     : 4;
		/*
		 * APB Clock Divider Configuration (Core Clock Divider)
		 * 000 - core/4
		 * 001 - core/8
		 * 010 - core/16
		 * 011 - core/32
		 * 100 - core/64
		 * 101 - core/128
		 * 110 - core/256
		 * 111 - core/512
		 */
		uint32_t cfg_clk_div     : 3;
		/* APB Clock Divider Enable Control */
		uint32_t cont_clk_div_en : 1;
	};

	uint32_t word;
} EMC_APB_IFC_t;


/*
 * MC_QUAD_MODE.
 * Control of Internal CS Routing to CS/CID and ODT device outputs for Quad
 * Physical Rank Support in a Single RDIMM/LRDIMM
 */

__extension__
typedef union {
	struct {
		/*
		 * 0 - Direct 2 CS Mode
		 * 1 - Direct Quad CS Mode
		 * 2 - Reserved
		 * 3 - Encoded Quad CS Mode
		 */
		uint32_t rcd_mode      : 2;
		/* Reserved. */
		uint32_t __reserved_0  : 2;
		/*
		 * Controls the C[0] Logical Signal rout through the C[2]
		 * Physical signals
		 */
		uint32_t c0_over_c2    : 1;
		/* Reserved. */
		uint32_t __reserved_1  : 3;
		/*
		 * Controls the ODT[3:2] Logical Signal rout through the
		 * ODT[1:0] Physical signals
		 */
		uint32_t odt_quad_mode : 1;
		/* Reserved. */
		uint32_t __reserved_2  : 23;
	};

	uint32_t word;
} EMC_MC_QUAD_MODE_t;


/*
 * MC_DDR_IF.
 * Holds parameters refers to the interface between the MC and the physical
 * interface
 */

__extension__
typedef union {
	struct {
		/* Enable the memory controller$s request arbiter */
		uint32_t mc_en          : 1;
		/* Reserved. */
		uint32_t __reserved_0   : 3;
		/*
		 * Enable PPR mode for the DDR.
		 * i.e. - connect ind0-8 to the data bus
		 */
		uint32_t ppr_en         : 1;
		/* Reserved. */
		uint32_t __reserved_1   : 3;
		/*
		 * Allow short calibration.
		 * 1 - calibration enable
		 * 0 - calibration disable
		 */
		uint32_t calib_en       : 1;
		/* Reserved. */
		uint32_t __reserved_2   : 7;
		/* Enable refresh mechanism */
		uint32_t ref_en         : 1;
		/* Reserved. */
		uint32_t __reserved_3   : 1;
		/* Switching the refresh rate OTF */
		uint32_t ref_switch     : 1;
		/* Reserved. */
		uint32_t __reserved_4   : 5;
		/*
		 * Expeditious Refresh:
		 * 0 - Refresh interval is Trefi
		 * 1 - Refresh interval is Trefi/4
		 */
		uint32_t ref_exped      : 1;
		/* Reserved. */
		uint32_t __reserved_5   : 5;
		/* Enable the statistics counter */
		uint32_t stat_cntr_en   : 1;
		/*
		 * 0 - Refresh interval is Trefi
		 * 1 - Refresh interval is Trefi/2
		 */
		uint32_t ref_hot_update : 1;
	};

	uint32_t word;
} EMC_MC_DDR_IF_t;


/*
 * MC_MI_IF.
 * Holds parameters refers to the mc and mi interface
 */

__extension__
typedef union {
	struct {
		/*
		 * valid values 1..6
		 * clocks gap between mc_mi_ack and mi_mc_addr valid signal
		 */
		uint32_t ack_addr_gap    : 3;
		/* Reserved. */
		uint32_t __reserved_0    : 1;
		/*
		 * valid values 1..7
		 * clocks gap between mc_mi_ack and mi_mc_data valid signal
		 */
		uint32_t ack_data_gap    : 3;
		/* Reserved. */
		uint32_t __reserved_1    : 1;
		/* gap between rd_next signal and the returned data */
		uint32_t ret_data_gap    : 4;
		/*
		 * Num of colock in advance to assert the bank ready
		 * signal,artificially.
		 */
		uint32_t bank_rdy_pre    : 4;
		/* Reserved. */
		uint32_t __reserved_2    : 12;
		/*
		 * 15 for DDR4 x8
		 * else it should be 0x7
		 */
		uint32_t mc_highest_bank : 4;
	};

	uint32_t word;
} EMC_MC_MI_IF_t;


/*
 * ARB_ALGORITHM.
 * Arbiter Algorithm Mode Register
 */

__extension__
typedef union {
	struct {
		/*
		 * 0 - BIST
		 * 1- functional
		 */
		uint32_t mode       : 1;
		/* Reserved. */
		uint32_t __reserved : 31;
	};

	uint32_t word;
} EMC_ARB_ALGORITHM_t;


/*
 * EXT_MC_LATENCY.
 * Describe the external latency (phy and board delay)
 */

__extension__
typedef union {
	struct {
		/* External read latency */
		uint32_t ext_mc_rl  : 8;
		/* Reserved. */
		uint32_t __reserved : 24;
	};

	uint32_t word;
} EMC_EXT_MC_LATENCY_t;


/*
 * MC_ADDR_MIRROR.
 * enable/disable the address mirroring feature
 */

__extension__
typedef union {
	struct {
		/*
		 * bit 0 - package rank 0
		 * bit 1 - package rank 1
		 * bit 2 - package rank 2
		 * bit 3 - package rank 3
		 */
		uint32_t enable     : 4;
		/* Reserved. */
		uint32_t __reserved : 28;
	};

	uint32_t word;
} EMC_MC_ADDR_MIRROR_t;


/*
 * PHY_CTRL.
 * Holds parameters refers to the interface between the MC and the physical
 * interface
 */

__extension__
typedef union {
	struct {
		/*
		 * configure the clock enable pin (cke) - bit per rank:
		 * bit 0 - rank 0
		 * bit 1 - rank 1
		 * bit 2 - rank 2
		 * bit 3 - rank 3
		 */
		uint32_t ck_en            : 4;
		/*
		 * Self refresh control enable - bit per rank:
		 * bit 4 - rank 0
		 * bit 5 - rank 1
		 * bit 6 - rank 2
		 * bit 7 - rank 3
		 */
		uint32_t ddr_self_refresh : 4;
		/* Reserved. */
		uint32_t __reserved       : 24;
	};

	uint32_t word;
} EMC_PHY_CTRL_t;


/*
 * PHY_UPDATE.
 * phy calibration parameters
 */

__extension__
typedef union {
	struct {
		/*
		 * Enables DFI MC Initiated PHY Update operation trigger on
		 * ZCAL SDRAM operation
		 */
		uint32_t mc_en         : 1;
		/*
		 * Enables DFI PHY Initiated Update operation triggered by
		 * the DFI signal dfi_phyupd_req
		 */
		uint32_t phy_en        : 1;
		/* Reserved. */
		uint32_t __reserved_0  : 2;
		/*
		 * Determines one of the 2 DFI MC Initiated PHY update types
		 */
		uint32_t mc_type       : 1;
		/* Reserved. */
		uint32_t __reserved_1  : 3;
		/*
		 * Controls the delay of the DFI MC Initiated PHY update from
		 * the SDRAM ZCAL operation.
		 */
		uint32_t mc_delay      : 4;
		/*
		 * Controls the DFI MC Initiated PHY Update signal pulse width
		 * (relevant for MC Initiated Update Type 0)
		 */
		uint32_t mc_maxpw      : 4;
		/* Controls the delay of the acknowledge to PHY Update. */
		uint32_t phy_ack_delay : 4;
		/* Reserved. */
		uint32_t __reserved_2  : 12;
	};

	uint32_t word;
} EMC_PHY_UPDATE_t;


/*
 * WRD_LVL_MODES.
 * Write and Read Leveling operational modes
 */

__extension__
typedef union {
	struct {
		/*
		 * ODT activation for Write Leveling Mode or SDRAM PDA Mode
		 * or LRDIMM DB PBA Mode.
		 * bit 0 - package rank 0
		 * bit 1 - package rank 1
		 * bit 2 - package rank 2
		 * bit 3 - package rank 3
		 * Per Bit Value = 1 - ODT active
		 * Per Bit Value = 0 - ODT in-active
		 */
		uint32_t wlm_odt_en   : 4;
		/* Reserved. */
		uint32_t __reserved_0 : 4;
		/*
		 * MPR Activated Mode
		 * bit 8 - package rank 0
		 * bit 9 - package rank 1
		 * bit 10 - package rank 2
		 * bit 11 - package rank 3
		 */
		uint32_t mpr_mode     : 4;
		/* Reserved. */
		uint32_t __reserved_1 : 4;
		/* Auto precharge disable */
		uint32_t ap_dis       : 1;
		/* Reserved. */
		uint32_t __reserved_2 : 15;
	};

	uint32_t word;
} EMC_WRD_LVL_MODES_t;


/*
 * WRD_LVL_BL_DELAY.
 * Write and Read Leveling Per Byte lane Delay control
 */

__extension__
typedef union {
	struct {
		/*
		 * Per Byte Lane Write Leveling Configuration.
		 * 2 bits for each byte lane.
		 * Bits 1:0 - Byte Lane 0
		 * Bits 3:2 - Byte Lane 1
		 * Bits 5:4 - Byte Lane 2
		 * Bits 7:6 - Byte Lane 3
		 */
		uint32_t bl_wld_cfg   : 8;
		/* Reserved. */
		uint32_t __reserved_0 : 8;
		/*
		 * Per Byte Lane Read Leveling Configuration.
		 * 2 bits for each byte lane.
		 * Bits 17:16 - Byte Lane 0
		 * Bits 19:18 - Byte Lane 1
		 * Bits 21:20 - Byte Lane 2
		 * Bits 23:22 - Byte Lane 3
		 */
		uint32_t bl_rld_cfg   : 8;
		/* Reserved. */
		uint32_t __reserved_1 : 8;
	};

	uint32_t word;
} EMC_WRD_LVL_BL_DELAY_t;


/*
 * TRACE_TRANS_PARAMS.
 * parameters to count MC transactions
 */

__extension__
typedef union {
	struct {
		/*
		 * 0 - trace read command
		 * 1 - trace write command
		 * 2 - trace all commands
		 */
		uint32_t cmd           : 2;
		/*
		 * 0 - 3 traced logical rank
		 * 4 - trace all logical ranks
		 */
		uint32_t logical_rank  : 3;
		/*
		 * 0 - 3 traced package rank
		 * 4 - trace all package ranks
		 */
		uint32_t package_rank  : 3;
		/*
		 * 0 - 15 traced bank
		 * 16 - Trace all banks
		 */
		uint32_t bank          : 5;
		/* Reserved. */
		uint32_t __reserved    : 3;
		/* lowest stream id to measure statistics on */
		uint32_t low_streamid  : 8;
		/* highest stream id to measure statistics on */
		uint32_t high_streamid : 8;
	};

	uint32_t word;
} EMC_TRACE_TRANS_PARAMS_t;


/*
 * TRACE_TRANS_CNTR.
 * The statistical parameters resides on register 0x2A. the enable reside on
 * register 0x16
 */

__extension__
typedef union {
	struct {
		/* Holds Transactions Count value */
		uint32_t val : 32;
	};

	uint32_t word;
} EMC_TRACE_TRANS_CNTR_t;


/*
 * DDR_ERR_INJ.
 * SDRAM device and DIMM Debug Modes Error Injection Control
 */

__extension__
typedef union {
	struct {
		/*
		 * Enables CRC Error Generation on Specific Data Byte Lane.
		 * Each bit is related to a specific nibble or byte lane.
		 */
		uint32_t crc_error_bmp : 18;
		/* Reserved. */
		uint32_t __reserved_0  : 6;
		/*
		 * CRC Error Injection on the first Data Transaction.
		 * Writing a value of 1 triggers the Error injection.
		 */
		uint32_t crc_error     : 1;
		/* Reserved. */
		uint32_t __reserved_1  : 3;
		/*
		 * Parity Error Injection on the first Command.
		 * Writing a value of 1 triggers the Error injection.
		 */
		uint32_t parity_err    : 1;
		/* Reserved. */
		uint32_t __reserved_2  : 3;
	};

	uint32_t word;
} EMC_DDR_ERR_INJ_t;


/*
 * ROW_HAMMER_STAT.
 * Row hammer events counter
 */

__extension__
typedef union {
	struct {
		/* Counts number of approaches to a hammered bank. */
		uint32_t counter : 32;
	};

	uint32_t word;
} EMC_ROW_HAMMER_STAT_t;


/*
 * ROW_HAMMER_STAT2.
 * Row hammer notification and latched address
 */

__extension__
typedef union {
	struct {
		/* Latch the first MC hammered row. */
		uint32_t row          : 18;
		/* Reserved. */
		uint32_t __reserved_0 : 2;
		/* Latch the first MC hammered bank. */
		uint32_t bank         : 4;
		/* Logical rank of the hammered row */
		uint32_t lrank        : 2;
		/* Package rank of the hammered row */
		uint32_t prank        : 2;
		/* Indicate whether a hammered row is found. */
		uint32_t mc_hammered  : 1;
		/* Reserved. */
		uint32_t __reserved_1 : 3;
	};

	uint32_t word;
} EMC_ROW_HAMMER_STAT2_t;


/*
 * ROW_HAMMER_TIME.
 * Row hammer algorithm T times
 */

__extension__
typedef union {
	struct {
		/*
		 * Counts T1_time in resolution of 9 bits.
		 * (~960 ns resolution for DDR3 - 2133)
		 * Tailored to 0x1FF in the code.
		 */
		uint32_t t1         : 8;
		/*
		 * Counts T2_time in resolution of 9 bits.
		 * (~960 ns resolution for DDR3 - 2133)
		 * Tailored to 0x1FF in the code.
		 */
		uint32_t t2         : 8;
		/*
		 * Counts T3_time in resolution of 9 bits.
		 * (~960 ns resolution for DDR3 - 2133)
		 * Tailored to 0x1FF in the code.
		 */
		uint32_t t3         : 8;
		/* Reserved. */
		uint32_t __reserved : 8;
	};

	uint32_t word;
} EMC_ROW_HAMMER_TIME_t;


/*
 * ROW_HAMMER_EVENT.
 * Row hammer algorithm E events
 */

__extension__
typedef union {
	struct {
		/*
		 * Counts E1_events in resolution of single event.
		 * (State B)
		 */
		uint32_t e1 : 8;
		/*
		 * Counts E2_events in resolution of single event.
		 * (State B)
		 */
		uint32_t e2 : 8;
		/*
		 * Counts E3_events in resolution of single event.
		 * (State B)
		 */
		uint32_t e3 : 8;
		/*
		 * Counts E4 events which are the added penalty events to the
		 * hammer counter.
		 * (State C)
		 */
		uint32_t e4 : 8;
	};

	uint32_t word;
} EMC_ROW_HAMMER_EVENT_t;


/*
 * ROW_HAMMER.
 * Row hammer algorithm general configurations
 */

__extension__
typedef union {
	struct {
		/*
		 * Hot button.
		 * Feature enable.
		 */
		uint32_t en              : 1;
		/*
		 * Determine the frequency of ruffling a probability to
		 * switch address
		 */
		uint32_t switch_addr_gap : 3;
		/*
		 * threshold that compared to LFSR result.
		 * if its larger than the LFSR, the address is switched.
		 */
		uint32_t switch_addr_th  : 16;
		/*
		 * The hammered bank's penalty.
		 * Resolution of single clock.
		 */
		uint32_t trc_offset      : 9;
		/*
		 * Hot button.
		 * Reset all row hammer mechanism, to start counting again.
		 */
		uint32_t reset           : 1;
		/*
		 * Decide whether to hammer all banks or just the captured
		 * bank
		 * 0 - Only captured bank
		 * 1 - All banks
		 */
		uint32_t all_banks       : 1;
		/*
		 * Hot button.
		 * Clears all rowReset hammer statistics.
		 */
		uint32_t clear_stat      : 1;
	};

	uint32_t word;
} EMC_ROW_HAMMER_t;


/*
 * ROW_HAMMER_ADDR_CHECK.
 * the suspicious checked address
 */

__extension__
typedef union {
	struct {
		/*
		 * 3:0 - Bank
		 * 5:4 - Logical Rank
		 * 7:6 - Physical Rank
		 * 25:8 - Row
		 */
		uint32_t vec        : 26;
		/* Reserved. */
		uint32_t __reserved : 6;
	};

	uint32_t word;
} EMC_ROW_HAMMER_ADDR_CHECK_t;


/*
 * ROW_HAMMER_ADDR_MASK.
 * per address bit counter masking
 */

__extension__
typedef union {
	struct {
		/*
		 * 3:0 - Bank
		 * 5:4 - Logical Rank
		 * 7:6 - Physical Rank
		 * 25:8 - Row
		 *
		 * 1 - not masked
		 * 0 - masked
		 */
		uint32_t vec        : 26;
		/* Reserved. */
		uint32_t __reserved : 6;
	};

	uint32_t word;
} EMC_ROW_HAMMER_ADDR_MASK_t;


/*
 * ROW_HAMMER_ADDR_CNTR.
 * Row counter enable
 */

__extension__
typedef union {
	struct {
		/* Enables Row Hammer Address Counter operation */
		uint32_t en         : 1;
		/* Reserved. */
		uint32_t __reserved : 31;
	};

	uint32_t word;
} EMC_ROW_HAMMER_ADDR_CNTR_t;


/*
 * ROW_HAMMER_STAT3.
 * row_hammer_addr_counter
 */

__extension__
typedef union {
	struct {
		/*
		 * the bit which control counter to reflect (MC0 or MC1) is
		 * located at register 0x52 bit 30
		 */
		uint32_t counter : 32;
	};

	uint32_t word;
} EMC_ROW_HAMMER_STAT3_t;


/*
 * DDR_MC_CLOCK_MEASUREMENT.
 * DDR Memory Controller Clocks Counter
 */

__extension__
typedef union {
	struct {
		/* Holds the value of the Memory Controller Clock count */
		uint32_t val : 32;
	};

	uint32_t word;
} EMC_DDR_MC_CLOCK_MEASUREMENT_t;


/*
 * RFRSH_TREFI_OFFS.
 * Holds Trefi offsets for each rank (logical and physical)
 */

__extension__
typedef union {
	struct {
		/* index*(Trefi/#of ranks) */
		uint32_t val_0        : 15;
		/* Reserved. */
		uint32_t __reserved_0 : 1;
		/* index*(Trefi/#of ranks) */
		uint32_t val_1        : 15;
		/* Reserved. */
		uint32_t __reserved_1 : 1;
	};

	uint32_t word;
} EMC_RFRSH_TREFI_OFFS_t;


/*
 * NVDIMM_FLUSH.
 * NVDIMM Flush Configuration Register
 */

__extension__
typedef union {
	struct {
		/*
		 * WatchDog counter value.
		 * The counter starts after disabling refresh and calibration
		 */
		uint32_t wd_val       : 20;
		/* Reserved. */
		uint32_t __reserved_0 : 4;
		/*
		 * Delay between setting Self Refresh and sending Done
		 * indication
		 */
		uint32_t done_delay   : 6;
		/* Reserved. */
		uint32_t __reserved_1 : 1;
		/*
		 * Enable/Disable NVDIMM Flush State Machine
		 * 0 - Disable
		 * 1 - Enable
		 */
		uint32_t en           : 1;
	};

	uint32_t word;
} EMC_NVDIMM_FLUSH_t;


/*
 * IFC_BIST_CFG.
 * memory controller BIST machines configurations
 */

__extension__
typedef union {
	struct {
		/*
		 * Number of repetotions:
		 * 00 - endless
		 * 01 - single
		 * 10 - run until error detected
		 * 11 - reserved
		 */
		uint32_t rep_num          : 2;
		/*
		 * 00 - data mode
		 * 01 - address mode
		 * 10 - reserved
		 * 11 - reserved
		 */
		uint32_t test_mode        : 2;
		/* Reserved. */
		uint32_t __reserved_0     : 1;
		/*
		 * Operand for address increment:
		 * 00 - 1;
		 * 01 - 16;
		 * 10 - 64;
		 * 11 - 128
		 */
		uint32_t inc_addr_operand : 2;
		/*
		 * Number of entries to be read from the SRAM. Valid values
		 * are 1-256. This is the highest entry +1 in the SRAM before
		 * wrap around). E.g. If SEQ_LENGTH=256, then the SRAM
		 * address is 255.
		 */
		uint32_t seq_len          : 9;
		/*
		 * 0- suspend
		 * 1 - active
		 */
		uint32_t wr_enable        : 1;
		/*
		 * 0 - suspend
		 * 1 - active
		 */
		uint32_t rd_enable        : 1;
		/*
		 * 0 - Change pattern each DRAM ack (move to next entry on
		 * pattern SRAM)
		 * 1 - Change pattern after configurable DRAM acks number
		 */
		uint32_t ptrn_mode        : 1;
		/*
		 * 0 - in order
		 * 1 - scrambled (with LFSR)
		 */
		uint32_t ptrn_seq         : 1;
		/*
		 * Bank and Address Scanning Mode:
		 * 00 - Logical Rank
		 * 01 - Physical Rank
		 * 10 - Address
		 * 11 - Random
		 */
		uint32_t scan_mode        : 2;
		/* Reserved. */
		uint32_t __reserved_1     : 10;
	};

	uint32_t word;
} EMC_IFC_BIST_CFG_t;


/*
 * IFC_BIST_BASE_ADDR.
 * lowest address for access by the BIST machine.
 */

__extension__
typedef union {
	struct {
		/* lowest address for access by the BIST machine. */
		uint32_t val : 32;
	};

	uint32_t word;
} EMC_IFC_BIST_BASE_ADDR_t;


/*
 * IFC_BIST_HIGH_ADDR.
 * Highest address for access by the BIST machine
 */

__extension__
typedef union {
	struct {
		/*
		 * Formulas for Data mode only:
		 * when PTRN Mode = 0
		 * Base address + [(number of active banks) x SEQ_length x
		 * INC_ADDR_OPERAND_VAL x N] - 1
		 *
		 * when PTRN Mode = 1
		 * Base address + [(number of active banks) x SEQ_length x
		 * INC_ADDR_OPERAND_VAL x N x PTRN_ACK_TH] - 1
		 *
		 * For address mode:
		 * Base address + N - 1
		 *
		 * N = integer > 1
		 * number of active banks = number_of_active_pranks *
		 * number_of_active_lranks * banks_per_dram.
		 */
		uint32_t val : 32;
	};

	uint32_t word;
} EMC_IFC_BIST_HIGH_ADDR_t;


/*
 * IFC_BIST_TIMING_PARAMS.
 * BIST parameters
 */

__extension__
typedef union {
	struct {
		/* Reserved. */
		uint32_t __reserved_0 : 8;
		/*
		 * Number of clocks between acknowledge and write address
		 * plus one.
		 */
		uint32_t ack_addr_gap : 5;
		/* Reserved. */
		uint32_t __reserved_1 : 3;
		/*
		 * Number of clocks between read acknowledge and write data
		 * minus one.
		 */
		uint32_t ack_data_gap : 5;
		/* Reserved. */
		uint32_t __reserved_2 : 11;
	};

	uint32_t word;
} EMC_IFC_BIST_TIMING_PARAMS_t;


/*
 * IFC_BIST_DIMM_RANGE.
 * Defines Minimum and Maximum for Logical Rank and Physical Rank
 */

__extension__
typedef union {
	struct {
		/* Minimum Logical Rank for scanning */
		uint32_t min_lr       : 2;
		/* Reserved. */
		uint32_t __reserved_0 : 2;
		/* Maximum Logical Rank for scanning */
		uint32_t max_lr       : 2;
		/* Reserved. */
		uint32_t __reserved_1 : 2;
		/* Minimum Physical Rank for scanning */
		uint32_t min_pr       : 2;
		/* Reserved. */
		uint32_t __reserved_2 : 2;
		/* Maximum Physical Rank for scanning */
		uint32_t max_pr       : 2;
		/* Reserved. */
		uint32_t __reserved_3 : 18;
	};

	uint32_t word;
} EMC_IFC_BIST_DIMM_RANGE_t;


/*
 * IFC_BIST_EN.
 */

__extension__
typedef union {
	struct {
		/*
		 * 0 - disable
		 * 1 - enable
		 */
		uint32_t val        : 1;
		/* Reserved. */
		uint32_t __reserved : 31;
	};

	uint32_t word;
} EMC_IFC_BIST_EN_t;


/*
 * IFC_BIST_STATUS.
 * BIST machine statistics register
 */

__extension__
typedef union {
	struct {
		/*
		 * 0 - test not ended
		 * 1 - test ended
		 */
		uint32_t end_of_test      : 1;
		/*
		 * 0 - read completed successfully
		 * 1 - read error
		 * (no meening on Tx only test)
		 */
		uint32_t test_status      : 1;
		/* Reserved. */
		uint32_t __reserved_0     : 2;
		/* Bank of first read error */
		uint32_t err_bank         : 4;
		/* Logical Rank of first read error */
		uint32_t err_lr           : 2;
		/* Physical Rank of first read error */
		uint32_t err_pr           : 2;
		/* The 36 bytes of the 72 where the Error happened. */
		uint32_t err_chunk        : 1;
		/* Reserved. */
		uint32_t __reserved_1     : 3;
		/* SRAM entry of 1st data chunk */
		uint32_t err_sram_entry_0 : 8;
		/* SRAM entry of 2nd data chunk */
		uint32_t err_sram_entry_1 : 8;
	};

	uint32_t word;
} EMC_IFC_BIST_STATUS_t;


/*
 * IFC_BIST_ERR_ADDR.
 * recent address of Bist machine read error
 */

__extension__
typedef union {
	struct {
		/* DRAM address of first read error. */
		uint32_t val : 32;
	};

	uint32_t word;
} EMC_IFC_BIST_ERR_ADDR_t;


/*
 * IFC_BIST_ERR_CNTR.
 * BIST machine accomulated errors
 */

__extension__
typedef union {
	struct {
		/* accomulated number of errors */
		uint32_t val : 32;
	};

	uint32_t word;
} EMC_IFC_BIST_ERR_CNTR_t;


/*
 * IFC_BIST_ERR_DATA.
 * recent data with error
 */

__extension__
typedef union {
	struct {
		/*
		 * Data of first rd error.
		 * reg 0x1AB holds b31:0.
		 * reg 0x1AC holds b63:32.
		 * reg 0x1AD holds b95:64.
		 * reg 0x1AE holds b127:96.
		 * reg 0x1AF holds b159:128.
		 * reg 0x1B0 holds b191:160.
		 * reg 0x1B1 holds b223:192.
		 * reg 0x1B2 holds b255:224.
		 * reg 0x1B3 holds b287:256.
		 * reg 0x1B4 holds b319:288.
		 * reg 0x1B5 holds b351:320.
		 * reg 0x1B6 holds b383:352.
		 * reg 0x1B7 holds b415:384.
		 * reg 0x1B8 holds b447:416.
		 * reg 0x1B9 holds b479:448.
		 * reg 0x1BA holds b511:480.
		 * reg 0x1BB holds b543:512.
		 * reg 0x1BC holds b575:544.
		 */
		uint32_t val : 32;
	};

	uint32_t word;
} EMC_IFC_BIST_ERR_DATA_t;


/*
 * IFC_BIST_MASK.
 * Which bits to mask while checking the returned data
 */

__extension__
typedef union {
	struct {
		/*
		 * 0 - masked
		 * 1 - not masked
		 */
		uint32_t bits : 32;
	};

	uint32_t word;
} EMC_IFC_BIST_MASK_t;


/*
 * IFC_BIST_ADDR_LFSR.
 * When SCAN_MODE = "11", This is the LFSR that randomize the Address and
 * Bank.
 */

__extension__
typedef union {
	struct {
		/* LFSR SEED */
		uint32_t seed       : 25;
		/* Reserved. */
		uint32_t __reserved : 7;
	};

	uint32_t word;
} EMC_IFC_BIST_ADDR_LFSR_t;


/*
 * IFC_BIST_RND_BITS.
 * When SCAN_MODE = "11", this registers sets the enable and offset of the
 * random bits
 */

__extension__
typedef union {
	struct {
		/*
		 * BANK MSB is needed from RND_BITS
		 * 0 - BANK MSB constant 0.
		 * 1 - BANK MSB taken from RND_BITS according to BANK_OFFS
		 */
		uint32_t bank_en      : 1;
		/* bit offset in RND_BITS */
		uint32_t bank_offs    : 3;
		/*
		 * Logical Rank LSB is needed from RND_BITS
		 * 0 - LR LSB constant 0.
		 * 1 - LR LSB taken from RND_BITS according to LR_0_OFFS
		 */
		uint32_t lr_0_en      : 1;
		/* bit offset in RND_BITS */
		uint32_t lr_0_offs    : 3;
		/*
		 * Logical Rank MSB is needed from RND_BITS
		 * 0 - LR MSB constant 0.
		 * 1 - LR MSB taken from RND_BITS according to LR_1_OFFS
		 */
		uint32_t lr_1_en      : 1;
		/* bit offset in RND_BITS */
		uint32_t lr_1_offs    : 3;
		/*
		 * Physical Rank LSB is needed from RND_BITS
		 * 0 - PR LSB constant 0.
		 * 1 - PR LSB taken from RND_BITS according to PR_0_OFFS
		 */
		uint32_t pr_0_en      : 1;
		/* bit offset in RND_BITS */
		uint32_t pr_0_offs    : 3;
		/*
		 * Physical Rank MSB is needed from RND_BITS
		 * 0 - PR MSB constant 0.
		 * 1 - PR MSB taken from RND_BITS according to PR_1_OFFS
		 */
		uint32_t pr_1_en      : 1;
		/* bit offset in RND_BITS */
		uint32_t pr_1_offs    : 3;
		/*
		 * Address bit 22 is need from RND_BITS
		 * 0 - Bit 22 is constant 0
		 * 1 - Bit 22 is taken from RND_BITS according to ADDR_22_OFFS
		 */
		uint32_t addr_22_en   : 1;
		/* bit offset in RND_BITS */
		uint32_t addr_22_offs : 3;
		/*
		 * Address bit 23 is need from RND_BITS
		 * 0 - Bit 23 is constant 0
		 * 1 - Bit 23 is taken from RND_BITS according to ADDR_23_OFFS
		 */
		uint32_t addr_23_en   : 1;
		/* bit offset in RND_BITS */
		uint32_t addr_23_offs : 3;
		/*
		 * Address bit 24 is need from RND_BITS
		 * 0 - Bit 24 is constant 0
		 * 1 - Bit 24 is taken from RND_BITS according to ADDR_24_OFFS
		 */
		uint32_t addr_24_en   : 1;
		/* bit offset in RND_BITS */
		uint32_t addr_24_offs : 3;
	};

	uint32_t word;
} EMC_IFC_BIST_RND_BITS_t;


/*
 * IFC_BIST_RND_BITS_LEN.
 * When SCAN_MODE = "11", this registers sets the maximum number for the
 * random bits
 */

__extension__
typedef union {
	struct {
		/*
		 * Maximum value of random bits when working with SCAN_MODE =
		 * 11.
		 */
		uint32_t val        : 8;
		/* Reserved. */
		uint32_t __reserved : 24;
	};

	uint32_t word;
} EMC_IFC_BIST_RND_BITS_LEN_t;


/*
 * IFC_BIST_RND_SEQ_LEN.
 * When SCAN_MODE = "11", this registers sets the number of DRAM commands
 * (Write/Read) for the test.
 */

__extension__
typedef union {
	struct {
		/*
		 * Number of DRAM commands per test when working in SCAN_MODE
		 * = 11.
		 */
		uint32_t val : 32;
	};

	uint32_t word;
} EMC_IFC_BIST_RND_SEQ_LEN_t;


/*
 * IFC_BIST_PTRN_ACK.
 * Defines acknowledge threshold to promote the pattern sequence
 */

__extension__
typedef union {
	struct {
		/*
		 * Valid only when
		 * PTRN Mode = 1 (register 0xA0 bit 18).
		 * Defines after how many reads to promote the address
		 * pattern.
		 */
		uint32_t th : 32;
	};

	uint32_t word;
} EMC_IFC_BIST_PTRN_ACK_t;


/*
 * IFC_BIST_PTRN_LFSR.
 * BIST patern LFSR
 */

__extension__
typedef union {
	struct {
		uint32_t seed : 32;
	};

	uint32_t word;
} EMC_IFC_BIST_PTRN_LFSR_t;


/*
 * BIST_CG.
 * BIST clock gate to save power when not used
 */

__extension__
typedef union {
	struct {
		/* clock gate enable (1 is enabled) */
		uint32_t en         : 1;
		/* Reserved. */
		uint32_t __reserved : 31;
	};

	uint32_t word;
} EMC_BIST_CG_t;

#endif /* !defined(__DOXYGEN__) */

#endif /* !defined(__ASSEMBLER__) */

#endif /* !defined(__REGS_EMC_H__) */
