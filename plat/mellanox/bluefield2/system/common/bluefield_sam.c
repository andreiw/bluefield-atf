/*
 * Copyright (c) 2016, ARM Limited and Contributors. All rights reserved.
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
 * Neither the name of ARM nor the names of its contributors may be used
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
#include <debug.h>
#include <mmio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "bluefield_private.h"
#include "hnf.h"
#include "l3c.h"
#include "trio.h"
#include "rsh.h"
#include "tyu_def.h"

DEFINE_SYSREG_READ_FUNC(id_isar5_el1)

/* The physical address of the first byte of memory in gigabytes. */
#define PA_START_GB			(MEMORY_BASE >> 30)
/*
 * We are storing the size of the memory in gigabits, so this macro is called
 * when we want to convert it into gigabytes. We are using gigabits as we have
 * to deal with single DRAM chips which have capacity that doesn't align well
 * with gigabytes, and when converting it to gigabytes (which is used as the
 * smallest unit) in order to setup the PMR configuration registers properly, we
 * have to ensure that it doesn't gets rounded to zero so that the corresponding
 * PMR have memory mapped, this (as well as the fact that dealing with the ECC
 * issue where we need 9 bits for a valid byte) inevitably creates holes in the
 * memory space, but is something that have to be dealt with when using single
 * DRAM setups.
 */
#define PMR_ROUND2GB(x)	((x) > 8 ? ((x) / 8) : ((x) != 0))

/*
 * Calculate the bit in the MSS bit mask corresponding to MSS indexed 'mss' with
 * port number 'port'.
 */
#define MSS_PORT_BIT(mss, port)	(1 << (NUM_PORTS_PER_MEM_CTRL * (mss) + (port)))

/* CR space register offsets. */
#define PCIE_SWITCH_0		0xf8b0
#define PCIE_SWITCH_1		0xf8bc
/* PCIe Switch Enable bit field.
 * 0: Trio is in end-point mode
 * 1: Trio is in RC mode
 */
#define PCIE_SWITCH_EN		0x1

typedef union {
	struct {
		uint32_t reserved_1 : 1;
		uint32_t data_valid : 1;
		uint32_t pcore0_switch_enable : 1;
		uint32_t pcore1_switch_enable : 1;
		uint32_t pcore0_link_width : 2;
		uint32_t pcore1_link_width : 2;
		uint32_t reserved_0 : 24;
	};
	uint32_t word;
} tyu_gpio_datain_t;

struct pmr_info {
	/*
	 * Non-zero indicates used, size of DIMM excluding ECC
	 * or size of single DRAM chip attached in gigabits.
	 */
	uint16_t size_in_gb;
	/* Nonzero if this PMR maps to a nonvolatile DIMM device. */
	uint8_t is_nvdimm : 1;
	/*
	 * Indicate the DIMM device this PMR maps to should receive memory
	 * requests which maps to the high rank(s) in the attached MSS instead
	 * of just letting the MSS do striping between the attached DIMMs.
	 * The hardware supports physical rank division when only the two DIMMs
	 * attached are marked as regualr and nonvolatile DIMMs respectively,
	 * but this is not the only case when we want to do physical rank
	 * division, we also need this when given the PMR_DISABLE_DIMM_STRIPING
	 * option or if two NVDIMMs are attached. Luckily as this property is
	 * not used elsewhere in the hardware, we are abusing it just for the
	 * physical rank division function by using this field to set the
	 * DIMM_TYPE property for the PMR setup.
	 */
	uint8_t is_mss_high_rank : 1;
	/* Nonzero if this PMR maps to a single DRAM chip. */
	uint8_t is_single_DRAM : 1;
	/*
	 * Bitmap of which MSS ports are used, the bits are mapped by the
	 * MSS_PORT_BIT macro defined above.
	 */
	uint8_t mss_ports;
	/*
	 * Non-zero indicates that this PMR is sharing a DIMM with another
	 * PMR and is using the top region of the DIMM, the value indicates
	 * which is the starting point of this PMR in the DIMM in gigabytes,
	 * if this is non-zero, this PMR should only map to one MSS.
	 */
	uint16_t offset_in_GB;
	/*
	 * Pointer to the mem_dimm_info struct this PMR is corresponding to.
	 * This field is only relavent if the DIMM is not striped.
	 */
	struct bf_mem_dimm_info *dimm_info;
};

/* Local struct used to keep track of the PMR configuration. */
struct pmr_usage {
	struct pmr_info pmrs[NUM_PMRS];
};

/* Get the log 2 of a given number, number given must be a power of 2. */
static int log2(int num)
{
	return __builtin_ctz(num);
}

/*
 * Read and write shim registers indirectly, using the rshim mem_acc
 * widget.
 */
