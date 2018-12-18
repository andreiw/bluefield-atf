/*
 * Copyright (c) 2017, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of Mellanox nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <console.h>
#include <ctype.h>
#include <debug.h>
#include <delay_timer.h>
#include <limits.h>
#include <mmio.h>
#include <pl011.h>
#include <stdarg.h>
#include <string.h>
#include "bluefield_ddr.h"
#include "rsh.h"

#define MAX_BUF_LEN 200
#define MAX_ARG_NUM 10
#define MAX_CMD_HIS 10

#define PROMPT_STR ">>"

struct function_entry {
	char *func_name;
	int (*func)(int, char * const[]);
};

/* All the needed values to keep track of the state of a console. */
struct console_instance {
	/* The history of commands the user has entered. */
	char bufs[MAX_CMD_HIS][MAX_BUF_LEN];
	/* The buffer used to store the current line of user input. */
	char buf[MAX_BUF_LEN];
	/* The length of the current entered buffer. */
	int buf_len;
	/* Current position of the cursor */
	int buf_pos;
	/* Index of the shown buffer (when using up/down arrow keys). */
	int buf_idx;
	/* Index to where the current buf will be stored. */
	int buf_next;
};

#define DECLARE_FUNC(x) \
	extern int x(int argc, char * const argv[])
#define ADD_FUNC(x) { #x , x },

DECLARE_FUNC(list_func);
DECLARE_FUNC(call_as);
DECLARE_FUNC(sleep);
DECLARE_FUNC(echo);
DECLARE_FUNC(repeat);
DECLARE_FUNC(do_ato_probing);
DECLARE_FUNC(do_dto_probing);
DECLARE_FUNC(do_vref_probing);
DECLARE_FUNC(do_ddr_basic);
DECLARE_FUNC(do_phy_bist);
DECLARE_FUNC(do_edit_tap_results);
DECLARE_FUNC(do_vref);
DECLARE_FUNC(do_marg_and_d2);
DECLARE_FUNC(do_configure_emem);
DECLARE_FUNC(ddr_operating_frequency_change);
DECLARE_FUNC(ddr_bist);
DECLARE_FUNC(log);
DECLARE_FUNC(ddr_params);
DECLARE_FUNC(ddr_sweep);
DECLARE_FUNC(settings);
DECLARE_FUNC(cmd);

const struct function_entry func_list[] = {
ADD_FUNC(list_func)
{"ls", list_func},	/* Make an alias for list_func. */
ADD_FUNC(call_as)
ADD_FUNC(sleep)
ADD_FUNC(echo)
ADD_FUNC(repeat)
ADD_FUNC(do_ato_probing)
ADD_FUNC(do_dto_probing)
ADD_FUNC(do_vref_probing)
{"edit_wctap", do_edit_tap_results},
{"edit_rctap", do_edit_tap_results},
{"edit_wdtap", do_edit_tap_results},
{"edit_rdtap", do_edit_tap_results},
{"edit_wltap", do_edit_tap_results},
{"edit_wetap", do_edit_tap_results},
{"edit_sdram_vref", do_vref},
{"edit_ddr_phy_vref", do_vref},
ADD_FUNC(do_phy_bist)
{"show_wcm", do_marg_and_d2},
{"show_rcm", do_marg_and_d2},
{"show_wsm", do_marg_and_d2},
{"show_rsm", do_marg_and_d2},
{"write_2d", do_marg_and_d2},
{"read_2d", do_marg_and_d2},
ADD_FUNC(do_configure_emem)
ADD_FUNC(ddr_operating_frequency_change)
{"pub_record", do_ddr_basic},
{"pub_restore", do_ddr_basic},
{"pub_read", do_ddr_basic},
{"pub_write", do_ddr_basic},
{"pub_dump", do_ddr_basic},
{"emc_record", do_ddr_basic},
{"emc_restore", do_ddr_basic},
{"emc_read", do_ddr_basic},
{"emc_write", do_ddr_basic},
{"emc_dump", do_ddr_basic},
{"emi_record", do_ddr_basic},
{"emi_restore", do_ddr_basic},
{"emi_read", do_ddr_basic},
{"emi_write", do_ddr_basic},
{"emi_dump", do_ddr_basic},
ADD_FUNC(ddr_bist)
ADD_FUNC(log)
ADD_FUNC(ddr_params)
ADD_FUNC(ddr_sweep)
ADD_FUNC(settings)
ADD_FUNC(cmd)
};

