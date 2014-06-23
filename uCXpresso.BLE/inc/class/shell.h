/*
 ===============================================================================
 Name        : shell.h
 Author      : uCXpresso
 Version     : v1.0.1
 Date		 : 2014/5/2
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : sell class
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	First Edition for nano11Uxx							Jason
 2014/5/2	v1.0.1	Add parseInt() and parseFloat() member functions	Jason
 ===============================================================================
 */

#ifndef SHELL_H_
#define SHELL_H_

#include "class/thread.h"
#include "class/console.h"
#include "class/string.h"
#include "class/stream.h"
#include "class/semaphore.h"


class CShell: public CThread {
public:
	CShell(CStream &s);
	virtual bool start();

	virtual void onQuery(CString &str);
	virtual void show_welcome();
	virtual void show_prompt();
	virtual void show_menu();
	virtual void show_task();
	virtual void show_heap();
	virtual void show_version();
	virtual void show_clear();

	//
	// for Debug Mode
	//
	bool isDebugMode();

	// in debug mode, check serial input available.
	int available();

	inline uint8_t read() {
		return m_console.iStream()->read();
	}

	inline long	parseInt(bool echo=true) {
		return m_console.iStream()->parseInt(echo);
	}

	inline float parseFloat(bool echo=true) {
		return m_console.iStream()->parseFloat(echo);
	}

// internal use
public:
	Console 	m_console;
	virtual void run();
	virtual void onDebug();
	virtual void onClose() {}
	virtual ~CShell();

protected:
	CStream 	*m_sock;
	CString 	cmd;
};

#endif /* SHELL_H_ */
