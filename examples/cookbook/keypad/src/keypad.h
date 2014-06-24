/*
 ===============================================================================
 Name        : keypad.h
 Author      : uCXpresso
 Version     : 0.0.1
 Date		 : 2014/6/17
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description :
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/6/17	v0.0.1	First Edition.									Arik
 ===============================================================================
 */

#ifndef CKEYPAD_H_
#define CKEYPAD_H_

#include "class/thread.h"
#include "class/timeout.h"
#include "class/bus.h"
#include "class/semaphore.h"

class CKeypad: public CThread {
public:
	CKeypad(CBus &rows, CBus &cols);
	virtual ~CKeypad();

	virtual void enable();		// start to scan
	virtual void disable();		// stop

	//
	// wait for Key code
	//
	int wait(uint32_t timeout = MAX_DELAY_TIME);

protected:
	CSemaphore	m_keydown_event;

	CBus		*m_rows;
	CBus		*m_cols;

	int			m_key_code;

	//
	// thread override function
	//
	virtual void run();
};

#endif /* CKEYPAD_H_ */
