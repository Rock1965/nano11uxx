/*
 ===============================================================================
 Name        : pinint.h
 Author      : uCXpresso
 Version     : v1.0.1
 Date		 : 2014/8/21
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : Pin Interrupt Class
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	First Edition for nano11Uxx						Jason
 2014/8/21	v1.0.1	Add clear() member to clear pending interrupt.	Jason
 ===============================================================================
 */

#ifndef PININT_H_
#define PININT_H_

#include "class/object.h"
#include "class/semaphore.h"
#include "class/pin.h"
#include "class/list.h"

/**Interrupt Edge Enumerations
 * \ingroup Enumerations
 */
typedef enum {
	FALLING = 0,
	RISING = 1
}EDGE_STATE_T;

/**Pin interrupt service
 * \class CPinINT pinint.h "class/pinint.h"
 * \ingroup Peripherals
 */
class CPinINT: public CPin {
public:
	/**construct for EDGE mode
	 * \param pin to specify a PIN_NAME_T for IRQ.
	 * \param edge to specify the EDGE_STATE_T to active the interrupt.
	 * \param mode to set the PIN_INPUT_MODE_T.
	 */
	CPinINT(PIN_NAME_T pin, EDGE_STATE_T edge, PIN_INPUT_MODE_T mode=INTERNAL_PULL_UP);

	/**construct for LEVEL mode
	 * \param pin to specify a PIN_NAME_T for IRQ.
	 * \param level to specify the PIN_LEVEL_T to active the interrupt.
	 * \param mode to set the PIN_INPUT_MODE_T.
	 */
	CPinINT(PIN_NAME_T pin, PIN_LEVEL_T level, PIN_INPUT_MODE_T mode=INTERNAL_PULL_UP);


	/**enable the interrupt object
	 */
	virtual void enable();

	/**disable the interrupt object
	 */
	virtual void disable();

	/**Clear pending interrupt
	 */
	virtual void clear();

	/**Waiting for interrupt active
	 * \param tm is a waiting timeout and unit in millisecond.
	 * \return trun if an interrupt occurred.
	 */
	virtual bool wait(uint32_t tm=MAX_DELAY_TIME);

	/**Trigger by software
	 */
	virtual void release();

	/**As a weakup source for Power Save Feature
	 * \brief The interrupt will weakup the MCU from power down mode.
	 */
	void asWeakupSource();	// set the pin IRQ as Power weakup source

	// inline function for ARDUINO
	inline void begin() { enable(); }
	inline void end() { disable(); }

	//
	// PRIVATE
	/// @cond
	virtual ~CPinINT();
	CSemaphore	m_semIrq;
	uint32_t	m_event;

protected:
	virtual void assign(PIN_NAME_T pin, PIN_INPUT_MODE_T mode);
	static  int  getFreeChannel();
	int 		m_ch;
	/// @endcond
};

#endif /* PININT_H_ */
