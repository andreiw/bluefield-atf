#ifndef __HW_PUB_H__
#define __HW_PUB_H__

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif
#include "pub_def.h"

#ifndef __ASSEMBLER__


#ifndef __DOXYGEN__


__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_2    : 1;
		uint32_t	zcalbyp         : 1;
		uint32_t	dcalpse         : 1;
		uint32_t	__reserved_1    : 9;
		uint32_t	rdimminit       : 1;
		uint32_t	ctldinit        : 1;
		uint32_t	vref            : 1;
		uint32_t	srd             : 1;
		uint32_t	wreye           : 1;
		uint32_t	rdeye           : 1;
		uint32_t	wrdskw          : 1;
		uint32_t	rddskw          : 1;
		uint32_t	wladj           : 1;
		uint32_t	qsgate          : 1;
		uint32_t	wl              : 1;
		uint32_t	draminit        : 1;
		uint32_t	dramrst         : 1;
		uint32_t	phyrst          : 1;
		uint32_t	dcal            : 1;
		uint32_t	pllinit         : 1;
		uint32_t	__reserved_0    : 1;
		uint32_t	ca              : 1;
		uint32_t	zcal            : 1;
		uint32_t	init            : 1;
#else
		uint32_t	init            : 1;
		uint32_t	zcal            : 1;
		uint32_t	ca              : 1;
		uint32_t	__reserved_0    : 1;
		uint32_t	pllinit         : 1;
		uint32_t	dcal            : 1;
		uint32_t	phyrst          : 1;
		uint32_t	dramrst         : 1;
		uint32_t	draminit        : 1;
		uint32_t	wl              : 1;
		uint32_t	qsgate          : 1;
		uint32_t	wladj           : 1;
		uint32_t	rddskw          : 1;
		uint32_t	wrdskw          : 1;
		uint32_t	rdeye           : 1;
		uint32_t	wreye           : 1;
		uint32_t	srd             : 1;
		uint32_t	vref            : 1;
		uint32_t	ctldinit        : 1;
		uint32_t	rdimminit       : 1;
		uint32_t	__reserved_1    : 9;
		uint32_t	dcalpse         : 1;
		uint32_t	zcalbyp         : 1;
		uint32_t	__reserved_2    : 1;
#endif
	};
	uint32_t word;
} PUB_PIR_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	adcp            : 1;
		uint32_t	x4oscwddl       : 2;
		uint32_t	x4oscwdl        : 2;
		uint32_t	phyfrst         : 1;
		uint32_t	oscacdl         : 2;
		uint32_t	oscwddl         : 2;
		uint32_t	__reserved_0    : 1;
		uint32_t	oscwdl          : 2;
		uint32_t	dtosel          : 5;
		uint32_t	acwlpon         : 1;
		uint32_t	oscdiv          : 4;
		uint32_t	oscen           : 1;
		uint32_t	dltst           : 1;
		uint32_t	dltmode         : 1;
		uint32_t	clrtstat        : 1;
		uint32_t	clrperr         : 1;
		uint32_t	initfsmbyp      : 1;
		uint32_t	pllfsmbyp       : 1;
		uint32_t	clrzcal         : 1;
		uint32_t	icpc            : 1;
#else
		uint32_t	icpc            : 1;
		uint32_t	clrzcal         : 1;
		uint32_t	pllfsmbyp       : 1;
		uint32_t	initfsmbyp      : 1;
		uint32_t	clrperr         : 1;
		uint32_t	clrtstat        : 1;
		uint32_t	dltmode         : 1;
		uint32_t	dltst           : 1;
		uint32_t	oscen           : 1;
		uint32_t	oscdiv          : 4;
		uint32_t	acwlpon         : 1;
		uint32_t	dtosel          : 5;
		uint32_t	oscwdl          : 2;
		uint32_t	__reserved_0    : 1;
		uint32_t	oscwddl         : 2;
		uint32_t	oscacdl         : 2;
		uint32_t	phyfrst         : 1;
		uint32_t	x4oscwdl        : 2;
		uint32_t	x4oscwddl       : 2;
		uint32_t	adcp            : 1;
#endif
	};
	uint32_t word;
} PUB_PGCR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	lbmode          : 1;
		uint32_t	lbgdqs          : 2;
		uint32_t	lbdqss          : 1;
		uint32_t	iolb            : 1;
		uint32_t	lyldtm          : 1;
		uint32_t	phyhrst         : 1;
		uint32_t	acvldtrn        : 1;
		uint32_t	acvlddly        : 3;
		uint32_t	lrdimmst        : 1;
		uint32_t	prcfg_en        : 1;
		uint32_t	updmstrc0       : 1;
		uint32_t	lpmstrc0        : 1;
		uint32_t	acpddc          : 1;
		uint32_t	dualchn         : 1;
		uint32_t	fdepth          : 2;
		uint32_t	lpfdepth        : 2;
		uint32_t	lpfen           : 1;
		uint32_t	mdlen           : 1;
		uint32_t	ioddrm          : 2;
		uint32_t	pubmode         : 1;
		uint32_t	ddlbypmode      : 2;
		uint32_t	wluncrt         : 1;
		uint32_t	wlstep          : 1;
		uint32_t	wlmode          : 1;
		uint32_t	dtomode         : 1;
#else
		uint32_t	dtomode         : 1;
		uint32_t	wlmode          : 1;
		uint32_t	wlstep          : 1;
		uint32_t	wluncrt         : 1;
		uint32_t	ddlbypmode      : 2;
		uint32_t	pubmode         : 1;
		uint32_t	ioddrm          : 2;
		uint32_t	mdlen           : 1;
		uint32_t	lpfen           : 1;
		uint32_t	lpfdepth        : 2;
		uint32_t	fdepth          : 2;
		uint32_t	dualchn         : 1;
		uint32_t	acpddc          : 1;
		uint32_t	lpmstrc0        : 1;
		uint32_t	updmstrc0       : 1;
		uint32_t	prcfg_en        : 1;
		uint32_t	lrdimmst        : 1;
		uint32_t	acvlddly        : 3;
		uint32_t	acvldtrn        : 1;
		uint32_t	phyhrst         : 1;
		uint32_t	lyldtm          : 1;
		uint32_t	iolb            : 1;
		uint32_t	lbdqss          : 1;
		uint32_t	lbgdqs          : 2;
		uint32_t	lbmode          : 1;
#endif
	};
	uint32_t word;
} PUB_PGCR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_0    : 1;
		uint32_t	rfshmode        : 2;
		uint32_t	fxdlatincr      : 1;
		uint32_t	dtpmxtmr        : 8;
		uint32_t	fxdlat          : 1;
		uint32_t	csncidmux       : 1;
		uint32_t	trefprd         : 18;
#else
		uint32_t	trefprd         : 18;
		uint32_t	csncidmux       : 1;
		uint32_t	fxdlat          : 1;
		uint32_t	dtpmxtmr        : 8;
		uint32_t	fxdlatincr      : 1;
		uint32_t	rfshmode        : 2;
		uint32_t	__reserved_0    : 1;
#endif
	};
	uint32_t word;
} PUB_PGCR2_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	acrdmode        : 1;
		uint32_t	acdccbyp        : 1;
		uint32_t	acddlld         : 1;
		uint32_t	acddlbyp        : 5;
		uint32_t	acvldtrnp       : 1;
		uint32_t	__reserved_0    : 2;
		uint32_t	dxddlld         : 5;
		uint32_t	dxddlbyp        : 16;
#else
		uint32_t	dxddlbyp        : 16;
		uint32_t	dxddlld         : 5;
		uint32_t	__reserved_0    : 2;
		uint32_t	acvldtrnp       : 1;
		uint32_t	acddlbyp        : 5;
		uint32_t	acddlld         : 1;
		uint32_t	acdccbyp        : 1;
		uint32_t	acrdmode        : 1;
