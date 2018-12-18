#ifndef __REGS_RSH_WD_CONTROL_H__
#define __REGS_RSH_WD_CONTROL_H__

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif
#include "rsh_wd_control_def.h"

#ifndef __ASSEMBLER__


#ifndef __DOXYGEN__

/*
 * MMIO Address Space.
 * The MMIO physical address space for the ARM Watchdog control frame is
 * described below.  This is a general description of the MMIO space as
 * opposed to a register description
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint32_t __reserved_0 : 12;
		/*
		 * This field of the address selects the region to be
		 * accessed.
		 */
		uint32_t frames       : 5;
		/* Reserved. */
		uint32_t __reserved_1 : 15;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1 : 15;
		uint32_t frames       : 5;
		uint32_t __reserved_0 : 12;
#endif
	};

	uint32_t word;
} RSH_WD_CONTROL_MMIO_ADDRESS_SPACE_t;


/*
 * ARM Watchdog Control and Status.
 * Watchdog Control and Status.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * A write of 1 to this bit enables the Watchdog, a 0
		 * disables it.  Enable is reset to 0 on cold reset.
		 */
		uint32_t en         : 1;
		/*
		 * Indicates the current state of the watchdog signal WS0.  A
		 * write to this bit has no effect.
		 */
		uint32_t ws0_status : 1;
		/*
		 * Indicates the current state of the watchdog signal WS1.  A
		 * write to this bit has no effect.
		 */
		uint32_t ws1_status : 1;
		/* Reserved. */
		uint32_t __reserved : 29;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved : 29;
		uint32_t ws1_status : 1;
		uint32_t ws0_status : 1;
		uint32_t en         : 1;
#endif
	};

	uint32_t word;
} RSH_WD_CONTROL_WCS_t;


/*
 * ARM Watchdog Offset.
 * Watchdog Offset register.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * A read/write register containing the unsigned 32bit
		 * watchdog countdown timer value
		 */
		uint32_t offset : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t offset : 32;
#endif
	};

	uint32_t word;
} RSH_WD_CONTROL_WOR_t;


/*
 * ARM Watchdog Compare Value Lo.
 * Watchdog Compare Value register, low 32bits.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * A read/write register containing the low 32bits of the
		 * current value in the watchdog compare register.
		 */
		uint32_t comp_val_lo : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t comp_val_lo : 32;
#endif
	};

	uint32_t word;
} RSH_WD_CONTROL_WCV_LO_t;


/*
 * ARM Watchdog Compare Value Hi.
 * Watchdog Compare Value register, high 32bits.
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/*
		 * A read/write register containing the high 32bits of the
		 * current value in the watchdog compare register.
		 */
		uint32_t comp_val_hi : 32;
#else   /* __BIG_ENDIAN__ */
		uint32_t comp_val_hi : 32;
#endif
	};

	uint32_t word;
} RSH_WD_CONTROL_WCV_HI_t;


/*
 * ARM Watchdog Interface Identification Register.
 * Read-only register that contains watchdog identification information
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Contains the JEP106 code of the company */
		uint32_t impl     : 12;
		/* Revision number for component */
		uint32_t rev      : 4;
		/* Architectural version of generic watchdog */
		uint32_t arch_ver : 4;
		/* Product identifier */
		uint32_t prod_id  : 12;
#else   /* __BIG_ENDIAN__ */
		uint32_t prod_id  : 12;
		uint32_t arch_ver : 4;
		uint32_t rev      : 4;
		uint32_t impl     : 12;
#endif
	};

	uint32_t word;
} RSH_WD_CONTROL_W_IIDR_t;


/*
 * ARM Watchdog Peripheral ID2 Register.
 * Read-only watchdog identification register
 */

__extension__
typedef union {
	struct {
#ifndef __BIG_ENDIAN__
		/* Reserved. */
		uint32_t __reserved_0 : 4;
		/*
		 * Generic watchdog architecture revision.  0x0 for generic
		 * watchdog v0
		 */
		uint32_t arch_rev     : 4;
		/* Reserved. */
		uint32_t __reserved_1 : 24;
#else   /* __BIG_ENDIAN__ */
		uint32_t __reserved_1 : 24;
		uint32_t arch_rev     : 4;
		uint32_t __reserved_0 : 4;
#endif
	};

	uint32_t word;
} RSH_WD_CONTROL_W_PIDR2_t;

#endif /* !defined(__DOXYGEN__) */

#endif /* !defined(__ASSEMBLER__) */

#endif /* !defined(__REGS_RSH_WD_CONTROL_H__) */
