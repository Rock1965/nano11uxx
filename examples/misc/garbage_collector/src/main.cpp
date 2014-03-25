/*
 ===============================================================================
 Name        : main.cpp
 Author      : uCXpresso
 Version     : v1.0.0
 Date		 : 2014/3/24
 Copyright   : MIT
 Description : Garbage Collector Test
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/3/24	v1.0.0	First Edition.									Jason
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
#include "class/mailbox.h"
#include "class/gc.h"

//
// TODO: insert other definitions and declarations here
//

//
// a MailBox for Test
//
CMailBox	mail("LED");

//
// Define a GC point with CPin type.
//
#define GCPin gcHandleT<CPin>

//
// GC LED Test Thread
//
class gcLED : public CThread {
protected:
	virtual void run() {
		GCPin pin;

		// wait for led object from mailbox
		pin = mail.wait();		// use operator '= (xHandle)' to receive the GC object.

		// turn off the LED by CPin operator
		*pin = LED_OFF;

		// thread while-loop
		while(isAlive()) {

			// blink LED
			if ( pin ) {
				pin->invert();
			}
			sleep(200);
		}
		DBG("gcLED destroyed\n");
	}
};

//
// Send a PIN to mailbox
//
void send_mail_test() {
	// Allocates a CPin Garbage Collector object and connects to the LED1
	GCPin pin = new CPin(LED1);

	// Output LED ON
	pin->output();
	*pin = LED_ON;

	// post pin to mail
	if ( mail.post(pin) ) {		// use operator 'xHandle' to post the GC object.
		pin.reference();		// increase reference count for receiver, IF POST SUCCESSFUL.
	}

	// the pin object will be destroyed after the function returned
}

//
// Auto release test
//
void auto_release_test() {
	DBG("auto_release_test in:\n");
	GCPin p1 = new CPin(LED2);	// Constructs p1 to point to new CPin(LED2)
	GCPin p2(new CPin(LED3));	// Constructs p2 to point to new CPin(LED3)
	GCPin p3;					// p3 is an empty pointer.

	*p1 = LED_OFF;				// turns OFF the LED2 by operator of CPin class.
	*p2 = LED_OFF;				// turns OFF the LED3 by operator of CPin class.

	p3 = new CPin(LED4);		// assigns p3 to new CPin(LED4)
	p3->write(LED_ON);			// turns ON the LED4 by member write() of CPin class.

	sleep(1000);

	*p3 = LED_OFF;				// Also can be turned OFF by CPin operator.

	p3 = p1;					// p3 point to p1, and FORGET TO FREE the CPin(LED4) ?
	p3->write(LED_ON);			// turns ON the LED2 ?

	p3 = p2;					// p3 point to p2,
	p3->write(LED_ON);			// turns ON the LED3 ?

	p2 = p1;					// p2 point to p1
	p1 = p3;					// p1 point to p3, and FORGET TO FREE something ?

	p3 = NOTHING;				// forces to releases the p3.
								// are p1 and p2 freed after function returned ?

	DBG("auto_release_test out:\n");
}

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
	// start the GC Test Thread
	gcLED gc;
	gc.start("gc", 48);

	// send Pin to mailbox
	send_mail_test();

	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/
		switch(dbg.isAnyKey()) {
		//
		// in DEBUG mode, press 'a' to test the 'auto_release_test()'
		//
		case 'a':
			auto_release_test();
			break;
		}
	}
    return 0 ;
}

//
// default memory pool
//
static uint8_t mem_pool[DEFAULT_POOL_SIZE];

//
// setup before the system startup
//
extern "C" void sys_setup(void) {
	pool_memadd((uint32_t)mem_pool, sizeof(mem_pool));
#if __USE_USB==0
	pool_memadd(USB_MEM_BASE, USB_MEM_SIZE);
#endif
}

