#ifndef __REGS_OOB_H__
#define __REGS_OOB_H__

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif
#include "oob_def.h"

#ifndef __ASSEMBLER__


#ifndef __DOXYGEN__

/* Scratchpad. */

__extension__
typedef union {
	struct {
		uint64_t scratchpad : 64;
	};

	uint64_t word;
} OOB_SCRATCHPAD_t;


/*
 * TX work queue size log2.
 * log2 of the size of the work queue (number of entries), minimum value is
 * 2, maximum value is 8
 */

__extension__
typedef union {
	struct {
		/*
		 * TX work queue size log2.
		 * log2 of the size of the work queue (number of entries),
		 * minimum value is 2, maximum value is 8
		 */
		uint64_t tx_wq_size_log2 : 4;
		/* Reserved. */
		uint64_t __reserved      : 60;
	};

	uint64_t word;
} OOB_TX_WQ_SIZE_LOG2_t;


/*
 * TX work queue first address.
 * The first address of the work queue
 */

__extension__
typedef union {
	struct {
		/* Reserved. */
		uint64_t __reserved_0        : 12;
		/*
		 * TX work queue first address.
		 * The first address of the work queue
		 */
		uint64_t tx_wq_first_address : 28;
		/* Reserved. */
		uint64_t __reserved_1        : 24;
	};

	uint64_t word;
} OOB_TX_WQ_FIRST_ADDRESS_t;


/*
 * TX producer index.
 * SW updates this register when there are available wqes to read
 */

__extension__
typedef union {
	struct {
		/*
		 * TX producer index.
		 * SW updates this register when there are available wqes to
		 * read
		 */
		uint64_t tx_producer_index : 16;
		/* Reserved. */
		uint64_t __reserved        : 48;
	};

	uint64_t word;
} OOB_TX_PRODUCER_INDEX_t;


/*
 * TX consumer index.
 * HW updates this register when wqe was written to tx fifo
 */

__extension__
typedef union {
	struct {
		/*
		 * TX consumer index.
		 * HW updates this register when wqe was written to tx fifo
		 */
		uint64_t tx_consumer_index : 16;
		/* Reserved. */
		uint64_t __reserved        : 48;
	};

	uint64_t word;
} OOB_TX_CONSUMER_INDEX_t;


/*
 * TX control Register.
 * This is the control register, with a couple common register types.
 */

__extension__
typedef union {
	struct {
		/*
		 * Stop tx transmission, if in middle of packet, transmission
		 * will be stopped after eop
		 */
		uint64_t gracefully_stop_req : 1;
		/* Reserved. */
		uint64_t __reserved          : 63;
	};

	uint64_t word;
} OOB_TX_CONTROL_t;


/*
 * TX counters cycle number register.
 * The amount of cycles the counters will use to count
 */

__extension__
typedef union {
	struct {
		/*
		 * TX counters cycle number register.
		 * The amount of cycles the counters will use to count
		 */
		uint64_t tx_counters_cycles_number : 32;
		/* Reserved. */
		uint64_t __reserved                : 32;
	};

	uint64_t word;
} OOB_TX_COUNTERS_CYCLES_NUMBER_t;


/*
 * TX fifo out frames counter cycles number.
 * The number of frames exit from tx fifo in the configured cycles number
 */

__extension__
typedef union {
	struct {
		/*
		 * TX fifo out frames counter cycles number.
		 * The number of frames exit from tx fifo in the configured
		 * cycles number
		 */
		uint64_t tx_fifo_out_frames_counter : 32;
		/* Reserved. */
		uint64_t __reserved                 : 32;
	};

	uint64_t word;
} OOB_TX_FIFO_OUT_FRAMES_COUNTER_t;


/*
 * TX producer index equal to consumer index counter cycles number.
 * The number of cycles producer index equal to consumer index in the
 * configured cycles number
 */

__extension__
typedef union {
	struct {
		/*
		 * TX producer index equal to consumer index counter cycles
		 * number.
		 * The number of cycles producer index equal to consumer
		 * index in the configured cycles number
		 */
		uint64_t tx_producer_equal_consumer_counter : 32;
		/* Reserved. */
		uint64_t __reserved                         : 32;
	};

	uint64_t word;
} OOB_TX_PRODUCER_EQUAL_CONSUMER_COUNTER_t;


