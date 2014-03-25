/*
 ===============================================================================
 Name        : main.cpp
 Author      :
 Version     : v1.0.0
 Date		 :
 Copyright   :
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

class tskLED1 : public CThread {
protected:

	virtual void run() {
		CPin led(LED1);		// use LED1
		//
		// thread run-loop
		//
		while(isAlive()) {
			led = !led;		// blink led
			sleep(100);
		}
	}
};

class tskLED2 : public CThread {
protected:

	virtual void run() {
		CPin led(LED2);		// use LED2
		//
		// thread run-loop
		//
		while(isAlive()) {
			led = !led;		// blink led
			sleep(200);
		}
	}
};

class tskLED3 : public CThread {
protected:

	virtual void run() {
		CPin led(LED3);		// use LED3
		//
		// thread run-loop
		//
		while(isAlive()) {
			led = !led;		// blink led
			sleep(300);
		}
	}
};

class tskLED4 : public CThread {
protected:

	virtual void run() {
		CPin led(LED4);		// use LED4
		//
		// thread run-loop
		//
		while(isAlive()) {
			led = !led;		// blink led
			sleep(400);
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
	tskLED1 led1;
	led1.start();			// use default parameters to call the start member

	tskLED2 led2;
	led2.start("led2");		// task name ="led2", stack = default

	tskLED3 led3;
	led3.start("led3", 40);	// task name ="led3", stack=40 (DWORD)

	tskLED4 led4;
	led4.start("led4", 40);	// task name ="led4", stack=40 (DWORD)

	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/
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

