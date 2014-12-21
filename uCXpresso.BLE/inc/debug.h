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

///@cond
#ifdef __cplusplus
///@endcond

#include <stdarg.h>
#include "class/shell.h"
#include "class/mutex.h"

/**
 * @brief  CDebug class provide the shell terminal and debug message services.
 * @class CDebug debug.h "debug.h"
 * @ingroup RTOS
 */
class CDebug: public CThread {
public:
	/**
	 * @brief Constructs the CDebug object.
	 * @param port Indicate a @ref DEBUG_PORT_T for input/outpu stream.
	 */
	CDebug(CStream &s);

	/**
	 * @brief Start the debug task.
	 * @param stack Indicate the stack size of the debug task, default 128 words.
	 */
	virtual bool start(uint32_t stack=128);

	/**
	 * @brief waiting for entered to the debug mode.
	 */
	virtual void waitToDebugMode();

	/** @brief Output format message.
	 */
	virtual void printf(LPCTSTR format, va_list varg);

	/** @brief Output format message.
	 */
	virtual void printf(LPCTSTR format, ...);

	/** @brief Output format message with the CRLF.
	 */
	virtual void println(LPCTSTR format, ...);

	/** @brief Output format message for a integer value with the CRLF.
	 */
	virtual void println(int value);

	/** @brief Output format message for a uint32_t value with the CRLF.
	 */
	virtual void println(uint32_t value);

	/** @brief Output format message for a floating value with the CRLF.
	 */
	virtual void println(float value);

	/** @brief Output format message for a string with the CRLF.
	 */
	virtual inline void println(CString &str){
		println((LPCTSTR)str);
	}

	/** @brief Check now is in debug mode, or not.
	 */
	virtual inline bool isDebugMode() {
		return m_shell.isDebugMode();
	}

	/**@brief Shorthand to check in debug mode or not.
	 */
	inline operator bool () {
		return isDebugMode();
	}

	/**@brief Output a character to stream.
	 */
	virtual int putc(int c);

	/**@brief Input a character from stream.
	 * @note The member function will push the task in block state to waiting for character inputed.
	 */
	inline uint8_t read() {
		return m_shell.read();
	}

	/**@brief wait for a character.
	 */
	inline uint8_t wait() {
		return read();
	}

	/**@brief Retrieve the number of characters in the input stream buffer.
	 */
	inline int available() {
		return m_shell.available();
	}

	/**@brief Input a integer value from the input stream.
	 */
	inline long parseInt() {
		return m_shell.parseInt();
	}

	/**@brief Input a floating value from the input stream.
	 */
	inline float parseFloat() {
		return m_shell.parseFloat();
	}

	//
	///@cond Internal used
	//
protected:
	virtual void run();

	CStream *m_stream;
	CShell   m_shell;
	CMutex  m_mutex;
	///@endcond
};

/******************************************************************************
 *
 * for Debug Build
 *
 ******************************************************************************/

extern CDebug *gpDBG;

/**
 * @brief Pointer to the CDebug object of the global.
 */
extern CDebug *gpDBG;

///@cond
#endif // __cplusplus
///@endcond

/**
 * @brief To output the format message in debug mode.
 */
extern "C" void dbg_printf(LPCTSTR format, ...);

/**
 * @brief Assert the condition with message output.
 * @param condition	Output the message when condition is false.
 * @param format Format the output message that is same as the printf.
 *
 * @note
 * In normal mode, the dbg_assert will push the task in suspend when condition is false.
 * In debug mode, the dbg_assert will waiting for any key input to exit the dbg_assert function.
 * In "Release Build", the dbg_assert will in endless loop when condition is false.
 *
 * The dbg_assert will block the task in normal mode when condition is false.
 */
extern "C" void dbg_assert(bool condition, LPCTSTR format, ...);

/**\example /rtos/debug/src/main.cpp
 * This is an example of how to use the debug services.
 * More details about this example.
 */

#endif	// _CDEBUG_H
