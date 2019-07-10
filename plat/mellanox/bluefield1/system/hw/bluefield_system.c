/*
 * Copyright (c) 2018, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <debug.h>
#include <delay_timer.h>
#include <io_storage.h>
#include <mmio.h>
#include <platform.h>
#include "bluefield_ddr.h"
#include "bluefield_ddr_bist.h"
#include "bluefield_def.h"
#include "bluefield_private.h"
#include "io_flash.h"
#include "platform_def.h"
#include "tyu_def.h"
#include "rsh.h"

/*******************************************************************************
 * Candidates for the bf_sys_get_spd() function.
 ******************************************************************************/
/* Read the SPD. */
static int __bf_spd_read(uint8_t *spd, int offset, int len, int mss, int dimm)
{
	return bf_sys_get_spd_default(spd, offset, len, mss, dimm);
}

/* Skip reading the SPD. */
static int __bf_spd_skip(uint8_t *spd, int offset, int len, int mss, int dimm)
{
	return 0;
}

/*******************************************************************************
 * Candidates for the bf_sys_ddr_get_info_user() function.
 ******************************************************************************/
/* Get information solely from the SPD. */
static int __bf_user_noinfo(struct ddr_params *dp)
{
	return 1;
}
/* Fixed values for on board soldered 8 UDIMM. (BlueTang) */
static int __bf_user_soldered_8GU(struct ddr_params *dp)
{
	dp->tck = 833333;
	dp->type = UDIMM;
	dp->dimm_num = 1;
	dp->dimm[0].ranks = 1;
	dp->dimm[0].is_nvdimm = 0;
	dp->dimm[0].density = DENSITY_8Gbit;
	dp->dimm[0].capacity_gbit = 64;
	dp->speed_bin = DDR4_2666U;
	dp->package = PACKAGE_x8;
	dp->ddr_3ds = 1;
	dp->phy_rtd = dp->tck * 25 / 2;
	dp->mem_rtt_nom = RZQ_DIV_4;
	dp->mem_rtt_park = DISABLED;
	dp->mem_vref = 820;

	return 1;
}
/* Fixed values for on board soldered 8 UDIMM. (Lamina SmartNIC) */
static int __bf_user_soldered_8GU_delay(struct ddr_params *dp)
{
	dp->tck = 833333;
	dp->type = UDIMM;
	dp->dimm_num = 1;
	dp->dimm[0].ranks = 1;
	dp->dimm[0].is_nvdimm = 0;
	dp->dimm[0].density = DENSITY_8Gbit;
	dp->dimm[0].capacity_gbit = 64;
	dp->speed_bin = DDR4_2666U;
	dp->package = PACKAGE_x8;
	dp->ddr_3ds = 1;
	dp->phy_rtd = dp->tck * 25 / 2;
	dp->mem_rtt_nom = RZQ_DIV_4;
	dp->mem_rtt_park = DISABLED;
	dp->mem_vref = 820;
	dp->ddr_clk[0] = 80000;
	dp->ddr_clk[1] = 80000;
	return 1;
}
/* Fixed values for on board soldered 16G UDIMM. */
static int __bf_user_soldered_16GU(struct ddr_params *dp)
{
	dp->tck = 833333;
	dp->type = UDIMM;
	dp->dimm_num = 1;
	dp->dimm[0].ranks = 2;
	dp->dimm[0].is_nvdimm = 0;
	dp->dimm[0].density = DENSITY_8Gbit;
	dp->dimm[0].capacity_gbit = 128;
	dp->speed_bin = DDR4_2400R;
	dp->package = PACKAGE_x8;
	dp->ddr_3ds = 1;
	dp->phy_rtd = dp->tck * 25 / 2;

	return 1;
}

