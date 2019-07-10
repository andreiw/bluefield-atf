/*
 * Copyright (c) 2017 - 2019, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_helpers.h>
#include <console.h>
#include <debug.h>
#include <mmio.h>
#include <string.h>
#include "bluefield_def.h"
#include "io_flash.h"
#include "rsh.h"

/* Magic number which enables the life cycle fuse update. */
#define		LIFE_CYCLE_EN_WORD	(0x65862b194ecb053d)

#if defined(INIT_SBKEY) || defined(UPDATE_SBKEY)
extern const uint8_t fuse_data[BF_KEY_SIZE_BYTES];
#endif
#ifdef SET_RMA
extern const uint8_t exp_chip_id[BF_CHIP_ID_SIZE_BYTES];
#endif

#if defined(INIT_SBKEY) || defined(UPDATE_SBKEY)
static int fls(unsigned int x)
{
	return x ? sizeof(unsigned int) * 8 - __builtin_clz(x) : 0;
}

/* Write new fuse data to the secure boot fuses. Return 0 if success else 1. */
static int program_sb_fuses(const uint8_t *fuse_data)
{
	RSH_SB_KEY_ACCESS_t rska;
	RSH_SB_KEY_VLD_t rskv;
	RSH_SB_KEY_WRITE_CTL_t rskwc, rskwc_st;

	rska.word = mmio_read_64(RSHIM_BASE + RSH_SB_KEY_ACCESS);
	if (rska.write_disable & (1 << BF_USE_KEY_NUM)) {
		ERROR("Fuses disabled for updating\n");
		return 1;
	}
	rska.efuse_reread_enable = 1;
	mmio_write_64(RSHIM_BASE + RSH_SB_KEY_ACCESS, rska.word);

	rskv.word = mmio_read_64(RSHIM_BASE + RSH_SB_KEY_VLD);

	int next_free_version = fls(rskv.BF_KEY_VLD & 0xf);

	if (next_free_version > 3) {
		ERROR("All the key versions are used. Can not set a new key\n");
		return 1;
	}

	/* Start programming the keys! */
	rskwc = (RSH_SB_KEY_WRITE_CTL_t){
		.key = BF_USE_KEY_NUM,
		.version = next_free_version,
		.step = RSH_SB_KEY_WRITE_CTL__STEP_VAL_START,
		.go = 1,
	};

	mmio_write_64(RSHIM_BASE + RSH_SB_KEY_WRITE_CTL, rskwc.word);
	do {
		rskwc_st.word = mmio_read_64(RSHIM_BASE + RSH_SB_KEY_WRITE_CTL);
	} while(rskwc_st.go);

	/* Write the key bits word by word! */
	rskwc.step = RSH_SB_KEY_WRITE_CTL__STEP_VAL_DATA;
	for (int i = 0; i < BF_KEY_SIZE_WORDS; i++) {
		rskwc.segment = i;
		mmio_write_64(RSHIM_BASE + RSH_SB_KEY_WRITE_DATA,
			      ((uint64_t *)fuse_data)[i]);
		mmio_write_64(RSHIM_BASE + RSH_SB_KEY_WRITE_CTL, rskwc.word);
		do {
			rskwc_st.word = mmio_read_64(RSHIM_BASE +
						     RSH_SB_KEY_WRITE_CTL);
		} while(rskwc_st.go);
	}

	/* Finish programming the keys! */
	rskwc.step = RSH_SB_KEY_WRITE_CTL__STEP_VAL_END;
	mmio_write_64(RSHIM_BASE + RSH_SB_KEY_WRITE_CTL, rskwc.word);
	do {
		rskwc_st.word = mmio_read_64(RSHIM_BASE + RSH_SB_KEY_WRITE_CTL);
	} while(rskwc_st.go);

	NOTICE("Finished writing the fuses\n");

	return 0;
}

/*
 * Read the current fuse bits and compare it with the given one.
 * data: Pointer to the start bit of the array of data to write.
 * Returns 0 if the given data matches the fuse, else nonzero.
 */
static int cmp_current_sb_fuse(const uint8_t *given_data)
{
	uint64_t read_data[BF_KEY_SIZE_WORDS];

	for (int i = 0; i < BF_KEY_SIZE_WORDS; i++)
		read_data[i] = mmio_read_64(BF_TBB_KEY_REG_START +
						  i * sizeof(uint64_t));

	return memcmp(given_data, read_data, BF_KEY_SIZE_BYTES);
}
#endif

#if defined(INIT_SBKEY) || defined(SET_RMA)
/*
 * Update the chip status that is in the chip life cycle fuses.
 * Argument passed should only be one of the BF_CHIP_STATE_XXX macros.
 * Return 0 on success, else 1.
 */
