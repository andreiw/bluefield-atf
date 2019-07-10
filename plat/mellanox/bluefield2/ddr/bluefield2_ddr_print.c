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

#include <debug.h>
#include <mmio.h>
#include <string.h>
#include "bluefield_ddr.h"
#include "bluefield_private.h"
#include "i2c_smbus.h"

#define ENUM_START(x)	const char * x ##_fields[] = {
#define ENUM_ENTRY(x)	[ x ] = #x,
#define ENUM_END	};

#define STRUCT_START(x)	void print_ ##x (struct x *data) \
			{ tf_printf("(struct " #x ") {\n");
#define STRUCT_string(x)	tf_printf("\t." #x " = %s,\n", \
					  data->x ? data->x : "<NULL>");
#define STRUCT_uint8_t(x)	tf_printf("\t." #x " = %u (0x%x),\n", \
					  data->x, data->x);
#define STRUCT_uint16_t(x)	tf_printf("\t." #x " = %u (0x%x),\n", \
					  data->x, data->x);
#define STRUCT_uint32_t(x)	tf_printf("\t." #x " = %u (0x%x),\n", \
					  data->x, data->x);
#define STRUCT_uint64_t(x)	tf_printf("\t." #x " = %llu (0x%llx),\n", \
					  data->x, data->x);
#define STRUCT_uintptr_t(x)	tf_printf("\t." #x " = %lu (0x%lx),\n", \
					  data->x, data->x);
#define STRUCT_int8_t(x)	tf_printf("\t." #x " = %d (0x%x),\n", \
					  data->x, data->x);
#define STRUCT_int16_t(x)	tf_printf("\t." #x " = %d (0x%x),\n", \
					  data->x, data->x);
#define STRUCT_int32_t(x)	tf_printf("\t." #x " = %d (0x%x),\n", \
					  data->x, data->x);
#define STRUCT_int64_t(x)	tf_printf("\t." #x " = %lld (0x%llx),\n", \
					  data->x, data->x);
#define STRUCT_int(x)	tf_printf("\t." #x " = %d (0x%x),\n", data->x, data->x);
#define STRUCT_enum(type, x)	tf_printf("\t." #x " = %s\n", \
					  type ##_fields[data->x]);
#define STRUCT_struct(type, x)	tf_printf("\t." #x " = "); \
				print_##type(&data->x);
#define STRUCT_END	tf_printf("}\n"); }

#define SPD(name, val)		tf_printf("\t" name " =\t%d.%d%d%d ns\n", \
				val / 1000, (val % 1000) / 100, \
				(val % 100) / 10, val % 10);

#define SPD_NS(x)		SPD(#x, ((uint32_t)(data->x / 1000)))

#define SPD_ENUM(type, name, x)	tf_printf("\t" #name "=%s\n", \
				type ##_fields[x]);


ENUM_START(dimm_type)
ENUM_ENTRY(ONBOARD)
ENUM_ENTRY(UDIMM)
ENUM_ENTRY(RDIMM)
ENUM_ENTRY(LRDIMM)
ENUM_END

ENUM_START(ddr_freq)
ENUM_ENTRY(DDR4_1600)
ENUM_ENTRY(DDR4_1866)
ENUM_ENTRY(DDR4_2133)
ENUM_ENTRY(DDR4_2400)
ENUM_ENTRY(DDR4_2666)
ENUM_ENTRY(NUM_OF_FREQ)
ENUM_END

ENUM_START(ddr_speed_bin)
ENUM_ENTRY(DDR4_1600J)
ENUM_ENTRY(DDR4_1600K)
ENUM_ENTRY(DDR4_1600L)
ENUM_ENTRY(DDR4_1866L)
ENUM_ENTRY(DDR4_1866M)
ENUM_ENTRY(DDR4_1866N)
ENUM_ENTRY(DDR4_2133N)
ENUM_ENTRY(DDR4_2133P)
ENUM_ENTRY(DDR4_2133R)
ENUM_ENTRY(DDR4_2400P)
ENUM_ENTRY(DDR4_2400R)
ENUM_ENTRY(DDR4_2400T)
ENUM_ENTRY(DDR4_2400U)
ENUM_ENTRY(DDR4_2666T)
ENUM_ENTRY(DDR4_2666U)
ENUM_ENTRY(DDR4_2666V)
ENUM_ENTRY(DDR4_2666W)
ENUM_ENTRY(DDR4_1600K_E)
ENUM_ENTRY(DDR4_1866M_E)
ENUM_ENTRY(DDR4_2133P_E)
ENUM_ENTRY(DDR4_2400T_E)
ENUM_ENTRY(DDR4_2666V_E)
ENUM_ENTRY(NUM_OF_SPEED_BIN)
ENUM_ENTRY(GET_VAL_FROM_SPD)
ENUM_END