static void reg_indirect_setup(void)
{
	RSH_MEM_ACC_SETUP_t rmas = {
		.attr = RSH_MEM_ACC_SETUP__ATTR_VAL_DEV_NON_BUF,
	};

	mmio_write_64(RSHIM_BASE + RSH_MEM_ACC_SETUP, rmas.word);
}

static void reg_indirect_wait(uint64_t resp_count)
{
	int retries = 1000;

	while (mmio_read_64(RSHIM_BASE + RSH_MEM_ACC_RSP_CNT) == resp_count) {
		if (--retries < 0) {
			ERROR("RSH_MEM_ACC timeout\n");
			/*
			 * Some devices (like crypto) are not ready on Lx
			 * platform yet. So just continue if we encounter this
			 * for now. Redmine issue: 1682781
			 */
			break;
		}
	}
}

static uint64_t read_reg_indirect(uintptr_t pa, uint8_t size)
{
	uint64_t resp_count = mmio_read_64(RSHIM_BASE + RSH_MEM_ACC_RSP_CNT);

	RSH_MEM_ACC_CTL_t rmac = {
		.address = pa,
		.size = size,
		.send = 1,
	};

	mmio_write_64(RSHIM_BASE + RSH_MEM_ACC_CTL, rmac.word);

	reg_indirect_wait(resp_count);

	uint64_t data = mmio_read_64(RSHIM_BASE + RSH_MEM_ACC_DATA__FIRST_WORD);

	return data;
}

static void write_reg_indirect(uintptr_t pa, uint8_t size, uint64_t data)
{
	uint64_t resp_count = mmio_read_64(RSHIM_BASE + RSH_MEM_ACC_RSP_CNT);

	mmio_write_64(RSHIM_BASE + RSH_MEM_ACC_DATA__FIRST_WORD, data);

	RSH_MEM_ACC_CTL_t rmac = {
		.address = pa,
		.size = size,
		.write = 1,
		.send = 1,
	};

	mmio_write_64(RSHIM_BASE + RSH_MEM_ACC_CTL, rmac.word);

	reg_indirect_wait(resp_count);
}

/*
 * Read the crspace registers using the rshim_mem_acc widget.
 * Accesses are 4-bytes long and the data is byte swapped.
 */
static uint32_t read_crspace(uintptr_t pa)
{
	uint32_t data = (uint32_t)read_reg_indirect(pa,
				RSH_MEM_ACC_CTL__SIZE_VAL_SZ4);

	data = ((data & 0x000000ff) << 24) | ((data & 0x0000ff00) << 8) |
		((data & 0x00ff0000) >> 8) | ((data & 0xff000000) >> 24);

	return data;
}

/*
 * Read and write shim registers. Most devices have physical addresses within
 * the [DEVICES_BASE, DEVICES_BASE + DEVICES_SIZE] range; these are mapped into
 * the MMU and can be accessed via direct load and store. A few devices (e.g.
 * TRIO) do not, so we use the rshim's memory access widget to get them.
 */
static uint64_t read_reg(uintptr_t shim_pa, uint32_t offset)
{
	uintptr_t reg_addr = shim_pa + offset;

	if (reg_addr < DEVICES_BASE + DEVICES_SIZE)
		return mmio_read_64(reg_addr);
	else
		return read_reg_indirect(reg_addr,
			RSH_MEM_ACC_CTL__SIZE_VAL_SZ8);
}

static void write_reg(uintptr_t shim_pa, uint32_t offset, uint64_t data)
{
	uintptr_t reg_addr = shim_pa + offset;

	if (reg_addr < DEVICES_BASE + DEVICES_SIZE)
		mmio_write_64(reg_addr, data);
	else
		write_reg_indirect(reg_addr,
			RSH_MEM_ACC_CTL__SIZE_VAL_SZ8, data);
}

/*
 * Returns 1 if the ARM cores have the AES instruction support, and
 * thus the setup have the crypto blocks working.
 */
static int have_crypto(void)
{
#define ID_ISAR5_AES_OFFSET	4
#define ID_ISAR5_AES_RMASK	0xf
	uint32_t id_isar5 = read_id_isar5_el1();
	uint32_t aes = (id_isar5 >> ID_ISAR5_AES_OFFSET) & ID_ISAR5_AES_RMASK;

	if (aes == 0)
		return 0;
	else if (aes == 2)
		return 1;

	ERROR("AES field not valid for ARM-v8 processer.\n");
	panic();
}

/* Read and write SAM table entries. */

#if 0  /* Not used in normal flow but helpful for debugging. */

