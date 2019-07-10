#include <i2c_smbus.h>

/* Polling frequency in microseconds */
#define POLL_FREQ_IN_USEC       1

#define ALIGN_ROUNDUP(x, align) (((x) + (align) - 1) & ~((align) - 1))
#define IOSWAP32(x)              (__builtin_bswap32((x)))

/* Macro to read 32-bit word registers */
#define TYU_READ(off)        mmio_read_32(TYU_BASE_ADDRESS + (off))
/* Macro to write 32-bit word registers */
#define TYU_WRITE(off, val)  mmio_write_32((TYU_BASE_ADDRESS + (off)), (val))

/*
 * This macro is used to read data from Master GW Data Descriptor.
 * Data bytes in the Master GW Data Descriptor are shifted left so
 * the data starts at the MSB of the descriptor registers as set by
 * the underlying hardware. TYU_READ_DATA enables byte swapping while
 * reading data bytes, and MUST be called by the SMBus read routines
 * to copy data from the 32 * 32-bit HW Data registers a.k.a Master GW
 * Data Descriptor.
 */
#define TYU_READ_DATA(off)       IOSWAP32(TYU_READ((off)))

/*
 * This macro is used to write data to the Master GW Data Descriptor.
 * Data copied to the Master GW Data Descriptor MUST be shifted left so
 * the data starts at the MSB of the descriptor registers as required by
 * the underlying hardware. TYU_WRITE_DATA enables byte swapping when
 * writing data bytes, and MUST be called by the SMBus write routines to
 * copy data to the 32 * 32-bit HW Data registers a.k.a Master GW Data
 * Descriptor.
 */
#define TYU_WRITE_DATA(off, val) TYU_WRITE((off), IOSWAP32((val)))

/*
 * Function to poll a set of bits at a specific address; it checks whether
 * the bits are equal to zero when eq_zero is set, and and not equal to zero
 * when eq_zero is not set.
 */
static uint32_t smbus_master_poll(uint32_t addr,
				  uint32_t mask,
				  uint8_t  eq_zero,
				  uint32_t timeout)
{
	uint32_t bits;

	timeout = (timeout / POLL_FREQ_IN_USEC) + 1;

	do {
		bits = TYU_READ(addr) & mask;
		if (eq_zero ? bits == 0 : bits != 0)
			return eq_zero ? 1 : bits;
		udelay(POLL_FREQ_IN_USEC);
	} while (timeout-- != 0);

	return 0;
}

/*
 * SW must make sure that the SMBus Master GW is idle before starting
 * a transaction. Accordingly, this function polls the Master FSM stop
 * bit; it returns 0 when the bit is asserted, 1 if not.
 */
static uint8_t i2c_smbus_master_is_idle(void)
{
	uint32_t addr    = SMBUS_MASTER_FSM;
	uint32_t mask    = SMBUS_MASTER_FSM_STOP_MASK;
	uint32_t timeout = SMBUS_START_TRANS_TIMEOUT;

	return smbus_master_poll(addr, mask, 1, timeout);
}

/*
 * Wrapper function to poll Cause arbiter bits and return cause
 * status. Note that timeout is given in microseconds.
 */
static uint16_t i2c_smbus_master_poll_cause(uint32_t timeout)
{
	uint32_t addr = TYU_CAUSE_ARBITER_BITS;
	uint32_t mask = TYU_CAUSE_ARBITER_BITS_MASK;

	return smbus_master_poll(addr, mask, 0, timeout);
}

/*
 * Poll SMBus master status and return transaction status, i.e. whether
 * succeeded or failed.
 */
