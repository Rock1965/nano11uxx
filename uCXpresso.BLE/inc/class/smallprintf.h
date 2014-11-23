/*
 ===============================================================================
 Name        : smallprintf.h
 Author      : uCXpresso
 Version     :
 Date		 : 2011/12/18
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 History     :
 ===============================================================================
 */

#ifndef SMALLPRINTF_H
#define SMALLPRINTF_H

#include <cstdarg>
#include "class/object.h"

/**
 * @brief Provides a small size of printf library for MCU base.
 * @class CSmallPrintf smallprintf.h "class/smallprintf.h"
 * @ingroup Miscellaneous
 */
class CSmallPrintf: public CObject {
public:
	CSmallPrintf();

	int printf(LPCTSTR format, ...);
	int printf(LPCTSTR format, va_list args);

	int putv(int val);
	int putv(unsigned val);
	int putv(double val);

	/*! \cond PRIVATE */
public:
	virtual int putc(int ch) = PURE_VIRTUAL_FUNC;
	virtual int getc() = PURE_VIRTUAL_FUNC;
	virtual LPCTSTR gets() = PURE_VIRTUAL_FUNC;
//	int scanf(LPCTSTR format, va_list args);

protected:
	virtual void alloc(int bufsize=256) { NOTHING }
	virtual void free() { NOTHING }
	int		 m_base;
	/*! \endcond */
};

#endif /* SMALLPRINTF_H */