/*
 * TX producer index higher by 4 or more than consumer index counter cycles
 * number.
 * The number of cycles producer index higher by 4 or more than consumer
 * index in the configured cycles number
 */

__extension__
typedef union {
	struct {
		/*
		 * TX producer index higher by 4 or more than consumer index
		 * counter cycles number.
		 * The number of cycles producer index higher by 4 or more
		 * than consumer index in the configured cycles number
		 */
		uint64_t tx_producer_higher_by_4_or_more_than_consumer_counter : 32;
		/* Reserved. */
		uint64_t __reserved                                            : 32;
	};

	uint64_t word;
} OOB_TX_PRODUCER_HIGHER_BY_4_OR_MORE_THAN_CONSUMER_COUNTER_t;


/*
 * TX wq raddr not ready after valid counter cycles number.
 * The number of cycles tx wq raddr not ready after valid in the configured
 * cycles number
 */

__extension__
typedef union {
	struct {
		/*
		 * TX wq raddr not ready after valid counter cycles number.
		 * The number of cycles tx wq raddr not ready after valid in
		 * the configured cycles number
		 */
		uint64_t tx_wq_raddr_not_ready_after_valid_counter : 32;
		/* Reserved. */
		uint64_t __reserved                                : 32;
	};

	uint64_t word;
} OOB_TX_WQ_RADDR_NOT_READY_AFTER_VALID_COUNTER_t;


/*
 * TX data raddr not ready after valid counter cycles number.
 * The number of cycles tx data raddr not ready after valid in the configured
 * cycles number
 */

__extension__
typedef union {
	struct {
		/*
		 * TX data raddr not ready after valid counter cycles number.
		 * The number of cycles tx data raddr not ready after valid
		 * in the configured cycles number
		 */
		uint64_t tx_data_raddr_not_ready_after_valid_counter : 32;
		/* Reserved. */
		uint64_t __reserved                                  : 32;
	};

	uint64_t word;
} OOB_TX_DATA_RADDR_NOT_READY_AFTER_VALID_COUNTER_t;


/*
 * TX fifo in bytes counter.
 * The number of bytes enter to tx fifo in the configured cycles number
 */

__extension__
typedef union {
	struct {
		/*
		 * TX fifo in bytes counter.
		 * The number of bytes enter to tx fifo in the configured
		 * cycles number
		 */
		uint64_t tx_fifo_in_bytes_counter : 32;
		/* Reserved. */
		uint64_t __reserved               : 32;
	};

	uint64_t word;
} OOB_TX_FIFO_IN_BYTES_COUNTER_t;


/*
 * TX fifo in frames counter cycles number.
 * The number of frames enter to tx fifo in the configured cycles number
 */

__extension__
typedef union {
	struct {
		/*
		 * TX fifo in frames counter cycles number.
		 * The number of frames enter to tx fifo in the configured
		 * cycles number
		 */
		uint64_t tx_fifo_in_frames_counter : 32;
		/* Reserved. */
		uint64_t __reserved                : 32;
	};

	uint64_t word;
} OOB_TX_FIFO_IN_FRAMES_COUNTER_t;


/*
 * TX fifo out bytes counter.
 * The number of bytes exit from tx fifo in the configured cycles number
 */

__extension__
typedef union {
	struct {
		/*
		 * TX fifo out bytes counter.
		 * The number of bytes exit from tx fifo in the configured
		 * cycles number
		 */
		uint64_t tx_fifo_out_bytes_counter : 32;
		/* Reserved. */
		uint64_t __reserved                : 32;
	};

	uint64_t word;
} OOB_TX_FIFO_OUT_BYTES_COUNTER_t;


/*
 * TX adapter not ready after valid counter cycles number.
 * The number of cycles tx adapter not ready after valid in the configured
 * cycles number
 */

__extension__
typedef union {
	struct {
		/*
		 * TX adapter not ready after valid counter cycles number.
		 * The number of cycles tx adapter not ready after valid in
		 * the configured cycles number
		 */
		uint64_t tx_adapter_not_ready_after_valid_counter : 32;
		/* Reserved. */
		uint64_t __reserved                               : 32;
	};

	uint64_t word;
} OOB_TX_ADAPTER_NOT_READY_AFTER_VALID_COUNTER_t;


/*
 * TX errors Register.
 * This is the errors register.
 */

