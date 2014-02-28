/*
 ===============================================================================
 Name        : main.cpp
 Author      :
 Version     : v1.0.0
 Date		 :
 Copyright   :
 Description : for ARDUINO
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------

 ===============================================================================
 */
#include "uCXpresso.h"
#include "arduino.h"
/*===============================================================================*/
int pin = 13;
volatile int state = LOW;

void blink()
{
  state = !state;
}

void setup()
{
  pinMode(pin, OUTPUT);
  attachInterrupt(2, blink, FALLING );	// toggle by UNO Pin 2  (nano11U37 p18)
}

void loop()
{
  digitalWrite(pin, state);
}
