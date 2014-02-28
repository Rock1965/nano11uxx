/*
===============================================================================
 Name        : main.c
 Author      : Jason
 Version     : v1.0.0
 Copyright   : www.ucxpresso.net
 Description : main definition
===============================================================================
*/
#include "uCXpresso.h"
#include "class/serial.h"
#include "class/usb_cdc.h"
#include "debug.h"

//
// TODO: insert other include files here
//
#include "class/bus.h"
#include "class/watchdog.h"

//
// LED Demo (can be removed)
//
#include "class/bus.h"
static const uint8_t led_scripts[] = { // bit0=LED1, bit1=LED2, bit2=LED3, bit4=LED4
	1, 2, 4, 8, 4, 2
};

//
// led demo task
//
class ledTask: public CThread {
protected:
	virtual void run() {
		CBus leds(LED1, LED2, LED3, LED4, END);

		sleep(500);
		leds = 0x0F;	// turn all on
		sleep(500);
		leds = 0x00;	// turn all off

		uint8_t i = 0;
		while(1) {
			leds = i;
			i = (i+1) & 0x0F;
			sleep(200);
//			CWatchdog::feed();	// if mask the CWatchdog::feed() that will cause the system reset
		}
	}
};

//
// main routine
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

	//
	// your setup code here
	//
	CWatchdog wdt;
	wdt.setTimeout(10); 		// set watchdog timeout 10 seconds
	wdt.enable();				// enable watchdog

	ledTask led;				// a led task for test
	led.start("leds");

	while(1) {
		//
		// your loop code here
		//
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
#if USE_USB==0
	pool_memadd(USB_MEM_BASE, USB_MEM_SIZE);
#endif
}

