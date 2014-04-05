/*
 ===============================================================================
 Name        : i2c.h
 Author      : uCXpresso
 Version     : v1.0.1
 Date		 : 2014/4/5
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : I2C driver
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	First Edition for nano11Uxx						Jason
 2014/4/5	v1.0.1	Add more read & write member functions in		Jason
 	 	 	 	 	    CMaster class.
 	 	 	 	 	Fixed I2C driver lock problems.
 	 	 	 	 	Remove I2C-Slave mode.
 	 	 	 	 	Move readwrite member to base clase.
 ===============================================================================
 */

#ifndef _I2C_H_
#define _I2C_H_

#include "class/peripheral.h"
#include "class/stream.h"
#include "class/semaphore.h"

/**I2C Error Code
 * \ingroup Enumerations
 */
typedef enum {
	I2C_BUSY = 6,				///<
	I2C_NO_DATA = 7,			///<
	I2C_NACK_ON_ADDRESS = 8,	///<
	I2C_NACK_ON_DATA = 9,		///<
	I2C_ARBITRATION_LOST = 10,	///<
	I2C_TIME_OUT = 11,			///<
	I2C_OK = 12,				///<
	I2C_BUFFER = 13				///< Memory Buffer Error
}I2C_ERROR_T;

#define DEF_I2C_TIMEOUT	1000


/**I2C base class
 * \class CI2C i2c.h "class/i2c.h"
 * \ingroup Peripherals
 */
class CI2C: public CPeripheral {
public:

	/**Enable I2C interface
	 */
	void enable();

	/**Disable I2C interface
	 */
	void disable();

	/**Set I2C frequency
	 * \param hz a unsigned long to indicate the frequency of I2C.
	 */
	virtual void frequency(uint32_t hz);

	/** start read & write engine
	 */
	I2C_ERROR_T	readwrite(void *txbuf, int txsize, void *rxbuf, int rxsize, uint32_t timeout=DEF_I2C_TIMEOUT);

	//
	//	PRIVATE
	/// @cond
	CI2C();
	virtual 	~CI2C();

	/**Indicate the transmit buffer
	 */
	virtual void write(uint8_t *data, int length);

	/**Indicate the receive buffer
	 */
	virtual void read(uint8_t *data, int length);

	/**Begin transmit and receive the I2C data
	 */
	virtual I2C_ERROR_T engine(uint32_t timeout);

	virtual void onState(uint32_t value);
	virtual bool start();	// Create I2C start condition
	virtual bool stop();	// Set the I2C stop condition

	// irq & status control
	CSemaphore	m_semState;
	uint32_t	m_state;

	// buffer control
	uint8_t		*I2CMasterBuffer;
	uint8_t		*I2CSlaveBuffer;
	int			RdIndex;
	int			WrIndex;
	int			I2CWriteLength;
	int			I2CReadLength;

	/// @endcond
};

/**The I2CMaster class provides the i2c interface and compatibility of i2cdevlib/i2cdev class.
 * \class CI2CMaster i2c.h "class/i2c.h"
 * \ingroup Peripherals
 */
class CI2CMaster: public CI2C {
public:

