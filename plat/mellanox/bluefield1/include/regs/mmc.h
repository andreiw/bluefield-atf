#ifndef __REGS_MMC_H__
#define __REGS_MMC_H__

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif
#include "mmc_def.h"

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
} MMC_DEV_INFO_t;


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
} MMC_DEV_CTL_t;


/* Scratchpad. */

__extension__
typedef union {
	struct {
		/* Scratchpad. */
		uint64_t scratchpad : 64;
	};

	uint64_t word;
} MMC_SCRATCHPAD_t;


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
} MMC_SEMAPHORE0_t;


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
} MMC_DMA_STATUS_t;


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
} MMC_CLOCK_COUNT_t;


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
} MMC_INT_SETUP_t;


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
} MMC_FEATURE_CTL_t;


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
} MMC_CRED_CTL_t;


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
} MMC_SAM_CTL_t;


/*
 * CTRL.
 */

__extension__
typedef union {
	struct {
		/*
		 * 0-No change
		 *                                                  1-Reset
		 * DWC_mobile_storage controller
		 *                                                  To reset
		 * controller, firmware should set bit to 1. This bit is
		 * auto-cleared after two AHB and two cclk_in clock cycles.
		 *                                                  This
		 * resets:
		 *
		 * BIU/CIU interface
		 *
		 * CIU and state machines
		 *
		 * abort_read_data, send_irq_response, and read_wait bits of
		 * Control register
		 *
		 * start_cmd bit of Command register
		 *                                                  Does not
		 * affect any registers or DMA interface, or FIFO or host
		 *
		 * interrupts
		 */
		uint32_t controller_reset              : 1;
		/*
		 * 0-No change
		 *                                                  1-Reset
		 * to data FIFO To reset FIFO pointers
		 *                                                  To reset
		 * FIFO, firmware should set bit to 1. This bit is
		 * auto-cleared after completion of reset operation.
		 */
		uint32_t fifo_reset                    : 1;
		/*
		 * 0-No change
		 *                                                  1-Reset
		 * internal DMA interface control logic
		 *                                                  To reset
		 * DMA interface, firmware should set bit to 1. This bit is
		 * auto-cleared after two AHB clocks.
		 */
		uint32_t dma_reset                     : 1;
		/* Reserved. */
		uint32_t __reserved_0                  : 1;
		/*
		 * Global interrupt enable/disable bit:
		 *                                                  0-Disable
		 * interrupts
		 *                                                  1-Enable
		 * interrupts
		 *                                                  The int
		 * port is 1 only when this bit is 1 and one or more unmasked
		 * interrupts are set.
		 */
		uint32_t int_enable                    : 1;
		/*
		 * 0-Disable DMA transfer mode
		 *                                                  1-Enable
		 * DMA transfer mode
		 *                                                  Valid
		 * only if DWC_mobile_storage configured for External DMA
		 * interface.
		 */
		uint32_t dma_enable                    : 1;
		/*
		 * 0-Clear read wait
		 *                                                  1-Assert
		 * read wait
		 *                                                  For
		 * sending read-wait to SDIO cards.
		 */
		uint32_t read_wait                     : 1;
		/*
		 * 0-No Change in this
		 *                                                  1-Send
		 * auto IRQ response
		 *                                                  Bit
		 * automatically clears once response is sent.
		 * To wait for MMC card interrupts, host issues CMD40, and
		 * DWC_mobile_storage waits for interrupt response from MMC
		 * card(s). In meantime, if host wants DWC_mobile_storage to
		 * exit waiting for interrupt state, it can set this bit, at
		 * which time DWC_mobile_storage command state-machine sends
		 * CMD40 response on bus and returns to idle state.
		 */
		uint32_t send_irq_response             : 1;
		/*
		 * 0-No change
		 *                                                  1-After
		 * suspend command is issued during read-transfer, software
		 * polls card to find when suspend happened. Once suspend
		 * occurs,software sets bit to reset data state-machine,
		 * which is waiting for next block of data. Bit automatically
		 * clears once data statemachine resets to idle.
		 * Used in SDIO card suspend sequence.
		 */
		uint32_t abort_read_data               : 1;
		/*
		 * 0-Clear this bit if DWC_mobile_storage does not reset the
		 * bit
		 * 1-Send Command Completion Signal Disable (CCSD) to CE-ATA
		 * device
		 */
		uint32_t send_ccsd                     : 1;
		/*
		 * 0-Clear bit if DWC_mobile_storage does not reset the bit
		 *                                                  1-Send
		 * internally generated STOP after sending CCSD to
		 * CE-ATA device
		 */
		uint32_t send_auto_stop_ccsd           : 1;
		/*
		 * 0-Interrupts not enabled in CE-ATA device
		 *
		 * 1-Interrupts are enabled in CE-ATA device
		 */
		uint32_t ceata_device_interrupt_status : 1;
		/* Reserved. */
		uint32_t __reserved_1                  : 4;
		/*
		 * Card regulator-A voltage setting; output to card_volt_a
		 * port.
		 * Optional feature; ports can be used as general-purpose
		 * outputs
		 */
		uint32_t card_voltage_a                : 4;
		/*
		 * Card regulator-B voltage setting; output to card_volt_b
		 * port.
		 * Optional feature; ports can be used as general-purpose
		 * outputs
		 */
		uint32_t card_voltage_b                : 4;
		/*
		 * External open-drain pullup
		 *                                                  0-Disable
		 *                                                  1-Enable
		 *                                                  Inverted
		 * value of this bit is output to ccmd_od_pullup_en_n port.
		 * When bit is set, command output always driven in
		 * open-drive mode; that is, DWC_mobile_storage drives either
		 * 0 or high impedance, and does not drive hard 1.
		 */
		uint32_t enable_od_pullup              : 1;
		/*
		 * Present only for the Internal DMAC configuration; else, it
		 * is reserved.
		 *                                                 0-The host
		 * performs data transfers through the slave interface
		 *                                                 1-Internal
		 * DMAC used for data transfer
		 */
		uint32_t use_internal_dmac             : 1;
		/* Reserved. */
		uint32_t __reserved_2                  : 6;
	};

	uint32_t word;
} MMC_CTRL_t;


/*
 * PWREN.
 */

__extension__
typedef union {
	struct {
		/*
		 * Power on/off switch for up to 16 cards; for example,
		 * bit[0] controls card 0.Once power is turned on, firmware
		 * should wait for regulator/switch ramp-up time before
		 * trying to initialize card.
		 *                                                 0-power off
		 *                                                 1-power on
		 *                                                 Only
		 * NUM_CARDS number of bits are implemented.Bit values output
		 * to card_power_en port. Optional feature; ports can be used
		 * as general-purpose outputs.
		 */
		uint32_t power_enable : 2;
		/* Reserved. */
		uint32_t __reserved   : 30;
	};

	uint32_t word;
} MMC_PWREN_t;


/*
 * CLKDIV.
 */

__extension__
typedef union {
	struct {
		/*
		 * Clock divider-0 value. Clock division is 2*n. For example,
		 * value of 0 means divide by 2*0 = 0 (no division, bypass),
		 * value of 1 means divide by 2*1 = 2, value of ff means
		 * divide by 2*255 = 510, and so on.
		 */
		uint32_t clk_divider0 : 8;
		/*
		 * Clock divider-1 value. Clock division is 2*n. For example,
		 * value of 0 means divide by 2*0 = 0 (no division, bypass),
		 * value of 1 means divide by 2*1 = 2, value of ff means
		 * divide by 2*255 = 510, and so on. In MMC-Ver3.3-only mode,
		 * bits not implemented because only one clock divider is
		 * supported
		 */
		uint32_t clk_divider1 : 8;
		/*
		 * Clock divider-2 value. Clock division is 2*n. For example,
		 * value of 0 means divide by 2*0 = 0 (no division, bypass),
		 * value of 1 means divide by 2*1 = 2, value of ff means
		 * divide by 2*255 = 510, and so on. In MMC-Ver3.3-only mode,
		 * bits not implemented because only one clock divider is
		 * supported.
		 */
		uint32_t clk_divider2 : 8;
		/*
		 * Clock divider-3 value. Clock division is 2*n. For example,
		 * value of 0 means divide by 2*0 = 0 (no division, bypass),
		 * a value of 1 means divide by 2*1 = 2, a value of ff means
		 * divide by 2*255 = 510, and so on. In MMC-Ver3.3-only mode,
		 * bits not implemented because only one clock divider is
		 * supported.
		 */
		uint32_t clk_divider3 : 8;
	};

	uint32_t word;
} MMC_CLKDIV_t;


/*
 * CLKSRC.
 */

