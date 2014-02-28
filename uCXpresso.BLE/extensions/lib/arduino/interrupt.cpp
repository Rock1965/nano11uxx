/*
 ===============================================================================
 Name        : interrupt.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/9/5
 Copyright   : 2012 (C) Embeda Technology Inc.
 License	 : CC BY-SA 3.0
 Description : extern interrupt
 History	 :

 ===============================================================================
 */
#include "arduino/interrupt.h"

CInterrupt::CInterrupt(PIN_NAME_T pin, EDGE_STATE_T edge, PIN_INPUT_MODE_T mode) :
m_int(pin, edge, mode) {
	m_pin = pin;
	m_func = NULL;
}

CInterrupt::~CInterrupt() {
	m_int.disable();	// disable the interrupt
}

void CInterrupt::run() {
	m_int.enable();
	while(1){
		m_int.wait();	// wait interrupt
		if ( m_func ) {
			m_func();		// call use's interrupt function
		}
	}
}
