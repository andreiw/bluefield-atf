#ifndef __REGS_EMI_DEF_H__
#define __REGS_EMI_DEF_H__

#ifdef __ASSEMBLER__
#define _64bit(x) x
#else /* __ASSEMBLER__ */
#ifdef __tile__
#define _64bit(x) x ## UL
#else /* __tile__ */
#define _64bit(x) x ## ULL
#endif /* __tile__ */
#endif /* __ASSEMBLER */



#ifndef __DOXYGEN__

/*
 * Device Info.
 * This register provides general information about the device attached to
 * this port and channel.
 */
#define EMI_DEV_INFO 0x0000
#define EMI_DEV_INFO__LENGTH 0x0001

#define EMI_DEV_INFO__TYPE_SHIFT 0
#define EMI_DEV_INFO__TYPE_WIDTH 12
#define EMI_DEV_INFO__TYPE_RESET_VAL 0
#define EMI_DEV_INFO__TYPE_RMASK 0xfff
#define EMI_DEV_INFO__TYPE_MASK  0xfff
#define EMI_DEV_INFO__TYPE_FIELD 0,11
#define EMI_DEV_INFO__TYPE_VAL_PCIE 0x1
#define EMI_DEV_INFO__TYPE_VAL_GBE 0x10
#define EMI_DEV_INFO__TYPE_VAL_XGBE 0x11
#define EMI_DEV_INFO__TYPE_VAL_MPIPE 0x13
#define EMI_DEV_INFO__TYPE_VAL_TRIO 0x14
#define EMI_DEV_INFO__TYPE_VAL_CRYPTO 0x16
#define EMI_DEV_INFO__TYPE_VAL_COMPRESSION 0x18
#define EMI_DEV_INFO__TYPE_VAL_GPIO 0x20
#define EMI_DEV_INFO__TYPE_VAL_RSHIM 0x21
#define EMI_DEV_INFO__TYPE_VAL_SROM 0x22
#define EMI_DEV_INFO__TYPE_VAL_I2CM 0x25
#define EMI_DEV_INFO__TYPE_VAL_I2CS 0x26
#define EMI_DEV_INFO__TYPE_VAL_UART 0x28
#define EMI_DEV_INFO__TYPE_VAL_USBH 0x29
#define EMI_DEV_INFO__TYPE_VAL_USBS 0x2a
#define EMI_DEV_INFO__TYPE_VAL_USBHS 0x2b
#define EMI_DEV_INFO__TYPE_VAL_MMC 0x2c
#define EMI_DEV_INFO__TYPE_VAL_DDR2 0x40
#define EMI_DEV_INFO__TYPE_VAL_DDR3 0x42
#define EMI_DEV_INFO__TYPE_VAL_DDR4 0x44
#define EMI_DEV_INFO__TYPE_VAL_HMC 0x45
#define EMI_DEV_INFO__TYPE_VAL_MSS 0x46
#define EMI_DEV_INFO__TYPE_VAL_DIAG_SNP 0x80
#define EMI_DEV_INFO__TYPE_VAL_IPIC 0x81
#define EMI_DEV_INFO__TYPE_VAL_TILE 0x82
#define EMI_DEV_INFO__TYPE_VAL_TM 0x83
#define EMI_DEV_INFO__TYPE_VAL_TCAM 0x84
#define EMI_DEV_INFO__TYPE_VAL_SMMU 0x85
#define EMI_DEV_INFO__TYPE_VAL_GIC 0x86
#define EMI_DEV_INFO__TYPE_VAL_MPIPE_TX 0x87

#define EMI_DEV_INFO__DEVICE_REV_SHIFT 16
#define EMI_DEV_INFO__DEVICE_REV_WIDTH 8
#define EMI_DEV_INFO__DEVICE_REV_RMASK 0xff
#define EMI_DEV_INFO__DEVICE_REV_MASK  0xff0000
#define EMI_DEV_INFO__DEVICE_REV_FIELD 16,23

#define EMI_DEV_INFO__REGISTER_REV_SHIFT 24
#define EMI_DEV_INFO__REGISTER_REV_WIDTH 4
#define EMI_DEV_INFO__REGISTER_REV_RESET_VAL 0
#define EMI_DEV_INFO__REGISTER_REV_RMASK 0xf
#define EMI_DEV_INFO__REGISTER_REV_MASK  0xf000000
#define EMI_DEV_INFO__REGISTER_REV_FIELD 24,27

#define EMI_DEV_INFO__INSTANCE_SHIFT 32
#define EMI_DEV_INFO__INSTANCE_WIDTH 4
#define EMI_DEV_INFO__INSTANCE_RMASK 0xf
#define EMI_DEV_INFO__INSTANCE_MASK  _64bit(0xf00000000)
#define EMI_DEV_INFO__INSTANCE_FIELD 32,35


/*
 * Device Control.
 * This register provides general device control.
 */
#define EMI_DEV_CTL 0x0008
#define EMI_DEV_CTL__LENGTH 0x0001

#define EMI_DEV_CTL__NDN_ROUTE_ORDER_SHIFT 0
#define EMI_DEV_CTL__NDN_ROUTE_ORDER_WIDTH 1
#define EMI_DEV_CTL__NDN_ROUTE_ORDER_RESET_VAL 0
#define EMI_DEV_CTL__NDN_ROUTE_ORDER_RMASK 0x1
#define EMI_DEV_CTL__NDN_ROUTE_ORDER_MASK  0x1
#define EMI_DEV_CTL__NDN_ROUTE_ORDER_FIELD 0,0

#define EMI_DEV_CTL__CDN_ROUTE_ORDER_SHIFT 1
#define EMI_DEV_CTL__CDN_ROUTE_ORDER_WIDTH 1
#define EMI_DEV_CTL__CDN_ROUTE_ORDER_RESET_VAL 1
#define EMI_DEV_CTL__CDN_ROUTE_ORDER_RMASK 0x1
#define EMI_DEV_CTL__CDN_ROUTE_ORDER_MASK  0x2
#define EMI_DEV_CTL__CDN_ROUTE_ORDER_FIELD 1,1

#define EMI_DEV_CTL__DDN_ROUTE_ORDER_SHIFT 2
#define EMI_DEV_CTL__DDN_ROUTE_ORDER_WIDTH 1
#define EMI_DEV_CTL__DDN_ROUTE_ORDER_RESET_VAL 1
#define EMI_DEV_CTL__DDN_ROUTE_ORDER_RMASK 0x1
#define EMI_DEV_CTL__DDN_ROUTE_ORDER_MASK  0x4
#define EMI_DEV_CTL__DDN_ROUTE_ORDER_FIELD 2,2

#define EMI_DEV_CTL__DMA_RD_CA_ENA_SHIFT 3
#define EMI_DEV_CTL__DMA_RD_CA_ENA_WIDTH 1
#define EMI_DEV_CTL__DMA_RD_CA_ENA_RESET_VAL 1
#define EMI_DEV_CTL__DMA_RD_CA_ENA_RMASK 0x1
#define EMI_DEV_CTL__DMA_RD_CA_ENA_MASK  0x8
#define EMI_DEV_CTL__DMA_RD_CA_ENA_FIELD 3,3

#define EMI_DEV_CTL__L3_PROFILE_OVD_SHIFT 4
#define EMI_DEV_CTL__L3_PROFILE_OVD_WIDTH 1
#define EMI_DEV_CTL__L3_PROFILE_OVD_RESET_VAL 0
#define EMI_DEV_CTL__L3_PROFILE_OVD_RMASK 0x1
#define EMI_DEV_CTL__L3_PROFILE_OVD_MASK  0x10
#define EMI_DEV_CTL__L3_PROFILE_OVD_FIELD 4,4

#define EMI_DEV_CTL__L3_PROFILE_VAL_SHIFT 5
#define EMI_DEV_CTL__L3_PROFILE_VAL_WIDTH 4
#define EMI_DEV_CTL__L3_PROFILE_VAL_RESET_VAL 0
#define EMI_DEV_CTL__L3_PROFILE_VAL_RMASK 0xf
#define EMI_DEV_CTL__L3_PROFILE_VAL_MASK  0x1e0
#define EMI_DEV_CTL__L3_PROFILE_VAL_FIELD 5,8

#define EMI_DEV_CTL__WR_SLVERR_MAP_SHIFT 9
#define EMI_DEV_CTL__WR_SLVERR_MAP_WIDTH 2
#define EMI_DEV_CTL__WR_SLVERR_MAP_RESET_VAL 2
#define EMI_DEV_CTL__WR_SLVERR_MAP_RMASK 0x3
#define EMI_DEV_CTL__WR_SLVERR_MAP_MASK  0x600
#define EMI_DEV_CTL__WR_SLVERR_MAP_FIELD 9,10
#define EMI_DEV_CTL__WR_SLVERR_MAP_VAL_OKAY 0x0
#define EMI_DEV_CTL__WR_SLVERR_MAP_VAL_DATAERROR 0x2
#define EMI_DEV_CTL__WR_SLVERR_MAP_VAL_NONDATAERROR 0x3

#define EMI_DEV_CTL__WR_DECERR_MAP_SHIFT 11
#define EMI_DEV_CTL__WR_DECERR_MAP_WIDTH 2
#define EMI_DEV_CTL__WR_DECERR_MAP_RESET_VAL 3
#define EMI_DEV_CTL__WR_DECERR_MAP_RMASK 0x3
#define EMI_DEV_CTL__WR_DECERR_MAP_MASK  0x1800
#define EMI_DEV_CTL__WR_DECERR_MAP_FIELD 11,12
#define EMI_DEV_CTL__WR_DECERR_MAP_VAL_OKAY 0x0
#define EMI_DEV_CTL__WR_DECERR_MAP_VAL_DATAERROR 0x2
#define EMI_DEV_CTL__WR_DECERR_MAP_VAL_NONDATAERROR 0x3

#define EMI_DEV_CTL__RD_SLVERR_MAP_SHIFT 13
#define EMI_DEV_CTL__RD_SLVERR_MAP_WIDTH 2
#define EMI_DEV_CTL__RD_SLVERR_MAP_RESET_VAL 2
#define EMI_DEV_CTL__RD_SLVERR_MAP_RMASK 0x3
#define EMI_DEV_CTL__RD_SLVERR_MAP_MASK  0x6000
#define EMI_DEV_CTL__RD_SLVERR_MAP_FIELD 13,14
#define EMI_DEV_CTL__RD_SLVERR_MAP_VAL_OKAY 0x0
#define EMI_DEV_CTL__RD_SLVERR_MAP_VAL_DATAERROR 0x2
#define EMI_DEV_CTL__RD_SLVERR_MAP_VAL_NONDATAERROR 0x3

#define EMI_DEV_CTL__RD_DECERR_MAP_SHIFT 15
#define EMI_DEV_CTL__RD_DECERR_MAP_WIDTH 2
#define EMI_DEV_CTL__RD_DECERR_MAP_RESET_VAL 3
#define EMI_DEV_CTL__RD_DECERR_MAP_RMASK 0x3
#define EMI_DEV_CTL__RD_DECERR_MAP_MASK  0x18000
#define EMI_DEV_CTL__RD_DECERR_MAP_FIELD 15,16
#define EMI_DEV_CTL__RD_DECERR_MAP_VAL_OKAY 0x0
#define EMI_DEV_CTL__RD_DECERR_MAP_VAL_DATAERROR 0x2
#define EMI_DEV_CTL__RD_DECERR_MAP_VAL_NONDATAERROR 0x3

#define EMI_DEV_CTL__CDN_REQ_BUF_ENA_SHIFT 17
#define EMI_DEV_CTL__CDN_REQ_BUF_ENA_WIDTH 1
#define EMI_DEV_CTL__CDN_REQ_BUF_ENA_RESET_VAL 1
#define EMI_DEV_CTL__CDN_REQ_BUF_ENA_RMASK 0x1
#define EMI_DEV_CTL__CDN_REQ_BUF_ENA_MASK  0x20000
#define EMI_DEV_CTL__CDN_REQ_BUF_ENA_FIELD 17,17

#define EMI_DEV_CTL__DMA_WRQ_HWM_SHIFT 20
#define EMI_DEV_CTL__DMA_WRQ_HWM_WIDTH 8
#define EMI_DEV_CTL__DMA_WRQ_HWM_RESET_VAL 255
#define EMI_DEV_CTL__DMA_WRQ_HWM_RMASK 0xff
#define EMI_DEV_CTL__DMA_WRQ_HWM_MASK  0xff00000
#define EMI_DEV_CTL__DMA_WRQ_HWM_FIELD 20,27

#define EMI_DEV_CTL__GTHR_DELAY_ADJ_SHIFT 28
#define EMI_DEV_CTL__GTHR_DELAY_ADJ_WIDTH 4
#define EMI_DEV_CTL__GTHR_DELAY_ADJ_RESET_VAL 0
#define EMI_DEV_CTL__GTHR_DELAY_ADJ_RMASK 0xf
#define EMI_DEV_CTL__GTHR_DELAY_ADJ_MASK  0xf0000000
#define EMI_DEV_CTL__GTHR_DELAY_ADJ_FIELD 28,31


/* Scratchpad. */
#define EMI_SCRATCHPAD 0x0020
#define EMI_SCRATCHPAD__LENGTH 0x0001

#define EMI_SCRATCHPAD__SCRATCHPAD_SHIFT 0
#define EMI_SCRATCHPAD__SCRATCHPAD_WIDTH 64
#define EMI_SCRATCHPAD__SCRATCHPAD_RESET_VAL 0
#define EMI_SCRATCHPAD__SCRATCHPAD_RMASK _64bit(0xffffffffffffffff)
#define EMI_SCRATCHPAD__SCRATCHPAD_MASK  _64bit(0xffffffffffffffff)
#define EMI_SCRATCHPAD__SCRATCHPAD_FIELD 0,63


/*
 * Semaphore0.
 * Semaphore
 */
#define EMI_SEMAPHORE0 0x0028
#define EMI_SEMAPHORE0__LENGTH 0x0001

#define EMI_SEMAPHORE0__VAL_SHIFT 0
#define EMI_SEMAPHORE0__VAL_WIDTH 1
#define EMI_SEMAPHORE0__VAL_RESET_VAL 0
#define EMI_SEMAPHORE0__VAL_RMASK 0x1
#define EMI_SEMAPHORE0__VAL_MASK  0x1
#define EMI_SEMAPHORE0__VAL_FIELD 0,0


/*
 * DMA Status.
 * DMA status information for debug. Unused for devices that do not have DMA.
 */
#define EMI_DMA_STATUS 0x0030
#define EMI_DMA_STATUS__LENGTH 0x0001

#define EMI_DMA_STATUS__RDQ_INFL_COUNT_SHIFT 0
#define EMI_DMA_STATUS__RDQ_INFL_COUNT_WIDTH 9
#define EMI_DMA_STATUS__RDQ_INFL_COUNT_RESET_VAL 0
#define EMI_DMA_STATUS__RDQ_INFL_COUNT_RMASK 0x1ff
#define EMI_DMA_STATUS__RDQ_INFL_COUNT_MASK  0x1ff
#define EMI_DMA_STATUS__RDQ_INFL_COUNT_FIELD 0,8

#define EMI_DMA_STATUS__WRQ_INFL_COUNT_SHIFT 9
#define EMI_DMA_STATUS__WRQ_INFL_COUNT_WIDTH 9
#define EMI_DMA_STATUS__WRQ_INFL_COUNT_RESET_VAL 0
#define EMI_DMA_STATUS__WRQ_INFL_COUNT_RMASK 0x1ff
#define EMI_DMA_STATUS__WRQ_INFL_COUNT_MASK  0x3fe00
#define EMI_DMA_STATUS__WRQ_INFL_COUNT_FIELD 9,17

#define EMI_DMA_STATUS__WRQ_DIAG_VEC_SHIFT 25
#define EMI_DMA_STATUS__WRQ_DIAG_VEC_WIDTH 39
#define EMI_DMA_STATUS__WRQ_DIAG_VEC_RESET_VAL 0
#define EMI_DMA_STATUS__WRQ_DIAG_VEC_RMASK _64bit(0x7fffffffff)
#define EMI_DMA_STATUS__WRQ_DIAG_VEC_MASK  _64bit(0xfffffffffe000000)
#define EMI_DMA_STATUS__WRQ_DIAG_VEC_FIELD 25,63


/* Clock Count. */
#define EMI_CLOCK_COUNT 0x0038
#define EMI_CLOCK_COUNT__LENGTH 0x0001

#define EMI_CLOCK_COUNT__RUN_SHIFT 0
#define EMI_CLOCK_COUNT__RUN_WIDTH 1
#define EMI_CLOCK_COUNT__RUN_RESET_VAL 0
#define EMI_CLOCK_COUNT__RUN_RMASK 0x1
#define EMI_CLOCK_COUNT__RUN_MASK  0x1
#define EMI_CLOCK_COUNT__RUN_FIELD 0,0

#define EMI_CLOCK_COUNT__COUNT_SHIFT 1
#define EMI_CLOCK_COUNT__COUNT_WIDTH 15
#define EMI_CLOCK_COUNT__COUNT_RMASK 0x7fff
#define EMI_CLOCK_COUNT__COUNT_MASK  0xfffe
#define EMI_CLOCK_COUNT__COUNT_FIELD 1,15


/*
 * Interrupt Setup.
 * Configuration for device interrupts.
 */
#define EMI_INT_SETUP 0x0040
#define EMI_INT_SETUP__LENGTH 0x0001

#define EMI_INT_SETUP__GBL_ENA_SHIFT 0
#define EMI_INT_SETUP__GBL_ENA_WIDTH 1
#define EMI_INT_SETUP__GBL_ENA_RESET_VAL 0
#define EMI_INT_SETUP__GBL_ENA_RMASK 0x1
#define EMI_INT_SETUP__GBL_ENA_MASK  0x1
#define EMI_INT_SETUP__GBL_ENA_FIELD 0,0

#define EMI_INT_SETUP__BASE_INT_NUM_SHIFT 48
#define EMI_INT_SETUP__BASE_INT_NUM_WIDTH 16
#define EMI_INT_SETUP__BASE_INT_NUM_RMASK 0xffff
#define EMI_INT_SETUP__BASE_INT_NUM_MASK  _64bit(0xffff000000000000)
#define EMI_INT_SETUP__BASE_INT_NUM_FIELD 48,63


/*
 * Feature Control.
 * Device-specific feature controls.
 */
#define EMI_FEATURE_CTL 0x0048
#define EMI_FEATURE_CTL__LENGTH 0x0001

#define EMI_FEATURE_CTL__FEATURE_CTL_SHIFT 0
#define EMI_FEATURE_CTL__FEATURE_CTL_WIDTH 64
#define EMI_FEATURE_CTL__FEATURE_CTL_RESET_VAL 12297829382473034410
#define EMI_FEATURE_CTL__FEATURE_CTL_RMASK _64bit(0xffffffffffffffff)
#define EMI_FEATURE_CTL__FEATURE_CTL_MASK  _64bit(0xffffffffffffffff)
#define EMI_FEATURE_CTL__FEATURE_CTL_FIELD 0,63


/*
 * Credit Control.
 * Provides access to the request-credit counters that control end-to-end
 * flow control between the device and other nodes in the system.
 */
#define EMI_CRED_CTL 0x0050
#define EMI_CRED_CTL__LENGTH 0x0001

#define EMI_CRED_CTL__UPDATE_SHIFT 0
#define EMI_CRED_CTL__UPDATE_WIDTH 1
#define EMI_CRED_CTL__UPDATE_RESET_VAL 0
#define EMI_CRED_CTL__UPDATE_RMASK 0x1
#define EMI_CRED_CTL__UPDATE_MASK  0x1
#define EMI_CRED_CTL__UPDATE_FIELD 0,0

#define EMI_CRED_CTL__NODEID_SEL_SHIFT 1
#define EMI_CRED_CTL__NODEID_SEL_WIDTH 11
#define EMI_CRED_CTL__NODEID_SEL_RESET_VAL 0
#define EMI_CRED_CTL__NODEID_SEL_RMASK 0x7ff
#define EMI_CRED_CTL__NODEID_SEL_MASK  0xffe
#define EMI_CRED_CTL__NODEID_SEL_FIELD 1,11

