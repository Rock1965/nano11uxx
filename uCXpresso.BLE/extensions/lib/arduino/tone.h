/*
 ===============================================================================
 Name        : tone.h
 Author      : Jason
 Version     : 1.0.1
 Date		 : 2014/2/22
 Description : tone functions for Arduino
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/2/16	v1.0.1	Crate CTone class								Jason
 2014/2/22	v1.0.2	CTone task support the count for alert			Jason
 ===============================================================================
 */

#ifndef TONE_H_
#define TONE_H_

#include "class/pin.h"

bool tone(PIN_NAME_T pin, uint32_t frequency, uint32_t duration, uint16_t count=1);
void noTone(PIN_NAME_T pin);

#ifdef ARDUINO
void tone(int pin, uint32_t frequency, uint32_t duration);
void noTone(int pin);
#endif

#endif /* TONE_H_ */
