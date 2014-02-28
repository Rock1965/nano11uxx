/*
 ===============================================================================
 Name        : power.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2014/1/12
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : Power Save for nano11Uxx
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/12	v1.0.0	first edition for nano11Uxx						Jason
 ===============================================================================
 */

#ifndef POWER_H_
#define POWER_H_

#include "class/peripheral.h"
#include "class/watchdog.h"

typedef enum {
	DEEP_SLEEP = 1,
	POWER_DOWN = 2
}PS_MODE_T;

class CPowerSave: public CPeripheral {
public:
	CPowerSave();
	void enable(PS_MODE_T mode=DEEP_SLEEP);
	void disable();

	//
	// PRIVATE (Internal use)
	//
	virtual ~CPowerSave();
	virtual void onSleep(uint32_t ulExpectedIdleTime);
	virtual void onWakeup(uint32_t *pExpectedIdleTime);
	virtual bool isEnable();

protected:
	uint32_t  m_flag;
	CWatchdog m_wdt;	// a wakeup source
};

#endif /* PMU_H_ */