static int i2c_smbus_poll_status(void)
{
	uint32_t cause_status_bits;
	uint32_t master_status_bits;

	/* First, poll cause status bits */
	cause_status_bits = i2c_smbus_master_poll_cause(SMBUS_TRANSFER_TIMEOUT);

	/*
	 * Parse both Cause and Master GW bits, and return transaction status.
	 */

	master_status_bits  = TYU_READ(SMBUS_MASTER_STATUS);
	master_status_bits &= SMBUS_MASTER_STATUS_MASK;

	/*
	 * When transaction ended with STOP, all bytes were transmitted,
	 * and no NACK recieved, then the transaction ended successfully.
	 * On the other hand, when the GW is configured with the stop bit
	 * de-asserted then the SMBus expects the following GW configuration
	 * for transfer continuation.
	 */
	if ((cause_status_bits & TYU_CAUSE_ARBITER_WAIT_FOR_FW_DATA) ||
	    ((cause_status_bits & TYU_CAUSE_ARBITER_TRANSACTION_ENDED) &&
	     (master_status_bits & SMBUS_STATUS_BYTE_CNT_DONE) &&
	     !(master_status_bits & SMBUS_STATUS_NACK_RCV)))
		return SMBUS_NO_ERROR;

	/*
	 * In case of timeout on GW busy, the ISR will clear busy bit but
	 * transaction ended bits cause will not be set so the transaction
	 * fails. Then, we must check Master GW status bits.
	 */
	if ((master_status_bits & (SMBUS_STATUS_NACK_RCV |
				   SMBUS_STATUS_READ_ERR |
				   SMBUS_STATUS_FW_TIMEOUT)) &&
	    (cause_status_bits & (TYU_CAUSE_ARBITER_TRANSACTION_ENDED |
				  TYU_CAUSE_ARBITER_GW_BUSY_FALL)))
		return SMBUS_BAD_MASTER_STATUS;

	if (cause_status_bits & (TYU_CAUSE_ARBITER_ARBITRATION_LOST |
				 TYU_CAUSE_ARBITER_UNEXPECTED_START |
				 TYU_CAUSE_ARBITER_UNEXPECTED_STOP  |
				 TYU_CAUSE_ARBITER_PUT_STOP_FAILED  |
				 TYU_CAUSE_ARBITER_PUT_START_FAILED |
				 TYU_CAUSE_ARBITER_CLK_TOGGLE_DONE  |
				 TYU_CAUSE_ARBITER_FW_TIMEOUT))
		return SMBUS_BAD_CAUSE_STATUS;

	return SMBUS_TRANSFER_FAILURE;
}

static uint8_t i2c_smbus_set_control(uint8_t   slave,
				     uint32_t  command,
				     uint32_t  transmitted,
				     uint32_t *control)
{
	uint8_t control_len;

	/*
	 * According to the specification, an SMBus command refers to the
	 * data sent to the slave during SMBus cycles. It is usually sent
	 * after the slave address byte. As an example this command could
	 * be used to put a device in a specific mode dependent upon the
	 * data sent in the command.
	 * In the case where the host controller communicates with the SPD
	 * EEPROMs found on NVDIMMs, the command will refer to the device
	 * internal address from which the data bytes will be written.
	 * Note that the number of bytes sent as a command strongly depends
	 * on the slave device.
	 */

	/*
	 * *TBD* Based on the slave device and the command bytes, the host
	 * controller should determine the control bytes and its length, to
	 * send before data bytes. These control bytes usually include the
	 * SMBus command byte, and/or internal address bytes (specific to
	 * the slave device CSRs). These bytes are appended to the slave
	 * address and are copied to the first word into the Master GW Data
	 * Descriptor. Control bytes are expected to mainly vary between 1
	 * and 2.
	 *
	 * Currently the host controller is dealing with NVDIMM devices,
	 * the SMBus operation requires one or two byte address following
	 * the device address. So the length of the control bytes is set
	 * accordingly. In case where different slave devices are supported,
	 * the following statements must be extended.
	 */
	command  += transmitted;
	*control  = 0;

	switch (slave) {
	case I2C_SPD_0_ADDR:
	case I2C_SPD_1_ADDR:
	case I2C_SPD_2_ADDR:
	case I2C_SPD_3_ADDR:
	case I2C_DIMM_0_ADDR:
	case I2C_DIMM_1_ADDR:
	case I2C_DIMM_2_ADDR:
	case I2C_DIMM_3_ADDR:
		*control    |= command << 24;
		control_len  = 1;
		break;

	case I2C_SPD_SPA0:
	case I2C_SPD_SPA1:
		control_len  = 0; /* Do not send any command */
		break;

	default:
		*control    |= command << 24;
		control_len  = 1; /* As defined by SMBus specification */
		break;
	}

	return control_len;
}