#endif
	};
	uint32_t word;
} PUB_PGCR4_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_1    : 8;
		uint32_t	dldlmt          : 8;
		uint32_t	babvt           : 1;
		uint32_t	a15_10bvt       : 1;
		uint32_t	acdlvt          : 1;
		uint32_t	a9_0bvt         : 1;
		uint32_t	odtbvt          : 1;
		uint32_t	ckebvt          : 1;
		uint32_t	csnbvt          : 1;
		uint32_t	ckbvt           : 1;
		uint32_t	cidbvt          : 1;
		uint32_t	a16bvt          : 1;
		uint32_t	a17bvt          : 1;
		uint32_t	actnbvt         : 1;
		uint32_t	parbvt          : 1;
		uint32_t	__reserved_0    : 1;
		uint32_t	fvt             : 1;
		uint32_t	inhvt           : 1;
#else
		uint32_t	inhvt           : 1;
		uint32_t	fvt             : 1;
		uint32_t	__reserved_0    : 1;
		uint32_t	parbvt          : 1;
		uint32_t	actnbvt         : 1;
		uint32_t	a17bvt          : 1;
		uint32_t	a16bvt          : 1;
		uint32_t	cidbvt          : 1;
		uint32_t	ckbvt           : 1;
		uint32_t	csnbvt          : 1;
		uint32_t	ckebvt          : 1;
		uint32_t	odtbvt          : 1;
		uint32_t	a9_0bvt         : 1;
		uint32_t	acdlvt          : 1;
		uint32_t	a15_10bvt       : 1;
		uint32_t	babvt           : 1;
		uint32_t	dldlmt          : 8;
		uint32_t	__reserved_1    : 8;
#endif
	};
	uint32_t word;
} PUB_PGCR6_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	wrpstex         : 1;
		uint32_t	dxrsvd          : 4;
		uint32_t	dxdtomux        : 1;
		uint32_t	dxcalclk        : 1;
		uint32_t	dxrclkmd        : 1;
		uint32_t	dxqsgsel        : 1;
		uint32_t	dxdtosel        : 2;
		uint32_t	dxgsmd          : 1;
		uint32_t	dxddlldt        : 1;
		uint32_t	dxqsdbyp        : 1;
		uint32_t	dxgbyp          : 1;
		uint32_t	dxtmode         : 1;
		uint32_t	__reserved_0    : 8;
		uint32_t	cknstopl        : 1;
		uint32_t	acdtomux        : 1;
		uint32_t	accalclk        : 1;
		uint32_t	acrclkmd        : 1;
		uint32_t	acdldt          : 1;
		uint32_t	acrsvd          : 1;
		uint32_t	acdtosel        : 1;
		uint32_t	actmode         : 1;
#else
		uint32_t	actmode         : 1;
		uint32_t	acdtosel        : 1;
		uint32_t	acrsvd          : 1;
		uint32_t	acdldt          : 1;
		uint32_t	acrclkmd        : 1;
		uint32_t	accalclk        : 1;
		uint32_t	acdtomux        : 1;
		uint32_t	cknstopl        : 1;
		uint32_t	__reserved_0    : 8;
		uint32_t	dxtmode         : 1;
		uint32_t	dxgbyp          : 1;
		uint32_t	dxqsdbyp        : 1;
		uint32_t	dxddlldt        : 1;
		uint32_t	dxgsmd          : 1;
		uint32_t	dxdtosel        : 2;
		uint32_t	dxqsgsel        : 1;
		uint32_t	dxrclkmd        : 1;
		uint32_t	dxcalclk        : 1;
		uint32_t	dxdtomux        : 1;
		uint32_t	dxrsvd          : 4;
		uint32_t	wrpstex         : 1;
#endif
	};
	uint32_t word;
} PUB_PGCR7_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_0    : 22;
		uint32_t	dcaltype        : 1;
		uint32_t	dcalsval        : 9;
#else
		uint32_t	dcalsval        : 9;
		uint32_t	dcaltype        : 1;
		uint32_t	__reserved_0    : 22;
#endif
	};
	uint32_t word;
} PUB_PGCR8_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	aplock          : 1;
		uint32_t	srderr          : 1;
		uint32_t	cawrn           : 1;
		uint32_t	caerr           : 1;
		uint32_t	weerr           : 1;
		uint32_t	reerr           : 1;
		uint32_t	wderr           : 1;
		uint32_t	rderr           : 1;
		uint32_t	wlaerr          : 1;
		uint32_t	qsgerr          : 1;
		uint32_t	wlerr           : 1;
		uint32_t	zcerr           : 1;
		uint32_t	verr            : 1;
		uint32_t	__reserved_0    : 4;
		uint32_t	vdone           : 1;
		uint32_t	srddone         : 1;
		uint32_t	cadone          : 1;
		uint32_t	wedone          : 1;
		uint32_t	redone          : 1;
		uint32_t	wddone          : 1;
		uint32_t	rddone          : 1;
		uint32_t	wladone         : 1;
		uint32_t	qsgdone         : 1;
		uint32_t	wldone          : 1;
		uint32_t	didone          : 1;
		uint32_t	zcdone          : 1;
		uint32_t	dcdone          : 1;
		uint32_t	pldone          : 1;
		uint32_t	idone           : 1;
#else
		uint32_t	idone           : 1;
		uint32_t	pldone          : 1;
		uint32_t	dcdone          : 1;
		uint32_t	zcdone          : 1;
		uint32_t	didone          : 1;
		uint32_t	wldone          : 1;
		uint32_t	qsgdone         : 1;
		uint32_t	wladone         : 1;
		uint32_t	rddone          : 1;
		uint32_t	wddone          : 1;
		uint32_t	redone          : 1;
		uint32_t	wedone          : 1;
		uint32_t	cadone          : 1;
		uint32_t	srddone         : 1;
		uint32_t	vdone           : 1;
		uint32_t	__reserved_0    : 4;
		uint32_t	verr            : 1;
		uint32_t	zcerr           : 1;
		uint32_t	wlerr           : 1;
		uint32_t	qsgerr          : 1;
		uint32_t	wlaerr          : 1;
		uint32_t	rderr           : 1;
		uint32_t	wderr           : 1;
		uint32_t	reerr           : 1;
		uint32_t	weerr           : 1;
		uint32_t	caerr           : 1;
		uint32_t	cawrn           : 1;
		uint32_t	srderr          : 1;
		uint32_t	aplock          : 1;
#endif
	};
	uint32_t word;
} PUB_PGSR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	pllbyp          : 1;
		uint32_t	pllrst          : 1;
		uint32_t	pllpd           : 1;
		uint32_t	__reserved_0    : 4;
		uint32_t	lockps          : 1;
		uint32_t	lockcs          : 1;
		uint32_t	lockds          : 1;
		uint32_t	rgvint          : 1;
		uint32_t	frqsel          : 2;
		uint32_t	rgshift         : 1;
		uint32_t	cppc            : 4;
		uint32_t	cpic            : 2;
		uint32_t	gshift          : 1;
		uint32_t	atoen           : 4;
		uint32_t	atc             : 4;
		uint32_t	dtc             : 3;
#else
		uint32_t	dtc             : 3;
		uint32_t	atc             : 4;
		uint32_t	atoen           : 4;
		uint32_t	gshift          : 1;
		uint32_t	cpic            : 2;
		uint32_t	cppc            : 4;
		uint32_t	rgshift         : 1;
		uint32_t	frqsel          : 2;
		uint32_t	rgvint          : 1;
		uint32_t	lockds          : 1;
		uint32_t	lockcs          : 1;
		uint32_t	lockps          : 1;
		uint32_t	__reserved_0    : 4;
		uint32_t	pllpd           : 1;
		uint32_t	pllrst          : 1;
		uint32_t	pllbyp          : 1;
#endif
	};
	uint32_t word;
} PUB_PLLCR_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	x4mode          : 1;
		uint32_t	x4dqsmd         : 1;
		uint32_t	rkloop          : 1;
		uint32_t	__reserved_1    : 5;
		uint32_t	dxdccbyp        : 1;
		uint32_t	qscnten         : 1;
		uint32_t	udqiom          : 1;
		uint32_t	__reserved_0    : 3;
		uint32_t	msbudq          : 3;
		uint32_t	dxsr            : 2;
		uint32_t	dqsnres         : 4;
		uint32_t	dqsres          : 4;
		uint32_t	dqsglb          : 2;
		uint32_t	mdlen           : 1;
		uint32_t	dxiom           : 1;
		uint32_t	dxodt           : 1;