__extension__
typedef union {
	struct {
		/*
		 * Clock divider source for up to 16 SD cards supported. Each
		 * card has two bits assigned to it. For example, bits[1:0]
		 * assigned for card-0, which maps and internally routes
		 * clock divider[3:0] outputs to cclk_out[15:0] pins,
		 * depending on bit value.
		 *                                                  00  Clock
		 * divider 0
		 *                                                  01  Clock
		 * divider 1
		 *                                                  10  Clock
		 * divider 2
		 *                                                  11  Clock
		 * divider 3
		 * In MMC-Ver3.3-only controller, only one clock divider
		 * supported. The cclk_out is always from clock divider 0,
		 * and this register is not implemented.
		 */
		uint32_t clk_source : 32;
	};

	uint32_t word;
} MMC_CLKSRC_t;


/*
 * CLKENA.
 */

__extension__
typedef union {
	struct {
		/*
		 * Clock-enable control for up to 16 SD card clocks and one
		 * MMC card clock supported.
		 *                                                  0-Clock
		 * disabled
		 *                                                  1-Clock
		 * enabled
		 * In MMC-Ver3.3-only mode, since there is only one cclk_out,
		 * only cclk_enable[0] is used.
		 */
		uint32_t cclk_enable    : 16;
		/*
		 * Low-power control for up to 16 SD card clocks and one MMC
		 * card clock supported.
		 *
		 * 0-Non-low-power mode
		 *
		 * 1-Low-power mode; stop clock when card in IDLE (should be
		 * normally set to only MMC and SD memory cards; for SDIO
		 * cards, if interrupts must be detected, clock should not be
		 * stopped).
		 * In MMC-Ver3.3-only mode, since there is only one cclk_out,
		 * only cclk_low_power[0] is used.
		 */
		uint32_t cclk_low_power : 16;
	};

	uint32_t word;
} MMC_CLKENA_t;


/*
 * TMOUT.
 */

__extension__
typedef union {
	struct {
		/*
		 * Response timeout value.
		 * Value is in number of card output clocks cclk_out.
		 */
		uint32_t response_timeout : 8;
		/*
		 * Value for card Data Read Timeout; same value also used for
		 * Data
		 * Starvation by Host timeout. The timeout counter is started
		 * only after the card clock is stopped.Value is in number of
		 * card output clocks  cclk_out of selected card.
		 * Note: The software timer should be used if the timeout
		 * value is in the order
		 * of 100 ms. In this case, read data timeout interrupt needs
		 * to be disabled.
		 */
		uint32_t data_timeout     : 24;
	};

	uint32_t word;
} MMC_TMOUT_t;


/*
 * CTYPE.
 */

__extension__
typedef union {
	struct {
		/*
		 * One bit per card indicates if card is 1-bit or 4-bit:
		 *                                                  0-1-bit
		 * mode
		 *                                                  1-4-bit
		 * mode
		 * Bit[15] corresponds to card[15], bit[0] corresponds to
		 * card[0].
		 * Only NUM_CARDS*2 number of bits are implemented.
		 */
		uint32_t card_width2  : 2;
		/* Reserved. */
		uint32_t __reserved_0 : 14;
		/*
		 * One bit per card indicates if card is 8-bit:
		 *                                                  0-Non
		 * 8-bit mode
		 *                                                  1-8-bit
		 * mode
		 * Bit[31] corresponds to card[15]; bit[16] corresponds to
		 * card[0].
		 */
		uint32_t card_width1  : 2;
		/* Reserved. */
		uint32_t __reserved_1 : 14;
	};

	uint32_t word;
} MMC_CTYPE_t;


/*
 * BLKSIZ.
 */

__extension__
typedef union {
	struct {
		/* Block size */
		uint32_t block_size : 16;
		/* Reserved. */
		uint32_t __reserved : 16;
	};

	uint32_t word;
} MMC_BLKSIZ_t;


/*
 * BYTCNT.
 */

__extension__
typedef union {
	struct {
		/*
		 * Number of bytes to be transferred; should be integer
		 * multiple of Block Size for block transfers.
		 * For undefined number of byte transfers, byte count should
		 * be set to 0. When byte count is set to 0, it is
		 * responsibility of host to explicitly send stop/abort
		 * command to terminate data transfer.
		 */
		uint32_t byte_count : 32;
	};

	uint32_t word;
} MMC_BYTCNT_t;


/*
 * INTMASK.
 */

__extension__
typedef union {
	struct {
		/*
		 * Bits used to mask unwanted interrupts. Value of 0 masks
		 * interrupt; value of 1 enables interrupt.
		 *                                                  bit
		 * 15-End-bit error (read)/Write no CRC (EBE)
		 *                                                  bit
		 * 14-Auto command done (ACD)
		 *                                                  bit
		 * 13-Start Bit Error(SBE)/Busy Complete Interrupt (BCI)
		 *                                                  bit
		 * 12-Hardware locked write error (HLE)
		 *                                                  bit
		 * 11-FIFO underrun/overrun error (FRUN)
		 *                                                  bit
		 * 10-Data starvation-by-host timeout (HTO) /Volt_switch_int
		 *                                                  bit 9
		 * -Data read timeout (DRTO)
		 *                                                  bit 8
		 * -Response timeout (RTO)
		 *                                                  bit 7
		 * -Data CRC error (DCRC)
		 *                                                  bit 6
		 * -Response CRC error (RCRC)
		 *                                                  bit 5
		 * -Receive FIFO data request (RXDR)
		 *                                                  bit 4
		 * -Transmit FIFO data request (TXDR)
		 *                                                  bit 3
		 * -Data transfer over (DTO)
		 *                                                  bit 2
		 * -Command done (CD)
		 *                                                  bit 1
		 * -Response error (RE)
		 *                                                  bit 0
		 * -Card detect (CD)
		 */
		uint32_t int_mask      : 16;
		/*
		 * Mask SDIO interrupts
		 * One bit for each card. Bit[31] corresponds to card[15],
		 * and bit[16] corresponds to card[0]. When masked, SDIO
		 * interrupt detection for that card is disabled. A 0 masks
		 * an interrupt, and 1 enables an interrupt.
		 * In MMC-Ver3.3-only mode, these bits are always 0.
		 */
		uint32_t sdio_int_mask : 16;
	};

	uint32_t word;
} MMC_INTMASK_t;


/*
 * CMDARG.
 */

__extension__
typedef union {
	struct {
		/* Value indicates command argument to be passed to card */
		uint32_t cmd_arg : 32;
	};

	uint32_t word;
} MMC_CMDARG_t;


/*
 * CMD.
 */

