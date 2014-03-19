/*
 ===============================================================================
 Name        : timeout.h
 Author      : Jason
 Version     : 1.0.1
 Created on	 : 2012/3/18
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : A microsecond (us) timeout/expired checker
 History	 :
 ===============================================================================
 */

#ifndef TIMEOUT_H_
#define TIMEOUT_H_

#include "class/object.h"

/**CTimeout provides the timeout count service.
 * \class CTimeout timeout.h "class/timeout.h"
 * \ingroup Miscellaneous
 */
class CTimeout: public CObject {
public:
	/**Call the reset() member to reset timeout count.
	 */
	virtual void reset();

	/**Call the wait() member to wait the timeout count expired.
	 * \param ms is an integer value to indicate the waiting time in millisecond.
	 */
	virtual void wait(int ms);				// timeout unit:ms

	/**Call the wait() member to wait the timeout count expired.
	 * \param sec is a floating value to indicate the waiting time in second.
	 */
	virtual inline void wait(float sec){ 	// 0.1=100ms 0.02=20ms...
		wait((int)(sec * 1000));
	}

	/**Call the elapsed() member to retrieve the current time elapsed.
	 * \return an uint32_t value to indicate the time elapsed in millisecond.
	 */
	virtual uint32_t elapsed();

	/**Call the isExpired() to check whether time is expired.
	 * \code
	 * CTimeout tm;
	 * ...
	 * while(1) {
	 * 		if ( tm.isExpired(3000) ) {		// check timeout whether expire 3000ms
	 *
	 * 		}
	 *		...
	 * }
	 * \endcode
	 * \return true if time is expired, otherwise not.
	 */
	virtual inline bool isExpired(uint32_t to) {
		return ((elapsed()>to )? true : false);
	}

	/**Call the read() member to retrieve the time count value in second.
	 * \return a floating value to indicate the time count value in second,
	 */
	virtual inline float read() {
		return (elapsed() / 1000.0f);
	}

	/// @cond
	CTimeout();
protected:
	uint32_t m_tick;
	/// @endcond
};

#endif /* TIMEOUT_H_ */