/* Fixed values for on board soldered 16G UDIMM delayed ddr_clk. */
static int __bf_user_soldered_16GU_delay(struct ddr_params *dp)
{
	dp->tck = 833333;
	dp->type = UDIMM;
	dp->dimm_num = 1;
	dp->dimm[0].ranks = 2;
	dp->dimm[0].is_nvdimm = 0;
	dp->dimm[0].density = DENSITY_8Gbit;
	dp->dimm[0].capacity_gbit = 128;
	dp->speed_bin = DDR4_2400R;
	dp->package = PACKAGE_x8;
	dp->ddr_3ds = 1;
	dp->phy_rtd = dp->tck * 25 / 2;
	dp->ddr_clk[0] = 80000;
	dp->ddr_clk[1] = 80000;

	return 1;
}

/*******************************************************************************
 * Candidates for the bf_sys_ddr_get_info_board() function.
 ******************************************************************************/
 /* Use the default analog parameter table. */
static int __bf_board_default(struct ddr_params *dp)
{
	return bf_sys_ddr_get_info_board_default(dp);
}

 /*
  * Mellanox boards using SODIMM have a differnet round trip delay.
  * NOTE that this value only stands true for the existing boards, NOT
  * all BlueField boards which use SODIMMs. Board partners which design
  * their own boards are strongly adviced to measure the round trip delay
  * of their own design and adjust the parameter to match it.
  */
static int __bf_board_sodimm(struct ddr_params *dp)
{
	dp->sysrtd_max = 2120 * FS_PER_PS;
	return bf_sys_ddr_get_info_board_default(dp);
}

 /*
  * Due to the PCB trace layout of the Sella board, additional delays are
  * required for CK and address/control signals.
  */
static int __bf_board_sella(struct ddr_params *dp)
{
	dp->sysrtd_max = 2120 * FS_PER_PS;
	dp->ddr_clk[0] = 250 * FS_PER_PS;
	dp->ddr_clk[1] = 250 * FS_PER_PS;
	dp->ddr_addr_ctrl = 250 * FS_PER_PS;
	return bf_sys_ddr_get_info_board_default(dp);
}

 /*
  * Mellanox boards using soldered DRAM have a differnet round trip delay.
  * NOTE that this value only stands true for the existing boards, NOT
  * all BlueField boards which use soldered DRAM. Board partners which design
  * their own boards are strongly adviced to measure the round trip delay
  * of their own design and adjust the parameter to match it.
  */
static int __bf_board_soldered(struct ddr_params *dp)
{
	dp->sysrtd_max = 1810 * FS_PER_PS;
	return bf_sys_ddr_get_info_board_default(dp);
}

/* Fixed parameters for the on board soldered 16G UDIMM. */
static int __bf_board_soldered_16GU(struct ddr_params *dp)
{
	dp->phy_wr_drv = RZQ_DIV_7;
	dp->phy_rd_odt = RZQ_DIV_5;
	dp->phy_rd_vref = 876;
	dp->mem_odic = RZQ_DIV_7;
	dp->mem_rtt_nom = RZQ_DIV_4;
	dp->mem_rtt_park = RZQ_DIV_1;
	dp->mem_vref = 849;
	dp->ddr_tcase = 80;
	dp->sysrtd_max = 1810 * FS_PER_PS;

	return 1;
}

/*******************************************************************************
 * All the current setup that we have currently.
 ******************************************************************************/
/* Functions which can sufficiently define a DDR setup. */
/* Do not setup the memory. */
const struct ddr_config mem_no = {
	.get_spd = __bf_spd_skip,
	.info_user = __bf_user_noinfo,
	.info_board = __bf_board_default,
	.config = "no-mem",
};

/* Setup the memory by just reading the SPD. */
const struct ddr_config mem_spd = {
	.get_spd = __bf_spd_read,
	.info_user = __bf_user_noinfo,
	.info_board = __bf_board_default,
	.config = "via-SPD",
};

/* Setup the memory by just reading the SPD (sodimms). */
const struct ddr_config mem_spd_sodimm = {
	.get_spd = __bf_spd_read,
	.info_user = __bf_user_noinfo,
	.info_board = __bf_board_sodimm,
	.config = "soD-SPD",
};

/* Setup the memory for Sella boards. */
const struct ddr_config mem_spd_sella = {
	.get_spd = __bf_spd_read,
	.info_user = __bf_user_noinfo,
	.info_board = __bf_board_sella,
	.config = "sella",
};