static void i2c_smbus_read_data(uint8_t *buffer,
				uint8_t  length)
{
	uint8_t  byte;
	uint32_t data32;
	uint32_t offset;

	for (byte = 0, offset = 0; offset < (length & ~0x3); offset += 4) {
		data32 = TYU_READ_DATA(MASTER_DATA_DESC_ADDR + offset);

		buffer[byte++] = ((uint8_t *) &data32)[0];
		buffer[byte++] = ((uint8_t *) &data32)[1];
		buffer[byte++] = ((uint8_t *) &data32)[2];
		buffer[byte++] = ((uint8_t *) &data32)[3];
	}

	if (!(length & 0x3))
		return;

	data32 = TYU_READ_DATA(MASTER_DATA_DESC_ADDR + offset);

	/* Shift right remaining data */
	for (byte = 0; byte < (length & 0x3); byte++) {
		buffer[offset + byte] = data32 & 0xff;
		data32 >>= 8;
	}
}

static int i2c_smbus_read(uint8_t  slave,
			  uint8_t *data,
			  uint8_t  length,
			  uint8_t  pec_en)
{
	uint32_t data32, control32;
	uint8_t  read_size;
	uint8_t  desc_len;
	int      status;

	/*
	 * The Smbus Data Read flow:
	 * - The control bits are copied to the first Master GW control word.
	 * - The slave address byte is copied to the first data word in the
	 *   Master GW Data Descriptor.
	 * Note that Master GW data is shifted left so the data will start
	 * at the beginning.
	 */

	/* Check whether the SMBus Master GW is idle */
	if (!i2c_smbus_master_is_idle())
		return SMBUS_DEVICE_BUSY;

	/* Set slave address byte */
	data32 = (slave & 0x7f) << 1;

	while (length > 0) {
		/* Write control bytes into Master GW Data Descriptor */
		TYU_WRITE_DATA(MASTER_DATA_DESC_ADDR, data32);

		/* Set number of data bytes to read. Unlike the write, the
		 * read operation allows the master controller to read up to
		 * 128 bytes.
		 */
		desc_len = (length <= MASTER_DATA_R_LENGTH) ?
		    length : MASTER_DATA_R_LENGTH;
		/* The HW requires that SW subtract 1 */
		read_size = desc_len - 1;

		/* Set Master GW control words */
		control32  = 0;
		control32 |= 0x1       << MASTER_LOCK_BIT_OFF;
		control32 |= 0x1       << MASTER_BUSY_BIT_OFF;
		control32 |= slave     << MASTER_SLV_ADDR_BIT_OFF;
		control32 |= 0x1       << MASTER_START_BIT_OFF;
		control32 |= 0x1       << MASTER_STOP_BIT_OFF;
		control32 |= read_size << MASTER_READ_BIT_OFF;
		control32 |= 0x1       << MASTER_CTL_READ_BIT_OFF;
		control32 |= 0         << MASTER_WRITE_BIT_OFF;
		control32 |= 0         << MASTER_CTL_WRITE_BIT_OFF;
		control32 |= 0         << MASTER_PARSE_EXP_BIT_OFF;
		control32 |= pec_en    << MASTER_SEND_PEC_BIT_OFF;

		TYU_WRITE(SMBUS_MASTER_STATUS, 0x0);	/* Clear status bits  */
		TYU_WRITE(TYU_CAUSE_OR_BULK,  ~0x0);	/* Set the cause data */
		TYU_WRITE(SMBUS_MASTER_PEC,    0x0);	/* Zero PEC byte      */
		TYU_WRITE(SMBUS_RS_BYTES,      0x0);	/* Zero byte count    */

		TYU_WRITE(SMBUS_MASTER_GW, control32);	/* GW activation      */

		/* Poll master status and check status bits */
		status = i2c_smbus_poll_status();
		if (status != 0)
			return status;

		/* Read data bytes */
		i2c_smbus_read_data(data, desc_len);

		/*
		 * After a read operation the SMBus FSM ps (present state)
		 * needs to be 'manually' reset. This should be removed in
		 * next tag integration.
		 */
		TYU_WRITE(SMBUS_MASTER_FSM, SMBUS_MASTER_FSM_PS_STATE_MASK);

		/* Update remaining data bytes to read */
		length -= desc_len;
	}

	return 0;		/* SMBUS_NO_ERROR */
}

