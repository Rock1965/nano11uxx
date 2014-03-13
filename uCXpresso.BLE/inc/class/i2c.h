/*
 ===============================================================================
 Name        : i2c.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2014/1/1
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : I2C driver
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	First Edition for nano11Uxx						Jason
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
	I2C_OK = 12					///<
}I2C_ERROR_T;

#define I2C_TIMEOUT	3000


/**I2C base Class
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

	/**Indicate the receive buffer
	 *
	 */
	virtual void read(uint8_t *data, int length);

	/**Indicate the transmit buffer
	 *
	 */
	virtual void write(uint8_t *data, int length);

	/**Begin transmit and receive the I2C data
	 */
	virtual I2C_ERROR_T engine(uint32_t timeout=I2C_TIMEOUT);

	//
	//	PRIVATE
	/// @cond
	CI2C();
	virtual 	~CI2C();

	virtual void onState(uint32_t value);
	virtual bool start();	// Create I2C start condition
	virtual bool stop();	// Set the I2C stop condition

	// irq & status control
	CSemaphore	m_semIrq;
	CSemaphore	m_semState;
	uint32_t	m_state;
	uint32_t	m_flag;

	// buffer control
	uint8_t		*m_MasterBuffer;
	uint8_t		*m_SlaveBuffer;
	int			m_rdIndex;
	int			m_wrIndex;
	int			m_wrLength;
	int			m_rdLength;

protected:
	xHandle		m_handle;
	/// @endcond
};

/**I2C Master Class
 * \class CI2CMaster i2c.h "class/i2c.h"
 * \ingroup Peripherals
 */
class CI2CMaster: public CI2C {
public:
	/**Transmit and receive the data from I2C interface.
	 *
	 */
	virtual I2C_ERROR_T readwrite(uint8_t slaveAddr, void *txbuf, int txsize, void *rxbuf, int rxsize);
};

/**I2C Slave Class
 * \class CI2CSlave i2c.h "class/i2c.h"
 * \ingroup Peripherals
 */
class CI2CSlave: public CI2C {
public:
	/**CI2CSlave Constructor
	 * \prarm slaveAddr is uint8_t value to indicate the I2C address in slave mode.
	 */
	CI2CSlave(uint8_t slaveAddr);

	/**Transmit & receive the data from I2C Interface.
	 *
	 */
	virtual I2C_ERROR_T readwrite(void *txbuf, int txsize, void *rxbuf, int rxsize);
};


#endif /* I2C_H_ */
