/*
 ===============================================================================
 Name        : spi.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2014/1/1
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : SPI class
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	First Edition for nano11Uxx						Jason
 ===============================================================================
 */

#ifndef SPI_H_
#define SPI_H_

#include "class/peripheral.h"
#include "class/stream.h"
#include "class/pin.h"

/**********************************************************
 SPI Data Bits between the 8 and 16
 **********************************************************/
typedef enum {
	SPI0 = 0,
	SPI1 = 1
}SPI_PORT_T;

/**********************************************************
 SPI Data Bits between the 8 and 16
 **********************************************************/
typedef enum {
	SPI_DATABIT_4	= 3,
	SPI_DATABIT_5	= 4,
	SPI_DATABIT_6	= 5,
	SPI_DATABIT_7	= 6,
	SPI_DATABIT_8	= 7,	// Default
	SPI_DATABIT_9 	= 8,
	SPI_DATABIT_10 	= 9,
	SPI_DATABIT_11 	= 10,
	SPI_DATABIT_12 	= 11,
	SPI_DATABIT_13 	= 12,
	SPI_DATABIT_14 	= 13,
	SPI_DATABIT_15 	= 14,
	SPI_DATABIT_16 	= 15
}SPI_DATABIT_T;

/**********************************************************
 SPI Data To Clock Phase Relationship
	mode | POL PHA
	-----+---------
	  0  |  0   0  (Default)
	  1  |  0   1
	  2  |  1   0
	  3  |  1   1
 **********************************************************/
typedef enum {
	SPI_MODE_0 = 0,
	SPI_MODE_1,
	SPI_MODE_2,
	SPI_MODE_3
}SPI_MODE_T;

typedef enum {
	SPI_MSBFIRST = 0,	// default
	SPI_LSBFIRST = 1
}SPI_BIT_ORDER_T;

typedef enum {
	SPI_MASTER = 0,
	SPI_SLAVE = 1
}SPI_ROLE_T;


// *****************************************************************************
// Class Name: CSPI
// Description:
// The SPI base class to define hardware interface
// Default frequency = 2MHz
// *****************************************************************************
class CSPI: public CPeripheral {
public:
	/*
	 * Construct CSPI(int port)
	 * 		Descriptions:
	 * 			To assign a SPI port for the class
	 * 		Parameters:
	 * 			port: SPI0 or SPI1
	 * 			pullup: SCK/MOSI/MISO internal pullup, true=pullup, false=neither
	 * 		Return:
	 * 			bool : true = successful.
	 */
	CSPI(SPI_PORT_T port=SPI0, bool pullup=true);
	virtual ~CSPI();

	/*
	 * CSPI::format(SPI_DATABIT_T bits, SPI_MODE_T mode)
	 *		Descriptions:
	 *			To set the SPI interface format.
	 *		Parameters:
	 *			bits: SPI_DATABIT_8~SPI_DATABIT_16 (Default 8)
	 *			mode: SPI Data To Clock Phase Relationship:
	 *					mode | POL PHA
	 *					-----+---------
	 * 	  	  	  	  	  0  |  0   0  (Default)
	 * 	  	  	  	  	  1  |  0   1
	 * 	  	  	  	  	  2  |  1   0
	 *					  3  |  1   1
	 *		Return:
	 *			None (void)
	 */
	void format(SPI_DATABIT_T bits, SPI_MODE_T mode=SPI_MODE_0);

	/*
	 * CSPI::bitOrder(SPI_BIT_ORDER value)
	 * 		Descriptions:
	 * 			To set the SPI bit sequence of data-byte.
	 *
	 * 		Parameter:
	 * 			SPI_MSBFIRST (Default)
	 * 			SPI_LSBFIRST
	 *
	 * 		Return:
	 * 			None.
	 */
	void bitOrder(SPI_BIT_ORDER_T value);

	/*
	 * CSPI::frequency(hz)
	 * 		Descriptions:
	 * 			Set the SPI data clock rate
	 * 		Parameters:
	 * 			hz = a clock rate, (Default 2Mhz, Max 25Mhz)
	 * 		Return:
	 * 			None (void)
	 */
	bool frequency(uint32_t hz);

	virtual void enable(SPI_ROLE_T role=SPI_MASTER); 	// enable SPI
	virtual void disable();								// disable SPI

	bool isSlave();

	// for arduino user
	inline void begin(SPI_ROLE_T role=SPI_MASTER) { enable(role); }
	inline void end() { disable(); }

public:
	virtual int readwrite(void *txbuf, void *rxbuf, uint32_t length);

	inline int readwrite(uint8_t tx, uint8_t &rx) {
		return readwrite(&tx, &rx, 1);
	}

	inline uint8_t readwrite(uint8_t tx) {
		uint8_t rx;
		readwrite(&tx, &rx, 1);
		return rx;
	}

	inline uint8_t write(uint8_t tx) {
		uint8_t rx;
		readwrite(&tx, &rx, 1);
		return rx;
	}

	// shorthand operator
	inline void operator >> (uint8_t &val) { readwrite(NULL, &val, 1); }
	inline void operator << (uint8_t val)  { readwrite(&val, NULL, 1); }
	inline void operator >> (uint16_t &val) { readwrite(NULL, &val, 2); }
	inline void operator << (uint16_t val)  { readwrite(&val, NULL, 2); }
	inline void operator >> (uint32_t &val) { readwrite(NULL, &val, 4); }
	inline void operator << (uint32_t val)  { readwrite(&val, NULL, 4); }

protected:
	SPI_PORT_T 	m_port;
	uint32_t	m_nFlag;
};

// *****************************************************************************
// Class Name: CSPI
// Description:
// The SPI base class to define hardware interface
// Default frequency = 2MHz
// *****************************************************************************
class CSpiMaster: public CSPI {
protected:
	CPin		m_cs;	// chip select pin

public:
	/*
	 * CSpiMaster Construct
	 * 		Descriptions:
	 * 			To assign a SPI port and chip select pin for the class
	 * 		Parameters:
	 * 			port: SPI0 or SPI1
	 * 			csPin: chip select pin
	 * 		Return:
	 * 			None
	 */
	CSpiMaster(PIN_NAME_T selPin);

	virtual int readwrite(void *txbuf, void *rxbuf, uint32_t length);
};

#endif /* SPI_H_ */
