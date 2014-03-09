/*
 ===============================================================================
 Name        : pin.h
 Author      : Jason
 Version     : v1.0.1
 Date		 : 2014/1/12
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : Pin (GPIO) class
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	First Edition for nano11Uxx						Jason
 2014/1/12	v1.0.1	Update pin MAP for GPIO							Jason
 ===============================================================================
 */

#ifndef PIN_H_
#define PIN_H_

#include "class/peripheral.h"

/**Pins defines in enumeration
 * \ingroup Peripheral
 */
typedef enum {
	END		= 0,
	LED1 	= 1,
	LED2 	= 2,
	LED3 	= 3,
	LED4 	= 4,
	P5 		= 5,
	P6 		= 6,
	P7 		= 7,
	P8		= 8,
	P9		= 9,
	P10		= 10,
	P11		= 11,
	P12		= 12,
	P13		= 13,
	P14		= 14,
//	P15		= 15,	GND
	P16		= 16,
	P17		= 17,
	P18		= 18,
	P19		= 19,
	P20		= 20,
	P21		= 21,
	P22		= 22,
	P23		= 23,
	P24		= 24,	// Open Drain ONLY
	P25		= 25,	// OPen Drain ONLY
	P26		= 26,
	P27		= 27,
	P28		= 28,
	P29		= 29,
//	P30		= 30,	V3.3
}PIN_NAME_T;

/**When PIN as an input, the PIN_MODE control the input mode of all ports.
 * This includes the use of the on-chip pull-up/pull-down resistor feature
 * and a special open drain operating mode.
 * \ingroup Peripheral
*/
typedef enum PIN_INPUT_MODE {
	INTERNAL_PULL_UP = 2,	///< Pin has an on-chip pull-up resistor enabled
	REPEATER = 3,			///< enables the pull-up resistor if the pin is at a logic high, <br/>
							///< enables the pull-down resistor if the pin is at a logic low
	NEITHER	= 0,			///< Pin has neither pull-up nor pull-down resistor enabled
	INTERNAL_PULL_DOWN = 1	///< Pin has an on-chip pull-down resistor enabled
}PIN_INPUT_MODE_T;

/**When pin as an output, the open drain mode causes the pin to be pulled low normally
 * if it is configured as an output and the data value is 0.
 * If the data value is 1, the output drive of the pin is turned off
 * \ingroup Peripheral
 */
typedef enum PIN_OUTPUT_MODE {
	NOT_OPEN = 0,	///< Pin is in the normal (not open drain) mode
	OPEN_DRAIN = 1	///< Pin is in the open drain mode.
}PIN_OUTPUT_MODE_T;

typedef enum PIN_LEVEL {
	LOW = 0,
	HIGH = 1,
	LED_OFF = LOW,
	LED_ON = HIGH
}PIN_LEVEL_T;

/**Pin define class
 * 	\class CPin pin.h "class/pin.h"
 *	\ingroup Peripheral
 */
class CPin: public CPeripheral {
public:
	/**Constructs a CPin object to connect to the specified pin.
	 * \param p ... are PIN_NAME_T to specified a pin to the object.
	 *
	 * \code
	 * Example:
	 * 		CPin key(P21);		// Create a key object to connect to pin 21.
	 * 		key.input();		// set the key object as a input.
	 *
	 * 		if ( key==LOW ) {	// check the pin level
	 *
	 * 		}
	 * \endcode
	 *
	 * \remark to 'END' of the pin arguments is MUST!!
	 */
	CPin(PIN_NAME_T p);

	// set as an output pin
	virtual void output(PIN_OUTPUT_MODE_T mode=NOT_OPEN, PIN_LEVEL_T defValue=LOW);

	// set as an input pin
	virtual void input(PIN_INPUT_MODE_T mode=INTERNAL_PULL_UP);

	virtual void write(PIN_LEVEL_T val);	// write new pin value (for output)
	virtual PIN_LEVEL_T  read();			// read current pin value (for input)
	virtual void invert();					// invert current pin output value (for output)

	// a shorthand operator
	inline void operator =(PIN_LEVEL_T val) { this->write(val); }
	inline void operator =(CPin &pin) { this->write(pin.read());}
	inline operator PIN_LEVEL_T() { return read(); }
	inline PIN_LEVEL_T  operator !() { return (read()==HIGH? LOW : HIGH); }
	inline bool operator ==(PIN_LEVEL_T val) { return (this->read()==val ? true : false); }
	inline bool operator !=(PIN_LEVEL_T val) { return (this->read()!=val ? true : false); }
	inline PIN_NAME_T name() { return (PIN_NAME_T)m_nPin; }

	/*! \cond PRIVATE */
	CPin();
	virtual ~CPin();
	virtual void assign(PIN_NAME_T p);		// assign pin number to CPin

protected:
	int m_nPin;
	/*! \endcond */
};

#endif /* PIN_H_ */
