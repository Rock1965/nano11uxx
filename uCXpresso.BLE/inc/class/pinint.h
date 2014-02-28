/*
 ===============================================================================
 Name        : pinint.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2014/1/1
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : Pin Interrupt Class
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	First Edition for nano11Uxx						Jason
 ===============================================================================
 */

#ifndef PININT_H_
#define PININT_H_

#include "class/object.h"
#include "class/semaphore.h"
#include "class/pin.h"
#include "class/list.h"

typedef enum {
	FALLING = 0,
	RISING = 1
}EDGE_STATE_T;

class CPinINT: public CPin {
public:
	// construct for EDGE mode
	CPinINT(PIN_NAME_T pin, EDGE_STATE_T edge, PIN_INPUT_MODE_T mode=INTERNAL_PULL_UP);

	// construct for LEVEL mode
	CPinINT(PIN_NAME_T pin, PIN_LEVEL_T level, PIN_INPUT_MODE_T mode=INTERNAL_PULL_UP);


	// enable / disable
	virtual void enable();
	virtual void disable();
	inline void begin() { enable(); }
	inline void end() { disable(); }

	virtual bool wait(uint32_t tm=MAX_DELAY_TIME);

	// trigger by software
	virtual void release();

	//
	// for Power Management Unit (PMU)
	//
	void asWeakupSource();	// set the pin IRQ as Power weakup source

	//
	// PRIVATE
	//
	virtual ~CPinINT();
	CSemaphore	m_semIrq;
	uint32_t	m_event;

protected:
	virtual void assign(PIN_NAME_T pin, PIN_INPUT_MODE_T mode);
	static  int  getFreeChannel();

	int 		m_ch;
};

#endif /* PININT_H_ */
