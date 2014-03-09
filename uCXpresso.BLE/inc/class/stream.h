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

/**An abstract class, to define the serial stream input and output interface.
 * \class CStream stream.h "class/stream.h"
 */
class CStream: public CObject {
public:
	/**CStream constructor */
	CStream();

	/**CStream destructor */
	virtual ~CStream();

	/**Determine how many data bytes are available to read.
	 * \return A value to indicate how many data byte is available in the input buffer.
	 * \remark the pure virtual function have to implement by child class.
	 */
	virtual int	 readable() = PURE_VIRTUAL_FUNC;

	/**Determine how many data space are available to write.
	 * \return A value to indicate how many data space is available in the output buffer.
	 * \remark the pure virtual function have to implement by child class.
	 */
	virtual int	 writeable() = PURE_VIRTUAL_FUNC;

	/**To read the stream to buffer.
	 * \param[in] buf Destination buffer.
	 * \param[in] len Length of destination buffer.
	 * \param[in] block If true, to block in the read function unit to the indication length (len) be read.
	 * \return A value to indicate how many data bytes to read.
	 * \remark the pure virtual function have to implement by child class.
	 */
	virtual int  read(void *buf, int len, bool block=true) = PURE_VIRTUAL_FUNC;

	/**To write the buffer to stream.
	 * \param[out] buf Source buffer.
	 * \param[in] len Length of source buffer.
	 * \param[in] block If true, to block in the write function unit to the indication length (len) be sent.
	 * \return A value to indicate how many data bytes to write.
	 * \remark the pure virtual function have to implement by child class.
	 */
	virtual int  write(const void *buf, int len, bool block=true) = PURE_VIRTUAL_FUNC;

	/**Check the current connection is valid or not.
	 * \return true if current connection is valid.
	 * \remark the pure virtual function have to implement by child class.
	 */
	virtual bool isConnected() = PURE_VIRTUAL_FUNC;

	/**Flush the stream the both input and output buffer
	 * \remark the pure virtual function have to implement by child class.
	 */
	virtual void flush() = PURE_VIRTUAL_FUNC;

	//
	// inline functions
	//

	/**An inline function to write a character to output buffer of stream.
	 * \param ch is an integer data that will be send to the stream buffer, the data size is one byte.
	 * \return 1 (length) if write successful, otherwise is fail.
	 * \note The inline function is a overload function to call to the write() member.
	 */
	inline int write(int ch) {
		return write(&ch, 1);
	}

	/**An inline function to read a character from input buffer of stream.
	 * \return the character value, the character size is one byte.
	 * \note The inline function is a overload function to call to the read() member.
	 */
	inline int read() {
		int ch=0;
		if ( read(&ch,1) ) {
			return ch;
		}
		return -1;
	}
};

#endif /* STREAM_H_ */
