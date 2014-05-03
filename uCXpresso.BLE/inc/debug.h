/*
 ===============================================================================
 Name        : debug.h
 Author      : uCXpresso
 Version     : 1.0.5
 Date		 : 2014/2/28
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : Battery Level Service for BLE
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/9/6	v1.0.1 	add isAnyKey()									Jason
 2012/9/12	v1.0.2 	add println(CString&)
 2012/9/12	v1.0.3	rename waitEnterToDebugMode() to waitToDebugMode()
 2013/5/15	v1.0.4	add dbg_printf() function						Jason
 2014/2/28	v1.0.5	Add ASSERT() function							Jason
 ===============================================================================
 */
#ifndef _CDEBUG_H
#define _CDEBUG_H

#include <stdarg.h>
#include "class/shell.h"
#include "class/mutex.h"

class CDebug: public CThread {
public:
	//
	// construct use user's stream
	//
	CDebug(CStream &s);

	//
	// start DEBUG task
	//
	virtual bool start();

	//
	// for DEBUG
	//
	virtual void waitToDebugMode();
	virtual void printf(LPCTSTR format, va_list varg);
	virtual void printf(LPCTSTR format, ...);
	virtual void println(LPCTSTR format, ...);
	virtual void println(int value);
	virtual void println(uint32_t value);
	virtual void println(float value);

	virtual inline void println(CString &str){
		println((LPCTSTR)str);
	}

	virtual inline bool isDebugMode() {
		return m_shell.isDebugMode();
	}

	inline operator bool () {
		return isDebugMode();
	}


	virtual void putc(int c);

	inline uint8_t read() {
		return m_shell.read();
	}

	inline uint8_t wait() {
		return read();
	}

	inline int available() {
		return m_shell.available();
	}

	inline long parseInt() {
		return m_shell.parseInt();
	}

	inline float parseFloat() {
		return m_shell.parseFloat();
	}

	//
	// internal use
	//
protected:
	virtual void run();

	CStream *m_stream;
	CShell   m_shell;
	CMutex  m_mutex;
};

/******************************************************************************
 *
 * for Debug Build
 *
 ******************************************************************************/
#ifdef DEBUG
extern CDebug *gpDBG;

//
// show message in Debug Mode
//
extern "C" void dbg_printf(LPCTSTR format, ...);

//
// show message in Debug Mode with condition, if condition is false,
// and press any key to continue.
//
extern "C" void ASSERT(bool condition, LPCTSTR format, ...);

#else
/******************************************************************************
 *
 * for Release Build
 *
 ******************************************************************************/
#define dbg_printf(...)
#define ASSERT(...)
#endif

#endif	// _CDEBUG_H
