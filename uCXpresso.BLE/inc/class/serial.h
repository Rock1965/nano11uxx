/*
 ===============================================================================
 Name        : serial.h
 Author      : Jason
 Version     : v1.0.2
 Date		 : 2014/3/18
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : serial stream class
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	First Edition for nano11Uxx						Jason
 2014/2/28	v1.0.1	Add parameter in onLineStatus() member.			Jason
 	 	 	 	 	Rename settings() to enable()
 	 	 	 	 	Add "pull up" function on TXD, RXD pins.
 2014/3/18	v1.0.2	Add semaphore for TX/RX fifo control.			Jason
 ===============================================================================
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include "class/stream.h"
#include "class/pin.h"
#include "utils/fifo.h"
#include "class/semaphore.h"

#define DEF_UART_BAUDRATE	9600

/**Use the CSerial class to transceiver the serial stream.
 * \class CSerial serial.h "class/serial.h"
 * \ingroup Peripherals
 */
class CSerial: public CStream {
public:
	/**
	 * @brief UART Parity type definitions
	 */
	typedef enum {
		UART_PARITY_NONE 	= 4,			/*!< No parity */
		UART_PARITY_ODD		= 0, 			/*!< Odd parity */
		UART_PARITY_EVEN	= 1, 			/*!< Even parity */
		UART_PARITY_SP_1	= 2, 			/*!< Forced "1" stick parity */
		UART_PARITY_SP_0	= 3 			/*!< Forced "0" stick parity */
	} UART_PARITY_T;

	/**
	 * @brief UART Databit type definitions
	 */
	typedef enum {
		UART_DATABIT_5		= 0,     		/*!< UART 5 bit data mode */
		UART_DATABIT_6,		     			/*!< UART 6 bit data mode */
		UART_DATABIT_7,		     			/*!< UART 7 bit data mode */
		UART_DATABIT_8		     			/*!< UART 8 bit data mode */
	} UART_DATABIT_T;

	/**
	 * @brief UART Stop bit type definitions
	 */
	typedef enum {
		UART_STOPBIT_1		= (0),   		/*!< UART 1 Stop Bits Select */
		UART_STOPBIT_2		 				/*!< UART Two Stop Bits Select */
	} UART_STOPBIT_T;


	/**
	 * @brief RX FIFO Trigger Level type definitions
	 */
	typedef enum {
		UART_FIFO_TRGLEV0 = 0,	/*!< UART FIFO trigger level 0: 1 character */
		UART_FIFO_TRGLEV1, 		/*!< UART FIFO trigger level 1: 4 character */
		UART_FIFO_TRGLEV2,		/*!< UART FIFO trigger level 2: 8 character */
		UART_FIFO_TRGLEV3		/*!< UART FIFO trigger level 3: 14 character */
	} UART_FITO_LEVEL_T;

public:
	/**Constructs a CSerial object.
	 * \param fifo_size
	 */
	CSerial(size_t fifo_size=32);

	/**Call the member function to enable the serial port.
	 * \param baudrate is a unsigned long integer to specified the data baud-rate.
	 * \param parity is an UART_PARITY_T enumerations.
	 * \param databits is an UART_DATABIT_T enumerations.
	 * \param stopbits is an UART_STOPBIT_T enumerations.
	 * \param level is UART_FITO_LEVEL_T to trigger how many receiver UARTn FIFO characters must be written before an interrupt or DMA request is activated.
	 */
	virtual void enable(uint32_t baudrate,
						  UART_PARITY_T parity = UART_PARITY_NONE,
						  UART_DATABIT_T databits = UART_DATABIT_8,
						  UART_STOPBIT_T stopbits = UART_STOPBIT_1,
						  UART_FITO_LEVEL_T level=UART_FIFO_TRGLEV0);

	/**Outmoded, redirect to enable() */
	inline void settings(uint32_t baudrate,
			  UART_PARITY_T parity = UART_PARITY_NONE,
			  UART_DATABIT_T databits = UART_DATABIT_8,
			  UART_STOPBIT_T stopbits = UART_STOPBIT_1,
			  UART_FITO_LEVEL_T level=UART_FIFO_TRGLEV0) {
		enable(baudrate, parity, databits, stopbits, level);
	}

	/**Call the member function to check that receive buffer is ready to read.
	 * \return greater than zero if receive data from serial port.
	 */
	virtual int	readable();

	/**Call the member function to check that transmit buffer is ready to write.
	 * \return how many bytes can be write to transmit buffer.
	 */
	virtual int	writeable();

	/**Call the member function to read a data block from serial port.
	 * \param[out] buf is a pointer which contain the data block from serial port.
	 * \param[in] len is a integer to specified the buffer length.
	 * \param[in] block is a boolean value to specified to wait for reading.
	 */
	virtual int	read(void *buf, int len, bool block=true);

	/**Call the member function to write a data block to serial port
	 * \param[in] buf is a pointer which data block want to send to serial port.
	 * \param[in] len is a integer value to specified the buffer length.
	 * \param[in] block is a boolean value to specified to wait for writing.
	 */
	virtual int	write(const void *buf, int len, bool block=true);

	/**Call the member function to wait for transmission  of outgoing serial data to complete.
	 */
	virtual void flush();

	// for Arduino user
	inline void begin(uint32_t speed) {
		enable(speed);
	}

	// for Arduino user
	inline void end() {
		// nothing
	}

	/**Call the member function to check that receive buffer is ready.
	 * \note The 'available' is an inline code to call the exist readable() member function.
	 */
	inline virtual int available() {
		return readable();
	}

	/**Call the member function to check that serial port is ready.
	 */
	virtual inline bool isConnected() {
		return true;
	}

	/**Enable hardware handshaking RTS/CTS pins
	 */
	void handshaking();

	/*! \cond PRIVATE */
public:
	virtual ~CSerial();
	virtual void onUartRecv();
	virtual void onUartSend();
	virtual void onLineStatus(uint32_t reg);	// bitwise line status
	virtual void onCharTimeout();

	CSemaphore	m_semIrq;
protected:
	FIFO_T 	 m_txFifo;
	FIFO_T 	 m_rxFifo;
	CSemaphore	m_semRx;	// rx fifo buffer semaphore
	CSemaphore	m_semTx;	// tx fifo buffer semaphore
private:
	xHandle  m_handle;
	uint32_t m_flag;
	/*! \endcond */
};

//
// UART Line Status Define (bit position)
// for CSerial::onLineStatus()
// Refer to : Page 236 of UM10462
//
#define UART_LSR_RDR	(1<<0)		// Receiver Data Ready:
#define UART_LSR_OE		(1<<1)		// Overrun Error
#define UART_LSR_PE		(1<<2)		// Parity Error
#define UART_LSR_FE		(1<<3)		// Framing Error
#define UART_LSR_BI		(1<<4)		// Break Interrupt
#define UART_LSR_THRE	(1<<5)		// Transmitter Holding
#define UART_LSR_TEMT	(1<<6)		// Transmitter Empty
#define UART_LSR_RXFE	(1<<7)		// Error in RX FIFO
#define UART_LSR_TXERR	(1<<8)		// Tx Error

#endif /* STREAM_H_ */