static uint8_t read_sam(uintptr_t shim_pa, int is_hnf, int index)
{
	RSH_SAM_CTL_t rsc = {
		.update = 1,
		.read = 1,
		.tbl_sel = is_hnf ? RSH_SAM_CTL__TBL_SEL_VAL_HNF
				  : RSH_SAM_CTL__TBL_SEL_VAL_LDR,
		.idx = index,
	};

	write_reg(shim_pa, RSH_SAM_CTL, rsc.word);

	do {
		rsc.word = read_reg(shim_pa, RSH_SAM_CTL);
	} while (rsc.update != 0);

	return rsc.val;
}
#endif

static void write_sam(uintptr_t shim_pa, int is_hnf, int index, uint8_t data)
{
	RSH_SAM_CTL_t rsc = {
		.update = 1,
		.tbl_sel = is_hnf ? RSH_SAM_CTL__TBL_SEL_VAL_HNF
				  : RSH_SAM_CTL__TBL_SEL_VAL_LDR,
		.idx = index,
		.val = data,
	};

	write_reg(shim_pa, RSH_SAM_CTL, rsc.word);

	do {
		rsc.word = read_reg(shim_pa, RSH_SAM_CTL);
	} while (rsc.update != 0);
}

#pragma weak bf_sys_setup_sam
/* Configure the system address map. */
void bf_sys_setup_sam(struct bf_dev_tbl *dev_tbl, uint32_t disabled_devs)
{
	/* Prepare to read/write registers. */
	reg_indirect_setup();

	/*
	 * First, we set up the LDR part of the SAM.  We iterate over each
	 * target shim, skipping any that are disabled or don't have a SAM.
	 * Then we iterate over each shim, and for each enabled one that is
	 * supposed to go into the LDR, we write that entry into the
	 * target.
	 *
	 * Note that we assume the device table contains the tiles, then
	 * the rshim, then the other devices.  This order works if we're
	 * doing direct tile accesses (since the tiles are first, and we
	 * don't need the SAM to get to them), or if we're using the
	 * rshim's mem_acc widget (since the rshim's SAM gets set up before
	 * we need it to get to the other shims).
	 */
	for (int target = 0; dev_tbl[target].base_addr != ~0ULL; target++) {
		if (((disabled_devs >> target) & 1) || !dev_tbl[target].has_sam)
			continue;
		for (int entry = 0; dev_tbl[entry].base_addr != ~0ULL;
		     entry++) {
			if ((disabled_devs >> entry) & 1)
				continue;
			if (dev_tbl[entry].devtbl_idx >= 0)
				write_sam(dev_tbl[target].base_addr, 0,
					  dev_tbl[entry].devtbl_idx,
					  dev_tbl[entry].nodeid);
		}
	}

	/*
	 * Now we set up the HNF part of the SAM.  Logically what we'd like
	 * to do is to set up a 2039-entry table and then write it to each
	 * shim.  However, that won't fit on the stack.  So instead we do
	 * it in much smaller chunks, each the length of the number of
	 * tiles we have.  We randomly shuffle each chunk to avoid any
	 * stride hitting the same directory all the time.
	 */
	uint8_t chunk[BF_CLUSTER_COUNT];
	int chunk_len = 0;

	for (int i = 0; dev_tbl[i].base_addr != ~0ULL; i++)
		if (!((disabled_devs >> i) & 1) &&
		    dev_tbl[i].dev_type == DEV_TILE)
			chunk[chunk_len++] = dev_tbl[i].nodeid;

	assert(chunk_len <= BF_CLUSTER_COUNT);

	int index = 0;

	uint64_t rand_state = 1;

	int pass_len = chunk_len;

	while (index < RSH_SAM_CTL__IDX_VAL_HNF_SIZE) {
		if (pass_len > RSH_SAM_CTL__IDX_VAL_HNF_SIZE - index)
			pass_len = RSH_SAM_CTL__IDX_VAL_HNF_SIZE - index;

		/* Shuffle the chunk. */
		for (int i = pass_len - 1; i > 0; i--) {
			int j = rand_gen(&rand_state) % (i + 1);
			uint8_t temp = chunk[i];

			chunk[i] = chunk[j];
			chunk[j] = temp;
		}

		/* Write the chunk of entries to all SAMs. */
		for (int target = 0; dev_tbl[target].base_addr != ~0ULL;
		     target++) {
			if (((disabled_devs >> target) & 1) ||
			    !dev_tbl[target].has_sam)
				continue;
			for (int i = 0; i < pass_len; i++)
				write_sam(dev_tbl[target].base_addr, 1,
					  index + i, chunk[i]);
		}

		index += pass_len;
	}
}