#define EMI_CRED_CTL__TGT_SEL_SHIFT 12
#define EMI_CRED_CTL__TGT_SEL_WIDTH 2
#define EMI_CRED_CTL__TGT_SEL_RESET_VAL 0
#define EMI_CRED_CTL__TGT_SEL_RMASK 0x3
#define EMI_CRED_CTL__TGT_SEL_MASK  0x3000
#define EMI_CRED_CTL__TGT_SEL_FIELD 12,13
#define EMI_CRED_CTL__TGT_SEL_VAL_TX_REQ 0x0
#define EMI_CRED_CTL__TGT_SEL_VAL_RETRYQ 0x1
#define EMI_CRED_CTL__TGT_SEL_VAL_RING 0x2

#define EMI_CRED_CTL__CLIENT_SEL_SHIFT 14
#define EMI_CRED_CTL__CLIENT_SEL_WIDTH 2
#define EMI_CRED_CTL__CLIENT_SEL_RESET_VAL 0
#define EMI_CRED_CTL__CLIENT_SEL_RMASK 0x3
#define EMI_CRED_CTL__CLIENT_SEL_MASK  0xc000
#define EMI_CRED_CTL__CLIENT_SEL_FIELD 14,15

#define EMI_CRED_CTL__VAL_SHIFT 16
#define EMI_CRED_CTL__VAL_WIDTH 16
#define EMI_CRED_CTL__VAL_RESET_VAL 0
#define EMI_CRED_CTL__VAL_RMASK 0xffff
#define EMI_CRED_CTL__VAL_MASK  0xffff0000
#define EMI_CRED_CTL__VAL_FIELD 16,31


/*
 * SAM Control.
 * Provides access to SAM initialization.
 */
#define EMI_SAM_CTL 0x0058
#define EMI_SAM_CTL__LENGTH 0x0001

#define EMI_SAM_CTL__UPDATE_SHIFT 0
#define EMI_SAM_CTL__UPDATE_WIDTH 1
#define EMI_SAM_CTL__UPDATE_RESET_VAL 0
#define EMI_SAM_CTL__UPDATE_RMASK 0x1
#define EMI_SAM_CTL__UPDATE_MASK  0x1
#define EMI_SAM_CTL__UPDATE_FIELD 0,0

#define EMI_SAM_CTL__IDX_SHIFT 1
#define EMI_SAM_CTL__IDX_WIDTH 12
#define EMI_SAM_CTL__IDX_RESET_VAL 0
#define EMI_SAM_CTL__IDX_RMASK 0xfff
#define EMI_SAM_CTL__IDX_MASK  0x1ffe
#define EMI_SAM_CTL__IDX_FIELD 1,12
#define EMI_SAM_CTL__IDX_VAL_HNF_SIZE 0x7f7
#define EMI_SAM_CTL__IDX_VAL_LDR_SIZE 0x20

#define EMI_SAM_CTL__TBL_SEL_SHIFT 13
#define EMI_SAM_CTL__TBL_SEL_WIDTH 3
#define EMI_SAM_CTL__TBL_SEL_RESET_VAL 0
#define EMI_SAM_CTL__TBL_SEL_RMASK 0x7
#define EMI_SAM_CTL__TBL_SEL_MASK  0xe000
#define EMI_SAM_CTL__TBL_SEL_FIELD 13,15
#define EMI_SAM_CTL__TBL_SEL_VAL_LDR 0x0
#define EMI_SAM_CTL__TBL_SEL_VAL_HNF 0x1
#define EMI_SAM_CTL__TBL_SEL_VAL_PCI_NPF 0x2
#define EMI_SAM_CTL__TBL_SEL_VAL_PCI_PF 0x3
#define EMI_SAM_CTL__TBL_SEL_VAL_PCI_CFG 0x4

#define EMI_SAM_CTL__CLIENT_SEL_SHIFT 16
#define EMI_SAM_CTL__CLIENT_SEL_WIDTH 1
#define EMI_SAM_CTL__CLIENT_SEL_RESET_VAL 0
#define EMI_SAM_CTL__CLIENT_SEL_RMASK 0x1
#define EMI_SAM_CTL__CLIENT_SEL_MASK  0x10000
#define EMI_SAM_CTL__CLIENT_SEL_FIELD 16,16

#define EMI_SAM_CTL__READ_SHIFT 17
#define EMI_SAM_CTL__READ_WIDTH 1
#define EMI_SAM_CTL__READ_RESET_VAL 0
#define EMI_SAM_CTL__READ_RMASK 0x1
#define EMI_SAM_CTL__READ_MASK  0x20000
#define EMI_SAM_CTL__READ_FIELD 17,17

#define EMI_SAM_CTL__VAL_SHIFT 18
#define EMI_SAM_CTL__VAL_WIDTH 34
#define EMI_SAM_CTL__VAL_RESET_VAL 0
#define EMI_SAM_CTL__VAL_RMASK _64bit(0x3ffffffff)
#define EMI_SAM_CTL__VAL_MASK  _64bit(0xffffffffc0000)
#define EMI_SAM_CTL__VAL_FIELD 18,51


/*
 * IND_DATA.
 * Indirect Data: Data to write to the indirect register/SRAM or data read
 * from the indirect data register/SRAM
 *
 * IND_DATA_0 - First part of data.
 *
 * Summary:
 * The indirect register mechanism is used for accessing SRAMs and indirect
 * registers. Indirect registers are 32-bits wide, internally divided into
 * multiple fields. However, they do not have an address directly mapped to
 * the host configuration memory space. Instead, they are accessed indirectly
 * through the indirect access interface registers. SRAMs are memory
 * structures consisting of several addressable SRAM lines. Each SRAM line
 * can be made up of several fields.
 *
 * EZcp APIs for indirect register.
 * -       EZapiPrm_WriteReg
 * -       EZapiPrm_ReadReg
 * -       EZapiPrm_RMWReg
 *
 * EZcp APIs for SRAMs.
 * -       EZapiPrm_WriteSRAM
 * -       EZapiPrm_ReadSRAM
 * -       EZapiPrm_RMWSRAM
 *
 * Read Sequence Indirect Registers:
 * 1.      Specify the indirect register address to read - IND_ADDR.
 * 2.      Specify the indirect register command - IND_CMD.
 * 3.      Check the IND_CMD status - IND_STS.
 * 4.      Read the requested data - IND_DATA.
 *
 * Write Sequence Indirect Registers:
 * 1.      Specify the data to write - IND_DATA.
 * 2.      Specify the indirect register address to write - IND_ADDR.
 * 3.      Specify the indirect register command - IND_CMD.
 * 4.      Check the IND_CMD status - IND_STS.
 *
 * Read Sequence SRAMs:
 * 1.      Specify the SRAM line to read - IND_ADDR.
 * 2.      Specify the indirect SRAM command and SRAM ID - IND_CMD.
 * 3.      Check the IND_CMD status - IND_STS.
 * 4.      Read the requested data - IND_DATA.
 *
 * Write Sequence SRAMs:
 * 1.      Specify the data to write - IND_DATA.
 * 2.      Specify the first SRAM line to write - IND_ADDR.
 * 3.      Specify the end SRAM line if relevant - IND_CMD_RANGE if field OP
 * = 0x4/0x5 in register IND_CMD.
 * 4.      Specify the SRAM fields to modify if relevant - IND_CMD_BITMAP if
 * field OP = 0x3/0x5 in register IND_CMD.
 * 5.      Specify the indirect register command and SRAM ID - IND_CMD.
 * 6.      Check the IND_CMD status - IND_STS.
 */
#define EMI_IND_DATA__FIRST_WORD 0x0000
#define EMI_IND_DATA__LAST_WORD 0x000f
#define EMI_IND_DATA__LENGTH 0x0010
#define EMI_IND_DATA__STRIDE 0x0001

#define EMI_IND_DATA__VAL_SHIFT 0
#define EMI_IND_DATA__VAL_WIDTH 32
#define EMI_IND_DATA__VAL_RESET_VAL 0
#define EMI_IND_DATA__VAL_RMASK 0xffffffff
#define EMI_IND_DATA__VAL_MASK  0xffffffff
#define EMI_IND_DATA__VAL_FIELD 0,31


/*
 * IND_CMD.
 * Indirect Command: Indirect register/SRAM command. The status is in
 * register IND_STS.
 */
#define EMI_IND_CMD 0x0010
#define EMI_IND_CMD__LENGTH 0x0001

#define EMI_IND_CMD__OP_SHIFT 0
#define EMI_IND_CMD__OP_WIDTH 8
#define EMI_IND_CMD__OP_RESET_VAL 0
#define EMI_IND_CMD__OP_RMASK 0xff
#define EMI_IND_CMD__OP_MASK  0xff
#define EMI_IND_CMD__OP_FIELD 0,7

#define EMI_IND_CMD__GROUP_SHIFT 8
#define EMI_IND_CMD__GROUP_WIDTH 8
#define EMI_IND_CMD__GROUP_RESET_VAL 0
#define EMI_IND_CMD__GROUP_RMASK 0xff
#define EMI_IND_CMD__GROUP_MASK  0xff00
#define EMI_IND_CMD__GROUP_FIELD 8,15

#define EMI_IND_CMD__INSTANCE_SHIFT 16
#define EMI_IND_CMD__INSTANCE_WIDTH 8
#define EMI_IND_CMD__INSTANCE_RESET_VAL 0
#define EMI_IND_CMD__INSTANCE_RMASK 0xff
#define EMI_IND_CMD__INSTANCE_MASK  0xff0000
#define EMI_IND_CMD__INSTANCE_FIELD 16,23

#define EMI_IND_CMD__ID_SHIFT 24
#define EMI_IND_CMD__ID_WIDTH 8
#define EMI_IND_CMD__ID_RESET_VAL 0
#define EMI_IND_CMD__ID_RMASK 0xff
#define EMI_IND_CMD__ID_MASK  0xff000000
#define EMI_IND_CMD__ID_FIELD 24,31


/*
 * IND_CMD_RANGE.
 * Indirect Command Range: The SRAM end-line address for SRAM write/modify
 * range.
 */
#define EMI_IND_CMD_RANGE 0x0011
#define EMI_IND_CMD_RANGE__LENGTH 0x0001

#define EMI_IND_CMD_RANGE__MEM_END_ADDR_SHIFT 0
#define EMI_IND_CMD_RANGE__MEM_END_ADDR_WIDTH 32
#define EMI_IND_CMD_RANGE__MEM_END_ADDR_RESET_VAL 0
#define EMI_IND_CMD_RANGE__MEM_END_ADDR_RMASK 0xffffffff
#define EMI_IND_CMD_RANGE__MEM_END_ADDR_MASK  0xffffffff
#define EMI_IND_CMD_RANGE__MEM_END_ADDR_FIELD 0,31


/*
 * IND_ADDR.
 * Indirect Address. Specifies the indirect register address or SRAM
 * line/start-line
 */
#define EMI_IND_ADDR 0x0014
#define EMI_IND_ADDR__LENGTH 0x0001

#define EMI_IND_ADDR__VAL_SHIFT 0
#define EMI_IND_ADDR__VAL_WIDTH 32
#define EMI_IND_ADDR__VAL_RESET_VAL 0
#define EMI_IND_ADDR__VAL_RMASK 0xffffffff
#define EMI_IND_ADDR__VAL_MASK  0xffffffff
#define EMI_IND_ADDR__VAL_FIELD 0,31


/*
 * IND_STS.
 * Indirect Status: Indicates the status of the IND_CMD
 */
#define EMI_IND_STS 0x0015
#define EMI_IND_STS__LENGTH 0x0001

#define EMI_IND_STS__RDY_SHIFT 0
#define EMI_IND_STS__RDY_WIDTH 1
#define EMI_IND_STS__RDY_RESET_VAL 1
#define EMI_IND_STS__RDY_RMASK 0x1
#define EMI_IND_STS__RDY_MASK  0x1
#define EMI_IND_STS__RDY_FIELD 0,0

#define EMI_IND_STS__SUCCESS_SHIFT 1
#define EMI_IND_STS__SUCCESS_WIDTH 1
#define EMI_IND_STS__SUCCESS_RESET_VAL 1
#define EMI_IND_STS__SUCCESS_RMASK 0x1
#define EMI_IND_STS__SUCCESS_MASK  0x2
#define EMI_IND_STS__SUCCESS_FIELD 1,1


/*
 * SPARE.
 * Spare register
 */
#define EMI_SPARE 0x0021
#define EMI_SPARE__LENGTH 0x0001

#define EMI_SPARE__VAL_SHIFT 0
#define EMI_SPARE__VAL_WIDTH 32
#define EMI_SPARE__VAL_RESET_VAL 0
#define EMI_SPARE__VAL_RMASK 0xffffffff
#define EMI_SPARE__VAL_MASK  0xffffffff
#define EMI_SPARE__VAL_FIELD 0,31


/*
 * ECC_SINGLE_ERROR_CNT.
 * EZcp API: EZapiPrm_readReg
 *
 * ECC Single Error Counter: Provides a total single error counter for all
 * SRAM memories that have ECC. Clear on read. No wrap around.
 *
 * Soft Errors
 *
 * Soft errors are a transient random "bit-flip" of memory cells. It is an
 * event that corrupts the value stored in a memory cell without damaging the
 * cell itself.
 *
 * Soft errors are caused by the following events:
 *
 * -       Alpha particles (helium nucleus of 2 protons +2 neutrons)
 * emissions from chip packaging. This has been largely eliminated by chip
 * manufacturers.
 * -       Cosmic rays (high-energy particle originating in outer space). At
 * ground level, cosmic radiation is about 95% neutrons and 5% protons. These
 * particles can cause soft errors directly; and they can also interact with
 * atomic nuclei to produce troublesome short-range heavy ions.
 *
 * SRAM and DRAM Soft Error Information Description
 *
 * -       All SRAMs and DRAMs have ECC SECDED (Single Error Correction
 * Double Error Detection). SRAMs can also have parity instead of ECC. The
 * ECC is using the Hamming code algorithm with one additional parity bit for
 * double error detection.
 * -       All SRAMs and DRAMs with ECC protection have single and double
 * error indication via interrupt.
 * -       All SRAMs and DRAMs with ECC protection have double and single
 * error injection.
 */
#define EMI_ECC_SINGLE_ERROR_CNT 0x0040
#define EMI_ECC_SINGLE_ERROR_CNT__LENGTH 0x0001

#define EMI_ECC_SINGLE_ERROR_CNT__VAL_SHIFT 0
#define EMI_ECC_SINGLE_ERROR_CNT__VAL_WIDTH 4
#define EMI_ECC_SINGLE_ERROR_CNT__VAL_RESET_VAL 0
#define EMI_ECC_SINGLE_ERROR_CNT__VAL_RMASK 0xf
#define EMI_ECC_SINGLE_ERROR_CNT__VAL_MASK  0xf
#define EMI_ECC_SINGLE_ERROR_CNT__VAL_FIELD 0,3


/*
 * ECC_DOUBLE_ERROR_CNT.
 * EZcp API: EZapiPrm_readReg
 *
 * ECC Double Error Counter: Provides a total double error counter for all
 * SRAM memories that have ECC. Clear on read. No wrap around.
 */
#define EMI_ECC_DOUBLE_ERROR_CNT 0x0041
#define EMI_ECC_DOUBLE_ERROR_CNT__LENGTH 0x0001

#define EMI_ECC_DOUBLE_ERROR_CNT__VAL_SHIFT 0
#define EMI_ECC_DOUBLE_ERROR_CNT__VAL_WIDTH 4
#define EMI_ECC_DOUBLE_ERROR_CNT__VAL_RESET_VAL 0
#define EMI_ECC_DOUBLE_ERROR_CNT__VAL_RMASK 0xf
#define EMI_ECC_DOUBLE_ERROR_CNT__VAL_MASK  0xf
#define EMI_ECC_DOUBLE_ERROR_CNT__VAL_FIELD 0,3


/*
 * ECC_ADDR_ERROR_SELECT.
 * EZcp API: EZapiPrm_SERCheckError
 *
 * ECC Address Error Select: Specifies the ECC ID of the SRAM (ECCID)
 * according to register ECC_SINGLE_ERROR / ECC_DOUBLE_ERROR for register
 * ECC_ADDR_ERROR_DATA.
 */
#define EMI_ECC_ADDR_ERROR_SELECT 0x0042
#define EMI_ECC_ADDR_ERROR_SELECT__LENGTH 0x0001

#define EMI_ECC_ADDR_ERROR_SELECT__ECCID_WDATA_SHIFT 0
#define EMI_ECC_ADDR_ERROR_SELECT__ECCID_WDATA_WIDTH 2
#define EMI_ECC_ADDR_ERROR_SELECT__ECCID_WDATA_RESET_VAL 0
#define EMI_ECC_ADDR_ERROR_SELECT__ECCID_WDATA_RMASK 0x3
#define EMI_ECC_ADDR_ERROR_SELECT__ECCID_WDATA_MASK  0x3
#define EMI_ECC_ADDR_ERROR_SELECT__ECCID_WDATA_FIELD 0,1

#define EMI_ECC_ADDR_ERROR_SELECT__INDEX_SHIFT 28
#define EMI_ECC_ADDR_ERROR_SELECT__INDEX_WIDTH 4
#define EMI_ECC_ADDR_ERROR_SELECT__INDEX_RESET_VAL 0
#define EMI_ECC_ADDR_ERROR_SELECT__INDEX_RMASK 0xf
#define EMI_ECC_ADDR_ERROR_SELECT__INDEX_MASK  0xf0000000
#define EMI_ECC_ADDR_ERROR_SELECT__INDEX_FIELD 28,31


/*
 * ECC_ADDR_ERROR_DATA_LOW.
 * EZcp API: EZapiPrm_SERCheckError
 *
 * ECC Address Error Data Low: Provides the ECC error address (line number)
 * for the SRAM ECC ID specified in register ECC_ADDR_ERROR_SELECT (even it
 * does not match field LAST_ERR in registers ECC_ID_FIRST_LAST_SERR/
 * ECC_ID_FIRST_LAST_DERR). Register ECC_LATCH_MODE_0 controls if first or
 * last ECC error is stored.
 */
#define EMI_ECC_ADDR_ERROR_DATA_LOW 0x0043
#define EMI_ECC_ADDR_ERROR_DATA_LOW__LENGTH 0x0001

#define EMI_ECC_ADDR_ERROR_DATA_LOW__ADDRESS_SHIFT 0
#define EMI_ECC_ADDR_ERROR_DATA_LOW__ADDRESS_WIDTH 16
#define EMI_ECC_ADDR_ERROR_DATA_LOW__ADDRESS_RESET_VAL 0
#define EMI_ECC_ADDR_ERROR_DATA_LOW__ADDRESS_RMASK 0xffff
#define EMI_ECC_ADDR_ERROR_DATA_LOW__ADDRESS_MASK  0xffff
#define EMI_ECC_ADDR_ERROR_DATA_LOW__ADDRESS_FIELD 0,15


/*
 * ECC_ADDR_ERROR_DATA_HIGH.
 * EZcp API: EZapiPrm_SERCheckError
 *
 * ECC Address Error Data High: Provides ECC error additional information for
 * the SRAM specified in register ECC_ADDR_ERROR_SELECT.
 */
#define EMI_ECC_ADDR_ERROR_DATA_HIGH 0x0044
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__LENGTH 0x0001

#define EMI_ECC_ADDR_ERROR_DATA_HIGH__SYNDROM_SHIFT 0
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__SYNDROM_WIDTH 16
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__SYNDROM_RESET_VAL 0
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__SYNDROM_RMASK 0xffff
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__SYNDROM_MASK  0xffff
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__SYNDROM_FIELD 0,15

#define EMI_ECC_ADDR_ERROR_DATA_HIGH__INDEX_SHIFT 24
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__INDEX_WIDTH 4
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__INDEX_RESET_VAL 0
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__INDEX_RMASK 0xf
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__INDEX_MASK  0xf000000
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__INDEX_FIELD 24,27

#define EMI_ECC_ADDR_ERROR_DATA_HIGH__MULTIPLE_ERRORS_SHIFT 29
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__MULTIPLE_ERRORS_WIDTH 1
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__MULTIPLE_ERRORS_RESET_VAL 0
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__MULTIPLE_ERRORS_RMASK 0x1
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__MULTIPLE_ERRORS_MASK  0x20000000
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__MULTIPLE_ERRORS_FIELD 29,29

#define EMI_ECC_ADDR_ERROR_DATA_HIGH__S_D_BIT_SHIFT 30
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__S_D_BIT_WIDTH 1
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__S_D_BIT_RESET_VAL 0
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__S_D_BIT_RMASK 0x1
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__S_D_BIT_MASK  0x40000000
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__S_D_BIT_FIELD 30,30