#define FUNC_NUM (sizeof(func_list)/sizeof(func_list[0]))
static int find_func(const char *func_name);

/*
 * In our implemenation, we know the tmfifo console sits at the head of the
 * console list.
 */
extern console_t tm_console;

/*
 * A non-blocking console_getc implementation. Return the received character
 * if there is one, else return -1. It ignores the console scope as it doesn't
 * have access to the assembly console_state variable. But our implemenation
 * uses all the consoles for boot time so it doesn't affect anything.
 */
int console_getc_nonblock(void)
{
	console_t *current_console = &tm_console;

	while (current_console != NULL) {
		int c = current_console->getc(current_console);

		if (c >= 0)
			return c;

		current_console = current_console->next;
	}

	return -1;
}

/*
 * Returns 1 if the user pressed Ctrl+c on the keyboard, else 0.
 * Note that this function have the side effect of draining all console inputs
 * to the point where a Ctrl-c is pressed (or to the end if none is pressed).
 */
int ctrlc(void)
{
	if (console_getc_nonblock() == '\03') /* Ctrl + c */
		return 1;

	return 0;
}

/******************************************************************************/
/* Helper functions used when parsing command line arguments. */
/******************************************************************************/
/*
 * Uses strtol to return an int from the provided string, but also check that
 * the provided string can be interpreted as a valid string, and also check if
 * the value provided is in range if required.
 * arg:       The input string.
 * data:      The address where the parsed int value would be stored.
 * is_ranged: Non-zero to check the range of the parsed value.
 * min:       Inclusive minimum value if checking the range.
 * max:       Exclusive maximum value if checking the range.
 * name:      The name of the argument parsed. If not NULL, error message
 *            will be printed to state the reason.
 * Returns 1 if the parse is successful or 0 if something failed.
 */
int get_arg_int(char *const arg, int *data, int is_ranged, int min, int max,
		char *const name)
{
	char *end;
	long value = strtol(arg, &end, 0);

	if (*end != 0) {
		if (name)
			tf_printf("%s given invalid!\n", name);
		return 0;
	}
	if (is_ranged && (value < min || value >= max)) {
		if (name)
			tf_printf("%s %ld(0x%lx) not in range of [%d:%d)!\n",
				name, value, value, min, max);
		return 0;
	}

	*data = value;
	return 1;
}

/*
 * Compare the provided arg string and compare it to all of the optional given
 * string arguments sequentially. If it matches one, the function returns with
 * *data stored the given value of the string it matched.
 * arg:       The input string.
 * data:      The address where the parsed int value would be stored.
 * name:      The name of the argument parsed. If not NULL, error message
 *            will be printed to state the reason.
 * count:     How many optional argument strings are provided.
 * ...:       The strings to compare, followed by the value to set to data if
 *	      the string matched.
 * Returns 1 if the compare matches a string, else returns 0.
 */
int get_arg_str(char *const arg, int *data, char *const name, int count, ...)
{
	va_list arg_list;

	va_start(arg_list, count);

	for (int i = 0; i < count; i++) {
		if (strcmp(arg, va_arg(arg_list, char *const)) == 0) {
			*data = va_arg(arg_list, int);
			va_end(arg_list);
			return 1;
		}
		va_arg(arg_list, int);
	}

	va_end(arg_list);

	if (name) {
		tf_printf("%s \"%s\" given invalid! Valid options are:\n",
			name, arg);
		va_start(arg_list, count);
		for (int i = 0; i < count; i++) {
			tf_printf("\t%s\n", va_arg(arg_list, char *const));
			va_arg(arg_list, int);
		}
		va_end(arg_list);
	}
	return 0;
}

