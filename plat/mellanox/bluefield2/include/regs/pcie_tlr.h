#ifndef __REGS_PCIE_TLR_H__
#define __REGS_PCIE_TLR_H__

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif
#include "pcie_tlr_def.h"

#ifndef __ASSEMBLER__


#ifndef __DOXYGEN__

/*
 * MMIO Address Space.
 * The MMIO physical address space for the Pcie_tlr is described below.  This
 * is a general description of the MMIO space as opposed to a register
 * description
 */

__extension__
typedef union {
	struct {
		/*
		 * This field of the address provides an offset into the
		 * region being accessed.
		 */
		uint64_t offset     : 15;
		/*
		 * This field of the address selects the region to be
		 * accessed.
		 */
		uint64_t region     : 1;
		/* Reserved. */
		uint64_t __reserved : 48;
	};

	uint64_t word;
} PCIE_TLR_MMIO_ADDRESS_SPACE_t;


/*
 * SCRATCHPAD.
 * Scratchpad register
 */

__extension__
typedef union {
	struct {
		/* Scratchpad register */
		uint64_t scratchpad : 64;
	};

	uint64_t word;
} PCIE_TLR_SCRATCHPAD_t;


/*
 * TP0 Ring Node ID.
 * Ring node ID for TP0 block, used to direct PCIe packets on the ring.
 * MAX/MIN fields are used as a comparison range when decoding LinkID from
 * PXT.  Range values must not overlap another NODEID's range.  Set MAX/MIN
 * the same for single LinkID value.
 */

__extension__
typedef union {
	struct {
		/* Ring destination node ID */
		uint64_t nodeid       : 5;
		/* Reserved. */
		uint64_t __reserved_0 : 3;
		/* Min value for LinkID range comparison */
		uint64_t linkid_min   : 5;
		/* Max value for LinkID range comparison */
		uint64_t linkid_max   : 5;
		/* Reserved. */
		uint64_t __reserved_1 : 46;
	};

	uint64_t word;
} PCIE_TLR_TP0_RING_NODEID_t;


/*
 * TP1 Ring Node ID.
 * Ring node ID for TP1 block, used to direct PCIe packets on the ring.
 * MAX/MIN fields are used as a comparison range when decoding LinkID from
 * PXT.  Range values must not overlap another NODEID's range.  Set MAX/MIN
 * the same for single LinkID value.
 */

__extension__
typedef union {
	struct {
		/* Ring destination node ID */
		uint64_t nodeid       : 5;
		/* Reserved. */
		uint64_t __reserved_0 : 3;
		/* Min value for LinkID range comparison */
		uint64_t linkid_min   : 5;
		/* Max value for LinkID range comparison */
		uint64_t linkid_max   : 5;
		/* Reserved. */
		uint64_t __reserved_1 : 46;
	};

	uint64_t word;
} PCIE_TLR_TP1_RING_NODEID_t;


/*
 * HCA0 Ring Node ID.
 * Ring node ID for HCA0 block, used to direct PCIe packets on the ring.
 * MAX/MIN fields are used as a comparison range when decoding LinkID from
 * PXT.  Range values must not overlap another NODEID's range.  Set MAX/MIN
 * the same for single LinkID value.
 */

__extension__
typedef union {
	struct {
		/* Ring destination node ID */
		uint64_t nodeid       : 5;
		/* Reserved. */
		uint64_t __reserved_0 : 3;
		/* Min value for LinkID range comparison */
		uint64_t linkid_min   : 5;
		/* Max value for LinkID range comparison */
		uint64_t linkid_max   : 5;
		/* Reserved. */
		uint64_t __reserved_1 : 46;
	};

	uint64_t word;
} PCIE_TLR_HCA0_RING_NODEID_t;


/*
 * ARM LINK ID.
 * LinkID for ARM cores, used to decode incoming PXT packets. The ARM cores
 * are not accessed via the ring.
 */

__extension__
typedef union {
	struct {
		/* LinkID value for ARM */
		uint64_t linkid     : 5;
		/* Reserved. */
		uint64_t __reserved : 59;
	};

	uint64_t word;
} PCIE_TLR_ARM_LINKID_t;


/*
 * TLR2PXT interface credits.
 * Controls interface credits between TLR and PXT
 */

__extension__
typedef union {
	struct {
		/*
		 * Each credit represents a line in the PXT receive buffer.
		 * When written, this signed integer will be added to the
		 * credit count.  When read, returns the current credit value.
		 */
		uint64_t val        : 4;
		/* Reserved. */
		uint64_t __reserved : 60;
	};

	uint64_t word;
} PCIE_TLR_TLR2PXT_INTF_CREDITS_t;