#define EMI_ECC_ADDR_ERROR_DATA_HIGH__DATA_READY_SHIFT 31
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__DATA_READY_WIDTH 1
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__DATA_READY_RESET_VAL 0
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__DATA_READY_RMASK 0x1
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__DATA_READY_MASK  0x80000000
#define EMI_ECC_ADDR_ERROR_DATA_HIGH__DATA_READY_FIELD 31,31


/*
 * SERR_INJ.
 * EZcp API: EZapiPrm_writeReg
 *
 * Single Error Injection: Injects a single error (flips one data bit) for
 * specified SRAM ECCID during a single write. ECCID is defined in
 * ECC_SINGLE_ERROR_0.
 */
#define EMI_SERR_INJ 0x0045
#define EMI_SERR_INJ__LENGTH 0x0001

#define EMI_SERR_INJ__ID_SHIFT 0
#define EMI_SERR_INJ__ID_WIDTH 2
#define EMI_SERR_INJ__ID_RESET_VAL 0
#define EMI_SERR_INJ__ID_RMASK 0x3
#define EMI_SERR_INJ__ID_MASK  0x3
#define EMI_SERR_INJ__ID_FIELD 0,1


/*
 * DERR_INJ.
 * EZcp API: EZapiPrm_writeReg
 *
 * Double Error Injection: Injects a double error (flips two data bits) for
 * specified memory ID during a single write. ID is defined in
 * ECC_DOUBLE_ERROR_0
 */
#define EMI_DERR_INJ 0x0046
#define EMI_DERR_INJ__LENGTH 0x0001

#define EMI_DERR_INJ__ID_SHIFT 0
#define EMI_DERR_INJ__ID_WIDTH 2
#define EMI_DERR_INJ__ID_RESET_VAL 0
#define EMI_DERR_INJ__ID_RMASK 0x3
#define EMI_DERR_INJ__ID_MASK  0x3
#define EMI_DERR_INJ__ID_FIELD 0,1


/*
 * ECC_ID_FIRST_LAST_SERR.
 * EZcp API: EZapiPrm_SERCheckError
 *
 * ECC ID First Last Single Error: Indicates the first and last ECC ID of the
 * SRAM (ECCID) for the block that had a single error (e.g. the block had
 * three ECC errors in three different SRAMs). This may be useful
 * understanding multi ECC error events in different memories
 */
#define EMI_ECC_ID_FIRST_LAST_SERR 0x0047
#define EMI_ECC_ID_FIRST_LAST_SERR__LENGTH 0x0001

#define EMI_ECC_ID_FIRST_LAST_SERR__FIRST_ERR_SHIFT 0
#define EMI_ECC_ID_FIRST_LAST_SERR__FIRST_ERR_WIDTH 8
#define EMI_ECC_ID_FIRST_LAST_SERR__FIRST_ERR_RESET_VAL 0
#define EMI_ECC_ID_FIRST_LAST_SERR__FIRST_ERR_RMASK 0xff
#define EMI_ECC_ID_FIRST_LAST_SERR__FIRST_ERR_MASK  0xff
#define EMI_ECC_ID_FIRST_LAST_SERR__FIRST_ERR_FIELD 0,7

#define EMI_ECC_ID_FIRST_LAST_SERR__LAST_ERR_SHIFT 16
#define EMI_ECC_ID_FIRST_LAST_SERR__LAST_ERR_WIDTH 8
#define EMI_ECC_ID_FIRST_LAST_SERR__LAST_ERR_RESET_VAL 0
#define EMI_ECC_ID_FIRST_LAST_SERR__LAST_ERR_RMASK 0xff
#define EMI_ECC_ID_FIRST_LAST_SERR__LAST_ERR_MASK  0xff0000
#define EMI_ECC_ID_FIRST_LAST_SERR__LAST_ERR_FIELD 16,23

#define EMI_ECC_ID_FIRST_LAST_SERR__MULTI_FIRST_SHIFT 30
#define EMI_ECC_ID_FIRST_LAST_SERR__MULTI_FIRST_WIDTH 1
#define EMI_ECC_ID_FIRST_LAST_SERR__MULTI_FIRST_RESET_VAL 0
#define EMI_ECC_ID_FIRST_LAST_SERR__MULTI_FIRST_RMASK 0x1
#define EMI_ECC_ID_FIRST_LAST_SERR__MULTI_FIRST_MASK  0x40000000
#define EMI_ECC_ID_FIRST_LAST_SERR__MULTI_FIRST_FIELD 30,30

#define EMI_ECC_ID_FIRST_LAST_SERR__MULTI_LAST_SHIFT 31
#define EMI_ECC_ID_FIRST_LAST_SERR__MULTI_LAST_WIDTH 1
#define EMI_ECC_ID_FIRST_LAST_SERR__MULTI_LAST_RESET_VAL 0
#define EMI_ECC_ID_FIRST_LAST_SERR__MULTI_LAST_RMASK 0x1
#define EMI_ECC_ID_FIRST_LAST_SERR__MULTI_LAST_MASK  0x80000000
#define EMI_ECC_ID_FIRST_LAST_SERR__MULTI_LAST_FIELD 31,31


/*
 * ECC_ID_FIRST_LAST_DERR.
 * EZcp API: EZapiPrm_SERCheckError
 *
 * ECC ID First Last Double Error: Indicates the first and last ECC ID of the
 * SRAM (ECCID) for the block that had a double error (e.g. the block had
 * three ECC errors in three different SRAMs).
 */
#define EMI_ECC_ID_FIRST_LAST_DERR 0x0048
#define EMI_ECC_ID_FIRST_LAST_DERR__LENGTH 0x0001

#define EMI_ECC_ID_FIRST_LAST_DERR__FIRST_ERR_SHIFT 0
#define EMI_ECC_ID_FIRST_LAST_DERR__FIRST_ERR_WIDTH 8
#define EMI_ECC_ID_FIRST_LAST_DERR__FIRST_ERR_RESET_VAL 0
#define EMI_ECC_ID_FIRST_LAST_DERR__FIRST_ERR_RMASK 0xff
#define EMI_ECC_ID_FIRST_LAST_DERR__FIRST_ERR_MASK  0xff
#define EMI_ECC_ID_FIRST_LAST_DERR__FIRST_ERR_FIELD 0,7

#define EMI_ECC_ID_FIRST_LAST_DERR__LAST_ERR_SHIFT 16
#define EMI_ECC_ID_FIRST_LAST_DERR__LAST_ERR_WIDTH 8
#define EMI_ECC_ID_FIRST_LAST_DERR__LAST_ERR_RESET_VAL 0
#define EMI_ECC_ID_FIRST_LAST_DERR__LAST_ERR_RMASK 0xff
#define EMI_ECC_ID_FIRST_LAST_DERR__LAST_ERR_MASK  0xff0000
#define EMI_ECC_ID_FIRST_LAST_DERR__LAST_ERR_FIELD 16,23

#define EMI_ECC_ID_FIRST_LAST_DERR__MULTI_FIRST_SHIFT 30
#define EMI_ECC_ID_FIRST_LAST_DERR__MULTI_FIRST_WIDTH 1
#define EMI_ECC_ID_FIRST_LAST_DERR__MULTI_FIRST_RESET_VAL 0
#define EMI_ECC_ID_FIRST_LAST_DERR__MULTI_FIRST_RMASK 0x1
#define EMI_ECC_ID_FIRST_LAST_DERR__MULTI_FIRST_MASK  0x40000000
#define EMI_ECC_ID_FIRST_LAST_DERR__MULTI_FIRST_FIELD 30,30

#define EMI_ECC_ID_FIRST_LAST_DERR__MULTI_LAST_SHIFT 31
#define EMI_ECC_ID_FIRST_LAST_DERR__MULTI_LAST_WIDTH 1
#define EMI_ECC_ID_FIRST_LAST_DERR__MULTI_LAST_RESET_VAL 0
#define EMI_ECC_ID_FIRST_LAST_DERR__MULTI_LAST_RMASK 0x1
#define EMI_ECC_ID_FIRST_LAST_DERR__MULTI_LAST_MASK  0x80000000
#define EMI_ECC_ID_FIRST_LAST_DERR__MULTI_LAST_FIELD 31,31


/*
 * ECC_SINGLE_ERROR_0.
 * EZcp API: EZapiPrm_SERCheckError
 *
 * ECC Single Error 0: Provides a single error host interrupt cause for all
 * SRAMs that have ECC or parity.
 * Bit Value:
 * 0 - SRAM has no error.
 * 1 - SRAM has an error.
 * Bit Description:
 * b0 - Name of ECCID 0 with ECC.
 */
#define EMI_ECC_SINGLE_ERROR_0 0x0049
#define EMI_ECC_SINGLE_ERROR_0__LENGTH 0x0001

#define EMI_ECC_SINGLE_ERROR_0__CTRL_INFO_SHIFT 0
#define EMI_ECC_SINGLE_ERROR_0__CTRL_INFO_WIDTH 1
#define EMI_ECC_SINGLE_ERROR_0__CTRL_INFO_RESET_VAL 0
#define EMI_ECC_SINGLE_ERROR_0__CTRL_INFO_RMASK 0x1
#define EMI_ECC_SINGLE_ERROR_0__CTRL_INFO_MASK  0x1
#define EMI_ECC_SINGLE_ERROR_0__CTRL_INFO_FIELD 0,0

#define EMI_ECC_SINGLE_ERROR_0__WR_DATA_SHIFT 1
#define EMI_ECC_SINGLE_ERROR_0__WR_DATA_WIDTH 1
#define EMI_ECC_SINGLE_ERROR_0__WR_DATA_RESET_VAL 0
#define EMI_ECC_SINGLE_ERROR_0__WR_DATA_RMASK 0x1
#define EMI_ECC_SINGLE_ERROR_0__WR_DATA_MASK  0x2
#define EMI_ECC_SINGLE_ERROR_0__WR_DATA_FIELD 1,1

#define EMI_ECC_SINGLE_ERROR_0__RDATA_SHIFT 2
#define EMI_ECC_SINGLE_ERROR_0__RDATA_WIDTH 1
#define EMI_ECC_SINGLE_ERROR_0__RDATA_RESET_VAL 0
#define EMI_ECC_SINGLE_ERROR_0__RDATA_RMASK 0x1
#define EMI_ECC_SINGLE_ERROR_0__RDATA_MASK  0x4
#define EMI_ECC_SINGLE_ERROR_0__RDATA_FIELD 2,2


/*
 * ECC_SINGLE_ERROR_MASK_0.
 * EZcp API: EZapiPrm_writeReg
 *
 * ECC Single Error Mask 0: Provides a single error interrupt mask for all
 * SRAMs that have ECC or parity. The interrupt will be sent to external host
 * if in register ECC_DISABLE_STATUS the status indication is enabled.
 * Bit Value:
 * 0 - Enable ECC single error interrupt for this SRAM.
 * 1 - Disable ECC single error interrupt to external host for this SRAM.
 * Bit Description:
 * b0 - Name of first SRAM with ECC.
 * ...
 */
#define EMI_ECC_SINGLE_ERROR_MASK_0 0x0051
#define EMI_ECC_SINGLE_ERROR_MASK_0__LENGTH 0x0001

#define EMI_ECC_SINGLE_ERROR_MASK_0__CTRL_INFO_SHIFT 0
#define EMI_ECC_SINGLE_ERROR_MASK_0__CTRL_INFO_WIDTH 1
#define EMI_ECC_SINGLE_ERROR_MASK_0__CTRL_INFO_RESET_VAL 0
#define EMI_ECC_SINGLE_ERROR_MASK_0__CTRL_INFO_RMASK 0x1
#define EMI_ECC_SINGLE_ERROR_MASK_0__CTRL_INFO_MASK  0x1
#define EMI_ECC_SINGLE_ERROR_MASK_0__CTRL_INFO_FIELD 0,0

#define EMI_ECC_SINGLE_ERROR_MASK_0__WR_DATA_SHIFT 1
#define EMI_ECC_SINGLE_ERROR_MASK_0__WR_DATA_WIDTH 1
#define EMI_ECC_SINGLE_ERROR_MASK_0__WR_DATA_RESET_VAL 0
#define EMI_ECC_SINGLE_ERROR_MASK_0__WR_DATA_RMASK 0x1
#define EMI_ECC_SINGLE_ERROR_MASK_0__WR_DATA_MASK  0x2
#define EMI_ECC_SINGLE_ERROR_MASK_0__WR_DATA_FIELD 1,1

#define EMI_ECC_SINGLE_ERROR_MASK_0__RDATA_SHIFT 2
#define EMI_ECC_SINGLE_ERROR_MASK_0__RDATA_WIDTH 1
#define EMI_ECC_SINGLE_ERROR_MASK_0__RDATA_RESET_VAL 0
#define EMI_ECC_SINGLE_ERROR_MASK_0__RDATA_RMASK 0x1
#define EMI_ECC_SINGLE_ERROR_MASK_0__RDATA_MASK  0x4
#define EMI_ECC_SINGLE_ERROR_MASK_0__RDATA_FIELD 2,2


/*
 * ECC_DOUBLE_ERROR_0.
 * EZcp API: EZapiPrm_SERCheckError
 *
 * ECC Double Error 0: Provides a host interrupt cause for all SRAMs that
 * have ECC or parity.
 * Bit Value:
 * 0 - SRAM has no error.
 * 1 - SRAM has an error.
 * Bit Description:
 * b0 - Name of ECCID 0 with ECC.
 */
#define EMI_ECC_DOUBLE_ERROR_0 0x0059
#define EMI_ECC_DOUBLE_ERROR_0__LENGTH 0x0001

#define EMI_ECC_DOUBLE_ERROR_0__CTRL_INFO_SHIFT 0
#define EMI_ECC_DOUBLE_ERROR_0__CTRL_INFO_WIDTH 1
#define EMI_ECC_DOUBLE_ERROR_0__CTRL_INFO_RESET_VAL 0
#define EMI_ECC_DOUBLE_ERROR_0__CTRL_INFO_RMASK 0x1
#define EMI_ECC_DOUBLE_ERROR_0__CTRL_INFO_MASK  0x1
#define EMI_ECC_DOUBLE_ERROR_0__CTRL_INFO_FIELD 0,0

#define EMI_ECC_DOUBLE_ERROR_0__WR_DATA_SHIFT 1
#define EMI_ECC_DOUBLE_ERROR_0__WR_DATA_WIDTH 1
#define EMI_ECC_DOUBLE_ERROR_0__WR_DATA_RESET_VAL 0
#define EMI_ECC_DOUBLE_ERROR_0__WR_DATA_RMASK 0x1
#define EMI_ECC_DOUBLE_ERROR_0__WR_DATA_MASK  0x2
#define EMI_ECC_DOUBLE_ERROR_0__WR_DATA_FIELD 1,1

#define EMI_ECC_DOUBLE_ERROR_0__RDATA_SHIFT 2
#define EMI_ECC_DOUBLE_ERROR_0__RDATA_WIDTH 1
#define EMI_ECC_DOUBLE_ERROR_0__RDATA_RESET_VAL 0
#define EMI_ECC_DOUBLE_ERROR_0__RDATA_RMASK 0x1
#define EMI_ECC_DOUBLE_ERROR_0__RDATA_MASK  0x4
#define EMI_ECC_DOUBLE_ERROR_0__RDATA_FIELD 2,2


/*
 * ECC_DOUBLE_ERROR_MASK_0.
 * EZcp API: EZapiPrm_writeReg
 *
 * ECC Double Error Mask 0: Provides a double error interrupt mask for all
 * SRAMs that have ECC or parity. The interrupt will be sent to external host
 * if in register ECC_DISABLE_STATUS the status indication is enabled.
 * Bit Value:
 * 0 - Enable ECC double error interrupt to external host for this SRAM.
 * 1 - Disable ECC double error interrupt for this SRAM.
 * Bit Description:
 * b0 - Name of first SRAM with ECC.
 * ...
 */
#define EMI_ECC_DOUBLE_ERROR_MASK_0 0x0061
#define EMI_ECC_DOUBLE_ERROR_MASK_0__LENGTH 0x0001

#define EMI_ECC_DOUBLE_ERROR_MASK_0__CTRL_INFO_SHIFT 0
#define EMI_ECC_DOUBLE_ERROR_MASK_0__CTRL_INFO_WIDTH 1
#define EMI_ECC_DOUBLE_ERROR_MASK_0__CTRL_INFO_RESET_VAL 0
#define EMI_ECC_DOUBLE_ERROR_MASK_0__CTRL_INFO_RMASK 0x1
#define EMI_ECC_DOUBLE_ERROR_MASK_0__CTRL_INFO_MASK  0x1
#define EMI_ECC_DOUBLE_ERROR_MASK_0__CTRL_INFO_FIELD 0,0

#define EMI_ECC_DOUBLE_ERROR_MASK_0__WR_DATA_SHIFT 1
#define EMI_ECC_DOUBLE_ERROR_MASK_0__WR_DATA_WIDTH 1
#define EMI_ECC_DOUBLE_ERROR_MASK_0__WR_DATA_RESET_VAL 0
#define EMI_ECC_DOUBLE_ERROR_MASK_0__WR_DATA_RMASK 0x1
#define EMI_ECC_DOUBLE_ERROR_MASK_0__WR_DATA_MASK  0x2
#define EMI_ECC_DOUBLE_ERROR_MASK_0__WR_DATA_FIELD 1,1

#define EMI_ECC_DOUBLE_ERROR_MASK_0__RDATA_SHIFT 2
#define EMI_ECC_DOUBLE_ERROR_MASK_0__RDATA_WIDTH 1
#define EMI_ECC_DOUBLE_ERROR_MASK_0__RDATA_RESET_VAL 0
#define EMI_ECC_DOUBLE_ERROR_MASK_0__RDATA_RMASK 0x1
#define EMI_ECC_DOUBLE_ERROR_MASK_0__RDATA_MASK  0x4
#define EMI_ECC_DOUBLE_ERROR_MASK_0__RDATA_FIELD 2,2


/*
 * ECC_DISABLE_FIX_0.
 * EZcp API: EZapiPrm_writeReg
 *
 * ECC Disable Fix: Disables ECC correction that corrects single-error in
 * SRAMs that have ECC protection.
 * Bit Value:
 * 0 - Enable ECC correction (only single errors can be corrected).
 * 1 - Disable ECC correction.
 * Bit Description:
 * b0 - First ECC ID of the SRAM (ECCID)
 */
#define EMI_ECC_DISABLE_FIX_0 0x0069
#define EMI_ECC_DISABLE_FIX_0__LENGTH 0x0001

#define EMI_ECC_DISABLE_FIX_0__CTRL_INFO_SHIFT 0
#define EMI_ECC_DISABLE_FIX_0__CTRL_INFO_WIDTH 1
#define EMI_ECC_DISABLE_FIX_0__CTRL_INFO_RESET_VAL 0
#define EMI_ECC_DISABLE_FIX_0__CTRL_INFO_RMASK 0x1
#define EMI_ECC_DISABLE_FIX_0__CTRL_INFO_MASK  0x1
#define EMI_ECC_DISABLE_FIX_0__CTRL_INFO_FIELD 0,0

#define EMI_ECC_DISABLE_FIX_0__WR_DATA_SHIFT 1
#define EMI_ECC_DISABLE_FIX_0__WR_DATA_WIDTH 1
#define EMI_ECC_DISABLE_FIX_0__WR_DATA_RESET_VAL 0
#define EMI_ECC_DISABLE_FIX_0__WR_DATA_RMASK 0x1
#define EMI_ECC_DISABLE_FIX_0__WR_DATA_MASK  0x2
#define EMI_ECC_DISABLE_FIX_0__WR_DATA_FIELD 1,1

#define EMI_ECC_DISABLE_FIX_0__RDATA_SHIFT 2
#define EMI_ECC_DISABLE_FIX_0__RDATA_WIDTH 1
#define EMI_ECC_DISABLE_FIX_0__RDATA_RESET_VAL 0
#define EMI_ECC_DISABLE_FIX_0__RDATA_RMASK 0x1
#define EMI_ECC_DISABLE_FIX_0__RDATA_MASK  0x4
#define EMI_ECC_DISABLE_FIX_0__RDATA_FIELD 2,2


/*
 * ECC_DISABLE_STATUS_0.
 * EZcp API: EZapiPrm_writeReg
 *
 * ECC Disable Status 0: Disables SRAM ECC status indication for memories to
 * host block. An interrupt will be sent to external host if in register
 * ECC_SINGLE_ERROR_MASK or ECC_DOUBLE_ERROR_MASK the interrupt bit is
 * enabled.
 * Bit Value:
 * 0 - Enable ECC status indication to host block.
 * 1 - Disable ECC status indication.
 * Bit Description:
 * b0 - First ECC ID of the SRAM (ECCID)
 */
