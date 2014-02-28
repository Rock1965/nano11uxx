/*
 ===============================================================================
 Name        : stream.h
 Author      : Jason
 Version     :
 Date		 : 2011/12/18
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 History     :
 ===============================================================================
 */

#ifndef STREAM_H_
#define STREAM_H_

#include "class/object.h"

/*!	\class CStream stream.h "class/stream.h"
	\brief An stream abstract class.<br />
	To define the stream input and output interface.
 */
class CStream: public CObject {
public:
	CStream();
	virtual ~CStream();

	virtual int	 readable() = PURE_VIRTUAL_FUNC;
	virtual int	 writeable() = PURE_VIRTUAL_FUNC;
	virtual int  read(void *buf, int len, bool block=true) = PURE_VIRTUAL_FUNC;
	virtual int  write(const void *buf, int len, bool block=true) = PURE_VIRTUAL_FUNC;
	virtual bool isConnected() = PURE_VIRTUAL_FUNC;
	virtual void flush() = PURE_VIRTUAL_FUNC;

	//
	// inlince
	//
	inline int write(int ch) {
		return write(&ch, 1);
	}

	inline int read() {
		int ch=0;
		if ( read(&ch,1) ) {
			return ch;
		}
		return -1;
	}

	/*! \cond PRIVATE */
};

#endif /* STREAM_H_ */
