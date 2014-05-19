/*
 ===============================================================================
 Name        : pwm.h
 Author      : uCXpresso
 Version     : v1.0.2
 Date		 : 2014/5/19
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : PWM class
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	First Edition for nano11Uxx						Jason
 2014/5/6	v1.0.1	Fully LOW level for duty-cycle zero.			Jason
 2014/5/19	v1.0.2	Add PWM Group									Jason
 ===============================================================================
 */

#ifndef PWM_H_
#define PWM_H_

#include "class/peripheral.h"

/**PWM Channels
 * \ingroup Enumerations
 */
typedef enum {
	PWM1 = 1,	///< PWM1 (P20)
	PWM2,		///< PWM2 (P21)
	PWM3,		///< PWM3 (P22)
	PWM4,		///< PWM4 (P23)
}PWM_CH_T;

/**PWM GROUP
 * \ingroup Enumerations
 */
typedef enum {
	PG_ALL = 0,	///< All group for PWM1, PWM2, PWM3, PWM4
	PG_1,		///< PWM Group 1 for PWM1 and PWM2
	PG_2		///< PWM Group 2 for PWM3 and PWM4
}PWM_GROUP_T;

/*! \def Max frequency
 */
#define MAX_PWM_FREQUENCY	KHZ(20)	// 20KHz (period time: 50us +/- 5us)

/**Pulse-width modulated output.
 * \class CPwm pwm.h "class/pwm.h"
 * \ingroup Peripherals
 */
class CPwm: public CPeripheral {
public:
	/**Constructs a CPwm object to connect to the specified PWM channel.
	 * \param ch ... are PWM_CH_T to specified a PWM channel to the object.
	 *
	 * \code
	 * Example:
	 * 		CPwm::period(0.02);		// Set global PWM period time = 20ms
	 *
	 * 		CPwm servo(PWM1);		// create a servo objecj
	 * 		servo.dutyCycle(0.8);	// set servo object to 80% dutyCycle
	 * 		servo.enable();			// begin the servo PWM output
	 * \endcode
	 */
	CPwm(PWM_CH_T ch);

	/**enable the PWM channel output
	 */
	virtual void enable();

	/**disable of the PWM channel output
	 */
	virtual void disable();

	/**Set the output duty-cycle, specified as a percentage (float)
	 */
	virtual void dutyCycle(float percentage);  	// 1.0=100% 0.5=50% ...

	/**Set the PWM pulse-width, specified in seconds (float)
	 */
	virtual void pulseWidth(float sec);			// 0.5=500ms 0.0005=500us ...

	/*Return the current output duty-cycle setting, measured as a percentage (float)
	 */
	virtual float read();

	/**Set output duty-cycle; inline function call to the member dutyCycle()
	 */
	inline void write(float percentage) {
		dutyCycle(percentage);
	}

	/**A shorthand to call the member pulseWidth()
	 */
	inline void operator = (float sec) {
		pulseWidth(sec);
	}

	/*A shorthand to retrieve the current output duty-cycle.
	 */
	inline operator float() {
		return read();
	}

	/**A static member function.
	 * Set the PWM MAIN period (or frequency), specified in seconds (float).
	 */
	static void period(float sec, PWM_GROUP_T pg=PG_ALL);
	static void frequency(uint32_t f, PWM_GROUP_T pg=PG_ALL);

	/**inline functions for ARDUINO
	 */
	inline void begin() { enable(); }
	inline void end() { disable(); }

	/*! \cond PRIVATE */
	virtual ~CPwm();

protected:
	uint32_t	m_flag;
	uint32_t 	m_nPeriod;
	uint8_t		m_timer_num;
	uint8_t		m_match_num;
	uint8_t		m_ch;
	void		pin_func(bool pwm);
	/*! \endcond */
};

#endif /* PWM_H_ */