ENUM_START(ddr_density)
ENUM_ENTRY(DENSITY_2Gbit)
ENUM_ENTRY(DENSITY_4Gbit)
ENUM_ENTRY(DENSITY_8Gbit)
ENUM_ENTRY(DENSITY_16Gbit)
ENUM_ENTRY(NUM_DENSITY)
ENUM_END

ENUM_START(ddr_package)
ENUM_ENTRY(PACKAGE_x4)
ENUM_ENTRY(PACKAGE_x8)
ENUM_ENTRY(PACKAGE_x16)
ENUM_ENTRY(NUM_PACKAGE)
ENUM_END

ENUM_START(ddr_package_type)
ENUM_ENTRY(PKG_TYPE_SDP)
ENUM_ENTRY(PKG_TYPE_DDP)
ENUM_ENTRY(PKG_TYPE_QDP)
ENUM_ENTRY(PKG_TYPE_3DS)
ENUM_ENTRY(NUM_PKG_TYPE)
ENUM_END

ENUM_START(ddr_rzq)
ENUM_ENTRY(DISABLED)
ENUM_ENTRY(RZQ_DIV_1)
ENUM_ENTRY(RZQ_DIV_2)
ENUM_ENTRY(RZQ_DIV_3)
ENUM_ENTRY(RZQ_DIV_4)
ENUM_ENTRY(RZQ_DIV_5)
ENUM_ENTRY(RZQ_DIV_6)
ENUM_ENTRY(RZQ_DIV_7)
ENUM_ENTRY(RZQ_DIV_8)
ENUM_ENTRY(HIGH_Z)
ENUM_END


STRUCT_START(dimm_params)
STRUCT_uint32_t(ranks)
STRUCT_uint8_t(is_nvdimm)
STRUCT_uint8_t(ddr_3ds)
STRUCT_uint16_t(capacity_gbit)
STRUCT_enum(ddr_density, density)
STRUCT_END

STRUCT_START(ddr_params)
STRUCT_uintptr_t(mss_addr)
STRUCT_int(mss_index)
STRUCT_enum(dimm_type, type)
STRUCT_uint32_t(dimm_num)
STRUCT_struct(dimm_params, dimm[0])
STRUCT_struct(dimm_params, dimm[1])
STRUCT_enum(ddr_speed_bin, speed_bin)
STRUCT_enum(ddr_package, package)
STRUCT_enum(ddr_package_type, pkg_type)
STRUCT_uint8_t(srx16ddp)
STRUCT_uint8_t(ddr_3ds)
STRUCT_uint64_t(tck)
STRUCT_uint64_t(tck_max)
STRUCT_uint64_t(tck_min)
STRUCT_uint64_t(ddr_max_freq_khz)
STRUCT_uint32_t(system_address_mirror)
STRUCT_uint8_t(wr_dbi_en)
STRUCT_uint8_t(rd_dbi_en)
STRUCT_uint8_t(parity_en)
STRUCT_uint8_t(crc_en)
STRUCT_enum(ddr_rzq, mem_rtt_nom)
STRUCT_enum(ddr_rzq, mem_rtt_wr)
STRUCT_enum(ddr_rzq, mem_rtt_park)
STRUCT_enum(ddr_rzq, mem_odic)
STRUCT_uint8_t(asr_en)
STRUCT_uint32_t(mem_vref)
STRUCT_enum(ddr_rzq, phy_wr_drv)
STRUCT_enum(ddr_rzq, phy_rd_odt)
STRUCT_uint32_t(phy_rd_vref)
STRUCT_uint64_t(phy_rtd)
STRUCT_string(sys_name)
STRUCT_uint32_t(ddr_tcase)
STRUCT_uint64_t(ddr_clk[0])
STRUCT_uint64_t(ddr_clk[1])
STRUCT_uint64_t(ddr_addr_ctrl)
//STRUCT_uint32_t(*sysrtd)
STRUCT_uint8_t(wlrdqsg_lcdl_norm)
STRUCT_uint8_t(vref_train_bypass)
STRUCT_uint8_t(vref_train_host_en)
STRUCT_uint8_t(vref_train_mem_en)
STRUCT_uint8_t(vref_train_pdaen)
STRUCT_uint8_t(vref_train_hres2)
STRUCT_uint8_t(vref_train_dqres2)
STRUCT_uint32_t(ddr_clk_ref)
STRUCT_uint64_t(trcd)
STRUCT_uint64_t(trp)
STRUCT_uint64_t(tras)
STRUCT_uint64_t(trc)
STRUCT_uint64_t(tccd_l)
STRUCT_uint64_t(trrd_s)
STRUCT_uint64_t(trrd_l)
STRUCT_uint64_t(tfaw)
STRUCT_uint64_t(trfc)
STRUCT_uint64_t(trfc1)
STRUCT_uint64_t(trfc2)
STRUCT_uint64_t(trfc4)
STRUCT_uint64_t(twtr_fs)
STRUCT_uint64_t(twtr_s_fs)
STRUCT_uint64_t(twtr_l_fs)
STRUCT_uint64_t(twr)
STRUCT_uint64_t(taa)
STRUCT_uint64_t(cl_map)
STRUCT_uint8_t(cl)
STRUCT_enum(ddr_freq, freq)
STRUCT_uint64_t(twtr)
STRUCT_uint64_t(twtr_s)
STRUCT_uint64_t(twtr_l)
STRUCT_uint64_t(pl)
STRUCT_uint64_t(tccd_l_slr)
STRUCT_uint64_t(tccd_dlr)
STRUCT_uint64_t(trrd_s_slr)
STRUCT_uint64_t(trrd_l_slr)
STRUCT_uint64_t(tfaw_slr)
STRUCT_uint64_t(tcke)
STRUCT_uint64_t(txp)
STRUCT_uint64_t(twlo)
STRUCT_uint64_t(tdqsck)
STRUCT_uint64_t(tdllk)
STRUCT_uint64_t(trfc_dlr1)
STRUCT_uint64_t(trfc_dlr2)
STRUCT_uint64_t(trfc_dlr4)
STRUCT_uint64_t(trefi)
STRUCT_uint64_t(tccd_s)
STRUCT_uint64_t(tccd_s_slr)
STRUCT_uint64_t(trrd_dlr)
STRUCT_uint64_t(tfaw_dlr)
STRUCT_uint64_t(trtp)
STRUCT_uint64_t(tmrd)
STRUCT_uint64_t(tmod)
STRUCT_uint64_t(twlmrd)
STRUCT_uint64_t(txs)
STRUCT_uint64_t(trtodt)
STRUCT_uint64_t(trtw)
STRUCT_uint64_t(tzqcs)
STRUCT_uint8_t(cwl)
STRUCT_uint8_t(al)
STRUCT_uint32_t(ddr_max_bank)
STRUCT_uint32_t(ddr_max_col)
STRUCT_uint32_t(ddr_max_row)
STRUCT_uint8_t(rcd_cs_mode)
STRUCT_uint8_t(rcd_dimm_type)
STRUCT_uint8_t(rcd_addr_lat)
STRUCT_uint8_t(rdimm_ca_latency)
STRUCT_uint8_t(rdimm_rd_wr_gap)
STRUCT_END