static void i2c_smbus_write_data(const uint8_t *buffer,
				 uint8_t        length)
{
	uint32_t data32;
	uint32_t byte, offset;

	/* Copy data from 4-byte aligned source buffer */
	for (byte = 0, offset = 0; offset < (length & ~0x3); offset += 4) {
		((uint8_t *) &data32)[0] = buffer[byte++];
		((uint8_t *) &data32)[1] = buffer[byte++];
		((uint8_t *) &data32)[2] = buffer[byte++];
		((uint8_t *) &data32)[3] = buffer[byte++];

		TYU_WRITE_DATA(MASTER_DATA_DESC_ADDR + offset, data32);
	}

	if (!(length & 0x3))
		return;

	data32 = 0;
	/* Shift right remaining data */
	for (byte = 0; byte < (length & 0x3); byte++)
		data32 |= (buffer[offset + byte] << (8 * byte));

	TYU_WRITE_DATA(MASTER_DATA_DESC_ADDR + offset, data32);
}

static int i2c_smbus_write(uint8_t        slave,
			   uint32_t       command,
			   const uint8_t *data,
			   uint8_t        length,
			   uint8_t        pec_en)
{
	uint32_t control32, control;
	uint8_t  sent, write_size, byte, control_len, data_len;
	uint8_t  data_desc[MASTER_DATA_DESC_SIZE] = { 0 };
	uint8_t  desc_len, data_idx;
	int      status;

	/*
	 * The Smbus Data Write flow:
	 * - The control bits are copied to the first Master GW control word.
	 * - The slave address byte and SMBus command bytes are copied to the
	 *   first data word in the Master GW Data Descriptor, followed by the
	 *   data bytes.
	 * Note that Master GW data is shifted left so the data will start at
	 * the beginning.
	 */
	sent = 0;

	/* Check whether the SMBus Master GW is idle */
	if (!i2c_smbus_master_is_idle())
		return SMBUS_DEVICE_BUSY;

	do {
		/*
		 * Prepare the control bytes and its length, to send before
		 * data bytes. Control bytes strongly depend on the SMBus
		 * Command as well as the slave device, i.e. these bytes might
		 * enable the control of the slave device.
		 */
		control_len =
		    i2c_smbus_set_control(slave, command, sent, &control);

		/*
		 * Set number of data bytes to write. Unlike the read, the
		 * write operation allows the master controller to write up to
		 * 127 bytes only. The first data bytes might be reserved for
		 * control bytes (e.g. command bytes). The total number of
		 * bytes to write to the Master GW data descriptor includes the
		 * slave address byte, control bytes and data bytes.
		 */
		data_len = (length <= (MASTER_DATA_W_LENGTH - control_len) ?
			    length : (MASTER_DATA_W_LENGTH - control_len));
		desc_len = control_len + data_len + 1;
		/* The HW requires that SW subtract 1 */
		write_size = desc_len - 1;

		data_idx = 0;
		/*
		 * Write Slave address to the data descriptor buffer. Slave
		 * address is shifted left by 1 as required by hardware.
		 */
		data_desc[data_idx++] = (slave & 0x7f) << 1;

		/* Copy control data to the data descriptor */
		for (byte = 0; byte < control_len; byte++, data_idx++)
			data_desc[data_idx] =
			    (control >> (24 - (8 * byte))) & 0xff;

		/* Copy data to write to the data descriptor */
		for (; data_idx < desc_len; data_idx++, sent++)
			data_desc[data_idx] = data[sent];

		i2c_smbus_write_data(data_desc, desc_len);

		/* Set Master GW control words */
		control32  = 0;
		control32 |= 0x1        << MASTER_LOCK_BIT_OFF;
		control32 |= 0x1        << MASTER_BUSY_BIT_OFF;
		control32 |= slave      << MASTER_SLV_ADDR_BIT_OFF;
		control32 |= 0x1        << MASTER_START_BIT_OFF;
		control32 |= 0x1        << MASTER_STOP_BIT_OFF;
		control32 |= 0          << MASTER_READ_BIT_OFF;
		control32 |= 0          << MASTER_CTL_READ_BIT_OFF;
		control32 |= write_size << MASTER_WRITE_BIT_OFF;
		control32 |= 0x1        << MASTER_CTL_WRITE_BIT_OFF;
		control32 |= 0          << MASTER_PARSE_EXP_BIT_OFF;
		control32 |= pec_en     << MASTER_SEND_PEC_BIT_OFF;

		TYU_WRITE(SMBUS_MASTER_STATUS, 0x0);	/* Clear status bits  */
		TYU_WRITE(TYU_CAUSE_OR_BULK,  ~0x0);	/* Set the cause data */
		TYU_WRITE(SMBUS_MASTER_PEC,    0x0);	/* Zero PEC byte      */
		TYU_WRITE(SMBUS_RS_BYTES,      0x0);	/* Zero byte count    */

		TYU_WRITE(SMBUS_MASTER_GW, control32);	/* GW activation      */

		/*
		 * Poll master status and check status bits.
		 * An ACK is sent when completing writing data to the bus
		 * (Master 'byte_count_done' bit is set to 1).
		 */
		status = i2c_smbus_poll_status();
		if (status != 0)
			return status;

		/*
		 * Update remaining data bytes to write. Note that control
		 * bytes does not count. Indeed, these bytes are required by
		 * the write transfer.
		 */
		length -= data_len;
	} while (length > 0);

	return 0;		/* SMBUS_NO_ERROR */
}