/*
 * ARM TLP Packet IDs.
 * Programmable TLP RequesteID and CompleterID fields, used for packets sent
 * from ARM to PXT
 */

__extension__
typedef union {
	struct {
		/* RequesterID used for ARM packets */
		uint64_t req_id     : 16;
		/* CompleterID used for ARM completion packets */
		uint64_t cmpltr_id  : 16;
		/* Reserved. */
		uint64_t __reserved : 32;
	};

	uint64_t word;
} PCIE_TLR_ARM_TLP_PKT_ID_t;


/*
 * PKT Stop Mode Config.
 * TLR reponse mode to PXT packet stop request signal
 */

__extension__
typedef union {
	struct {
		/* Configure TLR response to PXT packet stop request. */
		uint64_t mode       : 1;
		/* Reserved. */
		uint64_t __reserved : 63;
	};

	uint64_t word;
} PCIE_TLR_PKT_STOP_CFG_t;


/*
 * SW Coherence Fence.
 * Coherence fence register. When enabled, sequence number of  packet write
 * is recorded.  HW will clear the enable when write with recorded sequence
 * number is completed.
 */

__extension__
typedef union {
	struct {
		/*
		 * When written, enables a coherence fence that records
		 * current packet sequence number. Cleared by HW, SW may poll
		 * this bit to determine when sequence number has been
		 * retired.
		 */
		uint64_t fence_enable : 1;
		/* Reserved. */
		uint64_t __reserved_0 : 15;
		/*
		 * Target sequence number recorded when fence enabled. Valid
		 * only when FENCE_ENABLE asserted.
		 */
		uint64_t fence_sqn    : 11;
		/* Reserved. */
		uint64_t __reserved_1 : 5;
		/* Last sequence number retired. */
		uint64_t retired_sqn  : 11;
		/* Reserved. */
		uint64_t __reserved_2 : 21;
	};

	uint64_t word;
} PCIE_TLR_COHERENCE_FENCE_t;


/*
 * Incoming Posted Packet Count.
 * Count register for incoming (from PXT) Posted Packets
 */

__extension__
typedef union {
	struct {
		/*
		 * Counts the number of Posted packets seen in this
		 * direction. Write to clear
		 */
		uint64_t pkt_count  : 56;
		/* Reserved. */
		uint64_t __reserved : 8;
	};

	uint64_t word;
} PCIE_TLR_IN_P_PKT_CNT_t;


/*
 * Incoming Non-Posted Packet Count.
 * Count register for incoming (from PXT) Non-Posted Packets
 */

__extension__
typedef union {
	struct {
		/*
		 * Counts the number of Non-Posted packets seen in this
		 * direction.  Write to clear.
		 */
		uint64_t pkt_count  : 56;
		/* Reserved. */
		uint64_t __reserved : 8;
	};

	uint64_t word;
} PCIE_TLR_IN_NP_PKT_CNT_t;


/*
 * Incoming C Packet Count.
 * Count register for incoming (from PXT) Completion Packets
 */

__extension__
typedef union {
	struct {
		/*
		 * Counts the number of Completion packets seen in this
		 * direction.  Write to clear.
		 */
		uint64_t pkt_count  : 56;
		/* Reserved. */
		uint64_t __reserved : 8;
	};

	uint64_t word;
} PCIE_TLR_IN_C_PKT_CNT_t;


/*
 * Outgoing Posted Packet Count.
 * Count register for outgoing (to PXT) Posted Packets
 */

__extension__
typedef union {
	struct {
		/*
		 * Counts the number of Posted packets seen in this
		 * direction.  Write to clear.
		 */
		uint64_t pkt_count  : 56;
		/* Reserved. */
		uint64_t __reserved : 8;
	};

	uint64_t word;
} PCIE_TLR_OUT_P_PKT_CNT_t;


/*
 * Outgoing Non-Posted Packet Count.
 * Count register for outgoing (to PXT) Non-Posted Packets
 */

__extension__
typedef union {
	struct {
		/*
		 * Counts the number of Non-Posted packets seen in this
		 * direction.  Write to clear.
		 */
		uint64_t pkt_count  : 56;
		/* Reserved. */
		uint64_t __reserved : 8;
	};

	uint64_t word;
} PCIE_TLR_OUT_NP_PKT_CNT_t;


/*
 * Outgoing C Packet Count.
 * Count register for outgoing (to PXT) Completion Packets
 */