/* Setup the memory for soldered 8G UDIMM. (BlueTang) */
const struct ddr_config mem_8GU = {
	.get_spd = __bf_spd_skip,
	.info_user = __bf_user_soldered_8GU,
	.info_board = __bf_board_soldered,
	.config = "8G-U",
};

/* Setup the memory for soldered 8G UDIMM. (Lamina SmartNIC 8G) */
const struct ddr_config mem_8GUd = {
	.get_spd = __bf_spd_skip,
	.info_user = __bf_user_soldered_8GU_delay,
	.info_board = __bf_board_soldered,
	.config = "8G-Ud",
};

/* Setup the memory for soldered 16G UDIMM. */
const struct ddr_config mem_16GU = {
	.get_spd = __bf_spd_skip,
	.info_user = __bf_user_soldered_16GU,
	.info_board = __bf_board_soldered_16GU,
	.config = "16G-U",
};

/* Setup the memory for soldered 16G UDIMM delayed (Lamina Smartnic 16G */
const struct ddr_config mem_16GUd = {
	.get_spd = __bf_spd_skip,
	.info_user = __bf_user_soldered_16GU_delay,
	.info_board = __bf_board_soldered_16GU,
	.config = "16G-Ud",
};

/* All the different hardcoded memory configurations we have. */
const struct ddr_config *const ddr_fixed_configs[] = {
	&mem_16GU, &mem_16GUd, &mem_8GU, &mem_8GUd, NULL,
};
/*******************************************************************************
 * Code which tries all the setups
 ******************************************************************************/
const struct ddr_config *current_config;
static int reduced_freq; /* Set to train at the lowest reduced frequency. */

int bf_sys_get_spd(uint8_t *spd, int offset, int len, int mss, int dimm)
{
	return current_config->get_spd(spd, offset, len, mss, dimm);
}
int bf_sys_ddr_get_info_user(struct ddr_params *dp)
{
	int ret = current_config->info_user(dp);

	if (reduced_freq) {
		dp->tck = 1250000;
		dp->phy_rtd = dp->tck * 25 / 2;
	}

	return ret;
}
int bf_sys_ddr_get_info_board(struct ddr_params *dp)
{
	return current_config->info_board(dp);
}

static struct ddr_params *try_all_fixed_configs(uintptr_t mem_base,
						int mem_ctrl_num)
{
	struct ddr_params *dp = NULL;

	for (int i = 0; ddr_fixed_configs[i] != NULL; i++) {
		current_config = ddr_fixed_configs[i];
		dp = bluefield_setup_mss(mem_base, mem_ctrl_num);

		if (dp != NULL)
			break;
	}
	return dp;
}

static int mem_spd_present(int mem_ctrl_num)
{
	uint8_t c;
	int spd_present;
	int err_lv, verb_lv;

	/* Suppress all logs. */
	err_lv = ddr_error_level;
	verb_lv = ddr_verbose_level;
	ddr_error_level = 0;
	ddr_verbose_level = 0;
	tf_log_set_max_level(0);

	/* See if we have SPD present on this side. */
	spd_present = __bf_spd_read(&c, 0, 1, mem_ctrl_num, 0) ||
		      __bf_spd_read(&c, 0, 1, mem_ctrl_num, 1);

	/* Restore log level. */
	ddr_error_level = err_lv;
	ddr_verbose_level = verb_lv;
	tf_log_set_max_level(LOG_LEVEL);

	return spd_present;
}