#else
		uint32_t	dxodt           : 1;
		uint32_t	dxiom           : 1;
		uint32_t	mdlen           : 1;
		uint32_t	dqsglb          : 2;
		uint32_t	dqsres          : 4;
		uint32_t	dqsnres         : 4;
		uint32_t	dxsr            : 2;
		uint32_t	msbudq          : 3;
		uint32_t	__reserved_0    : 3;
		uint32_t	udqiom          : 1;
		uint32_t	qscnten         : 1;
		uint32_t	dxdccbyp        : 1;
		uint32_t	__reserved_1    : 5;
		uint32_t	rkloop          : 1;
		uint32_t	x4dqsmd         : 1;
		uint32_t	x4mode          : 1;
#endif
	};
	uint32_t word;
} PUB_DXCCR_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_1    : 8;
		uint32_t	phyzuen         : 1;
		uint32_t	rrmode          : 1;
		uint32_t	rstoe           : 1;
		uint32_t	sdrmode         : 2;
		uint32_t	wrrmode         : 1;
		uint32_t	atoae           : 1;
		uint32_t	dtooe           : 1;
		uint32_t	dtoiom          : 1;
		uint32_t	dtopdr          : 1;
		uint32_t	__reserved_0    : 1;
		uint32_t	dtoodt          : 1;
		uint32_t	puad            : 4;
		uint32_t	dqsgx           : 2;
		uint32_t	cuaen           : 1;
		uint32_t	lppllpd         : 1;
		uint32_t	lpiopd          : 1;
		uint32_t	ctlzuen         : 1;
		uint32_t	bdisen          : 1;
		uint32_t	puren           : 1;
#else
		uint32_t	puren           : 1;
		uint32_t	bdisen          : 1;
		uint32_t	ctlzuen         : 1;
		uint32_t	lpiopd          : 1;
		uint32_t	lppllpd         : 1;
		uint32_t	cuaen           : 1;
		uint32_t	dqsgx           : 2;
		uint32_t	puad            : 4;
		uint32_t	dtoodt          : 1;
		uint32_t	__reserved_0    : 1;
		uint32_t	dtopdr          : 1;
		uint32_t	dtoiom          : 1;
		uint32_t	dtooe           : 1;
		uint32_t	atoae           : 1;
		uint32_t	wrrmode         : 1;
		uint32_t	sdrmode         : 2;
		uint32_t	rstoe           : 1;
		uint32_t	rrmode          : 1;
		uint32_t	phyzuen         : 1;
		uint32_t	__reserved_1    : 8;
#endif
	};
	uint32_t word;
} PUB_DSGCR_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_1    : 4;
		uint32_t	wrodt           : 12;
		uint32_t	__reserved_0    : 4;
		uint32_t	rdodt           : 12;
#else
		uint32_t	rdodt           : 12;
		uint32_t	__reserved_0    : 4;
		uint32_t	wrodt           : 12;
		uint32_t	__reserved_1    : 4;
#endif
	};
	uint32_t word;
} PUB_ODTCR_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_1    : 1;
		uint32_t	ubg             : 1;
		uint32_t	udimm           : 1;
		uint32_t	ddr2t           : 1;
		uint32_t	nosra           : 1;
		uint32_t	__reserved_0    : 9;
		uint32_t	bytemask        : 8;
		uint32_t	ddrtype         : 2;
		uint32_t	mprdq           : 1;
		uint32_t	pdq             : 3;
		uint32_t	ddr8bnk         : 1;
		uint32_t	ddrmd           : 3;
#else
		uint32_t	ddrmd           : 3;
		uint32_t	ddr8bnk         : 1;
		uint32_t	pdq             : 3;
		uint32_t	mprdq           : 1;
		uint32_t	ddrtype         : 2;
		uint32_t	bytemask        : 8;
		uint32_t	__reserved_0    : 9;
		uint32_t	nosra           : 1;
		uint32_t	ddr2t           : 1;
		uint32_t	udimm           : 1;
		uint32_t	ubg             : 1;
		uint32_t	__reserved_1    : 1;
#endif
	};
	uint32_t word;
} PUB_DCR_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	trrd            : 6;
		uint32_t	__reserved_2    : 1;
		uint32_t	tras            : 7;
		uint32_t	__reserved_1    : 1;
		uint32_t	trp             : 7;
		uint32_t	__reserved_0    : 4;
		uint32_t	trtp            : 4;
#else
		uint32_t	trtp            : 4;
		uint32_t	__reserved_0    : 4;
		uint32_t	trp             : 7;
		uint32_t	__reserved_1    : 1;
		uint32_t	tras            : 7;
		uint32_t	__reserved_2    : 1;
		uint32_t	trrd            : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_DTPR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_2    : 2;
		uint32_t	twlmrd          : 6;
		uint32_t	tfaw            : 8;
		uint32_t	__reserved_1    : 5;
		uint32_t	tmod            : 3;
		uint32_t	__reserved_0    : 3;
		uint32_t	tmrd            : 5;
#else
		uint32_t	tmrd            : 5;
		uint32_t	__reserved_0    : 3;
		uint32_t	tmod            : 3;
		uint32_t	__reserved_1    : 5;
		uint32_t	tfaw            : 8;
		uint32_t	twlmrd          : 6;
		uint32_t	__reserved_2    : 2;
#endif
	};
	uint32_t word;
} PUB_DTPR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 3;
		uint32_t	trtw            : 1;
		uint32_t	__reserved_2    : 3;
		uint32_t	trtodt          : 1;
		uint32_t	__reserved_1    : 4;
		uint32_t	tcke            : 4;
		uint32_t	__reserved_0    : 6;
		uint32_t	txs             : 10;
#else
		uint32_t	txs             : 10;
		uint32_t	__reserved_0    : 6;
		uint32_t	tcke            : 4;
		uint32_t	__reserved_1    : 4;
		uint32_t	trtodt          : 1;
		uint32_t	__reserved_2    : 3;
		uint32_t	trtw            : 1;
		uint32_t	__reserved_3    : 3;
#endif
	};
	uint32_t word;
} PUB_DTPR2_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	tofdx           : 3;
		uint32_t	tccd            : 3;
		uint32_t	tdllk           : 10;
		uint32_t	__reserved_1    : 5;
		uint32_t	tdqsckmax       : 3;
		uint32_t	__reserved_0    : 5;
		uint32_t	tdqsck          : 3;
#else
		uint32_t	tdqsck          : 3;
		uint32_t	__reserved_0    : 5;
		uint32_t	tdqsckmax       : 3;
		uint32_t	__reserved_1    : 5;
		uint32_t	tdllk           : 10;
		uint32_t	tccd            : 3;
		uint32_t	tofdx           : 3;
#endif
	};
	uint32_t word;
} PUB_DTPR3_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	taond_taofd     : 2;
		uint32_t	__reserved_2    : 2;
		uint32_t	trfc            : 10;
		uint32_t	__reserved_1    : 4;
		uint32_t	twlo            : 4;
		uint32_t	__reserved_0    : 3;
		uint32_t	txp             : 5;
#else
		uint32_t	txp             : 5;
		uint32_t	__reserved_0    : 3;
		uint32_t	twlo            : 4;
		uint32_t	__reserved_1    : 4;
		uint32_t	trfc            : 10;
		uint32_t	__reserved_2    : 2;
		uint32_t	taond_taofd     : 2;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_DTPR4_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_2    : 8;
		uint32_t	trc             : 8;
		uint32_t	__reserved_1    : 1;
		uint32_t	trcd            : 7;
		uint32_t	__reserved_0    : 3;
		uint32_t	twtr            : 5;
#else
		uint32_t	twtr            : 5;
		uint32_t	__reserved_0    : 3;
		uint32_t	trcd            : 7;
		uint32_t	__reserved_1    : 1;
		uint32_t	trc             : 8;
		uint32_t	__reserved_2    : 8;
#endif
	};
	uint32_t word;
} PUB_DTPR5_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	pubwlen         : 1;
		uint32_t	pubrlen         : 1;
		uint32_t	__reserved_1    : 16;
		uint32_t	pubwl           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	pubrl           : 6;
#else
		uint32_t	pubrl           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	pubwl           : 6;
		uint32_t	__reserved_1    : 16;
		uint32_t	pubrlen         : 1;
		uint32_t	pubwlen         : 1;
