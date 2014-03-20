/*
 ===============================================================================
 Name        : servo.cpp
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2014/3/20
 License   	 : MIT
 Description : Servo Motor Control
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/3/20 v1.0.0	First Edition.									Jason
 ===============================================================================
 */

#include "servo.h"

#if 0
#include <debug.h>
#define DBG	dbg_printf
#else
#define DBG(...)
#endif

CServo::CServo(PWM_CH_T ch):m_pwm(ch) {
	settings(MIN_PULSE_WIDTH, MAX_PULSE_WIDTH, MAX_ANGLE);
	CPwm::period(SERVO_PWM_PERIOD);		// set global PWM period time
	m_pwm = 0;
}

void CServo::settings(int minPulseWidth, int maxPulseWidth, int maxAngle) {
	m_minPulseWidth = minPulseWidth;
	m_maxPulseWidth = maxPulseWidth;
	m_maxAngle = maxAngle;
}

CServo::~CServo() {
	disable();
}

void CServo::enable() {
	m_pwm.enable();
}

void CServo::disable() {
	m_pwm.disable();
}

void CServo::write(int value) {
	float pw;

	if ( value<m_minPulseWidth ) {
		value = constrain(value, 0, m_maxAngle);
		// get new pulse width
		pw = map(value, 0, m_maxAngle, m_minPulseWidth, m_maxPulseWidth);
	} else {
		pw = value;
	}

	// convert microsecond to second
	pw = pw / 1000000.0;

	DBG("servo min=%d, max=%d, val=%d, pw=%0.6f\n", m_minPulseWidth, m_maxPulseWidth, value, pw);

	// set new pulse width to servo motor
	m_pwm.pulseWidth(pw);
}