__extension__
typedef union {
	struct {
		/*
		 * producer index minus consumer index is larger than wq size
		 */
		uint64_t pi_minus_ci_larger_than_wq_size : 1;
		/* frame size is too small */
		uint64_t frame_size_small                : 1;
		/* checksum inputs were illegal */
		uint64_t checksum_inputs                 : 1;
		/* error in wq response */
		uint64_t wq_response                     : 2;
		/* error in data response */
		uint64_t data_response                   : 2;
		/* Reserved. */
		uint64_t __reserved                      : 57;
	};

	uint64_t word;
} OOB_TX_ERRORS_t;


/*
 * TX errors interrupts enable Register.
 * This is the errors interrupts enable register, decides which errors will
 * cause interrupt.
 */

__extension__
typedef union {
	struct {
		/*
		 * producer index minus consumer index is larger than wq size
		 */
		uint64_t pi_minus_ci_larger_than_wq_size : 1;
		/* frame size is too small */
		uint64_t frame_size_small                : 1;
		/* checksum inputs were illegal */
		uint64_t checksum_inputs                 : 1;
		/* error in wq response */
		uint64_t wq_response                     : 1;
		/* error in data response */
		uint64_t data_response                   : 1;
		/* Reserved. */
		uint64_t __reserved                      : 59;
	};

	uint64_t word;
} OOB_TX_ERRORS_INT_ENABLE_t;


/*
 * TX status Register.
 * This is the status register.
 */

__extension__
typedef union {
	struct {
		/* tx is disabled due to error */
		uint64_t disabled                  : 1;
		/* no space for new request in tx fifo */
		uint64_t data_fifo_full            : 1;
		/* there is flow control from mac */
		uint64_t mac_flow_control          : 1;
		/* the counters finished counting */
		uint64_t counters_finished         : 1;
		/* wq size log2 isn't between 2-8 */
		uint64_t wq_size_log2_out_of_range : 1;
		/* Reserved. */
		uint64_t __reserved                : 59;
	};

	uint64_t word;
} OOB_TX_STATUS_t;


/*
 * TX fifos status Register.
 * This is the tx fifos status register.
 */

__extension__
typedef union {
	struct {
		/* wqes fifo fullness */
		uint64_t wqes               : 8;
		/* read requests info fifo fullness */
		uint64_t read_requests_info : 5;
		/* data fifo fullness */
		uint64_t data               : 11;
		/* checksum info fifo fullness */
		uint64_t checksum_info      : 8;
		/* checksum result fifo fullness */
		uint64_t checksum_result    : 9;
		/* Reserved. */
		uint64_t __reserved         : 23;
	};

	uint64_t word;
} OOB_TX_FIFOS_STATUS_t;


/*
 * TX debug Register.
 * This is register for debug purpose.
 */

__extension__
typedef union {
	struct {
		/* tx won't disabled due to error */
		uint64_t no_disable : 1;
		/* Reserved. */
		uint64_t __reserved : 63;
	};

	uint64_t word;
} OOB_TX_DEBUG_t;


/*
 * TX consumer index update address.
 * The address of the tx consumer index update
 */

__extension__
typedef union {
	struct {
		/* Reserved. */
		uint64_t __reserved_0                     : 6;
		/*
		 * TX consumer index update address.
		 * The address of the tx consumer index update
		 */
		uint64_t tx_consumer_index_update_address : 34;
		/* Reserved. */
		uint64_t __reserved_1                     : 24;
	};

	uint64_t word;
} OOB_TX_CONSUMER_INDEX_UPDATE_ADDRESS_t;


/*
 * MAC id allowed to insert to device.
 * which mac id's to insert into the oob rx path
 */

__extension__
typedef union {
	struct {
		/* MAC ID which is enabled */
		uint64_t id         : 48;
		/* this filter is enabled. */
		uint64_t en         : 1;
		/* Reserved. */
		uint64_t __reserved : 15;
	};

	uint64_t word;
} OOB_RX_MAC_FILTER_t;


/*
 * MAC id allowed to insert to device.
 * which mac id's to insert into the oob rx path
 */

__extension__
typedef union {
	struct {
		/* MAC ID which is enabled */
		uint64_t id         : 48;
		/* this filter is enabled. */
		uint64_t en         : 1;
		/* Reserved. */
		uint64_t __reserved : 15;
	};

	uint64_t word;
} OOB_RX_MAC_FILTER_1_t;


