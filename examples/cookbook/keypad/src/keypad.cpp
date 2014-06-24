/*
 ===============================================================================
 Name        : keypad.h
 Author      : Arik
 Version     : 0.0.1
 Date		 : 2014/6/16
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description :
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/6/16	v0.0.1	First Edition.									Arik
 ===============================================================================
 */

#include "keypad.h"

#if 0
#include "debug.h"
#define DBG		dbg_printf
#else
#define DBG(...)
#endif

CKeypad::CKeypad(CBus &rows, CBus &cols) {
	m_rows = &rows;
	m_cols = &cols;

	m_rows->output();
	m_cols->input(INTERNAL_PULL_UP);	// active Low

	m_key_code = 0;
	m_keydown_event.binary();
}

CKeypad::~CKeypad() {
	disable();
}

void CKeypad::enable() {
	if (isSuspend()) {
		resume();
	} else {
//		start("keypad", 168);	//for debug log
		start("keypad", 46);
	}
}

void CKeypad::disable() {
	suspend();
}

void CKeypad::run() {
	CTimeout tmBounce;						// time count for scanning keypad
	uint8_t input, row, col;
	uint32_t colMask = bit(m_cols->count()) - 1;

	row = 0;
	while (isAlive()) {	// check thread alive

		*m_rows = bitmask(row);							// scan a low bit to row
		input = (m_cols->read() & colMask);
		if (input != colMask) {
			if (tmBounce.isExpired(10)) {				// key bounce check (10ms)
				for (col = 0; col < m_cols->count(); col++) {
					if ( bit_chk(input, col) == 0) {	// keypad is active low
						// key down
						m_key_code = (row * m_cols->count()) + col;
						m_keydown_event.release();
						break; // exit for-loop
					}
				}

				// wait key release
				while ((m_cols->read() & colMask) != colMask) {
					sleep(10);
				}
			}
		} else {
			tmBounce.reset();
			row = (row + 1) < m_rows->count() ? row + 1 : 0;	// scan for next row
		}
	}
}

int CKeypad::wait(uint32_t timeout) {
	if ( m_keydown_event.wait(timeout) ) {
		return m_key_code;
	}
	return -1;
}