#define EMI_ECC_DISABLE_STATUS_0 0x0071
#define EMI_ECC_DISABLE_STATUS_0__LENGTH 0x0001

#define EMI_ECC_DISABLE_STATUS_0__CTRL_INFO_SHIFT 0
#define EMI_ECC_DISABLE_STATUS_0__CTRL_INFO_WIDTH 1
#define EMI_ECC_DISABLE_STATUS_0__CTRL_INFO_RESET_VAL 0
#define EMI_ECC_DISABLE_STATUS_0__CTRL_INFO_RMASK 0x1
#define EMI_ECC_DISABLE_STATUS_0__CTRL_INFO_MASK  0x1
#define EMI_ECC_DISABLE_STATUS_0__CTRL_INFO_FIELD 0,0

#define EMI_ECC_DISABLE_STATUS_0__WR_DATA_SHIFT 1
#define EMI_ECC_DISABLE_STATUS_0__WR_DATA_WIDTH 1
#define EMI_ECC_DISABLE_STATUS_0__WR_DATA_RESET_VAL 0
#define EMI_ECC_DISABLE_STATUS_0__WR_DATA_RMASK 0x1
#define EMI_ECC_DISABLE_STATUS_0__WR_DATA_MASK  0x2
#define EMI_ECC_DISABLE_STATUS_0__WR_DATA_FIELD 1,1

#define EMI_ECC_DISABLE_STATUS_0__RDATA_SHIFT 2
#define EMI_ECC_DISABLE_STATUS_0__RDATA_WIDTH 1
#define EMI_ECC_DISABLE_STATUS_0__RDATA_RESET_VAL 0
#define EMI_ECC_DISABLE_STATUS_0__RDATA_RMASK 0x1
#define EMI_ECC_DISABLE_STATUS_0__RDATA_MASK  0x4
#define EMI_ECC_DISABLE_STATUS_0__RDATA_FIELD 2,2


/*
 * ECC_LATCH_MODE_0.
 * EZcp API: EZapiPrm_SERCheckError
 *
 * ECC Latch Mode 0: Indicates if the first or last ECC error (faulty line
 * and syndrome bit) will be stored by the SRAM. This may be useful
 * understanding multi ECC error events. When reading the registers
 * ECC_ADDR_ERROR_DATA_LOW/HIGH the results will be according to register
 * ECC_LATCH_MODE.
 * Bit Value:
 * 1 - First ECC error will be stored by the SRAM.
 * 0 - Last ECC error will be stored by the SRAM.
 * Bit Description:
 * b0 - First ECC ID of the SRAM (ECCID)
 */
#define EMI_ECC_LATCH_MODE_0 0x0079
#define EMI_ECC_LATCH_MODE_0__LENGTH 0x0001

#define EMI_ECC_LATCH_MODE_0__CTRL_INFO_SHIFT 0
#define EMI_ECC_LATCH_MODE_0__CTRL_INFO_WIDTH 1
#define EMI_ECC_LATCH_MODE_0__CTRL_INFO_RESET_VAL 0
#define EMI_ECC_LATCH_MODE_0__CTRL_INFO_RMASK 0x1
#define EMI_ECC_LATCH_MODE_0__CTRL_INFO_MASK  0x1
#define EMI_ECC_LATCH_MODE_0__CTRL_INFO_FIELD 0,0

#define EMI_ECC_LATCH_MODE_0__WR_DATA_SHIFT 1
#define EMI_ECC_LATCH_MODE_0__WR_DATA_WIDTH 1
#define EMI_ECC_LATCH_MODE_0__WR_DATA_RESET_VAL 0
#define EMI_ECC_LATCH_MODE_0__WR_DATA_RMASK 0x1
#define EMI_ECC_LATCH_MODE_0__WR_DATA_MASK  0x2
#define EMI_ECC_LATCH_MODE_0__WR_DATA_FIELD 1,1

#define EMI_ECC_LATCH_MODE_0__RDATA_SHIFT 2
#define EMI_ECC_LATCH_MODE_0__RDATA_WIDTH 1
#define EMI_ECC_LATCH_MODE_0__RDATA_RESET_VAL 0
#define EMI_ECC_LATCH_MODE_0__RDATA_RMASK 0x1
#define EMI_ECC_LATCH_MODE_0__RDATA_MASK  0x4
#define EMI_ECC_LATCH_MODE_0__RDATA_FIELD 2,2


/*
 * DRAM_ECC_COUNT.
 * EZcp API: EZapiPrm_readReg
 *
 * DRAM ECC Counter: Provides a total single and double error counter for all
 * DRAMs that have ECC. Clear on read. No wrap around.
 */
#define EMI_DRAM_ECC_COUNT 0x00d0
#define EMI_DRAM_ECC_COUNT__LENGTH 0x0001

#define EMI_DRAM_ECC_COUNT__SINGLE_ERROR_COUNT_SHIFT 0
#define EMI_DRAM_ECC_COUNT__SINGLE_ERROR_COUNT_WIDTH 16
#define EMI_DRAM_ECC_COUNT__SINGLE_ERROR_COUNT_RESET_VAL 0
#define EMI_DRAM_ECC_COUNT__SINGLE_ERROR_COUNT_RMASK 0xffff
#define EMI_DRAM_ECC_COUNT__SINGLE_ERROR_COUNT_MASK  0xffff
#define EMI_DRAM_ECC_COUNT__SINGLE_ERROR_COUNT_FIELD 0,15

#define EMI_DRAM_ECC_COUNT__DOUBLE_ERROR_COUNT_SHIFT 16
#define EMI_DRAM_ECC_COUNT__DOUBLE_ERROR_COUNT_WIDTH 16
#define EMI_DRAM_ECC_COUNT__DOUBLE_ERROR_COUNT_RESET_VAL 0
#define EMI_DRAM_ECC_COUNT__DOUBLE_ERROR_COUNT_RMASK 0xffff
#define EMI_DRAM_ECC_COUNT__DOUBLE_ERROR_COUNT_MASK  0xffff0000
#define EMI_DRAM_ECC_COUNT__DOUBLE_ERROR_COUNT_FIELD 16,31


/*
 * DRAM_ECC_INJECT.
 * EZcp API: EZapiPrm_writeReg
 *
 * DRAM ECC Injection: Injects a single error (flips one data bit) or double
 * error (flips two data bits) for specified DRAM wrapper during a single
 * write.
 * Write 1 another time for an additional error injection. It is not possible
 * e.g. to select both DRAM_ECC_0_SINGLE_INJECT and DRAM_ECC_0_DOUBLE_INJECT.
 *
 * Note: Generic register that is only valid for EMEM_MI, PSTAT, and ODSTAT
 * blocks.
 */
#define EMI_DRAM_ECC_INJECT 0x00d1
#define EMI_DRAM_ECC_INJECT__LENGTH 0x0001

#define EMI_DRAM_ECC_INJECT__DRAM_ECC_SINGLE_INJECT_SHIFT 0
#define EMI_DRAM_ECC_INJECT__DRAM_ECC_SINGLE_INJECT_WIDTH 1
#define EMI_DRAM_ECC_INJECT__DRAM_ECC_SINGLE_INJECT_RESET_VAL 0
#define EMI_DRAM_ECC_INJECT__DRAM_ECC_SINGLE_INJECT_RMASK 0x1
#define EMI_DRAM_ECC_INJECT__DRAM_ECC_SINGLE_INJECT_MASK  0x1
#define EMI_DRAM_ECC_INJECT__DRAM_ECC_SINGLE_INJECT_FIELD 0,0

#define EMI_DRAM_ECC_INJECT__DRAM_ECC_DOUBLE_INJECT_SHIFT 16
#define EMI_DRAM_ECC_INJECT__DRAM_ECC_DOUBLE_INJECT_WIDTH 1
#define EMI_DRAM_ECC_INJECT__DRAM_ECC_DOUBLE_INJECT_RESET_VAL 0
#define EMI_DRAM_ECC_INJECT__DRAM_ECC_DOUBLE_INJECT_RMASK 0x1
#define EMI_DRAM_ECC_INJECT__DRAM_ECC_DOUBLE_INJECT_MASK  0x10000
#define EMI_DRAM_ECC_INJECT__DRAM_ECC_DOUBLE_INJECT_FIELD 16,16


/*
 * DRAM_ECC_ERROR.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM ECC Error: Provides a host interrupt cause for all DRAMs that have
 * single or double ECC error.
 */
#define EMI_DRAM_ECC_ERROR 0x00d2
#define EMI_DRAM_ECC_ERROR__LENGTH 0x0001

#define EMI_DRAM_ECC_ERROR__DRAM_ECC_SINGLE_SHIFT 0
#define EMI_DRAM_ECC_ERROR__DRAM_ECC_SINGLE_WIDTH 1
#define EMI_DRAM_ECC_ERROR__DRAM_ECC_SINGLE_RESET_VAL 0
#define EMI_DRAM_ECC_ERROR__DRAM_ECC_SINGLE_RMASK 0x1
#define EMI_DRAM_ECC_ERROR__DRAM_ECC_SINGLE_MASK  0x1
#define EMI_DRAM_ECC_ERROR__DRAM_ECC_SINGLE_FIELD 0,0

#define EMI_DRAM_ECC_ERROR__DRAM_ECC_DOUBLE_SHIFT 16
#define EMI_DRAM_ECC_ERROR__DRAM_ECC_DOUBLE_WIDTH 1
#define EMI_DRAM_ECC_ERROR__DRAM_ECC_DOUBLE_RESET_VAL 0
#define EMI_DRAM_ECC_ERROR__DRAM_ECC_DOUBLE_RMASK 0x1
#define EMI_DRAM_ECC_ERROR__DRAM_ECC_DOUBLE_MASK  0x10000
#define EMI_DRAM_ECC_ERROR__DRAM_ECC_DOUBLE_FIELD 16,16


/*
 * DRAM_ECC_MASK_ERROR.
 * EZcp API: EZapiPrm_writeReg
 *
 * DRAM ECC Mask Error: Provides a single and double error interrupt mask for
 * all DRAMs that have ECC or parity.
 */
#define EMI_DRAM_ECC_MASK_ERROR 0x00d3
#define EMI_DRAM_ECC_MASK_ERROR__LENGTH 0x0001

#define EMI_DRAM_ECC_MASK_ERROR__DRAM_ECC_SINGLE_SHIFT 0
#define EMI_DRAM_ECC_MASK_ERROR__DRAM_ECC_SINGLE_WIDTH 1
#define EMI_DRAM_ECC_MASK_ERROR__DRAM_ECC_SINGLE_RESET_VAL 0
#define EMI_DRAM_ECC_MASK_ERROR__DRAM_ECC_SINGLE_RMASK 0x1
#define EMI_DRAM_ECC_MASK_ERROR__DRAM_ECC_SINGLE_MASK  0x1
#define EMI_DRAM_ECC_MASK_ERROR__DRAM_ECC_SINGLE_FIELD 0,0

#define EMI_DRAM_ECC_MASK_ERROR__DRAM_ECC_DOUBLE_SHIFT 16
#define EMI_DRAM_ECC_MASK_ERROR__DRAM_ECC_DOUBLE_WIDTH 1
#define EMI_DRAM_ECC_MASK_ERROR__DRAM_ECC_DOUBLE_RESET_VAL 0
#define EMI_DRAM_ECC_MASK_ERROR__DRAM_ECC_DOUBLE_RMASK 0x1
#define EMI_DRAM_ECC_MASK_ERROR__DRAM_ECC_DOUBLE_MASK  0x10000
#define EMI_DRAM_ECC_MASK_ERROR__DRAM_ECC_DOUBLE_FIELD 16,16


/*
 * DRAM_ECC_DISABLE.
 * EZcp API: EZapiPrm_writeReg
 *
 * DRAM ECC Disable: Disables ECC calculation and fix that corrects
 * single-error in memories that have ECC protection.
 */
#define EMI_DRAM_ECC_DISABLE 0x00d4
#define EMI_DRAM_ECC_DISABLE__LENGTH 0x0001

#define EMI_DRAM_ECC_DISABLE__DRAM_ECC_DISABLE_FIX_SHIFT 0
#define EMI_DRAM_ECC_DISABLE__DRAM_ECC_DISABLE_FIX_WIDTH 1
#define EMI_DRAM_ECC_DISABLE__DRAM_ECC_DISABLE_FIX_RESET_VAL 0
#define EMI_DRAM_ECC_DISABLE__DRAM_ECC_DISABLE_FIX_RMASK 0x1
#define EMI_DRAM_ECC_DISABLE__DRAM_ECC_DISABLE_FIX_MASK  0x1
#define EMI_DRAM_ECC_DISABLE__DRAM_ECC_DISABLE_FIX_FIELD 0,0

#define EMI_DRAM_ECC_DISABLE__DRAM_ECC_DISABLE_STATUS_SHIFT 16
#define EMI_DRAM_ECC_DISABLE__DRAM_ECC_DISABLE_STATUS_WIDTH 1
#define EMI_DRAM_ECC_DISABLE__DRAM_ECC_DISABLE_STATUS_RESET_VAL 0
#define EMI_DRAM_ECC_DISABLE__DRAM_ECC_DISABLE_STATUS_RMASK 0x1
#define EMI_DRAM_ECC_DISABLE__DRAM_ECC_DISABLE_STATUS_MASK  0x10000
#define EMI_DRAM_ECC_DISABLE__DRAM_ECC_DISABLE_STATUS_FIELD 16,16


/*
 * DRAM_ECC_LATCH_SELECT.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM ECC Latch Select: Indicates if the first or last ECC error will be
 * stored by the DRAM
 */
#define EMI_DRAM_ECC_LATCH_SELECT 0x00d5
#define EMI_DRAM_ECC_LATCH_SELECT__LENGTH 0x0001

#define EMI_DRAM_ECC_LATCH_SELECT__DRAM_ECC_FIRST_SHIFT 0
#define EMI_DRAM_ECC_LATCH_SELECT__DRAM_ECC_FIRST_WIDTH 1
#define EMI_DRAM_ECC_LATCH_SELECT__DRAM_ECC_FIRST_RESET_VAL 1
#define EMI_DRAM_ECC_LATCH_SELECT__DRAM_ECC_FIRST_RMASK 0x1
#define EMI_DRAM_ECC_LATCH_SELECT__DRAM_ECC_FIRST_MASK  0x1
#define EMI_DRAM_ECC_LATCH_SELECT__DRAM_ECC_FIRST_FIELD 0,0

#define EMI_DRAM_ECC_LATCH_SELECT__EDGE_SEL_SHIFT 16
#define EMI_DRAM_ECC_LATCH_SELECT__EDGE_SEL_WIDTH 4
#define EMI_DRAM_ECC_LATCH_SELECT__EDGE_SEL_RESET_VAL 0
#define EMI_DRAM_ECC_LATCH_SELECT__EDGE_SEL_RMASK 0xf
#define EMI_DRAM_ECC_LATCH_SELECT__EDGE_SEL_MASK  0xf0000
#define EMI_DRAM_ECC_LATCH_SELECT__EDGE_SEL_FIELD 16,19

#define EMI_DRAM_ECC_LATCH_SELECT__START_SHIFT 24
#define EMI_DRAM_ECC_LATCH_SELECT__START_WIDTH 1
#define EMI_DRAM_ECC_LATCH_SELECT__START_RESET_VAL 0
#define EMI_DRAM_ECC_LATCH_SELECT__START_RMASK 0x1
#define EMI_DRAM_ECC_LATCH_SELECT__START_MASK  0x1000000
#define EMI_DRAM_ECC_LATCH_SELECT__START_FIELD 24,24


/*
 * DRAM_ERR_ADDR_0.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Error Address: Provides the ECC error address for the DRAM wrapper
 * selected by fields DRAM_ID_SEL and INDEX_SEL in register
 * DRAM_ECC_LATCH_SELECT. See register DRAM_SYNDROM for additional ECC error
 * information.
 */
#define EMI_DRAM_ERR_ADDR_0 0x00d6
#define EMI_DRAM_ERR_ADDR_0__LENGTH 0x0001

#define EMI_DRAM_ERR_ADDR_0__ERROR_ADDR_SHIFT 0
#define EMI_DRAM_ERR_ADDR_0__ERROR_ADDR_WIDTH 32
#define EMI_DRAM_ERR_ADDR_0__ERROR_ADDR_RESET_VAL 0
#define EMI_DRAM_ERR_ADDR_0__ERROR_ADDR_RMASK 0xffffffff
#define EMI_DRAM_ERR_ADDR_0__ERROR_ADDR_MASK  0xffffffff
#define EMI_DRAM_ERR_ADDR_0__ERROR_ADDR_FIELD 0,31


/*
 * DRAM_SYNDROM.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Syndrome: Provides ECC error information. See register DRAM_ERR_ADDR
 * for ECC error address.
 */
#define EMI_DRAM_SYNDROM 0x00d7
#define EMI_DRAM_SYNDROM__LENGTH 0x0001

#define EMI_DRAM_SYNDROM__DERR_SHIFT 0
#define EMI_DRAM_SYNDROM__DERR_WIDTH 1
#define EMI_DRAM_SYNDROM__DERR_RESET_VAL 0
#define EMI_DRAM_SYNDROM__DERR_RMASK 0x1
#define EMI_DRAM_SYNDROM__DERR_MASK  0x1
#define EMI_DRAM_SYNDROM__DERR_FIELD 0,0

#define EMI_DRAM_SYNDROM__SERR_SHIFT 1
#define EMI_DRAM_SYNDROM__SERR_WIDTH 1
#define EMI_DRAM_SYNDROM__SERR_RESET_VAL 0
#define EMI_DRAM_SYNDROM__SERR_RMASK 0x1
#define EMI_DRAM_SYNDROM__SERR_MASK  0x2
#define EMI_DRAM_SYNDROM__SERR_FIELD 1,1

#define EMI_DRAM_SYNDROM__SYNDROM_SHIFT 16
#define EMI_DRAM_SYNDROM__SYNDROM_WIDTH 10
#define EMI_DRAM_SYNDROM__SYNDROM_RESET_VAL 0
#define EMI_DRAM_SYNDROM__SYNDROM_RMASK 0x3ff
#define EMI_DRAM_SYNDROM__SYNDROM_MASK  0x3ff0000
#define EMI_DRAM_SYNDROM__SYNDROM_FIELD 16,25


/*
 * DRAM_FIRST_LAST.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * Stores the DRAM wrapper ID of first and last ECC error, and indicates if
 * more than one error occurred.
 */
#define EMI_DRAM_FIRST_LAST 0x00d8
#define EMI_DRAM_FIRST_LAST__LENGTH 0x0001

#define EMI_DRAM_FIRST_LAST__FIRST_DRAM_ID_SERR_SHIFT 0
#define EMI_DRAM_FIRST_LAST__FIRST_DRAM_ID_SERR_WIDTH 4
#define EMI_DRAM_FIRST_LAST__FIRST_DRAM_ID_SERR_RESET_VAL 0
#define EMI_DRAM_FIRST_LAST__FIRST_DRAM_ID_SERR_RMASK 0xf
#define EMI_DRAM_FIRST_LAST__FIRST_DRAM_ID_SERR_MASK  0xf
#define EMI_DRAM_FIRST_LAST__FIRST_DRAM_ID_SERR_FIELD 0,3

#define EMI_DRAM_FIRST_LAST__MULTI_FIRST_SERR_SHIFT 7
#define EMI_DRAM_FIRST_LAST__MULTI_FIRST_SERR_WIDTH 1
#define EMI_DRAM_FIRST_LAST__MULTI_FIRST_SERR_RESET_VAL 0
#define EMI_DRAM_FIRST_LAST__MULTI_FIRST_SERR_RMASK 0x1
#define EMI_DRAM_FIRST_LAST__MULTI_FIRST_SERR_MASK  0x80
#define EMI_DRAM_FIRST_LAST__MULTI_FIRST_SERR_FIELD 7,7

#define EMI_DRAM_FIRST_LAST__LAST_DRAM_ID_SERR_SHIFT 8
#define EMI_DRAM_FIRST_LAST__LAST_DRAM_ID_SERR_WIDTH 4
#define EMI_DRAM_FIRST_LAST__LAST_DRAM_ID_SERR_RESET_VAL 0
#define EMI_DRAM_FIRST_LAST__LAST_DRAM_ID_SERR_RMASK 0xf
#define EMI_DRAM_FIRST_LAST__LAST_DRAM_ID_SERR_MASK  0xf00
#define EMI_DRAM_FIRST_LAST__LAST_DRAM_ID_SERR_FIELD 8,11