/*
 * MAC id allowed to insert to device.
 * which mac id's to insert into the oob rx path
 */

__extension__
typedef union {
	struct {
		/* MAC ID which is enabled */
		uint64_t id         : 48;
		/* this filter is enabled. */
		uint64_t en         : 1;
		/* Reserved. */
		uint64_t __reserved : 15;
	};

	uint64_t word;
} OOB_RX_MAC_FILTER_2_t;


/*
 * MAC id allowed to insert to device.
 * which mac id's to insert into the oob rx path
 */

__extension__
typedef union {
	struct {
		/* MAC ID which is enabled */
		uint64_t id         : 48;
		/* this filter is enabled. */
		uint64_t en         : 1;
		/* Reserved. */
		uint64_t __reserved : 15;
	};

	uint64_t word;
} OOB_RX_MAC_FILTER_3_t;


/*
 * Filter by DMAC ranges configurations.
 * set of registers that define the range of dmac id's
 */

__extension__
typedef union {
	struct {
		/*
		 * Filter by DMAC ranges configurations.
		 * set of registers that define the range of dmac id's
		 */
		uint64_t rx_mac_filter_dmac_range_start : 48;
		/* Reserved. */
		uint64_t __reserved                     : 16;
	};

	uint64_t word;
} OOB_RX_MAC_FILTER_DMAC_RANGE_START_t;


/*
 * Filter by DMAC ranges configurations.
 * set of registers that define the range of dmac id's
 */

__extension__
typedef union {
	struct {
		/*
		 * Filter by DMAC ranges configurations.
		 * set of registers that define the range of dmac id's
		 */
		uint64_t rx_mac_filter_dmac_range_end : 48;
		/* Reserved. */
		uint64_t __reserved                   : 16;
	};

	uint64_t word;
} OOB_RX_MAC_FILTER_DMAC_RANGE_END_t;


/*
 * Filter by DMAC configurations.
 * set of registers that define rx filtering
 */

__extension__
typedef union {
	struct {
		/* enable the filtering by ranges */
		uint64_t en_range     : 1;
		/* enable multicast frames */
		uint64_t en_multicast : 1;
		/* Reserved. */
		uint64_t __reserved   : 62;
	};

	uint64_t word;
} OOB_RX_MAC_FILTER_GENERAL_t;


/*
 * counting configuration in case of discard.
 * which cases to count in case of discard event at the rx filter
 */

__extension__
typedef union {
	struct {
		/* enable discard counting (all mac id's) */
		uint64_t en           : 1;
		/* enable discard counting on a specific dmac ID */
		uint64_t dmac_en      : 1;
		/* Reserved. */
		uint64_t __reserved_0 : 2;
		/* the specific DMAC id to count it's discard events */
		uint64_t dmac_id      : 48;
		/* Reserved. */
		uint64_t __reserved_1 : 12;
	};

	uint64_t word;
} OOB_RX_MAC_FILTER_COUNT_DISC_t;


/*
 * counting configuration in case of pass.
 * which cases to count in case of pass event at the rx filter
 */

__extension__
typedef union {
	struct {
		/* enable pass counting (all mac id's) */
		uint64_t en           : 1;
		/* enable pass counting on a specific dmac ID */
		uint64_t dmac_en      : 1;
		/* Reserved. */
		uint64_t __reserved_0 : 2;
		/* the specific DMAC id to count it's pass events */
		uint64_t dmac_id      : 48;
		/* Reserved. */
		uint64_t __reserved_1 : 12;
	};

	uint64_t word;
} OOB_RX_MAC_FILTER_COUNT_PASS_t;


/*
 * counting all the pass packets through the RX filter.
 * count all the pass (not a specific DMAC)
 */

__extension__
typedef union {
	struct {
		/*
		 * counting all the pass packets through the RX filter.
		 * count all the pass (not a specific DMAC)
		 */
		uint64_t rx_mac_filter_pass_counter_all : 64;
	};

	uint64_t word;
} OOB_RX_MAC_FILTER_PASS_COUNTER_ALL_t;


/*
 * counting all the pass packets through the RX filter.
 * according to a specific DMAC ID from the configuration
 */

__extension__
typedef union {
	struct {
		/*
		 * counting all the pass packets through the RX filter.
		 * according to a specific DMAC ID from the configuration
		 */
		uint64_t rx_mac_filter_pass_counter_dmacid : 64;
	};

	uint64_t word;
} OOB_RX_MAC_FILTER_PASS_COUNTER_DMACID_t;


