/*
 ===============================================================================
 Name        : dcmotor.cpp
 Author      : uCXpresso
 Version     : 1.0.0
 Date		 : 2014/4/6
 License   	 : MIT
 Description : DC Motor Control with H-Bridge
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/4/6	v1.0.0	First Edition.									Jason
 ===============================================================================
 */

#include "dcmotor.h"

#if 0
#include <debug.h>
#define DBG	dbg_printf
#else
#define DBG(...)
#endif


DCMotor::DCMotor(PWM_CH_T pwm, PIN_NAME_T dir) : m_pwm(pwm), m_dir(dir)
{
	m_pwm = 0;
	m_dir.output(NOT_OPEN);
	m_dir = HIGH;
}

DCMotor::~DCMotor() {
	disable();
}

void DCMotor::enable() {
	m_pwm.enable();
}

void DCMotor::disable() {
	m_pwm.disable();
}

DCMotor& DCMotor::operator = (float percent) {
	m_pwm.write(percent);
	DBG("pwm=%0.2f\n", percent);
	return *this;
}

DCMotor& DCMotor::operator = (MOTOR_ACTION_T action) {
	m_dir = (PIN_LEVEL_T)action;
	DBG("pin=%d\n", action);
	return *this;
}