#endif
	};
	uint32_t word;
} PUB_DTPR6_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	mirror          : 1;
		uint32_t	qcsen           : 1;
		uint32_t	mirroroe        : 1;
		uint32_t	qcsenoe         : 1;
		uint32_t	rdimmiom        : 1;
		uint32_t	rdimmpdr        : 1;
		uint32_t	__reserved_3    : 1;
		uint32_t	rdimmodt        : 1;
		uint32_t	erroutoe        : 1;
		uint32_t	erroutiom       : 1;
		uint32_t	erroutpdr       : 1;
		uint32_t	__reserved_2    : 1;
		uint32_t	erroutodt       : 1;
		uint32_t	lrdimm          : 1;
		uint32_t	pariniom        : 1;
		uint32_t	parinpdr        : 1;
		uint32_t	__reserved_1    : 1;
		uint32_t	parinodt        : 1;
		uint32_t	__reserved_0    : 10;
		uint32_t	perrdis         : 1;
		uint32_t	soperr          : 1;
		uint32_t	errnoreg        : 1;
		uint32_t	rdimm           : 1;
#else
		uint32_t	rdimm           : 1;
		uint32_t	errnoreg        : 1;
		uint32_t	soperr          : 1;
		uint32_t	perrdis         : 1;
		uint32_t	__reserved_0    : 10;
		uint32_t	parinodt        : 1;
		uint32_t	__reserved_1    : 1;
		uint32_t	parinpdr        : 1;
		uint32_t	pariniom        : 1;
		uint32_t	lrdimm          : 1;
		uint32_t	erroutodt       : 1;
		uint32_t	__reserved_2    : 1;
		uint32_t	erroutpdr       : 1;
		uint32_t	erroutiom       : 1;
		uint32_t	erroutoe        : 1;
		uint32_t	rdimmodt        : 1;
		uint32_t	__reserved_3    : 1;
		uint32_t	rdimmpdr        : 1;
		uint32_t	rdimmiom        : 1;
		uint32_t	qcsenoe         : 1;
		uint32_t	mirroroe        : 1;
		uint32_t	qcsen           : 1;
		uint32_t	mirror          : 1;
#endif
	};
	uint32_t word;
} PUB_RDIMMGCR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	a17bid          : 4;
		uint32_t	__reserved_3    : 1;
		uint32_t	tbcmrd_l2       : 3;
		uint32_t	__reserved_2    : 1;
		uint32_t	tbcmrd_l        : 3;
		uint32_t	__reserved_1    : 1;
		uint32_t	tbcmrd          : 3;
		uint32_t	__reserved_0    : 2;
		uint32_t	tbcstab         : 14;
#else
		uint32_t	tbcstab         : 14;
		uint32_t	__reserved_0    : 2;
		uint32_t	tbcmrd          : 3;
		uint32_t	__reserved_1    : 1;
		uint32_t	tbcmrd_l        : 3;
		uint32_t	__reserved_2    : 1;
		uint32_t	tbcmrd_l2       : 3;
		uint32_t	__reserved_3    : 1;
		uint32_t	a17bid          : 4;
#endif
	};
	uint32_t word;
} PUB_RDIMMGCR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	crinit          : 32;
#else
		uint32_t	crinit          : 32;
#endif
	};
	uint32_t word;
} PUB_RDIMMGCR2_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	rc7             : 4;
		uint32_t	rc6             : 4;
		uint32_t	rc5             : 4;
		uint32_t	rc4             : 4;
		uint32_t	rc3             : 4;
		uint32_t	rc2             : 4;
		uint32_t	rc1             : 4;
		uint32_t	rc0             : 4;
#else
		uint32_t	rc0             : 4;
		uint32_t	rc1             : 4;
		uint32_t	rc2             : 4;
		uint32_t	rc3             : 4;
		uint32_t	rc4             : 4;
		uint32_t	rc5             : 4;
		uint32_t	rc6             : 4;
		uint32_t	rc7             : 4;
#endif
	};
	uint32_t word;
} PUB_RDIMMCR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	rc15            : 4;
		uint32_t	rc14            : 4;
		uint32_t	rc13            : 4;
		uint32_t	rc12            : 4;
		uint32_t	rc11            : 4;
		uint32_t	rc10            : 4;
		uint32_t	rc9             : 4;
		uint32_t	rc8             : 4;
#else
		uint32_t	rc8             : 4;
		uint32_t	rc9             : 4;
		uint32_t	rc10            : 4;
		uint32_t	rc11            : 4;
		uint32_t	rc12            : 4;
		uint32_t	rc13            : 4;
		uint32_t	rc14            : 4;
		uint32_t	rc15            : 4;
#endif
	};
	uint32_t word;
} PUB_RDIMMCR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	rc4x            : 8;
		uint32_t	rc3x            : 8;
		uint32_t	rc2x            : 8;
		uint32_t	rc1x            : 8;
#else
		uint32_t	rc1x            : 8;
		uint32_t	rc2x            : 8;
		uint32_t	rc3x            : 8;
		uint32_t	rc4x            : 8;
#endif
	};
	uint32_t word;
} PUB_RDIMMCR2_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	rc8x            : 8;
		uint32_t	rc7x            : 8;
		uint32_t	rc6x            : 8;
		uint32_t	rc5x            : 8;
#else
		uint32_t	rc5x            : 8;
		uint32_t	rc6x            : 8;
		uint32_t	rc7x            : 8;
		uint32_t	rc8x            : 8;
#endif
	};
	uint32_t word;
} PUB_RDIMMCR3_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	rccx            : 8;
		uint32_t	rcbx            : 8;
		uint32_t	rcax            : 8;
		uint32_t	rc9x            : 8;
#else
		uint32_t	rc9x            : 8;
		uint32_t	rcax            : 8;
		uint32_t	rcbx            : 8;
		uint32_t	rccx            : 8;
#endif
	};
	uint32_t word;
} PUB_RDIMMCR4_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	schdqv          : 18;
		uint32_t	__reserved_0    : 2;
		uint32_t	sp_cmd          : 4;
		uint32_t	cmd             : 4;
		uint32_t	schtrig         : 4;
#else
		uint32_t	schtrig         : 4;
		uint32_t	cmd             : 4;
		uint32_t	sp_cmd          : 4;
		uint32_t	__reserved_0    : 2;
		uint32_t	schdqv          : 18;
#endif
	};
	uint32_t word;
} PUB_SCHCR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	scrnk           : 4;
		uint32_t	scaddr          : 20;
		uint32_t	scbg            : 2;
		uint32_t	scbk            : 2;
		uint32_t	__reserved_1    : 1;
		uint32_t	allrank         : 1;
		uint32_t	__reserved_0    : 2;
#else
		uint32_t	__reserved_0    : 2;
		uint32_t	allrank         : 1;
		uint32_t	__reserved_1    : 1;
		uint32_t	scbk            : 2;
		uint32_t	scbg            : 2;
		uint32_t	scaddr          : 20;
		uint32_t	scrnk           : 4;
#endif
	};
	uint32_t word;
} PUB_SCHCR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	rfshdt          : 4;
		uint32_t	__reserved_2    : 2;
		uint32_t	dtdrs           : 2;
		uint32_t	dtexg           : 1;
		uint32_t	dtexd           : 1;
		uint32_t	dtdstp          : 1;
		uint32_t	dtden           : 1;
		uint32_t	dtdbs           : 4;
		uint32_t	dtrdbitr        : 2;
		uint32_t	dtbdc           : 1;
		uint32_t	dtwbddm         : 1;
		uint32_t	dtdbs4          : 1;
		uint32_t	__reserved_1    : 3;
		uint32_t	dtcmpd          : 1;
		uint32_t	dtmpr           : 1;
		uint32_t	__reserved_0    : 2;
		uint32_t	dtrptn          : 4;
