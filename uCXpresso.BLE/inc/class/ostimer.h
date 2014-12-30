/*
 ===============================================================================
 Name        : osTimer
 Author      : uCXpresso
 Version     : v1.0.0
 Date		 : 2012/5/16
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : Timer service provides by RTOS
 History     :
 ===============================================================================
 */

#ifndef _OS_TIMER_H_
#define _OS_TIMER_H_

#include "class/object.h"
#include "class/thread.h"
#include "class/timeout.h"

/**
 * @brief osTimer provide the RTOS timer service
 * @class osTimer ostimer.h "class/ostimer.h"
 * @ingroup RTOS
 */
class osTimer: public CThread {
	/**
	 * @brief timer callback handle
	 */
	typedef void (*timer_handle_t) (osTimer *p_timer, xHandle param);
public:

	/**
	 * @brief Constructs the osTimer class
	 * @param period Set timer period in millisecond.
	 * @param handle Pointer to the timer event handle function.
	 * @param param Pointer to the timer event handle parameter.
	 *
	 * @note When timer handle is NULL, the osTimer will call the onTimer member function.
	 */
	osTimer(uint32_t period, timer_handle_t handle=NULL, xHandle param=NULL);

	/**
	 * @brief Start the timer object
	 * @param name	Set the name of timer task.
	 * @param stack Set the stack size of timer task.
	 */
	virtual bool start(LPCTSTR name ="timer", uint32_t stack = DEF_THREAD_STACK);

	/**
	 * @brief Reset the timer.
	 */
	virtual void reset();

	/**
	 * @brief Stop the timer
	 */
	virtual void stop();

	/**
	 * @brief Check the timer active or not.
	 * @note
	 *	A timer will be dormant if:
	 *     1) It has been created but not started, or
	 *     2) It is in stoped.
	 * @return true if the timer is active, otherwise the timer is inactive.
	 */
	virtual bool isActive();

	/**
	 * @brief Timer event handle.
	 * @note If timer handle is NULL, the member function will be called when time expired occurred.
	 */
	virtual void onTimer() { NOTHING }

	//
	///@cond
	//
	virtual ~osTimer();
private:
	CTimeout 		m_timer;
	timer_handle_t	m_handle;
	xHandle			m_param;
	uint32_t		m_period;
	virtual void run();
	///@endcond
};

/**
 * @example /rtos/rtos_timer/src/main.cpp
 * This is an example of how to use the osTimer class.
 * More details about this example.
 */

#endif /* TIMER_H_ */
