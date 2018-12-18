/*
 * Copyright (c) 2014-2015, ARM Limited and Contributors. All rights reserved.
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

#ifndef __BLUEFIELD_PRIVATE_H__
#define __BLUEFIELD_PRIVATE_H__

#include <bakery_lock.h>
#include <cassert.h>
#include <cpu_data.h>
#include <generic_delay_timer.h>
#include <mmio.h>
#include <stdint.h>
#include <utils.h>
#include <xlat_tables.h>

void bluefield_setup_page_tables(uintptr_t total_base,
				 size_t total_size,
				 uintptr_t code_start,
				 uintptr_t code_limit,
				 uintptr_t rodata_start,
				 uintptr_t rodata_limit,
				 uintptr_t coh_start,
				 uintptr_t coh_limit
);

uint32_t bluefield_get_spsr_for_bl33_entry(void);

unsigned int bluefield_get_cluster_core_count(u_register_t mpidr);
void bluefield_gic_driver_init(void);
void bluefield_gic_init(void);
void bluefield_gic_cpuif_enable(void);
void bluefield_gic_cpuif_disable(void);
void bluefield_gic_pcpu_init(void);
void bluefield_gic_redistif_on(void);
void bluefield_gic_redistif_off(void);

void bluefield_io_setup(void);
int bluefield_get_alt_image_source(
	unsigned int image_id,
	uintptr_t *dev_handle,
	uintptr_t *image_spec);
unsigned int bluefield_calc_core_pos(u_register_t mpidr);
unsigned int bluefield_get_baudrate(void);
void bluefield_console_init(void);

const mmap_region_t *bluefield_get_mmap(void);

void bluefield_handle_alternate_boot(void);

/* Enums and data structures */

/* The type of shims/blocks that can be accessed by the ARM. */
enum shim_dev_type {
	DEV_TILE,
	DEV_RSH,
	DEV_SMMU,
	DEV_GIC,
	DEV_MSS,
	DEV_CRYPTO,
	DEV_TP,
	DEV_HCA,
};


/* Device table.  This describes the I/O shims in the present chip. */
struct bf_dev_tbl {
	/* Node ID of the device. */
	uint8_t nodeid;
	/* Index of the device in the LDR.  Negative if the device should
	 * not have an entry there.
	 */
	int8_t devtbl_idx;
	/* Secondary node ID of the device; valid only for memory
	 * controllers for now.
	 */
	uint8_t sec_nodeid;
	/* Nonzero if this device has a SAM to be programmed. */
	uint8_t has_sam : 1;
	/* Nonzero if this device is a tile (and thus has an HNF). */
	enum shim_dev_type dev_type : 7;
	/* Base physical address for this device. */
	uint64_t base_addr;
};

/* Memory info per DIMM. This describes the properties of the DIMM attached. */
struct bf_mem_dimm_info {
	/*
	 *  Size of the DIMM attached (or single DRAM chip) in gigabits not
	 *  counting any ECC bits. If not inserted should be 0.
	 */
	uint16_t size_in_gb;
	/* Nonzero if this DIMM is a nonvolatile DIMM device. */
	uint8_t is_nvdimm : 1;
	/* Nonzero if this slot is inserted with a single DRAM chip. */
	uint8_t is_single_DRAM : 1;
	/* Number of ranks this DIMM uses, valid numbers are 1, 2, 4. */
	uint8_t ranks_num : 3;
	/* Die count, is 1 for monolithic device, 2 or 4 for 3DS device. */
	uint8_t ddr_3ds_num : 3;
	/*
	 * The physical address in gigabytes this DIMM starts at,
	 * only valid if is NVDIMM.
	 */
	uint8_t pa_start_GB;
};

/*
 * Memory info per memory controller. This describes the memory installed on
 * each memory controller.
 */
struct bf_mem_ctrl_info {
	/* Storing per DIMM information attached to this memory controller. */
	struct bf_mem_dimm_info dimm_info[MAX_DIMM_PER_MEM_CTRL];
};

/* Memory info.  This describes the memory installed in the system. */
struct bf_mem_info {
	/* Storing per memory controller information. */
	struct bf_mem_ctrl_info mem_ctrl_info[MAX_MEM_CTRL];
};

/* Memory configuration command opcode definitions */
#define READ_CMD	6
#define WRITE_CMD	4
#define LOOP_CMD	3
#define ENOP_CMD	2
#define XNOP_CMD	1

/* Get the real wait time from power of 2 transformation */
#define GET_REAL_WAIT(x)	((x) ? ((1 << ((x) - 1))) : 0)

/* Write command struct for memory setup */
struct mem_config_write_cmd {
	uint32_t cmd_type : 3;	/* Place for the write opcode */
	uint32_t wait_cycle : 5;/* log2(cycles to wait before execute) */
	uint32_t block_id : 12;	/* Block id to write to */
	uint32_t reg_addr : 12;	/* Register address to write to */
	uint32_t reg_data;	/* The data to write */
};

/* Read command struct for memory setup, required to be followed by loop cmd */
struct mem_config_read_cmd {
	uint32_t cmd_type : 3;	/* Place for the read opcode */
	uint32_t wait_cycle : 5;/* log2(cycles to wait before execute) */
	uint32_t block_id : 12;	/* Block id to read from */
	uint32_t reg_addr : 12;	/* Register address to read from */
	uint32_t loop_reg_msk;	/* Register mask for following loop command */
};

