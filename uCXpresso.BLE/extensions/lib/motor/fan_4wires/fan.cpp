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

#include "fan.h"

#define FAN_RPM_RESOLUTION	30
#define FAN_RPM_TIMEOUT		300		// 300ms

CFan::CFan(LPCTSTR name, PWM_CH_T ctrl, PIN_NAME_T sense) :
m_ctrl(ctrl),
m_sense(sense, FALLING)
{
	m_rpmcount = 0;
	m_rpm = 0;
	m_name = name;
}

CFan::~CFan() {
	disable();
}

void CFan::enable() {

	m_ctrl.dutyCycle(0.0);	// turn off FAN
	m_ctrl.enable();		// start PWM
	m_sense.enable();		// start Interrupt

	if ( isSuspend() ) {
		resume();
	} else {
		start(m_name, 128, PRI_HARDWARE);
	}
}

void CFan::disable() {
	m_ctrl.dutyCycle(0.0);
	m_ctrl.disable();
	m_sense.disable();
	suspend();
}

void CFan::write (float percentage) {
	m_ctrl.dutyCycle(percentage);
}

void CFan::run() {
	CTimeout tm;			// time count for RPM
	tm.reset();
	while(isAlive()) {		// check thread alive

		//
		// wait for sense pin trigger (interrupt)
		//
		if ( m_sense.wait(FAN_RPM_TIMEOUT) ) {
			if ( m_sense==LOW ) {	// filter noise (for Falling only)
				//
				// RPM calculate
				//
				m_rpmcount++;
				if ( m_rpmcount>=20 ) {
					// Update RPM every 20 counts, increase this for better RPM resolution,
					// decrease for faster update
					m_rpm = (FAN_RPM_RESOLUTION * 1000 * m_rpmcount) / tm.elapsed();
					tm.reset();
					m_rpmcount = 0;

				} // end of if ( m_rpmcount>=20 )
			}

		} else {
			//
			// timeout
			//
			m_ctrl.dutyCycle(0.0f);	// turn off fan
			m_rpm = 0;
		}
	}
}
