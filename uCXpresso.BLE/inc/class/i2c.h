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

typedef enum {
	I2C_BUSY = 6,
	I2C_NO_DATA = 7,
	I2C_NACK_ON_ADDRESS = 8,
	I2C_NACK_ON_DATA = 9,
	I2C_ARBITRATION_LOST = 10,
	I2C_TIME_OUT = 11,
	I2C_OK = 12
}I2C_ERROR_T;

#define I2C_TIMEOUT	3000


/**I2C base Class
 * \class CI2C i2c.h "class/i2c.h"
 * \ingroup Peripheral
 */
class CI2C: public CPeripheral {
public:
	//
	// Construct
	//
	CI2C();

	//
	// I2C Interface Control
	//
	void enable();
	void disable();
	virtual void frequency(uint32_t hz);
	virtual void onState(uint32_t value);
	virtual bool 	 start();	// Create I2C start condition
	virtual bool 	 stop();	// Set the I2C stop condition
	virtual I2C_ERROR_T engine(uint32_t timeout=I2C_TIMEOUT);

	//
	// I2C Data Read/Write
	//
	virtual void read(uint8_t *data, int length);
	virtual void write(uint8_t *data, int length);

	//
	//	PRIVATE
	//
	virtual 	~CI2C();

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
};

//
// I2C Master Class
//
class CI2CMaster: public CI2C {
public:
	virtual I2C_ERROR_T readwrite(uint8_t slaveAddr, void *txbuf, int txsize, void *rxbuf, int rxsize);
};

//
//
//
class CI2CSlave: public CI2C {
public:
	CI2CSlave(uint8_t slaveAddr);
	virtual I2C_ERROR_T readwrite(uint8_t slaveAddr, void *txbuf, int txsize, void *rxbuf, int rxsize);
};


#endif /* I2C_H_ */