/* Loop command struct for memory setup, should be following a read command */
struct mem_config_loop_cmd {
	uint32_t cmd_type : 3;	/* Place for the loop opcode */
	uint32_t max_cnt : 29;	/* Max loop cycles if expected value mismatch */
	uint32_t exp_val;	/* Expected value (masked) from previous read */
};

/* E-Nop command struct for memory setup */
struct mem_config_enop_cmd {
	uint32_t cmd_type : 3;	/* Place for the E-Nop opcode */
	uint32_t wait_cycle : 29;/* log2(cycles to delay)  */
	uint32_t reg_data;	/* Register data field in original txt file */
};

/* X-Nop command struct for memory setup, wait cycle should be greater than 0 */
struct mem_config_xnop_cmd {
	uint32_t cmd_type : 3;	/* Place for the X-Nop opcode */
	uint32_t block_id : 12;	/* Block id field in original txt file */
	uint32_t wait_cycle;	/* log2(cycles to delay) */
};

/* General command struct for reading cmd_type for memory setup */
struct mem_config_general_cmd {
	uint32_t cmd_type : 3;	/* Place to read the opcode */
	uint64_t payload : 61;	/* Other bit fields used in the command */
};

/*
 * Union to hold the memory setup configuation commands.
 * The structs it included are laid out so that the cmd_type field all map
 * to the same bits in this union, and therefore by reading general.cmd_type
 * which command the current command maps to can be known
 * A raw value of 0 means it's the last command in the sequence.
 */
union mem_config_cmd {
	struct mem_config_write_cmd write;	/* Interpret cmd as a write */
	struct mem_config_read_cmd read;	/* Interpret cmd as a read */
	struct mem_config_loop_cmd loop;	/* Interpret cmd as a loop */
	struct mem_config_enop_cmd enop;	/* Interpret cmd as a E-Nop */
	struct mem_config_xnop_cmd xnop;	/* Interpret cmd as an X-Nop */
	struct mem_config_general_cmd general;	/* Used to read cmd_type */
	uint64_t raw_val;		/* Raw bit values of the command */
};

/* PMR setup options to be passed to bluefield_setup_pmr(). */
/* Pass this option to disable MSS port 0, that is to only use port 1. */
#define PMR_DISABLE_MSS_FIRST_PORT	(1 << 0)
/*
 * Pass this option to disable MSS port 1, that is to only use port 0,
 * this option is ignored if the option PMR_DISABLE_MSS_FIRST_PORT is also used.
 */
#define PMR_DISABLE_MSS_SECOND_PORT	(1 << 1)
/*
 * Pass this option to disable all striping between DIMMs, including striping
 * between DIMMs on the same MSS and also striping between MSS.
 */
#define PMR_DISABLE_DIMM_STRIPING	(1 << 2)
/*
 * Pass this option to only disable striping between MSS, that is if
 * PMR_DISABLE_DIMM_STRIPING is not used, striping is only done between DIMMs
 * on the same MSS.
 */
#define PMR_DISABLE_MSS_STRIPING_ONLY	(1 << 3)

/* Function and variable prototypes */

extern uint8_t boot_on_spi_flash_recovery;

struct ddr_params *bluefield_setup_mss(uintptr_t mss_addr, int mem_ctrl_num);

/* Return a random number each time this function is called. */
uint64_t rand_gen(uint64_t *rand_state);

int popcount(unsigned int num);

uint64_t __random(uint64_t *randseed);

#ifdef IMAGE_BL2

/* Configure memory. */
void bluefield_setup_memory(struct bf_dev_tbl *dev_tbl, uint32_t disabled_devs,
			    struct bf_mem_info *bmi);

/* Set up EFI information. */
void bluefield_setup_efi_info(uintptr_t addr);

/* Set up NVDIMM restore. */
void bluefield_setup_nvdimm_restore(void);

#ifdef ATF_CONSOLE
int ctrlc(void);
int get_arg_int(char *const arg, int *data, int is_ranged, int min, int max,
		char *const name);
int get_arg_str(char *const arg, int *data, char *const name, int count, ...);
#else
 #define ctrlc() 0
#endif
#elif defined(IMAGE_BL31)

/* Get EFI information. */
void bluefield_init_efi_info(uintptr_t addr);

/* Start NVDIMM Save operation. */
void bluefield_setup_nvdimm_save(void);

/* Check NVDIMM related serror. */
void bluefield_nvdimm_serror_check(void);

/* Initialize the interrupt handler. */
typedef uint64_t (*bluefield_irq_handler)(int irq, void *arg);
int bluefield_irq_register(int irq, void *arg, bluefield_irq_handler handler);
int bluefield_irq_unregister(int irq, bluefield_irq_handler handler);
void bluefield_irq_init(void);
void bluefield_irq_enable(unsigned int id, int enable);

#endif

static inline void bluefield_delay_timer_init(void)
{
	/* Enable and initialize the System level generic timer */
	mmio_write_32(SYS_CNTCTL_BASE + CNTCR_OFF,
		      CNTCR_FCREQ(0) | CNTCR_EN);

	generic_delay_timer_init();
}

#endif /* __BLUEFIELD_PRIVATE_H__ */
