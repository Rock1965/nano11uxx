/*
 ===============================================================================
 Name        : main.cpp
 Author      :
 Version     : v1.0.0
 Date		 :
 Copyright   :
 License	 :
 Description :
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
#include <class/thread.h>

//
// TODO: insert other definitions and declarations here
//

class ledTask: public CThread {
public:
	virtual void run() {
		CPin led(LED2);
		while(isAlive()) {
			led.invert();
			sleep(100);
		}
		led = LED_OFF;
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
	ledTask *t = NULL;

	CPin led(LED1);
	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/
		if ( t==NULL ) {
			t = new ledTask;	// alloc a LED task
			t->start("LED");	// start the task
		} else {
			delete t;			// delete the LED task
			t = NULL;
		}

		led.invert();
		sleep(500);
	}
    return 0 ;
}