#else
		uint32_t	dtrptn          : 4;
		uint32_t	__reserved_0    : 2;
		uint32_t	dtmpr           : 1;
		uint32_t	dtcmpd          : 1;
		uint32_t	__reserved_1    : 3;
		uint32_t	dtdbs4          : 1;
		uint32_t	dtwbddm         : 1;
		uint32_t	dtbdc           : 1;
		uint32_t	dtrdbitr        : 2;
		uint32_t	dtdbs           : 4;
		uint32_t	dtden           : 1;
		uint32_t	dtdstp          : 1;
		uint32_t	dtexd           : 1;
		uint32_t	dtexg           : 1;
		uint32_t	dtdrs           : 2;
		uint32_t	__reserved_2    : 2;
		uint32_t	rfshdt          : 4;
#endif
	};
	uint32_t word;
} PUB_DTCR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	ranken          : 16;
		uint32_t	__reserved_3    : 2;
		uint32_t	dtrank          : 2;
		uint32_t	__reserved_2    : 1;
		uint32_t	rdlvlgdiff      : 3;
		uint32_t	__reserved_1    : 1;
		uint32_t	rdlvlgs         : 3;
		uint32_t	__reserved_0    : 1;
		uint32_t	rdprmbl_trn     : 1;
		uint32_t	rdlvlen         : 1;
		uint32_t	bsten           : 1;
#else
		uint32_t	bsten           : 1;
		uint32_t	rdlvlen         : 1;
		uint32_t	rdprmbl_trn     : 1;
		uint32_t	__reserved_0    : 1;
		uint32_t	rdlvlgs         : 3;
		uint32_t	__reserved_1    : 1;
		uint32_t	rdlvlgdiff      : 3;
		uint32_t	__reserved_2    : 1;
		uint32_t	dtrank          : 2;
		uint32_t	__reserved_3    : 2;
		uint32_t	ranken          : 16;
#endif
	};
	uint32_t word;
} PUB_DTCR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	hvrefmx         : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	hvrefmn         : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dvrefmx         : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dvrefmn         : 6;
#else
		uint32_t	dvrefmn         : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dvrefmx         : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	hvrefmn         : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	hvrefmx         : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_VTDR_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_0    : 1;
		uint32_t	bsoma           : 1;
		uint32_t	bccsel          : 2;
		uint32_t	bcksel          : 2;
		uint32_t	bdxsel          : 4;
		uint32_t	bdpat           : 5;
		uint32_t	bdmen           : 1;
		uint32_t	bacen           : 1;
		uint32_t	bdxen           : 1;
		uint32_t	bsonf           : 1;
		uint32_t	nfail           : 8;
		uint32_t	binf            : 1;
		uint32_t	bmode           : 1;
		uint32_t	binst           : 3;
#else
		uint32_t	binst           : 3;
		uint32_t	bmode           : 1;
		uint32_t	binf            : 1;
		uint32_t	nfail           : 8;
		uint32_t	bsonf           : 1;
		uint32_t	bdxen           : 1;
		uint32_t	bacen           : 1;
		uint32_t	bdmen           : 1;
		uint32_t	bdpat           : 5;
		uint32_t	bdxsel          : 4;
		uint32_t	bcksel          : 2;
		uint32_t	bccsel          : 2;
		uint32_t	bsoma           : 1;
		uint32_t	__reserved_0    : 1;
#endif
	};
	uint32_t word;
} PUB_BISTRR_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_0    : 16;
		uint32_t	bwcnt           : 16;
#else
		uint32_t	bwcnt           : 16;
		uint32_t	__reserved_0    : 16;
#endif
	};
	uint32_t word;
} PUB_BISTWCR_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	csmsk           : 12;
		uint32_t	actmsk          : 1;
		uint32_t	__reserved_0    : 1;
		uint32_t	amsk            : 18;
#else
		uint32_t	amsk            : 18;
		uint32_t	__reserved_0    : 1;
		uint32_t	actmsk          : 1;
		uint32_t	csmsk           : 12;
#endif
	};
	uint32_t word;
} PUB_BISTMSKR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	dmmsk           : 4;
		uint32_t	parinmsk        : 1;
		uint32_t	__reserved_0    : 3;
		uint32_t	odtmsk          : 8;
		uint32_t	ckemsk          : 8;
		uint32_t	bamsk           : 4;
		uint32_t	x4dmmsk         : 4;
#else
		uint32_t	x4dmmsk         : 4;
		uint32_t	bamsk           : 4;
		uint32_t	ckemsk          : 8;
		uint32_t	odtmsk          : 8;
		uint32_t	__reserved_0    : 3;
		uint32_t	parinmsk        : 1;
		uint32_t	dmmsk           : 4;
#endif
	};
	uint32_t word;
} PUB_BISTMSKR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	dqmsk           : 32;
#else
		uint32_t	dqmsk           : 32;
#endif
	};
	uint32_t word;
} PUB_BISTMSKR2_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	seed            : 32;
#else
		uint32_t	seed            : 32;
#endif
	};
	uint32_t word;
} PUB_BISTLSR_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	bbank           : 4;
		uint32_t	__reserved_0    : 16;
		uint32_t	bcol            : 12;
#else
		uint32_t	bcol            : 12;
		uint32_t	__reserved_0    : 16;
		uint32_t	bbank           : 4;
#endif
	};
	uint32_t word;
} PUB_BISTAR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_0    : 12;
		uint32_t	bmrank          : 4;
		uint32_t	bainc           : 12;
		uint32_t	brank           : 4;
#else
		uint32_t	brank           : 4;
		uint32_t	bainc           : 12;
		uint32_t	bmrank          : 4;
		uint32_t	__reserved_0    : 12;
#endif
	};
	uint32_t word;
} PUB_BISTAR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	bmbank          : 4;
		uint32_t	__reserved_0    : 16;
		uint32_t	bmcol           : 12;
#else
		uint32_t	bmcol           : 12;
		uint32_t	__reserved_0    : 16;
		uint32_t	bmbank          : 4;
#endif
	};
	uint32_t word;
} PUB_BISTAR2_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_0    : 14;
		uint32_t	brow            : 18;
#else
		uint32_t	brow            : 18;
		uint32_t	__reserved_0    : 14;
#endif
	};
	uint32_t word;
} PUB_BISTAR3_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_0    : 14;
		uint32_t	bmrow           : 18;
#else
		uint32_t	bmrow           : 18;
		uint32_t	__reserved_0    : 14;
#endif
	};
	uint32_t word;
} PUB_BISTAR4_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	budp1           : 16;
		uint32_t	budp0           : 16;
#else
		uint32_t	budp0           : 16;
		uint32_t	budp1           : 16;
#endif
	};
	uint32_t word;
} PUB_BISTUDPR_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 1;
		uint32_t	parber          : 1;
		uint32_t	__reserved_2    : 1;
		uint32_t	rasber          : 1;
		uint32_t	__reserved_1    : 4;
		uint32_t	dmber           : 4;
		uint32_t	__reserved_0    : 5;
		uint32_t	x4dmber         : 4;
		uint32_t	bdxerr          : 9;
		uint32_t	bacerr          : 1;
		uint32_t	bdone           : 1;
#else
		uint32_t	bdone           : 1;
		uint32_t	bacerr          : 1;
		uint32_t	bdxerr          : 9;
		uint32_t	x4dmber         : 4;
		uint32_t	__reserved_0    : 5;
		uint32_t	dmber           : 4;
		uint32_t	__reserved_1    : 4;
		uint32_t	rasber          : 1;
		uint32_t	__reserved_2    : 1;
		uint32_t	parber          : 1;
		uint32_t	__reserved_3    : 1;
#endif
	};
	uint32_t word;
} PUB_BISTGSR_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_1    : 12;
		uint32_t	rankrid         : 4;
		uint32_t	__reserved_0    : 12;
		uint32_t	rankwid         : 4;
#else
		uint32_t	rankwid         : 4;
		uint32_t	__reserved_0    : 12;
		uint32_t	rankrid         : 4;
		uint32_t	__reserved_1    : 12;
#endif
	};
	uint32_t word;
} PUB_RANKIDR_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_0    : 2;
		uint32_t	coemode         : 6;
		uint32_t	csoemode        : 24;
#else
		uint32_t	csoemode        : 24;
		uint32_t	coemode         : 6;
		uint32_t	__reserved_0    : 2;
#endif
	};
	uint32_t word;
} PUB_RIOCR2_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_0    : 16;
		uint32_t	ckeoemode       : 16;
