/*
 ===============================================================================
 Name        : shell.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2014/1/1
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : sell class
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	First Edition for nano11Uxx						Jason
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

	bool isDebugMode();
	int  dbgWaitKey(uint32_t t=MAX_DELAY_TIME);

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
	CSemaphore	m_semInput;
	int			m_dbgKey;
};

#endif /* SHELL_H_ */
