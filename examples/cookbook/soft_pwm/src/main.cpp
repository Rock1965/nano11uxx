/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2014/1/4
 Copyright   : MIT
 Description : software PWM demo
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/4	v1.0.0	Softwar PWM with MATH function (sin) demo		Jason
 ===============================================================================
 */

#include "uCXpresso.h"
#include "class/serial.h"
#include "class/usb_cdc.h"
#include "debug.h"

//
// TODO: insert other include files here
//
#include <math.h>
#include "swpwm.h"

//
// TODO: insert other definitions and declarations here
//


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
	SWPwm pwm(TIMER0);
	pwm.period(0.02);   // set period time = 20ms for all channels
	pwm.add(LED1, 0.0);	// add PIN LED1 to channel 0
	pwm.add(LED2, 0.0);	// add PIN LED2 to channel 1
	pwm.add(LED3, 0.0); // add PIN LED3 to channel 2
	pwm.add(LED4, 0.0);	// add PIN LED4 to channel 3
	pwm.start();

	float y;
	int x[4] = { 0, 15, 30, 45 };        // initialize all channels x degree

	while (1) {
		//
		// your loop code here
		//
		for (int i = 0; i < 4; i++) {
			x[i] = (x[i] + 2) % 360;                  		// degree 0~360, step by 2
			y = sin((x[i] * M_PI) / 180.0);                 // y = sine @x
			pwm.update(i, map(y, -1.0f, 1.0f, 0.0f, 1.0f)); // update the duty-cycle of channel
		}
		sleep(8);    // speed
	}
	return 0;
}

//
// default memory pool
//
static uint8_t mem_pool[DEFAULT_POOL_SIZE];

//
// setup before the system startup
//
extern "C" void sys_setup(void) {
	pool_memadd((uint32_t) mem_pool, sizeof(mem_pool));
#if __USE_USB==0
	pool_memadd(USB_MEM_BASE, USB_MEM_SIZE);
#endif
}

