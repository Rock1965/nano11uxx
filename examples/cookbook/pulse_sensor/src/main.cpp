/*
 ===============================================================================
 Name        : main.cpp (for pulse sensor)
 Author      : uCXpresso
 Version     : v1.0.0
 Date		 : 2014/2/17
 Copyright   :
 Description : pulse sensor
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/2/17	v1.0.0	First Edition.									Jason
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
#include "sensor/heart_beat/pulse_sensor.h"
#include "class/bus.h"
#include "class/timeout.h"

//
// TODO: insert other definitions and declarations here
//
class myPulseSensor: public CPulseSensor {
public:
	int fadeRate;

	myPulseSensor(ADC_CH_T pulsePin) :
		CPulseSensor(pulsePin) {	// Construct for CPulseSensor
		fadeRate = 0;				// used to fade LED on with PWM on fadePin
	}

protected:
	//
	// Override the onBlink for your led event
	//
	virtual void onBlink(PIN_LEVEL_T val) {
		if ( val==HIGH ) {
			fadeRate = 256;
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

	CBus leds(LED1, LED2, LED3, LED4, END);

	//
	// your setup code here
	//
	myPulseSensor pulse(AD0);
	pulse.start();

	CTimeout t1, t2;

	//
	// your loop code here
	//
	while(1) {
		//
		// Indicate led in every 20ms
		//
		if ( t1.isExpired(20) ) {
			t1.reset();
			if ( pulse.fadeRate>0 ) {
				leds[0] = LED_ON;
				if ( pulse.fadeRate>64 ) {
					leds[1] = LED_ON;
					if ( pulse.fadeRate>128 ) {
						leds[2] = LED_ON;
						if ( pulse.fadeRate>196 ) {
							leds[3] = LED_ON;
						} else {
							leds[3] = LED_OFF;
						}
					} else {
						leds[2] = LED_OFF;
					}
				} else {
					leds[1] = LED_OFF;
				}
				pulse.fadeRate -= 16;
			} else leds = 0;	// turn off all leds
		}

		//
		// Show BPM in every 1 second
		//
		if ( t2.isExpired(1000) ) {
			t2.reset();
			DBG("BPM=%d\n", pulse.BPM);
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

