/*
 * Copyright (c) 2017, Mellanox Technologies Ltd. All rights reserved.
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

/*
 * This file contains the typical configurations the user would want to change
 * to suit building ATF for BlueField for their particular system. For a
 * complete list of functions which can be overwritten please see the header
 * file plat/mellanox/bluefield/include/bluefield_system.h
 */

#include "bluefield_ddr.h"
#include "bluefield_system.h"

/*
 * This function is called to read the SPD for a given DIMM. The default
 * implementation is at plat/mellanox/bluefield/system/common/bluefield_memory.c
 * If the system have soldered DIMMs and does not require reading the SPD to
 * figure out the type of DIMM present, then uncomment this function here,
 * which will result in no SPD read attempts. In this case please also uncomment
 * the  bf_sys_ddr_get_info_user() function below and specify the fixed memory
 * configuration.
 */
#if 0
int bf_sys_get_spd(uint8_t *spd, int offset, int len, int mss, int dimm)
{
	return 0;
}
#endif

/*
 * This function should fill in all the per-board specific memory parameters.
 * ddr_params is defined in plat/mellanox/bluefield/include/bluefield_def.h
 * though you should only be modifying the parameters as shown in the example
 * below. This function is called once per memory controller after the system
 * has read the SPD of the DIMM(s) attached and populated ddr_params *dp with
 * the corresponding information and before doing any calculations and
 * memory setup. For cases where there are no SPDs attached (e.g. direct
 * attached DRAM configuration), the DIMM configurations are not set.
 * In that case, this function shouldn't rely on on it to determine the analog
 * parameters.
 * This function should return 1 on success or 0 if failed, in which case the
 * configuration for the current memory controller would be skipped.
 */
int bf_sys_ddr_get_info_board(struct ddr_params *dp)
{
	return bf_sys_ddr_get_info_board_default(dp);
}

/* Uncomment this section to modify the memory parameters read from SPD. */
#if 0

/*
 * Enter any changes to be made to the memory parameters read from the SPD.
 * ddr_params is defined in plat/mellanox/bluefield/include/bluefield_def.h
 * though you should only be modifying the parameters as shown in the example
 * below. This function is called once per memory controller after the system
 * has read the SPD of the DIMM(s) attached and populated ddr_params *dp with
 * the corresponding information and before doing any calculations (e.g.
 * figuring out the cycle values for the various timings based on tck) and
 * memory setup. For cases where there are no SPDs attached (e.g. direct
 * attached DRAM configuration), all the timing parameters are not set (have
 * a value of 0). In that case, this function is used to tell the system
 * everything needed to set up the DRAM and not just overwriting the default
 * value.
 * This function should return 1 on success or 0 if failed, in which case the
 * configuration for the current memory controller would be skipped.
 */