#define EMI_DRAM_FIRST_LAST__MULTI_LAST_SERR_SHIFT 15
#define EMI_DRAM_FIRST_LAST__MULTI_LAST_SERR_WIDTH 1
#define EMI_DRAM_FIRST_LAST__MULTI_LAST_SERR_RESET_VAL 0
#define EMI_DRAM_FIRST_LAST__MULTI_LAST_SERR_RMASK 0x1
#define EMI_DRAM_FIRST_LAST__MULTI_LAST_SERR_MASK  0x8000
#define EMI_DRAM_FIRST_LAST__MULTI_LAST_SERR_FIELD 15,15

#define EMI_DRAM_FIRST_LAST__FIRST_DRAM_ID_DERR_SHIFT 16
#define EMI_DRAM_FIRST_LAST__FIRST_DRAM_ID_DERR_WIDTH 4
#define EMI_DRAM_FIRST_LAST__FIRST_DRAM_ID_DERR_RESET_VAL 0
#define EMI_DRAM_FIRST_LAST__FIRST_DRAM_ID_DERR_RMASK 0xf
#define EMI_DRAM_FIRST_LAST__FIRST_DRAM_ID_DERR_MASK  0xf0000
#define EMI_DRAM_FIRST_LAST__FIRST_DRAM_ID_DERR_FIELD 16,19

#define EMI_DRAM_FIRST_LAST__MULTI_FIRST_DERR_SHIFT 23
#define EMI_DRAM_FIRST_LAST__MULTI_FIRST_DERR_WIDTH 1
#define EMI_DRAM_FIRST_LAST__MULTI_FIRST_DERR_RESET_VAL 0
#define EMI_DRAM_FIRST_LAST__MULTI_FIRST_DERR_RMASK 0x1
#define EMI_DRAM_FIRST_LAST__MULTI_FIRST_DERR_MASK  0x800000
#define EMI_DRAM_FIRST_LAST__MULTI_FIRST_DERR_FIELD 23,23

#define EMI_DRAM_FIRST_LAST__LAST_DRAM_ID_DERR_SHIFT 24
#define EMI_DRAM_FIRST_LAST__LAST_DRAM_ID_DERR_WIDTH 4
#define EMI_DRAM_FIRST_LAST__LAST_DRAM_ID_DERR_RESET_VAL 0
#define EMI_DRAM_FIRST_LAST__LAST_DRAM_ID_DERR_RMASK 0xf
#define EMI_DRAM_FIRST_LAST__LAST_DRAM_ID_DERR_MASK  0xf000000
#define EMI_DRAM_FIRST_LAST__LAST_DRAM_ID_DERR_FIELD 24,27

#define EMI_DRAM_FIRST_LAST__MULTI_LAST_DERR_SHIFT 31
#define EMI_DRAM_FIRST_LAST__MULTI_LAST_DERR_WIDTH 1
#define EMI_DRAM_FIRST_LAST__MULTI_LAST_DERR_RESET_VAL 0
#define EMI_DRAM_FIRST_LAST__MULTI_LAST_DERR_RMASK 0x1
#define EMI_DRAM_FIRST_LAST__MULTI_LAST_DERR_MASK  0x80000000
#define EMI_DRAM_FIRST_LAST__MULTI_LAST_DERR_FIELD 31,31


/*
 * DRAM_ADDITIONAL_INFO_0.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Additional Information: Specific DRAM information provided by the
 * block's DRAM wrappers. EZchip internal use. The information is valid when
 * bit READY = 1 in register DRAM_SYNDROM.
 */
#define EMI_DRAM_ADDITIONAL_INFO_0 0x00d9
#define EMI_DRAM_ADDITIONAL_INFO_0__LENGTH 0x0001

#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_BANK_SHIFT 0
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_BANK_WIDTH 4
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_BANK_RESET_VAL 0
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_BANK_RMASK 0xf
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_BANK_MASK  0xf
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_BANK_FIELD 0,3

#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_LRANK_SHIFT 4
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_LRANK_WIDTH 2
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_LRANK_RESET_VAL 0
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_LRANK_RMASK 0x3
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_LRANK_MASK  0x30
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_LRANK_FIELD 4,5

#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_PRANK_SHIFT 8
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_PRANK_WIDTH 2
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_PRANK_RESET_VAL 0
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_PRANK_RMASK 0x3
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_PRANK_MASK  0x300
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_PRANK_FIELD 8,9

#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_EDGE_SHIFT 16
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_EDGE_WIDTH 8
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_EDGE_RESET_VAL 0
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_EDGE_RMASK 0xff
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_EDGE_MASK  0xff0000
#define EMI_DRAM_ADDITIONAL_INFO_0__ERR_EDGE_FIELD 16,23


/*
 * DRAM_ADDITIONAL_INFO_1.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Additional Information: Specific DRAM information provided by the
 * block's DRAM wrappers. EZchip internal use. The information is valid when
 * bit READY = 1 in register DRAM_SYNDROM.
 */
#define EMI_DRAM_ADDITIONAL_INFO_1 0x00da
#define EMI_DRAM_ADDITIONAL_INFO_1__LENGTH 0x0001

#define EMI_DRAM_ADDITIONAL_INFO_1__ERR_ADDR_SHIFT 0
#define EMI_DRAM_ADDITIONAL_INFO_1__ERR_ADDR_WIDTH 25
#define EMI_DRAM_ADDITIONAL_INFO_1__ERR_ADDR_RESET_VAL 0
#define EMI_DRAM_ADDITIONAL_INFO_1__ERR_ADDR_RMASK 0x1ffffff
#define EMI_DRAM_ADDITIONAL_INFO_1__ERR_ADDR_MASK  0x1ffffff
#define EMI_DRAM_ADDITIONAL_INFO_1__ERR_ADDR_FIELD 0,24


/*
 * DRAM_ADDITIONAL_INFO_2.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Additional Information: Specific DRAM information provided by the
 * block's DRAM wrappers. EZchip internal use. The information is valid when
 * bit READY = 1 in register DRAM_SYNDROM.
 */
#define EMI_DRAM_ADDITIONAL_INFO_2 0x00db
#define EMI_DRAM_ADDITIONAL_INFO_2__LENGTH 0x0001

#define EMI_DRAM_ADDITIONAL_INFO_2__ERR_STREAM_ID_SHIFT 0
#define EMI_DRAM_ADDITIONAL_INFO_2__ERR_STREAM_ID_WIDTH 8
#define EMI_DRAM_ADDITIONAL_INFO_2__ERR_STREAM_ID_RESET_VAL 0
#define EMI_DRAM_ADDITIONAL_INFO_2__ERR_STREAM_ID_RMASK 0xff
#define EMI_DRAM_ADDITIONAL_INFO_2__ERR_STREAM_ID_MASK  0xff
#define EMI_DRAM_ADDITIONAL_INFO_2__ERR_STREAM_ID_FIELD 0,7

#define EMI_DRAM_ADDITIONAL_INFO_2__HOST_ACCESS_SHIFT 16
#define EMI_DRAM_ADDITIONAL_INFO_2__HOST_ACCESS_WIDTH 1
#define EMI_DRAM_ADDITIONAL_INFO_2__HOST_ACCESS_RESET_VAL 0
#define EMI_DRAM_ADDITIONAL_INFO_2__HOST_ACCESS_RMASK 0x1
#define EMI_DRAM_ADDITIONAL_INFO_2__HOST_ACCESS_MASK  0x10000
#define EMI_DRAM_ADDITIONAL_INFO_2__HOST_ACCESS_FIELD 16,16


/*
 * DRAM_ADDITIONAL_INFO_3.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Additional Information: Specific DRAM information provided by the
 * block's DRAM wrappers. EZchip internal use. The information is valid when
 * bit READY = 1 in register DRAM_SYNDROM.
 */
#define EMI_DRAM_ADDITIONAL_INFO_3 0x00dc
#define EMI_DRAM_ADDITIONAL_INFO_3__LENGTH 0x0001

#define EMI_DRAM_ADDITIONAL_INFO_3__ERR_USR_FREE_SHIFT 0
#define EMI_DRAM_ADDITIONAL_INFO_3__ERR_USR_FREE_WIDTH 32
#define EMI_DRAM_ADDITIONAL_INFO_3__ERR_USR_FREE_RESET_VAL 0
#define EMI_DRAM_ADDITIONAL_INFO_3__ERR_USR_FREE_RMASK 0xffffffff
#define EMI_DRAM_ADDITIONAL_INFO_3__ERR_USR_FREE_MASK  0xffffffff
#define EMI_DRAM_ADDITIONAL_INFO_3__ERR_USR_FREE_FIELD 0,31


/*
 * DRAM_ADDITIONAL_INFO_4.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Additional Information: Specific DRAM information provided by the
 * block's DRAM wrappers. EZchip internal use. The information is valid when
 * bit READY = 1 in register DRAM_SYNDROM.
 */
#define EMI_DRAM_ADDITIONAL_INFO_4 0x00dd
#define EMI_DRAM_ADDITIONAL_INFO_4__LENGTH 0x0001

#define EMI_DRAM_ADDITIONAL_INFO_4__VAL_SHIFT 0
#define EMI_DRAM_ADDITIONAL_INFO_4__VAL_WIDTH 32
#define EMI_DRAM_ADDITIONAL_INFO_4__VAL_RESET_VAL 0
#define EMI_DRAM_ADDITIONAL_INFO_4__VAL_RMASK 0xffffffff
#define EMI_DRAM_ADDITIONAL_INFO_4__VAL_MASK  0xffffffff
#define EMI_DRAM_ADDITIONAL_INFO_4__VAL_FIELD 0,31


/*
 * DRAM_ADDITIONAL_INFO_5.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Additional Information: Specific DRAM information provided by the
 * block's DRAM wrappers. EZchip internal use. The information is valid when
 * bit READY = 1 in register DRAM_SYNDROM.
 */
#define EMI_DRAM_ADDITIONAL_INFO_5 0x00de
#define EMI_DRAM_ADDITIONAL_INFO_5__LENGTH 0x0001

#define EMI_DRAM_ADDITIONAL_INFO_5__VAL_SHIFT 0
#define EMI_DRAM_ADDITIONAL_INFO_5__VAL_WIDTH 32
#define EMI_DRAM_ADDITIONAL_INFO_5__VAL_RESET_VAL 0
#define EMI_DRAM_ADDITIONAL_INFO_5__VAL_RMASK 0xffffffff
#define EMI_DRAM_ADDITIONAL_INFO_5__VAL_MASK  0xffffffff
#define EMI_DRAM_ADDITIONAL_INFO_5__VAL_FIELD 0,31


/*
 * DRAM_ERR_ADDR_1.
 * EZcp API: EZapiChannel_StatCmd_GetSERInfo
 *
 * DRAM Error Address: Provides the ECC error address for the DRAM wrapper
 * selected by fields DRAM_ID_SEL and INDEX_SEL in register
 * DRAM_ECC_LATCH_SELECT. See register DRAM_SYNDROM for additional ECC error
 * information.
 */
#define EMI_DRAM_ERR_ADDR_1 0x00df
#define EMI_DRAM_ERR_ADDR_1__LENGTH 0x0001

#define EMI_DRAM_ERR_ADDR_1__ERROR_ADDR_SHIFT 0
#define EMI_DRAM_ERR_ADDR_1__ERROR_ADDR_WIDTH 32
#define EMI_DRAM_ERR_ADDR_1__ERROR_ADDR_RESET_VAL 0
#define EMI_DRAM_ERR_ADDR_1__ERROR_ADDR_RMASK 0xffffffff
#define EMI_DRAM_ERR_ADDR_1__ERROR_ADDR_MASK  0xffffffff
#define EMI_DRAM_ERR_ADDR_1__ERROR_ADDR_FIELD 0,31


/*
 * REQ_SEQUENCE.
 * Request sequence number configurations
 */
#define EMI_REQ_SEQUENCE 0x0100
#define EMI_REQ_SEQUENCE__LENGTH 0x0001

#define EMI_REQ_SEQUENCE__NUM_OPEN_REQ_SHIFT 0
#define EMI_REQ_SEQUENCE__NUM_OPEN_REQ_WIDTH 7
#define EMI_REQ_SEQUENCE__NUM_OPEN_REQ_RESET_VAL 63
#define EMI_REQ_SEQUENCE__NUM_OPEN_REQ_RMASK 0x7f
#define EMI_REQ_SEQUENCE__NUM_OPEN_REQ_MASK  0x7f
#define EMI_REQ_SEQUENCE__NUM_OPEN_REQ_FIELD 0,6

#define EMI_REQ_SEQUENCE__RD_HP_TH_SHIFT 8
#define EMI_REQ_SEQUENCE__RD_HP_TH_WIDTH 7
#define EMI_REQ_SEQUENCE__RD_HP_TH_RESET_VAL 3
#define EMI_REQ_SEQUENCE__RD_HP_TH_RMASK 0x7f
#define EMI_REQ_SEQUENCE__RD_HP_TH_MASK  0x7f00
#define EMI_REQ_SEQUENCE__RD_HP_TH_FIELD 8,14

#define EMI_REQ_SEQUENCE__WR_HP_TH_SHIFT 16
#define EMI_REQ_SEQUENCE__WR_HP_TH_WIDTH 7
#define EMI_REQ_SEQUENCE__WR_HP_TH_RESET_VAL 3
#define EMI_REQ_SEQUENCE__WR_HP_TH_RMASK 0x7f
#define EMI_REQ_SEQUENCE__WR_HP_TH_MASK  0x7f0000
#define EMI_REQ_SEQUENCE__WR_HP_TH_FIELD 16,22


/*
 * RD_LAT_EST.
 * Latency estimation configurations - read requests
 */
#define EMI_RD_LAT_EST 0x0101
#define EMI_RD_LAT_EST__LENGTH 0x0001

#define EMI_RD_LAT_EST__AVG_LAT_GRAN_SHIFT 8
#define EMI_RD_LAT_EST__AVG_LAT_GRAN_WIDTH 5
#define EMI_RD_LAT_EST__AVG_LAT_GRAN_RESET_VAL 3
#define EMI_RD_LAT_EST__AVG_LAT_GRAN_RMASK 0x1f
#define EMI_RD_LAT_EST__AVG_LAT_GRAN_MASK  0x1f00
#define EMI_RD_LAT_EST__AVG_LAT_GRAN_FIELD 8,12

#define EMI_RD_LAT_EST__LAT_DELTA_FACTOR_SHIFT 16
#define EMI_RD_LAT_EST__LAT_DELTA_FACTOR_WIDTH 3
#define EMI_RD_LAT_EST__LAT_DELTA_FACTOR_RESET_VAL 6
#define EMI_RD_LAT_EST__LAT_DELTA_FACTOR_RMASK 0x7
#define EMI_RD_LAT_EST__LAT_DELTA_FACTOR_MASK  0x70000
#define EMI_RD_LAT_EST__LAT_DELTA_FACTOR_FIELD 16,18

#define EMI_RD_LAT_EST__DEV_DELTA_FACTOR_SHIFT 20
#define EMI_RD_LAT_EST__DEV_DELTA_FACTOR_WIDTH 3
#define EMI_RD_LAT_EST__DEV_DELTA_FACTOR_RESET_VAL 6
#define EMI_RD_LAT_EST__DEV_DELTA_FACTOR_RMASK 0x7
#define EMI_RD_LAT_EST__DEV_DELTA_FACTOR_MASK  0x700000
#define EMI_RD_LAT_EST__DEV_DELTA_FACTOR_FIELD 20,22

#define EMI_RD_LAT_EST__DEV_FACTOR_SHIFT 24
#define EMI_RD_LAT_EST__DEV_FACTOR_WIDTH 4
#define EMI_RD_LAT_EST__DEV_FACTOR_RESET_VAL 0
#define EMI_RD_LAT_EST__DEV_FACTOR_RMASK 0xf
#define EMI_RD_LAT_EST__DEV_FACTOR_MASK  0xf000000
#define EMI_RD_LAT_EST__DEV_FACTOR_FIELD 24,27


/*
 * WR_LAT_EST.
 * Latency estimation configurations - write requests
 */
#define EMI_WR_LAT_EST 0x0102
#define EMI_WR_LAT_EST__LENGTH 0x0001

#define EMI_WR_LAT_EST__AVG_LAT_GRAN_SHIFT 8
#define EMI_WR_LAT_EST__AVG_LAT_GRAN_WIDTH 5
#define EMI_WR_LAT_EST__AVG_LAT_GRAN_RESET_VAL 3
#define EMI_WR_LAT_EST__AVG_LAT_GRAN_RMASK 0x1f
#define EMI_WR_LAT_EST__AVG_LAT_GRAN_MASK  0x1f00
#define EMI_WR_LAT_EST__AVG_LAT_GRAN_FIELD 8,12

#define EMI_WR_LAT_EST__LAT_DELTA_FACTOR_SHIFT 16
#define EMI_WR_LAT_EST__LAT_DELTA_FACTOR_WIDTH 3
#define EMI_WR_LAT_EST__LAT_DELTA_FACTOR_RESET_VAL 6
#define EMI_WR_LAT_EST__LAT_DELTA_FACTOR_RMASK 0x7
#define EMI_WR_LAT_EST__LAT_DELTA_FACTOR_MASK  0x70000
#define EMI_WR_LAT_EST__LAT_DELTA_FACTOR_FIELD 16,18

#define EMI_WR_LAT_EST__DEV_DELTA_FACTOR_SHIFT 20
#define EMI_WR_LAT_EST__DEV_DELTA_FACTOR_WIDTH 3
#define EMI_WR_LAT_EST__DEV_DELTA_FACTOR_RESET_VAL 6
#define EMI_WR_LAT_EST__DEV_DELTA_FACTOR_RMASK 0x7
#define EMI_WR_LAT_EST__DEV_DELTA_FACTOR_MASK  0x700000
#define EMI_WR_LAT_EST__DEV_DELTA_FACTOR_FIELD 20,22

#define EMI_WR_LAT_EST__DEV_FACTOR_SHIFT 24
#define EMI_WR_LAT_EST__DEV_FACTOR_WIDTH 4
#define EMI_WR_LAT_EST__DEV_FACTOR_RESET_VAL 0
#define EMI_WR_LAT_EST__DEV_FACTOR_RMASK 0xf
#define EMI_WR_LAT_EST__DEV_FACTOR_MASK  0xf000000
#define EMI_WR_LAT_EST__DEV_FACTOR_FIELD 24,27


/*
 * RD_RES.
 * Read response path configurations
 */
#define EMI_RD_RES 0x0103
#define EMI_RD_RES__LENGTH 0x0001

#define EMI_RD_RES__FIFO_CREDITS_SHIFT 0
#define EMI_RD_RES__FIFO_CREDITS_WIDTH 6
#define EMI_RD_RES__FIFO_CREDITS_RESET_VAL 32
#define EMI_RD_RES__FIFO_CREDITS_RMASK 0x3f
#define EMI_RD_RES__FIFO_CREDITS_MASK  0x3f
#define EMI_RD_RES__FIFO_CREDITS_FIELD 0,5


/*
 * RANK_ARB.
 * Rank arbiter configurations
 */
#define EMI_RANK_ARB 0x0104
#define EMI_RANK_ARB__LENGTH 0x0001

#define EMI_RANK_ARB__TH1_SHIFT 0
#define EMI_RANK_ARB__TH1_WIDTH 6
#define EMI_RANK_ARB__TH1_RESET_VAL 5
#define EMI_RANK_ARB__TH1_RMASK 0x3f
#define EMI_RANK_ARB__TH1_MASK  0x3f
#define EMI_RANK_ARB__TH1_FIELD 0,5

#define EMI_RANK_ARB__TH2_SHIFT 8
#define EMI_RANK_ARB__TH2_WIDTH 6
#define EMI_RANK_ARB__TH2_RESET_VAL 12
#define EMI_RANK_ARB__TH2_RMASK 0x3f
#define EMI_RANK_ARB__TH2_MASK  0x3f00
#define EMI_RANK_ARB__TH2_FIELD 8,13

#define EMI_RANK_ARB__POOR_BANK_DISTR_SHIFT 16
#define EMI_RANK_ARB__POOR_BANK_DISTR_WIDTH 7
#define EMI_RANK_ARB__POOR_BANK_DISTR_RESET_VAL 6
#define EMI_RANK_ARB__POOR_BANK_DISTR_RMASK 0x7f
#define EMI_RANK_ARB__POOR_BANK_DISTR_MASK  0x7f0000
#define EMI_RANK_ARB__POOR_BANK_DISTR_FIELD 16,22

