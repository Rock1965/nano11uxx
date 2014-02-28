/*
 ===============================================================================
 Name        : usb_cdc.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2014/1/1
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : USB CDC (virtual comm. port) driver
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	First Edition for nano11Uxx						Jason
 ===============================================================================
 */

#ifndef _USB_CDC_H_
#define _USB_CDC_H_

#include "class/stream.h"
#include "class/semaphore.h"
#include "utils/fifo.h"

typedef struct PACK_STRUCT _cdc_line_coding_ {
  uint32_t dwDTERate;                            /* Data terminal rate in bits per second */
  uint8_t  bCharFormat;                          /* Number of stop bits */
  uint8_t  bParityType;                          /* Parity bit type */
  uint8_t  bDataBits;                            /* Number of data bits */
} CDC_LINE_CODING_T;

class usbCDC: public CStream {
public:
	usbCDC(int FIFO_SIZE=128);

	//
	// Line Control
	//
	bool connect();
	bool disconnect();
	void onSetLineCode(CDC_LINE_CODING_T *line);
	void onCtrlLineState(uint16_t state);

	inline bool enable() {
		return connect();
	}

	inline bool disable() {
		return disconnect();
	}

	//
	// implement the CStream virtual functions
	//
	virtual int	 readable();
	virtual int	 writeable();
	virtual int  read(void *buf, int len, bool block=true);
	virtual int  write(const void *buf, int len, bool block=true);
	virtual bool isConnected();
	virtual void flush();

//
//private:
//
	~usbCDC();
	int  on_usb_send();
	void on_usb_recv();

	xHandle		m_hUsb;
	CSemaphore	m_semIrq;
	FIFO_T		m_txFiFo;
	FIFO_T		m_rxFiFo;
	uint16_t	m_state;
	uint32_t	m_flag;
protected:
	xHandle 	m_hCdc;
	xHandle		m_irqTask;
	uint8_t		*abBulkBuf;
	uint32_t	pbuf;
};

#endif /* USB_CDC_H_ */