/*
 * counting all the disc packets through the RX filter.
 * count all the disc (not a specific DMAC)
 */

__extension__
typedef union {
	struct {
		/*
		 * counting all the disc packets through the RX filter.
		 * count all the disc (not a specific DMAC)
		 */
		uint64_t rx_mac_filter_disc_counter_all : 64;
	};

	uint64_t word;
} OOB_RX_MAC_FILTER_DISC_COUNTER_ALL_t;


/*
 * counting all the disc packets through the RX filter.
 * according to a specific DMAC ID from the configuration
 */

__extension__
typedef union {
	struct {
		/*
		 * counting all the disc packets through the RX filter.
		 * according to a specific DMAC ID from the configuration
		 */
		uint64_t rx_mac_filter_disc_counter_dmacid : 64;
	};

	uint64_t word;
} OOB_RX_MAC_FILTER_DISC_COUNTER_DMACID_t;


/*
 * global rx configuration.
 * collection of rx configuration
 */

__extension__
typedef union {
	struct {
		/* if to switch to little endian 1 means yes */
		uint64_t litend_switch : 1;
		/* enable removing CRC according to it's size */
		uint64_t strip_crc_en  : 1;
		/* Reserved. */
		uint64_t __reserved_0  : 2;
		/* CRC SIZE IN Bytes. upto 15Bytes */
		uint64_t crc_size      : 4;
		/* Reserved. */
		uint64_t __reserved_1  : 56;
	};

	uint64_t word;
} OOB_RX_t;


/*
 * counting configuration in case of pass.
 * which cases to count in case of pass event at the rx filter
 */

__extension__
typedef union {
	struct {
		/* enable pass counting (all mac id's) */
		uint64_t low          : 13;
		/* Reserved. */
		uint64_t __reserved_0 : 3;
		/* enable pass counting on a specific dmac ID */
		uint64_t high         : 13;
		/* Reserved. */
		uint64_t __reserved_1 : 35;
	};

	uint64_t word;
} OOB_RX_MAC_FC_TH_t;


/*
 * in stage buffering configuration.
 * which cases to count in case of pass event at the rx filter
 */

__extension__
typedef union {
	struct {
		/*
		 * the maximum fullness allowed in case new sop arrive. if
		 * above the frame will be discarded
		 */
		uint64_t max_fullness_at_sop   : 13;
		/* Reserved. */
		uint64_t __reserved_0          : 3;
		/*
		 * the maximum frame nmber allowed in the fifo to enter
		 * another new frame. if above will be deiscarded
		 */
		uint64_t max_framenum_at_sop   : 11;
		/* Reserved. */
		uint64_t __reserved_1          : 5;
		/* discard frame in case of truncation - no write to memory */
		uint64_t disc_err_trn          : 1;
		/* discard frame in case of truncation - no write to memory */
		uint64_t disc_err_mac          : 1;
		/* discard packet which is large than MTU */
		uint64_t disc_packet_too_large : 1;
		/* if packet is too large and not discarded - truncate it */
		uint64_t trn_packet_too_large  : 1;
		/* Reserved. */
		uint64_t __reserved_2          : 4;
		/*
		 * the maximum packet size allowed to enter into the chip.
		 * must be small than buffer size!
		 */
		uint64_t max_packet_size       : 14;
		/* Reserved. */
		uint64_t __reserved_3          : 10;
	};

	uint64_t word;
} OOB_RX_BUFFER_IN_t;


/*
 * DMA configurations.
 * collection of
 */

__extension__
typedef union {
	struct {
		/* enable dma to start writing new packets to memory */
		uint64_t en                    : 1;
		/*
		 * the wqe page size (pointer to) is 0 - 4KByte 1 - 64 KByte
		 */
		uint64_t wqe_page_size         : 1;
		/* Reserved. */
		uint64_t __reserved_0          : 2;
		/*
		 * continue new packet in new line and not at the next 16B in
		 * case of cache line os not full
		 */
		uint64_t sop_in_new_cache_line : 1;
		/* Reserved. */
		uint64_t __reserved_1          : 3;
		/*
		 * max on-the-fly wr operation allowed (before response is
		 * back) - for debug
		 */
		uint64_t max_otf_wr            : 15;
		/* Reserved. */
		uint64_t __reserved_2          : 1;
		/*
		 * how much cycles to wait if the number of ready CQES does
		 * not fill cache line
		 */
		uint64_t cqe_aggr_timer        : 8;
		/* Reserved. */
		uint64_t __reserved_3          : 32;
	};

	uint64_t word;
} OOB_RX_DMA_t;


