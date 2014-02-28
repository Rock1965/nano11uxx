/*
 ===============================================================================
 Name        : arduino.cpp
 Author      : Jason
 Version     : 1.0.1
 Date		 : 2012/9/8
 Copyright   : 2012 (C) Embeda Technology Inc.
 License	 : CC BY-SA 3.0
 Description : arduino core library
 History	 :
 	 2012/9/4: remove DEFAULT
 	 2012/9/8: update pulseIn count by CTick to return microsecond unit.
 	 2014/1/7: update for NANO11UXX
 ===============================================================================
 */
#include "uCXpresso.h"
#include "arduino.h"
#include "class/timeout.h"
#include "class/pwm.h"
#include "class/list.h"
#include "class/thread.h"

//
// UNO pin number to NANO pin name
//
#ifdef NANO176X
static const PIN_NAME_T unoPIN[MAX_DIGITAL_PIN] = {
		P27,	// 0
		P28,	// 1
		P26,	// 2	, PWM1
		P25,	// 3	, PWM2
		P24,	// 4	, PWM3
		P23,	// 5	, PWM4
		P22,	// 6	, PWM5
		P21,	// 7	, PWM6
		P9,		// 8
		P10,	// 9
		P8,		// 10
		P5,		// 11
		P6,		// 12
		P7,		// 13
		P11,	// 14
		P12,	// 15
		P13,	// 16
		P14,	// 17
		P30,	// 18
		P29,	// 19
};

#define MAX_PWM_PINS		6
#define IS_PWM_PIN			((p)>=2 && (p)<=7)
#define PIN_TO_PWM(p)   	((p)-2)
CPwm pwm[] = { PWM1, PWM2, PWM3, PWM4, PWM5, PWM6 };

#endif

#ifdef NANO11UXX
static const PIN_NAME_T unoPIN[MAX_DIGITAL_PIN] = {
		P16,	// 0
		P17,	// 1
		P18,	// 2,
		P19,	// 3
		P20,	// 4	PWM1
		P21,	// 5	PWM2
		P22,	// 6	PWM3
		P23,	// 7	PWM4
		P24,	// 8
		P25,	// 9
		P26,	// 10
		P27,	// 11
		P28,	// 12
		P29,	// 13
		P9,		// 14, 	A0
		P10,	// 15,	A1
		P11,	// 16,	A2
		P12,	// 17,	A3
		P13,	// 18,	A4
		P14,	// 19,	A5
};

#define MAX_PWM_PINS		4
#define IS_PWM_PIN(p)		((p)>=4 && (p)<=7)
#define PIN_TO_PWM(p)   	((p)-4)
CPwm pwm[] = { PWM1, PWM2, PWM3, PWM4 };

#endif

PIN_NAME_T _PIN_(int pin){
	return unoPIN[pin];
}

//
// Digital I/O
//
void pinMode(int pin, PIN_DIR_T mode) {
	if ( pin>=0 && pin<MAX_DIGITAL_PIN ) {
		CPin p(unoPIN[pin]);
		if ( mode==OUTPUT )
			 p.output(NOT_OPEN);
		else p.input(INTERNAL_PULL_UP);
	}
}

void digitalWrite(int pin, int value) {
	if ( pin>=0 && pin<MAX_DIGITAL_PIN ) {
		CPin p(unoPIN[pin]);
		p = (PIN_LEVEL_T)value;
	}
}

int digitalRead(int pin) {
	PIN_LEVEL_T value = LOW;
	if ( pin>=0 && pin<MAX_DIGITAL_PIN ) {
		CPin p(unoPIN[pin]);
		value = p;
	}
	return (int)value;
}

//
// Analog I/O
//
int analogRead(ADC_CH_T pin) {
	CAdc adc(pin);
	adc.begin();
	return adc.read();
}

void analogWrite(int pin, uint8_t value, uint32_t f) {
	if ( IS_PWM_PIN(pin) ) {
		// global PWM setting
		CPwm::frequency(f);	// default frequency = 490Hz
#ifdef NANO176X
		CPwm::start();
#endif
		// pwm object
		pwm[PIN_TO_PWM(pin)].dutyCycle((float)map((float)value, 0.0f, 255.0f, 0.0f, 100.0f)/100.0f);
		pwm[PIN_TO_PWM(pin)].enable();
	}
}