#define EMI_RANK_ARB__POOR_BANK_DISTR_3DS_SHIFT 24
#define EMI_RANK_ARB__POOR_BANK_DISTR_3DS_WIDTH 7
#define EMI_RANK_ARB__POOR_BANK_DISTR_3DS_RESET_VAL 5
#define EMI_RANK_ARB__POOR_BANK_DISTR_3DS_RMASK 0x7f
#define EMI_RANK_ARB__POOR_BANK_DISTR_3DS_MASK  0x7f000000
#define EMI_RANK_ARB__POOR_BANK_DISTR_3DS_FIELD 24,30


/*
 * OLDEST_TIMER.
 * Oldest request service demand timer
 */
#define EMI_OLDEST_TIMER 0x0105
#define EMI_OLDEST_TIMER__LENGTH 0x0001

#define EMI_OLDEST_TIMER__RD_TH_SHIFT 0
#define EMI_OLDEST_TIMER__RD_TH_WIDTH 8
#define EMI_OLDEST_TIMER__RD_TH_RESET_VAL 12
#define EMI_OLDEST_TIMER__RD_TH_RMASK 0xff
#define EMI_OLDEST_TIMER__RD_TH_MASK  0xff
#define EMI_OLDEST_TIMER__RD_TH_FIELD 0,7

#define EMI_OLDEST_TIMER__WR_TH_SHIFT 8
#define EMI_OLDEST_TIMER__WR_TH_WIDTH 8
#define EMI_OLDEST_TIMER__WR_TH_RESET_VAL 12
#define EMI_OLDEST_TIMER__WR_TH_RMASK 0xff
#define EMI_OLDEST_TIMER__WR_TH_MASK  0xff00
#define EMI_OLDEST_TIMER__WR_TH_FIELD 8,15


/*
 * RD_LAT.
 * read latency thresholds
 */
#define EMI_RD_LAT 0x0106
#define EMI_RD_LAT__LENGTH 0x0001

#define EMI_RD_LAT__LOW_TH_SHIFT 0
#define EMI_RD_LAT__LOW_TH_WIDTH 9
#define EMI_RD_LAT__LOW_TH_RESET_VAL 20
#define EMI_RD_LAT__LOW_TH_RMASK 0x1ff
#define EMI_RD_LAT__LOW_TH_MASK  0x1ff
#define EMI_RD_LAT__LOW_TH_FIELD 0,8

#define EMI_RD_LAT__MED_TH_SHIFT 9
#define EMI_RD_LAT__MED_TH_WIDTH 9
#define EMI_RD_LAT__MED_TH_RESET_VAL 40
#define EMI_RD_LAT__MED_TH_RMASK 0x1ff
#define EMI_RD_LAT__MED_TH_MASK  0x3fe00
#define EMI_RD_LAT__MED_TH_FIELD 9,17

#define EMI_RD_LAT__HIGH_TH_SHIFT 18
#define EMI_RD_LAT__HIGH_TH_WIDTH 9
#define EMI_RD_LAT__HIGH_TH_RESET_VAL 60
#define EMI_RD_LAT__HIGH_TH_RMASK 0x1ff
#define EMI_RD_LAT__HIGH_TH_MASK  0x7fc0000
#define EMI_RD_LAT__HIGH_TH_FIELD 18,26


/*
 * WR_LAT.
 * write latency thresholds
 */
#define EMI_WR_LAT 0x0107
#define EMI_WR_LAT__LENGTH 0x0001

#define EMI_WR_LAT__LOW_TH_SHIFT 0
#define EMI_WR_LAT__LOW_TH_WIDTH 9
#define EMI_WR_LAT__LOW_TH_RESET_VAL 20
#define EMI_WR_LAT__LOW_TH_RMASK 0x1ff
#define EMI_WR_LAT__LOW_TH_MASK  0x1ff
#define EMI_WR_LAT__LOW_TH_FIELD 0,8

#define EMI_WR_LAT__MED_TH_SHIFT 9
#define EMI_WR_LAT__MED_TH_WIDTH 9
#define EMI_WR_LAT__MED_TH_RESET_VAL 40
#define EMI_WR_LAT__MED_TH_RMASK 0x1ff
#define EMI_WR_LAT__MED_TH_MASK  0x3fe00
#define EMI_WR_LAT__MED_TH_FIELD 9,17

#define EMI_WR_LAT__HIGH_TH_SHIFT 18
#define EMI_WR_LAT__HIGH_TH_WIDTH 9
#define EMI_WR_LAT__HIGH_TH_RESET_VAL 60
#define EMI_WR_LAT__HIGH_TH_RMASK 0x1ff
#define EMI_WR_LAT__HIGH_TH_MASK  0x7fc0000
#define EMI_WR_LAT__HIGH_TH_FIELD 18,26


/*
 * LRANK_ARB.
 * Logical Rank arbiter configurations
 */
#define EMI_LRANK_ARB 0x0108
#define EMI_LRANK_ARB__LENGTH 0x0001

#define EMI_LRANK_ARB__TH1_SHIFT 0
#define EMI_LRANK_ARB__TH1_WIDTH 5
#define EMI_LRANK_ARB__TH1_RESET_VAL 2
#define EMI_LRANK_ARB__TH1_RMASK 0x1f
#define EMI_LRANK_ARB__TH1_MASK  0x1f
#define EMI_LRANK_ARB__TH1_FIELD 0,4

#define EMI_LRANK_ARB__TH2_SHIFT 8
#define EMI_LRANK_ARB__TH2_WIDTH 5
#define EMI_LRANK_ARB__TH2_RESET_VAL 4
#define EMI_LRANK_ARB__TH2_RMASK 0x1f
#define EMI_LRANK_ARB__TH2_MASK  0x1f00
#define EMI_LRANK_ARB__TH2_FIELD 8,12

#define EMI_LRANK_ARB__POOR_BANK_DISTR_SHIFT 16
#define EMI_LRANK_ARB__POOR_BANK_DISTR_WIDTH 5
#define EMI_LRANK_ARB__POOR_BANK_DISTR_RESET_VAL 2
#define EMI_LRANK_ARB__POOR_BANK_DISTR_RMASK 0x1f
#define EMI_LRANK_ARB__POOR_BANK_DISTR_MASK  0x1f0000
#define EMI_LRANK_ARB__POOR_BANK_DISTR_FIELD 16,20

#define EMI_LRANK_ARB__EN_SHIFT 24
#define EMI_LRANK_ARB__EN_WIDTH 1
#define EMI_LRANK_ARB__EN_RESET_VAL 1
#define EMI_LRANK_ARB__EN_RMASK 0x1
#define EMI_LRANK_ARB__EN_MASK  0x1000000
#define EMI_LRANK_ARB__EN_FIELD 24,24


/*
 * L3C_IF.
 * L3 Cache Interface configurations
 */
#define EMI_L3C_IF 0x0109
#define EMI_L3C_IF__LENGTH 0x0001

#define EMI_L3C_IF__EN_SHIFT 0
#define EMI_L3C_IF__EN_WIDTH 1
#define EMI_L3C_IF__EN_RESET_VAL 1
#define EMI_L3C_IF__EN_RMASK 0x1
#define EMI_L3C_IF__EN_MASK  0x1
#define EMI_L3C_IF__EN_FIELD 0,0


/*
 * HOST_ENDLESS.
 * Host Endless Operation configuration register
 */
#define EMI_HOST_ENDLESS 0x010a
#define EMI_HOST_ENDLESS__LENGTH 0x0001

#define EMI_HOST_ENDLESS__EN_SHIFT 0
#define EMI_HOST_ENDLESS__EN_WIDTH 1
#define EMI_HOST_ENDLESS__EN_RESET_VAL 1
#define EMI_HOST_ENDLESS__EN_RMASK 0x1
#define EMI_HOST_ENDLESS__EN_MASK  0x1
#define EMI_HOST_ENDLESS__EN_FIELD 0,0


/*
 * STATUS.
 * reflects the statuses from the block
 */
#define EMI_STATUS__FIRST_WORD 0x0124
#define EMI_STATUS__LAST_WORD 0x012c
#define EMI_STATUS__LENGTH 0x0009
#define EMI_STATUS__STRIDE 0x0001

#define EMI_STATUS__VAL_SHIFT 0
#define EMI_STATUS__VAL_WIDTH 32
#define EMI_STATUS__VAL_RESET_VAL 0
#define EMI_STATUS__VAL_RMASK 0xffffffff
#define EMI_STATUS__VAL_MASK  0xffffffff
#define EMI_STATUS__VAL_FIELD 0,31


/*
 * STATUS_SEL.
 * controls the desired status information which is presented in the status
 * regs
 */
#define EMI_STATUS_SEL 0x012f
#define EMI_STATUS_SEL__LENGTH 0x0001

#define EMI_STATUS_SEL__CNTRS_EN_SHIFT 0
#define EMI_STATUS_SEL__CNTRS_EN_WIDTH 1
#define EMI_STATUS_SEL__CNTRS_EN_RESET_VAL 0
#define EMI_STATUS_SEL__CNTRS_EN_RMASK 0x1
#define EMI_STATUS_SEL__CNTRS_EN_MASK  0x1
#define EMI_STATUS_SEL__CNTRS_EN_FIELD 0,0

#define EMI_STATUS_SEL__CMD_TYPE_SHIFT 4
#define EMI_STATUS_SEL__CMD_TYPE_WIDTH 1
#define EMI_STATUS_SEL__CMD_TYPE_RESET_VAL 0
#define EMI_STATUS_SEL__CMD_TYPE_RMASK 0x1
#define EMI_STATUS_SEL__CMD_TYPE_MASK  0x10
#define EMI_STATUS_SEL__CMD_TYPE_FIELD 4,4

#define EMI_STATUS_SEL__DATA_SHIFT 8
#define EMI_STATUS_SEL__DATA_WIDTH 4
#define EMI_STATUS_SEL__DATA_RESET_VAL 0
#define EMI_STATUS_SEL__DATA_RMASK 0xf
#define EMI_STATUS_SEL__DATA_MASK  0xf00
#define EMI_STATUS_SEL__DATA_FIELD 8,11

#define EMI_STATUS_SEL__LOW_STREAMID_SHIFT 16
#define EMI_STATUS_SEL__LOW_STREAMID_WIDTH 8
#define EMI_STATUS_SEL__LOW_STREAMID_RESET_VAL 0
#define EMI_STATUS_SEL__LOW_STREAMID_RMASK 0xff
#define EMI_STATUS_SEL__LOW_STREAMID_MASK  0xff0000
#define EMI_STATUS_SEL__LOW_STREAMID_FIELD 16,23

#define EMI_STATUS_SEL__HIGH_STREAMID_SHIFT 24
#define EMI_STATUS_SEL__HIGH_STREAMID_WIDTH 8
#define EMI_STATUS_SEL__HIGH_STREAMID_RESET_VAL 0
#define EMI_STATUS_SEL__HIGH_STREAMID_RMASK 0xff
#define EMI_STATUS_SEL__HIGH_STREAMID_MASK  0xff000000
#define EMI_STATUS_SEL__HIGH_STREAMID_FIELD 24,31


/*
 * AVG_LAT_CALC.
 * Average latency calculation mode register
 */
#define EMI_AVG_LAT_CALC 0x01af
#define EMI_AVG_LAT_CALC__LENGTH 0x0001

#define EMI_AVG_LAT_CALC__LAT_DELTA_AND_DEV_LFSR_SEED_SHIFT 16
#define EMI_AVG_LAT_CALC__LAT_DELTA_AND_DEV_LFSR_SEED_WIDTH 16
#define EMI_AVG_LAT_CALC__LAT_DELTA_AND_DEV_LFSR_SEED_RESET_VAL 31566
#define EMI_AVG_LAT_CALC__LAT_DELTA_AND_DEV_LFSR_SEED_RMASK 0xffff
#define EMI_AVG_LAT_CALC__LAT_DELTA_AND_DEV_LFSR_SEED_MASK  0xffff0000
#define EMI_AVG_LAT_CALC__LAT_DELTA_AND_DEV_LFSR_SEED_FIELD 16,31


/*
 * HP_LATENCY.
 * High Priority Latency Configuration Register
 */
#define EMI_HP_LATENCY 0x01b0
#define EMI_HP_LATENCY__LENGTH 0x0001

#define EMI_HP_LATENCY__EN_SHIFT 0
#define EMI_HP_LATENCY__EN_WIDTH 1
#define EMI_HP_LATENCY__EN_RESET_VAL 1
#define EMI_HP_LATENCY__EN_RMASK 0x1
#define EMI_HP_LATENCY__EN_MASK  0x1
#define EMI_HP_LATENCY__EN_FIELD 0,0

#define EMI_HP_LATENCY__RES_SHIFT 8
#define EMI_HP_LATENCY__RES_WIDTH 8
#define EMI_HP_LATENCY__RES_RESET_VAL 64
#define EMI_HP_LATENCY__RES_RMASK 0xff
#define EMI_HP_LATENCY__RES_MASK  0xff00
#define EMI_HP_LATENCY__RES_FIELD 8,15

#define EMI_HP_LATENCY__TH_WR_SHIFT 16
#define EMI_HP_LATENCY__TH_WR_WIDTH 8
#define EMI_HP_LATENCY__TH_WR_RESET_VAL 10
#define EMI_HP_LATENCY__TH_WR_RMASK 0xff
#define EMI_HP_LATENCY__TH_WR_MASK  0xff0000
#define EMI_HP_LATENCY__TH_WR_FIELD 16,23

#define EMI_HP_LATENCY__TH_RD_SHIFT 24
#define EMI_HP_LATENCY__TH_RD_WIDTH 8
#define EMI_HP_LATENCY__TH_RD_RESET_VAL 10
#define EMI_HP_LATENCY__TH_RD_RMASK 0xff
#define EMI_HP_LATENCY__TH_RD_MASK  0xff000000
#define EMI_HP_LATENCY__TH_RD_FIELD 24,31


/*
 * SM_RD_WR_WIN.
 * TH1 and TH2 for the RD/WR SM
 */
#define EMI_SM_RD_WR_WIN 0x01c2
#define EMI_SM_RD_WR_WIN__LENGTH 0x0001

#define EMI_SM_RD_WR_WIN__RD_THR1_SHIFT 0
#define EMI_SM_RD_WR_WIN__RD_THR1_WIDTH 8
#define EMI_SM_RD_WR_WIN__RD_THR1_RESET_VAL 60
#define EMI_SM_RD_WR_WIN__RD_THR1_RMASK 0xff
#define EMI_SM_RD_WR_WIN__RD_THR1_MASK  0xff
#define EMI_SM_RD_WR_WIN__RD_THR1_FIELD 0,7

#define EMI_SM_RD_WR_WIN__RD_THR2_SHIFT 8
#define EMI_SM_RD_WR_WIN__RD_THR2_WIDTH 8
#define EMI_SM_RD_WR_WIN__RD_THR2_RESET_VAL 120
#define EMI_SM_RD_WR_WIN__RD_THR2_RMASK 0xff
#define EMI_SM_RD_WR_WIN__RD_THR2_MASK  0xff00
#define EMI_SM_RD_WR_WIN__RD_THR2_FIELD 8,15

#define EMI_SM_RD_WR_WIN__WR_THR1_SHIFT 16
#define EMI_SM_RD_WR_WIN__WR_THR1_WIDTH 8
#define EMI_SM_RD_WR_WIN__WR_THR1_RESET_VAL 40
#define EMI_SM_RD_WR_WIN__WR_THR1_RMASK 0xff
#define EMI_SM_RD_WR_WIN__WR_THR1_MASK  0xff0000
#define EMI_SM_RD_WR_WIN__WR_THR1_FIELD 16,23

#define EMI_SM_RD_WR_WIN__WR_THR2_SHIFT 24
#define EMI_SM_RD_WR_WIN__WR_THR2_WIDTH 8
#define EMI_SM_RD_WR_WIN__WR_THR2_RESET_VAL 60
#define EMI_SM_RD_WR_WIN__WR_THR2_RMASK 0xff
#define EMI_SM_RD_WR_WIN__WR_THR2_MASK  0xff000000
#define EMI_SM_RD_WR_WIN__WR_THR2_FIELD 24,31


/*
 * WR_REQ_SM.
 * Low and MED thresholds used for translating latched fullness(accumulative
 * sizes of all wr requests) as part of the RD-WR profile selection.
 */
#define EMI_WR_REQ_SM 0x01c3
#define EMI_WR_REQ_SM__LENGTH 0x0001

#define EMI_WR_REQ_SM__LOW_LOAD_THR_SHIFT 0
#define EMI_WR_REQ_SM__LOW_LOAD_THR_WIDTH 8
#define EMI_WR_REQ_SM__LOW_LOAD_THR_RESET_VAL 15
#define EMI_WR_REQ_SM__LOW_LOAD_THR_RMASK 0xff
#define EMI_WR_REQ_SM__LOW_LOAD_THR_MASK  0xff
#define EMI_WR_REQ_SM__LOW_LOAD_THR_FIELD 0,7

#define EMI_WR_REQ_SM__MED_LOAD_THR_SHIFT 8
#define EMI_WR_REQ_SM__MED_LOAD_THR_WIDTH 8
#define EMI_WR_REQ_SM__MED_LOAD_THR_RESET_VAL 30
#define EMI_WR_REQ_SM__MED_LOAD_THR_RMASK 0xff
#define EMI_WR_REQ_SM__MED_LOAD_THR_MASK  0xff00
#define EMI_WR_REQ_SM__MED_LOAD_THR_FIELD 8,15

#define EMI_WR_REQ_SM__POOR_BANK_DISTR_SHIFT 16
#define EMI_WR_REQ_SM__POOR_BANK_DISTR_WIDTH 9
#define EMI_WR_REQ_SM__POOR_BANK_DISTR_RESET_VAL 6
#define EMI_WR_REQ_SM__POOR_BANK_DISTR_RMASK 0x1ff
#define EMI_WR_REQ_SM__POOR_BANK_DISTR_MASK  0x1ff0000
#define EMI_WR_REQ_SM__POOR_BANK_DISTR_FIELD 16,24


/*
 * RD_REQ_SM.
 * Low and MED thresholds used for translating latched fullness(accumulative
 * sizes of all rd requests) as part of the RD-WR profile selection.
 */
#define EMI_RD_REQ_SM 0x01c4
#define EMI_RD_REQ_SM__LENGTH 0x0001

#define EMI_RD_REQ_SM__LOW_LOAD_THR_SHIFT 0
#define EMI_RD_REQ_SM__LOW_LOAD_THR_WIDTH 8
#define EMI_RD_REQ_SM__LOW_LOAD_THR_RESET_VAL 15
#define EMI_RD_REQ_SM__LOW_LOAD_THR_RMASK 0xff
#define EMI_RD_REQ_SM__LOW_LOAD_THR_MASK  0xff
#define EMI_RD_REQ_SM__LOW_LOAD_THR_FIELD 0,7

#define EMI_RD_REQ_SM__MED_LOAD_THR_SHIFT 8
#define EMI_RD_REQ_SM__MED_LOAD_THR_WIDTH 8
#define EMI_RD_REQ_SM__MED_LOAD_THR_RESET_VAL 30
#define EMI_RD_REQ_SM__MED_LOAD_THR_RMASK 0xff
#define EMI_RD_REQ_SM__MED_LOAD_THR_MASK  0xff00
#define EMI_RD_REQ_SM__MED_LOAD_THR_FIELD 8,15

#define EMI_RD_REQ_SM__POOR_BANK_DISTR_SHIFT 16
#define EMI_RD_REQ_SM__POOR_BANK_DISTR_WIDTH 9
#define EMI_RD_REQ_SM__POOR_BANK_DISTR_RESET_VAL 6
#define EMI_RD_REQ_SM__POOR_BANK_DISTR_RMASK 0x1ff
#define EMI_RD_REQ_SM__POOR_BANK_DISTR_MASK  0x1ff0000
#define EMI_RD_REQ_SM__POOR_BANK_DISTR_FIELD 16,24


/*
 * WR_RD_REQ_SM.
 * High thresholds used for translating latched fullness(accumulative sizes
 * of all rd requests) as part of the RD-WR profile selection.
 */
#define EMI_WR_RD_REQ_SM 0x01c5
#define EMI_WR_RD_REQ_SM__LENGTH 0x0001