/*
 * wqe producer index supplied by SW.
 * indicates in wrapping mode the last aloocated wqe page index
 */

__extension__
typedef union {
	struct {
		/*
		 * wqe producer index supplied by SW.
		 * indicates in wrapping mode the last aloocated wqe page
		 * index
		 */
		uint64_t rx_wqe_page_pi : 16;
		/* Reserved. */
		uint64_t __reserved     : 48;
	};

	uint64_t word;
} OOB_RX_WQE_PAGE_PI_t;


/*
 * wqe consumer supplied by by HW.
 * indicates in wrapping mode the last used WQE
 */

__extension__
typedef union {
	struct {
		/*
		 * wqe consumer supplied by by HW.
		 * indicates in wrapping mode the last used WQE
		 */
		uint64_t rx_wqe_page_alloc_ci : 16;
		/* Reserved. */
		uint64_t __reserved           : 48;
	};

	uint64_t word;
} OOB_RX_WQE_PAGE_ALLOC_CI_t;


/*
 * wqe consumer supplied by by HW.
 * indicates in wrapping mode the last used WQE
 */

__extension__
typedef union {
	struct {
		/*
		 * wqe consumer supplied by by HW.
		 * indicates in wrapping mode the last used WQE
		 */
		uint64_t rx_wqe_page_used_ci : 16;
		/* Reserved. */
		uint64_t __reserved          : 48;
	};

	uint64_t word;
} OOB_RX_WQE_PAGE_USED_CI_t;


/*
 * cqe consumer supplied by by HW - per ready packet on memory.
 * indicates in wrapping mode the last written CQE
 */

__extension__
typedef union {
	struct {
		/*
		 * cqe consumer supplied by by HW - per ready packet on
		 * memory.
		 * indicates in wrapping mode the last written CQE
		 */
		uint64_t rx_cqe_packet_ci : 64;
	};

	uint64_t word;
} OOB_RX_CQE_PACKET_CI_t;


/*
 * accumulative CQE wr sent to axi (counter inc before response is back).
 * how much cqes was sent to write (not finished the write!)
 */

__extension__
typedef union {
	struct {
		/*
		 * accumulative CQE wr sent to axi (counter inc before
		 * response is back).
		 * how much cqes was sent to write (not finished the write!)
		 */
		uint64_t rx_cqe_used_cntr : 64;
	};

	uint64_t word;
} OOB_RX_CQE_USED_CNTR_t;


/*
 * cqe wr transaction counter.
 * how much cqes wr transactions generated to axi CQE IF
 */

__extension__
typedef union {
	struct {
		/*
		 * cqe wr transaction counter.
		 * how much cqes wr transactions generated to axi CQE IF
		 */
		uint64_t rx_cqe_trans_cntr : 64;
	};

	uint64_t word;
} OOB_RX_CQE_TRANS_CNTR_t;


/*
 * wqe configurations.
 * set of configurations that define how to access to the WQE pointers
 */

__extension__
typedef union {
	struct {
		/*
		 * the wqe list size (as log2) - i.e. the list of wqe's size
		 * is 2^(SIZE_LOG2)
		 */
		uint64_t size_log2     : 4;
		/* Reserved. */
		uint64_t __reserved_0  : 2;
		/* the base address to access */
		uint64_t first_address : 34;
		/* Reserved. */
		uint64_t __reserved_1  : 24;
	};

	uint64_t word;
} OOB_RX_WQE_t;


/*
 * cqe configurations.
 * set of configurations that define how to access to the CQE pointers
 */

__extension__
typedef union {
	struct {
		/*
		 * the cqe list size (as log2) - i.e. the list of cqe's size
		 * is 2^(SIZE_LOG2)
		 */
		uint64_t size_log2       : 4;
		/* Reserved. */
		uint64_t __reserved_0    : 2;
		/* the base address to access */
		uint64_t first_address   : 34;
		/*
		 * what start value to write in the valid bit - at the first
		 * cycle of the CQE
		 */
		uint64_t valid_bit_start : 1;
		/* Reserved. */
		uint64_t __reserved_1    : 23;
	};

	uint64_t word;
} OOB_RX_CQE_t;