/* Try to bring up the memory with all of the configurations. */
static struct ddr_params *heuristic_mem_config(uintptr_t mem_base,
					       int mem_ctrl_num)
{
	struct ddr_params *dp = NULL;
	int err_lv, verb_lv;

	reduced_freq = 0;

	/* If SPD is present, bring up the memory with it. */
	if (mem_spd_present(mem_ctrl_num)) {
		current_config = &mem_spd;
		dp = bluefield_setup_mss(mem_base, mem_ctrl_num);

		/* If we failed to bring up, try reduced frequency. */
		if (dp == NULL) {
			reduced_freq = 1;
			dp = bluefield_setup_mss(mem_base, mem_ctrl_num);
		}
	} else { /* If no SPD, try all fixed configurations. */
		/*
		 * Suppress all logs as wrong configuration would generate
		 * a lot of unhelpful error messages.
		 */
		err_lv = ddr_error_level;
		verb_lv = ddr_verbose_level;
		ddr_error_level = 0;
		ddr_verbose_level = 0;
		tf_log_set_max_level(0);

		dp = try_all_fixed_configs(mem_base, mem_ctrl_num);
		if (dp == NULL) {
			/* Try again with reduced frequency. */
			reduced_freq = 1;
			dp = try_all_fixed_configs(mem_base, mem_ctrl_num);
		}
		/* Restore log level. */
		ddr_error_level = err_lv;
		ddr_verbose_level = verb_lv;
		tf_log_set_max_level(LOG_LEVEL);
	}

	/* If no memory is found, do the setup for no memory. */
	if (dp == NULL) {
		current_config = &mem_no;
		bluefield_setup_mss(mem_base, mem_ctrl_num);

		INFO("No memory on MSS %d\n", mem_ctrl_num);
		return NULL;
	} else {
		NOTICE("Brought up MSS %d using %s config%s.\n",
		       mem_ctrl_num,
		       current_config->config,
		       reduced_freq ? " reduced" : "");
	}
	reduced_freq = 0;

	return dp;
}
/*******************************************************************************
 * All known setup configurations
 ******************************************************************************/