#else
		uint32_t	ckeoemode       : 16;
		uint32_t	__reserved_0    : 16;
#endif
	};
	uint32_t word;
} PUB_RIOCR4_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_0    : 16;
		uint32_t	odtoemode       : 16;
#else
		uint32_t	odtoemode       : 16;
		uint32_t	__reserved_0    : 16;
#endif
	};
	uint32_t word;
} PUB_RIOCR5_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	aoemode         : 32;
#else
		uint32_t	aoemode         : 32;
#endif
	};
	uint32_t word;
} PUB_ACIOCR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	paroemode       : 2;
		uint32_t	bgoemode        : 4;
		uint32_t	baoemode        : 4;
		uint32_t	a17oemode       : 2;
		uint32_t	a16oemode       : 2;
		uint32_t	actoemode       : 2;
		uint32_t	__reserved_0    : 8;
		uint32_t	ckoemode        : 8;
#else
		uint32_t	ckoemode        : 8;
		uint32_t	__reserved_0    : 8;
		uint32_t	actoemode       : 2;
		uint32_t	a16oemode       : 2;
		uint32_t	a17oemode       : 2;
		uint32_t	baoemode        : 4;
		uint32_t	bgoemode        : 4;
		uint32_t	paroemode       : 2;
#endif
	};
	uint32_t word;
} PUB_ACIOCR3_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	acvrefiom       : 3;
		uint32_t	acvrefpen       : 1;
		uint32_t	acvrefeen       : 2;
		uint32_t	acvrefsen       : 1;
		uint32_t	acvrefien       : 1;
		uint32_t	__reserved_2    : 2;
		uint32_t	acvrefesel      : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	acvrefssel      : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	acvrefisel      : 6;
#else
		uint32_t	acvrefisel      : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	acvrefssel      : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	acvrefesel      : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	acvrefien       : 1;
		uint32_t	acvrefsen       : 1;
		uint32_t	acvrefeen       : 2;
		uint32_t	acvrefpen       : 1;
		uint32_t	acvrefiom       : 3;
#endif
	};
	uint32_t word;
} PUB_IOVCR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_1    : 22;
		uint32_t	zqvrefpen       : 1;
		uint32_t	zqvrefien       : 1;
		uint32_t	__reserved_0    : 2;
		uint32_t	zqvrefisel      : 6;
#else
		uint32_t	zqvrefisel      : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	zqvrefien       : 1;
		uint32_t	zqvrefpen       : 1;
		uint32_t	__reserved_1    : 22;
#endif
	};
	uint32_t word;
} PUB_IOVCR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	tvref           : 3;
		uint32_t	dven            : 1;
		uint32_t	pdaen           : 1;
		uint32_t	__reserved_0    : 5;
		uint32_t	dvss            : 4;
		uint32_t	dvmax           : 6;
		uint32_t	dvmin           : 6;
		uint32_t	dvinit          : 6;
#else
		uint32_t	dvinit          : 6;
		uint32_t	dvmin           : 6;
		uint32_t	dvmax           : 6;
		uint32_t	dvss            : 4;
		uint32_t	__reserved_0    : 5;
		uint32_t	pdaen           : 1;
		uint32_t	dven            : 1;
		uint32_t	tvref           : 3;
#endif
	};
	uint32_t word;
} PUB_VTCR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	hvss            : 4;
		uint32_t	hvmax           : 6;
		uint32_t	hvmin           : 6;
		uint32_t	vwcr            : 4;
		uint32_t	__reserved_0    : 1;
		uint32_t	shrnk           : 2;
		uint32_t	shren           : 1;
		uint32_t	tvrefio         : 3;
		uint32_t	eoff            : 2;
		uint32_t	enum0           : 1;
		uint32_t	hven            : 1;
		uint32_t	hvio            : 1;
#else
		uint32_t	hvio            : 1;
		uint32_t	hven            : 1;
		uint32_t	enum0           : 1;
		uint32_t	eoff            : 2;
		uint32_t	tvrefio         : 3;
		uint32_t	shren           : 1;
		uint32_t	shrnk           : 2;
		uint32_t	__reserved_0    : 1;
		uint32_t	vwcr            : 4;
		uint32_t	hvmin           : 6;
		uint32_t	hvmax           : 6;
		uint32_t	hvss            : 4;
#endif
	};
	uint32_t word;
} PUB_VTCR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	ck3bd           : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	ck2bd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	ck1bd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	ck0bd           : 6;
#else
		uint32_t	ck0bd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	ck1bd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	ck2bd           : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	ck3bd           : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_ACBDLR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	parbd           : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	a16bd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	a17bd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	actbd           : 6;
#else
		uint32_t	actbd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	a17bd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	a16bd           : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	parbd           : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_ACBDLR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	bg1bd           : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	bg0bd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	ba1bd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	ba0bd           : 6;
#else
		uint32_t	ba0bd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	ba1bd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	bg0bd           : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	bg1bd           : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_ACBDLR2_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	cs3bd           : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	cs2bd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	cs1bd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	cs0bd           : 6;
#else
		uint32_t	cs0bd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	cs1bd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	cs2bd           : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	cs3bd           : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_ACBDLR3_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	odt3bd          : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	odt2bd          : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	odt1bd          : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	odt0bd          : 6;
#else
		uint32_t	odt0bd          : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	odt1bd          : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	odt2bd          : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	odt3bd          : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_ACBDLR4_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	cke3bd          : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	cke2bd          : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	cke1bd          : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	cke0bd          : 6;
#else
		uint32_t	cke0bd          : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	cke1bd          : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	cke2bd          : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	cke3bd          : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_ACBDLR5_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	a03bd           : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	a02bd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	a01bd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	a00bd           : 6;
#else
		uint32_t	a00bd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	a01bd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	a02bd           : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	a03bd           : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_ACBDLR6_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	a07bd           : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	a06bd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	a05bd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	a04bd           : 6;
#else
		uint32_t	a04bd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	a05bd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	a06bd           : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	a07bd           : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_ACBDLR7_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	a11bd           : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	a10bd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	a09bd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	a08bd           : 6;
#else
		uint32_t	a08bd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	a09bd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	a10bd           : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	a11bd           : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_ACBDLR8_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	a15bd           : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	a14bd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	a13bd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	a12bd           : 6;
#else
		uint32_t	a12bd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	a13bd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	a14bd           : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	a15bd           : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_ACBDLR9_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	cid2bd          : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	cid1bd          : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	cid0bd          : 6;
		uint32_t	__reserved_0    : 8;
#else
		uint32_t	__reserved_0    : 8;
		uint32_t	cid0bd          : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	cid1bd          : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	cid2bd          : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_ACBDLR10_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_0    : 23;
		uint32_t	acd             : 9;
#else
		uint32_t	acd             : 9;
		uint32_t	__reserved_0    : 23;
#endif
	};
	uint32_t word;
} PUB_ACLCDLR_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_1    : 7;
		uint32_t	tprd            : 9;
		uint32_t	__reserved_0    : 7;
		uint32_t	iprd            : 9;
#else
		uint32_t	iprd            : 9;
		uint32_t	__reserved_0    : 7;
		uint32_t	tprd            : 9;
		uint32_t	__reserved_1    : 7;
#endif
	};
	uint32_t word;
} PUB_ACMDLR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_0    : 23;
		uint32_t	mdld            : 9;
#else
		uint32_t	mdld            : 9;
		uint32_t	__reserved_0    : 23;
#endif
	};
	uint32_t word;
} PUB_ACMDLR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	zctrl_upper     : 4;
		uint32_t	force_zcat_vt_updat: 1;
		uint32_t	dis_mon_lin_comp: 1;
		uint32_t	pu_odt_only     : 1;
		uint32_t	asym_drv_en     : 1;
		uint32_t	iodlmt          : 7;
		uint32_t	avgen           : 1;
		uint32_t	avgmax          : 2;
		uint32_t	zcalt           : 3;
		uint32_t	pgwait          : 3;
		uint32_t	__reserved_1    : 5;
		uint32_t	zqpd            : 1;
		uint32_t	term_off        : 1;
		uint32_t	__reserved_0    : 1;