__extension__
typedef union {
	struct {
		/* Command index */
		uint32_t cmd_index                   : 6;
		/*
		 * 0-No response expected from card
		 *
		 * 1-Response expected from card
		 */
		uint32_t response_expect             : 1;
		/*
		 * 0-Short response expected from card
		 *                                                  1-Long
		 * response expected from card
		 */
		uint32_t response_length             : 1;
		/*
		 * 0-Do not check response CRC
		 *                                                  1-Check
		 * response CRC
		 * Some of command responses do not return valid CRC bits.
		 * Software should disable CRC checks for those commands in
		 * order to disable CRC checking by controller.
		 */
		uint32_t check_response_crc          : 1;
		/*
		 * 0-No data transfer expected (read/write)
		 *                                                  1-Data
		 * transfer expected (read/write)
		 */
		uint32_t data_expected               : 1;
		/*
		 * 0-Read from card
		 *                                                  1-Write
		 * to card
		 *                                                  Dont care
		 * if no data expected from card.
		 */
		uint32_t read_write                  : 1;
		/*
		 * 0-Block data transfer command
		 *                                                  1-Stream
		 * data transfer command
		 *                                                  Dont care
		 * if no data expected.
		 */
		uint32_t transfer_mode               : 1;
		/*
		 * 0-No stop command sent at end of data transfer
		 *                                                  1-Send
		 * stop command at end of data transfer
		 * When set, DWC_mobile_storage sends stop command to
		 * SD_MMC_CEATA cards at end of data transfer.
		 *
		 * when send_auto_stop bit should be set, since some data
		 *    transfers do not need explicit stop commands
		 *
		 * open-ended transfers that software should explicitly send
		 * to
		 *    stop command
		 *
		 * Additionally, when resume is sent to resume  suspended
		 * memory access of SD-Combo card  bit should be set
		 * correctly if suspended data transfer needs
		 * send_auto_stop.Don't care if no data expected from card.
		 */
		uint32_t send_auto_stop              : 1;
		/*
		 * 0-Send command at once, even if previous data transfer has
		 * not
		 *
		 * completed
		 *                                                  1-Wait
		 * for previous data transfer completion before sending
		 *                                                    command
		 * The wait_prvdata_complete = 0 option typically used to
		 * query status of card during data transfer or to stop
		 * current data transfer;card_number should be same as in
		 * previous command.
		 */
		uint32_t wait_prvdata_complete       : 1;
		/*
		 * 0-Neither stop nor abort command to stop current data
		 * transfer
		 *                                                    in
		 * progress. If abort is sent to function-number currently
		 *
		 * selected or not in data-transfer mode, then bit should be
		 * set
		 *                                                    to 0.
		 *                                                  1-Stop or
		 * abort command intended to stop current data transfer
		 *                                                    in
		 * progress.
		 * When open-ended or predefined data transfer is in
		 * progress, and host issues stop or abort command to stop
		 * data transfer, bit should be set so that command/data
		 * state-machines of CIU can return correctly to idle state.
		 * This is also applicable for Boot mode transfers. To Abort
		 * boot mode, this bit should be set along with CMD[26] =
		 * disable_boot.
		 */
		uint32_t stop_abort_cmd              : 1;
		/*
		 * 0-Do not send initialization sequence (80 clocks of 1)
		 * before
		 *                                                    sending
		 * this command
		 *                                                  1-Send
		 * initialization sequence before sending this command
		 * After power on, 80 clocks must be sent to card for
		 * initialization before sending any commands to card. Bit
		 * should be set while sending first command to card so that
		 * controller will initialize clocks before sending command
		 * to card. This bit should not be set for either of the boot
		 * modes (alternate or mandatory).
		 */
		uint32_t send_initialization         : 1;
		/*
		 * Card number in use. Represents physical slot number of
		 * card being
		 * accessed. In MMC-Ver3.3-only mode, up to 30 cards are
		 * supported; in SD-only mode, up to 16 cards are supported.
		 * Registered version of this is reflected on dw_dma_card_num
		 * and ge_dma_card_num ports, which can be used to create
		 * separate DMA requests, if needed.
		 * In addition, in SD mode this is used to mux or demux
		 * signals from selected card because each card is interfaced
		 * to
		 * DWC_mobile_storage by separate bus.
		 */
		uint32_t card_number                 : 5;
		/*
		 * 0-Normal command sequence
		 *                                                  1-Do not
		 * send commands, just update clock register value into
		 *                                                    card
		 * clock domain
		 * Following register values transferred into card clock
		 * domain: CLKDIV, CLRSRC, CLKENA. Changes card clocks
		 * (change frequency, truncate off or on, and set
		 * low-frequency mode); provided in order to change clock
		 * frequency or stop clock without having to send command to
		 * cards.During normal command sequence, when
		 * update_clock_registers_only = 0, following control
		 * registers are transferred from BIU to CIU: CMD, CMDARG,
		 * TMOUT, CTYPE,BLKSIZ, BYTCNT. CIU uses new register values
		 * for new command sequence to card(s).When bit is set, there
		 * are no Command Done interrupts because no
		 * command is sent to SD_MMC_CEATA cards.
		 */
		uint32_t update_clock_registers_only : 1;
		/*
		 * 0-Host is not performing read access (RW_REG or RW_BLK)
		 *                                                    towards
		 * CE-ATA device
		 *                                                  1-Host is
		 * performing read access (RW_REG or RW_BLK)
		 *                                                    towards
		 * CE-ATA device
		 * Software should set this bit to indicate that CE-ATA
		 * device is being accessed for read transfer. This bit is
		 * used to disable read data timeout indication while
		 * performing CE-ATA read transfers.Maximum value of I/O
		 * transmission delay can be no less than 10 seconds.
		 * DWC_mobile_storage should not indicate read data timeout
		 * while waiting for data from CE-ATA device.
		 */
		uint32_t read_ceata_device           : 1;
		/*
		 * 0-Interrupts are not enabled in CE-ATA device (nIEN = 1 in
		 *                                                    ATA
		 * control register), or command does not expect CCS
		 *                                                    from
		 * device
		 *
		 * 1-Interrupts are enabled in CE-ATA device (nIEN = 0), and
		 * RW_BLK
		 *                                                    command
		 * expects command completion signal from CE-ATA device
		 * If the command expects Command Completion Signal (CCS)
		 * from the CE-ATA device, the software should set this
		 * control bit.DWC_mobile_storage sets Data Transfer Over
		 * (DTO) bit in RINTSTS register and generates interrupt to
		 * host if Data Transfer Over interrupt is not masked.
		 */
		uint32_t ccs_expected                : 1;
		/*
		 * Enable Bootthis bit should be set only for mandatory boot
		 * mode.
		 * When Software sets this bit along with start_cmd, CIU
		 * starts the boot sequence for the corresponding card by
		 * asserting the CMD line low. Do NOT set disable_boot and
		 * enable_boot together.
		 */
		uint32_t enable_boot                 : 1;
		/*
		 * Expect Boot Acknowledge. When Software sets this bit along
		 * with
		 * enable_boot, CIU expects a boot acknowledge start pattern
		 * of 0-1-0 from the selected card.
		 */
		uint32_t expect_boot_ack             : 1;
		/*
		 * Disable Boot. When software sets this bit along with
		 * start_cmd, CIU terminates the boot operation. Do NOT set
		 * disable_boot and enable_boot together.
		 */
		uint32_t disable_boot                : 1;
		/*
		 * Boot Mode
		 *                                                  0 -
		 * Mandatory Boot operation
		 *                                                  1 -
		 * Alternate Boot operation
		 */
		uint32_t boot_mode                   : 1;
		/*
		 * Voltage switch bit
		 *                                                  0 - No
		 * voltage switching
		 *                                                  1 -
		 * Voltage switching enabled; must be set for CMD11 only
		 */
		uint32_t volt_switch                 : 1;
		/*
		 * Use Hold Register
		 *                                                  0 - CMD
		 * and DATA sent to card bypassing HOLD Register
		 *                                                  1 - CMD
		 * and DATA sent to card through the HOLD Register
		 */
		uint32_t use_hold_reg                : 1;
		/* Reserved. */
		uint32_t __reserved                  : 1;
		/*
		 * Start command. Once command is taken by CIU, bit is
		 * cleared.
		 * When bit is set, host should not attempt to write to any
		 * command registers. If write is attempted, hardware lock
		 * error is set in raw interrupt register.
		 * Once command is sent and response is received from
		 * SD_MMC_CEATA cards, Command Done bit is set in raw
		 * interrupt
		 * register.
		 */
		uint32_t start_cmd                   : 1;
	};

	uint32_t word;
} MMC_CMD_t;


/*
 * RESP0.
 */

__extension__
typedef union {
	struct {
		/* Bit[31:0] of response */
		uint32_t response0 : 32;
	};

	uint32_t word;
} MMC_RESP0_t;


/*
 * RESP1.
 */

__extension__
typedef union {
	struct {
		/*
		 * Register represents bit[63:32] of long response.
		 * When CIU sends auto-stop command, then response is saved
		 * in register. Response for previous command sent by host is
		 * still preserved in Response 0 register. Additional
		 * auto-stop issued only for data transfer commands, and
		 * response type is always short for them.
		 */
		uint32_t response1 : 32;
	};

	uint32_t word;
} MMC_RESP1_t;


/*
 * RESP2.
 */

__extension__
typedef union {
	struct {
		/* Bit[95:64] of long response */
		uint32_t response2 : 32;
	};

	uint32_t word;
} MMC_RESP2_t;


/*
 * RESP3.
 */

__extension__
typedef union {
	struct {
		/* Bit[127:96] of long response */
		uint32_t response3 : 32;
	};

	uint32_t word;
} MMC_RESP3_t;


/*
 * MINTSTS.
 */

