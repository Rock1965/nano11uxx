/*
 ===============================================================================
 Name        : spi.h
 Author      : uCXpresso
 Version     : v1.0.1
 Date		 : 2014/3/14
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : SPI class
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	First Edition for nano11Uxx						Jason
 2014/3/14	v1.0.1	modify inline write() member function			Jason
 ===============================================================================
 */

#ifndef SPI_H_
#define SPI_H_

#include "class/peripheral.h"
#include "class/stream.h"
#include "class/pin.h"

/**SPI Data Bits between the 8 and 16
 * \ingroup Enumerations
 */
typedef enum {
	SPI0 = 0,	///< default spi port
	SPI1 = 1	///< internal used!!
}SPI_PORT_T;

/**SPI Data Bits between the 8 and 16
 * \ingroup Enumerations
 */
typedef enum {
	SPI_DATABIT_4	= 3,
	SPI_DATABIT_5	= 4,
	SPI_DATABIT_6	= 5,
	SPI_DATABIT_7	= 6,
	SPI_DATABIT_8	= 7,	///< Default
	SPI_DATABIT_9 	= 8,
	SPI_DATABIT_10 	= 9,
	SPI_DATABIT_11 	= 10,
	SPI_DATABIT_12 	= 11,
	SPI_DATABIT_13 	= 12,
	SPI_DATABIT_14 	= 13,
	SPI_DATABIT_15 	= 14,
	SPI_DATABIT_16 	= 15
}SPI_DATABIT_T;

/**SPI Data To Clock Phase Relationship
 * \ingroup Enumerations
 */
typedef enum {
	SPI_MODE_0 = 0,	///< Mode 0: POL=0 / PHA=0
	SPI_MODE_1,		///< Mode 1: POL=0 / PHA=1
	SPI_MODE_2,		///< Mode 2: POL=1 / PHA=0
	SPI_MODE_3		///< Mode 3: POL=1 / PHA=1
}SPI_MODE_T;

/**SPI Bit Order
 * \ingroup Enumerations
 */
typedef enum {
	SPI_MSBFIRST = 0,	///< default
	SPI_LSBFIRST = 1	///
}SPI_BIT_ORDER_T;

/**SPI Role
 * \ingroup Enumerations
 */
typedef enum {
	SPI_MASTER = 0,		///< Master
	SPI_SLAVE = 1		///< Slave
}SPI_ROLE_T;


/**CSPI class provides the Serial Peripheral Interface.
 * \class CSPI spi.h "class/spi.h"
 * \ingroup Peripherals
 */
class CSPI: public CPeripheral {
public:
	/**Construct CSPI
	 * \code
	 * CSPI spi;
	 * spi.format(SPI_DATABIT_8);	// data bit=8 and default mode = 0
	 * spi.frequency(MHZ(4));		// set frequency to 4MHz
	 * spi.enable();				// enable spi with default Master Role
	 * \endcode
	 * \param port is SPI_PORT_T to assign a SPI port for the class
	 * \param pullup is boolean to enable the internal pullup resistor for MISO and MISO and CLK pins
	 */
	CSPI(SPI_PORT_T port=SPI0, bool pullup=true);

	/**Set the SPI format <br/>
	 *	\param bits is SPI_DATABIT_T enumerations.
	 *	\param mode is SPI_MODE_T enumerations.
	 */
	void format(SPI_DATABIT_T bits, SPI_MODE_T mode=SPI_MODE_0);

	/**Set the SPI bit order
	 * \param value is a SPI_BIT_ORDER_T enumeration to indicate the bit order.
	 */
	void bitOrder(SPI_BIT_ORDER_T value);

	/**Set the SPI CLK frequency
	 * \param hz is unsigned long value to set the frequency of spi interface.
	 * \remark: SPI slave don't need to set the frequency of spi.
	 */
	bool frequency(uint32_t hz);

	/**Enable the SPI interface */
	virtual void enable(SPI_ROLE_T role=SPI_MASTER); 	// enable SPI

	/**Disable the SPI interface */
	virtual void disable();								// disable SPI

	/**Check the object is SPI slave or not
	 * \return true if the spi object is in slave role, otherwise is master role.
	 */
	bool isSlave();

	//
	// for arduino user
	//
	inline void begin(SPI_ROLE_T role=SPI_MASTER) { enable(role); }
	inline void end() { disable(); }

public:
	/**Transmit and receive the data block from SPI interface.
	 * \param txbuf is a point to transmit buffer.
	 * \param rxbuf is a point to receive buffer.
	 * \param length is indicate the both buffer size.
	 * \return transmit or receive length.
	 */
	virtual int readwrite(void *txbuf, void *rxbuf, uint32_t length);

	/**Transmit and receive a byte (overload functions)
	 * \param tx is a byte which send to spi interface.
	 * \param rx is a byte which receive from spi interface.
	 * \return 1 if successful, otherwise failed.
	 */
	virtual inline int readwrite(uint8_t tx, uint8_t &rx) {
		return readwrite(&tx, &rx, 1);
	}

	/**Transmit and receive a byte (overload functions)
	 * \param tx is a byte which send to spi interface.
	 * \return a byte which receive from spi interface.
	 */
	virtual inline uint8_t write(uint8_t tx) {
		uint8_t rx;
		readwrite(&tx, &rx, 1);
		return rx;
	}

	//
	// inline shorthand operators
	//
	inline void operator >> (uint8_t &val) { readwrite(NULL, &val, 1); }
	inline void operator << (uint8_t val)  { readwrite(&val, NULL, 1); }
	inline void operator >> (uint16_t &val) { readwrite(NULL, &val, 2); }
	inline void operator << (uint16_t val)  { readwrite(&val, NULL, 2); }
	inline void operator >> (uint32_t &val) { readwrite(NULL, &val, 4); }
	inline void operator << (uint32_t val)  { readwrite(&val, NULL, 4); }

	/// @cond
	virtual ~CSPI();

protected:
	SPI_PORT_T 	m_port;
	uint32_t	m_nFlag;
	/// @endcond
};

/**CSpiMaster class and inherit from CSPI class, and provides the master sel pin.
 * \class CSpiMaster spi.h "class/spi.h"
 * \ingroup Peripherals
 */
class CSpiMaster: public CSPI {
public:
	/**CSpiMaster Constructor
	 * \param selPin is a PIN_NAME_T to indicate a user's SEL pin for SPI interface.
	 * \param pullpu is a boolean to enable the internal pull-up resistors for SPI interface.
	 */
	CSpiMaster(PIN_NAME_T selPin, bool pullup=true);

	/**Override function, to transmit and receive the SPI data block. <br/>
	 * The function provides the SEL pin function in the readwrite member function.<br/>
	 */
	virtual int readwrite(void *txbuf, void *rxbuf, uint32_t length);

	/// @cond
protected:
	CPin		m_cs;	// chip select pin
	/// @endcond
};

/**\example /peripheral/spi/src/main.cpp
 * This is an example of how to use the CSpiMaster class.
 * More details about this example.
 */

#endif /* SPI_H_ */
