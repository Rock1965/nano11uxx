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

typedef enum {
	TIMER_BUSY = -1,
	TIMER0 = 0,
	TIMER1 = 1,
}TIMER_T;

class CTimer: public CPeripheral {
public:
	CTimer(TIMER_T t);
	virtual ~CTimer();

	//
	// settings
	//
	void second(float sec);

	inline void millisecond(uint32_t ms) {
		second((float) (ms/1000.0f));
	}

	//
	// control
	//
	void enable();	// start
	void disable();	// stop

	inline void begin() {
		enable();
	}

	inline void end() {
		disable();
	}

	void reset();	// restart & recount
	bool wait(uint32_t timeout=MAX_DELAY_TIME);

	//
	// get current count
	uint32_t count();

	static TIMER_T getUnused();	// get Unused timer, if all in used, return -1

	//
	// PRIVATE
	//
	CSemaphore	m_semIrq;
	uint32_t 	m_count;

private:
	TIMER_T  m_timer;
};

#endif /* TIMER_H_ */