#pragma weak bf_sys_setup_interrupts
void bf_sys_setup_interrupts(struct bf_dev_tbl *dev_tbl, uint32_t disabled_devs)
{
	/* Prepare to read/write registers. */
	reg_indirect_setup();

	/*
	 * The only setup we need to do for interrupts is to set the shim's
	 * global enable bit.
	 */
	for (int target = 0; dev_tbl[target].base_addr != ~0ULL; target++) {
		if ((disabled_devs >> target) & 1)
			continue;
		/*
		 * We skip setting the crypto block if it is disabled or we
		 * would hang here.
		 */
		if ((dev_tbl[target].dev_type == DEV_CRYPTO ||
		    (dev_tbl[target].dev_type == DEV_GGA_CRYPT)) && !have_crypto())
			continue;
		write_reg(dev_tbl[target].base_addr, RSH_INT_SETUP,
			  RSH_INT_SETUP__GBL_ENA_MASK);
	}
}

/* Return a pmr_info struct describing the specified DIMM. */
static struct pmr_info fill_pmr_by_dimm(struct bf_mem_info *bmi,
					uint8_t mss_index,
					uint8_t dimm_index,
					uint8_t pmr_options)
{
	struct pmr_info pi;
	struct bf_mem_dimm_info *mdi =
		&(bmi->mem_ctrl_info[mss_index].dimm_info[dimm_index]);

	memset(&pi, 0, sizeof(pi));
	if (mdi->size_in_gb == 0)
		return pi;

	pi.size_in_gb = mdi->size_in_gb;
	pi.is_nvdimm = mdi->is_nvdimm;
	pi.is_mss_high_rank = 0;
	pi.is_single_DRAM = mdi->is_single_DRAM;
	pi.offset_in_GB = 0;
	if (pmr_options & PMR_DISABLE_MSS_FIRST_PORT)
		pi.mss_ports = MSS_PORT_BIT(mss_index, 1);
	else if (pmr_options & PMR_DISABLE_MSS_SECOND_PORT)
		pi.mss_ports = MSS_PORT_BIT(mss_index, 0);
	else
		pi.mss_ports = MSS_PORT_BIT(mss_index, 0) |
			       MSS_PORT_BIT(mss_index, 1);
	pi.dimm_info = mdi;
	return pi;
}

/* Fills out the empty pmr_usage struct given for the specified MSS. */
static void fill_pmr_by_mss(struct pmr_usage *pu, struct bf_mem_info *bmi,
			    uint8_t mss_index, uint8_t pmr_options)
{
#if MAX_DIMM_PER_MEM_CTRL != 2
#error Code assumes MAX_DIMM_PER_MEM_CTRL == 2, must fix
#endif
	memset(pu, 0, sizeof(*pu));
	pu->pmrs[0] = fill_pmr_by_dimm(bmi, mss_index, 0, pmr_options);
	struct pmr_info p2  = fill_pmr_by_dimm(bmi, mss_index, 1, pmr_options);

	/*
	 * We assume that if there is a regular among with a nonvolatile DIMM
	 * inserted then the regular DIMM is inserted in the first slot, this
	 * case is asserted in the memory configuration code.
	 */
	if (p2.size_in_gb == 0)
		return;
	if (pu->pmrs[0].size_in_gb == 0) {
		p2.is_mss_high_rank = 1;
		pu->pmrs[0] = p2;
		return;
	}
	/*
	 * If we reach here, all two DIMMs are populated, we still require that
	 * if there is only 1 NVDIMM out of the 2 DIMMs, it should be in the
	 * second slot.
	 */
	if (p2.is_nvdimm) {
		/* Second DIMM is NVDIMM, first might be NVDIMM. */
		pu->pmrs[0].is_mss_high_rank = 0;
		p2.is_mss_high_rank = 1;
		pu->pmrs[1] = p2;
	} else {
		/* Both DIMMs are regular DIMMs with same properties. */
		if (pmr_options & PMR_DISABLE_DIMM_STRIPING) {
			p2.is_mss_high_rank = 1;
			pu->pmrs[1] = p2;
		} else {
			/* We don't support regular DIMMs of different sizes */
			pu->pmrs[0].size_in_gb *= 2;
			pu->pmrs[0].mss_ports |= p2.mss_ports;
		}
	}
	/*
	 * Notice that we guarantee that PMR for regular DIMM goes before NVDIMM
	 * and if we are separating two DIMMs using the is_mss_high_rank flag
	 * DIMM0 gets marked as .is_mss_high_rank = 0 while DIMM1 gets marked as
	 * 1. This is assumed to be true in later stages of the PMR
	 * configuration code.
	 */
}
/*
 * We transform the mss_ports bitmap to the actual struct to write to register.
 * Note that we are varyng the MSS number the most frequent.
 */
