/*
 * Copyright (c) 2018, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <console.h>
#include <mmio.h>
#include <string.h>
#include <utils_def.h>

#include "bluefield_private.h"
#include "tmfifo_console.h"
#include "rsh_def.h"

/*
 * This function prototype isn't declared anywhere as ATF only expects
 * console_xxx_register to call it, and it assumes that all
 * console_xxx_register functions are written in assembly.
 */
int console_register(console_t *console);

/* The message header we use to send out one character. */
const tmfifo_msg_header_t tx_header = {
	.type = TMFIFO_MSG_CONSOLE,
	.len_lo = 1,
};

/* Return 1 if the tx tmfifo can't handle any more packet of 2 word size. */
static __inline int tmfifo_tx_full(uintptr_t tx_sts)
{
	return (mmio_read_64(tx_sts) > (0x100 - 2));
}

int console_tmfifo_putc(int character, struct console *cons)
{
	/*
	 * After the Arm side resets, the host side disconnects and reconnects
	 * the Rshim driver. In this short interval, it is not actively draining
	 * the tmfifo buffer, and thus some of the ATF BL2 boot messages are
	 * lost. So during the first 1 second after a soft reset, we buffer and
	 * densely package the console message so that the tmfifo is not full
	 * by the time the host side starts to drain it again.
	 */
	static int no_buffer; /* Set to 1 to stop buffering console message. */

	console_tmfifo_t *console = (console_tmfifo_t *)cons;

	/* Prepend \r to \n */
	if (character == '\n')
		console_tmfifo_putc('\r', cons);
	/*
	 * If the tmfifo is full, it probably means that the other side is
	 * not actively draining the fifo, thus it doesn't care about getting
	 * the latest output, so dropping it would be okay.
	 */
	if (no_buffer) {
		if (!tmfifo_tx_full(console->tx_sts_addr)) {
			mmio_write_64(console->tx_data_addr, tx_header.data);
			mmio_write_64(console->tx_data_addr, character & 0xff);
		}

		return character;
	}

	/* Add the new character to the tx buffer. */
	console->tx_buf_chars |= (((uint64_t)(uint8_t)character) <<
				 (8 * console->tx_buf_count));
	console->tx_buf_count++;

	/* See if we have already booted up for more than one second. */
	if (mmio_read_64(RSHIM_BASE + RSH_UPTIME) > BF_REF_CLK_IN_HZ)
		no_buffer = 1;

	/* See if we need to flush the buffer to the tmfifo. */
	if (no_buffer || console->tx_buf_count == sizeof(uint64_t)) {
		tmfifo_msg_header_t tx_buf_header = {
			.type = TMFIFO_MSG_CONSOLE,
			.len_lo = console->tx_buf_count,
		};
		if (!tmfifo_tx_full(console->tx_sts_addr)) {
			mmio_write_64(console->tx_data_addr,
				      tx_buf_header.data);
			mmio_write_64(console->tx_data_addr,
				      console->tx_buf_chars);
		}

		console->tx_buf_chars = 0;
		console->tx_buf_count = 0;
	}

	return character;
}

int console_tmfifo_getc(struct console *cons)
{
	console_tmfifo_t *console = (console_tmfifo_t *)cons;

	/*
	 * If not in the middle of a console message, try to read a
	 * message from the rx_fifo and see if it's a console message
	 * header and get how many character are expected to be received.
	 */
	if (console->rx_msg_char_left == 0) {
		if (mmio_read_64(console->rx_sts_addr) > 0) {
			tmfifo_msg_header_t rx_header = {
				.data = mmio_read_64(console->rx_data_addr),
			};
			if (rx_header.type == TMFIFO_MSG_CONSOLE)
				console->rx_msg_char_left = rx_header.len_lo +
						      256 * rx_header.len_hi;
		}
	}

	/*
	 * If in the middle of a console message and previous rx fifo data
	 * have all been consumed, try to read a new word from the rx fifo
	 * and get the next character.
	 */
	if (console->rx_msg_char_left > 0 && console->rx_buf_count == 0) {
		if (mmio_read_64(console->rx_sts_addr) > 0) {
			console->rx_buf_chars =
					mmio_read_64(console->rx_data_addr);
			console->rx_buf_count = MIN(console->rx_msg_char_left,
			       (unsigned int)(sizeof(uint64_t) / sizeof(char)));
		}
	}

	/* Consume a character from the read rx_buf_chars. */
	if (console->rx_buf_count > 0) {
		int c;

		c = console->rx_buf_chars & 0xff;
		console->rx_buf_chars >>= 8;
		console->rx_buf_count--;
		console->rx_msg_char_left--;

		return c;
	}

	return ERROR_NO_PENDING_CHAR;
}

int console_tmfifo_flush(struct console *cons)
{
	console_tmfifo_t *console = (console_tmfifo_t *)cons;

	/*
	 * We don't flush the fifo as this will take infinite amount of time
	 * if the other side is not actively draining the fifo. Thus here we
	 * only need to flush what we had buffered (if any) to the fifo.
	 */
	if (console->tx_buf_count > 0) {
		tmfifo_msg_header_t tx_buf_header = {
			.type = TMFIFO_MSG_CONSOLE,
			.len_lo = console->tx_buf_count,
		};
		if (!tmfifo_tx_full(console->tx_sts_addr)) {
			mmio_write_64(console->tx_data_addr,
				      tx_buf_header.data);
			mmio_write_64(console->tx_data_addr,
				      console->tx_buf_chars);
		}

		console->tx_buf_chars = 0;
		console->tx_buf_count = 0;
	}

	return 0;
}

int console_tmfifo_register(console_tmfifo_t *console,
			    uintptr_t tx_data, uintptr_t tx_sts,
			    uintptr_t rx_data, uintptr_t rx_sts)
{
	if (console == NULL)
		return 0;

	/* Zero out the console struct. */
	memset(console, 0, sizeof(*console));

	/* Initialize the address of the tmfifo in the struct. */
	console->tx_data_addr = tx_data;
	console->tx_sts_addr  = tx_sts;
	console->rx_data_addr = rx_data;
	console->rx_sts_addr  = rx_sts;

	/* Initialize the console_t struct. */
	console->console.putc  = console_tmfifo_putc;
	console->console.getc  = console_tmfifo_getc;
	console->console.flush = console_tmfifo_flush;
	console->console.flags = CONSOLE_FLAG_BOOT | CONSOLE_FLAG_CRASH;

	/* Add the console to the list of consoles to use. */
	return console_register(&console->console);
}