/*
 * Process the input buffer, substitute all spaces with \0 and mark how many
 * and where each argument is. Return how many argument are detected.
 */
static int process_buffer(char *buf, int buf_len, char **argv)
{
	int in_token = 0; /* 1 = in middle of token, elsewise 0. */
	int argc = 0;

	for (int i = 0; i < buf_len; i++) {
		if (in_token == 0 && buf[i] != ' ') {
			argv[argc++] = &buf[i];
			in_token = 1;
		} else if (buf[i] == ' ') {
			in_token = 0;
			buf[i] = 0;
		}
	}

	return argc;
}

/* Some basic functions the console can call. */
int list_func(int argc, char * const argv[])
{
	tf_printf("The following commands are available in this console:\n");
	for (int i = 0; i < FUNC_NUM; i++) {
		tf_printf("%s\n", func_list[i].func_name);
	}
	tf_printf("quit\nexit\n");
	return 0;
}

/* Delay for X seconds. */
int sleep(int argc, char * const argv[])
{
	int seconds;
	int parsed;

	parsed = argc >1 &&
		 get_arg_int(argv[1], &seconds, 1, 0, INT_MAX, "second");

	if (!parsed) {
		tf_printf("Invalid argument\n");
		return -1;
	}

	while (seconds--)
		mdelay(1000);

	return 0;
}

/* Echo whatever is entered */
int echo(int argc, char * const argv[])
{
	for (int i = 1; i < argc; i++) {
		for (int j = 0; argv[i][j] != 0; j++)
			console_putc(argv[i][j]);
		console_putc(' ');
	}
	tf_printf("\b\n");

	return 0;
}

int call_as(int argc, char * const argv[])
{
	int i;
	if (argc < 3) {
		tf_printf("Wrong number of arguments. Should be called:\n");
		tf_printf("%s <cmd_to_run> <name_to_pass> args...\n", argv[0]);
		return -1;
	}
	i = find_func(argv[1]);
	if (i < 0) {
		tf_printf("No command named \"%s\" found.\n", argv[1]);
		return -1;
	}
	return func_list[i].func(argc - 2, argv + 2);
}

int repeat(int argc, char * const argv[])
{
	int i;
	int result = 0;
	int repeat;

	if (argc < 3) {
		tf_printf("Wrong number of arguments. Should be called:\n");
		tf_printf("%s <times_to_repeat> <command_to_repeat> args...\n",
			  argv[0]);
		return -1;
	}
	i = find_func(argv[2]);
	if (i < 0) {
		tf_printf("No command named \"%s\" found.\n", argv[2]);
		return -1;
	}

	/* get_arg_int contains it's own error message. */
	if (!get_arg_int(argv[1], &repeat, 1, 0, INT_MAX, "times to repeat"))
		return -1;

	for (int j = 0; j < repeat; j++) {
		result |= func_list[i].func(argc - 2, argv + 2);

		if (ctrlc()) {
			tf_printf("Terminated\n");
			break;
		}
	}

	return result;
}

/*
 * We are using the now unused SRAM where BL31 would be loaded to store all
 * our buffered commands.
 */