/*
 * the fetch unit prepare cqes/wqes page pointers has a limit how much to
 * fill in the fifo.
 * the fifo depth is 16, limit the maximal fullness as start of new request
 */

__extension__
typedef union {
	struct {
		/* the pre read wqe page-pointers */
		uint64_t wqe        : 7;
		/* Reserved. */
		uint64_t __reserved : 57;
	};

	uint64_t word;
} OOB_RX_QE_READY_MAX_FIFO_TH_t;


/*
 * mac related configurations.
 * the mac reset is controlloed by ARM for security (reset high)
 */

__extension__
typedef union {
	struct {
		/* clock number to count till force cr operation with error */
		uint64_t meter_maid_clocks_num : 8;
		/* global enable to all performance counter in the mac */
		uint64_t ir_perf_cnt_en        : 1;
		/* Reserved. */
		uint64_t __reserved            : 55;
	};

	uint64_t word;
} OOB_MAC_CFG_t;


/*
 * enable interrupt raising in rx oob - configuration.
 * each interrupt can be enabled (1) or disables (0)
 */

__extension__
typedef union {
	struct {
		uint64_t din_mac_err_passed    : 1;
		uint64_t din_trn_err_passed    : 1;
		uint64_t din_mac_err_discard   : 1;
		uint64_t din_trn_err_discard   : 1;
		uint64_t din_too_large_discard : 1;
		uint64_t din_trn_err_fatal     : 1;
		uint64_t dma_in_wresp          : 1;
		uint64_t dma_cqe_wresp         : 1;
		uint64_t dma_in_4k_bndr        : 1;
		uint64_t wqe_rd_resp_err       : 1;
		uint64_t llu_access_err        : 1;
		uint64_t plu_access_err        : 1;
		/* Reserved. */
		uint64_t __reserved            : 52;
	};

	uint64_t word;
} OOB_RX_INT_EN_t;


/*
 * counting all the pass packets through the RX filter.
 * count all the pass (not a specific DMAC)
 */

__extension__
typedef union {
	struct {
		/*
		 * the packet arrived with mac error - and passed (due to
		 * configuration)
		 */
		uint64_t din_mac_err_passed    : 1;
		/*
		 * there was a truncation but acording to configuration
		 * packet entered into chip
		 */
		uint64_t din_trn_err_passed    : 1;
		/* the packet artrived with mac error sign - and discarded */
		uint64_t din_mac_err_discard   : 1;
		/*
		 * interrupt in case a packet arrived with mac error to din
		 * stage and discarded
		 */
		uint64_t din_trn_err_discard   : 1;
		/*
		 * the accumulative size of the packet is too large - and due
		 * to that were discarded
		 */
		uint64_t din_too_large_discard : 1;
		/*
		 * there was a fatal error - no place to close packet at din
		 * stage
		 */
		uint64_t din_trn_err_fatal     : 1;
		/* the wdata with axi4 interfaces finished with error */
		uint64_t dma_in_wresp          : 1;
		/* the cqe write via axi4 interfaces finished with error */
		uint64_t dma_cqe_wresp         : 1;
		uint64_t dma_in_4k_bndr        : 1;
		uint64_t wqe_rd_resp_err       : 1;
		uint64_t llu_access_err        : 1;
		uint64_t plu_access_err        : 1;
		/* Reserved. */
		uint64_t __reserved            : 52;
	};

	uint64_t word;
} OOB_RX_INT_STATUS_t;


/*
 * to be removed.
 * none
 */

__extension__
typedef union {
	struct {
		/*
		 * to be removed.
		 * none
		 */
		uint64_t oob_rx_cqe_page_used_ci : 64;
	};

	uint64_t word;
} OOB_OOB_RX_CQE_PAGE_USED_CI_t;


/*
 * to be removed.
 * none
 */

__extension__
typedef union {
	struct {
		/*
		 * to be removed.
		 * none
		 */
		uint64_t oob_rx_cqe_page_pi : 64;
	};

	uint64_t word;
} OOB_OOB_RX_CQE_PAGE_PI_t;

#endif /* !defined(__DOXYGEN__) */

#endif /* !defined(__ASSEMBLER__) */

#endif /* !defined(__REGS_OOB_H__) */