static int update_chip_state(int new_state)
{
	RSH_LIFE_CYCLE_UPDATE_t rlcu;

	/* Enable updates to the life cycle state. */
	mmio_write_64(RSHIM_BASE + RSH_LIFE_CYCLE_ENABLE, LIFE_CYCLE_EN_WORD);

	/* Update the life cycle fuses. */
	rlcu = (RSH_LIFE_CYCLE_UPDATE_t){
#ifdef RSH_LIFE_CYCLE_UPDATE__NEW_STATE_FIELD
		.new_state = new_state,
#else
		.new_lc = new_state,
#endif
		.go = 1,
	};
	mmio_write_64(RSHIM_BASE + RSH_LIFE_CYCLE_UPDATE, rlcu.word);

	/* Check for completion. */
	do {
		rlcu.word = mmio_read_64(RSHIM_BASE + RSH_LIFE_CYCLE_UPDATE);
	} while (rlcu.go);

	/* Check if update was successful. */
	if (rlcu.status) {
		ERROR("Write was not allowed\n");
		return 1;
	}
	NOTICE("Chip status update completed\n");
	return 0;
}
#endif

#ifdef SET_RMA
/*
 * Function which generates the unique id of the current chip and returns a
 * pointer pointing to it.
 */
static const uint8_t *get_unique_chip_id(void)
{
	static uint8_t chip_id[BF_CHIP_ID_SIZE_BYTES] = {
		0xb1, 0xef, 0x1e, 0x1d, 0xb1, 0xef, 0x1e, 0x1d
	};
	int size;
	uint8_t *buf = (void *)BL31_BASE;
	const int GUI_MAC_START_OFFSET = 0x20;

	size = flash_io_get_mfg_info_section(buf, MAX_BL31_SIZE);

	if (size < GUI_MAC_START_OFFSET)
		return chip_id;

	int j = 0;
	for (int i = GUI_MAC_START_OFFSET; i < size; i++) {
		chip_id[j++] ^= buf[i];
		if (j >= BF_CHIP_ID_SIZE_BYTES)
			j = 0;
	}

	return chip_id;
}
#endif

#ifdef INIT_SBKEY
/* Function which initializes the secure boot fuses. */
static void bluefield_init_sbkey(RSH_SB_MODE_t rsb)
{
	NOTICE("Running code to initialize secure boot fuse\n");

	if (rsb.secure != BF_CHIP_STATE_NONSECURE) {
		ERROR("Chip not in non-secure state! State=%d\n", rsb.secure);
		ERROR("Can not initialize chip\n");
		return;
	}

	if (!cmp_current_sb_fuse(&fuse_data[0])) {
		ERROR("Chip already has current fuse bits set\n");
		NOTICE("Perhaps previous state update unsuccessful?\n");
	} else {
		NOTICE("Initializing the fuses.\n");
		if (program_sb_fuses(&fuse_data[0])) {
			ERROR("Fuse initializing failed\n");
			return;
		}
	}
	NOTICE("Updating chip state from non-secure to secure\n");
	if (update_chip_state(BF_CHIP_STATE_SECURE)) {
			ERROR("Fuse initializing failed\n");
			return;
	}
	NOTICE("Chip fuse initialization successful\n");
}
#endif

#ifdef UPDATE_SBKEY
/* Function which updates the secure boot fuses. */
static void bluefield_update_sbkey(RSH_SB_MODE_t rsb)
{
	NOTICE("Running code to update secure boot fuse\n");

	if (rsb.secure != BF_CHIP_STATE_SECURE) {
		ERROR("Chip not in secure state! State=%d\n", rsb.secure);
		ERROR("Can not update chip fuses\n");
		if (rsb.secure == BF_CHIP_STATE_NONSECURE)
			NOTICE("Perhaps you want to initialize the chip?\n");
		return;
	}
	if (!cmp_current_sb_fuse(&fuse_data[0])) {
		ERROR("Chip already has current fuse bits set\n");
		NOTICE("No point in burning another set of fuses.\n");
		NOTICE("Perhaps you want to run the regular boot code?\n");
		return;
	}
	NOTICE("Updating the fuses\n");
	if (program_sb_fuses(&fuse_data[0])) {
		ERROR("Fuse updating failed\n");
		return;
	}

	NOTICE("Chip fuse update successful\n");
}
#endif

#ifdef SET_RMA
/* Function which sets the chip to RMA mode. */
static void bluefield_set_rma(RSH_SB_MODE_t rsb)
{
	const uint8_t *current_chip_id;

	NOTICE("Running code to put chip to RMA state\n");

	current_chip_id = get_unique_chip_id();

	if (memcmp(current_chip_id, &exp_chip_id[0], BF_CHIP_ID_SIZE_BYTES)) {
		ERROR("Chip ID doesn't match current chip\n");
		NOTICE("Current chip id: ");
#if LOG_LEVEL >= LOG_LEVEL_NOTICE
		/*
		 * tf_printf supports only a subset of the formatting options
		 * found in printf to keep its code size small, and sadly
		 * "%02x" is one of the formats it doesn't support. Thus we
		 * have to manually implement something of the same effect
		 * here.
		 */
		for (int i = 0; i < BF_CHIP_ID_SIZE_BYTES; i++) {
			if (current_chip_id[i] < 0x10)
				tf_printf("0");
			tf_printf("%x", current_chip_id[i]);
		}
		tf_printf("\"\n");
#endif
		return;
	}
	NOTICE("Setting chip to RMA state.\n");
	if (update_chip_state(BF_CHIP_STATE_RMA)) {
			ERROR("Setup of chip RMA state failed\n");
			return;
	}
	NOTICE("Chip RMA mode setup successful\n");
}
#endif