#define EMI_WR_RD_REQ_SM__HIGH_LOAD_THR_RD_SHIFT 0
#define EMI_WR_RD_REQ_SM__HIGH_LOAD_THR_RD_WIDTH 8
#define EMI_WR_RD_REQ_SM__HIGH_LOAD_THR_RD_RESET_VAL 60
#define EMI_WR_RD_REQ_SM__HIGH_LOAD_THR_RD_RMASK 0xff
#define EMI_WR_RD_REQ_SM__HIGH_LOAD_THR_RD_MASK  0xff
#define EMI_WR_RD_REQ_SM__HIGH_LOAD_THR_RD_FIELD 0,7

#define EMI_WR_RD_REQ_SM__HIGH_LOAD_THR_WR_SHIFT 8
#define EMI_WR_RD_REQ_SM__HIGH_LOAD_THR_WR_WIDTH 8
#define EMI_WR_RD_REQ_SM__HIGH_LOAD_THR_WR_RESET_VAL 60
#define EMI_WR_RD_REQ_SM__HIGH_LOAD_THR_WR_RMASK 0xff
#define EMI_WR_RD_REQ_SM__HIGH_LOAD_THR_WR_MASK  0xff00
#define EMI_WR_RD_REQ_SM__HIGH_LOAD_THR_WR_FIELD 8,15

#define EMI_WR_RD_REQ_SM__MIN_WIN_TH_SHIFT 16
#define EMI_WR_RD_REQ_SM__MIN_WIN_TH_WIDTH 5
#define EMI_WR_RD_REQ_SM__MIN_WIN_TH_RESET_VAL 0
#define EMI_WR_RD_REQ_SM__MIN_WIN_TH_RMASK 0x1f
#define EMI_WR_RD_REQ_SM__MIN_WIN_TH_MASK  0x1f0000
#define EMI_WR_RD_REQ_SM__MIN_WIN_TH_FIELD 16,20

#define EMI_WR_RD_REQ_SM__LEFTOVER_EN_SHIFT 24
#define EMI_WR_RD_REQ_SM__LEFTOVER_EN_WIDTH 1
#define EMI_WR_RD_REQ_SM__LEFTOVER_EN_RESET_VAL 1
#define EMI_WR_RD_REQ_SM__LEFTOVER_EN_RMASK 0x1
#define EMI_WR_RD_REQ_SM__LEFTOVER_EN_MASK  0x1000000
#define EMI_WR_RD_REQ_SM__LEFTOVER_EN_FIELD 24,24

#define EMI_WR_RD_REQ_SM__CONSIDER_LOW_LOAD_SHIFT 28
#define EMI_WR_RD_REQ_SM__CONSIDER_LOW_LOAD_WIDTH 1
#define EMI_WR_RD_REQ_SM__CONSIDER_LOW_LOAD_RESET_VAL 1
#define EMI_WR_RD_REQ_SM__CONSIDER_LOW_LOAD_RMASK 0x1
#define EMI_WR_RD_REQ_SM__CONSIDER_LOW_LOAD_MASK  0x10000000
#define EMI_WR_RD_REQ_SM__CONSIDER_LOW_LOAD_FIELD 28,28


/*
 * RD_WR_SM_4TO2.
 * Global (rd / wr) table used to translate the latched fullness and latency
 * status of all RD/WR sorters to 2 bits later used for RD-WR profile
 * selection.
 */
#define EMI_RD_WR_SM_4TO2 0x01c6
#define EMI_RD_WR_SM_4TO2__LENGTH 0x0001

#define EMI_RD_WR_SM_4TO2__MAP_SHIFT 0
#define EMI_RD_WR_SM_4TO2__MAP_WIDTH 32
#define EMI_RD_WR_SM_4TO2__MAP_RESET_VAL 4175749344
#define EMI_RD_WR_SM_4TO2__MAP_RMASK 0xffffffff
#define EMI_RD_WR_SM_4TO2__MAP_MASK  0xffffffff
#define EMI_RD_WR_SM_4TO2__MAP_FIELD 0,31


/*
 * RD_WR_SM_PROF_INDEX0.
 * Global (RD/WR) table used to translate the combined RD/WR priorities to 1
 * of 8 profile indexes.
 * In this register exists the first 10 entries of the table.
 */
#define EMI_RD_WR_SM_PROF_INDEX0 0x01c7
#define EMI_RD_WR_SM_PROF_INDEX0__LENGTH 0x0001

#define EMI_RD_WR_SM_PROF_INDEX0__MAP_SHIFT 0
#define EMI_RD_WR_SM_PROF_INDEX0__MAP_WIDTH 30
#define EMI_RD_WR_SM_PROF_INDEX0__MAP_RESET_VAL 778982044
#define EMI_RD_WR_SM_PROF_INDEX0__MAP_RMASK 0x3fffffff
#define EMI_RD_WR_SM_PROF_INDEX0__MAP_MASK  0x3fffffff
#define EMI_RD_WR_SM_PROF_INDEX0__MAP_FIELD 0,29


/*
 * RD_WR_SM_PROF_INDEX1.
 * Global (RD/WR) table used to translate the combined RD/WR priorities to 1
 * of 8 profile indexes.
 * In this register exists the last 6 entries of the table.
 */
#define EMI_RD_WR_SM_PROF_INDEX1 0x01c8
#define EMI_RD_WR_SM_PROF_INDEX1__LENGTH 0x0001

#define EMI_RD_WR_SM_PROF_INDEX1__MAP_SHIFT 0
#define EMI_RD_WR_SM_PROF_INDEX1__MAP_WIDTH 18
#define EMI_RD_WR_SM_PROF_INDEX1__MAP_RESET_VAL 155100
#define EMI_RD_WR_SM_PROF_INDEX1__MAP_RMASK 0x3ffff
#define EMI_RD_WR_SM_PROF_INDEX1__MAP_MASK  0x3ffff
#define EMI_RD_WR_SM_PROF_INDEX1__MAP_FIELD 0,17


/*
 * RD_WR_SM_STARVATION_PREVENTION.
 * highest possible starvation duration
 */
#define EMI_RD_WR_SM_STARVATION_PREVENTION 0x01c9
#define EMI_RD_WR_SM_STARVATION_PREVENTION__LENGTH 0x0001

#define EMI_RD_WR_SM_STARVATION_PREVENTION__RD_TH4_SHIFT 0
#define EMI_RD_WR_SM_STARVATION_PREVENTION__RD_TH4_WIDTH 13
#define EMI_RD_WR_SM_STARVATION_PREVENTION__RD_TH4_RESET_VAL 3000
#define EMI_RD_WR_SM_STARVATION_PREVENTION__RD_TH4_RMASK 0x1fff
#define EMI_RD_WR_SM_STARVATION_PREVENTION__RD_TH4_MASK  0x1fff
#define EMI_RD_WR_SM_STARVATION_PREVENTION__RD_TH4_FIELD 0,12

#define EMI_RD_WR_SM_STARVATION_PREVENTION__WR_TH4_SHIFT 16
#define EMI_RD_WR_SM_STARVATION_PREVENTION__WR_TH4_WIDTH 13
#define EMI_RD_WR_SM_STARVATION_PREVENTION__WR_TH4_RESET_VAL 500
#define EMI_RD_WR_SM_STARVATION_PREVENTION__WR_TH4_RMASK 0x1fff
#define EMI_RD_WR_SM_STARVATION_PREVENTION__WR_TH4_MASK  0x1fff0000
#define EMI_RD_WR_SM_STARVATION_PREVENTION__WR_TH4_FIELD 16,28


/*
 * UTIL_DEGRADATION.
 * utility degradation feature - aimed to reduce DDR power consumption
 */
#define EMI_UTIL_DEGRADATION 0x01fb
#define EMI_UTIL_DEGRADATION__LENGTH 0x0001

#define EMI_UTIL_DEGRADATION__EN_SHIFT 0
#define EMI_UTIL_DEGRADATION__EN_WIDTH 1
#define EMI_UTIL_DEGRADATION__EN_RESET_VAL 0
#define EMI_UTIL_DEGRADATION__EN_RMASK 0x1
#define EMI_UTIL_DEGRADATION__EN_MASK  0x1
#define EMI_UTIL_DEGRADATION__EN_FIELD 0,0

#define EMI_UTIL_DEGRADATION__LONG_EN_SHIFT 1
#define EMI_UTIL_DEGRADATION__LONG_EN_WIDTH 1
#define EMI_UTIL_DEGRADATION__LONG_EN_RESET_VAL 0
#define EMI_UTIL_DEGRADATION__LONG_EN_RMASK 0x1
#define EMI_UTIL_DEGRADATION__LONG_EN_MASK  0x2
#define EMI_UTIL_DEGRADATION__LONG_EN_FIELD 1,1

#define EMI_UTIL_DEGRADATION__MAX_TOKENS_SHIFT 13
#define EMI_UTIL_DEGRADATION__MAX_TOKENS_WIDTH 19
#define EMI_UTIL_DEGRADATION__MAX_TOKENS_RESET_VAL 39559
#define EMI_UTIL_DEGRADATION__MAX_TOKENS_RMASK 0x7ffff
#define EMI_UTIL_DEGRADATION__MAX_TOKENS_MASK  0xffffe000
#define EMI_UTIL_DEGRADATION__MAX_TOKENS_FIELD 13,31


/*
 * UTIL_DEGRADATION_BUCKET_INCREMENT.
 * utility degradation feature - bucket tokens incremetation related
 * configurations
 */
#define EMI_UTIL_DEGRADATION_BUCKET_INCREMENT 0x01fc
#define EMI_UTIL_DEGRADATION_BUCKET_INCREMENT__LENGTH 0x0001

#define EMI_UTIL_DEGRADATION_BUCKET_INCREMENT__COUNTDOWN_SHIFT 0
#define EMI_UTIL_DEGRADATION_BUCKET_INCREMENT__COUNTDOWN_WIDTH 15
#define EMI_UTIL_DEGRADATION_BUCKET_INCREMENT__COUNTDOWN_RESET_VAL 54
#define EMI_UTIL_DEGRADATION_BUCKET_INCREMENT__COUNTDOWN_RMASK 0x7fff
#define EMI_UTIL_DEGRADATION_BUCKET_INCREMENT__COUNTDOWN_MASK  0x7fff
#define EMI_UTIL_DEGRADATION_BUCKET_INCREMENT__COUNTDOWN_FIELD 0,14

#define EMI_UTIL_DEGRADATION_BUCKET_INCREMENT__TOKEN_NUM_SHIFT 15
#define EMI_UTIL_DEGRADATION_BUCKET_INCREMENT__TOKEN_NUM_WIDTH 17
#define EMI_UTIL_DEGRADATION_BUCKET_INCREMENT__TOKEN_NUM_RESET_VAL 103
#define EMI_UTIL_DEGRADATION_BUCKET_INCREMENT__TOKEN_NUM_RMASK 0x1ffff
#define EMI_UTIL_DEGRADATION_BUCKET_INCREMENT__TOKEN_NUM_MASK  0xffff8000
#define EMI_UTIL_DEGRADATION_BUCKET_INCREMENT__TOKEN_NUM_FIELD 15,31


/*
 * INTR_CAUSE.
 * There are 3 interrupt causes:
 * 1. Flush Error Interrupt
 * 2. Row Hammer
 * 3. Page Boundary Exception (only relevant when working in Single SDRAM
 * mode).
 * 4. Rank Error Interrupt
 */
#define EMI_INTR_CAUSE 0x01fe
#define EMI_INTR_CAUSE__LENGTH 0x0001

#define EMI_INTR_CAUSE__FLUSH_ERR_SHIFT 0
#define EMI_INTR_CAUSE__FLUSH_ERR_WIDTH 1
#define EMI_INTR_CAUSE__FLUSH_ERR_RESET_VAL 0
#define EMI_INTR_CAUSE__FLUSH_ERR_RMASK 0x1
#define EMI_INTR_CAUSE__FLUSH_ERR_MASK  0x1
#define EMI_INTR_CAUSE__FLUSH_ERR_FIELD 0,0

#define EMI_INTR_CAUSE__ROW_HAMMER_SHIFT 4
#define EMI_INTR_CAUSE__ROW_HAMMER_WIDTH 1
#define EMI_INTR_CAUSE__ROW_HAMMER_RESET_VAL 0
#define EMI_INTR_CAUSE__ROW_HAMMER_RMASK 0x1
#define EMI_INTR_CAUSE__ROW_HAMMER_MASK  0x10
#define EMI_INTR_CAUSE__ROW_HAMMER_FIELD 4,4

#define EMI_INTR_CAUSE__PAGE_BOUND_EXCPT_SHIFT 8
#define EMI_INTR_CAUSE__PAGE_BOUND_EXCPT_WIDTH 1
#define EMI_INTR_CAUSE__PAGE_BOUND_EXCPT_RESET_VAL 0
#define EMI_INTR_CAUSE__PAGE_BOUND_EXCPT_RMASK 0x1
#define EMI_INTR_CAUSE__PAGE_BOUND_EXCPT_MASK  0x100
#define EMI_INTR_CAUSE__PAGE_BOUND_EXCPT_FIELD 8,8

#define EMI_INTR_CAUSE__ALERT_SHIFT 12
#define EMI_INTR_CAUSE__ALERT_WIDTH 1
#define EMI_INTR_CAUSE__ALERT_RESET_VAL 0
#define EMI_INTR_CAUSE__ALERT_RMASK 0x1
#define EMI_INTR_CAUSE__ALERT_MASK  0x1000
#define EMI_INTR_CAUSE__ALERT_FIELD 12,12

#define EMI_INTR_CAUSE__RANK_ERR_SHIFT 16
#define EMI_INTR_CAUSE__RANK_ERR_WIDTH 1
#define EMI_INTR_CAUSE__RANK_ERR_RESET_VAL 0
#define EMI_INTR_CAUSE__RANK_ERR_RMASK 0x1
#define EMI_INTR_CAUSE__RANK_ERR_MASK  0x10000
#define EMI_INTR_CAUSE__RANK_ERR_FIELD 16,16


/*
 * INTR_MASK.
 * There are 3 interrupt causes:
 * 1. Flush Error Interrupt
 * 2. Row Hammer
 * 3. Page Boundary Exception (only relevant when working in Single SDRAM
 * mode).
 * 4. Rank Error Interrupt
 */
#define EMI_INTR_MASK 0x01ff
#define EMI_INTR_MASK__LENGTH 0x0001

#define EMI_INTR_MASK__FLUSH_ERR_SHIFT 0
#define EMI_INTR_MASK__FLUSH_ERR_WIDTH 1
#define EMI_INTR_MASK__FLUSH_ERR_RESET_VAL 0
#define EMI_INTR_MASK__FLUSH_ERR_RMASK 0x1
#define EMI_INTR_MASK__FLUSH_ERR_MASK  0x1
#define EMI_INTR_MASK__FLUSH_ERR_FIELD 0,0

#define EMI_INTR_MASK__ROW_HAMMER_SHIFT 4
#define EMI_INTR_MASK__ROW_HAMMER_WIDTH 1
#define EMI_INTR_MASK__ROW_HAMMER_RESET_VAL 0
#define EMI_INTR_MASK__ROW_HAMMER_RMASK 0x1
#define EMI_INTR_MASK__ROW_HAMMER_MASK  0x10
#define EMI_INTR_MASK__ROW_HAMMER_FIELD 4,4

#define EMI_INTR_MASK__PAGE_BOUND_EXCPT_SHIFT 8
#define EMI_INTR_MASK__PAGE_BOUND_EXCPT_WIDTH 1
#define EMI_INTR_MASK__PAGE_BOUND_EXCPT_RESET_VAL 0
#define EMI_INTR_MASK__PAGE_BOUND_EXCPT_RMASK 0x1
#define EMI_INTR_MASK__PAGE_BOUND_EXCPT_MASK  0x100
#define EMI_INTR_MASK__PAGE_BOUND_EXCPT_FIELD 8,8

#define EMI_INTR_MASK__ALERT_SHIFT 12
#define EMI_INTR_MASK__ALERT_WIDTH 1
#define EMI_INTR_MASK__ALERT_RESET_VAL 0
#define EMI_INTR_MASK__ALERT_RMASK 0x1
#define EMI_INTR_MASK__ALERT_MASK  0x1000
#define EMI_INTR_MASK__ALERT_FIELD 12,12

#define EMI_INTR_MASK__RANK_ERR_SHIFT 16
#define EMI_INTR_MASK__RANK_ERR_WIDTH 1
#define EMI_INTR_MASK__RANK_ERR_RESET_VAL 0
#define EMI_INTR_MASK__RANK_ERR_RMASK 0x1
#define EMI_INTR_MASK__RANK_ERR_MASK  0x10000
#define EMI_INTR_MASK__RANK_ERR_FIELD 16,16


/*
 * DRAM_ERR_DATA.
 * These registers capture the data of the transaction in which an ECC error
 * has occurred.
 */
#define EMI_DRAM_ERR_DATA__FIRST_WORD 0x0200
#define EMI_DRAM_ERR_DATA__LAST_WORD 0x0211
#define EMI_DRAM_ERR_DATA__LENGTH 0x0012
#define EMI_DRAM_ERR_DATA__STRIDE 0x0001

#define EMI_DRAM_ERR_DATA__VAL_SHIFT 0
#define EMI_DRAM_ERR_DATA__VAL_WIDTH 32
#define EMI_DRAM_ERR_DATA__VAL_RESET_VAL 0
#define EMI_DRAM_ERR_DATA__VAL_RMASK 0xffffffff
#define EMI_DRAM_ERR_DATA__VAL_MASK  0xffffffff
#define EMI_DRAM_ERR_DATA__VAL_FIELD 0,31


/*
 * FLUSH_ERR_CNTR.
 * Counts the number of requests which arrived to the EMI during Flush
 * activity
 */
#define EMI_FLUSH_ERR_CNTR 0x0221
#define EMI_FLUSH_ERR_CNTR__LENGTH 0x0001

#define EMI_FLUSH_ERR_CNTR__VAL_SHIFT 0
#define EMI_FLUSH_ERR_CNTR__VAL_WIDTH 32
#define EMI_FLUSH_ERR_CNTR__VAL_RESET_VAL 0
#define EMI_FLUSH_ERR_CNTR__VAL_RMASK 0xffffffff
#define EMI_FLUSH_ERR_CNTR__VAL_MASK  0xffffffff
#define EMI_FLUSH_ERR_CNTR__VAL_FIELD 0,31


/*
 * FLUSH_ERR_INFO_0.
 * This registers holds the controls for the first requests which arrived
 * during Flush activity
 */
#define EMI_FLUSH_ERR_INFO_0 0x0222
#define EMI_FLUSH_ERR_INFO_0__LENGTH 0x0001

#define EMI_FLUSH_ERR_INFO_0__ADDR_SHIFT 0
#define EMI_FLUSH_ERR_INFO_0__ADDR_WIDTH 32
#define EMI_FLUSH_ERR_INFO_0__ADDR_RESET_VAL 0
#define EMI_FLUSH_ERR_INFO_0__ADDR_RMASK 0xffffffff
#define EMI_FLUSH_ERR_INFO_0__ADDR_MASK  0xffffffff
#define EMI_FLUSH_ERR_INFO_0__ADDR_FIELD 0,31


/*
 * FLUSH_ERR_INFO_1.
 * This registers holds the controls for the first requests which arrived
 * during Flush activity
 */
#define EMI_FLUSH_ERR_INFO_1 0x0223
#define EMI_FLUSH_ERR_INFO_1__LENGTH 0x0001

#define EMI_FLUSH_ERR_INFO_1__ADDR_SHIFT 0
#define EMI_FLUSH_ERR_INFO_1__ADDR_WIDTH 2
#define EMI_FLUSH_ERR_INFO_1__ADDR_RESET_VAL 0
#define EMI_FLUSH_ERR_INFO_1__ADDR_RMASK 0x3
#define EMI_FLUSH_ERR_INFO_1__ADDR_MASK  0x3
#define EMI_FLUSH_ERR_INFO_1__ADDR_FIELD 0,1

#define EMI_FLUSH_ERR_INFO_1__CMD_SHIFT 8
#define EMI_FLUSH_ERR_INFO_1__CMD_WIDTH 1
#define EMI_FLUSH_ERR_INFO_1__CMD_RESET_VAL 0
#define EMI_FLUSH_ERR_INFO_1__CMD_RMASK 0x1
#define EMI_FLUSH_ERR_INFO_1__CMD_MASK  0x100
#define EMI_FLUSH_ERR_INFO_1__CMD_FIELD 8,8


/*
 * FLUSH_ERR_INFO_2.
 * This registers holds the controls for the first requests which arrived
 * during Flush activity
 */
