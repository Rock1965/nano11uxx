/*
 ===============================================================================
 Name        : main.cpp
 Author      :
 Version     : v1.0.0
 Date		 : 2014/3/19
 Description : UART TX/RX ECHO Demo
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------

 ===============================================================================
 */

#include "uCXpresso.h"
#include "class/serial.h"
#include "class/usb_cdc.h"
#include "debug.h"

#ifdef DEBUG
#define DBG		dbg_printf
#else
#define DBG(...)
#endif

//
// TODO: insert other include files here
//
#include "class/thread.h"
#include "class/serial.h"
#include "class/pin.h"

//
// TODO: insert other definitions and declarations here
//

CSerial uart;

static const char txbuf[] = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n";

class txTask: public CThread {
protected:
	virtual void run() {
		CPin led(LED1);
		while(1) {
			led = !led;
			uart.write(txbuf, sizeof(txbuf));	// write a buffer to tx of uart
		}
	}
};

class rxTask: public CThread {
protected:
	virtual void run() {
		CPin led(LED2);
		uint8_t ch;
		while(1) {
			led = !led;
			ch = uart;		// read a byte from rx of uart (use C++ operator 'uint8_t')
			DBG("%c", ch);	// show rx data in Debug Mode
		}
	}
};

//
// main task
//
int main(void) {

#ifdef DEBUG
	#if __USE_USB
		usbCDC ser;
		ser.connect();
	#else
		CSerial ser;
		ser.settings(115200);
	#endif
	CDebug dbg(ser);
	dbg.start();
#endif

	/*************************************************************************
	 *
	 *                         your setup code here
	 *
	 **************************************************************************/
	CPin led(LED4);

	uart.enable(115200);

	rxTask rx;
	rx.start("rx", 128, PRI_HIGH);	// Increase stack size for DBG(...), Priority set to High for receive data

	txTask tx;
	tx.start("tx");

	char *buf = new char[128];
	buf[0] = 0;

	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/
		led = !led;
		sleep(100);
	}
    return 0 ;
}

