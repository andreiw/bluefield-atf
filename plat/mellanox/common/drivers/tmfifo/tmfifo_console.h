/*
 * Copyright (c) 2018, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __TMFIFO_CONSOLE_H__
#define __TMFIFO_CONSOLE_H__

#include <console.h>
#include <types.h>

/* Only message type of console is needed in ATF for tmfifo. */
#define TMFIFO_MSG_CONSOLE	3

/* Base tmfifo console struct. */
typedef struct {
	console_t console;	/* Base console struct */
	/* Tmfifo access register address. */
	uintptr_t tx_data_addr;	/* Addr of fifo to write tx data. */
	uintptr_t tx_sts_addr;	/* Addr of reg to get tx data inflight count. */
	uintptr_t rx_data_addr;	/* Addr of fifo to read rx data. */
	uintptr_t rx_sts_addr;	/* Addr of reg to get rx data buffered count. */
	/* Internal state that needs to be kept by the driver. */
	uint64_t rx_buf_chars;	/* Unconsumed console data from rx data fifo. */
	unsigned int rx_buf_count;	/* Remaining chars from rx_buf_chars. */
	unsigned int rx_msg_char_left;	/* Chars left from current msg. */
	uint64_t tx_buf_chars;	/* Buffered tx char not yet sent. */
	unsigned int tx_buf_count;	/* Unsent chars in txbuf_chars. */
} console_tmfifo_t;

/* Tmfifo message header struct. */
typedef union {
	struct {
		uint8_t type;		/* message type. */
		uint8_t len_hi;		/* payload length, high 8 bits. */
		uint8_t len_lo;		/* payload length, low 8 bits. */
		uint8_t unused[5];	/* reserved, set to 0. */
	} __packed;
	uint64_t data;
}tmfifo_msg_header_t;

/*
 * Initialize a new tmfifo console instance and register it with the console
 * framework. The |console| pointer must point to storage that will be valid
 * for the lifetime of the console, such as a global or static local variable.
 * Its contents will be reinitialized from scratch.
 */
int console_tmfifo_register(console_tmfifo_t *console,
			    uintptr_t tx_data, uintptr_t tx_sts,
			    uintptr_t rx_data, uintptr_t rx_sts);

#endif	/* __TMFIFO_CONSOLE_H__ */