#define CMD_BASE	BL31_BASE
#define CMD_BUF		2048
#define CMD_CNT		(MAX_BL31_SIZE / CMD_BUF)
/* Used as: char cmds[CMD_CNT][CMD_BUF]; */
#define cmds		((char (*)[CMD_BUF])CMD_BASE)
static const char *cmd_help_str =
"This command buffer input from the console so that it can be later executed.\n"
"  cmd enter : Listen to all input in the console (without echoing) and\n"
"              buffer all inputs until '#' is entered.\n"
"              Each buffer sequence should begin with a '!' character\n"
"              followed by the sequence name then followed by the '<'\n"
"              character. Subsequent lines should be the sequence command\n"
"              body and ends with a '>' character at the end. The entire\n"
"              input should be terminated by a '#' character.\n"
"              An example input would be:\n"
"                !dump_ddr0_info<\n"
"                ddr_params DDR0 show\n"
"                pub_dump DDR0\n"
"                emi_dump DDR0\n"
"                emc_dump DDR0\n"
"                >\n"
"                !dump_ddr1_info<\n"
"                ddr_params DDR1 show\n"
"                pub_dump DDR1\n"
"                emi_dump DDR1\n"
"                emc_dump DDR1\n"
"                >\n"
"                #\n"
"              This should be in to a file and catted to the serial via:\n"
"                cat dump_ddr_cmds.txt > /dev/ttyUSB0\n"
"              after issuing this command.\n"
"  cmd ls    : Lists the sequence name of all the sequence buffered.\n"
"  cmd dump <sequence_name> :\n"
"              Dump the sequence content of the specified sequence.\n"
"  cmd run <sequence_name> :\n"
"              Execute the specified sequence.\n"
"  cmd help  : Show this help message\n";
int cmd(int argc, char * const argv[])
{
	enum ops{CMD_RUN, CMD_ENTER, CMD_LS, CMD_DUMP, CMD_HELP};

	int parsed;
	int op;
	int idx = 0;

	parsed = argc > 1 &&
		 get_arg_str(argv[1], &op, "op", 5,
			     "run", CMD_RUN,
			     "enter", CMD_ENTER,
			     "ls", CMD_LS,
			     "dump", CMD_DUMP,
			     "help", CMD_HELP);

	if (!parsed) {
		tf_printf("Insufficient argument, enter '%s help' for help\n",
			  argv[0]);
		return -1;
	}

	/* Find the command given. */
	if (op == CMD_RUN || op == CMD_DUMP) {
		if (argc < 3) {
			tf_printf("Missing sequence name to %s.\n", argv[1]);
			return -1;
		}
		idx = -1;
		for (int i = 0; i < CMD_CNT; i++) {
			if (cmds[i][0] != '!')
				continue;
			if (cmds[i][0] == '#')
				break;
			if (strncmp(argv[2], &cmds[i][1],
				    strlen(argv[2]) - 1) == 0 &&
			    cmds[i][strlen(argv[2]) + 1] == '<') {
				idx = i;
				break;
			}
		}
		if (idx == -1) {
			tf_printf("Sequence %s not found\n", argv[2]);
			return -1;
		}
	}

	switch (op) {
	case CMD_RUN: {
		int i = strlen(argv[2]) + 2;

		tf_printf("cmd>>");

		while (cmds[idx][i] != '>') {
			char buf[MAX_BUF_LEN];
			int len = 0;
			char *args[MAX_ARG_NUM];
			int arg_num, func_index;

			/* Buffer one line at a time. */
			while(cmds[idx][i] != '\r' && cmds[idx][i] != '\n') {
				buf[len++] = cmds[idx][i];
				console_putc(cmds[idx][i]);
				i++;
			}
			i++;
			arg_num = process_buffer(buf, len, args);
			buf[len] = 0;

			if (arg_num == 0)
				continue;

			/* Execute the current line's command. */
			func_index = find_func(args[0]);
			if (func_index == -1) {
				tf_printf("\nCommand '%s' not present!\ncmd>>",
					  args[0]);
				continue;
			}
			tf_printf("\n");
			func_list[func_index].func(arg_num, args);
			tf_printf("\ncmd>>");
		}
		/* Erase the last "cmd>>" */
		tf_printf("\r\x1B[J");
		break;
	}
	case CMD_ENTER: {
		int len = 0;

		/*
		 * Populate the first char of every buffer with '#' so that if
		 * we later read it and it's not '#' we know we have overritten
		 * it when loading the previous buffer's content.
		 */
		for (int i = 0; i < CMD_CNT; i++)
			cmds[i][0] = '#';
		tf_printf("Input commands now, or press '#' to return:\n");

		while (1) {
			char c = console_getc();

			if (c == '>' && len != 0) {
				cmds[idx][len++] = c;
				len = 0;
				do {
					idx++;
				} while(cmds[idx][0] != '#');
			} else if (c == '!' && len == 0) {
				cmds[idx][len++] = c;
			} else if (len) {
				cmds[idx][len++] = c;
			} else if (c == '#')
				break;
		}
		tf_printf("Finished input\n");
		break;
	}
	case CMD_LS:
		for (int i = 0; i < CMD_CNT; i++) {
			if (cmds[i][0] != '!')
				continue;
			if (cmds[i][0] == '#')
				break;
			int j = 1;

			while (cmds[i][j] != '<') {
				console_putc(cmds[i][j]);
				j++;
			}
			tf_printf("\n");
		}
		break;
	case CMD_DUMP: {
		int i = 0;

		while (cmds[idx][i] != '>')
			console_putc(cmds[idx][i++]);

		tf_printf("\n");
		break;
	}
	default:
		tf_printf(cmd_help_str);
	}

	return 0;
}