#else
		uint32_t	__reserved_0    : 1;
		uint32_t	term_off        : 1;
		uint32_t	zqpd            : 1;
		uint32_t	__reserved_1    : 5;
		uint32_t	pgwait          : 3;
		uint32_t	zcalt           : 3;
		uint32_t	avgmax          : 2;
		uint32_t	avgen           : 1;
		uint32_t	iodlmt          : 7;
		uint32_t	asym_drv_en     : 1;
		uint32_t	pu_odt_only     : 1;
		uint32_t	dis_mon_lin_comp: 1;
		uint32_t	force_zcat_vt_updat: 1;
		uint32_t	zctrl_upper     : 4;
#endif
	};
	uint32_t word;
} PUB_ZQCR_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_0    : 4;
		uint32_t	zctrl_upper     : 4;
		uint32_t	pd_drv_adjust   : 2;
		uint32_t	pu_drv_adjust   : 2;
		uint32_t	zprog_pu_odt_only: 4;
		uint32_t	zprog_asym_drv_pd: 4;
		uint32_t	zprog_asym_drv_pu: 4;
		uint32_t	zqdiv           : 8;
#else
		uint32_t	zqdiv           : 8;
		uint32_t	zprog_asym_drv_pu: 4;
		uint32_t	zprog_asym_drv_pd: 4;
		uint32_t	zprog_pu_odt_only: 4;
		uint32_t	pu_drv_adjust   : 2;
		uint32_t	pd_drv_adjust   : 2;
		uint32_t	zctrl_upper     : 4;
		uint32_t	__reserved_0    : 4;
#endif
	};
	uint32_t word;
} PUB_ZQnPR_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	dxpdrmode       : 16;
		uint32_t	__reserved_0    : 16;
#else
		uint32_t	__reserved_0    : 16;
		uint32_t	dxpdrmode       : 16;
#endif
	};
	uint32_t word;
} PUB_DXnGCR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	dxoemode        : 16;
		uint32_t	dxtemode        : 16;
#else
		uint32_t	dxtemode        : 16;
		uint32_t	dxoemode        : 16;
#endif
	};
	uint32_t word;
} PUB_DXnGCR2_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	rdmbvt          : 1;
		uint32_t	wdmbvt          : 1;
		uint32_t	rdbvt           : 1;
		uint32_t	wdbvt           : 1;
		uint32_t	rglvt           : 1;
		uint32_t	rdlvt           : 1;
		uint32_t	wdlvt           : 1;
		uint32_t	wllvt           : 1;
		uint32_t	rgslvt          : 1;
		uint32_t	rdsbvt          : 1;
		uint32_t	wdsbvt          : 1;
		uint32_t	tebvt           : 1;
		uint32_t	pdrbvt          : 1;
		uint32_t	oebvt           : 1;
		uint32_t	__reserved_2    : 2;
		uint32_t	dmoemode        : 2;
		uint32_t	dmtemode        : 2;
		uint32_t	dmpdrmode       : 2;
		uint32_t	__reserved_1    : 2;
		uint32_t	dsoemode        : 2;
		uint32_t	dstemode        : 2;
		uint32_t	dspdrmode       : 2;
		uint32_t	__reserved_0    : 2;
#else
		uint32_t	__reserved_0    : 2;
		uint32_t	dspdrmode       : 2;
		uint32_t	dstemode        : 2;
		uint32_t	dsoemode        : 2;
		uint32_t	__reserved_1    : 2;
		uint32_t	dmpdrmode       : 2;
		uint32_t	dmtemode        : 2;
		uint32_t	dmoemode        : 2;
		uint32_t	__reserved_2    : 2;
		uint32_t	oebvt           : 1;
		uint32_t	pdrbvt          : 1;
		uint32_t	tebvt           : 1;
		uint32_t	wdsbvt          : 1;
		uint32_t	rdsbvt          : 1;
		uint32_t	rgslvt          : 1;
		uint32_t	wllvt           : 1;
		uint32_t	wdlvt           : 1;
		uint32_t	rdlvt           : 1;
		uint32_t	rglvt           : 1;
		uint32_t	wdbvt           : 1;
		uint32_t	rdbvt           : 1;
		uint32_t	wdmbvt          : 1;
		uint32_t	rdmbvt          : 1;
#endif
	};
	uint32_t word;
} PUB_DXnGCR3_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	dxrefiom        : 3;
		uint32_t	dxrefpen        : 1;
		uint32_t	dxrefeen        : 2;
		uint32_t	dxrefsen        : 1;
		uint32_t	__reserved_2    : 3;
		uint32_t	dxrefesel       : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dxrefssel       : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dxrefien        : 4;
		uint32_t	dxrefimon       : 2;
#else
		uint32_t	dxrefimon       : 2;
		uint32_t	dxrefien        : 4;
		uint32_t	__reserved_0    : 2;
		uint32_t	dxrefssel       : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dxrefesel       : 6;
		uint32_t	__reserved_2    : 3;
		uint32_t	dxrefsen        : 1;
		uint32_t	dxrefeen        : 2;
		uint32_t	dxrefpen        : 1;
		uint32_t	dxrefiom        : 3;
#endif
	};
	uint32_t word;
} PUB_DXnGCR4_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	dxrefiselr3     : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	dxrefiselr2     : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dxrefiselr1     : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dxrefiselr0     : 6;
#else
		uint32_t	dxrefiselr0     : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dxrefiselr1     : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dxrefiselr2     : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	dxrefiselr3     : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_DXnGCR5_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	dxdqvrefr3      : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	dxdqvrefr2      : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dxdqvrefr1      : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dxdqvrefr0      : 6;
#else
		uint32_t	dxdqvrefr0      : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dxdqvrefr1      : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dxdqvrefr2      : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	dxdqvrefr3      : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_DXnGCR6_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_4    : 2;
		uint32_t	x4rddly         : 4;
		uint32_t	x4rttoal        : 1;
		uint32_t	x4rttoh         : 2;
		uint32_t	x4dqsnsepdr     : 1;
		uint32_t	x4dqssepdr      : 1;
		uint32_t	__reserved_3    : 1;
		uint32_t	x4dqsgpdr       : 1;
		uint32_t	__reserved_2    : 1;
		uint32_t	x4dqsgodt       : 1;
		uint32_t	x4dqsgoe        : 1;
		uint32_t	x4dxoemode      : 2;
		uint32_t	x4dxtemode      : 2;
		uint32_t	x4dxpdrmode     : 2;
		uint32_t	__reserved_1    : 2;
		uint32_t	x4dsoemode      : 2;
		uint32_t	x4dstemode      : 2;
		uint32_t	x4dspdrmode     : 2;
		uint32_t	__reserved_0    : 2;
#else
		uint32_t	__reserved_0    : 2;
		uint32_t	x4dspdrmode     : 2;
		uint32_t	x4dstemode      : 2;
		uint32_t	x4dsoemode      : 2;
		uint32_t	__reserved_1    : 2;
		uint32_t	x4dxpdrmode     : 2;
		uint32_t	x4dxtemode      : 2;
		uint32_t	x4dxoemode      : 2;
		uint32_t	x4dqsgoe        : 1;
		uint32_t	x4dqsgodt       : 1;
		uint32_t	__reserved_2    : 1;
		uint32_t	x4dqsgpdr       : 1;
		uint32_t	__reserved_3    : 1;
		uint32_t	x4dqssepdr      : 1;
		uint32_t	x4dqsnsepdr     : 1;
		uint32_t	x4rttoh         : 2;
		uint32_t	x4rttoal        : 1;
		uint32_t	x4rddly         : 4;
		uint32_t	__reserved_4    : 2;
#endif
	};
	uint32_t word;
} PUB_DXnGCR7_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	x4dxrefiselr3   : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	x4dxrefiselr2   : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	x4dxrefiselr1   : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	x4dxrefiselr0   : 6;
#else
		uint32_t	x4dxrefiselr0   : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	x4dxrefiselr1   : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	x4dxrefiselr2   : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	x4dxrefiselr3   : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_DXnGCR8_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	x4dxdqvrefr3    : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	x4dxdqvrefr2    : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	x4dxdqvrefr1    : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	x4dxdqvrefr0    : 6;
