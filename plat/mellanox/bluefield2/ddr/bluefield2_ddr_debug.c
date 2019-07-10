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
#include <debug.h>
#include <delay_timer.h>
#include <limits.h>
#include <mmio.h>
#include <platform_def.h>
#include <string.h>

#include "bluefield_private.h"

#define WARNING_RET_VAL		0x00000005
#define INVALID_VAL		(-1)
#define D2_VREF_MAX_VAL		74
#define D2_R_VREF_MAX_VAL	64
#define D2_DLL_MAX_VAL		128


/*
 * Change the stored DDR params according to the desired freqeuncy.
 * Call do_configure_emem() after this to set up the memory according to the new
 * frequency.
 */
int ddr_operating_frequency_change(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

/* Rerun the memory configuration setup. */
int do_configure_emem(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

/*
 * Find the register by name/address.
 * If found, return the index of the register in the array, else return -1.
 */
int find_record(int reg_type, int addr_mode, int address, char *name)
{
	ERROR("Function unimplemented\n");
	return -1;
}

/*
 * This command is in charge of basic memory register access. It should be
 * called by one of it's alias names to do the corresponding action.
 */
int do_ddr_basic(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

/*
 * This commands sets up the specified dto signal to the output pad for probing.
 */
int do_dto_probing(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

/*
 * This commands sets up the specified ato signal to the output pad for probing.
 */
int do_ato_probing(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

/*
 * This commands sets up the specified Vref signal to the output pad for probing.
 */
int do_vref_probing(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}


/* Modify a Single Byte Lane Read/Write Centralization TAP. */
int edit_xctap(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

/* Modify a Single DQ Read/Write Delay TAP. */
int edit_xdtap(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

/* Modify a Single Data Lane DQS Write Leveling. */
int edit_wltap(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

/* Shift the output enable timing. */
int edit_wetap(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

/* This command modifies the DDR PHY TAP results. */
int do_edit_tap_results(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

/* This command modifies the Vref results. */
int do_vref(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

int do_phy_bist(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

void d2_eye_plotting(int rank_idx, int is_write, int bl_map, int nb_map)
{
	ERROR("Function unimplemented\n");
}

int do_marg_and_d2(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

/* Function which turns on/off the pci and verbose logging. */
int log(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

/* Function which manages the DDR params struct. */
int ddr_params(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

/* Function which let user manually update the DDR configuration */
int ddr_config_change(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

int ddr_sweep(int argc, char * const argv[])
{
	ERROR("Function unimplemented\n");
	return -1;
}

/* The type of value the option should receive. */
enum param_type {
	TYPE_INT, TYPE_STR
};

/* A struct describing a valid string option value. */
struct setting_param_val {
	int  val;	/* The value stored. */
	char *name;	/* The name of this option. */
	char *desc;	/* Optional description if not NULL. */
};

/* A struct describing the values if the parameter takes str values. */
struct settings_params_str {
	int num;	/* How many options are supported. */
	const struct setting_param_val *vals; /* The options. */
};

/* A struct describing the values if the parameter takes int values. */
struct settings_params_int {
	int is_bounded;	/* True if the value is size constrained. */
	int min;	/* Min (inclusive) value if size constrained. */
	int max;	/* Max (exclusive) value if size constrained. */
};

struct setting_var {
	char *name;	/* Name of the parameter. */
	char *desc;	/* Description of the parameter. */
	int  *addr;	/* Address parameter is stored. */
	enum param_type type; /* Parameter type */
	union {
		struct settings_params_str str;
		struct settings_params_int num;
	}val;	/* Description of the valid values. */
};

/* List of parameters that we can change */
static const struct setting_var params_info[] = {};

/* Print out the given value of the given parameter. */
static void print_current_param_val(const struct setting_var *sv)
{
	tf_printf("<%s> : %s\n", sv->name, sv->desc);
	tf_printf("\tCurrent value : ");
	if (sv->type == TYPE_INT)
		tf_printf("%d (0x%x)", *sv->addr, *sv->addr);
	else {
		int j = 0;
		while (j < sv->val.str.num &&
		       *sv->addr != sv->val.str.vals[j].val)
			j++;
		if (j == sv->val.str.num)
			tf_printf("<unknown value %d>", *sv->addr);
		else {
			tf_printf("%s", sv->val.str.vals[j].name);
			if (sv->val.str.vals[j].desc != NULL)
				tf_printf(" (%s)", sv->val.str.vals[j].desc);
		}
	}
	tf_printf("\n\n");
}

/* Print out all the valid options for an parameter. */
static void print_param_valid_vals(const struct setting_var *sv)
{
	/* First print out the parameter's current value. */
	print_current_param_val(sv);

	if (sv->type == TYPE_INT) {
		/* No need to print anything else if value is not bounded. */
		if (sv->val.num.is_bounded == 0)
			return;
		if (sv->val.num.min == INT_MIN)
			tf_printf("\tValue must be less than %d.\n",
				  sv->val.num.max);
		else if (sv->val.num.max == INT_MAX)
			tf_printf("\tValue must be greater or equal to %d.\n",
				  sv->val.num.min);
		else
			tf_printf("\tValue must be in [%d, %d).\n",
				  sv->val.num.min, sv->val.num.max);
		return;
	}
	/* sv->type == TYPE_STR */
	tf_printf("\tValid values are:\n");
	for (int i = 0; i < sv->val.str.num; i++) {
		tf_printf("\t\t%s", sv->val.str.vals[i].name);
		if (sv->val.str.vals[i].desc != NULL)
			tf_printf(" : %s", sv->val.str.vals[i].desc);
		tf_printf("\n");
	}
}

/* Find the index of the given argument in the parameter list. */
static int find_param(const struct setting_var *sv_l, int len, char * const arg)
{
	for (int i = 0; i < len; i++) {
		if (!strcmp(arg, sv_l[i].name)) {
			return i;
		}
	}
	return -1;
}

/*
 * Change the given parameter based on the entered value.
 * Return 1 if the change was successful or 0 if the argument given is invalid.
 */
static int change_param(const struct setting_var *sv, char * const arg)
{
	int idx = -1;

	if(sv->type == TYPE_INT)
		return get_arg_int(arg,
				   sv->addr,
				   sv->val.num.is_bounded,
				   sv->val.num.min,
				   sv->val.num.max,
				   sv->name);
	/* sv->type == TYPE_STR */
	for (int i = 0; i < sv->val.str.num; i++) {
		if (!strcmp(arg, sv->val.str.vals[i].name)) {
			idx = i;
			break;
		}
	}
	if (idx == -1) {
		tf_printf("Value \"%s\" not supported.\n", arg);
		return 0;
	}

	*sv->addr = sv->val.str.vals[idx].val;

	return 1;
}

/*
 * This command allows the user to change some global parameter values
 * affecting various commands in the ATF console.
 */
int settings(int argc, char * const argv[])
{
	int param_idx;

	/* Print out all the parameter values if no parameters are specified. */
	if (argc == 1) {
		tf_printf("Settings supports the following values:\n");
		for (int i = 0; i < ARRAY_SIZE(params_info); i++) {
			print_current_param_val(&params_info[i]);
		}
		return 0;
	}

	/* If at least one argument is specified, it must be the param name. */
	param_idx = find_param(params_info, ARRAY_SIZE(params_info), argv[1]);

	if (param_idx < 0) {
		tf_printf("Specified parameter [%s] not found.\n", argv[1]);
		tf_printf("Supported parameters are:\n");
		for (int i = 0; i < ARRAY_SIZE(params_info); i++) {
			tf_printf("\t%s\n", params_info[i].name);
		}
		return -1;
	}

	/*
	 * If only one arguement (beside function name) is specified, details
	 * about that parameter is printed out.
	 */
	if (argc == 2) {
		print_param_valid_vals(&params_info[param_idx]);
		return 0;
	}

	/* We have argc >= 3, treat it as to change the parameter values. */
	if (!change_param(&params_info[param_idx], argv[2]))
		return -1;

	return 0;
}
