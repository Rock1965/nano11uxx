/*
 ===============================================================================
 Name        : timer.h
 Author      : Jason
 Version     : v1.0.2
 Date		 : 2014/1/8
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/1/31	v1.0.0	First Edition									Jason
 2012/9/20	v1.0.1	Modify getUnused return for TIMER_T				Jason
 2014/1/8	v1.0.2  update for ARDUINO								Jason
 ===============================================================================
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "class/peripheral.h"
#include "class/semaphore.h"

/**Timer channels
 * \ingroup Enumerations
 */
typedef enum {
	TIMER_BUSY = -1,	///< All in used
	TIMER0 = 0,			///< Timer 0
	TIMER1 = 1,			///< Timer 1
}TIMER_T;

/**CTimer class
 * \class CTimer timer.h "class/timer.h
 * \ingroup Peripherals
 */
class CTimer: public CPeripheral {
public:

	/**CTimer constructor
	 * \param t is TIMER_T enumeration to specify a timer channel for the object
	 */
	CTimer(TIMER_T t);


	/**Call the second to set the period time by second.
	 * \param sec is floating value to indicate the time period (unit: second).
	 */
	void second(float sec);

	/**Call the millsecond to set the time period by millisecond.
	 * \param ms is uint32_t value to indicate the tie period (unit: millisecond).
	 */
	inline void millisecond(uint32_t ms) {
		second((float) (ms/1000.0f));
	}

	/**Enable the timer.
	 */
	void enable();	// start

	/**Disable the timer.
	 */
	void disable();	// stop

	/**Reset the time count
	 */
	void reset();	// restart & recount

	/**Waiting for timer interrupt.
	 */
	bool wait(uint32_t timeout=MAX_DELAY_TIME);


	/**Get current time count
	 *
	 */
	uint32_t count();

	/**Get unused timer channel
	 *
	 */
	static TIMER_T getUnused();	// get Unused timer, if all in used, return -1

	//
	// PRIVATE
	/// @cond

	// for arduino user
	inline void begin() { enable(); }
	inline void end() { disable(); }

	virtual ~CTimer();

	CSemaphore	m_semIrq;
	uint32_t 	m_count;

private:
	TIMER_T  m_timer;
	/// @endcond
};

#endif /* TIMER_H_ */