__extension__
typedef union {
	struct {
		/*
		 * Interrupt enabled only if corresponding bit in interrupt
		 * mask register is set.
		 *                                                  bit 15
		 * End-bit error (read)/write no CRC (EBE)
		 *                                                  bit 14
		 * Auto command done (ACD)
		 *                                                  bit 13
		 * Start Bit Error(SBE)/Busy Complete Interrupt (BCI)
		 *                                                  bit 12
		 * Hardware locked write error (HLE)
		 *                                                  bit 11
		 * FIFO underrun/overrun error (FRUN)
		 *                                                  bit 10
		 * Data starvation by host timeout (HTO)/Volt_switch_int
		 *                                                  bit 9
		 * Data read timeout (DRTO)
		 *                                                  bit 8
		 * Response timeout (RTO)
		 *                                                  bit 7
		 * Data CRC error (DCRC)
		 *                                                  bit 6
		 * Response CRC error (RCRC)
		 *                                                  bit 5
		 * Receive FIFO data request (RXDR)
		 *                                                  bit 4
		 * Transmit FIFO data request (TXDR)
		 *                                                  bit 3
		 * Data transfer over (DTO)
		 *                                                  bit 2
		 * Command done (CD)
		 *                                                  bit 1
		 * Response error (RE)
		 *                                                  bit 0
		 * Card detect (CD)
		 */
		uint32_t int_status     : 16;
		/*
		 * Interrupt from SDIO card; one bit for each card. Bit[31]
		 * corresponds to Card[15], and bit[16] is for Card[0]. SDIO
		 * interrupt for card enabled only if corresponding
		 * sdio_int_mask bit is set in Interrupt mask register (mask
		 * bit 1 enables interrupt; 0 masks interrupt).
		 *                                                  0-No SDIO
		 * interrupt from card
		 *                                                  1-SDIO
		 * interrupt from card
		 * In MMC-Ver3.3-only mode, bits always 0.
		 */
		uint32_t sdio_interrupt : 16;
	};

	uint32_t word;
} MMC_MINTSTS_t;


/*
 * RINTSTS.
 */

__extension__
typedef union {
	struct {
		/*
		 * Writes to bits clear status bit. Value of 1 clears status
		 * bit, and value of 0 leaves bit intact. Bits are logged
		 * regardless of interrupt mask status.
		 *                                                  bit 15
		 * End-bit error (read)/write no CRC (EBE)
		 *                                                  bit 14
		 * Auto command done (ACD)
		 *                                                  bit 13
		 * Start Bit Error(SBE)/Busy Complete Interrupt (BCI)
		 *                                                  bit 12
		 * Hardware locked write error (HLE)
		 *                                                  bit 11
		 * FIFO underrun/overrun error (FRUN)
		 *                                                  bit 10
		 * Data starvation by host timeout (HTO)/Volt_switch_int
		 *                                                  bit 9
		 * Data read timeout (DRTO)
		 *                                                  bit 8
		 * Response timeout (RTO)
		 *                                                  bit 7
		 * Data CRC error (DCRC)
		 *                                                  bit 6
		 * Response CRC error (RCRC)
		 *                                                  bit 5
		 * Receive FIFO data request (RXDR)
		 *                                                  bit 4
		 * Transmit FIFO data request (TXDR)
		 *                                                  bit 3
		 * Data transfer over (DTO)
		 *                                                  bit 2
		 * Command done (CD)
		 *                                                  bit 1
		 * Response error (RE)
		 *                                                  bit 0
		 * Card detect (CD)
		 */
		uint32_t int_status     : 16;
		/*
		 * Interrupt from SDIO card; one bit for each card. Bit[31]
		 * corresponds to Card[15], and bit[16] is for Card[0].
		 * Writes to these bits clear them. Value of 1 clears bit and
		 * 0 leaves bit intact.
		 *                                                  0-No SDIO
		 * interrupt from card
		 *                                                  1-SDIO
		 * interrupt from card
		 * In MMC-Ver3.3-only mode, bits always 0.
		 * Bits are logged regardless of interrupt-mask status.
		 */
		uint32_t sdio_interrupt : 16;
	};

	uint32_t word;
} MMC_RINTSTS_t;


/*
 * STATUS.
 */

__extension__
typedef union {
	struct {
		/*
		 * FIFO reached Receive watermark level; not qualified with
		 * data
		 * transfer.
		 */
		uint32_t fifo_rx_watermark  : 1;
		/*
		 * FIFO reached Transmit watermark level; not qualified with
		 * data
		 * transfer.
		 */
		uint32_t fifo_tx_watermark  : 1;
		/* FIFO is empty status */
		uint32_t fifo_empty         : 1;
		/* FIFO is full status */
		uint32_t fifo_full          : 1;
		/*
		 * Command FSM states:
		 *                                                  0  Idle
		 *                                                  1  Send
		 * init sequence
		 *                                                  2  Tx cmd
		 * start bit
		 *                                                  3  Tx cmd
		 * tx bit
		 *                                                  4  Tx cmd
		 * index + arg
		 *                                                  5  Tx cmd
		 * crc7
		 *                                                  6  Tx cmd
		 * end bit
		 *                                                  7  Rx
		 * resp start bit
		 *                                                  8  Rx
		 * resp IRQ response
		 *                                                  9  Rx
		 * resp tx bit
		 *                                                  10  Rx
		 * resp cmd idx
		 *                                                  11  Rx
		 * resp data
		 *                                                  12  Rx
		 * resp crc7
		 *                                                  13  Rx
		 * resp end bit
		 *                                                  14  Cmd
		 * path wait NCC
		 *                                                  15  Wait;
		 * CMD-to-response turnaround
		 * NOTE: The command FSM state is represented using 19 bits.
		 * The STATUS Register(7:4) has 4 bits to represent the
		 * command FSM states. Using these 4 bits, only 16 states can
		 * be represented. Thus three states cannot be represented in
		 * the STATUS(7:4) register. The three states that are not
		 * represented in the STATUS Register(7:4) are:
		 *
		 * Bit 16  Wait for CCS
		 *
		 * Bit 17  Send CCSD
		 *
		 * Bit 18  Boot Mode
		 *
		 * Due to this, while command FSM is in Wait for CCS state or
		 * Send CCSD or Boot Mode, the Status register indicates
		 * status as 0 for the bit field 7:4.
		 */
		uint32_t command_fsm_states : 4;
		/*
		 * Raw selected card_data[3]; checks whether card is present
		 *                                                  0-card
		 * not present
		 *                                                  1-card
		 * present
		 */
		uint32_t data_3_status      : 1;
		/*
		 * Inverted version of raw selected card_data[0]
		 *                                                  0-card
		 * data not busy
		 *                                                  1-card
		 * data busy
		 */
		uint32_t data_busy          : 1;
		/* Data transmit or receive state-machine is busy */
		uint32_t data_state_mc_busy : 1;
		/*
		 * Index of previous response, including any auto-stop sent
		 * by core
		 */
		uint32_t response_index     : 6;
		/* FIFO count Number of filled locations in FIFO */
		uint32_t fifo_count         : 13;
		/*
		 * DMA acknowledge signal state; either dw_dma_ack or
		 * ge_dma_ack, depending on DW-DMA or Generic-DMA selection.
		 */
		uint32_t dma_ack            : 1;
		/*
		 * DMA request signal state; either dw_dma_req or ge_dma_req,
		 * depending on DW-DMA or Generic-DMA selection.
		 */
		uint32_t dma_req            : 1;
	};

	uint32_t word;
} MMC_STATUS_t;


/*
 * FIFOTH.
 */

