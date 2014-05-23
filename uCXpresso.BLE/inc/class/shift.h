/*
 ===============================================================================
 Name        : shift.h
 Author      : uCXpresso
 Version     : v1.0.0
 Date		 : 2014/5/22
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : Shift In/Out class
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/5/22	v1.0.0	First Edition									Jason
 ===============================================================================
 */

#ifndef SHIFT_H_
#define SHIFT_H_

#include <class/peripheral.h>
#include <class/pin.h>

/**Shift In/Out bit order.
 * \ingroup Enumerations
 */
typedef enum {
	SF_MSBFIRST = 0,
	SF_LSBFIRST = 1
}SF_BIT_ORDER_T;

/**CShift class provides the shift bit input and output.
 * \class CShift shift.h "class/shift.h"
 * \ingroup Peripherals
 */
class CShift: public CPeripheral {
public:
	/**Construct CShift
	 * \code
	 * uint8_t value;
	 * CShift shift(P5, P6);	// P5=DATA, P6=CLOCK
	 *
	 * shift = 0x80;			// output 0x80;
	 * value = shift;			// input shift to value
	 *
	 * \endcode
	 * \param pinDAT is PIN_NAME_T to specify to the DATA pin.
	 * \param pinCLK is PIN_NAME_T to specify to the CLOCK pin.
	 * \param order indicate the shift bit order (optional).
	 */
	CShift(PIN_NAME_T pinDAT, PIN_NAME_T pinCLK, SF_BIT_ORDER_T order=SF_MSBFIRST);

	/**Input the shift data
	 *	\return a uint8_t (8 bits) data;
	 */
	virtual uint8_t input();


	/**Output the shift data
	 *	\return a uint8_t (8 bits) data;
	 */
	virtual void output(uint8_t val);

	//
	// short-hand operators
	//
	inline operator uint8_t () {
		return input();
	}

	inline void operator = (uint8_t val) {
		output(val);
	}

	inline CShift& operator << (uint8_t val) {
		output(val);
		return *this;
	}

	inline CShift& operator >> (uint8_t &val) {
		val = input();
		return *this;
	}

	/*! \cond PRIVATE */
protected:
	CPin	m_pinDAT;
	CPin	m_pinCLK;
	SF_BIT_ORDER_T	m_order;
	/*! \endcond */
};

#endif /* SHIFT_H_ */
