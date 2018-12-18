#ifndef __I2C_SMBUS_H__
#define __I2C_SMBUS_H__

#include <stdint.h>
#include <debug.h>
#include <assert.h>
#include <errno.h>
#include <delay_timer.h>
#include <mmio.h>
#include "bluefield_def.h"
#include "tyu_def.h"

/* Addresses of I2C devices */
#define I2C_SPD_0_ADDR    0x50
#define I2C_SPD_1_ADDR    0x51
#define I2C_SPD_2_ADDR    0x52
#define I2C_SPD_3_ADDR    0x53
/* *FIXME* Update DIMM device addresses */
#define I2C_DIMM_0_ADDR   0x40
#define I2C_DIMM_1_ADDR   0x41
#define I2C_DIMM_2_ADDR   0x42
#define I2C_DIMM_3_ADDR   0x43

/*
 * SPD Device Select Code.
 * This consists of a 4-bit Device Type Idenntifier, and a 3-bit
 * Select Address. To access the EEPROM page address, the 4 upper
 * bits are b'0110. The least significant bit will refer to either
 * I2C Read (1) or I2C Write (0). To Set the EEPROM page address
 * The SPA0 code and SPA1 code must be prepended by 0 (Write). To
 * read the EEPROM page address RPA code must be prepended by
 * 1 (Read). Note that the most significant bit, is sent first.
 */
#define I2C_SPD_SPA0      0x36  /* Set EEPROM Page Address to 0 */
#define I2C_SPD_SPA1      0x37  /* Set EEPROM Page Address to 1 */
#define I2C_SPD_RPA       I2C_SPD_SPA0 /* Read EEPROM Page Address */

/* Size of the SPD EEPROM */
#define I2C_SPD_SIZE      512
/* Page size of the SPD EEPROM */
#define I2C_SPD_PAGE_SIZE 256

/*
 * TYU shows 3 SMBus blocks. Currently, we use the second block which
 * interacts with SPD EEPROMs and DIMM devices.
 */
#define I2C_SMBUS_NUM               1

/* TYU - New and improved SMBus */
#define I2C_SMBUS_SIZE              0x0800
#define I2C_SMBUS_ADDR              TYU_SMBUS_BASE(I2C_SMBUS_NUM)


/* Use the same reference clock for I2C as the UART */
#define BLUEFIELD_TYU_PLL_IN_FREQ   BF_REF_CLK_IN_HZ

/*
 * SMBus Master core clock frequency. Timing configurations are
 * strongly dependent on the core clock frequency of the SMBus
 * Master. Default value is set to 400MHz as required in Bluefield.
 */
#define BLUEFIELD_TYU_PLL_OUT_FREQ  (400 * 1000 * 1000)

/*
 * Note that YU PLL configuration bits are now exposed to SW in TYU boot
 * record block.
 */
#define TYU_CORE_PLL_CFG_ADDR       TYU_YU_CORE_PLL_CFG

/* TYU - Configuration for cause */
#define TYU_CAUSE_OR_BULK      TYU_SMBUS_MST_CAUSE_OR_BULK(I2C_SMBUS_NUM)
#define TYU_CAUSE_ARBITER_BITS TYU_SMBUS_MST_CAUSE_ARBITER_BITS(I2C_SMBUS_NUM)

/*
 * TYU - Configuration for GPIO:
 * Note that Smbus GWs are on GPIOs 30:25. Two pins are used to control
 * SDA/SCL lines:
 *
 *  SMBUS GW0 -> bits[26:25]
 *  SMBUS GW1 -> bits[28:27]
 *  SMBUS GW2 -> bits[30:29]
 */
#define TYU_GPIO_SMBUS_GW_PINS(num) (25 + ((num) << 1))

#define TYU_GPIO_SMBUS_GW_0_MASK    0xf9ffffff
#define TYU_GPIO_SMBUS_GW_1_MASK    0xe7ffffff
#define TYU_GPIO_SMBUS_GW_2_MASK    0x9fffffff
#define TYU_GPIO_SMBUS_GW_MASK(num) (TYU_GPIO_SMBUS_GW_ ##num ##_MASK)

#define TYU_GPIO_SMBUS_GW_ASSERT_PINS(num, val) \
	((val) | (0x3 << TYU_GPIO_SMBUS_GW_PINS((num))))

#define TYU_GPIO_SMBUS_GW_RESET_PINS(num, val) \
	((val) & TYU_GPIO_SMBUS_GW_MASK(num))