/*
 * The start transaction implements the bus controller functionality.
 * It enables I2C SMBus transactions and issues SMBus commands to devices
 * attached to the bus. Note that this function is called by the wrapper
 * function 'i2c_smbus_transfer()' API. In fact, that API is responsible
 * for formatting the I2C request that is used to describe the I2C SMBus
 * transaction.
 *
 *  slave       Slave address on the bus, MUST be on 7-bit resolution.
 *  command     SMBus command bytes, see SMBus protocol specification.
 *  request     Pointer to the I2C request descriptor.
 *
 * return       0 on success; the result of i2c_smbus_poll_status() on failure.
 */
int i2c_smbus_start_transaction(uint8_t        slave,
				uint16_t       command,
				i2c_request_t *request)
{
	i2c_operation_t *operation;
	uint8_t          count, read_mode, pec_en;
	int              status;

	assert(request != 0);

	status = SMBUS_NO_ERROR;

	for (count = 0; count < request->operation_cnt; count++) {
		operation = &request->operation[count];
		read_mode = operation->flags & I2C_FLAG_READ;
		pec_en    = operation->flags & I2C_FLAG_SMBUS_PEC;

		/*
		 * Set the control word which is prepended to data bytes in
		 * Master GW. 'Command' signifies the SMBus command, i.e.
		 * control bytes which allow the master to control the slave
		 * device. These bytes are prepended to the slave address and
		 * are copied to the first Master GW data word. The slave
		 * address is written to byte 3, followed by the SMBus command
		 * bytes. Command bytes are expected to mainly vary between 1
		 * and 2. Note that the slave address is shifted left by 1.
		 */

		if (read_mode)
			status = i2c_smbus_read(slave,
						operation->buffer,
						operation->byte_len,
						pec_en);
		else
			status = i2c_smbus_write(slave,
						 command,
						 operation->buffer,
						 operation->byte_len,
						 pec_en);

		if (status != 0)
			break;
	}

	return status;
}

int i2c_smbus_transfer(uint8_t   device,
		       uint16_t  address,
		       uint32_t  length,
		       uint8_t  *buffer,
		       uint8_t   operation)
{
	i2c_request_t request;
	uint32_t      buf_len, transmitted;
	uint16_t      curr_addr;
	uint8_t       op_idx;
	int           status;

	op_idx      = 0;
	transmitted = 0;
	curr_addr   = address;
	status      = SMBUS_NO_ERROR;

	request.operation_cnt = 1;
	/*
	 * The master might use a combined W-R cycle to the slave. It can also
	 * do an W-W. The "Write followed by Read" operation sequence is by far
	 * one of the most common implementation. It is typically used to send
	 * a command to a device and then read data based on the command sent.
	 * For instance, the W-R operations could be used to read from EEPROMs.
	 * The initial write tells the EEPROM of the specific offset to be read
	 * from in the subsequent read operation. Note that operation count is
	 * set to 1 when EEPROM_WRITE is set because the underlying Mellanox
	 * SMBus hardware is able to perform the W-W operation through a single
	 * command. Unlike the W-W the combined R-W requires two separate
	 * operations.
	 */
	if (operation == I2C_SMBUS_READ) {
		request.operation[op_idx].byte_len = 0;
		request.operation_cnt++;
		op_idx++;
	}

	request.operation[op_idx].flags =
	    (operation == I2C_SMBUS_READ) ? I2C_FLAG_READ : 0;

	while (length > 0) {
		/* Set transaction control bytes (internal device address) */
		curr_addr = address + transmitted;
		buf_len   = (length <= MAX_BUFFER_LENGTH) ?
			length : MAX_BUFFER_LENGTH;
		request.operation[op_idx].byte_len = buf_len;
		request.operation[op_idx].buffer   = buffer + transmitted;

		/* Start I2C transaction */
		status =
		    i2c_smbus_start_transaction(device, curr_addr, &request);
		if (status != 0)
			break;

		length      -= buf_len;
		transmitted += buf_len;
	}

	return status;
}

