/*
 ===============================================================================
 Name        : power.h
 Author      : uCXpresso
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

/**Power Save Mode Enumeration
 * \ingroup Enumerations
 */
typedef enum {
	DEEP_SLEEP = 1,	///< deep sleep
	POWER_DOWN = 2	///< power down
}PS_MODE_T;

/**CPowerSave provides the low power feature by Tickless Technology.
 * \remark Don't use the CPowerSave and CWatchdog together, because the CPowerSave will use the watchdog as a weakup source.
 * \class CPowerSave power.h "class/power.h"
 * \ingroup Peripherals
 */
class CPowerSave: public CPeripheral {
public:

	/**Enable the power save feature
	 * \param mode is a PS_MODE_T to indicate the power save mode.
	 */
	void enable(PS_MODE_T mode=DEEP_SLEEP);

	/**Disable the power save feature
	 */
	void disable();

	//
	// PRIVATE (Internal use)
	/// @cond
	CPowerSave();
	virtual ~CPowerSave();
	virtual void onSleep(uint32_t ulExpectedIdleTime);
	virtual void onWakeup(uint32_t *pExpectedIdleTime);
	virtual bool isEnable();

protected:
	uint32_t  m_flag;
	CWatchdog m_wdt;	// a wakeup source
	/// @endcond
};

#endif /* PMU_H_ */