/* SMBus Timing Parameters */
#define SMBUS_TIMER_SCL_LOW_SCL_HIGH    (I2C_SMBUS_ADDR + 0x00)
#define SMBUS_TIMER_FALL_RISE_SPIKE     (I2C_SMBUS_ADDR + 0x04)
#define SMBUS_TIMER_THOLD               (I2C_SMBUS_ADDR + 0x08)
#define SMBUS_TIMER_TSETUP_START_STOP   (I2C_SMBUS_ADDR + 0x0c)
#define SMBUS_TIMER_TSETUP_DATA         (I2C_SMBUS_ADDR + 0x10)
#define SMBUS_THIGH_MAX_TBUF            (I2C_SMBUS_ADDR + 0x14)
#define SMBUS_SCL_LOW_TIMEOUT           (I2C_SMBUS_ADDR + 0x18)

/* SMBus Timing frequency mode */
#define SMBUS_SCL_HIGH_100KHZ           4810
#define SMBUS_SCL_HIGH_400KHZ           1011
#define SMBUS_SCL_HIGH_1MHZ             600

/* SMBus Master GW index */
#define SMBUS_MASTER_GW_IDX             0x1

/*
 * SMBus Master GW Registers
 */

/* SMBus Master GW */
#define SMBUS_MASTER_GW     (I2C_SMBUS_ADDR + 0x200)
/* Number of bytes received and sent */
#define SMBUS_RS_BYTES      (I2C_SMBUS_ADDR + 0x300)
/* Packet error check (PEC) value */
#define SMBUS_MASTER_PEC    (I2C_SMBUS_ADDR + 0x304)
/* Status bits (ACK/NACK/FW Timeout) */
#define SMBUS_MASTER_STATUS (I2C_SMBUS_ADDR + 0x308)
/* Shift left GW data bytes */
#define SMBUS_READ_SHIFT    (I2C_SMBUS_ADDR + 0x30c)
/* SMbus Master Finite State Machine */
#define SMBUS_MASTER_FSM    (I2C_SMBUS_ADDR + 0x310)
/* Toggle Clock */
#define SMBUS_MASTER_CLK    (I2C_SMBUS_ADDR + 0x314)
/* SDA and SCL configuration */
#define SMBUS_MASTER_CFG    (I2C_SMBUS_ADDR + 0x318)
/*
 * When enabled, the master will issue a stop condition in case of timeout
 * while waiting for FW response.
 */
#define SMBUS_EN_FW_TIMEOUT (I2C_SMBUS_ADDR + 0x31c)

/* SMBus Master GW control bits offset in SMBUS_MASTER_GW[31:3] */
#define MASTER_LOCK_BIT_OFF         31
#define MASTER_BUSY_BIT_OFF         30
#define MASTER_START_BIT_OFF        29	/* Control start */
#define MASTER_CTL_WRITE_BIT_OFF    28	/* Control write phase */
#define MASTER_WRITE_BIT_OFF        21	/* Control write bytes */
#define MASTER_SEND_PEC_BIT_OFF     20
#define MASTER_CTL_READ_BIT_OFF     19	/* Control read phase */
#define MASTER_PARSE_EXP_BIT_OFF    11	/* Control parse expected bytes */
#define MASTER_SLV_ADDR_BIT_OFF     12	/* Slave address */
#define MASTER_READ_BIT_OFF         4	/* Control read bytes */
#define MASTER_STOP_BIT_OFF         3	/* Control stop */

/* SMBus Master GW Data descriptor */
#define MASTER_DATA_DESC_ADDR   (I2C_SMBUS_ADDR + 0x280)	/* Address */
#define MASTER_DATA_DESC_SIZE   0x80	/* Data descriptor size in bytes */
#define MASTER_CTL_DATA_SIZE    2	/* Control data size in bytes */

/* Maximum bytes to read/write per SMBus transaction */
#define MASTER_DATA_R_LENGTH   MASTER_DATA_DESC_SIZE
#define MASTER_DATA_W_LENGTH  (MASTER_DATA_DESC_SIZE - 1)

/* SMBus Master GW Status flags */
#define SMBUS_STATUS_BYTE_CNT_DONE  0x1 /* All bytes were transmitted */
#define SMBUS_STATUS_NACK_RCV       0x2 /* NACK received */
#define SMBUS_STATUS_READ_ERR       0x4 /* Slave's byte count > 128 bytes */
#define SMBUS_STATUS_FW_TIMEOUT     0x8 /* Timeout occurred */

#define SMBUS_MASTER_STATUS_MASK        0x0000000f /* 4 bits */

#define SMBUS_MASTER_FSM_STOP_MASK      0x80000000
#define SMBUS_MASTER_FSM_PS_STATE_MASK  0x00008000

/*
 * Timeout is given in microsends. Note also that timeout handling is not
 * exact.
 */
#define SMBUS_TRANSFER_TIMEOUT      (200 * 1000) /* 200ms */
#define SMBUS_START_TRANS_TIMEOUT   (300 * 1000) /* 300ms */