__extension__
typedef union {
	struct {
		/*
		 * Counts the number of Completion packets seen in this
		 * direction.  Write to clear.
		 */
		uint64_t pkt_count  : 56;
		/* Reserved. */
		uint64_t __reserved : 8;
	};

	uint64_t word;
} PCIE_TLR_OUT_C_PKT_CNT_t;


/*
 * Incoming Posted Byte Count.
 * Count register for incoming (from PXT) Posted bytes
 */

__extension__
typedef union {
	struct {
		/*
		 * Counts the number of bytes associated with Posted packets
		 * seen in this direction.  Write to clear.
		 */
		uint64_t byte_count : 56;
		/* Reserved. */
		uint64_t __reserved : 8;
	};

	uint64_t word;
} PCIE_TLR_IN_P_BYTE_CNT_t;


/*
 * Incoming Non-Posted Byte Count.
 * Count register for incoming (from PXT) Non-Posted bytes
 */

__extension__
typedef union {
	struct {
		/* Reserved; this counter is not supported. */
		uint64_t byte_count : 56;
		/* Reserved. */
		uint64_t __reserved : 8;
	};

	uint64_t word;
} PCIE_TLR_IN_NP_BYTE_CNT_t;


/*
 * Incoming Completion Byte Count.
 * Count register for incoming (from PXT) Completion bytes
 */

__extension__
typedef union {
	struct {
		/*
		 * Counts the number of bytes associated with Completion
		 * packets seen in this direction.  Write to clear.
		 */
		uint64_t byte_count : 56;
		/* Reserved. */
		uint64_t __reserved : 8;
	};

	uint64_t word;
} PCIE_TLR_IN_C_BYTE_CNT_t;


/*
 * Outgoing Posted Byte Count.
 * Count register for outgoing (to PXT) Posted bytes
 */

__extension__
typedef union {
	struct {
		/*
		 * Counts the number of bytes associated with Posted packets
		 * seen in this direction.  Write to clear.
		 */
		uint64_t byte_count : 56;
		/* Reserved. */
		uint64_t __reserved : 8;
	};

	uint64_t word;
} PCIE_TLR_OUT_P_BYTE_CNT_t;


/*
 * Outgoing Non-Posted Byte Count.
 * Count register for outgoing (to PXT) Non-Posted bytes
 */

__extension__
typedef union {
	struct {
		/* Reserved; this counter is not supported. */
		uint64_t byte_count : 56;
		/* Reserved. */
		uint64_t __reserved : 8;
	};

	uint64_t word;
} PCIE_TLR_OUT_NP_BYTE_CNT_t;


/*
 * Outgoing Completion Byte Count.
 * Count register for outgoing (to PXT) Completion bytes
 */

__extension__
typedef union {
	struct {
		/*
		 * Counts the number of bytes associated with Completion
		 * packets seen in this direction.  Write to clear.
		 */
		uint64_t byte_count : 56;
		/* Reserved. */
		uint64_t __reserved : 8;
	};

	uint64_t word;
} PCIE_TLR_OUT_C_BYTE_CNT_t;


/*
 * TLR State Status.
 * Status register that captures current state of key elements in the PCIE_TLR
 */

__extension__
typedef union {
	struct {
		/* Empty status for incoming PXT packet fifo */
		uint64_t pxt_in_fifo_empty     : 1;
		/* Empty status for Ring Delay Buffer */
		uint64_t rdb_fifo_empty        : 1;
		/* Empty status for incoming TRIO packet fifo */
		uint64_t trio_in_fifo_empty    : 1;
		/* Empty status for incoming Ring packet fifo */
		uint64_t ring_in_fifo_empty    : 1;
		/* Empty status for outgoing PXT packet fifo */
		uint64_t pxt_out_fifo_empty    : 1;
		/*
		 * Indicator that there are no packets in flight in the TLR;
		 * based on TLR fifo empty status
		 */
		uint64_t tlr_no_pkts_in_flight : 1;
		/* Reserved. */
		uint64_t __reserved            : 58;
	};

	uint64_t word;
} PCIE_TLR_TLR_STATE_STATUS_t;


/*
 * TLR RX Sequence number status.
 * Status register that captures current sequence number states on RX (from
 * PXT) side
 */