static HNF_HNF_MSS_TABLE_t get_hhms_by_bitmap(uint8_t bitmap)
{
	uint8_t mss_ports[8];
	int index = 0;

	/* Assert bitmap is non-zero and have power of 2 of bits set. */
	assert(bitmap != 0);
	assert(popcount(popcount(bitmap)) == 1);
	while (index < 8)
		for (int p_num = 0; p_num < MAX_DIMM_PER_MEM_CTRL; p_num++)
			for (int m_num = 0; m_num < MAX_MEM_CTRL; m_num++)
				if (bitmap & MSS_PORT_BIT(m_num, p_num))
					mss_ports[index++] = p_num +
						NUM_PORTS_PER_MEM_CTRL * m_num;

	HNF_HNF_MSS_TABLE_t hhmt = {
		.mss_0 = mss_ports[0],
		.mss_1 = mss_ports[1],
		.mss_2 = mss_ports[2],
		.mss_3 = mss_ports[3],
		.mss_4 = mss_ports[4],
		.mss_5 = mss_ports[5],
		.mss_6 = mss_ports[6],
		.mss_7 = mss_ports[7],
	};

	return hhmt;
}

/* Fills out the empty pmr_usage struct given for the system. */
static void fill_pmr(struct pmr_usage *pu, struct bf_mem_info *bmi,
		     uint8_t pmr_options)
{
#if MAX_MEM_CTRL != 2
#error Code assumes MAX_MEM_CTRL == 2, must fix
#endif
	int index = 0;
	int p1_index = 0;
	int p2_index = 0;
	struct pmr_usage p1;
	struct pmr_usage p2;

	memset(pu, 0, sizeof(*pu));
	fill_pmr_by_mss(&p1, bmi, 0, pmr_options);
	fill_pmr_by_mss(&p2, bmi, 1, pmr_options);
	if (!(pmr_options & (PMR_DISABLE_DIMM_STRIPING |
			     PMR_DISABLE_MSS_STRIPING_ONLY)) &&
	    p1.pmrs[0].size_in_gb > 0 && !p1.pmrs[0].is_nvdimm &&
	    p2.pmrs[0].size_in_gb > 0 && !p2.pmrs[0].is_nvdimm &&
	    p1.pmrs[0].is_mss_high_rank == p2.pmrs[0].is_mss_high_rank &&
	    p1.pmrs[0].is_single_DRAM == p2.pmrs[0].is_single_DRAM &&
	    (!p1.pmrs[0].is_single_DRAM ||
	     (p1.pmrs[0].size_in_gb == p2.pmrs[0].size_in_gb))) {
		/* Each controller has a PMR for regular DIMM, do striping. */
		struct pmr_info pi[2];
		int min_index;

		pi[0] = p1.pmrs[p1_index++];
		pi[1] = p2.pmrs[p2_index++];
		min_index = pi[0].size_in_gb > pi[1].size_in_gb;

		/* Create PMR from merging the two PMRs. */
		pu->pmrs[index] = pi[min_index];
		pu->pmrs[index].size_in_gb *= 2;
		pu->pmrs[index].mss_ports |= pi[!min_index].mss_ports;
		index++;
		if (pi[0].size_in_gb != pi[1].size_in_gb) {
			/* PMRs not equally sized, create PMR for remainder. */
			pu->pmrs[index] = pi[!min_index];
			pu->pmrs[index].size_in_gb -= pi[min_index].size_in_gb;
			pu->pmrs[index].offset_in_GB =
						pi[min_index].size_in_gb / 8;
			index++;
		}
	} else {
		/* Not doing striping, forward all PMRs for regular memory. */
		while ((p1.pmrs[p1_index].size_in_gb > 0) &&
		       !p1.pmrs[p1_index].is_nvdimm && p1_index < NUM_PMRS &&
			index < NUM_PMRS)
			pu->pmrs[index++] = p1.pmrs[p1_index++];
		while ((p2.pmrs[p2_index].size_in_gb > 0) &&
		       !p2.pmrs[p2_index].is_nvdimm && p2_index < NUM_PMRS &&
			index < NUM_PMRS)
			pu->pmrs[index++] = p2.pmrs[p2_index++];
	}
	/* Then forward all the PMRs for NVDIMMs. */
	while (p1.pmrs[p1_index].size_in_gb > 0 && p1_index < NUM_PMRS &&
	       index < NUM_PMRS)
		pu->pmrs[index++] = p1.pmrs[p1_index++];
	while (p2.pmrs[p2_index].size_in_gb > 0 && p2_index < NUM_PMRS &&
	       index < NUM_PMRS)
		pu->pmrs[index++] = p2.pmrs[p2_index++];
}