__extension__
typedef union {
	struct {
		/*
		 * FIFO threshold watermark level when transmitting data to
		 * card.
		 * When FIFO data count is less than or equal to this
		 * number,DMA/FIFO request is raised. If Interrupt is
		 * enabled, then interrupt occurs. During end of packet,
		 * request or interrupt is generated,regardless of threshold
		 * programming.
		 * In non-DMA mode, when transmit FIFO threshold (TXDR)
		 * interrupt is enabled, then interrupt is generated instead
		 * of DMA request. During end of packet, on last interrupt,
		 * host is responsible for filling FIFO with only required
		 * remaining bytes (not before FIFO is full or after CIU
		 * completes data transfers, because FIFO may not be empty).
		 * In DMA mode, at end of packet, if last transfer is less
		 * than burst size, DMA controller does single cycles until
		 * required bytes are transferred.
		 * 12 bits-1 bit less than FIFO-count of status register,
		 * which is 13 bits.
		 * Limitation: TX_WMark = 1;
		 * Recommended: FIFO_DEPTH/2; (means less than or equal to
		 * FIFO_DEPTH/2)
		 */
		uint32_t tx_wmark                         : 12;
		/* Reserved. */
		uint32_t __reserved_0                     : 4;
		/*
		 * FIFO threshold watermark level when receiving data to card.
		 * When FIFO data count reaches greater than this
		 * number,DMA/FIFO request is raised. During end of packet,
		 * request is
		 * generated regardless of threshold programming in order to
		 * complete any remaining data.
		 * In non-DMA mode, when receiver FIFO threshold (RXDR)
		 * interrupt is enabled, then interrupt is generated instead
		 * of DMA request.
		 * During end of packet, interrupt is not generated if
		 * threshold programming is larger than any remaining data.
		 * It is responsibility of host to read remaining bytes on
		 * seeing Data Transfer Done interrupt.
		 * In DMA mode, at end of packet, even if remaining bytes are
		 * less than threshold, DMA request does single transfers to
		 * flush out any remaining bytes before Data Transfer Done
		 * interrupt is set.
		 * 12 bits-1 bit less than FIFO-count of status register,
		 * which is 13 bits.
		 * Limitation: RX_WMark = FIFO_DEPTH-2
		 * Recommended: (FIFO_DEPTH/2) - 1; (means greater than
		 * (FIFO_DEPTH/2) - 1)
		 * NOTE: In DMA mode during CCS time-out, the DMA does not
		 * generate the request at the end of packet, even if
		 * remaining bytes are less than threshold. In this case,
		 * there will be some data left in the FIFO. It is the
		 * responsibility of the application to reset the FIFO after
		 * the CCS timeout.
		 */
		uint32_t rx_wmark                         : 12;
		/*
		 * Burst size of multiple transaction; should be programmed
		 * same as
		 * DW-DMA controller multiple-transaction-size SRC/DEST_MSIZE.
		 *                                                  000  1
		 * transfers
		 *                                                  001  4
		 *                                                  010  8
		 *                                                  011  16
		 *                                                  100  32
		 *                                                  101  64
		 *                                                  110  128
		 *                                                  111  256
		 * The units for transfers is the H_DATA_WIDTH parameter. A
		 * single transfer (dw_dma_single assertion in case of Non DW
		 * DMA interface) would be signalled based on this value.
		 * Value should be sub-multiple of (RX_WMark + 1)*
		 * (F_DATA_WIDTH/H_DATA_WIDTH) and
		 * (FIFO_DEPTH - TX_WMark)* (F_DATA_WIDTH/H_DATA_WIDTH)
		 * For example, if FIFO_DEPTH = 16,
		 * FDATA_WIDTH == H_DATA_WIDTH
		 * Allowed combinations for MSize and TX_WMark are:
		 * MSize = 1, TX_WMARK = 1-15
		 * MSize = 4, TX_WMark = 8
		 * MSize = 4, TX_WMark = 4
		 * MSize = 4, TX_WMark = 12
		 * MSize = 8, TX_WMark = 8
		 * MSize = 8, TX_WMark = 4
		 * Allowed combinations for MSize and RX_WMark are:
		 * MSize = 1, RX_WMARK = 0-14
		 * MSize = 4, RX_WMark = 3
		 * MSize = 4, RX_WMark = 7
		 * MSize = 4, RX_WMark = 11
		 * MSize = 8, RX_WMark = 7
		 * Recommended:
		 * MSize = 8, TX_WMark = 8, RX_WMark = 7
		 */
		uint32_t dw_dma_multiple_transaction_size : 3;
		/* Reserved. */
		uint32_t __reserved_1                     : 1;
	};

	uint32_t word;
} MMC_FIFOTH_t;


/*
 * CDETECT.
 */

__extension__
typedef union {
	struct {
		/*
		 * Value on card_detect_n input ports (1 bit per card);
		 * read-only bits.0 represents presence of card. Only
		 * NUM_CARDS number of bits are implemented.
		 */
		uint32_t card_detect_n : 2;
		/* Reserved. */
		uint32_t __reserved    : 30;
	};

	uint32_t word;
} MMC_CDETECT_t;


/*
 * WRTPRT.
 */

__extension__
typedef union {
	struct {
		/*
		 * Value on card_write_prt input ports (1 bit per card).
		 * 1 represents write protection. Only NUM_CARDS number of
		 * bits are implemented.
		 */
		uint32_t write_protect : 1;
		/* Reserved. */
		uint32_t __reserved    : 31;
	};

	uint32_t word;
} MMC_WRTPRT_t;


/*
 * GPIO.
 */

__extension__
typedef union {
	struct {
		/*
		 * Value on gpi input ports; this portion of register is
		 * read-only. Valid only when AREA_OPTIMIZED parameter is 0.
		 */
		uint32_t gpi        : 8;
		/*
		 * Value needed to be driven to gpo pins; this portion of
		 * register is read/write. Valid only when AREA_OPTIMIZED
		 * parameter is 0.
		 */
		uint32_t gpo        : 16;
		/* Reserved. */
		uint32_t __reserved : 8;
	};

	uint32_t word;
} MMC_GPIO_t;


/*
 * TCBCNT.
 */

__extension__
typedef union {
	struct {
		/*
		 * Number of bytes transferred by CIU unit to card.
		 * In 32-bit or 64-bit AMBA data-bus-width modes, register
		 * should be accessed in full to avoid read-coherency
		 * problems.In 16-bit AMBA data-bus-width mode, internal
		 * 16-bit coherency register is implemented. User should
		 * first read lower 16 bits and then higher 16 bits. When
		 * reading lower 16 bits, higher 16 bits of counter are
		 * stored in temporary register. When higher 16 bits are
		 * read, data from temporary register is supplied. Both
		 * TCBCNT and TBBCNT share same coherency register. When
		 * AREA_OPTIMIZED parameter is 1, register should be read
		 * only after data transfer completes; during data
		 * transfer,register returns 0.
		 */
		uint32_t trans_card_byte_count : 32;
	};

	uint32_t word;
} MMC_TCBCNT_t;


/*
 * TBBCNT.
 */

__extension__
typedef union {
	struct {
		/*
		 * Number of bytes transferred between Host/DMA memory and
		 * BIU FIFO.
		 * In 32-bit or 64-bit AMBA data-bus-width modes, register
		 * should be accessed in full to avoid read-coherency
		 * problems.In 16-bit AMBA data-bus-width mode, internal
		 * 16-bit coherency register is implemented. User should
		 * first read lower 16 bits and then higher 16 bits. When
		 * reading lower 16 bits, higher 16 bits of counter are
		 * stored in temporary register. When higher 16 bits are
		 * read, data from temporary register is supplied.
		 * Both TCBCNT and TBBCNT share same coherency register.
		 */
		uint32_t trans_fifo_byte_count : 32;
	};

	uint32_t word;
} MMC_TBBCNT_t;


/*
 * DEBNCE.
 */

__extension__
typedef union {
	struct {
		/*
		 * Number of host clocks (clk) used by debounce filter logic;
		 * typical
		 * debounce time is 5-25 ms.
		 */
		uint32_t debounce_count : 24;
		/* Reserved. */
		uint32_t __reserved     : 8;
	};

	uint32_t word;
} MMC_DEBNCE_t;


/*
 * USRID.
 */

__extension__
typedef union {
	struct {
		/*
		 * User identification register; value set by user. Default
		 * reset value can be picked by user while configuring core
		 * before synthesis.
		 * Can also be used as scratch pad register by user.
		 */
		uint32_t usr_id : 32;
	};

	uint32_t word;
} MMC_USRID_t;


/*
 * VERID.
 */

__extension__
typedef union {
	struct {
		/*
		 * Synopsys version identification register; register value
		 * is hard-wired.Can be read by firmware to support different
		 * versions of core.
		 */
		uint32_t ver_id : 32;
	};

	uint32_t word;
} MMC_VERID_t;


/*
 * HCON.
 */

__extension__
typedef union {
	struct {
		/*
		 * Hardware configurations selected by user before
		 * synthesizing core. Register values can be used to develop
		 * configuration-independent software drivers.
		 *                                                  bit 0
		 * CARD_TYPE
		 *                                                  0
		 * MMC_ONLY
		 *                                                  1  SD_MMC
		 *                                                  bit [5:1]
		 *  NUM_CARDS - 1
		 *                                                  bit 6
		 * H_BUS_TYPE
		 *                                                  0  APB
		 *                                                  1  AHB
		 *                                                  bit [9:7]
		 * H_DATA_WIDTH
		 *                                                  000  16
		 * bits
		 *                                                  001  32
		 * bits
		 *                                                  010  64
		 * bits
		 *                                                  others
		 * reserved
		 *                                                  bit
		 * [15:10]  H_ADDR_WIDTH
		 *                                                  0 to 7
		 * reserved
		 *                                                  8  9 bits
		 *                                                  9  10 bits
		 *
		 *                                                  31  32
		 * bits
		 *                                                  32 to 63
		 * reserved
		 *
		 * bit[17:16]  DMA_INTERFACE
		 *                                                  00  none
		 *                                                  01  DW_DMA
		 *                                                  10
		 * GENERIC_DMA
		 *                                                  11
		 * NON-DW-DMA
		 *                                                  bit
		 * [20:18] GE_DMA_DATA_WIDTH
		 *                                                  000  16
		 * bits
		 *                                                  001  32
		 * bits
		 *                                                  010  64
		 * bits
		 *                                                  others
		 * reserved
		 *                                                  bit 21
		 * FIFO_RAM_INSIDE
		 *                                                  0  outside
		 *                                                  1  inside
		 *                                                  bit 22
		 * IMPLEMENT_HOLD_REG
		 *                                                  0  no
		 * hold register
		 *                                                  1  hold
		 * register
		 *                                                  bit 23
		 * SET_CLK_FALSE_PATH
		 *                                                  0  no
		 * false path
		 *                                                  1  false
		 * path set
		 *                                                  bit
		 * [25:24]  NUM_CLK_DIVIDER-1
		 *                                                  bit 26 -
		 * AREA_OPTIMIZED
		 *                                                  0  no
		 * area optimization
		 *                                                  1  Area
		 * optimization
		 *                                                  For
		 * 64-bit Address Configuration only
		 *                                                  bit 27 -
		 * ADDR_CONFIG
		 *                                                  0  32-bit
		 * addressing supported
		 *                                                  1  64-bit
		 * addressing supported
		 * For 32-bit Address Configuration only: bit [31:27]
		 * reserved (0)
		 * For 64-bit Address Configuration only: bit [31:28]
		 * reserved (0)
		 * For FIFO_DEPTH parameter, power-on value of RX_WMark value
		 * of FIFO
		 * Threshold Watermark Register represents FIFO_DEPTH - 1.
		 */
		uint32_t h_con : 32;
	};

	uint32_t word;
} MMC_HCON_t;