int i2c_smbus_spd_read(uint8_t   device,
		       uint16_t  address,
		       uint16_t  length,
		       uint8_t  *buffer)
{
	uint16_t curr_addr, buf_len, read = 0;
	uint8_t  prev_page, curr_page, dummy_byte = 0;
	int      status;

	/*
	 * There are four independent memory blocks, and each block may be
	 * independently protected.  The memory blocks are:
	 *   Block 0 = memory addresses 0x00 to 0x7F, SPD Page Address = 0
	 *   Block 1 = memory addresses 0x80 to 0xFF, SPD Page Address = 0
	 *   Block 2 = memory addresses 0x00 to 0x7F, SPD Page Address = 1
	 *   Block 3 = memory addresses 0x80 to 0xFF, SPD Page Address = 1
	 */

	/*
	 * Reading the EEPROM page address results in Ack when the current
	 * page is 0 and NoAck when the current page is 1.
	 */
	status = i2c_smbus_read(I2C_SPD_RPA, &dummy_byte, 1, 0);
	if (status == 0) /* if Ack */
		prev_page = 0;
	else /* if NoAck */
		prev_page = 1;

	while (length > 0) {
		/*
		 * The output data comes from consecutive addresses, with
		 * the internal address counter automatically incremented
		 * after each byte output. After the last memory address,
		 * the address counter rolls over, and the device continues
		 * to output data from memory address 0x00.
		 */
		curr_addr = (address + read) % I2C_SPD_SIZE;
		buf_len   = (length < I2C_SPD_PAGE_SIZE) ?
			length : I2C_SPD_PAGE_SIZE;

		/*
		 * Deduce the page number. This might be confusing if the
		 * SPD EEPROM size is >512 bytes. For now, we assume that
		 * 'curr_page' is set either to 0 (lower 256 bytes) or 1
		 * (upper 256 bytes).
		 */
		curr_page = curr_addr >> 8;

		if (curr_page != prev_page) {
			/*
			 * Set EEPROM page address to 0 selects the lower 256
			 * bytes of EEPROM, setting to 1 selects the upper 256
			 * bytes of EEPROM. Subsequent Read EEPROM or Write
			 * EEPROM commands operate on the selected EEPROM page.
			 */
			i2c_smbus_write(curr_page == 0 ?
					I2C_SPD_SPA0 : I2C_SPD_SPA1,
					0, &dummy_byte, 0, 0);

			/*
			 * Read the EEPROM page address to verify whether the
			 * change has been applied.
			 */
			status = i2c_smbus_read(I2C_SPD_RPA,
						&dummy_byte, 1, 0);
			if (status != 0 && curr_page == 0)
				return status;
			else if (status == 0 && curr_page == 1)
				return SMBUS_TRANSFER_FAILURE;

			prev_page = curr_page;
		}

		/* The read starts here */
		status = i2c_smbus_transfer(device,
					    curr_addr & 0xff,
					    buf_len,
					    buffer + read,
					    I2C_SMBUS_READ);
		if (status != 0)
			return status;

		length -= buf_len;
		read   += buf_len;
	}

	return status;
}


