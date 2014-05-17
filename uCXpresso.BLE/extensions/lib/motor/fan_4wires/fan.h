/*
 ===============================================================================
 Name        : fan.h
 Author      : uCXpresso
 Version     : 1.0.0
 Date		 : 2014/5/17
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description :
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/8/4	v1.0.0	First Edition.									Jason
 2014/5/17	v1.0.1	for uCXpresso.BLE
 ===============================================================================
 */

#ifndef FAN_H_
#define FAN_H_

#include "class/thread.h"
#include "class/pwm.h"
#include "class/pinint.h"
#include "class/timeout.h"

class CFan: public CThread {
public:
	CFan(LPCTSTR name, PWM_CH_T ctrl, PIN_NAME_T sense);
	virtual ~CFan();

	virtual void enable();
	virtual void disable();

	//
	//	set fan control (PWM duty-cycle, 0.5=50%, 0.2=20%...)
	//
	void write(float percentage);

	inline void operator = (float percentage) {
		write(percentage);
	}

	//
	// read fan RPM
	//
	inline operator uint32_t () {
		return m_rpm;
	}

	inline uint32_t rpm() {
		return m_rpm;
	}

protected:
	CPwm 		m_ctrl;
	CPinINT 	m_sense;
	uint32_t 	m_rpmcount, m_rpm;
	LPCTSTR		m_name;

	//
	// thread override function
	//
	virtual void run();
};

#endif /* CFAN_H_ */
