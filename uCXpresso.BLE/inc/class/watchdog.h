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

/**Watchdog Timer class.
 * \class CWatchdog watchdog.h "class/watchdog.h"
 * \ingroup Peripherals
 */
class CWatchdog: public CPeripheral {
public:
	/**CWatchdog constructor
	 * \remark Don't use the CWatchdog class when you use the CPowerSave feature.
	 */
	CWatchdog();

	/**set watchdog timeout*/
	void setTimeout(float second);


	/**enable watchdog
	 * \brief warming is a high water mask for watchdog time count. that will trigger a warming interrupt.
	 * \param warming is floating value (unit second), set to zero will cause system reset when watchdog timeout.
	 */
	void enable(float warming=0);

	/**disable watchdog
	 */
	void disable();

	/**Watchdog Feed functions. (static function)
	 */
	static void feed();

	/**Read WDT clock frequence (Hz)
	 */
	static uint32_t frequence();

	/**Waiting for Warming interrupt
	 * \param timeout in the wait() member function.
	 */
	virtual bool wait(uint32_t timeout=MAX_DELAY_TIME);

	/* for Save Power Feature
	 */
	void asWakeupSource();

	/**get current watchdog count
	 */
	uint32_t count();

	//
	// Private
	//
	/// @cond
	virtual ~CWatchdog();

	CSemaphore m_semIrq;
	/// @endcond
};

#endif /* WATCHDOG_H_ */