/* Setup the PMR portion of the HNF with the given base address. */
static void setup_pmr_for_hnf(uint64_t base_addr, struct pmr_usage *pu,
			      HNF_HNF_MSS_NODEID_t hhmn)
{
	/* Update the table for mssID to nodeID. */
	HNF_HNF_MSS_NODEID_t old_hhmn = {
		.word = read_reg(base_addr, HNF_HNF_MSS_NODEID)
	};
	HNF_HNF_MSS_NODEID_t new_hhmn = {
		.mss_0 = hhmn.mss_0 ? hhmn.mss_0 : old_hhmn.mss_0,
		.mss_1 = hhmn.mss_1 ? hhmn.mss_1 : old_hhmn.mss_1,
		.mss_2 = hhmn.mss_2 ? hhmn.mss_2 : old_hhmn.mss_2,
		.mss_3 = hhmn.mss_3 ? hhmn.mss_3 : old_hhmn.mss_3,
	};

	write_reg(base_addr, HNF_HNF_MSS_NODEID, new_hhmn.word);

	/* Update the PMR threshold values. */
	uint64_t pa_threshold = PA_START_GB;

	for (int i = 0; i < NUM_PMRS; i++) {
		if (pu->pmrs[i].is_nvdimm)
			pu->pmrs[i].dimm_info->pa_start_GB = pa_threshold;
		pa_threshold += PMR_ROUND2GB(pu->pmrs[i].size_in_gb);

		HNF_HNF_PMR_THRESHOLD_t hhpt = {
			.pa = pa_threshold,
		};

		write_reg(base_addr, HNF_HNF_PMR_THRESHOLD__FIRST_WORD +
			  sizeof(hhpt) * i, hhpt.word);
	}

	/* Update the LMR-to-MSS tables. */
	for (int i = 0; i < NUM_PMRS; i++) {
		if (pu->pmrs[i].mss_ports == 0)
			continue;

		HNF_HNF_MSS_TABLE_t hhmt =
				get_hhms_by_bitmap(pu->pmrs[i].mss_ports);

		write_reg(base_addr, HNF_HNF_MSS_TABLE__FIRST_WORD +
				     sizeof(hhmt) * i, hhmt.word);
	}
	/* Update the HNF_PMR_TYPE register. */
	uint8_t type = 0;

	for (int i = 0; i < NUM_PMRS; i++)
		type |= ((pu->pmrs[i].is_mss_high_rank) << i);

	HNF_HNF_PMR_TYPE_t hhpt = {
		.type = type,
	};

	write_reg(base_addr, HNF_HNF_PMR_TYPE, hhpt.word);
}

/*
 * The given offset for the L3C has to be left shifted by 2 to get the actual
 * register offset, each MSS has 2 L3C portions so we are writing to both here.
 */
static void write_l3c_reg(uintptr_t mss_pa, uint32_t db_offset, uint64_t data)
{
	for (int i = 0; i < NUM_PORTS_PER_MEM_CTRL; i++)
		mmio_write_32(mss_pa + (1 << 22) + (i << 14) + (db_offset << 2),
			      data);
}

/* Set up the PMR portion of the L3C(half) with the given base address. */
static void setup_pmr_for_l3c(uint64_t base_addr, struct pmr_usage *pu,
			      struct bf_mem_ctrl_info *bmci, int mss_index)
{
	/*
	 * We define the value for the cfg register here, if its default value
	 * gets changed it will be be written to the actual register at the end.
	 */
	L3C_EMEM_CFG_t lec = { .word = 0 };

	uint64_t pa_threshold = PA_START_GB;
	int use_rank_div = 0;

