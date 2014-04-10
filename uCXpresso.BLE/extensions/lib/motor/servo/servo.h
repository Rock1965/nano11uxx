/*
 ===============================================================================
 Name        : servo.h
 Author      : uCXpresso
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

#ifndef SERVO_H_
#define SERVO_H_

#include <class/pwm.h>

#define SERVO_PWM_PERIOD	0.02f		// servo motor period time 20ms

#define	MAX_PULSE_WIDTH		2400		// 2400 us
#define MIN_PULSE_WIDTH		544			// 544  us
#define DEF_PULSE_WIDTH		1500		// 1500 us
#define MAX_ANGLE			180

class CServo: public CPeripheral {
public:
	/**CServo Constructor
	 * \param ch is PWM_CH_T to indicate the PWM channel pin to the Servo Motor.
	 */
	CServo(PWM_CH_T ch);

	virtual void enable();
	virtual void disable();

	/**Write new angle to servo motor
	 * \param angle is integer value to set the servo motor angle
	 */
	virtual void write(int angle);

	/**Operator =, to write angle in the object
	 */
	virtual inline void operator = (int angle) {
		write(angle);
	}

	/**set the Servo parameters
	 * \param ch is PWM_CH_T to indicate the PWM channel
	 * \param minPulseWidth is min. pulse width in microsecond.
	 * \param maxPulseWidth is max. pulse width in microsecond.
	 */
	void settings(int minPulseWidth, int maxPulseWidth, int maxAngle=MAX_ANGLE);

	/// @cond
	virtual ~CServo();
protected:
	CPwm	m_pwm;
	int	 	m_minPulseWidth;
	int	 	m_maxPulseWidth;
	int		m_maxAngle;
	/// @endcond
};

#endif /* SERVO_H_ */
