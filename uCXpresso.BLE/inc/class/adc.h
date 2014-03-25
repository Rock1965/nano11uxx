/*
 ===============================================================================
 Name        : adc.h
 Author      : uCXpresso
 Version     : v1.0.0
 Date		 : 2014/1/1
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : The CAdc Interface is used to read the voltage applied to
 	 	 	   an analog input pin.
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	First Edition for nano11Uxx						Jason
 ===============================================================================
 */

#ifndef ADC_H_
#define ADC_H_

#include <cstdarg>
#include "class/peripheral.h"
#include "class/mutex.h"
#include "class/timer.h"

/*! \def MAX_ADC_VALUE
	Indicates the ADC maximum value
 */
#define MAX_ADC_VALUE		0x3FF	// 10 bits = 0~1023

/*! \def MAX_ADC_CHANNEL
 * 	Indicates the maximun ADC channel
 */
#define MAX_ADC_CHANNEL		6

/**ADC Channels
 * \ingroup Enumerations
 */
typedef enum {
	AD0 = 0,	///< pin name to AD0 (P9)
	AD1,		///< pin name to AD1 (P10)
	AD2,		///< pin name to AD2 (P11)
	AD3,		///< pin name to AD3 (P12)
	AD4,		///< pin name to AD4 (P13)
	AD5			///< pin name to AD5 (P14)
}ADC_CH_T;


/**an Analog-to-Digital converter class.
 * \class CAdc adc.h "class/adc.h"
 * \ingroup Peripherals
 */
class CAdc: public CPeripheral {
public:

	/**Constructs a CAdc object with an AD pin.
	 * \param ch is a ADC_CH_T to specified an AD pin to connected to the object.
	 *
	 * \code
	 * Example:
	 *		CAdc ad(AD5);
	 *		ad.begin();
	 * \endcode
	 */
	CAdc(ADC_CH_T ch);

	/** Enable the ADC function on the object
	 * \param filter is true to enable the internal simple filter feature, otherwise disabled.
	 */
	virtual void enable(bool filter=true);

	/** Disable the ADC function on the object
	 */
	virtual void disable();

	/** Read the ADC value from the AD pin
	 * \return 10 bits resolution (0~1023) ADC value, if return -1, an overrun error.
	 */
	virtual int read();

	/** Read the ADC value from the AD pin with Compare-Filter
	 * \param[in] filter a float value to identify the offset range with last value. (0.1 = 10%, 0.25=25%)
	 * \param[in] count a integer value to identify to retry count if ADC value over the filter range. (default value is 3)
	 * \return 12 bits resolution (0~4095) ADC value
	 *
	 * \code
	 * 	Example:
	 * 		CAdc ad(AD0);
	 * 		ad.enable();
	 * 		int value = ad.read(0.25, 3);	// return value when AD value < 25% offset with last value in 3 times.
	 * \endcode
	 */
	virtual int read(float filter, int count=3);

	/** Read the ADC value from the AD pin with Median-Filter
	 * \param[in] samples a integer to identify how many ADC value read in sample buffer.
	 * \param[in] interval a CTimer object to provide the interval timer service.
	 * \return 12 bits resolution (0~4095) ADC value
	 *
	 * \code
	 * Example:
	 * 		CAdc ad(AD0);
	 * 		ad.begin();
	 *
	 * 		CTime t(TIMER0);
	 * 		t.second(0.0005);	// 500us interval
	 * 		t.enable();
	 *
	 * 		int value = ad.read(5, t);	// ADC value with Median-Filter, total 5 samples, interval 500us
	 *
	 * \endcode
	 * <a href="http://en.wikipedia.org/wiki/Median_filter">WiKi for Median Filter</a>
	 */
	virtual int read(int samples, CTimer &interval);

	/** A shorthand operator code to read ADC value
	 * \return a float value, divide by MAX_ADC_VALUE
	 *
	 * \code
	 * 	Example:
	 * 		CAdc ad(AD5);
	 * 		ad.begin();
	 *
	 * 		float value = ad;
	 * \endcode
	 */
	virtual inline operator float() { return map( (float)read(0.2, 3), 0.0f, (float)MAX_ADC_VALUE, 0.0f, 3.3f); }

	//
	// inline functions for ARDUINO
	//
	inline void begin() { enable(); }
	inline void end() { disable(); }

	/*! \cond PRIVATE */
	virtual ~CAdc();

protected:
	int 		m_ch;
	uint32_t 	m_lastVal;
	/*! \endcond */
};

/**\example /peripheral/adc/src/main.cpp
 * This is an example of how to use the CAdc class.
 * More details about this example.
 */

#endif /* ADC_H_ */