const struct sys_info known_systems[] = {
	{	/* BlueTang */
		.mss0_config = &mem_spd,
		.mss1_config = &mem_8GU,
		.sys_name = "bluetang",
		.psid_vals = (uint16_t []){0}, /* No official PSID */
		.snic_model = 0,
	},
	{	/* BlueWhale */
		.mss0_config = &mem_spd,
		.mss1_config = &mem_spd,
		.sys_name = "MBE1x0x",
		.psid_vals = (uint16_t []){118, 117, 121, 120, 122, 119, 0},
		.snic_model = 0,
	},
	{	/* SmartNIC/Bekka - 16 cores/8G DDR */
		.mss0_config = &mem_no,
		.mss1_config = &mem_8GU,
		.sys_name = "MBF1M322A-AS",
		.psid_vals = (uint16_t []){311, 11, 0},
		.snic_model = 0,
	},
	{	/* SmartNIC/Bekka - 4 cores */
		.mss0_config = &mem_no,
		.mss1_config = &mem_16GU,
		.sys_name = "MBF1L332A-AF",
		.psid_vals = (uint16_t []){169, 168, 312, 315, 0},
		.snic_model = 2,
	},
	{	/* SmartNIC/Bekka - 8 cores */
		.mss0_config = &mem_no,
		.mss1_config = &mem_16GU,
		.sys_name = "MBF1M332A-AE",
		.psid_vals = (uint16_t []){131, 154, 313, 316, 0},
		.snic_model = 1,
	},
	{	/* SmartNIC/Bekka - 16 cores */
		.mss0_config = &mem_no,
		.mss1_config = &mem_16GU,
		.sys_name = "MBF1M332A-AS",
		.psid_vals = (uint16_t []){229, 230, 308, 314, 317, 0},
		.snic_model = 0,
	},
	{	/* SmartNIC/Bekka - 2 cores */
		.mss0_config = &mem_no,
		.mss1_config = &mem_16GU,
		.sys_name = "MBF1L332A-AT",
		.psid_vals = (uint16_t []){176, 177, 0},
		.snic_model = 3,
	},
	{	/* 8G Lamina SmartNIC */
		.mss0_config = &mem_no,
		.mss1_config = &mem_8GUd,
		.sys_name = "MBF1L516A-CSNAT_C11",
		.psid_vals = (uint16_t []){5, 0},
		.snic_model = 0,
	},
	{	/* Lamina */
		.mss0_config = &mem_no,
		.mss1_config = &mem_16GUd,
		.sys_name = "MBF1L516",
		.psid_vals = (uint16_t []){244, 245, 246, 247, 263, 0},
		.snic_model = 0,
	},
	{	/* Sella - 8 cores */
		.mss0_config = &mem_spd_sella,
		.mss1_config = &mem_spd_sella,
		.sys_name = "MBF1M6x6A-CE",
		.psid_vals = (uint16_t []){285, 287, 0},
		.snic_model = 1,
	},
	{	/* Sella - 16 cores */
		.mss0_config = &mem_spd_sella,
		.mss1_config = &mem_spd_sella,
		.sys_name = "MBF1M6x6A-CS",
		.psid_vals = (uint16_t []){3, 286, 288, 289, 0},
		.snic_model = 0,
	},
	{	/* Sella-SmartNIC */
		.mss0_config = &mem_spd_sella,
		.mss1_config = &mem_spd_sella,
		.sys_name = "MBF1L666A-ES",
		.psid_vals = (uint16_t []){290, 291, 0},
		.snic_model = 0,
	},
	{	/* Sella */
		.mss0_config = &mem_spd_sella,
		.mss1_config = &mem_spd_sella,
		.sys_name = "MBF1M656A-ESNAT_AX",
		.psid_vals = (uint16_t []){323, 0},
		.snic_model = 0,
	},
	{	/* Sella (deprecated) - 8 cores */
		.mss0_config = &mem_spd_sodimm,
		.mss1_config = &mem_spd_sodimm,
		.sys_name = "MBF1M6x6A-CE",
		.psid_vals = (uint16_t []){163, 164, 173, 0},
		.snic_model = 1,
	},
	{	/* Sella (deprecated) - 16 cores */
		.mss0_config = &mem_spd_sodimm,
		.mss1_config = &mem_spd_sodimm,
		.sys_name = "MBF1M6x6A-CS",
		.psid_vals = (uint16_t []){172, 175, 197, 6, 0},
		.snic_model = 0,
	},
	{	/* Sella-SmartNIC (deprecated) */
		.mss0_config = &mem_spd_sodimm,
		.mss1_config = &mem_spd_sodimm,
		.sys_name = "MBF1L666A-ES",
		.psid_vals = (uint16_t []){211, 210, 0},
		.snic_model = 0,
	},
	{	/* Denarious */
		.mss0_config = &mem_spd_sodimm,
		.mss1_config = &mem_spd_sodimm,
		.sys_name = "MBF1M216A-CS",
		.psid_vals = (uint16_t []){0}, /* PSID not given yet */
		.snic_model = 0,
	},
	{	/* Poondion */
		.mss0_config = &mem_spd,
		.mss1_config = &mem_spd,
		.sys_name = "MBF1M7x6A-CS",
		.psid_vals = (uint16_t []){132, 188, 0},
		.snic_model = 0,
	},
};

#if 0
void dump_data(void)
{
	for (int i = 0; i < ARRAY_SIZE(known_systems); i++) {
		tf_printf("System %s (snic %d):\n", known_systems[i].sys_name,
						   known_systems[i].snic_model);
		tf_printf("MSS 0 %s config\n",
					known_systems[i].mss0_config->config);
		tf_printf("MSS 1 %s config\n",
					known_systems[i].mss1_config->config);
		tf_printf("PSIDs:");
		for (int j = 0; known_systems[i].psid_vals[j] != 0; j++)
			tf_printf(" %d", known_systems[i].psid_vals[j]);
		tf_printf("\n");
	}
}
#endif

/*******************************************************************************
 * The actual setup code
 ******************************************************************************/
static const struct sys_info *__current_sys; /* The system we are running as */

/*
 * Read the sysinfo from the bfb and store it where BL31 would be loaded.
 * As it's not known at this point if it is the OPN or PSID, make both point
 * to it.
 */
static struct bluefield_dev_info get_sysinfo_via_bfb(void)
{
	struct bluefield_dev_info di = { .opn = NULL, .psid = NULL};
	uintptr_t dev_handle;
	uintptr_t image_handle;
	uintptr_t image_spec;
	size_t image_size;
	size_t bytes_read;