/* Return core frequency in Hz */
static uint64_t i2c_smbus_get_core_freq(void)
{
	/*
	 * Compute PLL output frequency as follow:
	 *
	 *                                       CORE_F + 1
	 * PLL_OUT_FREQ = PLL_IN_FREQ * ----------------------------
	 *                              (CORE_R + 1) * (CORE_OD + 1)
	 *
	 * Where PLL_OUT_FREQ and PLL_IN_FREQ refer to CoreFrequency and
	 * PadFrequency, respectively.
	 *
	 * For a CoreFrequency of 400MHz, expected PLL parameters are:
	 *
	 *  CORE_F     = 511
	 *  CORE_R     = 24
	 *  CORE_OD    = 7
	 *  CORE_BWADJ = 255
	 *
	 */
	uint64_t core_frequency;
	uint64_t pad_frequency;
	uint32_t core_pll_val;
	uint16_t core_f;
	uint8_t  core_od, core_r;

	/*
	 * PLL configuration bits are exposed to SW in TYU. This function
	 * is responsible for reading these bits and deriving the TYU
	 * frequency.
	 */

	core_pll_val  = TYU_READ(TYU_CORE_PLL_CFG_ADDR);
	pad_frequency = BLUEFIELD_TYU_PLL_IN_FREQ;

	/* Get Core PLL configuration bits */
	core_f  = GET_CORE_PLL_FIELD(CORE_F, core_pll_val);
	core_od = GET_CORE_PLL_FIELD(CORE_OD, core_pll_val);
	core_r  = GET_CORE_PLL_FIELD(CORE_R, core_pll_val);

	/*
	 * Compute PLL output frequency as follow:
	 *
	 *                                       CORE_F + 1
	 * PLL_OUT_FREQ = PLL_IN_FREQ * ----------------------------
	 *                              (CORE_R + 1) * (CORE_OD + 1)
	 *
	 * Where PLL_OUT_FREQ and PLL_IN_FREQ refer to CoreFrequency
	 * and PadFrequency, respectively.
	 */

	core_frequency  = pad_frequency * (core_f + 1);
	core_frequency /= ((core_r + 1) * (core_od + 1));

	return core_frequency;
}

/*
 * Convert elapsed time in nanoseconds to elapsed ticks of performance counter.
 */
static uint32_t i2c_smbus_get_ticks(uint64_t nanoseconds, uint8_t minimum)
{
	uint64_t frequency;
	uint32_t ticks;

	/*
	 * Compute ticks as follow:
	 *
	 *           Ticks
	 * Time = --------- x 10^9    =>    Ticks = Time x Frequency x 10^-9
	 *         Frequency
	 */

	frequency = i2c_smbus_get_core_freq();

	ticks     = (nanoseconds * frequency) / 1000000000;
	/*
	 * The number of ticks is rounded down and if minimum is equal to
	 * 1 then add one tick.
	 */
	if (minimum == 1)
		ticks += 1;

	return ticks;
}

static uint32_t i2c_smbus_set_timer_bits(uint32_t nsec,
					 uint8_t  opt,
					 uint32_t mask,
					 uint8_t  offset)
{
	return ((i2c_smbus_get_ticks(nsec, opt) & mask) << offset);
}

static void i2c_smbus_set_timings(i2c_timings_t *timings)
{
	uint32_t timer;

	timer  = i2c_smbus_set_timer_bits(timings->scl_high,    0, 0xffff,  0);
	timer |= i2c_smbus_set_timer_bits(timings->scl_low,     0, 0xffff, 16);
	TYU_WRITE(SMBUS_TIMER_SCL_LOW_SCL_HIGH, timer);

	timer  = i2c_smbus_set_timer_bits(timings->sda_rise,    0, 0xff,    0);
	timer |= i2c_smbus_set_timer_bits(timings->sda_fall,    0, 0xff,    8);
	timer |= i2c_smbus_set_timer_bits(timings->scl_rise,    0, 0xff,   16);
	timer |= i2c_smbus_set_timer_bits(timings->scl_fall,    0, 0xff,   24);
	TYU_WRITE(SMBUS_TIMER_FALL_RISE_SPIKE, timer);

	timer  = i2c_smbus_set_timer_bits(timings->hold_start,  1, 0xffff,  0);
	timer |= i2c_smbus_set_timer_bits(timings->hold_data,   1, 0xffff, 16);
	TYU_WRITE(SMBUS_TIMER_THOLD, timer);

	timer  = i2c_smbus_set_timer_bits(timings->setup_start, 1, 0xffff,  0);
	timer |= i2c_smbus_set_timer_bits(timings->setup_stop,  1, 0xffff, 16);
	TYU_WRITE(SMBUS_TIMER_TSETUP_START_STOP, timer);

	timer = i2c_smbus_set_timer_bits(timings->setup_data,   1, 0xffff,  0);
	TYU_WRITE(SMBUS_TIMER_TSETUP_DATA, timer);

	timer  = i2c_smbus_set_timer_bits(timings->buf,         0, 0xffff,  0);
	timer |= i2c_smbus_set_timer_bits(timings->thigh_max,   0, 0xffff, 16);
	TYU_WRITE(SMBUS_THIGH_MAX_TBUF, timer);

	timer = timings->timeout;
	TYU_WRITE(SMBUS_SCL_LOW_TIMEOUT, timer);
}

