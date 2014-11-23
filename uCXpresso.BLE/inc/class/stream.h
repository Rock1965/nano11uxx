/*
 ===============================================================================
 Name        : stream.h
 Author      : uCXpresso
 Version     : v1.0.2
 Date		 : 2014/8/4
 Description : Stream Base Class
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/8/1	v1.0.0	First Edition									Jason
 2014/8/4	v1.0.1	Add parseHex() member function.					Jason
 2014/8/22	v1.0.2	Add waitTxEmpty() member function.				Jason
 ===============================================================================
 */

#ifndef STREAM_H_
#define STREAM_H_

#include <class/object.h>
#include <class/semaphore.h>
#include <utils/fifo.h>

/**An abstract class, to define the serial stream input and output interface.
 * \class CStream stream.h "class/stream.h"
 * \ingroup Peripherals
 */
class CStream: public CObject {
public:
	CStream(size_t tx_fifo_size, size_t rx_fifo_size);

	/**Determine how many data bytes are available to read.
	 * \return A value to indicate how many data byte is available in the input buffer.
	 * \remark the pure virtual function have to implement by child class.
	 */
	virtual int	 readable();

	/**Determine how many data space are available to write.
	 * \return A value to indicate how many data space is available in the output buffer.
	 * \remark the pure virtual function have to implement by child class.
	 */
	virtual int	 writeable();

	/**To read the stream to buffer.
	 * \param[in] buf Destination buffer.
	 * \param[in] len Length of destination buffer.
	 * \param[in] block to block in the read function unit to the indication length (len) be read.
	 * \return A value to indicate how many data bytes to read.
	 * \remark the pure virtual function have to implement by child class.
	 */
	virtual int  read(void *buf, int len, uint32_t block=MAX_DELAY_TIME);

	/**To write the buffer to stream.
	 * \param[out] buf Source buffer.
	 * \param[in] len Length of source buffer.
	 * \param[in] block If true, to block in the write function unit to the indication length (len) be sent.
	 * \return A value to indicate how many data bytes to write.
	 * \remark the pure virtual function have to implement by child class.
	 */
	virtual int  write(const void *buf, int len, uint32_t block=MAX_DELAY_TIME);

	/**Check the current connection is valid or not.
	 * \return true if current connection is valid.
	 * \remark the pure virtual function have to implement by child class.
	 */
	virtual bool isConnected() = PURE_VIRTUAL_FUNC;

	/**Flush the stream the both input and output buffer
	 * \remark the pure virtual function have to implement by child class.
	 */
	virtual void flush();

	//
	// read/write for a byte
	//
	/**Read a byte from stream
	 * \return uint8_t (8 bits) data from the stream
	 */
	virtual uint8_t read();

	/**Write a byte to stream
	 * \param c is a uint8_t data to send to the stream.
	 */
	virtual void write(uint8_t c);

	//
	// Operators
	//

	/**Operator '<<', to output a byte to stream.
	 * \code
	 * CSerial uart;
	 * uart.enable(19200);
	 *
	 * uint8_t ch = 0xA5;
	 * uart << ch;
	 * \endcode
	 */
	virtual inline CStream& operator << (uint8_t c) {
		write(c);
		return *this;
	}

	/**Operator '>>', to input a byte from stream.
	 * \code
	 * CSerial uart;
	 * uart.enable(19200);
	 *
	 * uint8_t ch ;
	 * uart >> ch;
	 * \endcode
	 */
	virtual inline CStream& operator >> (uint8_t c) {
		c = read();
		return *this;
	}

	/**Operator 'uint8_t', to receive a byte from stream.
	 * \code
	 * CSerial uart;
	 * uart.enable(19200);
	 *
	 * uint8_t ch;
	 * ch = uart;
	 * \endcode
	 */
	virtual inline operator uint8_t () {
		return read();
	}

	/**Operator 'char', to receive a char from stream.
	 * \code
	 * CSerial uart;
	 * uart.enable(19200);
	 *
	 * char ch;
	 * ch = uart;
	 * \endcode
	 */
	virtual inline operator char () {
		return (char)read();
	}

	/**Read a byte from right stream and send to left stream.
	 */
	virtual inline CStream& operator << (CStream &s) {
		write(s.read());
		return *this;
	}

	/**Wait for Tx buffer empty.
	 * \param timeout	set the block timeout in millisecond.
	 * \return true, if transmit buffer is empty. false, if timeout.
	 */
	virtual bool waitTxEmpty(uint32_t timeout=500);

	//
	// for Arduino User (refer from Arduino.CC)
	//

	/**Arduino available() member for serial class.
	 */
	inline int available() {
		return readable();
	}


	/**Arduino parseInt() member for serial class
	 * \brief returns the first valid (long) integer value from the current position.
	 * initial characters that are not digits (or the minus sign) are skipped
	 * integer is terminated by the first character that is not a digit.
	 */
	 virtual long parseInt(bool echo=false);

	 /**parseHex member for stream class
	  *\brief return the first valid (hex) integer value from the current position.
	  */
	 virtual long parseHex(bool echo=false);

	 /**Arduino parseFloat() member for serial class
	  * \brief float version of parseInt
	  */
	 virtual float parseFloat(bool echo=false);

	 /**parseString member for stream class
	  * \brief input a string to specify buffer.
	  * \param buf a string buffer.
	  * \param size of the string buffer.
	  * \param echo to enable the echo to console.
	  * \return string length, if zero, no string input.
	  */
	 virtual int parseString(LPTSTR buf, size_t size, bool echo=false);

	/// @cond
	CStream();
	virtual ~CStream();

	CSemaphore *m_semESC;
	virtual void onSend(bool fromISR) = PURE_VIRTUAL_FUNC;
protected:
	CSemaphore	m_semTx;
	CSemaphore	m_semRx;
	fifo_t		m_tx_fifo;
	fifo_t		m_rx_fifo;
	uint8_t		*m_p_tx_buf;
	uint8_t		*m_p_rx_buf;
	/// @endcond
};

#endif /* STREAM_H_ */
