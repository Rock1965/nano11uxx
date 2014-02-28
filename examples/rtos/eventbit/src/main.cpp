/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2014/2/22
 Description : Event Bit Group Demo
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/2/22 v1.0.0	First Edition									Jason
 ===============================================================================
 */

#include "uCXpresso.h"
#include "class/serial.h"
#include "class/usb_cdc.h"

#ifdef DEBUG
#include "debug.h"
#define DBG		dbg_printf
#else
#define DBG(...)
#endif

//
// TODO: insert other include files here
//
#include "class/bus.h"
#include "class/thread.h"
#include "class/event.h"	// <--Include the Event Bit Group class

//
// TODO: insert other definitions and declarations here
//
CBus leds(LED1, LED2, LED3, LED4, END);

//
//	Global Event Bit Group Object
//
CEventBit ev;

#define BIT_0	(1<<0)
#define BIT_1	(1<<1)
#define BIT_2	(1<<2)
#define BIT_3	(1<<3)

//
// LED1 Task for LED1 & LED2  (bit0 & bit1)
//
class tskLED1 : public CThread {
protected:
	virtual void run() {
		uint32_t res;
		while( isAlive() ) {
			res = ev.wait(BIT_0 | BIT_1, 	// wait for bit 0 & bit 1 events
					true, 					// auto clear bit
					false); 				// not wait for all
			leds = res;
		}
	}
};

//
// LED2 Task for LED3 & LED4 (bit2 & bit3)
//
class tskLED2 : public CThread {
protected:
	virtual void run() {
		uint32_t res;
		while( isAlive() ) {
			res = ev.wait(BIT_2 | BIT_3,	// wait for bit 2 & bit 3 events
					true,					// auto clear bit
					false);					// not wait for all
			leds = res;
		}
	}
};

//
// LED scripts
//
static const uint8_t led_scripts[] = { // bit0=LED1, bit1=LED2, bit2=LED3, bit4=LED4
	BIT_0,
	BIT_1,
	BIT_2,
	BIT_3,
	BIT_2,
	BIT_1
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

	tskLED1 t1;
	t1.start("LED1");

	tskLED2 t2;
	t2.start("LED2");

	//
	// your setup code here
	//
	uint32_t bits, i=0;
	while(1) {
		//
		// your loop code here
		//

		bits = led_scripts[i];

		// set new bits value to event group
		ev.set(bits);

		// move to next script
		i = (i+1) < sizeof(led_scripts) ? i+1 : 0;

		DBG("bits=%d\n", bits);
		sleep(100);
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