__extension__
typedef union {
	struct {
		/* Last sequence number sent to TRIO */
		uint64_t last_trio_sqn    : 11;
		/* Reserved. */
		uint64_t __reserved_0     : 1;
		/* Last sequence number sent to Ring Delay Buffer (RDB) */
		uint64_t last_ring_sqn    : 11;
		/* Reserved. */
		uint64_t __reserved_1     : 1;
		/* Current sequence number of RDB output */
		uint64_t rdb_tx_sqn       : 11;
		/* RDB has valid commands */
		uint64_t rdb_not_empty    : 1;
		/*
		 * Current master sqn that gates sending RDB packets to ring
		 */
		uint64_t rdb_gate_sqn     : 11;
		/*
		 * Override: Override enforcement of sequence number check
		 * when transmitting packets onto ring. Ring packets will be
		 * considerd unordered and not reactive to or affecting ARM
		 * packets.  PCI ordering rules will be maintained for packet
		 * to ARM.
		 */
		uint64_t rx_ring_sqn_ovrd : 1;
		/*
		 * Override: Override enforcement of sequence number check
		 * when transmitting any packet. Packets recieved from PXT
		 * will be considerd unordered and not subject to sequence
		 * number checks.
		 */
		uint64_t rx_full_sqn_ovrd : 1;
		/* Reserved. */
		uint64_t __reserved_2     : 15;
	};

	uint64_t word;
} PCIE_TLR_TLR_RX_SQN_STATUS_t;


/*
 * TLR RX Sequence number status.
 * Status register that captures current sequence number states on TX (to
 * PXT) side
 */

__extension__
typedef union {
	struct {
		/* Sequence number of last TRIO packet up for arbitration */
		uint64_t last_trio_sqn    : 11;
		/* Indicates valid TRIO packet at arb */
		uint64_t trio_sqn_vld     : 1;
		/* Sequence number of last ring packet up for arbitration */
		uint64_t last_ring_sqn    : 11;
		/* Indicates valid ring packet at arb */
		uint64_t ring_sqn_vld     : 1;
		/*
		 * Current sequence number used to arb between TRIO/ring
		 * packets
		 */
		uint64_t tx_gate_sqn      : 11;
		/*
		 * Override: Override enforcement of sequence number check
		 * when transmitting any packet received from TRIO or RING.
		 * Ring and ARM packets will be considerd unordered and not
		 * subject to sequence number checks.
		 */
		uint64_t tx_full_sqn_ovrd : 1;
		/* Reserved. */
		uint64_t __reserved       : 28;
	};

	uint64_t word;
} PCIE_TLR_TLR_TX_SQN_STATUS_t;


/*
 * Part of Key to send to HCA.
 * Bits 63:0 of Key to send to HCA.
 */

__extension__
typedef union {
	struct {
		/* Key. */
		uint64_t value : 64;
	};

	uint64_t word;
} PCIE_TLR_HCA_KEY_0_t;


/*
 * Part of Key to send to HCA.
 * Bits 127:64 of Key to send to HCA.
 */

__extension__
typedef union {
	struct {
		/* Key. */
		uint64_t value : 64;
	};

	uint64_t word;
} PCIE_TLR_HCA_KEY_1_t;


/*
 * Part of Key to send to HCA.
 * Bits 191:128 of Key to send to HCA. Only used when HCA_KEY_CTL SIZE is 256
 * bits.
 */

__extension__
typedef union {
	struct {
		/* Key. */
		uint64_t value : 64;
	};

	uint64_t word;
} PCIE_TLR_HCA_KEY_2_t;


/*
 * Part of Key to send to HCA.
 * Bits 255:192 of Key to send to HCA. Only used when HCA_KEY_CTL SIZE is 256
 * bits.
 */

__extension__
typedef union {
	struct {
		/* Key. */
		uint64_t value : 64;
	};

	uint64_t word;
} PCIE_TLR_HCA_KEY_3_t;


/*
 * HCA Key Control.
 * Controls sending Key to HCA.
 */

__extension__
typedef union {
	struct {
		/*
		 * Write 1 to initiate sending key and accompanying control
		 * bits. This bit will clear upon completion of key sending
		 * and can be polled.
		 */
		uint64_t go           : 1;
		/* Key is valid. */
		uint64_t valid        : 1;
		/* ID value of the key. */
		uint64_t id           : 1;
		/* Reserved. */
		uint64_t __reserved_0 : 1;
		/* Size of the key being sent. */
		uint64_t size         : 1;
		/* Reserved. */
		uint64_t __reserved_1 : 59;
	};

	uint64_t word;
} PCIE_TLR_HCA_KEY_CTL_t;

#endif /* !defined(__DOXYGEN__) */

#endif /* !defined(__ASSEMBLER__) */

#endif /* !defined(__REGS_PCIE_TLR_H__) */
