/*
 ===============================================================================
 Name        : usb_cdc.h
 Author      : uCXpresso
 Version     : v1.0.2
 Date		 : 2014/7/21
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : USB CDC (virtual comm. port) driver
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	First Edition for nano11Uxx						Jason
 2014/3/18	v1.0.1	Add semaphore for TX/RX fifo control.			Jason
 2014/7/21	v1.0.2	Add asWeakupSource() member.					Jason
 ===============================================================================
 */

#ifndef _USB_CDC_H_
#define _USB_CDC_H_

#include "class/stream.h"
#include "class/semaphore.h"
#include "utils/fifo.h"

typedef struct {
	uint32_t dwDTERate; /* Data terminal rate in bits per second */
	uint8_t bCharFormat; /* Number of stop bits */
	uint8_t bParityType; /* Parity bit type */
	uint8_t bDataBits; /* Number of data bits */
}PACK_STRUCT CDC_LINE_CODING_T;

/**usbCDC class provides the USB CDC virtual COM. port.
 * \class usbCDC usb_cdc.h "class/usb_cdc.h"
 * \ingroup Peripherals
 */
class usbCDC: public CStream {
public:
	/**usbCDC constructor
	 * \param FIFO_SIZE is an integer value to indicate the RX/TX fifo size
	 */
	usbCDC(int FIFO_SIZE = 32);

	/**Enable the usb cdc in core
	 */
	bool enable();

	/**Disable the usb cdc in core
	 */
	bool disable();

	/**On line code change event
	 */
	void onSetLineCode(CDC_LINE_CODING_T *line);

	/**On control line state change event
	 */
	void onCtrlLineState(uint16_t state);

	/**
	 */
	static bool isVBUS();

	/**As a weakup source for Power Save Feature
	 * \brief The interrupt will weakup the MCU from power down mode.
	 */
	void asWeakupSource();

	//
	// old style function call
	//
	/**Inline function, and redirect to enable() member function. */
	inline bool connect() {
		return enable();
	}
	/**Inline function, and redirect to disable() member function. */
	inline bool disconnect() {
		return disable();
	}

	//
	// implement the CStream virtual functions
	//
	virtual int readable();
	virtual int writeable();
	virtual int read(void *buf, int len, bool block = true);
	virtual int write(const void *buf, int len, bool block = true);
	virtual bool isConnected();
	virtual void flush();

//
//private:
//
	/// @cond
	~usbCDC();
	int on_usb_send();
	void on_usb_recv();

	xHandle m_hUsb;
	CSemaphore m_semIrq;
	FIFO_T m_txFiFo;
	FIFO_T m_rxFiFo;
	uint16_t m_state;
	uint32_t m_flag;
protected:
	xHandle m_hCdc;
	xHandle m_irqTask;
	uint8_t *abBulkBuf;
	uint32_t pbuf;
	CSemaphore m_semTx;
	CSemaphore m_semRx;
	/// @endcond
};

#endif /* USB_CDC_H_ */