/*
 * UHS.
 */

__extension__
typedef union {
	struct {
		/*
		 * High Voltage mode. Determines the voltage fed to the
		 * buffers by an
		 * external voltage regulator.
		 *                                                  0
		 * Buffers supplied with 3.3V Vdd
		 *                                                  1
		 * Buffers supplied with 1.8V Vdd
		 * These bits function as the output of the host controller
		 * and are fed to an external voltage regulator. The voltage
		 * regulator must switch the voltage of the buffers of a
		 * particular card to either 3.3V or 1.8V, depending on the
		 * value programmed in the register.
		 * VOLT_REG[0] should be set to 1b1 for card number 0 in
		 * order to make it
		 * operate for 1.8V.
		 */
		uint32_t volt_reg : 16;
		/*
		 * DDR mode. These bits indicate DDR mode of operation to the
		 * core for the data transfer.
		 *                                                  0
		 * Non-DDR mode
		 *                                                  1  DDR
		 * mode
		 * UHS_REG [16] should be set for card number 0, UHS_REG [17]
		 * for card number 1 and so on.
		 */
		uint32_t ddr_reg  : 16;
	};

	uint32_t word;
} MMC_UHS_t;


/*
 * RST_N.
 */

__extension__
typedef union {
	struct {
		/*
		 * Hardware reset.
		 *                                                  1  Active
		 * mode
		 *                                                  0  Reset
		 * These bits cause the cards to enter pre-idle state, which
		 * requires them to be re-initialized.
		 *  CARD_RESET[0] should be set to 1b0 to reset card number 0
		 *  CARD_RESET[15] should be set to 1'b0 to reset card number
		 * 15.
		 * The number of bits implemented is restricted to NUM_CARDS.
		 */
		uint32_t card_reset : 1;
		/* Reserved. */
		uint32_t __reserved : 31;
	};

	uint32_t word;
} MMC_RST_N_t;


/*
 * BMOD.
 */

__extension__
typedef union {
	struct {
		/*
		 * Software Reset.When set,the DMA Controller resets all its
		 * internal registers.
		 * SWR is read/write. It is automatically cleared after 1
		 * clock cycle.
		 */
		uint32_t swr        : 1;
		/*
		 * Fixed Burst. Controls whether the AHB Master interface
		 * performs fixed burst transfers or not. When set,the AHB
		 * will use only SINGLE, INCR4, INCR8 or
		 * INCR16 during start of normal burst transfers.When
		 * reset,the AHB will use SINGLE and INCR burst transfer
		 * operations.
		 * FB is read/write.
		 */
		uint32_t fb         : 1;
		/*
		 * Descriptor Skip Length. Specifies the number of
		 * HWord/Word/Dword (depending on 16/32/64-bit bus) to skip
		 * between two unchained descriptors. This is applicable only
		 * for dual buffer structure.
		 * DSL is read/write.
		 */
		uint32_t dsl        : 5;
		/*
		 * IDMAC Enable. When set, the IDMAC is enabled.
		 * DE is read/write.
		 */
		uint32_t de         : 1;
		/*
		 * Programmable Burst Length. These bits indicate the maximum
		 * number of beats to be performed in one IDMAC transaction.
		 * The IDMAC will always attempt to burst as specified in PBL
		 * each time it starts a Burst transfer on the host bus. The
		 * permissible values are 1, 4, 8, 16, 32, 64, 128 and 256.
		 * This value is the mirror of MSIZE of FIFOTH register. In
		 * order to change this value, write the required value
		 * to FIFOTH register. This is an encode value as follows.
		 *                                                  000  1
		 * transfers
		 *                                                  001  4
		 * transfers
		 *                                                  010  8
		 * transfers
		 *                                                  011  16
		 * transfers
		 *                                                  100  32
		 * transfers
		 *                                                  101  64
		 * transfers
		 *                                                  110  128
		 * transfers
		 *                                                  111  256
		 * transfers
		 * Transfer unit is either 16, 32, or 64 bits, based on
		 * HDATA_WIDTH.
		 * PBL is a read-only value and is applicable only for Data
		 * Access; it does not apply to descriptor accesses.
		 */
		uint32_t pbl        : 3;
		/* Reserved. */
		uint32_t __reserved : 21;
	};

	uint32_t word;
} MMC_BMOD_t;


/*
 * PLDMND.
 */

__extension__
typedef union {
	struct {
		/*
		 * Poll Demand. If the OWN bit of a descriptor is not set,
		 * the FSM goes to the Suspend state. The host needs to write
		 * any value into this register for the IDMAC FSM to resume
		 * normal
		 * descriptor fetch operation. This is a write only register.
		 * PD bit is write-only.
		 */
		uint32_t pd : 32;
	};

	uint32_t word;
} MMC_PLDMND_t;


/*
 * DBADDRL.
 */

__extension__
typedef union {
	struct {
		/*
		 * Start of Descriptor List Lower. Contains the LSB 32-bits
		 * of the base address of the First Descriptor. The LSB bits
		 * [0/1/2:0] for 16/32/64-bit bus-width) are ignored and
		 * taken as all-zero by the IDMAC internally. Hence these LSB
		 * bits are read-only.
		 */
		uint32_t sdl : 32;
	};

	uint32_t word;
} MMC_DBADDRL_t;


/*
 * DBADDRU.
 */

__extension__
typedef union {
	struct {
		/*
		 * Start of Descriptor List Upper. Contains the MSB 32-bits
		 * of the base address of the First Descriptor. This register
		 * should be programed when core is configured with
		 * M_ADDR_WIDTH =64.
		 */
		uint32_t sdlu : 32;
	};

	uint32_t word;
} MMC_DBADDRU_t;


/*
 * IDSTS64.
 */