	/** Read a single bit from an 8-bit device register.
	 * @param devAddr I2C slave device address
	 * @param regAddr Register regAddr to read from
	 * @param bitNum Bit position to read (0-7)
	 * @param data Container for single bit value
	 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
	 * @return Status of read operation (I2C_OK = success)
	 */
	I2C_ERROR_T readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint32_t timeout=DEF_I2C_TIMEOUT);

	/** Read a single bit from a 16-bit device register.
	 * @param devAddr I2C slave device address
	 * @param regAddr Register regAddr to read from
	 * @param bitNum Bit position to read (0-15)
	 * @param data Container for single bit value
	 * @param timeout Optional read timeout in milliseconds
	 * @return Status of read operation (I2C_OK = success)
	 */
	I2C_ERROR_T readBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data, uint16_t timeout=DEF_I2C_TIMEOUT);

	/** Read multiple bits from an 8-bit device register.
	 * @param devAddr I2C slave device address
	 * @param regAddr Register regAddr to read from
	 * @param bitStart First bit position to read (0-7)
	 * @param length Number of bits to read (not more than 8)
	 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
	 * @param timeout Optional read timeout in milliseconds
	 * @return Status of read operation (I2C_OK = success)
	 */
	I2C_ERROR_T readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint32_t timeout=DEF_I2C_TIMEOUT);

	/** Read multiple bits from a 16-bit device register.
	 * @param devAddr I2C slave device address
	 * @param regAddr Register regAddr to read from
	 * @param bitStart First bit position to read (0-15)
	 * @param length Number of bits to read (not more than 16)
	 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
	 * @param timeout Optional read timeout in milliseconds
	 * @return Status of read operation (I2C_OK = success)
	 */
	I2C_ERROR_T readBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data, uint32_t timeout=DEF_I2C_TIMEOUT);

	/** Read single byte from an 8-bit device register.
	 * @param devAddr I2C slave device address
	 * @param regAddr Register regAddr to read from
	 * @param data Container for byte value read from device
	 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
	 * @return Status of read operation (I2C_OK = success)
	 */
	I2C_ERROR_T readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint32_t timeout=DEF_I2C_TIMEOUT);

	/** Read single word from a 16-bit device register.
	 * @param devAddr I2C slave device address
	 * @param regAddr Register regAddr to read from
	 * @param data Container for word value read from device
	 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
	 * @return Status of read operation (I2C_OK = success)
	 */
	I2C_ERROR_T readWord(uint8_t devAddr, uint8_t regAddr, uint16_t *data, uint32_t timeout=DEF_I2C_TIMEOUT);

	/** Read multiple bytes from an 8-bit device register.
	 * @param devAddr I2C slave device address
	 * @param regAddr First register regAddr to read from
	 * @param length Number of bytes to read
	 * @param data Buffer to store read data in
	 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
	 * @return Status of read operation (I2C_OK = success)
	 */
	I2C_ERROR_T readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint32_t timeout=DEF_I2C_TIMEOUT);

	/** Read multiple words from a 16-bit device register.
	 * @param devAddr I2C slave device address
	 * @param regAddr First register regAddr to read from
	 * @param length Number of words to read
	 * @param data Buffer to store read data in
	 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
	 * @return Status of read operation (I2C_OK = success)
	 */
	I2C_ERROR_T readWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data, uint32_t timeout=DEF_I2C_TIMEOUT);

	/** write a single bit in an 8-bit device register.
	 * @param devAddr I2C slave device address
	 * @param regAddr Register regAddr to write to
	 * @param bitNum Bit position to write (0-7)
	 * @param value New bit value to write
	 * @return Status of read operation (I2C_OK = success)
	 */
	I2C_ERROR_T writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t value);

	/** write a single bit in a 16-bit device register.
	 * @param devAddr I2C slave device address
	 * @param regAddr Register regAddr to write to
	 * @param bitNum Bit position to write (0-15)
	 * @param value New bit value to write
	 * @return Status of operation (I2C_OK = success)
	 */
	I2C_ERROR_T writeBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data);

	/** Write multiple bits in an 8-bit device register.
	 * @param devAddr I2C slave device address
	 * @param regAddr Register regAddr to write to
	 * @param bitStart First bit position to write (0-7)
	 * @param length Number of bits to write (not more than 8)
	 * @param data Right-aligned value to write
	 * @return Status of operation (I2C_OK = success)
	 */
	I2C_ERROR_T writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);

	/** Write multiple bits in a 16-bit device register.
	 * @param devAddr I2C slave device address
	 * @param regAddr Register regAddr to write to
	 * @param bitStart First bit position to write (0-15)
	 * @param length Number of bits to write (not more than 16)
	 * @param data Right-aligned value to write
	 * @return Status of operation (I2C_OK = success)
	 */
	I2C_ERROR_T writeBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data);

	/** Write single byte to an 8-bit device register.
	 * @param devAddr I2C slave device address
	 * @param regAddr Register address to write to
	 * @param data New byte value to write
	 * @return Status of operation (I2C_OK = success)
	 */
	I2C_ERROR_T writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);

	/** Write single word to a 16-bit device register.
	 * @param devAddr I2C slave device address
	 * @param regAddr Register address to write to
	 * @param data New word value to write
	 * @return Status of operation (I2C_OK = success)
	 */
	I2C_ERROR_T writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data);

	/** Write multiple bytes to an 8-bit device register.
	 * @param devAddr I2C slave device address
	 * @param regAddr First register address to write to
	 * @param length Number of bytes to write
	 * @param data Buffer to copy new data from
	 * @return Status of operation (I2C_OK = success)
	 */
	I2C_ERROR_T writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data);

	/** Write multiple words to a 16-bit device register.
	 * @param devAddr I2C slave device address
	 * @param regAddr First register address to write to
	 * @param length Number of words to write
	 * @param data Buffer to copy new data from
	 * @return Status of operation (I2C_OK = success)
	 */
	I2C_ERROR_T writeWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t* data);
};

/**\example /peripheral/i2c_scanner/src/main.cpp
 * This is an example of how to use the CI2CMaster class.
 * More details about this example.
 */

#endif /* I2C_H_ */