//
// Time
//
void delayMicroseconds(uint32_t us) {
#ifdef NANO176X
	CTick tick;
	tick.delay(us);
#endif
#ifdef NANO11UXX
	CTimer t(TIMER1);
	t.second(us/1000000);
	t.enable();
	t.wait();
#endif
}

//
// Advanced I/O
//
void shiftOut(int dataPin, int clockPin, BITORDER_T bitOrder, byte value) {
	CPin dat(unoPIN[dataPin]);
	CPin clk(unoPIN[clockPin]);
	dat.output(NOT_OPEN);
	clk.output(NOT_OPEN);
	for (int i = 0; i < 8; i++)  {
		if (bitOrder == LSBFIRST)
			dat = (bit_chk(value,i)? HIGH:LOW);
		else
			dat = (bit_chk(value,(7-i))? HIGH:LOW);

		clk = HIGH;
		delayMicroseconds(2);	// 2us
		clk = LOW;
	}
}

byte shiftIn(int dataPin, int clockPin, BITORDER_T bitOrder) {
	byte ret = 0;
	CPin dat(unoPIN[dataPin]);
	CPin clk(unoPIN[clockPin]);
	dat.input();
	clk.output(NOT_OPEN);
	for (int i = 0; i < 8; ++i) {
		clk = HIGH;
		delayMicroseconds(2);
		if ( dat==HIGH ) {
			if ( bitOrder==LSBFIRST ) {
				bitSet(ret, i);
			} else {
				bitSet(ret, (7-i));
			}
		}
		clk = LOW;
	}
	return (ret);
}

uint32_t pulseIn(int pin, PIN_LEVEL_T value, uint32_t timeout) {
	CPin p(unoPIN[pin]);
	p.input();
	CTimeout to;

#ifdef NANO176X
	CTick tick;

	to.reset();
	while( p!=value ) {
		if ( timeout && to.isExpired(timeout) ) return 0;
	}
	tick.reset();

	while( p==value ) {
		if ( timeout && to.isExpired(timeout) ) return 0;
	}
	return tick.elapsed();
#endif

#ifdef NANO11UXX
	CTimer t(TIMER1);
	t.second(0.000001); // 1 us

	to.reset();
	while( p!=value ) {
		if ( timeout && to.isExpired(timeout) ) return 0;
	}

	t.enable();
	while( p==value ) {
		if ( timeout && to.isExpired(timeout) ) return 0;
	}
	return t.count();

#endif
}

//
// External  Interrupts
//
#include "arduino/interrupt.h"
CList m_lstIRQ;
void attachInterrupt(int pin, INT_FUNC func, EDGE_STATE_T edge, int stack) {
	if ( pin>=0 && pin<MAX_DIGITAL_PIN ){
		CInterrupt *irq;
		if ( edge==FALLING ) {
			irq= new CInterrupt(unoPIN[pin], edge, INTERNAL_PULL_UP);
		} else {
			irq= new CInterrupt(unoPIN[pin], edge, INTERNAL_PULL_DOWN);
		}
		if ( irq ) {
			irq->m_func = func;
			irq->start("ExtInt", stack, PRI_HIGH);	// start the interrupt thered
			m_lstIRQ.addTail(irq);					// add irq object to list
		}
	}
}

void detachInterrupt(int pin) {
	if ( pin>=0 && pin<MAX_DIGITAL_PIN ) {
		CInterrupt *irq;
		LIST_POS	pos;

		// search interrupt with the pin
		pos = m_lstIRQ.getHeadPos();
		while(pos) {
			irq = (CInterrupt *) m_lstIRQ.getAt(pos);
			if ( irq->m_pin==pin ) {
				delete irq;
				m_lstIRQ.removeAt(pos);
				return;
			}
			pos = m_lstIRQ.getNext(pos);
		}
	}
}

