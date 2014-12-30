/*
 ===============================================================================
 Name        : debug.cpp @ extensions
 Author      : Jason
 Date		 : 2013/5/15
 Version     : 1.0.3
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 License	 : CC BY-SA 3.0
 Description : debugged shell
 History	 :
 	 2012/9/6: v1.0.1 update CDebug::println(uint32_t)
 	 2013/3/1: v1.0.2 add keep alive for DBG_NET
 	 2013/5/15: add dbg_printf(...) function
 ===============================================================================
 */
#include "debug.h"

#define TELNET_PORT	23

CDebug *gpDBG = NULL;

CDebug::CDebug(CStream &s) : m_shell(s){
	m_stream = &s;
	gpDBG = this;
}

bool CDebug::start(uint32_t stack) {
	return CThread::start("debug", stack);
}

void CDebug::run() {
	sleep(1000);
	while( isAlive() ) {
		if ( m_stream->isConnected() ) {
			m_shell.run();
		} else sleep(1000);
	}
}

void CDebug::waitToDebugMode() {
	while( m_shell.isDebugMode()==false ) {
		sleep(500);
	}
}


void CDebug::printf(LPCTSTR format, va_list varg) {
	if ( m_shell.isDebugMode() ) {
		m_mutex.lock();
		m_shell.m_console.printf(format, varg);
		m_mutex.unlock();
	}
}

void CDebug::printf(LPCTSTR format, ...) {
	va_list varg;

	if ( m_shell.isDebugMode() ) {
		m_mutex.lock();
		va_start(varg, format);
		m_shell.m_console.printf(format, varg);
		va_end(varg);
		m_mutex.unlock();
	}
}

void CDebug::println(LPCTSTR format, ...) {
	va_list varg;

	if ( m_shell.isDebugMode() ) {
		m_mutex.lock();
		va_start(varg, format);
		m_shell.m_console.printf(format, varg);
		va_end(varg);
		m_shell.m_console << endl;
		m_mutex.unlock();
	}
}

void CDebug::println(int value) {
	if ( m_shell.isDebugMode() ) {
		m_mutex.lock();
		m_shell.m_console << value << endl;
		m_mutex.unlock();
	}
}

void CDebug::println(uint32_t value) {
	if ( m_shell.isDebugMode() ) {
		m_mutex.lock();
		m_shell.m_console.printf("%u\n",value);
		m_mutex.unlock();
	}
}

void CDebug::println(float value) {
	if ( m_shell.isDebugMode() ) {
		m_mutex.lock();
		m_shell.m_console << value << endl;
		m_mutex.unlock();
	}
}

int CDebug::putc(int c) {
	if ( m_shell.isDebugMode() ) {
		m_mutex.lock();
		if ( c!=0x0D ) {
			m_shell.m_console << (char)c << flush;
		} else {
			m_shell.m_console << endl;
		}
		m_mutex.unlock();
	}
	return c;
}

extern "C" void dbg_printf(LPCTSTR format, ...) {
	va_list varg;
	if (gpDBG && gpDBG->isDebugMode() ) {
		va_start(varg, format);
		gpDBG->printf(format, varg);
		va_end(varg);
	}
}

extern "C" void dbg_assert(bool condition, LPCTSTR format, ...) {
	va_list varg;
	if ( condition==false ) {
		if ( gpDBG ) {
			gpDBG->waitToDebugMode();
			va_start(varg, format);
			gpDBG->printf(format, varg);
			va_end(varg);
			gpDBG->read();	// wait a key to exit the assert function.
		} else CThread::suspendCurrentTask();
	}
}
