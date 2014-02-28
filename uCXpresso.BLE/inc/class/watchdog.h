/*
 ===============================================================================
 Name        : watchdog.h
 Author      : Jason
 Version     : 1.0.1
 Date		 : 2014/1/9
 Copyright   : Copyright (C) www.embeda.com.tw
  Description : watchdog for nano11Uxx
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	Initialize										Jason
 2014/1/9	v1.0.1	add wait() for warming interrupt				Jason
 ===============================================================================
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include "class/peripheral.h"
#include "class/semaphore.h"

class CWatchdog: public CPeripheral {
public:
	CWatchdog();

	//
	// set watchdog timeout
	//
	void setTimeout(float second);

	//
	// enable watchdog,
	//
	void enable(float warming);

	//
	// disable watchdog
	//
	void disable();

	//
	// Stack Watchdog Feed functions
	//
	static void feed();

	//
	// read WDT clock frequence (Hz)
	//
	static uint32_t frequence();

	//
	// wait for Warming interrupt
	//
	virtual bool wait(uint32_t timeout=MAX_DELAY_TIME);

	// for low power use
	void asWakeupSource();

	// get current watchdog count
	uint32_t count();

	//
	// Private
	//
	virtual ~CWatchdog();

	CSemaphore m_semIrq;
};

#endif /* WATCHDOG_H_ */