/* Confirm that the user know what are they doing before updating the fuses. */
static void bluefield_warn_user(void)
{
#ifndef SKIP_FUSE_WARNING
	char buf[3];
	int idx = 0;
	char c;

	const char *warn_msg =
	"****************************************************************\n"
	"*** __          __       _____   _   _  _____  _   _   _____ ***\n"
	"*** \\ \\        / //\\    |  __ \\ | \\ | ||_   _|| \\ | | / ____|***\n"
	"***  \\ \\  /\\  / //  \\   | |__) ||  \\| |  | |  |  \\| || |  __ ***\n"
	"***   \\ \\/  \\/ // /\\ \\  |  _  / | . ` |  | |  | . ` || | |_ |***\n"
	"***    \\  /\\  // ____ \\ | | \\ \\ | |\\  | _| |_ | |\\  || |__| |***\n"
	"***     \\/  \\//_/    \\_\\|_|  \\_\\|_| \\_||_____||_| \\_| \\_____|***\n"
	"***                                                          ***\n"
	"****************************************************************\n"
#if defined(INIT_SBKEY)
	"You are about to burn the BlueField fuses to switch on secure boot.\n"
	"This is a NON REVERSIBLE process. After the fuses are burnt, you can\n"
	"ONLY boot SIGNED images and firmware. You CANNOT use the existing\n"
	"unsigned images to boot anymore. The provided install.bfb DOESN'T\n"
	"work anymore. The firmware won't be loaded unless it was signed\n"
	"when burnt. Please confirm that you know what you are doing, the\n"
	"firmware has already been signed, you know the hash of which public\n"
	"key you are burning and you have kept the private counterpart in a\n"
	"safe location and know how to use it to generate signed images.\n"
#elif defined(UPDATE_SBKEY)
	"You are about to update the BlueField secure boot key fuses to a new\n"
	"value. You can maximum update the key fuse values 3 times in total\n"
	"for the entire lifetime of one chip. This is a NON REVERSIBLE\n"
	"process, you CANNOT switch back to using the old key values. Thus\n"
	"you CANNOT use your existing certificates for your image, you have\n"
	"to generate new certificates based on the new key. Please confirm\n"
	"that you know what are doing, you know how to generate new\n"
	"certificates and you know which key you are updating.\n"
#elif defined(SET_RMA)
	"You are about to set the chip into RMA mode. This will permanently\n"
	"disable secure boot on this chip. This is a NON REVERSIBLE process.\n"
	"After the chip is put into RMA mode, there is no known way to force\n"
	"it back to secure state. The previously burnt keys can never be read\n"
	"again. Please confirm that you know what you are doing.\n"
#else
# error File should not be compiled with normal bootflow code!
#endif
	"Please type [yes] and then press enter to continue:";

	tf_printf(warn_msg);

	/*
	 * This is a very simple CLI, fancy things like backspace aren't
	 * supported. If backspace is needed, just repush the bfb file
	 * and start over again. Three letters aren't that hard to type.
	 */
	while (idx < 3) {
		c = console_getc();
		/* Ignore everything that isn't a lower case letter. */
		if (c >= 'a' && c <= 'z') {
			buf[idx++] = c;
			console_putc(c);
		}
	}
	/* Wait for the ENTER at the end. */
	do {
		c = console_getc();
	} while (c != '\r');
	tf_printf("\n");

	if (strncmp(buf, "yes", 3)) {
		tf_printf("Operation aborted, please shutdown or reboot.\n");
		while (1)
			wfi();
	}

#endif /* SKIP_FUSE_WARNING */
}

void bluefield_mod_fuses(void)
{
	RSH_SB_MODE_t rsb;

	do {
		rsb.word = mmio_read_64(RSHIM_BASE + RSH_SB_MODE);
	} while (!rsb.init_done);

	bluefield_warn_user();

#if defined(INIT_SBKEY)
	bluefield_init_sbkey(rsb);
#elif defined(UPDATE_SBKEY)
	bluefield_update_sbkey(rsb);
#elif defined(SET_RMA)
	bluefield_set_rma(rsb);
#else
# error File should not be compiled with normal bootflow code!
#endif

	NOTICE("Chip can now be powered off\n");

	while(1)
		wfi();
}