/* Encapsulates timing parameters */
typedef struct {
	uint16_t scl_high;	/* Clock high period    */
	uint16_t scl_low;	/* Clock low period     */
	uint8_t  sda_rise;	/* Data Rise Time       */
	uint8_t  sda_fall;	/* Data Fall Time       */
	uint8_t  scl_rise;	/* Clock Rise Time      */
	uint8_t  scl_fall;	/* Clock Fall Time      */
	uint16_t hold_start;    /* Hold time after (REPEATED) START */
	uint16_t hold_data;	/* Data hold time       */
	uint16_t setup_start;	/* REPEATED START Condition setup time  */
	uint16_t setup_stop;    /* STOP Condition setup time            */
	uint16_t setup_data;    /* Data setup time      */
	uint16_t pad;           /* Padding              */
	uint16_t buf;           /* Bus free time between STOP and START */
	uint16_t thigh_max;     /* Thigh max            */
	uint32_t timeout;	/* Detect clock low timeout */
} i2c_timings_t;

#define I2C_TIMING_CONFIG_KHZ  100

/*
 * The I2C device operation describes a subset of an I2C transaction in which
 * the I2C controller is either sending or receiving bytes from the bus. Some
 * transactions will consist of a single operation while others will be two.
 */
typedef struct {
	/* Flag to qualify the I2C operation */
	uint32_t flags;

	/*
	 * Number of bytes to send to or receive from the I2C device.   A ping
	 * (address only byte/bytes) is indicated by setting the 'byte_len' to
	 * zero.
	 */
	uint32_t byte_len;

	/*
	 * Pointer to a buffer containing the data to send or to receive from
	 * the I2C device. The Buffer must be at least 'byte_len' in size.
	 */
	uint8_t *buffer;
} i2c_operation_t;

/* Maximum number of operations allowed per I2C transaction */
#define I2C_MAX_OPERATION      2

/* Define the I2C operation flags */
#define I2C_FLAG_READ            0x00000001 /* I2C read operation when set */
#define I2C_FLAG_SMBUS_OPERATION 0x00010000 /* SMBus operation */
#define I2C_FLAG_SMBUS_PEC       0x00080000 /* SMBus use packet error code */

/*
 * The I2C device request describes a single I2C transaction. The transaction
 * starts with a start bit followed by the first operation in the operation
 * array. Subsequent operations are separated with repeated start bits and the
 * last operation is followed by a stop bit which concludes the transaction.
 * Each operation is described by one of the elements in the Operation array.
 */
typedef struct {
	/* Number of elements in the operation array */
	uint32_t operation_cnt;
	/* Description of the I2C operation */
	i2c_operation_t operation[I2C_MAX_OPERATION];
} i2c_request_t;

typedef enum {
	SMBUS_NO_ERROR          = 0x0,
	SMBUS_DEVICE_BUSY       = 0x1,
	SMBUS_TRANSFER_FAILURE  = 0x2,
	SMBUS_BAD_CAUSE_STATUS  = 0x4,
	SMBUS_BAD_MASTER_STATUS = 0x8
} i2c_smbus_status_t;

/* This function configures and sets up the I2C SMBus */
void i2c_smbus_initialize(void);

/* Maximum bytes to read/write per transaction */
#define MAX_BUFFER_LENGTH    64

/* Defines I2C SMBus operations */
typedef enum {
	I2C_SMBUS_READ,
	I2C_SMBUS_WRITE
} i2c_smbus_ops_t;

/*
 * This function consists of an interface to slave devices. It enables data
 * transfer between the master and the slave.
 * The transaction consists of writing command bytes to the slave device,
 * then reading or writing 'length' bytes of data.
 *
 *  device      7-bit address of the slave on the bus.
 *  address     Internal device address, e.g. EEPROM offset, register address.
 *  length      Requested data length.
 *  buffer      Pointer to the source/destination data buffer.
 *  operation   I2C_SMBUS_READ to read data, I2C_SMBUS_WRITE to write data.
 *
 * return       0 on success, >0 on failure.
 */
int i2c_smbus_transfer(uint8_t   device,
		       uint16_t  address,
		       uint32_t  length,
		       uint8_t  *buffer,
		       uint8_t   operation);

/*
 * This function consists of an interface to read 512-byte, JEDEC
 * JC-42.4-compliant EEPROM that is segregated into two 256-byte,
 * write-protectable pages. It reads 'length' bytes of data from
 * address 'address'.
 *
 *  device      7-bit address of the SPD on the bus.
 *  address     Internal EEPROM offset.
 *  length      Requested data length.
 *  buffer      Pointer to the destination data buffer.
 *
 * return       0 on success, >0 on failure.
 */
int i2c_smbus_spd_read(uint8_t   device,
		       uint16_t  address,
		       uint16_t  length,
		       uint8_t  *buffer);

#endif				// __I2C_SMBUS_H__