__extension__
typedef union {
	struct {
		/*
		 * Transmit Interrupt. Indicates that data transmission is
		 * finished for a descriptor. Writing a 1 clears this bit.
		 */
		uint32_t ti           : 1;
		/*
		 * Receive Interrupt. Indicates the completion of data
		 * reception for a descriptor. Writing a 1 clears this bit.
		 */
		uint32_t ri           : 1;
		/*
		 * Fatal Bus Error Interrupt. Indicates that a Bus Error
		 * occurred (IDSTS64[12:10]). When this bit is set, the DMA
		 * disables all its bus accesses. Writing a 1 clears this bit.
		 */
		uint32_t fbe          : 1;
		/* Reserved. */
		uint32_t __reserved_0 : 1;
		/*
		 * Descriptor Unavailable Interrupt. This bit is set when the
		 * descriptor is unavailable due to OWN bit = 0 (DES0[31]
		 * =0). Writing a 1 clears this bit.
		 */
		uint32_t du           : 1;
		/*
		 * Card Error Summary. Indicates the status of the
		 * transaction to/from the card; also present in RINTSTS.
		 * Indicates the logical OR of the following bits:
		 *                                                   EBE  End
		 * Bit Error
		 *                                                   RTO
		 * Response Timeout/Boot Ack Timeout
		 *                                                   RCRC
		 * Response CRC
		 *                                                   SBE
		 * Start Bit Error
		 *                                                   DRTO
		 * Data Read Timeout/BDS timeout
		 *                                                   DCRC
		 * Data CRC for Receive
		 *                                                   RE
		 * Response Error
		 *                                                  Writing a
		 * 1 clears this bit.
		 * The abort condition of the IDMAC depends on the setting of
		 * this CES bit. If the CES bit
		 * is enabled, then the IDMAC aborts on a response error;
		 * however, it will not abort if the
		 * CES bit is cleared.
		 */
		uint32_t ces          : 1;
		/* Reserved. */
		uint32_t __reserved_1 : 2;
		/*
		 * Normal Interrupt Summary. Logical OR of the following:
		 *
		 * IDSTS64[0]-Transmit Interrupt
		 *
		 * IDSTS64[1]-Receive Interrupt
		 * Only unmasked bits affect this bit.
		 * This is a sticky bit and must be cleared each time a
		 * corresponding bit that causes NIS
		 * to be set is cleared. Writing a 1 clears this bit.
		 */
		uint32_t nis          : 1;
		/*
		 * Abnormal Interrupt Summary. Logical OR of the following:
		 *
		 * IDSTS64[2]-Fatal Bus Interrupt
		 *
		 * IDSTS64[4]-DU bit Interrupt
		 *                                                  Only
		 * unmasked bits affect this bit.
		 * This is a sticky bit and must be cleared each time a
		 * corresponding bit that causes AIS
		 * to be set is cleared. Writing a 1 clears this bit.
		 */
		uint32_t ais          : 1;
		/*
		 * Error Bits. Indicates the type of error that caused a Bus
		 * Error.
		 * Valid only with Fatal Bus
		 * Error bit IDSTS64[2] set. This field does not generate an
		 * interrupt.
		 *                                                  3b001
		 * Host Abort received during transmission
		 *                                                  3b010
		 * Host Abort received during reception
		 *                                                  Others:
		 * Reserved
		 *                                                  EB is
		 * read-only.
		 */
		uint32_t eb           : 3;
		/*
		 * DMAC FSM present state.
		 *                                                  0
		 * DMA_IDLE
		 *                                                  1
		 * DMA_SUSPEND
		 *                                                  2  DESC_RD
		 *                                                  3
		 * DESC_CHK
		 *                                                  4
		 * DMA_RD_REQ_WAIT
		 *                                                  5
		 * DMA_WR_REQ_WAIT
		 *                                                  6  DMA_RD
		 *                                                  7  DMA_WR
		 *                                                  8
		 * DESC_CLOSE
		 *                                                  This bit
		 * is read-only.
		 */
		uint32_t fsm          : 4;
		/* Reserved. */
		uint32_t __reserved_2 : 15;
	};

	uint32_t word;
} MMC_IDSTS64_t;


/*
 * IDINTEN64.
 */

__extension__
typedef union {
	struct {
		/*
		 * Transmit Interrupt Enable. When set with Normal Interrupt
		 * Summary Enable,Transmit Interrupt is enabled. When reset,
		 * Transmit Interrupt is disabled.
		 */
		uint32_t ti           : 1;
		/*
		 * Receive Interrupt Enable. When set with Normal Interrupt
		 * Summary Enable,Receive Interrupt is enabled. When reset,
		 * Receive Interrupt is disabled.
		 */
		uint32_t ri           : 1;
		/*
		 * Fatal Bus Error Enable. When set with Abnormal Interrupt
		 * Summary Enable, the Fatal Bus Error Interrupt is enabled.
		 * When reset, Fatal Bus Error Enable Interrupt is disabled.
		 */
		uint32_t fbe          : 1;
		/* Reserved. */
		uint32_t __reserved_0 : 1;
		/*
		 * Descriptor Unavailable Interrupt. When set along with
		 * Abnormal Interrupt Summary Enable, the DU interrupt is
		 * enabled.
		 */
		uint32_t du           : 1;
		/*
		 * Card Error summary Interrupt Enable. When set, it enables
		 * the Card Interrupt summary.
		 */
		uint32_t ces          : 1;
		/* Reserved. */
		uint32_t __reserved_1 : 2;
		/*
		 * Normal Interrupt Summary Enable. When set, a normal
		 * interrupt is enabled. When reset, a normal interrupt is
		 * disabled. This bit enables the following bits:
		 *
		 * IDINTEN64[0] - Transmit Interrupt
		 *
		 * IDINTEN64[1] - Receive Interrupt
		 */
		uint32_t ni           : 1;
		/*
		 * Abnormal Interrupt Summary Enable. When set, an abnormal
		 * interrupt is enabled. This bit enables the following bits:
		 *
		 * IDINTEN64[2] - Fatal Bus Error Interrupt
		 *
		 * IDINTEN64[4] -  DU Interrupt
		 */
		uint32_t ai           : 1;
		/* Reserved. */
		uint32_t __reserved_2 : 22;
	};

	uint32_t word;
} MMC_IDINTEN64_t;


/*
 * DSCADDRL.
 */

__extension__
typedef union {
	struct {
		/*
		 * Host Descriptor Address Pointer Lower 32-bits. Cleared on
		 * reset. Pointer updated by IDMAC during operation. This
		 * register points to the LSB 32-bits start address of the
		 * current
		 * descriptor read by the IDMAC.
		 */
		uint32_t hdal : 32;
	};

	uint32_t word;
} MMC_DSCADDRL_t;


/*
 * DSCADDRU.
 */

__extension__
typedef union {
	struct {
		/*
		 * Host Descriptor Address Pointer Upper 32-bits. Cleared on
		 * reset. Pointer updated by IDMAC during operation. This
		 * register points to the MSB 32-bits start address of the
		 * current
		 * descriptor read by the IDMAC. This register should be
		 * programed when core is configured with M_ADDR_WIDTH =64
		 */
		uint32_t hdau : 32;
	};

	uint32_t word;
} MMC_DSCADDRU_t;


/*
 * BUFADDRL.
 */

__extension__
typedef union {
	struct {
		/*
		 * Host Buffer Address Pointer Upper 32 bits. Cleared on
		 * Reset.
		 * Pointer updated by IDMAC during operation. This register
		 * points to the current Data Buffer Address, upper 32 bits,
		 * being accessed by the IDMAC.
		 */
		uint32_t hbal : 32;
	};

	uint32_t word;
} MMC_BUFADDRL_t;


/*
 * BUFADDRU.
 */

__extension__
typedef union {
	struct {
		/*
		 * Host Buffer Address Pointer Upper 32 bits. Cleared on
		 * Reset.
		 * Pointer updated by IDMAC during operation. This register
		 * points to the current Data Buffer Address, upper 32 bits,
		 * being accessed by the IDMAC. This register should be
		 * programed when core is configured with M_ADDR_WIDTH =64.
		 */
		uint32_t hbau : 32;
	};

	uint32_t word;
} MMC_BUFADDRU_t;


/*
 * CARDTHRCTL.
 */

__extension__
typedef union {
	struct {
		/*
		 * Card Read Threshold Enable
		 *                                                   1'b0 -
		 * Card Read Threshold disabled
		 *                                                   1'b1 -
		 * Card Read Threshold enabled. Host Controller initiates
		 *                                                    Read
		 * Transfer only if CardRdThreshold amount of space is
		 *
		 * available in receive FIFO.
		 */
		uint32_t cardrdthren     : 1;
		/*
		 * Busy Clear Interrupt generation:
		 *                                                   1'b0 -
		 * Busy Clear Interrupt disabled
		 *                                                   1'b1 -
		 * Busy Clear Interrupt enabled
		 * Note: The application can disable this feature if it does
		 * not want to wait for a Busy Clear Interrupt. For example,
		 * in a multi-card scenario, the application can switch to
		 * the other card without waiting for a busy to be completed.
		 * In such cases, the application can use the polling method
		 * to determine the status of busy. By default this feature
		 * is disabled and backward-compatible to the legacy drivers
		 * where polling is used.
		 */
		uint32_t busy_clr_int_en : 1;
		/* Reserved. */
		uint32_t __reserved_0    : 14;
		/*
		 * Card Read Threshold size. Note: The maximum programmable
		 * value of Card Read Threshold size is 512.
		 */
		uint32_t cardrdthreshold : 11;
		/* Reserved. */
		uint32_t __reserved_1    : 5;
	};

	uint32_t word;
} MMC_CARDTHRCTL_t;


/*
 * BACK_END_POWER_R.
 */

__extension__
typedef union {
	struct {
		/*
		 * Back end power
		 *                                                   1'b0
		 * Off; Reset
		 *                                                   1'b1
		 * Back-end Power supplied to card application; one pin per
		 *
		 * card
		 */
		uint32_t back_end_power : 16;
		/* Reserved. */
		uint32_t __reserved     : 16;
	};

	uint32_t word;
} MMC_BACK_END_POWER_R_t;


/*
 * UHS_REG_EXT.
 */