#else
		uint32_t	x4dxdqvrefr0    : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	x4dxdqvrefr1    : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	x4dxdqvrefr2    : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	x4dxdqvrefr3    : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_DXnGCR9_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	dq3wbd          : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	dq2wbd          : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dq1wbd          : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dq0wbd          : 6;
#else
		uint32_t	dq0wbd          : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dq1wbd          : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dq2wbd          : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	dq3wbd          : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_DXnBDLR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	dq7wbd          : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	dq6wbd          : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dq5wbd          : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dq4wbd          : 6;
#else
		uint32_t	dq4wbd          : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dq5wbd          : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dq6wbd          : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	dq7wbd          : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_DXnBDLR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_2    : 10;
		uint32_t	dsoebd          : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dswbd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dmwbd           : 6;
#else
		uint32_t	dmwbd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dswbd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dsoebd          : 6;
		uint32_t	__reserved_2    : 10;
#endif
	};
	uint32_t word;
} PUB_DXnBDLR2_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	dq3rbd          : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	dq2rbd          : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dq1rbd          : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dq0rbd          : 6;
#else
		uint32_t	dq0rbd          : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dq1rbd          : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dq2rbd          : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	dq3rbd          : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_DXnBDLR3_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_3    : 2;
		uint32_t	dq7rbd          : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	dq6rbd          : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dq5rbd          : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dq4rbd          : 6;
#else
		uint32_t	dq4rbd          : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dq5rbd          : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dq6rbd          : 6;
		uint32_t	__reserved_2    : 2;
		uint32_t	dq7rbd          : 6;
		uint32_t	__reserved_3    : 2;
#endif
	};
	uint32_t word;
} PUB_DXnBDLR4_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_2    : 10;
		uint32_t	dsnrbd          : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dsrbd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dmrbd           : 6;
#else
		uint32_t	dmrbd           : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	dsrbd           : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	dsnrbd          : 6;
		uint32_t	__reserved_2    : 10;
#endif
	};
	uint32_t word;
} PUB_DXnBDLR5_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_2    : 10;
		uint32_t	x4dsoebd        : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	x4dswbd         : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	x4dmwbd         : 6;
#else
		uint32_t	x4dmwbd         : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	x4dswbd         : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	x4dsoebd        : 6;
		uint32_t	__reserved_2    : 10;
#endif
	};
	uint32_t word;
} PUB_DXnBDLR7_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_2    : 10;
		uint32_t	x4dsnrbd        : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	x4dsrbd         : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	x4dmrbd         : 6;
#else
		uint32_t	x4dmrbd         : 6;
		uint32_t	__reserved_0    : 2;
		uint32_t	x4dsrbd         : 6;
		uint32_t	__reserved_1    : 2;
		uint32_t	x4dsnrbd        : 6;
		uint32_t	__reserved_2    : 10;
#endif
	};
	uint32_t word;
} PUB_DXnBDLR8_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_1    : 7;
		uint32_t	x4wld           : 9;
		uint32_t	__reserved_0    : 7;
		uint32_t	wld             : 9;
#else
		uint32_t	wld             : 9;
		uint32_t	__reserved_0    : 7;
		uint32_t	x4wld           : 9;
		uint32_t	__reserved_1    : 7;
#endif
	};
	uint32_t word;
} PUB_DXnLCDLR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_1    : 7;
		uint32_t	x4wdqd          : 9;
		uint32_t	__reserved_0    : 7;
		uint32_t	wdqd            : 9;
#else
		uint32_t	wdqd            : 9;
		uint32_t	__reserved_0    : 7;
		uint32_t	x4wdqd          : 9;
		uint32_t	__reserved_1    : 7;
#endif
	};
	uint32_t word;
} PUB_DXnLCDLR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_1    : 7;
		uint32_t	x4dqsgd         : 9;
		uint32_t	__reserved_0    : 7;
		uint32_t	dqsgd           : 9;
#else
		uint32_t	dqsgd           : 9;
		uint32_t	__reserved_0    : 7;
		uint32_t	x4dqsgd         : 9;
		uint32_t	__reserved_1    : 7;
#endif
	};
	uint32_t word;
} PUB_DXnLCDLR2_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_1    : 7;
		uint32_t	x4rdqsd         : 9;
		uint32_t	__reserved_0    : 7;
		uint32_t	rdqsd           : 9;
#else
		uint32_t	rdqsd           : 9;
		uint32_t	__reserved_0    : 7;
		uint32_t	x4rdqsd         : 9;
		uint32_t	__reserved_1    : 7;
#endif
	};
	uint32_t word;
} PUB_DXnLCDLR3_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_1    : 7;
		uint32_t	x4rdqsnd        : 9;
		uint32_t	__reserved_0    : 7;
		uint32_t	rdqsnd          : 9;
#else
		uint32_t	rdqsnd          : 9;
		uint32_t	__reserved_0    : 7;
		uint32_t	x4rdqsnd        : 9;
		uint32_t	__reserved_1    : 7;
#endif
	};
	uint32_t word;
} PUB_DXnLCDLR4_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_1    : 7;
		uint32_t	x4dqsgsd        : 9;
		uint32_t	__reserved_0    : 7;
		uint32_t	dqsgsd          : 9;
#else
		uint32_t	dqsgsd          : 9;
		uint32_t	__reserved_0    : 7;
		uint32_t	x4dqsgsd        : 9;
		uint32_t	__reserved_1    : 7;
#endif
	};
	uint32_t word;
} PUB_DXnLCDLR5_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_1    : 7;
		uint32_t	tprd            : 9;
		uint32_t	__reserved_0    : 7;
		uint32_t	iprd            : 9;
#else
		uint32_t	iprd            : 9;
		uint32_t	__reserved_0    : 7;
		uint32_t	tprd            : 9;
		uint32_t	__reserved_1    : 7;
#endif
	};
	uint32_t word;
} PUB_DXnMDLR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_0    : 23;
		uint32_t	mdld            : 9;
#else
		uint32_t	mdld            : 9;
		uint32_t	__reserved_0    : 23;
#endif
	};
	uint32_t word;
} PUB_DXnMDLR1_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	__reserved_2    : 8;
		uint32_t	x4wlsl          : 4;
		uint32_t	wlsl            : 4;
		uint32_t	__reserved_1    : 3;
		uint32_t	x4dgsl          : 5;
		uint32_t	__reserved_0    : 3;
		uint32_t	dgsl            : 5;
#else
		uint32_t	dgsl            : 5;
		uint32_t	__reserved_0    : 3;
		uint32_t	x4dgsl          : 5;
		uint32_t	__reserved_1    : 3;
		uint32_t	wlsl            : 4;
		uint32_t	x4wlsl          : 4;
		uint32_t	__reserved_2    : 8;
#endif
	};
	uint32_t word;
} PUB_DXnGTR0_t;

__extension__
typedef union
{
	struct
	{
#ifdef __BIG_ENDIAN__
		uint32_t	gsdqsprd        : 9;
		uint32_t	gsdqscal        : 1;
		uint32_t	__reserved_0    : 1;
		uint32_t	srderr          : 1;
		uint32_t	dbdq            : 8;
		uint32_t	estat           : 4;
		uint32_t	wewn            : 1;
		uint32_t	weerr           : 1;
		uint32_t	rewn            : 1;
		uint32_t	reerr           : 1;
		uint32_t	wdwn            : 1;
		uint32_t	wderr           : 1;
		uint32_t	rdwn            : 1;
		uint32_t	rderr           : 1;
#else
		uint32_t	rderr           : 1;
		uint32_t	rdwn            : 1;
		uint32_t	wderr           : 1;
		uint32_t	wdwn            : 1;
		uint32_t	reerr           : 1;
		uint32_t	rewn            : 1;
		uint32_t	weerr           : 1;
		uint32_t	wewn            : 1;
		uint32_t	estat           : 4;
		uint32_t	dbdq            : 8;
		uint32_t	srderr          : 1;
		uint32_t	__reserved_0    : 1;
		uint32_t	gsdqscal        : 1;
		uint32_t	gsdqsprd        : 9;
#endif
	};
	uint32_t word;
} PUB_DXnGSR2_t;

#endif /* !defined(__DOXYGEN__) */

#endif /* !defined(__ASSEMBLER__) */

#endif /* !defined(__HW_PUB_H__) */
