/*
 ===============================================================================
 Name        : dcmotor.h
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

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include <class/pin.h>
#include <class/pwm.h>

typedef enum {
	FORWARD = HIGH,
	REVERSE = LOW
}MOTOR_ACTION_T;

class DCMotor: public CObject {
public:
	DCMotor(PWM_CH_T pwm, PIN_NAME_T dir);

	void enable();
	void disable();

	// a short hand code for speed
	DCMotor&  operator = (float percent);

	// a short hand code for action
	DCMotor& operator = (MOTOR_ACTION_T action);

	/// @cond
	virtual ~DCMotor();
protected:
	CPwm	m_pwm;
	CPin	m_dir;
	/// @endcond
};

#endif /* DCMOTOR_H_ */