/* This function only prints the values got from the SPD. */
void print_spd(struct ddr_params *data)
{
	tf_printf("SPD data interpreted as follows:\n");
	SPD_ENUM(dimm_type, type, data->type)
	SPD_ENUM(ddr_package, package, data->package)
	tf_printf("\t%s NVDIMM\n", data->dimm[0].is_nvdimm ? "is" : "not");
	SPD_ENUM(ddr_density, density, data->dimm[0].density)
	tf_printf("\tranks = %d\n", data->dimm[0].ranks);
	if (data->dimm[0].capacity_gbit < 8)
		tf_printf("\tcapacity = %d MB\n",
			  data->dimm[0].capacity_gbit * 128);
	else
		tf_printf("\tcapacity = %d GB\n",
			  data->dimm[0].capacity_gbit / 8);
	tf_printf("\t3ds = %d%s\n", data->dimm[0].ddr_3ds,
			data->dimm[0].ddr_3ds > 1 ? "H" : " (not 3DS DRAM)");
	SPD_NS(tck);
	SPD_NS(trcd);
	SPD_NS(trp);
	SPD_NS(tras);
	SPD_NS(trc);
	SPD_NS(tccd_l);
	SPD_NS(trrd_s);
	SPD_NS(trrd_l);
	SPD_NS(tfaw);
	SPD_NS(trfc);
	SPD_NS(trfc1);
	SPD_NS(trfc2);
	SPD_NS(trfc4);
	SPD_NS(twtr_fs);
	SPD_NS(twtr_s_fs);
	SPD_NS(twtr_l_fs);
	SPD_NS(twr);
	SPD_NS(taa);
	/* Calculate and print out all the supported CL values. */
	int cl_num = 0;
	tf_printf("\tcl =\t");
	for (int i = 0; i < 53; i++)
		if (data->cl_map & (1ull << i)) {
			if (cl_num != 0 && cl_num % 8 == 0)
				tf_printf("\n\t\t");
			tf_printf(" %d,", i);
			cl_num++;
		}
	tf_printf("\n");
}

void print_spd_val(int spd_len, const uint8_t *spd_data)
{
	for (int i = 0; i < spd_len; i++) {
		if (i % 16 == 0)
			tf_printf("\n%d%d%d:", i / 100,
				  (i % 100) / 10, i % 10);
		tf_printf(" %x%x", spd_data[i] / 16,
				   spd_data[i] % 16);
	}
	tf_printf("\n");
}
