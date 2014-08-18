/*
 ===============================================================================
 Name        : tone.cpp
 Author      : Jason
 Version     : 1.0.1
 Date		 : 2014/2/16
 Description : tone functions for Arduino
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/2/16	v1.0.1	Crate CTone class								Jason
 2014/3/22	v1.0.2	Rename onTone to noTone.						Jason
 	 	 	 	 	Reduce stack of tone task.
 ===============================================================================
 */
#include "arduino.h"
#include "arduino/tone.h"

#include "class/thread.h"
#include "class/timer.h"
#include "class/pin.h"
#include "class/timeout.h"
#include "class/list.h"

static CList lsTone;

//
// CTone Class (A tone thread object)
//
class CTone: public CThread {
public:
	CTone(PIN_NAME_T pin, TIMER_T t, uint32_t frequency, uint32_t duration, uint16_t count)
	: m_timer(t), m_pin(pin)
	{
		if ( frequency==0 ) {
			frequency = (1000 / duration);
		}

#if defined(NANO176X)
		uint32_t match = (1000000/(frequency*2));
		m_timer.setting(1, match);

#elif defined(NANO11UXX)
		m_timer.second(1.0f / (frequency * 2.0f));
#endif
		m_pin.output();
		m_duration = duration;
		m_count = count;
	}

	void setup(uint32_t frequency, uint32_t duration, uint16_t count) {
		m_timer.second(1.0f / (frequency * 2.0f));
		m_duration = duration;
		m_count = count;
	}

	bool isPin(PIN_NAME_T pin) {
		return (m_pin.name()==pin ? true : false);
	}

protected:
	uint32_t m_duration;
	uint16_t m_count;
	CTimer	 m_timer;
	CPin 	 m_pin;

	//
	// run loop
	//
	virtual void run() {
		// add to list
		LIST_POS pos;
		pos = lsTone.addTail(this);

		CTimeout tm;
		m_timer.enable();

		while(isAlive()) {
			m_timer.wait();
			m_pin.invert();
			if ( m_duration && tm.isExpired(m_duration) ) {
				m_timer.disable();
				m_pin = LOW;
				m_count--;
				if ( m_count>0 ) {
					sleep(m_duration);
				} else suspend();	// task suspend
				tm.reset();
				m_timer.enable();
			}
		}

		// remove from list
		lsTone.removeAt(pos);

		// end of the run() to destroy the task automatically, if task is allocated.
	}
};

static CTone *find(PIN_NAME_T pin) {
	LIST_POS pos;
	CTone *tone;
	pos = lsTone.getHeadPos();
	while(pos) {
		tone = (CTone *) lsTone.getAt(pos);
		if ( tone->isPin(pin) ) {
			return tone;
		}
		pos = lsTone.getNext(pos);
	}
	return NULL;
}

//
// tone function for uCXpresso
//
bool tone(PIN_NAME_T pin, uint32_t frequency, uint32_t duration, uint16_t count) {
	CTone *tn = find(pin);

	if ( tn ) {
		//
		// change tone parameters
		//
		tn->setup(frequency, duration, count);
		tn->resume();

	} else {
		//
		// allocate a new tone task
		//
		TIMER_T t = CTimer::getUnused();
		if ( t>=0 ) {
			tn = new CTone(pin, t, frequency, duration, count);	// allocate a task
			if ( tn ) {
				return tn->start("tone", 56, PRI_HARDWARE);
			}
		}
	}
	return false;
}

void noTone(PIN_NAME_T pin) {
	CTone *tone = find(pin);
	if ( tone ) {
		delete tone;
	}
}

#ifdef ARDUINO
//
// tone function for Arduino
//
void tone(int pin, uint32_t frequency, uint32_t duration) {
	tone(_PIN_(pin), frequency, duration);
}

void noTone(int pin) {
	noTone(_PIN_(pin));
}
#endif