	/*
	 * Try to open the file in bfb describing what is the system.
	 * Note that sequence is important here, as the 'or' implies
	 * that the following function would only be called if the
	 * previous one returns 0 (which is what it returns if successful).
	 */
	if (plat_get_image_source(BF_SYS_FILE, &dev_handle, &image_spec) ||
	    io_open(dev_handle, image_spec, &image_handle) ||
	    io_size(image_handle, &image_size) ||
	    image_size == 0) {
		INFO("No device description file found\n");
		return di;
	}

	if(image_size > MAX_BL31_SIZE) {
		WARN("Device description file obviously too large\n");
		return di;
	}

	if (io_read(image_handle, BL31_BASE, image_size, &bytes_read) ||
	    (bytes_read < image_size)) {
		WARN("Device description file read error\n");
		return di;
	}

	INFO("Device description is [%s]\n", (const char *)BL31_BASE);

	io_close(image_handle);
	io_dev_close(dev_handle);

	di.opn = (char *)BL31_BASE;
	di.psid = (char *)BL31_BASE;

	return di;
}

/*
 * Read the sysinfo information from the SPI flash, this should be reliable as
 * long as flash isn't corrupted.
 */
static struct bluefield_dev_info get_sysinfo_via_flash(void)
{
	return flash_io_get_dev_info((void *)BL31_BASE, MAX_BL31_SIZE);
}

/*
 * Weak function which returns the PSID/sys name string of the running sys.
 * Override this function to change the default behaviour of the generic sys.
 * We are temporary using the space to load BL31 to store this string.
 */
#pragma weak bf_sys_get_system_str
struct bluefield_dev_info bf_sys_get_system_str(void)
{
	struct bluefield_dev_info di = { .opn = NULL, .psid = NULL};
	int i = 0;

	struct bluefield_dev_info(* const get_sysinfo_methods[])(void) = {
		get_sysinfo_via_bfb,
		get_sysinfo_via_flash,
	};

	while (di.opn == NULL && di.psid == NULL &&
	       i < ARRAYSIZE(get_sysinfo_methods))
		di = get_sysinfo_methods[i++]();

	return di;
}

/*
 * Find the system name in the known_systems list.
 * Note that since similar system names are merged into one entry in the list,
 * they shall also be matched. E.g. "MBF1M616A-CECAT" and "MBF1M6x6A-CE" shall
 * match.
 */
static int get_sys_idx_via_name(const char *sys_name)
{
	if (sys_name == NULL)
		return -1;

	for (int i = 0; i < ARRAY_SIZE(known_systems); i++) {
		const char *match_name = known_systems[i].sys_name;
		int j;

		for (j = 0; match_name[j] != 0 && sys_name[j] != 0; j++) {
			if (match_name[j] != 'x' &&
			    match_name[j] != sys_name[j])
				break;
		}
		if (match_name[j] == 0)
			return i;
	}
	return -1;
}

/*
 * Find the given PSID in the known_systems list. The given string might be
 * something like "MT_0000000118" so we have to ignore the non-number text
 * at the beginning.
 */
static int get_sys_idx_via_psid(const char *psid_str)
{
	int psid = -1;

	if (psid_str == NULL)
		return -1;

	for (int i = 0; psid_str[i] != 0; i++) {
		if ((psid_str[i] >= '0') && (psid_str[i] <= '9')) {
			psid = (int)strtol(&psid_str[i], NULL, 10);
			break;
		}
	}
	if (psid <= 0)
		return -1;

	for (int i = 0; i < ARRAY_SIZE(known_systems); i++) {
		for (int j = 0; known_systems[i].psid_vals[j] != 0; j++) {
			if (psid == known_systems[i].psid_vals[j])
				return i;
		}
	}

	return -1;
}

