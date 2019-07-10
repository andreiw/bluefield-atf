#ifndef __HW_PUB_DEF_H__
#define __HW_PUB_DEF_H__


#ifndef __DOXYGEN__

//Revision Identification Register
#define	PUB_RIDR	0x000
//PHY Initialization Register
#define	PUB_PIR	0x001
//Clock Gating Configuration Register
#define	PUB_CGCR	0x002
//Clock Gating Configuration Register 1
#define	PUB_CGCR1	0x003
//PHY General Configuration Register 0
#define	PUB_PGCR0	0x004
//PHY General Configuration Register 1
#define	PUB_PGCR1	0x005
//PHY General Configuration Register 2
#define	PUB_PGCR2	0x006
//PHY General Configuration Register 3
#define	PUB_PGCR3	0x007
//PHY General Configuration Register 4
#define	PUB_PGCR4	0x008
//PHY General Configuration Register 5
#define	PUB_PGCR5	0x009
//PHY General Configuration Register 6
#define	PUB_PGCR6	0x00A
//PHY General Configuration Register 7
#define	PUB_PGCR7	0x00B
//PHY General Configuration Register 8
#define	PUB_PGCR8	0x00C
//PHY General Status Register 0
#define	PUB_PGSR0	0x00D
//PHY General Status Register 1
#define	PUB_PGSR1	0x00E
//PHY Timing Register 0
#define	PUB_PTR0	0x010
//PHY Timing Register 1
#define	PUB_PTR1	0x011
//PHY Timing Register 2
#define	PUB_PTR2	0x012
//PHY Timing Register 3
#define	PUB_PTR3	0x013
//PHY Timing Register 4
#define	PUB_PTR4	0x014
//PHY Timing Register 5
#define	PUB_PTR5	0x015
//PHY Timing Register 6
#define	PUB_PTR6	0x016
//PLL Control Register (Type A PLL Only)
#define	PUB_PLLCR	0x020
//DATX8 Common Configuration Register
#define	PUB_DXCCR	0x022
//DDR System General Configuration Register
#define	PUB_DSGCR	0x024
//ODT Configuration Register
#define	PUB_ODTCR	0x026
//Anti-Aging Configuration Register
#define	PUB_AACR	0x028
//General Purpose Register 0
#define	PUB_GPR0	0x030
//General Purpose Register 1
#define	PUB_GPR1	0x031
//DRAM Configuration Register
#define	PUB_DCR	0x040
//DRAM Timing Parameters Register 0
#define	PUB_DTPR0	0x044
//DRAM Timing Parameters Register 1
#define	PUB_DTPR1	0x045
//DRAM Timing Parameters Register 2
#define	PUB_DTPR2	0x046
//DRAM Timing Parameters Register 3
#define	PUB_DTPR3	0x047
//DRAM Timing Parameters Register 4
#define	PUB_DTPR4	0x048
//DRAM Timing Parameters Register 5
#define	PUB_DTPR5	0x049
//DRAM Timing Parameters Register 6
#define	PUB_DTPR6	0x04A
//RDIMM General Configuration Register 0
#define	PUB_RDIMMGCR0	0x050
//RDIMM General Configuration Register 1
#define	PUB_RDIMMGCR1	0x051
//RDIMM General Configuration Register 2
#define	PUB_RDIMMGCR2	0x052
//RDIMM Control Register 0
#define	PUB_RDIMMCR0	0x054
//RDIMM Control Register 1
#define	PUB_RDIMMCR1	0x055
//RDIMM Control Register 2
#define	PUB_RDIMMCR2	0x056
//RDIMM Control Register 3
#define	PUB_RDIMMCR3	0x057
//RDIMM Control Register 4
#define	PUB_RDIMMCR4	0x058
//Scheduler Command Register 0
#define	PUB_SCHCR0	0x05A
//Scheduler Command Register 1
#define	PUB_SCHCR1	0x05B
//Mode Register 0
#define	PUB_MR0	0x060
//Mode Register 1
#define	PUB_MR1	0x061
//Mode Register 2
#define	PUB_MR2	0x062
//Mode Register 3
#define	PUB_MR3	0x063
//Mode Register 4
#define	PUB_MR4	0x064
//Mode Register 5
#define	PUB_MR5	0x065
//Mode Register 6
#define	PUB_MR6	0x066
//Mode Register 7
#define	PUB_MR7	0x067
//Mode Register 11
#define	PUB_MR11	0x06B
//Data Training Configuration Register 0
#define	PUB_DTCR0	0x080
//Data Training Configuration Register 1
#define	PUB_DTCR1	0x081
//Data Training Address Register 0
#define	PUB_DTAR0	0x082
//Data Training Address Register 1
#define	PUB_DTAR1	0x083
//Data Training Address Register 2
#define	PUB_DTAR2	0x084
//Data Training Data Register 0
#define	PUB_DTDR0	0x086
//Data Training Data Register 1
#define	PUB_DTDR1	0x087
//User Defined Data Register 0
#define	PUB_UDDR0	0x088
//User Defined Data Register 1
#define	PUB_UDDR1	0x089
//Data Training Eye Data Register 0
#define	PUB_DTEDR0	0x08C
//Data Training Eye Data Register 1
#define	PUB_DTEDR1	0x08D
//Data Training Eye Data Register 2
#define	PUB_DTEDR2	0x08C
//VREF Training Data Register
#define	PUB_VTDR	0x08F
//Command Address Training Register 0
#define	PUB_CATR0	0x090
//Command Address Training Register 1
#define	PUB_CATR1	0x091
//DQS Drift Register 0
#define	PUB_DQSDR0	0x094
//DQS Drift Register 1
#define	PUB_DQSDR1	0x095
//DQS Drift Register 2
#define	PUB_DQSDR2	0x096
//DCU Address Register
#define	PUB_DCUAR	0x0C0
//DCU Data Register
#define	PUB_DCUDR	0x0C1
//DCU Run Register
#define	PUB_DCURR	0x0C2
//DCU Loop Register
#define	PUB_DCULR	0x0C3
//DCU Generation Configuration Register
#define	PUB_DCUGCR	0x0C4
//DCU Timing Parameter Register
#define	PUB_DCUTPR	0x0C5
//DCU Status Register 0
#define	PUB_DCUSR0	0x0C6
//DCU Status Register 1
#define	PUB_DCUSR1	0x0C7
//BIST Run Register
#define	PUB_BISTRR	0x100
//BIST Word Count Register
#define	PUB_BISTWCR	0x101
//BIST Mask Register 0
#define	PUB_BISTMSKR0	0x102
//BIST Mask Register 1
#define	PUB_BISTMSKR1	0x103
//BIST Mask Register 2
#define	PUB_BISTMSKR2	0x104
//BIST LFSR Seed Register
#define	PUB_BISTLSR	0x105
//BIST Address Register 0
#define	PUB_BISTAR0	0x106
//BIST Address Register 1
#define	PUB_BISTAR1	0x107
//BIST Address Register 2
#define	PUB_BISTAR2	0x108
//BIST Address Register 3
#define	PUB_BISTAR3	0x109
//BIST Address Register 4
#define	PUB_BISTAR4	0x10A
//BIST User Data Pattern Register
#define	PUB_BISTUDPR	0x10B
//BIST General Status Register
#define	PUB_BISTGSR	0x10C
//BIST Word Error Register 0
#define	PUB_BISTWER0	0x10D
//BIST Word Error Register 1
#define	PUB_BISTWER1	0x10E
//BIST Bit Error Register 0
#define	PUB_BISTBER0	0x10F
//BIST Bit Error Register 1
#define	PUB_BISTBER1	0x110
//BIST Bit Error Register 2
#define	PUB_BISTBER2	0x111
//BIST Bit Error Register 3
#define	PUB_BISTBER3	0x112
//BIST Bit Error Register 4
#define	PUB_BISTBER4	0x113
//BIST Word Count Status Register
#define	PUB_BISTWCSR	0x114
//BIST Fail Word Register 0
#define	PUB_BISTFWER0	0x115
//BIST Fail Word Register 1
#define	PUB_BISTFWER1	0x116
//BIST Fail Word Register 2
#define	PUB_BISTFWER2	0x117
//BIST Bit Error Register 5
#define	PUB_BISTBER5	0x118
//Rank ID Register
#define	PUB_RANKIDR	0x137
//Rank I/O Configuration Register 0
#define	PUB_RIOCR0	0x138
//Rank I/O Configuration Register 1
#define	PUB_RIOCR1	0x139
//Rank I/O Configuration Register 2
#define	PUB_RIOCR2	0x13A
//Rank I/O Configuration Register 3
#define	PUB_RIOCR3	0x13B
//Rank I/O Configuration Register 4
#define	PUB_RIOCR4	0x13C
//Rank I/O Configuration Register 5
#define	PUB_RIOCR5	0x13D
//AC I/O Configuration Register 0
#define	PUB_ACIOCR0	0x140
//AC I/O Configuration Register 1
#define	PUB_ACIOCR1	0x141
//AC I/O Configuration Register 2
#define	PUB_ACIOCR2	0x142
//AC I/O Configuration Register 3
#define	PUB_ACIOCR3	0x143
//AC I/O Configuration Register 4
#define	PUB_ACIOCR4	0x144
//I/O VREF Control Register 0
#define	PUB_IOVCR0	0x148
//I/O VREF Control Register 1
#define	PUB_IOVCR1	0x149
//VREF Training Control Register 0
#define	PUB_VTCR0	0x14A
//VREF Training Control Register 1
#define	PUB_VTCR1	0x14B
//AC Bit Delay Line Register 0
#define	PUB_ACBDLR0	0x150
//AC Bit Delay Line Register 1
#define	PUB_ACBDLR1	0x151
//AC Bit Delay Line Register 2
#define	PUB_ACBDLR2	0x152
//AC Bit Delay Line Register 3
#define	PUB_ACBDLR3	0x153
//AC Bit Delay Line Register 4
#define	PUB_ACBDLR4	0x154
//AC Bit Delay Line Register 5
#define	PUB_ACBDLR5	0x155
//AC Bit Delay Line Register 6
#define	PUB_ACBDLR6	0x156
//AC Bit Delay Line Register 7
#define	PUB_ACBDLR7	0x157
//AC Bit Delay Line Register 8
#define	PUB_ACBDLR8	0x158
//AC Bit Delay Line Register 9
#define	PUB_ACBDLR9	0x159
//AC Bit Delay Line Register 10
#define	PUB_ACBDLR10	0x15A
//AC Bit Delay Line Register 11
#define	PUB_ACBDLR11	0x15B
//AC Bit Delay Line Register 12
#define	PUB_ACBDLR12	0x15C
//AC Bit Delay Line Register 13
#define	PUB_ACBDLR13	0x15D
//AC Bit Delay Line Register 14
#define	PUB_ACBDLR14	0x15E
//AC Local Calibrated Delay Line Register
#define	PUB_ACLCDLR	0x160
//AC Master Calibrated Delay Line Register 0
#define	PUB_ACMDLR0	0x168
//AC Master Calibrated Delay Line Register 1
#define	PUB_ACMDLR1	0x169
//ZQ Impedance Control Register
#define	PUB_ZQCR	0x1A0
//ZQ Segment 0 Impedance Control Program Register
#define	PUB_ZQ0PR	0x1A1
//ZQ Segment 0 Impedance Control Data Register
#define	PUB_ZQ0DR	0x1A2
//ZQ Segment 0 Impedance Control Status Register
#define	PUB_ZQ0SR	0x1A3
//ZQ Segment 1 Impedance Control Program Register
#define	PUB_ZQ1PR	0x1A5
//ZQ Segment 1 Impedance Control Data Register
#define	PUB_ZQ1DR	0x1A6
//ZQ Segment 1 Impedance Control Status Register
#define	PUB_ZQ1SR	0x1A7
//ZQ Segment 2 Impedance Control Program Register
#define	PUB_ZQ2PR	0x1A9
//ZQ Segment 2 Impedance Control Data Register
#define	PUB_ZQ2DR	0x1AA
//ZQ Segment 2 Impedance Control Status Register
#define	PUB_ZQ2SR	0x1AB
//ZQ Segment 3 Impedance Control Program Register
#define	PUB_ZQ3PR	0x1AD
//ZQ Segment 3 Impedance Control Data Register
#define	PUB_ZQ3DR	0x1AE
//ZQ Segment 3 Impedance Control Status Register
#define	PUB_ZQ3SR	0x1AF
//DATX8 Byte Lane 0 General Configuration Register 0
#define	PUB_DX0GCR0	0x1C0
//DATX8 Byte Lane 0 General Configuration Register 1
#define	PUB_DX0GCR1	0x1C1
//DATX8 Byte Lane 0 General Configuration Register 2
#define	PUB_DX0GCR2	0x1C2
//DATX8 Byte Lane 0 General Configuration Register 3
#define	PUB_DX0GCR3	0x1C3
//DATX8 Byte Lane 0 General Configuration Register 4
#define	PUB_DX0GCR4	0x1C4
//DATX8 Byte Lane 0 General Configuration Register 5
#define	PUB_DX0GCR5	0x1C5
//DATX8 Byte Lane 0 General Configuration Register 6
#define	PUB_DX0GCR6	0x1C6
//DATX8 Byte Lane 0 General Configuration Register 7
#define	PUB_DX0GCR7	0x1C7
//DATX8 Byte Lane 0 General Configuration Register 8
#define	PUB_DX0GCR8	0x1C8
//DATX8 Byte Lane 0 General Configuration Register 9
#define	PUB_DX0GCR9	0x1C9
//DATX8 Byte Lane 0 Bit Delay Line Register 0
#define	PUB_DX0BDLR0	0x1D0
//DATX8 Byte Lane 0 Bit Delay Line Register 1
#define	PUB_DX0BDLR1	0x1D1
//DATX8 Byte Lane 0 Bit Delay Line Register 2
#define	PUB_DX0BDLR2	0x1D2
//DATX8 Byte Lane 0 Bit Delay Line Register 3
#define	PUB_DX0BDLR3	0x1D4
//DATX8 Byte Lane 0 Bit Delay Line Register 4
#define	PUB_DX0BDLR4	0x1D5
//DATX8 Byte Lane 0 Bit Delay Line Register 5
#define	PUB_DX0BDLR5	0x1D6
//DATX8 Byte Lane 0 Bit Delay Line Register 6
#define	PUB_DX0BDLR6	0x1D8
//DATX8 Byte Lane 0 Bit Delay Line Register 7
#define	PUB_DX0BDLR7	0x1D9
//DATX8 Byte Lane 0 Bit Delay Line Register 8
#define	PUB_DX0BDLR8	0x1DA
//DATX8 Byte Lane 0 Bit Delay Line Register 9
#define	PUB_DX0BDLR9	0x1DB
//DATX8 Byte Lane 0 Local Calibrated Delay Line Register 0
#define	PUB_DX0LCDLR0	0x1E0
//DATX8 Byte Lane 0 Local Calibrated Delay Line Register 1
#define	PUB_DX0LCDLR1	0x1E1
//DATX8 Byte Lane 0 Local Calibrated Delay Line Register 2
#define	PUB_DX0LCDLR2	0x1E2
//DATX8 Byte Lane 0 Local Calibrated Delay Line Register 3
#define	PUB_DX0LCDLR3	0x1E3
//DATX8 Byte Lane 0 Local Calibrated Delay Line Register 4
#define	PUB_DX0LCDLR4	0x1E4
//DATX8 Byte Lane 0 Local Calibrated Delay Line Register 5
#define	PUB_DX0LCDLR5	0x1E5
//DATX8 Byte Lane 0 Master Delay Line Register 0
#define	PUB_DX0MDLR0	0x1E8
//DATX8 Byte Lane 0 Master Delay Line Register 1
#define	PUB_DX0MDLR1	0x1E9
//DATX8 Byte Lane 0 General Timing Register 0
#define	PUB_DX0GTR0	0x1F0
//DATX8 Byte Lane 0 General Timing Register 1
#define	PUB_DX0GTR1	0x1F1
//DATX8 Byte Lane 0 General Timing Register 2
#define	PUB_DX0GTR2	0x1F2
//DATX8 Byte Lane 0 General Timing Register 3
#define	PUB_DX0GTR3	0x1F3
//DATX8 Byte Lane 0 Rank Status Register 0
#define	PUB_DX0RSR0	0x1F4
//DATX8 Byte Lane 0 Rank Status Register 1
#define	PUB_DX0RSR1	0x1F5
//DATX8 Byte Lane 0 Rank Status Register 2
#define	PUB_DX0RSR2	0x1F6
//DATX8 Byte Lane 0 Rank Status Register 3
#define	PUB_DX0RSR3	0x1F7
//DATX8 Byte Lane 0 General Status Register 0
#define	PUB_DX0GSR0	0x1F8
//DATX8 Byte Lane 0 General Status Register 1
#define	PUB_DX0GSR1	0x1F9
//DATX8 Byte Lane 0 General Status Register 2
#define	PUB_DX0GSR2	0x1FA
//DATX8 Byte Lane 0 General Status Register 3
#define	PUB_DX0GSR3	0x1FB
//DATX8 Byte Lane 0 General Status Register 4
#define	PUB_DX0GSR4	0x1FC
//DATX8 Byte Lane 0 General Status Register 5
#define	PUB_DX0GSR5	0x1FD
//DATX8 Byte Lane 0 General Status Register 6
#define	PUB_DX0GSR6	0x1FE
//DATX8 Byte Lane 1 General Configuration Register 0
#define	PUB_DX1GCR0	0x200
//DATX8 Byte Lane 1 General Configuration Register 1
#define	PUB_DX1GCR1	0x201
//DATX8 Byte Lane 1 General Configuration Register 2
#define	PUB_DX1GCR2	0x202
//DATX8 Byte Lane 1 General Configuration Register 3
#define	PUB_DX1GCR3	0x203
//DATX8 Byte Lane 1 General Configuration Register 4
#define	PUB_DX1GCR4	0x204
//DATX8 Byte Lane 1 General Configuration Register 5
#define	PUB_DX1GCR5	0x205
//DATX8 Byte Lane 1 General Configuration Register 6
#define	PUB_DX1GCR6	0x206
//DATX8 Byte Lane 1 General Configuration Register 7
#define	PUB_DX1GCR7	0x207
//DATX8 Byte Lane 1 General Configuration Register 8
#define	PUB_DX1GCR8	0x208
//DATX8 Byte Lane 1 General Configuration Register 9
#define	PUB_DX1GCR9	0x209
//DATX8 Byte Lane 1 Bit Delay Line Register 0
#define	PUB_DX1BDLR0	0x210
//DATX8 Byte Lane 1 Bit Delay Line Register 1
#define	PUB_DX1BDLR1	0x211
//DATX8 Byte Lane 1 Bit Delay Line Register 2
#define	PUB_DX1BDLR2	0x212
//DATX8 Byte Lane 1 Bit Delay Line Register 3
#define	PUB_DX1BDLR3	0x214
//DATX8 Byte Lane 1 Bit Delay Line Register 4
#define	PUB_DX1BDLR4	0x215
//DATX8 Byte Lane 1 Bit Delay Line Register 5
#define	PUB_DX1BDLR5	0x216
//DATX8 Byte Lane 1 Bit Delay Line Register 6
#define	PUB_DX1BDLR6	0x218
//DATX8 Byte Lane 1 Bit Delay Line Register 7
#define	PUB_DX1BDLR7	0x219
//DATX8 Byte Lane 1 Bit Delay Line Register 8
#define	PUB_DX1BDLR8	0x21A
//DATX8 Byte Lane 1 Bit Delay Line Register 9
#define	PUB_DX1BDLR9	0x21B
//DATX8 Byte Lane 1 Local Calibrated Delay Line Register 0
#define	PUB_DX1LCDLR0	0x220
//DATX8 Byte Lane 1 Local Calibrated Delay Line Register 1
#define	PUB_DX1LCDLR1	0x221
//DATX8 Byte Lane 1 Local Calibrated Delay Line Register 2
#define	PUB_DX1LCDLR2	0x222
//DATX8 Byte Lane 1 Local Calibrated Delay Line Register 3
#define	PUB_DX1LCDLR3	0x223
//DATX8 Byte Lane 1 Local Calibrated Delay Line Register 4
#define	PUB_DX1LCDLR4	0x224
//DATX8 Byte Lane 1 Local Calibrated Delay Line Register 5
#define	PUB_DX1LCDLR5	0x225
//DATX8 Byte Lane 1 Master Delay Line Register 0
#define	PUB_DX1MDLR0	0x228
//DATX8 Byte Lane 1 Master Delay Line Register 1
#define	PUB_DX1MDLR1	0x229
//DATX8 Byte Lane 1 General Timing Register 0
#define	PUB_DX1GTR0	0x230
//DATX8 Byte Lane 1 General Timing Register 1
#define	PUB_DX1GTR1	0x231
//DATX8 Byte Lane 1 General Timing Register 2
#define	PUB_DX1GTR2	0x232
//DATX8 Byte Lane 1 General Timing Register 3
#define	PUB_DX1GTR3	0x233
//DATX8 Byte Lane 1 Rank Status Register 0
#define	PUB_DX1RSR0	0x234
//DATX8 Byte Lane 1 Rank Status Register 1
#define	PUB_DX1RSR1	0x235
//DATX8 Byte Lane 1 Rank Status Register 2
#define	PUB_DX1RSR2	0x236
//DATX8 Byte Lane 1 Rank Status Register 3
#define	PUB_DX1RSR3	0x237
//DATX8 Byte Lane 1 General Status Register 0
#define	PUB_DX1GSR0	0x238
//DATX8 Byte Lane 1 General Status Register 1
#define	PUB_DX1GSR1	0x239
//DATX8 Byte Lane 1 General Status Register 2
#define	PUB_DX1GSR2	0x23A
//DATX8 Byte Lane 1 General Status Register 3
#define	PUB_DX1GSR3	0x23B
//DATX8 Byte Lane 1 General Status Register 4
#define	PUB_DX1GSR4	0x23C
//DATX8 Byte Lane 1 General Status Register 5
#define	PUB_DX1GSR5	0x23D
//DATX8 Byte Lane 1 General Status Register 6
#define	PUB_DX1GSR6	0x23E
//DATX8 Byte Lane 2 General Configuration Register 0
#define	PUB_DX2GCR0	0x240
//DATX8 Byte Lane 2 General Configuration Register 1
#define	PUB_DX2GCR1	0x241
//DATX8 Byte Lane 2 General Configuration Register 2
#define	PUB_DX2GCR2	0x242
//DATX8 Byte Lane 2 General Configuration Register 3
#define	PUB_DX2GCR3	0x243
//DATX8 Byte Lane 2 General Configuration Register 4
#define	PUB_DX2GCR4	0x244
//DATX8 Byte Lane 2 General Configuration Register 5
#define	PUB_DX2GCR5	0x245
//DATX8 Byte Lane 2 General Configuration Register 6
#define	PUB_DX2GCR6	0x246
//DATX8 Byte Lane 2 General Configuration Register 7
#define	PUB_DX2GCR7	0x247
//DATX8 Byte Lane 2 General Configuration Register 8
#define	PUB_DX2GCR8	0x248
//DATX8 Byte Lane 2 General Configuration Register 9
#define	PUB_DX2GCR9	0x249
//DATX8 Byte Lane 2 Bit Delay Line Register 0
#define	PUB_DX2BDLR0	0x250
//DATX8 Byte Lane 2 Bit Delay Line Register 1
#define	PUB_DX2BDLR1	0x251
//DATX8 Byte Lane 2 Bit Delay Line Register 2
#define	PUB_DX2BDLR2	0x252
//DATX8 Byte Lane 2 Bit Delay Line Register 3
#define	PUB_DX2BDLR3	0x254
//DATX8 Byte Lane 2 Bit Delay Line Register 4
#define	PUB_DX2BDLR4	0x255
//DATX8 Byte Lane 2 Bit Delay Line Register 5
#define	PUB_DX2BDLR5	0x256
//DATX8 Byte Lane 2 Bit Delay Line Register 6
#define	PUB_DX2BDLR6	0x258
//DATX8 Byte Lane 2 Bit Delay Line Register 7
#define	PUB_DX2BDLR7	0x259
//DATX8 Byte Lane 2 Bit Delay Line Register 8
#define	PUB_DX2BDLR8	0x25A
//DATX8 Byte Lane 2 Bit Delay Line Register 9
#define	PUB_DX2BDLR9	0x25B
//DATX8 Byte Lane 2 Local Calibrated Delay Line Register 0
#define	PUB_DX2LCDLR0	0x260
//DATX8 Byte Lane 2 Local Calibrated Delay Line Register 1
#define	PUB_DX2LCDLR1	0x261
//DATX8 Byte Lane 2 Local Calibrated Delay Line Register 2
#define	PUB_DX2LCDLR2	0x262
//DATX8 Byte Lane 2 Local Calibrated Delay Line Register 3
#define	PUB_DX2LCDLR3	0x263
//DATX8 Byte Lane 2 Local Calibrated Delay Line Register 4
#define	PUB_DX2LCDLR4	0x264
//DATX8 Byte Lane 2 Local Calibrated Delay Line Register 5
#define	PUB_DX2LCDLR5	0x265
//DATX8 Byte Lane 2 Master Delay Line Register 0
#define	PUB_DX2MDLR0	0x268
//DATX8 Byte Lane 2 Master Delay Line Register 1
#define	PUB_DX2MDLR1	0x269
//DATX8 Byte Lane 2 General Timing Register 0
#define	PUB_DX2GTR0	0x270
//DATX8 Byte Lane 2 General Timing Register 1
#define	PUB_DX2GTR1	0x271
//DATX8 Byte Lane 2 General Timing Register 2
#define	PUB_DX2GTR2	0x272
//DATX8 Byte Lane 2 General Timing Register 3
#define	PUB_DX2GTR3	0x273
//DATX8 Byte Lane 2 Rank Status Register 0
#define	PUB_DX2RSR0	0x274
//DATX8 Byte Lane 2 Rank Status Register 1
#define	PUB_DX2RSR1	0x275
//DATX8 Byte Lane 2 Rank Status Register 2
#define	PUB_DX2RSR2	0x276
//DATX8 Byte Lane 2 Rank Status Register 3
#define	PUB_DX2RSR3	0x277
//DATX8 Byte Lane 2 General Status Register 0
#define	PUB_DX2GSR0	0x278
//DATX8 Byte Lane 2 General Status Register 1
#define	PUB_DX2GSR1	0x279
//DATX8 Byte Lane 2 General Status Register 2
#define	PUB_DX2GSR2	0x27A
//DATX8 Byte Lane 2 General Status Register 3
#define	PUB_DX2GSR3	0x27B
//DATX8 Byte Lane 2 General Status Register 4
#define	PUB_DX2GSR4	0x27C
//DATX8 Byte Lane 2 General Status Register 5
#define	PUB_DX2GSR5	0x27D
//DATX8 Byte Lane 2 General Status Register 6
#define	PUB_DX2GSR6	0x27E
//DATX8 Byte Lane 3 General Configuration Register 0
#define	PUB_DX3GCR0	0x280
//DATX8 Byte Lane 3 General Configuration Register 1
#define	PUB_DX3GCR1	0x281
//DATX8 Byte Lane 3 General Configuration Register 2
#define	PUB_DX3GCR2	0x282
//DATX8 Byte Lane 3 General Configuration Register 3
#define	PUB_DX3GCR3	0x283
//DATX8 Byte Lane 3 General Configuration Register 4
#define	PUB_DX3GCR4	0x284
//DATX8 Byte Lane 3 General Configuration Register 5
#define	PUB_DX3GCR5	0x285
//DATX8 Byte Lane 3 General Configuration Register 6
#define	PUB_DX3GCR6	0x286
//DATX8 Byte Lane 3 General Configuration Register 7
#define	PUB_DX3GCR7	0x287
//DATX8 Byte Lane 3 General Configuration Register 8
#define	PUB_DX3GCR8	0x288
//DATX8 Byte Lane 3 General Configuration Register 9
#define	PUB_DX3GCR9	0x289
//DATX8 Byte Lane 3 Bit Delay Line Register 0
#define	PUB_DX3BDLR0	0x290
//DATX8 Byte Lane 3 Bit Delay Line Register 1
#define	PUB_DX3BDLR1	0x291
//DATX8 Byte Lane 3 Bit Delay Line Register 2
#define	PUB_DX3BDLR2	0x292
//DATX8 Byte Lane 3 Bit Delay Line Register 3
#define	PUB_DX3BDLR3	0x294
//DATX8 Byte Lane 3 Bit Delay Line Register 4
#define	PUB_DX3BDLR4	0x295
//DATX8 Byte Lane 3 Bit Delay Line Register 5
#define	PUB_DX3BDLR5	0x296
//DATX8 Byte Lane 3 Bit Delay Line Register 6
#define	PUB_DX3BDLR6	0x298
//DATX8 Byte Lane 3 Bit Delay Line Register 7
#define	PUB_DX3BDLR7	0x299
//DATX8 Byte Lane 3 Bit Delay Line Register 8
#define	PUB_DX3BDLR8	0x29A
//DATX8 Byte Lane 3 Bit Delay Line Register 9
#define	PUB_DX3BDLR9	0x29B
//DATX8 Byte Lane 3 Local Calibrated Delay Line Register 0
#define	PUB_DX3LCDLR0	0x2A0
//DATX8 Byte Lane 3 Local Calibrated Delay Line Register 1
#define	PUB_DX3LCDLR1	0x2A1
//DATX8 Byte Lane 3 Local Calibrated Delay Line Register 2
#define	PUB_DX3LCDLR2	0x2A2
//DATX8 Byte Lane 3 Local Calibrated Delay Line Register 3
#define	PUB_DX3LCDLR3	0x2A3
//DATX8 Byte Lane 3 Local Calibrated Delay Line Register 4
#define	PUB_DX3LCDLR4	0x2A4
//DATX8 Byte Lane 3 Local Calibrated Delay Line Register 5
#define	PUB_DX3LCDLR5	0x2A5
//DATX8 Byte Lane 3 Master Delay Line Register 0
#define	PUB_DX3MDLR0	0x2A8
//DATX8 Byte Lane 3 Master Delay Line Register 1
#define	PUB_DX3MDLR1	0x2A9
//DATX8 Byte Lane 3 General Timing Register 0
#define	PUB_DX3GTR0	0x2B0
//DATX8 Byte Lane 3 General Timing Register 1
#define	PUB_DX3GTR1	0x2B1
//DATX8 Byte Lane 3 General Timing Register 2
#define	PUB_DX3GTR2	0x2B2
//DATX8 Byte Lane 3 General Timing Register 3
#define	PUB_DX3GTR3	0x2B3
//DATX8 Byte Lane 3 Rank Status Register 0
#define	PUB_DX3RSR0	0x2B4
//DATX8 Byte Lane 3 Rank Status Register 1
#define	PUB_DX3RSR1	0x2B5
//DATX8 Byte Lane 3 Rank Status Register 2
#define	PUB_DX3RSR2	0x2B6
//DATX8 Byte Lane 3 Rank Status Register 3
#define	PUB_DX3RSR3	0x2B7
//DATX8 Byte Lane 3 General Status Register 0
#define	PUB_DX3GSR0	0x2B8
//DATX8 Byte Lane 3 General Status Register 1
#define	PUB_DX3GSR1	0x2B9
//DATX8 Byte Lane 3 General Status Register 2
#define	PUB_DX3GSR2	0x2BA
//DATX8 Byte Lane 3 General Status Register 3
#define	PUB_DX3GSR3	0x2BB
//DATX8 Byte Lane 3 General Status Register 4
#define	PUB_DX3GSR4	0x2BC
//DATX8 Byte Lane 3 General Status Register 5
#define	PUB_DX3GSR5	0x2BD
//DATX8 Byte Lane 3 General Status Register 6
#define	PUB_DX3GSR6	0x2BE
//DATX8 Byte Lane 4 General Configuration Register 0
#define	PUB_DX4GCR0	0x2C0
//DATX8 Byte Lane 4 General Configuration Register 1
#define	PUB_DX4GCR1	0x2C1
//DATX8 Byte Lane 4 General Configuration Register 2
#define	PUB_DX4GCR2	0x2C2
//DATX8 Byte Lane 4 General Configuration Register 3
#define	PUB_DX4GCR3	0x2C3
//DATX8 Byte Lane 4 General Configuration Register 4
#define	PUB_DX4GCR4	0x2C4
//DATX8 Byte Lane 4 General Configuration Register 5
#define	PUB_DX4GCR5	0x2C5
//DATX8 Byte Lane 4 General Configuration Register 6
#define	PUB_DX4GCR6	0x2C6
//DATX8 Byte Lane 4 General Configuration Register 7
#define	PUB_DX4GCR7	0x2C7
//DATX8 Byte Lane 4 General Configuration Register 8
#define	PUB_DX4GCR8	0x2C8
//DATX8 Byte Lane 4 General Configuration Register 9
#define	PUB_DX4GCR9	0x2C9
//DATX8 Byte Lane 4 Bit Delay Line Register 0
#define	PUB_DX4BDLR0	0x2D0
//DATX8 Byte Lane 4 Bit Delay Line Register 1
#define	PUB_DX4BDLR1	0x2D1
//DATX8 Byte Lane 4 Bit Delay Line Register 2
#define	PUB_DX4BDLR2	0x2D2
//DATX8 Byte Lane 4 Bit Delay Line Register 3
#define	PUB_DX4BDLR3	0x2D4
//DATX8 Byte Lane 4 Bit Delay Line Register 4
#define	PUB_DX4BDLR4	0x2D5
//DATX8 Byte Lane 4 Bit Delay Line Register 5
#define	PUB_DX4BDLR5	0x2D6
//DATX8 Byte Lane 4 Bit Delay Line Register 6
#define	PUB_DX4BDLR6	0x2D8
//DATX8 Byte Lane 4 Bit Delay Line Register 7
#define	PUB_DX4BDLR7	0x2D9
//DATX8 Byte Lane 4 Bit Delay Line Register 8
#define	PUB_DX4BDLR8	0x2DA
//DATX8 Byte Lane 4 Bit Delay Line Register 9
#define	PUB_DX4BDLR9	0x2DB
//DATX8 Byte Lane 4 Local Calibrated Delay Line Register 0
#define	PUB_DX4LCDLR0	0x2E0
//DATX8 Byte Lane 4 Local Calibrated Delay Line Register 1
#define	PUB_DX4LCDLR1	0x2E1
//DATX8 Byte Lane 4 Local Calibrated Delay Line Register 2
#define	PUB_DX4LCDLR2	0x2E2
//DATX8 Byte Lane 4 Local Calibrated Delay Line Register 3
#define	PUB_DX4LCDLR3	0x2E3
//DATX8 Byte Lane 4 Local Calibrated Delay Line Register 4
#define	PUB_DX4LCDLR4	0x2E4
//DATX8 Byte Lane 4 Local Calibrated Delay Line Register 5
#define	PUB_DX4LCDLR5	0x2E5
//DATX8 Byte Lane 4 Master Delay Line Register 0
#define	PUB_DX4MDLR0	0x2E8
//DATX8 Byte Lane 4 Master Delay Line Register 1
#define	PUB_DX4MDLR1	0x2E9
//DATX8 Byte Lane 4 General Timing Register 0
#define	PUB_DX4GTR0	0x2F0
//DATX8 Byte Lane 4 General Timing Register 1
#define	PUB_DX4GTR1	0x2F1
//DATX8 Byte Lane 4 General Timing Register 2
#define	PUB_DX4GTR2	0x2F2
//DATX8 Byte Lane 4 General Timing Register 3
#define	PUB_DX4GTR3	0x2F3
//DATX8 Byte Lane 4 Rank Status Register 0
#define	PUB_DX4RSR0	0x2F4
//DATX8 Byte Lane 4 Rank Status Register 1
#define	PUB_DX4RSR1	0x2F5
//DATX8 Byte Lane 4 Rank Status Register 2
#define	PUB_DX4RSR2	0x2F6
//DATX8 Byte Lane 4 Rank Status Register 3
#define	PUB_DX4RSR3	0x2F7
//DATX8 Byte Lane 4 General Status Register 0
#define	PUB_DX4GSR0	0x2F8
//DATX8 Byte Lane 4 General Status Register 1
#define	PUB_DX4GSR1	0x2F9
//DATX8 Byte Lane 4 General Status Register 2
#define	PUB_DX4GSR2	0x2FA
//DATX8 Byte Lane 4 General Status Register 3
#define	PUB_DX4GSR3	0x2FB
//DATX8 Byte Lane 4 General Status Register 4
#define	PUB_DX4GSR4	0x2FC
//DATX8 Byte Lane 4 General Status Register 5
#define	PUB_DX4GSR5	0x2FD
//DATX8 Byte Lane 4 General Status Register 6
#define	PUB_DX4GSR6	0x2FE
//DATX8 Byte Lane 5 General Configuration Register 0
#define	PUB_DX5GCR0	0x300
//DATX8 Byte Lane 5 General Configuration Register 1
#define	PUB_DX5GCR1	0x301
//DATX8 Byte Lane 5 General Configuration Register 2
#define	PUB_DX5GCR2	0x302
//DATX8 Byte Lane 5 General Configuration Register 3
#define	PUB_DX5GCR3	0x303
//DATX8 Byte Lane 5 General Configuration Register 4
#define	PUB_DX5GCR4	0x304
//DATX8 Byte Lane 5 General Configuration Register 5
#define	PUB_DX5GCR5	0x305
//DATX8 Byte Lane 5 General Configuration Register 6
#define	PUB_DX5GCR6	0x306
//DATX8 Byte Lane 5 General Configuration Register 7
#define	PUB_DX5GCR7	0x307
//DATX8 Byte Lane 5 General Configuration Register 8
#define	PUB_DX5GCR8	0x308
//DATX8 Byte Lane 5 General Configuration Register 9
#define	PUB_DX5GCR9	0x309
//DATX8 Byte Lane 5 Bit Delay Line Register 0
#define	PUB_DX5BDLR0	0x310
//DATX8 Byte Lane 5 Bit Delay Line Register 1
#define	PUB_DX5BDLR1	0x311
//DATX8 Byte Lane 5 Bit Delay Line Register 2
#define	PUB_DX5BDLR2	0x312
//DATX8 Byte Lane 5 Bit Delay Line Register 3
#define	PUB_DX5BDLR3	0x314
//DATX8 Byte Lane 5 Bit Delay Line Register 4
#define	PUB_DX5BDLR4	0x315
//DATX8 Byte Lane 5 Bit Delay Line Register 5
#define	PUB_DX5BDLR5	0x316
//DATX8 Byte Lane 5 Bit Delay Line Register 6
#define	PUB_DX5BDLR6	0x318
//DATX8 Byte Lane 5 Bit Delay Line Register 7
#define	PUB_DX5BDLR7	0x319
//DATX8 Byte Lane 5 Bit Delay Line Register 8
#define	PUB_DX5BDLR8	0x31A
//DATX8 Byte Lane 5 Bit Delay Line Register 9
#define	PUB_DX5BDLR9	0x31B
//DATX8 Byte Lane 5 Local Calibrated Delay Line Register 0
#define	PUB_DX5LCDLR0	0x320
//DATX8 Byte Lane 5 Local Calibrated Delay Line Register 1
#define	PUB_DX5LCDLR1	0x321
//DATX8 Byte Lane 5 Local Calibrated Delay Line Register 2
#define	PUB_DX5LCDLR2	0x322
//DATX8 Byte Lane 5 Local Calibrated Delay Line Register 3
#define	PUB_DX5LCDLR3	0x323
//DATX8 Byte Lane 5 Local Calibrated Delay Line Register 4
#define	PUB_DX5LCDLR4	0x324
//DATX8 Byte Lane 5 Local Calibrated Delay Line Register 5
#define	PUB_DX5LCDLR5	0x325
//DATX8 Byte Lane 5 Master Delay Line Register 0
#define	PUB_DX5MDLR0	0x328
//DATX8 Byte Lane 5 Master Delay Line Register 1
#define	PUB_DX5MDLR1	0x329
//DATX8 Byte Lane 5 General Timing Register 0
#define	PUB_DX5GTR0	0x330
//DATX8 Byte Lane 5 General Timing Register 1
#define	PUB_DX5GTR1	0x331
//DATX8 Byte Lane 5 General Timing Register 2
#define	PUB_DX5GTR2	0x332
//DATX8 Byte Lane 5 General Timing Register 3
#define	PUB_DX5GTR3	0x333
//DATX8 Byte Lane 5 Rank Status Register 0
#define	PUB_DX5RSR0	0x334
//DATX8 Byte Lane 5 Rank Status Register 1
#define	PUB_DX5RSR1	0x335
//DATX8 Byte Lane 5 Rank Status Register 2
#define	PUB_DX5RSR2	0x336
//DATX8 Byte Lane 5 Rank Status Register 3
#define	PUB_DX5RSR3	0x337
//DATX8 Byte Lane 5 General Status Register 0
#define	PUB_DX5GSR0	0x338
//DATX8 Byte Lane 5 General Status Register 1
#define	PUB_DX5GSR1	0x339
//DATX8 Byte Lane 5 General Status Register 2
#define	PUB_DX5GSR2	0x33A
//DATX8 Byte Lane 5 General Status Register 3
#define	PUB_DX5GSR3	0x33B
//DATX8 Byte Lane 5 General Status Register 4
#define	PUB_DX5GSR4	0x33C
//DATX8 Byte Lane 5 General Status Register 5
#define	PUB_DX5GSR5	0x33D
//DATX8 Byte Lane 5 General Status Register 6
#define	PUB_DX5GSR6	0x33E
//DATX8 Byte Lane 6 General Configuration Register 0
#define	PUB_DX6GCR0	0x340
//DATX8 Byte Lane 6 General Configuration Register 1
#define	PUB_DX6GCR1	0x341
//DATX8 Byte Lane 6 General Configuration Register 2
#define	PUB_DX6GCR2	0x342
//DATX8 Byte Lane 6 General Configuration Register 3
#define	PUB_DX6GCR3	0x343
//DATX8 Byte Lane 6 General Configuration Register 4
#define	PUB_DX6GCR4	0x344
//DATX8 Byte Lane 6 General Configuration Register 5
#define	PUB_DX6GCR5	0x345
//DATX8 Byte Lane 6 General Configuration Register 6
#define	PUB_DX6GCR6	0x346
//DATX8 Byte Lane 6 General Configuration Register 7
#define	PUB_DX6GCR7	0x347
//DATX8 Byte Lane 6 General Configuration Register 8
#define	PUB_DX6GCR8	0x348
//DATX8 Byte Lane 6 General Configuration Register 9
#define	PUB_DX6GCR9	0x349
//DATX8 Byte Lane 6 Bit Delay Line Register 0
#define	PUB_DX6BDLR0	0x350
//DATX8 Byte Lane 6 Bit Delay Line Register 1
#define	PUB_DX6BDLR1	0x351
//DATX8 Byte Lane 6 Bit Delay Line Register 2
#define	PUB_DX6BDLR2	0x352
//DATX8 Byte Lane 6 Bit Delay Line Register 3
#define	PUB_DX6BDLR3	0x354
//DATX8 Byte Lane 6 Bit Delay Line Register 4
#define	PUB_DX6BDLR4	0x355
//DATX8 Byte Lane 6 Bit Delay Line Register 5
#define	PUB_DX6BDLR5	0x356
//DATX8 Byte Lane 6 Bit Delay Line Register 6
#define	PUB_DX6BDLR6	0x358
//DATX8 Byte Lane 6 Bit Delay Line Register 7
#define	PUB_DX6BDLR7	0x359
//DATX8 Byte Lane 6 Bit Delay Line Register 8
#define	PUB_DX6BDLR8	0x35A
//DATX8 Byte Lane 6 Bit Delay Line Register 9
#define	PUB_DX6BDLR9	0x35B
//DATX8 Byte Lane 6 Local Calibrated Delay Line Register 0
#define	PUB_DX6LCDLR0	0x360
//DATX8 Byte Lane 6 Local Calibrated Delay Line Register 1
#define	PUB_DX6LCDLR1	0x361
//DATX8 Byte Lane 6 Local Calibrated Delay Line Register 2
#define	PUB_DX6LCDLR2	0x362
//DATX8 Byte Lane 6 Local Calibrated Delay Line Register 3
#define	PUB_DX6LCDLR3	0x363
//DATX8 Byte Lane 6 Local Calibrated Delay Line Register 4
#define	PUB_DX6LCDLR4	0x364
//DATX8 Byte Lane 6 Local Calibrated Delay Line Register 5
#define	PUB_DX6LCDLR5	0x365
//DATX8 Byte Lane 6 Master Delay Line Register 0
#define	PUB_DX6MDLR0	0x368
//DATX8 Byte Lane 6 Master Delay Line Register 1
#define	PUB_DX6MDLR1	0x369
//DATX8 Byte Lane 6 General Timing Register 0
#define	PUB_DX6GTR0	0x370
//DATX8 Byte Lane 6 General Timing Register 1
#define	PUB_DX6GTR1	0x371
//DATX8 Byte Lane 6 General Timing Register 2
#define	PUB_DX6GTR2	0x372
//DATX8 Byte Lane 6 General Timing Register 3
#define	PUB_DX6GTR3	0x373
//DATX8 Byte Lane 6 Rank Status Register 0
#define	PUB_DX6RSR0	0x374
//DATX8 Byte Lane 6 Rank Status Register 1
#define	PUB_DX6RSR1	0x375
//DATX8 Byte Lane 6 Rank Status Register 2
#define	PUB_DX6RSR2	0x376
//DATX8 Byte Lane 6 Rank Status Register 3
#define	PUB_DX6RSR3	0x377
//DATX8 Byte Lane 6 General Status Register 0
#define	PUB_DX6GSR0	0x378
//DATX8 Byte Lane 6 General Status Register 1
#define	PUB_DX6GSR1	0x379
//DATX8 Byte Lane 6 General Status Register 2
#define	PUB_DX6GSR2	0x37A
//DATX8 Byte Lane 6 General Status Register 3
#define	PUB_DX6GSR3	0x37B
//DATX8 Byte Lane 6 General Status Register 4
#define	PUB_DX6GSR4	0x37C
//DATX8 Byte Lane 6 General Status Register 5
#define	PUB_DX6GSR5	0x37D
//DATX8 Byte Lane 6 General Status Register 6
#define	PUB_DX6GSR6	0x37E
//DATX8 Byte Lane 7 General Configuration Register 0
#define	PUB_DX7GCR0	0x380
//DATX8 Byte Lane 7 General Configuration Register 1
#define	PUB_DX7GCR1	0x381
//DATX8 Byte Lane 7 General Configuration Register 2
#define	PUB_DX7GCR2	0x382
//DATX8 Byte Lane 7 General Configuration Register 3
#define	PUB_DX7GCR3	0x383
//DATX8 Byte Lane 7 General Configuration Register 4
#define	PUB_DX7GCR4	0x384
//DATX8 Byte Lane 7 General Configuration Register 5
#define	PUB_DX7GCR5	0x385
//DATX8 Byte Lane 7 General Configuration Register 6
#define	PUB_DX7GCR6	0x386
//DATX8 Byte Lane 7 General Configuration Register 7
#define	PUB_DX7GCR7	0x387
//DATX8 Byte Lane 7 General Configuration Register 8
#define	PUB_DX7GCR8	0x388
//DATX8 Byte Lane 7 General Configuration Register 9
#define	PUB_DX7GCR9	0x389
//DATX8 Byte Lane 7 Bit Delay Line Register 0
#define	PUB_DX7BDLR0	0x390
//DATX8 Byte Lane 7 Bit Delay Line Register 1
#define	PUB_DX7BDLR1	0x391
//DATX8 Byte Lane 7 Bit Delay Line Register 2
#define	PUB_DX7BDLR2	0x392
//DATX8 Byte Lane 7 Bit Delay Line Register 3
#define	PUB_DX7BDLR3	0x394
//DATX8 Byte Lane 7 Bit Delay Line Register 4
#define	PUB_DX7BDLR4	0x395
//DATX8 Byte Lane 7 Bit Delay Line Register 5
#define	PUB_DX7BDLR5	0x396
//DATX8 Byte Lane 7 Bit Delay Line Register 6
#define	PUB_DX7BDLR6	0x398
//DATX8 Byte Lane 7 Bit Delay Line Register 7
#define	PUB_DX7BDLR7	0x399
//DATX8 Byte Lane 7 Bit Delay Line Register 8
#define	PUB_DX7BDLR8	0x39A
//DATX8 Byte Lane 7 Bit Delay Line Register 9
#define	PUB_DX7BDLR9	0x39B
//DATX8 Byte Lane 7 Local Calibrated Delay Line Register 0
#define	PUB_DX7LCDLR0	0x3A0
//DATX8 Byte Lane 7 Local Calibrated Delay Line Register 1
#define	PUB_DX7LCDLR1	0x3A1
//DATX8 Byte Lane 7 Local Calibrated Delay Line Register 2
#define	PUB_DX7LCDLR2	0x3A2
//DATX8 Byte Lane 7 Local Calibrated Delay Line Register 3
#define	PUB_DX7LCDLR3	0x3A3
//DATX8 Byte Lane 7 Local Calibrated Delay Line Register 4
#define	PUB_DX7LCDLR4	0x3A4
//DATX8 Byte Lane 7 Local Calibrated Delay Line Register 5
#define	PUB_DX7LCDLR5	0x3A5
//DATX8 Byte Lane 7 Master Delay Line Register 0
#define	PUB_DX7MDLR0	0x3A8
//DATX8 Byte Lane 7 Master Delay Line Register 1
#define	PUB_DX7MDLR1	0x3A9
//DATX8 Byte Lane 7 General Timing Register 0
#define	PUB_DX7GTR0	0x3B0
//DATX8 Byte Lane 7 General Timing Register 1
#define	PUB_DX7GTR1	0x3B1
//DATX8 Byte Lane 7 General Timing Register 2
#define	PUB_DX7GTR2	0x3B2
//DATX8 Byte Lane 7 General Timing Register 3
#define	PUB_DX7GTR3	0x3B3
//DATX8 Byte Lane 7 Rank Status Register 0
#define	PUB_DX7RSR0	0x3B4
//DATX8 Byte Lane 7 Rank Status Register 1
#define	PUB_DX7RSR1	0x3B5
//DATX8 Byte Lane 7 Rank Status Register 2
#define	PUB_DX7RSR2	0x3B6
//DATX8 Byte Lane 7 Rank Status Register 3
#define	PUB_DX7RSR3	0x3B7
//DATX8 Byte Lane 7 General Status Register 0
#define	PUB_DX7GSR0	0x3B8
//DATX8 Byte Lane 7 General Status Register 1
#define	PUB_DX7GSR1	0x3B9
//DATX8 Byte Lane 7 General Status Register 2
#define	PUB_DX7GSR2	0x3BA
//DATX8 Byte Lane 7 General Status Register 3
#define	PUB_DX7GSR3	0x3BB
//DATX8 Byte Lane 7 General Status Register 4
#define	PUB_DX7GSR4	0x3BC
//DATX8 Byte Lane 7 General Status Register 5
#define	PUB_DX7GSR5	0x3BD
//DATX8 Byte Lane 7 General Status Register 6
#define	PUB_DX7GSR6	0x3BE
//DATX8 Byte Lane 8 General Configuration Register 0
#define	PUB_DX8GCR0	0x3C0
//DATX8 Byte Lane 8 General Configuration Register 1
#define	PUB_DX8GCR1	0x3C1
//DATX8 Byte Lane 8 General Configuration Register 2
#define	PUB_DX8GCR2	0x3C2
//DATX8 Byte Lane 8 General Configuration Register 3
#define	PUB_DX8GCR3	0x3C3
//DATX8 Byte Lane 8 General Configuration Register 4
#define	PUB_DX8GCR4	0x3C4
//DATX8 Byte Lane 8 General Configuration Register 5
#define	PUB_DX8GCR5	0x3C5
//DATX8 Byte Lane 8 General Configuration Register 6
#define	PUB_DX8GCR6	0x3C6
//DATX8 Byte Lane 8 General Configuration Register 7
#define	PUB_DX8GCR7	0x3C7
//DATX8 Byte Lane 8 General Configuration Register 8
#define	PUB_DX8GCR8	0x3C8
//DATX8 Byte Lane 8 General Configuration Register 9
#define	PUB_DX8GCR9	0x3C9
//DATX8 Byte Lane 8 Bit Delay Line Register 0
#define	PUB_DX8BDLR0	0x3D0
//DATX8 Byte Lane 8 Bit Delay Line Register 1
#define	PUB_DX8BDLR1	0x3D1
//DATX8 Byte Lane 8 Bit Delay Line Register 2
#define	PUB_DX8BDLR2	0x3D2
//DATX8 Byte Lane 8 Bit Delay Line Register 3
#define	PUB_DX8BDLR3	0x3D4
//DATX8 Byte Lane 8 Bit Delay Line Register 4
#define	PUB_DX8BDLR4	0x3D5
//DATX8 Byte Lane 8 Bit Delay Line Register 5
#define	PUB_DX8BDLR5	0x3D6
//DATX8 Byte Lane 8 Bit Delay Line Register 6
#define	PUB_DX8BDLR6	0x3D8
//DATX8 Byte Lane 8 Bit Delay Line Register 7
#define	PUB_DX8BDLR7	0x3D9
//DATX8 Byte Lane 8 Bit Delay Line Register 8
#define	PUB_DX8BDLR8	0x3DA
//DATX8 Byte Lane 8 Bit Delay Line Register 9
#define	PUB_DX8BDLR9	0x3DB
//DATX8 Byte Lane 8 Local Calibrated Delay Line Register 0
#define	PUB_DX8LCDLR0	0x3E0
//DATX8 Byte Lane 8 Local Calibrated Delay Line Register 1
#define	PUB_DX8LCDLR1	0x3E1
//DATX8 Byte Lane 8 Local Calibrated Delay Line Register 2
#define	PUB_DX8LCDLR2	0x3E2
//DATX8 Byte Lane 8 Local Calibrated Delay Line Register 3
#define	PUB_DX8LCDLR3	0x3E3
//DATX8 Byte Lane 8 Local Calibrated Delay Line Register 4
#define	PUB_DX8LCDLR4	0x3E4
//DATX8 Byte Lane 8 Local Calibrated Delay Line Register 5
#define	PUB_DX8LCDLR5	0x3E5
//DATX8 Byte Lane 8 Master Delay Line Register 0
#define	PUB_DX8MDLR0	0x3E8
//DATX8 Byte Lane 8 Master Delay Line Register 1
#define	PUB_DX8MDLR1	0x3E9
//DATX8 Byte Lane 8 General Timing Register 0
#define	PUB_DX8GTR0	0x3F0
//DATX8 Byte Lane 8 General Timing Register 1
#define	PUB_DX8GTR1	0x3F1
//DATX8 Byte Lane 8 General Timing Register 2
#define	PUB_DX8GTR2	0x3F2
//DATX8 Byte Lane 8 General Timing Register 3
#define	PUB_DX8GTR3	0x3F3
//DATX8 Byte Lane 8 Rank Status Register 0
#define	PUB_DX8RSR0	0x3F4
//DATX8 Byte Lane 8 Rank Status Register 1
#define	PUB_DX8RSR1	0x3F5
//DATX8 Byte Lane 8 Rank Status Register 2
#define	PUB_DX8RSR2	0x3F6
//DATX8 Byte Lane 8 Rank Status Register 3
#define	PUB_DX8RSR3	0x3F7
//DATX8 Byte Lane 8 General Status Register 0
#define	PUB_DX8GSR0	0x3F8
//DATX8 Byte Lane 8 General Status Register 1
#define	PUB_DX8GSR1	0x3F9
//DATX8 Byte Lane 8 General Status Register 2
#define	PUB_DX8GSR2	0x3FA
//DATX8 Byte Lane 8 General Status Register 3
#define	PUB_DX8GSR3	0x3FB
//DATX8 Byte Lane 8 General Status Register 4
#define	PUB_DX8GSR4	0x3FC
//DATX8 Byte Lane 8 General Status Register 5
#define	PUB_DX8GSR5	0x3FD
//DATX8 Byte Lane 8 General Status Register 6
#define	PUB_DX8GSR6	0x3FE


#endif /* !defined(__DOXYGEN__) */

#endif /* !defined(__HW_PUB_DEF_H__) */