int bf_sys_ddr_get_info_user(struct ddr_params *dp)
{
	/*
	 * When this function is called *dp is alreadly populated. The mss_index
	 * indicates which memory controller is being set up, this can be
	 * used to only change memory configurations on a specific memory
	 * controller as follows:
	 */
	/*
	 * if (dp->mss_index != 0) {
	 *	return 1;
	 * }
	 */
	/*
	 * The tck value is the actual cycle time used. It is specified in
	 * femtoseconds. This parameter is used to calculate all the cycle
	 * delay parameters from the timing parameters. Note that there are
	 * also tck_max and tck_min parameters which might be populated (in
	 * the case where there is an SPD to be read from), in this case the
	 * tck value should fall between the min and max values.
	 */
	/* dp->tck = 833333; */

	/*
	 * Maximum allowed DDR frequency in kilohertz. This is already inforced
	 * before calling this function, so if the default value of 1200000kHz
	 * is changed, the value of tck should be reinforced here to meet the
	 * given maximum frequency.
	 */
	/* dp->ddr_max_freq_khz = 1200000; */
	/* dp->tck = MAX(dp->tck, FS_PER_MS / dp->ddr_max_freq_khz); */

	/*
	 * Bitmask of which ranks are mirrored. In a dual rank setup valid
	 * values are 0 or 2. In a quad rank setup, valid values are 0 or 6.
	 */
	/* dp->system_address_mirror = 2; */

	/*
	 * This parameter specifies the type of memory attached to the
	 * controller. Valid options are ONBOARD (directly mounted SDRAM
	 * device), UDIMM, RDIMM and LRDIMM.
	 */
	/* dp->type = ONBOARD; */
	/*
	 * This indicates how many DIMMs are attached to the current memory
	 * controller. This number shouldn't exceed the maximum number of DIMMs
	 * supported on a single memory controller. In the case of directly
	 * mounted SDRAM device (dp->type == ONBOARD) please set this value to
	 * 1, else this value shouldn't be touched.
	 */
	/* dp->dimm_num = 1; */

	/*
	 * These are the per DIMM parameters which can be different between
	 * DIMMs. If only a single DIMM is used it should occupy index 0. If
	 * multiple DIMMs are used, this should be set for all the DIMMs in
	 * which a change from the default parameter is desired. The index to
	 * the dimm array should be less or equal to dp->dimm_num. For direct
	 * attahced DRAMs, all the parameters should be set as there is no SPD
	 * to get the default parameters from.
	 */
	/*
	 * Number of ranks the DIMM has, should only be 1, 2, or 4. In case of
	 * 4 it should be the only DIMM attached.
	 */
	/* dp->dimm[0].ranks = 2; */
	/* Set this to 1 if the DIMM attached is an NVDIMM device. */
	/* dp->dimm[0].is_nvdimm = 0; */
	/*
	 * Set this to 2 or 4 if the DIMM is a 3DS device with 2 or 4 logical
	 * ranks, or 1 if the DIMM is just a monolithic device.
	 */
	/* dp->dimm[0].ddr_3ds = 1; */
	/*
	 * Set this to the actual capacity of the DIMM module in gigabits, for
	 * example a 2GB module would have a value of 2GB x 8bits/byte = 16.
	 */
	/* dp->dimm[0].capacity_gbit = 16; */
	/*
	 * The density of the SDRAM chip, supported values are:
	 * DENSITY_2Gbit, DENSITY_4Gbit, DENSITY_8Gbit, DENSITY_16Gbit.
	 */
	/* dp->dimm[0].density = DENSITY_4Gbit; */

	/*
	 * Raw drive strength value for RDIMM taken from the SPD.
	 * In case we are hard coding the speed bin without reading the SPD for
	 * setting up RDIMMs, we should calculate the artificial values for
	 * SPD byte 137 and 138, which describes the Register Output Drive
	 * Strength for Control and Command/Address (byte 137) and Clock (byte
	 * 138). For more information, please refer to the DDR4 SPD spec
	 * (JEDEC Standard No. 21-C Page 4.1.2.L-4).
	 * For LRDIMM we are using the bytes 128~191 of the Module Specific
	 * Bytes, so it would be impractical to manually enter the SPD
	 * information and not just read the SPD.
	 */
	/* dp->dimm[0].spd[137] = 0xaa; */
	/* dp->dimm[0].spd[138] = 0x0a; */

	/*
	 * The speed bin class the DIMM falls into, if this value is set, the
	 * system will override all the timing parameters that are read from the
	 * SPD. If only changing part of the timing parameters, this parameter
	 * shouldn't be modified (don't set this parameter in this function) and
	 * instead set any of the timing parameters in femtoseconds here:
	 * trcd, trp, tras, trc, tccd_l, trrd_s, trrd_l, tfaw, trfc, trfc1,
	 * trfc2, trfc4, twtr_fs, twtr_s_fs, twtr_l_fs, twr and taa.
	 * Currently supported speed bins are:
	 * 1600J/K/L, 1866L/M/N, 2133N/P/R, 2400P/R/T/U, 2666T/U/V/W
	 */
	/* dp->speed_bin = DDR4_2666U; */

	/*
	 * The package of the SDRAM component, supported values are:
	 * PACKAGE_x4, PACKAGE_x8, PACKAGE_x16.
	 */
	/* dp->package = PACKAGE_x8; */
	/* This should be the max of all the dp->dimm[x].ddr_3ds values. */
	/* dp->ddr_3ds = 1; */

	/*
	 * The following are options to be set to enable certain options for the
	 * memory controller.
	 */
	/* Set this to enable DDR SDRAM Write DBI operation. */
	/* dp->wr_dbi_en=1; */
	/* Set this to enable DDR SDRAM Read DBI operation. */
	/* dp->rd_dbi_en=1; */
	/* Set this to enable DDR SDRAM Write DBI operation. */
	/* dp->parity_en=1; */
	/* Set this to enable DDR SDRAM Write Data CRC operation. */
	/* dp->crc_en=1; */

	/* Set this to enable DDR PHY VREF Training Bypass. */
	/* dp->vref_train_bypass = 0; */
	/*
	 * Set this to do DDR PHY VREF Training using Per DRAM Addressability
	 * Model.
	 */
	/* dp->vref_train_pdaen = 1 */
	/*
	 * Set this to do DDR PHY VREF HOST Training results of Min/Max values
	 * v.s. Setup.
	 */
	/* dp->vref_train_hres2 = 0 */
	/*
	 * Set this to do DDR PHY VREF DRAM Training results using both Ranges.
	 */
	/* dp->vref_train_dqres2 = 0 */

	/*
	 * The frequency (in Hz) of the device reference clock that drives the
	 * DDR PLL.
	 */
	/* dp->clk_ref = 50 * 1000 * 1000; */

	/*
	 * If we changed any of the parameters which would change the board
	 * parameters (DIMM type, DIMM num, rank num) or if we are hard coding
	 * the DDR parameters without an SPD to read, we should also call
	 * ddr_get_info_board() to get the relavent board parameters set up.
	 */
	/*
	 * if (!ddr_get_info_board(dp))
	 *	return 0;
	 */
	/*
	 * Return 1 at end if everything is successful. Return 0 if something
	 * failed and the memory controller should no longer be configured (e.g.
	 * reading from a user defined EEPROM for the parameters failed). Note
	 * that returning 0 would also generate an error message, so if the
	 * memory controller should be skipped with no error printed, then
	 * dp->dimm_num = 0 should be used instead.
	 */
	return 1;
}

#endif