static int bf_sys_get_system_type(void)
{
	int idx = -1;
	struct bluefield_dev_info di = bf_sys_get_system_str();

	if (di.opn == NULL && di.psid == NULL)
		return idx;

	/*
	 * The PSID takes priority over the OPN as there are boards which
	 * have the same OPN but different PSID that should make the board
	 * behave differently. We still use the OPN as a backup if no known
	 * PSID matches as for new variants of a board the OPN might still
	 * match and thus the board can still boot up normally.
	 */
	idx = get_sys_idx_via_psid(di.psid);

	if (idx < 0)
		idx = get_sys_idx_via_name(di.opn);

	/* @TODO: If still idx < 0, read breadcrumb register. */

	return idx;
}

/* Returns pointer to the system running, or NULL if can't be figured out. */
const struct sys_info *get_current_sys(void)
{
	static int initialized;

	if (!initialized) {
		int idx = bf_sys_get_system_type();

		if (idx >= 0)
			__current_sys = &known_systems[idx];
		else
			__current_sys = NULL;

		if (__current_sys != NULL)
			NOTICE("Running as %s system\n",
				__current_sys->sys_name);
		else {
			tf_printf("\n***System type can't be determined***\n");
			tf_printf("***Booting as a minimal system***\n\n");
			/* Delay 5s for people to notice message */
			mdelay(5000);
		}

		initialized = 1;
	}
	return __current_sys;
}

/* Called when using the given system's mem config doesn't pass setup. */
static void revoke_current_sys(void)
{
	tf_printf("\n***System type misconfigured***\n");
	tf_printf("***Booting as a minimal system***\n\n");
	/* Delay 5s for people to notice message */
	mdelay(5000);
	__current_sys = NULL;
}
/* __current_sys shouldn't be used directly. */
#pragma GCC poison __current_sys

int bf_sys_mem_config(uintptr_t mem_base,
		      struct bf_mem_ctrl_info *mem_ctrl_info,
		      int mem_ctrl_num)
{
	struct ddr_params *dp = NULL;
	const struct sys_info *current_sys = get_current_sys();
	int err_lv, verb_lv;

	if (current_sys != NULL) {
		if (mem_ctrl_num == 0)
			current_config = current_sys->mss0_config;
		else
			current_config = current_sys->mss1_config;

		dp = bluefield_setup_mss(mem_base, mem_ctrl_num);
	} else {
		/*
		 * Set up the memory controller as no memory when initial
		 * configurations is unknown.
		 */
		err_lv = ddr_error_level;
		verb_lv = ddr_verbose_level;
		ddr_error_level = 0;
		ddr_verbose_level = 0;
		tf_log_set_max_level(0);
		current_config = &mem_no;
		bluefield_setup_mss(mem_base, mem_ctrl_num);
		ddr_error_level = err_lv;
		ddr_verbose_level = verb_lv;
		tf_log_set_max_level(LOG_LEVEL);
		return 0;
	}

	if (dp == NULL)
		return 0;

	bluefield_fill_mem_ctrl_info(dp, mem_ctrl_info);

	return dp->dimm_num > 0;
}

/*
 * This function is only called when the initial memory bringup using the
 * specified system's configuration resulted in no memory being brought up.
 */
int bf_sys_mem_try_cfg(uintptr_t mem_base,
		       struct bf_mem_ctrl_info *mem_ctrl_info,
		       int mem_ctrl_num)
{
	const struct sys_info *current_sys;
	struct ddr_params *dp = NULL;

	dp = heuristic_mem_config(mem_base, mem_ctrl_num);

	if (dp == NULL)
		return 0;

	bluefield_fill_mem_ctrl_info(dp, mem_ctrl_info);

	/*
	 * If this point is reached, it means that memory was successfully
	 * brought up using a different configuration than the specified
	 * system's one, and thus the specified system must be wrong.
	 */
	current_sys = get_current_sys();
	if (current_sys != NULL)
		revoke_current_sys();

	return dp->dimm_num > 0;
}

int bf_sys_snic_model(void)
{
	const struct sys_info *current_sys = get_current_sys();

	if (current_sys == NULL)
		return 3;

	return current_sys->snic_model;
}