/*
 * Find the index of the given function name.
 * func_name: The name of the function to find.
 * return -1 if the function is not found, else the index of the function in
 * the func_list array.
 */
static int find_func(const char *func_name)
{
	for (int i = 0; i < FUNC_NUM; i++) {
		if (strcmp(func_list[i].func_name, func_name) == 0)
			return i;
	}
	return -1;
}

/* Refresh current displayed line on the console and return the buf size. */
static int update_line(const char *buf)
{
	/* \x1B[J clears the cursor to the end of the screen. */
	tf_printf("\r\x1B[J%s%s", PROMPT_STR, buf);

	/* Return the buf size. */
	return strlen(buf);
}

/* Parses the final character of cursor sequeces, e.g. 'A' in "^[A" for up. */
static void handle_arrow_keys(struct console_instance *ci)
{
	int c = console_getc();
	switch (c) {
	case 'A': /* Up */
		/*
		 * When the up key is pressed, we check if there is any
		 * previous command history. If there is, we switch the
		 * current buffer to display the previous one.
		 */
		/*
		 * We check if we are wrapping around to the buffer we would
		 * be filling here.
		 */
		if (ci->buf_idx == (ci->buf_next + 1) % MAX_BUF_LEN)
			return;
		/*
		 * If we are looping back in the circular buffer, we check
		 * that it indeed has a history stored. (It might not when
		 * the user has entered less than MAX_CMD_HIS commands.)
		 */
		if (ci->buf_idx == 0 && ci->bufs[MAX_CMD_HIS - 1][0] == 0)
			return;
		/*
		 * If this is the first time the user pressed the up key when
		 * entering the current command, we save what the user has
		 * entered so far so that pressing the down key would restore
		 * what the user entered so far.
		 */
		if (ci->buf_idx == ci->buf_next)
			memcpy(ci->bufs[ci->buf_idx], ci->buf, MAX_BUF_LEN);
		/* Switch buffer to previous entered command. */
		ci->buf_idx = (ci->buf_idx + MAX_CMD_HIS - 1) % MAX_CMD_HIS;
		memcpy(ci->buf, ci->bufs[ci->buf_idx], MAX_BUF_LEN);
		ci->buf_pos = ci->buf_len = update_line(ci->buf);
		break;
	case 'B': /* Down */
		/*
		 * When the down key is pressed, we see if the user has
		 * previously pressed the up key and switched the current
		 * buffer. If yes we switch back to the next buffer.
		 */
		if (ci->buf_idx == ci->buf_next)
			return;
		ci->buf_idx = (ci->buf_idx + 1) % MAX_CMD_HIS;
		memcpy(ci->buf, ci->bufs[ci->buf_idx], MAX_BUF_LEN);
		ci->buf_pos = ci->buf_len = update_line(ci->buf);
		break;
	case  'C': /* Right */
		/*
		 * We move the cursor right one charactor if it is not at the
		 * end of the current entered command string.
		 */
		if (ci->buf_len == ci->buf_pos)
			return;
		console_putc(ci->buf[ci->buf_pos]);
		ci->buf_pos++;
		break;
	case 'D': /* Left */
		/*
		 * We move the cursor left one charactor if it is not at the
		 * start of the current entered command string.
		 */
		if (ci->buf_len == 0)
			return;
		console_putc('\b');
		ci->buf_pos--;
		break;
	default:
		break;
	}
}