	for (int i = 0; i < NUM_PMRS; i++) {

		/* See if this PMR is related to the current L3C. */
		if (pu->pmrs[i].mss_ports & (3 << (2 * mss_index))) {

			/* Configured MSS to support single DRAM mode. */
			if (pu->pmrs[i].is_single_DRAM) {
				/* Set up EMEM related configuration. */
				lec.single_dram_mode = 1;
				lec.single_dram_size =
					log2(pu->pmrs[i].size_in_gb) - 1;
			}

			/* See if rank division is needed. */
			if (!use_rank_div && pu->pmrs[i].is_mss_high_rank) {
				lec.pmr_to_rank_en = 1;
				lec.rank_is_quad =
					(bmci->dimm_info[0].ranks_num +
					 bmci->dimm_info[1].ranks_num > 2) ||
					 bmci->dimm_info[0].size_in_gb == 0;
				lec.rank_map = 0;
				lec.rank_uneven_quad =
					(bmci->dimm_info[0].ranks_num !=
					 bmci->dimm_info[1].ranks_num);
				lec.lrank_zero =
					(bmci->dimm_info[0].ddr_3ds_num !=
					 bmci->dimm_info[1].ddr_3ds_num);

				use_rank_div = 1;
			}
		}
		if (pu->pmrs[i].size_in_gb == 0) {
			L3C_PMR_DFN_t lpd = {
				.threshold = pa_threshold,
			};

			write_l3c_reg(base_addr, L3C_PMR_DFN__FIRST_WORD + i,
				      lpd.word);
			continue;
		}
		uint64_t pa_base = pa_threshold;
		uint8_t mss_bitmap = pu->pmrs[i].mss_ports;

		pa_threshold += PMR_ROUND2GB(pu->pmrs[i].size_in_gb);

		L3C_PMR_DFN_t lpd = {
			.threshold = pa_threshold,
			.dimm_type = pu->pmrs[i].is_mss_high_rank,
			.mss_port_num = log2(popcount(mss_bitmap)),
			/*
			 * In debugging mode we might not have emem_num =
			 * mss_port_num / 2, so we count if at least one port of
			 * an MSS is used to count the emem_num.
			 */
			.emem_num = log2(popcount(
				((mss_bitmap & 0xaa) >> 1) | (mss_bitmap & 0x55)
			)),
		};

		write_l3c_reg(base_addr, L3C_PMR_DFN__FIRST_WORD + i, lpd.word);

		int64_t e_base = pa_base - pu->pmrs[i].offset_in_GB;

		L3C_PMR_DFN_1_t lpd1 = {
			.emi_base_th = (e_base > 0 ? e_base
						   : -e_base) >> lpd.emem_num,
			.emi_base_add = (e_base < 0),
		};
		write_l3c_reg(base_addr, L3C_PMR_DFN_1__FIRST_WORD + i,
			      lpd1.word);
	}

	if (lec.word != 0)
		write_l3c_reg(base_addr, L3C_EMEM_CFG, lec.word);
}

#pragma weak bf_sys_setup_pmr
/* Configure memory mapping. */
void bf_sys_setup_pmr(struct bf_dev_tbl *dev_tbl, uint32_t disabled_devs,
		      struct bf_mem_info *bmi, uint8_t options)
{
	/* Get the node id of the MSS so that we can fill out the table. */
	uint8_t mss_node_id[4] = {0};
	uint64_t mss_base_addr[MAX_MEM_CTRL] = {0};
	int mss_port_num = 0;

	for (int index = 0; dev_tbl[index].base_addr != ~0ULL; index++) {
		if (((disabled_devs >> index) & 1) ||
		    dev_tbl[index].dev_type != DEV_MSS)
			continue;

		for (int i = 0; i < MAX_DIMM_PER_MEM_CTRL; i++)
			if (bmi->mem_ctrl_info[mss_port_num / 2]
			    .dimm_info[i].size_in_gb != 0) {
				mss_base_addr[mss_port_num / 2] =
						dev_tbl[index].base_addr;
				break;
			}

		mss_node_id[mss_port_num++] = dev_tbl[index].nodeid;
		mss_node_id[mss_port_num++] = dev_tbl[index].sec_nodeid;
	}
	HNF_HNF_MSS_NODEID_t hhmn = {
		.mss_0 = mss_node_id[0],
		.mss_1 = mss_node_id[1],
		.mss_2 = mss_node_id[2],
		.mss_3 = mss_node_id[3],
	};
	/* Next get the necessary memory info for the PMR setup. */
	struct pmr_usage pu;

	fill_pmr(&pu, bmi, options);

	/* Now we update each HNF. */
	for (int index = 0; dev_tbl[index].base_addr != ~0ULL; index++) {
		if (((disabled_devs >> index) & 1) ||
		     dev_tbl[index].dev_type != DEV_TILE)
			continue;

		uint64_t base_addr = dev_tbl[index].base_addr;

		setup_pmr_for_hnf(base_addr, &pu, hhmn);
	}

	/* Finally we update each L3C corresponding field in the MSS. */
	for (int mss_index = 0; mss_index < MAX_MEM_CTRL; mss_index++)
		if (mss_base_addr[mss_index])
			setup_pmr_for_l3c(mss_base_addr[mss_index], &pu,
				  &(bmi->mem_ctrl_info[mss_index]), mss_index);
}

#pragma weak bf_sys_setup_hnf_errata
void bf_sys_setup_hnf_errata(struct bf_dev_tbl *dev_tbl, uint32_t disabled_devs)
{
	for (int index = 0; dev_tbl[index].base_addr != ~0ULL; index++) {
		if (((disabled_devs >> index) & 1) ||
		     dev_tbl[index].dev_type != DEV_TILE)
			continue;

		uint64_t base_addr = dev_tbl[index].base_addr;

		/* Work around PCIe TSO issue (RM 1055045). */
		uint64_t cfg = read_reg(base_addr, HNF_HNF_CFG);

		cfg |= HNF_HNF_CFG__TSO_CL_WAIT_MASK;
		write_reg(base_addr, HNF_HNF_CFG, cfg);
	}
}

