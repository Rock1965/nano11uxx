/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2014/1/5
 Copyright   :
 Description : Power Management Unit test
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/5	v1.0.0	First Edition									Jason
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
#include "class/power.h"

//
// TODO: insert other definitions and declarations here
//

//
// led & demo
//
CPin probe(P19);	// probe pin for oscilloscope
CPin led(LED4);		// led for demo

//
// Remoulds the CPowerSave class
// to add probe pin and led
//
class myPowerSave: public CPowerSave {
public:
	virtual void onSleep(uint32_t ulExpectedIdleTime) {
		probe = LOW;
		led = probe;
		CPowerSave::onSleep(ulExpectedIdleTime);
	}

	virtual void onWakeup(uint32_t *pExpectedIdleTime) {
		CPowerSave::onWakeup(pExpectedIdleTime);
		probe = HIGH;
		led = probe;
	}
}; // */


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

	//
	// your setup code here
	//
	//
	// LED Demo (can be removed)
	//
	CPin key(P18);	// a input pin for power mode switch
	key.input();

	probe.output();	// set probe as a output pin

	myPowerSave ps;	// declare myPowerSave class to ps

	while(1) {
		//
		// your loop code here
		//
		if ( key==LOW ) {
			ps.disable();
		} else {
			ps.enable(POWER_DOWN);
		}
		sleep(1000);
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