void get_next_cmd(struct console_instance *ci)
{
	int c;

	ci->buf_pos = 0;
	ci->buf_len = 0;
	ci->buf_idx = ci->buf_next;
	/*
	 * Clear the current buffer for fresh new user input. This is done so
	 * that the content of the previous buffer doesn't show up when the user
	 * press the up key and then the down key. Also we don't have to worry
	 * about NULL-terminating the string at various places.
	 */
	memset(ci->buf, 0, sizeof(ci->buf));

	while ((c = console_getc()) != '\r' && c != '\n') {

		if (c == 27) { /* Escape character */
			/* We only support arrow keys for the escape sequence */
			if (console_getc() == '[')
				handle_arrow_keys(ci);
			else
				continue;
		} else if (c == 127 || c == '\b') { /* 127 = DEL */
			/*
			 * If the user pressed backspace, clear the character
			 * before the buffer position.
			 */
			if (ci->buf_pos == 0)
				continue;
			memmove(&ci->buf[ci->buf_pos - 1],
				&ci->buf[ci->buf_pos],
				ci->buf_len - ci->buf_pos + 1);
			ci->buf_pos--;
			ci->buf_len--;
			/*
			 * Escape sequence to make all text right of the cursor
			 * move one charactor to the left.
			 */
			tf_printf("\b\x1B[P");

		} else if (ci->buf_len < MAX_BUF_LEN - 1 && isprint(c)) {
			if (ci->buf_pos != ci->buf_len) {
				memmove(&ci->buf[ci->buf_pos],
					&ci->buf[ci->buf_pos - 1],
					ci->buf_len - ci->buf_pos + 1);
				/*
				 * Escape sequence to make all text right of
				 * cursor move one charactor to the right.
				 */
				tf_printf("\x1B[@");
			}
			ci->buf[ci->buf_pos] = c;
			console_putc(c);
			ci->buf_pos++;
			ci->buf_len++;
		}
	}
	tf_printf("\n");
}

void bluefield_console(void)
{
	char *argv[MAX_ARG_NUM];
	int argc, exit_status;
	struct console_instance ci;

	tf_printf("Welcome to the DDR debug console.\n");
	tf_printf("Use \"list_func\" to see all available commands.\n");

	memset(&ci, 0, sizeof(ci));
	while (1) {
		tf_printf(PROMPT_STR);

		get_next_cmd(&ci);

		memcpy(ci.bufs[ci.buf_next], ci.buf, MAX_BUF_LEN);

		argc = process_buffer(ci.buf, ci.buf_len, argv);

		if (argc > 0) {
			/* Exit the console if we see "exit" or "quit". */
			if (strcmp(argv[0], "exit") == 0 ||
			    strcmp(argv[0], "quit") == 0)
				break;
			ci.buf_next = (ci.buf_next + 1) % MAX_CMD_HIS;
			int func_index = find_func(argv[0]);
			if (func_index == -1) {
				tf_printf("Command \"%s\" not available!\n",
					  argv[0]);
				tf_printf("Run \"list_func\" for a list of"
					  "commands.\n");
				continue;
			}
			exit_status = func_list[func_index].func(argc, argv);
			if (exit_status != 0)
				tf_printf("%s returned with non-zero exit code"
					  " %d.\n", argv[0], exit_status);
		}
	}
}
