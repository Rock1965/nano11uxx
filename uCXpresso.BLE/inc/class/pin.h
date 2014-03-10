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
 * \ingroup Enumerations
 */
typedef enum {
	END		= 0,	///< END (null) for CBus class
	LED1 	= 1,	///< On board LED1
	LED2 	= 2,	///< On board LED2
	LED3 	= 3,	///< On board LED3
	LED4 	= 4,	///< On board LED4
	P5 		= 5,	///< Pin 5 / Reset
	P6 		= 6,	///< Pin 6
	P7 		= 7,	///< Pin 7
	P8		= 8,	///< Pin 8
	P9		= 9,	///< Pin 9  / AD0
	P10		= 10,	///< Pin 10 / AD1
	P11		= 11,	///< Pin 11 / AD2
	P12		= 12,	///< Pin 12 / AD3
	P13		= 13,	///< Pin 13 / AD4
	P14		= 14,	///< Pin 14 / AD5
//	P15		= 15,	///< Pin 15 / GND (illegal IO pin)
	P16		= 16,	///< Pin 16 / RXD
	P17		= 17,	///< Pin 17 / TXD
	P18		= 18,	///< Pin 18 / RTS
	P19		= 19,	///< Pin 19 / CTS
	P20		= 20,	///< Pin 20 / PWM1
	P21		= 21,	///< Pin 21 / PWM2
	P22		= 22,	///< Pin 22 / PWM3
	P23		= 23,	///< Pin 23 / PWM4
	P24		= 24,	///< Pin 24 / SCL (Open Drain ONLY)
	P25		= 25,	///< Pin 25 / SDA (OPen Drain ONLY)
	P26		= 26,	///< Pin 26 / SEL
	P27		= 27,	///< Pin 27 / MOSI
	P28		= 28,	///< Pin 28 / MISO
	P29		= 29,	///< Pin 29 / SCK
//	P30		= 30,	///< Pin 30 / V3.3 (illegal IO pin)
}PIN_NAME_T;

/**When PIN as an input, the PIN_MODE control the input mode of all ports.
 * This includes the use of the on-chip pull-up/pull-down resistor feature
 * and a special open drain operating mode.
 * \ingroup Enumerations
*/
typedef enum {
	INTERNAL_PULL_UP = 2,	///< Pin has an on-chip pull-up resistor enabled
	REPEATER = 3,			///< enables the pull-up resistor if the pin is at a logic high, <br/>
							///< enables the pull-down resistor if the pin is at a logic low
	NEITHER	= 0,			///< Pin has neither pull-up nor pull-down resistor enabled
	INTERNAL_PULL_DOWN = 1	///< Pin has an on-chip pull-down resistor enabled
}PIN_INPUT_MODE_T;

/**When pin as an output, the open drain mode causes the pin to be pulled low normally
 * if it is configured as an output and the data value is 0.
 * If the data value is 1, the output drive of the pin is turned off
 * \ingroup Enumerations
 */
typedef enum {
	NOT_OPEN = 0,	///< Pin is in the normal (not open drain) mode
	OPEN_DRAIN = 1	///< Pin is in the open drain mode.
}PIN_OUTPUT_MODE_T;

/**Pin level defined
 * \ingroup Enumerations
 */
typedef enum {
	LOW = 0,		///< LOW level
	HIGH = 1,		///< HIGH level
	LED_OFF = LOW,	///< LED_OFF
	LED_ON = HIGH	///< LED_ON
}PIN_LEVEL_T;

/**Pin define class
 * 	\class CPin pin.h "class/pin.h"
 *	\ingroup Peripherals
 */
class CPin: public CPeripheral {
public:
	/**Constructs a CPin object to connect to the specified pin.
	 * \param p is a PIN_NAME_T enumeration to a pin of peripheral.
	 *
	 * \code
	 * Example:
	 * 		CPin key(P21);		// Create a key object to connect to pin 21.
	 * 		key.input();		// set the key object as a input.
	 *
	 * 		if ( key==LOW ) {	// check the pin level
	 *			...
	 * 		}
	 * \endcode
	 */
	CPin(PIN_NAME_T p);

	/**Set the pin as an output pin.
	 * \code
	 * CPin myPin(P19);
	 * myPin.output();		// set the P19 as an output pin. (use the default NOT_OPEN and LOW level output)
	 * myPin = HIGH;		// set P19 to HIGH (use the operator '=')
	 * myPin.write(LOW);	// set P19 to LOW  (use the write() member)
	 * \endcode
	 * \param mode is a PIN_OUT_MODE_T enumeration to indicate the output mode.
	 * \param defValue is a PIN_LEVEL_T enumeration to set the default level for the output pin.
	 */
	virtual void output(PIN_OUTPUT_MODE_T mode=NOT_OPEN, PIN_LEVEL_T defValue=LOW);

	/**Set as an input pin
	 * \code
	 * PIN_LEVEL_T level;
	 * CPin myPin(P19);
	 * myPin.input(); 		// Set the P19 as an input pin. (with default INTERNAL_PULL_UP feature)
	 * if ( myPin==HIGH ) {	// Read a pin level from myPin. (use the operator '==')
	 * 		...
	 * }
	 * level = myPin;		// Read a pin level from myPin. (use the operator '=')
	 * level = myPin.read();// Read a pin level from myPin. (use the read() member)
	 * \endcode
	 * \param mode is a PIN_INPUT_MODE_T enumeration to indicat the input mode.
	 */
	virtual void input(PIN_INPUT_MODE_T mode=INTERNAL_PULL_UP);

	/**Write a Pin Level to the output pin.
	 * \param val is a PIN_LEVEL_T enumeration to write to the output pin.
	 * \see output
	 */
	virtual void write(PIN_LEVEL_T val);	// write new pin value (for output)

	/**Read a Pin Level from the input pin.
	 * \return PIN_LEVEL_T is HIGH (or LOW) enumeration.
	 * \see input
	 */
	virtual PIN_LEVEL_T  read();			// read current pin value (for input)

	/**Invert an output pin.
	 * \code
	 * CPin myPin(P19);
	 * myPin.output();
	 *
	 * while(1) {
	 * 		myPin.invert();	// Invert the output pin level. (use the invert() member)
	 * //   myPin = !myPin; // Invert the output pin level. (use the '!' operator)
	 * 		sleep(100);
	 * }
	 * \endcode
	 */
	virtual void invert();					// invert current pin output value (for output)

	// Shorthand operators
	inline void operator =(PIN_LEVEL_T val) { this->write(val); }
	inline void operator =(CPin &pin) { this->write(pin.read());}
	inline operator PIN_LEVEL_T() { return read(); }
	inline PIN_LEVEL_T  operator !() { return (read()==HIGH? LOW : HIGH); }
	inline bool operator ==(PIN_LEVEL_T val) { return (this->read()==val ? true : false); }
	inline bool operator !=(PIN_LEVEL_T val) { return (this->read()!=val ? true : false); }

	/**Call the name() member to retrieve the Pin Name of Object
	 * \return PIN_NAME_T name
	 */
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