static void i2c_smbus_init_timings(uint32_t config_khz)
{
	i2c_timings_t timings;

	/*
	 * Smbus Timing initialization
	 *
	 *
	 * Parameter         100KHz link   400KHz link     1MHz link
	 *
	 * Clock frequency   100 KHz       400 KHz         1 MHz
	 * Clock low timeout 35 msec       35 msec         35 msec
	 * scl_high_time     4.87/4.81 us  1.05/1.01us     0.6 us
	 * scl_low_time      5 us          1.3 us          1.3 us
	 * sda_rise_time     100 ns        100 ns          100 ns
	 * sda_fall_time     100 ns        100 ns          100 ns
	 * scl_rise_time     100 ns        100 ns          100 ns
	 * scl_fall_time     100 ns        100 ns          100 ns
	 * thold_start       4.0 us        600 ns          600 ns
	 * thold_data        300 ns        300 ns          300 ns
	 * tsetup_start      4.8 us        700 ns          600 ns
	 * tsetup_stop       4.0 us        600 ns          600 ns
	 * tsetup_data       250 ns        100 ns          100 ns
	 * Tbuf              20  us        20  us          20  us
	 * thigh_max         50  us        50  us          50  us
	 *
	 * Spike pulse width 50  ns        50  ns          50  ns
	 */

	switch (config_khz) {
	default:
		/* Default settings is 100 KHz */
		tf_printf("i2c: Illegal value %d: defaulting to 100 Khz\n",
			  config_khz);

		/* FALLTHROUGH */

	case 100:
		timings.scl_high    = SMBUS_SCL_HIGH_100KHZ;
		timings.scl_low     = 5000;
		timings.hold_start  = 4000;
		timings.setup_start = 4800;
		timings.setup_stop  = 4000;
		timings.setup_data  = 250;
		break;

	case 400:
		timings.scl_high    = SMBUS_SCL_HIGH_400KHZ;
		timings.scl_low     = 1300;
		timings.hold_start  = 600;
		timings.setup_start = 700;
		timings.setup_stop  = 600;
		timings.setup_data  = 100;
		break;

	case 1000:
		timings.scl_high    = SMBUS_SCL_HIGH_1MHZ;
		timings.scl_low     = 1300;
		timings.hold_start  = 600;
		timings.setup_start = 600;
		timings.setup_stop  = 600;
		timings.setup_data  = 100;
		break;
	}

	timings.sda_rise  = timings.sda_fall = 50;
	timings.scl_rise  = timings.scl_fall = 50;
	timings.hold_data = 300;
	timings.buf       = 20000;
	timings.thigh_max = 5000;
	/*
	 * Note that the SCL_LOW_TIMEOUT value is not related to the bus
	 * frequency, it is impacted by the time it takes the driver to
	 * complete data transmission before transaction abort.
	 */
	timings.timeout   = 106500;

	i2c_smbus_set_timings(&timings);
}

/*
 * Initialize the I2C SMBus. It consists of master initialization and Timer
 * settings.
 */
void i2c_smbus_initialize(void)
{
	uint32_t config_reg;

	/*
	 * TYU - Configuration for GPIO pins. Those pins must be asserted in
	 * TYU_GPIO_0_FUNC_EN_0, i.e. GPIO 0 is controlled by HW, and must
	 * be reset in TYU_GPIO_0_FORCE_OE_EN, i.e. GPIO_OE will be driven
	 * instead of HW_OE.
	 */

	config_reg = TYU_READ(TYU_GPIO_0_FUNC_EN_0);
	config_reg = TYU_GPIO_SMBUS_GW_ASSERT_PINS(I2C_SMBUS_NUM, config_reg);
	TYU_WRITE(TYU_GPIO_0_FUNC_EN_0, config_reg);

	config_reg = TYU_READ(TYU_GPIO_0_FORCE_OE_EN);
	config_reg = TYU_GPIO_SMBUS_GW_RESET_PINS(I2C_SMBUS_NUM, config_reg);
	TYU_WRITE(TYU_GPIO_0_FORCE_OE_EN, config_reg);

	/*
	 * Smbus Timing initialization - Frequency setup at 100KHZ.
	 */

	i2c_smbus_init_timings(I2C_TIMING_CONFIG_KHZ);
}