__extension__
typedef union {
	struct {
		/*
		 * Support for 1.2V. MMC_VOLT_REG bits; must be read in
		 * combination
		 * with UHS_VOLT_REG to decode output selected voltage.
		 * The biu_volt_reg_1_2[NUM_CARD_BUS-1:0] signal decodes the
		 * voltage combination selected for the I/O voltage logic.
		 * Host controllers that support only SD standard or standard
		 * versions before eMMC4.41 do not program MMC_VOLT_REG.
		 * Only host controllers that support all three
		 * versions3.3,1.8,1.2 Vcan program MMC_VOLT_REG and connect
		 * biu_volt_reg_1_2.
		 */
		uint32_t mmc_volt_reg        : 16;
		/*
		 * Control for amount of phase shift on cclk_in_sample clock.
		 * Can choose three MSBs to control delay lines and four LSBs
		 * to control phase shift; alternatively, use only LSBs.
		 */
		uint32_t clk_smpl_phase_ctrl : 7;
		/*
		 * Control for amount of phase shift on cclk_in_drv clock.
		 * Can choose three MSBs to control delay lines and four LSBs
		 * to control phase shift; alternatively, use only LSBs.
		 */
		uint32_t clk_drv_phase_ctrl  : 7;
		/*
		 * Input clock control for cclk_in. The MUX controlled by
		 * these bits exists outside DWC_mobile_storage IP.
		 */
		uint32_t ext_clk_mux_ctrl    : 2;
	};

	uint32_t word;
} MMC_UHS_REG_EXT_t;


/*
 * EMMC_DDR.
 */

__extension__
typedef union {
	struct {
		/*
		 * Control for start bit detection mechanism inside
		 * DWC_mobile_storage based on duration of start bit; each
		 * bit refers to one slot. For eMMC 4.5, start bit can
		 * be:
		 *                                                   Full
		 * cycle (HALF_START_BIT = 0)
		 *                                                   Less
		 * than one full cycle (HALF_START_BIT = 1)
		 * Set HALF_START_BIT=1 for eMMC 4.5 and above; set to 0 for
		 * SD applications.
		 */
		uint32_t half_start_bit : 1;
		/* Reserved. */
		uint32_t __reserved     : 31;
	};

	uint32_t word;
} MMC_EMMC_DDR_t;


/*
 * ENABLE_SHIFT.
 */

__extension__
typedef union {
	struct {
		/*
		 * Control for the amount of phase shift provided on the
		 * default
		 * enables in the design. Two bits are assigned for each
		 * card/slot. For example, bits[1:0] control slot0 and
		 * indicate the following.
		 *                                                   00
		 * Default phase shift
		 *                                                   01
		 * Enables shifted to next immediate positive edge
		 *                                                   10
		 * Enables shifted to next immediate negative edge
		 *                                                   11
		 * Reserved
		 */
		uint32_t enable_shift_card : 2;
		/* Reserved. */
		uint32_t __reserved        : 30;
	};

	uint32_t word;
} MMC_ENABLE_SHIFT_t;


/*
 * FIFO.
 * MMC Data FIFO
 */

__extension__
typedef union {
	struct {
		/*
		 * On reads, provides 32-bits of data from the FIFO. On
		 * writes, puts 32-bits of data into the FIFO. The FIFO
		 * should only be read if data is available or written if
		 * space is available. The FIFO status is available in the
		 * MMC_STATUS register.
		 */
		uint32_t data : 32;
	};

	uint32_t word;
} MMC_FIFO_t;


/* Scratchpad. */

__extension__
typedef union {
	struct {
		uint64_t val        : 32;
		/* Reserved. */
		uint64_t __reserved : 32;
	};

	uint64_t word;
} MMC_SCRATCHPAD2_t;


/*
 * Interface Control.
 * Provides control over card-controller operations.
 */

__extension__
typedef union {
	struct {
		/*
		 * Override the Card-Detect input. When 0, the associated
		 * card will receive its card-detect input from the chip's
		 * EMMC_CARD_DETECT_N input (shared with a GPIO pin on some
		 * devices). When 1, the associated card uses the associated
		 * CARD_DET_OVD_VAL bit.
		 */
		uint64_t card_det_ovd       : 2;
		/* Value to apply to Card-Detect when CARD_DET_OVD is 1. */
		uint64_t card_det_ovd_val   : 2;
		/*
		 * Override the Write-Protect input. When 0, the associated
		 * card will receive its write-protect input from the chip's
		 * EMMC_CARD_WRITE_PRT input (shared with a GPIO pin on some
		 * devices). When 1, the associated card uses the associated
		 * WRITE_PROT_OVD_VAL bit.
		 */
		uint64_t write_prot_ovd     : 2;
		/*
		 * Value to apply to Write-Protect when WRITE_PROT_OVD is 1.
		 */
		uint64_t write_prot_ovd_val : 2;
		/*
		 * Override the Card-Interrupt input. When 0, the associated
		 * card will receive its cart-interrupt input from the chip's
		 * EMMC_INT_N input (shared with a GPIO pin on some devices).
		 * When 1, the associated card uses the associated
		 * CARD_INT_OVD_VAL bit.
		 */
		uint64_t card_int_ovd       : 2;
		/* Value to apply to Card-Interrupt when CARD_INT_OVD is 1. */
		uint64_t card_int_ovd_val   : 2;
		/* Reserved. */
		uint64_t __reserved_0       : 18;
		/*
		 * When 1, the device will be held in reset. After writing
		 * with a 1, software must write back to 0 in order to take
		 * device out of reset. Resetting the device will terminate
		 * any inflight DMA.
		 */
		uint64_t dev_reset          : 1;
		/*
		 * When written with a 1, the boot sequence will be rerun. A
		 * DEV_RESET should typically be performed prior to
		 * DEV_REBOOT.
		 */
		uint64_t dev_reboot         : 1;
		/* Reserved. */
		uint64_t __reserved_1       : 32;
	};

	uint64_t word;
} MMC_INTFC_CTL_t;


/*
 * Clock Control.
 * Provides control over eclk PLL.
 */

__extension__
typedef union {
	struct {
		/*
		 * When written with a 1, the PLL will be configured with the
		 * settings in PLL_F/R/OD.  When written with a zero, the PLL
		 * will be bypassed.
		 */
		uint64_t ena         : 1;
		/* Output divider. */
		uint64_t pll_od      : 4;
		/* Reference divider value. */
		uint64_t pll_r       : 6;
		/* Feedback divider value. */
		uint64_t pll_f       : 13;
		/* Reserved control - must be zero for normal operation. */
		uint64_t pll_rsvd    : 7;
		/*
		 * Indicates that PLL has been enabled and clock is now
		 * running off of the PLL output.
		 */
		uint64_t clock_ready : 1;
		/* Reserved. */
		uint64_t __reserved  : 32;
	};

	uint64_t word;
} MMC_HX_CLOCK_CONTROL_t;


/*
 * Interface Status.
 * Scratchpad
 */

__extension__
typedef union {
	struct {
		/*
		 * Indicates state of the EMMC bit in the RSH_BOOT_MODE
		 * register.
		 */
		uint64_t boot_ena          : 1;
		/* Provides boot state for diagnostics. */
		uint64_t boot_state        : 4;
		/* Reserved. */
		uint64_t __reserved_0      : 3;
		/* Provides boot record state for diagnostics. */
		uint64_t boot_record_state : 3;
		/* Reserved. */
		uint64_t __reserved_1      : 5;
		/* Provides REV_ID field from boot record. */
		uint64_t boot_revid        : 8;
		/* Reserved. */
		uint64_t __reserved_2      : 40;
	};

	uint64_t word;
} MMC_INTFC_STS_t;


/*
 * Boot Configuration.
 * Boot configuration parameters. This register initialized on HARD_RESET_N
 * deassertion but is left intact on soft reset.
 */

__extension__
typedef union {
	struct {
		/*
		 * Number of 128-Kilobyte blocks to request in boot mode. The
		 * boot process is typically aborted by software or the boot
		 * stream itself before this limit is reached.
		 */
		uint64_t boot_size_mult   : 8;
		/*
		 * Data bus width to use in alternate boot mode. Mandatory
		 * boot mode always uses 1-bit.
		 */
		uint64_t alt_boot_width   : 2;
		/*
		 * When set to 1, the interface will boot using mandatory
		 * boot mode at 400KHz.
		 */
		uint64_t legacy_boot_mode : 1;
		/* Reserved. */
		uint64_t __reserved       : 53;
	};

	uint64_t word;
} MMC_BOOT_CFG_t;

#endif /* !defined(__DOXYGEN__) */

#endif /* !defined(__ASSEMBLER__) */

#endif /* !defined(__REGS_MMC_H__) */