#define EMI_FLUSH_ERR_INFO_2 0x0224
#define EMI_FLUSH_ERR_INFO_2__LENGTH 0x0001

#define EMI_FLUSH_ERR_INFO_2__STREAM_ID_SHIFT 0
#define EMI_FLUSH_ERR_INFO_2__STREAM_ID_WIDTH 8
#define EMI_FLUSH_ERR_INFO_2__STREAM_ID_RESET_VAL 0
#define EMI_FLUSH_ERR_INFO_2__STREAM_ID_RMASK 0xff
#define EMI_FLUSH_ERR_INFO_2__STREAM_ID_MASK  0xff
#define EMI_FLUSH_ERR_INFO_2__STREAM_ID_FIELD 0,7

#define EMI_FLUSH_ERR_INFO_2__USR_FREE_SHIFT 8
#define EMI_FLUSH_ERR_INFO_2__USR_FREE_WIDTH 24
#define EMI_FLUSH_ERR_INFO_2__USR_FREE_RESET_VAL 0
#define EMI_FLUSH_ERR_INFO_2__USR_FREE_RMASK 0xffffff
#define EMI_FLUSH_ERR_INFO_2__USR_FREE_MASK  0xffffff00
#define EMI_FLUSH_ERR_INFO_2__USR_FREE_FIELD 8,31


/*
 * PAGE_BOUND_EXCPT_INFO_0.
 * This register holds the info of the exception boundary request
 */
#define EMI_PAGE_BOUND_EXCPT_INFO_0 0x0230
#define EMI_PAGE_BOUND_EXCPT_INFO_0__LENGTH 0x0001

#define EMI_PAGE_BOUND_EXCPT_INFO_0__BANK_SHIFT 0
#define EMI_PAGE_BOUND_EXCPT_INFO_0__BANK_WIDTH 4
#define EMI_PAGE_BOUND_EXCPT_INFO_0__BANK_RESET_VAL 0
#define EMI_PAGE_BOUND_EXCPT_INFO_0__BANK_RMASK 0xf
#define EMI_PAGE_BOUND_EXCPT_INFO_0__BANK_MASK  0xf
#define EMI_PAGE_BOUND_EXCPT_INFO_0__BANK_FIELD 0,3

#define EMI_PAGE_BOUND_EXCPT_INFO_0__LR_SHIFT 4
#define EMI_PAGE_BOUND_EXCPT_INFO_0__LR_WIDTH 2
#define EMI_PAGE_BOUND_EXCPT_INFO_0__LR_RESET_VAL 0
#define EMI_PAGE_BOUND_EXCPT_INFO_0__LR_RMASK 0x3
#define EMI_PAGE_BOUND_EXCPT_INFO_0__LR_MASK  0x30
#define EMI_PAGE_BOUND_EXCPT_INFO_0__LR_FIELD 4,5

#define EMI_PAGE_BOUND_EXCPT_INFO_0__CMD_SHIFT 8
#define EMI_PAGE_BOUND_EXCPT_INFO_0__CMD_WIDTH 1
#define EMI_PAGE_BOUND_EXCPT_INFO_0__CMD_RESET_VAL 0
#define EMI_PAGE_BOUND_EXCPT_INFO_0__CMD_RMASK 0x1
#define EMI_PAGE_BOUND_EXCPT_INFO_0__CMD_MASK  0x100
#define EMI_PAGE_BOUND_EXCPT_INFO_0__CMD_FIELD 8,8


/*
 * PAGE_BOUND_EXCPT_INFO_1.
 * This register holds the info of the exception boundary request
 */
#define EMI_PAGE_BOUND_EXCPT_INFO_1 0x0231
#define EMI_PAGE_BOUND_EXCPT_INFO_1__LENGTH 0x0001

#define EMI_PAGE_BOUND_EXCPT_INFO_1__ADDR_SHIFT 0
#define EMI_PAGE_BOUND_EXCPT_INFO_1__ADDR_WIDTH 25
#define EMI_PAGE_BOUND_EXCPT_INFO_1__ADDR_RESET_VAL 0
#define EMI_PAGE_BOUND_EXCPT_INFO_1__ADDR_RMASK 0x1ffffff
#define EMI_PAGE_BOUND_EXCPT_INFO_1__ADDR_MASK  0x1ffffff
#define EMI_PAGE_BOUND_EXCPT_INFO_1__ADDR_FIELD 0,24


/*
 * RANK_ERR_INFO_0.
 * This registers holds the controls for the request which arrived to a non
 * active rank
 */
#define EMI_RANK_ERR_INFO_0 0x0235
#define EMI_RANK_ERR_INFO_0__LENGTH 0x0001

#define EMI_RANK_ERR_INFO_0__ADDR_SHIFT 0
#define EMI_RANK_ERR_INFO_0__ADDR_WIDTH 32
#define EMI_RANK_ERR_INFO_0__ADDR_RESET_VAL 0
#define EMI_RANK_ERR_INFO_0__ADDR_RMASK 0xffffffff
#define EMI_RANK_ERR_INFO_0__ADDR_MASK  0xffffffff
#define EMI_RANK_ERR_INFO_0__ADDR_FIELD 0,31


/*
 * RANK_ERR_INFO_1.
 * This registers holds the controls for the request which arrived to a non
 * active rank
 */
#define EMI_RANK_ERR_INFO_1 0x0236
#define EMI_RANK_ERR_INFO_1__LENGTH 0x0001

#define EMI_RANK_ERR_INFO_1__ADDR_SHIFT 0
#define EMI_RANK_ERR_INFO_1__ADDR_WIDTH 2
#define EMI_RANK_ERR_INFO_1__ADDR_RESET_VAL 0
#define EMI_RANK_ERR_INFO_1__ADDR_RMASK 0x3
#define EMI_RANK_ERR_INFO_1__ADDR_MASK  0x3
#define EMI_RANK_ERR_INFO_1__ADDR_FIELD 0,1

#define EMI_RANK_ERR_INFO_1__CMD_SHIFT 8
#define EMI_RANK_ERR_INFO_1__CMD_WIDTH 1
#define EMI_RANK_ERR_INFO_1__CMD_RESET_VAL 0
#define EMI_RANK_ERR_INFO_1__CMD_RMASK 0x1
#define EMI_RANK_ERR_INFO_1__CMD_MASK  0x100
#define EMI_RANK_ERR_INFO_1__CMD_FIELD 8,8


/*
 * RANK_ERR_INFO_2.
 * This registers holds the controls for the request which arrived to a non
 * active rank
 */
#define EMI_RANK_ERR_INFO_2 0x0237
#define EMI_RANK_ERR_INFO_2__LENGTH 0x0001

#define EMI_RANK_ERR_INFO_2__STREAM_ID_SHIFT 0
#define EMI_RANK_ERR_INFO_2__STREAM_ID_WIDTH 8
#define EMI_RANK_ERR_INFO_2__STREAM_ID_RESET_VAL 0
#define EMI_RANK_ERR_INFO_2__STREAM_ID_RMASK 0xff
#define EMI_RANK_ERR_INFO_2__STREAM_ID_MASK  0xff
#define EMI_RANK_ERR_INFO_2__STREAM_ID_FIELD 0,7

#define EMI_RANK_ERR_INFO_2__USR_FREE_SHIFT 8
#define EMI_RANK_ERR_INFO_2__USR_FREE_WIDTH 24
#define EMI_RANK_ERR_INFO_2__USR_FREE_RESET_VAL 0
#define EMI_RANK_ERR_INFO_2__USR_FREE_RMASK 0xffffff
#define EMI_RANK_ERR_INFO_2__USR_FREE_MASK  0xffffff00
#define EMI_RANK_ERR_INFO_2__USR_FREE_FIELD 8,31


/*
 * RDWR_SM_PROFILE_0.
 * Parameters of the rdwr sm profiles
 */
#define EMI_RDWR_SM_PROFILE_0 0x0240
#define EMI_RDWR_SM_PROFILE_0__LENGTH 0x0001

#define EMI_RDWR_SM_PROFILE_0__RD_TH3_SHIFT 0
#define EMI_RDWR_SM_PROFILE_0__RD_TH3_WIDTH 9
#define EMI_RDWR_SM_PROFILE_0__RD_TH3_RESET_VAL 162
#define EMI_RDWR_SM_PROFILE_0__RD_TH3_RMASK 0x1ff
#define EMI_RDWR_SM_PROFILE_0__RD_TH3_MASK  0x1ff
#define EMI_RDWR_SM_PROFILE_0__RD_TH3_FIELD 0,8

#define EMI_RDWR_SM_PROFILE_0__WR_TH3_SHIFT 16
#define EMI_RDWR_SM_PROFILE_0__WR_TH3_WIDTH 9
#define EMI_RDWR_SM_PROFILE_0__WR_TH3_RESET_VAL 178
#define EMI_RDWR_SM_PROFILE_0__WR_TH3_RMASK 0x1ff
#define EMI_RDWR_SM_PROFILE_0__WR_TH3_MASK  0x1ff0000
#define EMI_RDWR_SM_PROFILE_0__WR_TH3_FIELD 16,24


/*
 * RDWR_SM_PROFILE_1.
 * Parameters of the rdwr sm profiles
 */
#define EMI_RDWR_SM_PROFILE_1 0x0241
#define EMI_RDWR_SM_PROFILE_1__LENGTH 0x0001

#define EMI_RDWR_SM_PROFILE_1__RD_TH3_SHIFT 0
#define EMI_RDWR_SM_PROFILE_1__RD_TH3_WIDTH 9
#define EMI_RDWR_SM_PROFILE_1__RD_TH3_RESET_VAL 172
#define EMI_RDWR_SM_PROFILE_1__RD_TH3_RMASK 0x1ff
#define EMI_RDWR_SM_PROFILE_1__RD_TH3_MASK  0x1ff
#define EMI_RDWR_SM_PROFILE_1__RD_TH3_FIELD 0,8

#define EMI_RDWR_SM_PROFILE_1__WR_TH3_SHIFT 16
#define EMI_RDWR_SM_PROFILE_1__WR_TH3_WIDTH 9
#define EMI_RDWR_SM_PROFILE_1__WR_TH3_RESET_VAL 168
#define EMI_RDWR_SM_PROFILE_1__WR_TH3_RMASK 0x1ff
#define EMI_RDWR_SM_PROFILE_1__WR_TH3_MASK  0x1ff0000
#define EMI_RDWR_SM_PROFILE_1__WR_TH3_FIELD 16,24


/*
 * RDWR_SM_PROFILE_2.
 * Parameters of the rdwr sm profiles
 */
#define EMI_RDWR_SM_PROFILE_2 0x0242
#define EMI_RDWR_SM_PROFILE_2__LENGTH 0x0001

#define EMI_RDWR_SM_PROFILE_2__RD_TH3_SHIFT 0
#define EMI_RDWR_SM_PROFILE_2__RD_TH3_WIDTH 9
#define EMI_RDWR_SM_PROFILE_2__RD_TH3_RESET_VAL 182
#define EMI_RDWR_SM_PROFILE_2__RD_TH3_RMASK 0x1ff
#define EMI_RDWR_SM_PROFILE_2__RD_TH3_MASK  0x1ff
#define EMI_RDWR_SM_PROFILE_2__RD_TH3_FIELD 0,8

#define EMI_RDWR_SM_PROFILE_2__WR_TH3_SHIFT 16
#define EMI_RDWR_SM_PROFILE_2__WR_TH3_WIDTH 9
#define EMI_RDWR_SM_PROFILE_2__WR_TH3_RESET_VAL 158
#define EMI_RDWR_SM_PROFILE_2__WR_TH3_RMASK 0x1ff
#define EMI_RDWR_SM_PROFILE_2__WR_TH3_MASK  0x1ff0000
#define EMI_RDWR_SM_PROFILE_2__WR_TH3_FIELD 16,24


/*
 * RDWR_SM_PROFILE_3.
 * Parameters of the rdwr sm profiles
 */
#define EMI_RDWR_SM_PROFILE_3 0x0243
#define EMI_RDWR_SM_PROFILE_3__LENGTH 0x0001

#define EMI_RDWR_SM_PROFILE_3__RD_TH3_SHIFT 0
#define EMI_RDWR_SM_PROFILE_3__RD_TH3_WIDTH 9
#define EMI_RDWR_SM_PROFILE_3__RD_TH3_RESET_VAL 192
#define EMI_RDWR_SM_PROFILE_3__RD_TH3_RMASK 0x1ff
#define EMI_RDWR_SM_PROFILE_3__RD_TH3_MASK  0x1ff
#define EMI_RDWR_SM_PROFILE_3__RD_TH3_FIELD 0,8

#define EMI_RDWR_SM_PROFILE_3__WR_TH3_SHIFT 16
#define EMI_RDWR_SM_PROFILE_3__WR_TH3_WIDTH 9
#define EMI_RDWR_SM_PROFILE_3__WR_TH3_RESET_VAL 148
#define EMI_RDWR_SM_PROFILE_3__WR_TH3_RMASK 0x1ff
#define EMI_RDWR_SM_PROFILE_3__WR_TH3_MASK  0x1ff0000
#define EMI_RDWR_SM_PROFILE_3__WR_TH3_FIELD 16,24


/*
 * RDWR_SM_PROFILE_4.
 * Parameters of the rdwr sm profiles
 */
#define EMI_RDWR_SM_PROFILE_4 0x0244
#define EMI_RDWR_SM_PROFILE_4__LENGTH 0x0001

#define EMI_RDWR_SM_PROFILE_4__RD_TH3_SHIFT 0
#define EMI_RDWR_SM_PROFILE_4__RD_TH3_WIDTH 9
#define EMI_RDWR_SM_PROFILE_4__RD_TH3_RESET_VAL 202
#define EMI_RDWR_SM_PROFILE_4__RD_TH3_RMASK 0x1ff
#define EMI_RDWR_SM_PROFILE_4__RD_TH3_MASK  0x1ff
#define EMI_RDWR_SM_PROFILE_4__RD_TH3_FIELD 0,8

#define EMI_RDWR_SM_PROFILE_4__WR_TH3_SHIFT 16
#define EMI_RDWR_SM_PROFILE_4__WR_TH3_WIDTH 9
#define EMI_RDWR_SM_PROFILE_4__WR_TH3_RESET_VAL 138
#define EMI_RDWR_SM_PROFILE_4__WR_TH3_RMASK 0x1ff
#define EMI_RDWR_SM_PROFILE_4__WR_TH3_MASK  0x1ff0000
#define EMI_RDWR_SM_PROFILE_4__WR_TH3_FIELD 16,24


/*
 * RDWR_SM_PROFILE_5.
 * Parameters of the rdwr sm profiles
 */
#define EMI_RDWR_SM_PROFILE_5 0x0245
#define EMI_RDWR_SM_PROFILE_5__LENGTH 0x0001

#define EMI_RDWR_SM_PROFILE_5__RD_TH3_SHIFT 0
#define EMI_RDWR_SM_PROFILE_5__RD_TH3_WIDTH 9
#define EMI_RDWR_SM_PROFILE_5__RD_TH3_RESET_VAL 230
#define EMI_RDWR_SM_PROFILE_5__RD_TH3_RMASK 0x1ff
#define EMI_RDWR_SM_PROFILE_5__RD_TH3_MASK  0x1ff
#define EMI_RDWR_SM_PROFILE_5__RD_TH3_FIELD 0,8

#define EMI_RDWR_SM_PROFILE_5__WR_TH3_SHIFT 16
#define EMI_RDWR_SM_PROFILE_5__WR_TH3_WIDTH 9
#define EMI_RDWR_SM_PROFILE_5__WR_TH3_RESET_VAL 110
#define EMI_RDWR_SM_PROFILE_5__WR_TH3_RMASK 0x1ff
#define EMI_RDWR_SM_PROFILE_5__WR_TH3_MASK  0x1ff0000
#define EMI_RDWR_SM_PROFILE_5__WR_TH3_FIELD 16,24


/*
 * RDWR_SM_PROFILE_6.
 * Parameters of the rdwr sm profiles
 */
#define EMI_RDWR_SM_PROFILE_6 0x0246
#define EMI_RDWR_SM_PROFILE_6__LENGTH 0x0001

#define EMI_RDWR_SM_PROFILE_6__RD_TH3_SHIFT 0
#define EMI_RDWR_SM_PROFILE_6__RD_TH3_WIDTH 9
#define EMI_RDWR_SM_PROFILE_6__RD_TH3_RESET_VAL 250
#define EMI_RDWR_SM_PROFILE_6__RD_TH3_RMASK 0x1ff
#define EMI_RDWR_SM_PROFILE_6__RD_TH3_MASK  0x1ff
#define EMI_RDWR_SM_PROFILE_6__RD_TH3_FIELD 0,8

#define EMI_RDWR_SM_PROFILE_6__WR_TH3_SHIFT 16
#define EMI_RDWR_SM_PROFILE_6__WR_TH3_WIDTH 9
#define EMI_RDWR_SM_PROFILE_6__WR_TH3_RESET_VAL 90
#define EMI_RDWR_SM_PROFILE_6__WR_TH3_RMASK 0x1ff
#define EMI_RDWR_SM_PROFILE_6__WR_TH3_MASK  0x1ff0000
#define EMI_RDWR_SM_PROFILE_6__WR_TH3_FIELD 16,24


/*
 * RDWR_SM_PROFILE_7.
 * Parameters of the rdwr sm profiles
 */
#define EMI_RDWR_SM_PROFILE_7 0x0247
#define EMI_RDWR_SM_PROFILE_7__LENGTH 0x0001

#define EMI_RDWR_SM_PROFILE_7__RD_TH3_SHIFT 0
#define EMI_RDWR_SM_PROFILE_7__RD_TH3_WIDTH 9
#define EMI_RDWR_SM_PROFILE_7__RD_TH3_RESET_VAL 270
#define EMI_RDWR_SM_PROFILE_7__RD_TH3_RMASK 0x1ff
#define EMI_RDWR_SM_PROFILE_7__RD_TH3_MASK  0x1ff
#define EMI_RDWR_SM_PROFILE_7__RD_TH3_FIELD 0,8

#define EMI_RDWR_SM_PROFILE_7__WR_TH3_SHIFT 16
#define EMI_RDWR_SM_PROFILE_7__WR_TH3_WIDTH 9
#define EMI_RDWR_SM_PROFILE_7__WR_TH3_RESET_VAL 70
#define EMI_RDWR_SM_PROFILE_7__WR_TH3_RMASK 0x1ff
#define EMI_RDWR_SM_PROFILE_7__WR_TH3_MASK  0x1ff0000
#define EMI_RDWR_SM_PROFILE_7__WR_TH3_FIELD 16,24


/*
 * ADD_IND_DATA.
 * These registers holds the ECC data for Host Write/Read with ECC Bypass
 * commands.
 */
#define EMI_ADD_IND_DATA__FIRST_WORD 0x0250
#define EMI_ADD_IND_DATA__LAST_WORD 0x0251
#define EMI_ADD_IND_DATA__LENGTH 0x0002
#define EMI_ADD_IND_DATA__STRIDE 0x0001

#define EMI_ADD_IND_DATA__VAL_SHIFT 0
#define EMI_ADD_IND_DATA__VAL_WIDTH 32
#define EMI_ADD_IND_DATA__VAL_RESET_VAL 0
#define EMI_ADD_IND_DATA__VAL_RMASK 0xffffffff
#define EMI_ADD_IND_DATA__VAL_MASK  0xffffffff
#define EMI_ADD_IND_DATA__VAL_FIELD 0,31


/*
 * DEBUG_CTRL.
 * Chicken Bits Registers
 */
#define EMI_DEBUG_CTRL 0x0255
#define EMI_DEBUG_CTRL__LENGTH 0x0001

#define EMI_DEBUG_CTRL__PHY_UPDATE_ACK_FIX_DIS_SHIFT 0
#define EMI_DEBUG_CTRL__PHY_UPDATE_ACK_FIX_DIS_WIDTH 1
#define EMI_DEBUG_CTRL__PHY_UPDATE_ACK_FIX_DIS_RESET_VAL 0
#define EMI_DEBUG_CTRL__PHY_UPDATE_ACK_FIX_DIS_RMASK 0x1
#define EMI_DEBUG_CTRL__PHY_UPDATE_ACK_FIX_DIS_MASK  0x1
#define EMI_DEBUG_CTRL__PHY_UPDATE_ACK_FIX_DIS_FIELD 0,0

#endif /* !defined(__DOXYGEN__) */

#endif /* !defined(__REGS_EMI_DEF_H__) */