static void bf_setup_trio_pci(struct bf_dev_tbl *dev_tbl,
			      uint32_t disabled_devs,
			      uint32_t links_width,
			      int8_t trio_idx)
{
	int index;
	int seen_trio = 0;

	for (index = 0; dev_tbl[index].base_addr != ~0ULL; index++)
		if (dev_tbl[index].dev_type == DEV_TP ||
		    dev_tbl[index].dev_type == DEV_HCA) {
			if (seen_trio == trio_idx)
				break;
			seen_trio++;
		}

	if ((dev_tbl[index].base_addr == ~0ULL) ||
		((disabled_devs >> index) & 1))
		return;

	uint64_t base_addr = dev_tbl[index].base_addr;

	/*
	 * We are opening up the whole VA space so we just need one
	 * mapping. The reason we can do this is because access
	 * control is not needed here when the TRIO is in RC mode.
	 * This will not be done when the TRIO is in endpoint mode
	 * in order to use TRIO's "claim all" setting.
	 */
	TRIO_MAP_MEM_SETUP_t tmms = {
		.mac_ena = 0xff,
	};
	write_reg(base_addr, TRIO_MAP_MEM_SETUP__FIRST_WORD, tmms.word);
	TRIO_MAP_MEM_BASE_t tmmb = {
		.addr = 0,
		.slice_size = 0,
		.slice_stride = 0,
	};
	write_reg(base_addr, TRIO_MAP_MEM_BASE__FIRST_WORD, tmmb.word);
	TRIO_MAP_MEM_LIM_t tmml = {
		.addr = 0xfffffffffffff,
	};
	write_reg(base_addr, TRIO_MAP_MEM_LIM__FIRST_WORD, tmml.word);

	/*
	 * Assign read queues to PCIe links according to the width
	 * of each link.
	 */
	TRIO_MAP_MEM_RDQ_ASSIGN_t readq;
	uint fifo = 0;
	readq.word = 0;
	for (uint link = 0; link < 8; link += (links_width / 2)) {
		for (; fifo < (link + links_width / 2); ++fifo)
			readq.word |= (link << (fifo * 3));
	}
	write_reg(base_addr, TRIO_MAP_MEM_RDQ_ASSIGN, readq.word);

}

/*
 * The rshim bridge copy engine is used by the HCA firmware to
 * copy the bus number and memory region windows to the SAM
 * tables of each tile.  It must be enabled before config writes
 * to the switches can be handled.
 */
static void bluefield_setup_bridge_copy(void)
{
	RSH_PCI_BRIDGE_COPY_CTL_t copy_ctl = {
		.copy_ena = 1
	};

	mmio_write_64(RSHIM_BASE + RSH_PCI_BRIDGE_COPY_CTL, copy_ctl.word);
}

#pragma weak bf_sys_setup_trio
/* Configure trio memory mapping. */
void bf_sys_setup_trio(struct bf_dev_tbl *dev_tbl, uint32_t disabled_devs)
{
	/* Find the HCA TRIO */
	for (int index = 0; dev_tbl[index].base_addr != ~0ULL; index++) {
		if (((disabled_devs >> index) & 1) ||
		    (dev_tbl[index].dev_type != DEV_HCA))
			continue;

		/* The HCA trio is always enabled and configured as x16 */
		bf_setup_trio_pci(dev_tbl, disabled_devs, 16, 2);

		tyu_gpio_datain_t gpio = { 0 };

		gpio.word = mmio_read_32(TYU_BASE_ADDRESS +
                                         TYU_GPIO_INTERNAL_DATAIN);

		/*
		 * Old firmware is running, so fall back to the TRIO
		 * CR space window method
		 */
		if (!gpio.data_valid) {
			TRIO_CFG_REGION_ADDR_t cra = {
				.intfc = 0x2,
			};
			cra.reg = PCIE_SWITCH_0;
			gpio.pcore0_switch_enable =
				read_crspace(dev_tbl[index].base_addr +
					     cra.word) & PCIE_SWITCH_EN;
			cra.reg = PCIE_SWITCH_1;
			gpio.pcore1_switch_enable =
				read_crspace(dev_tbl[index].base_addr +
					     cra.word) & PCIE_SWITCH_EN;
		}

		if (gpio.pcore0_switch_enable)
			bf_setup_trio_pci(dev_tbl, disabled_devs,
					  2 << gpio.pcore0_link_width, 0);

		if (gpio.pcore1_switch_enable)
			bf_setup_trio_pci(dev_tbl, disabled_devs,
					  2 << gpio.pcore1_link_width, 1);

		break;
	}

	bluefield_setup_bridge_copy();
}
