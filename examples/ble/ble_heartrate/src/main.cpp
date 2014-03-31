/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : v1.0.2
 Date		 : 2014/2/23
 Description :  Heart rate measurement service for BLE
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/2/18	v1.0.0	First Edition									Jason
 2014/2/21	v1.0.1	Add LED Faded task								Jason
 2014/2/23	v1.0.2	Add Health Thermometer Service					Jason
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
#include "class/ble_serial.h"
#include "class/ble_heartrate.h"
#include "class/ble_battery.h"
#include "class/ble_ht.h"
#include "sensor/heart_beat/pulse_sensor.h"	// use extensions library

//
// TODO: insert other definitions and declarations here
//

//
// myPulseSensor Class
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
// Fade LED Task
//
class ledTask: public CThread {
public:
	ledTask(myPulseSensor &pulse) {
		m_pulse = &pulse;
	}
protected:
	myPulseSensor *m_pulse;

	//
	// Override CThread::run() to show the Faded LEDs
	//
	virtual void run() {
		CBus leds(LED1, LED2, LED3, LED4, END);
		while( isAlive() ) {	// use the isAlive() mean the task can be kill()
			//
			// Indicate led in every 20ms
			//
			if ( m_pulse->fadeRate>0 ) {
				leds[0] = LED_ON;
				if ( m_pulse->fadeRate>64 ) {
					leds[1] = LED_ON;
					if ( m_pulse->fadeRate>128 ) {
						leds[2] = LED_ON;
						if ( m_pulse->fadeRate>196 ) {
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
				m_pulse->fadeRate -= 16;
			} else leds = 0;	// turn off all leds
			sleep(20);
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
	//
	// BLE engine (serial stream)
	//
	bleSerial	ble("myHRM");
	ble.enable(96);

	//
	// Heart Beat Rate Service
	//
	bleHeartRate hrm(ble);
	hrm.supportContact(true);
	hrm.contactStatus(true);
	hrm.setSensorLocation(HRSL_FINGER);

	//
	// Battery Level & Health Thermometer Service (Use internal sensors)
	//
	bleBatteryLevel		 bat(ble);
	bleHealthThermometer ht(ble);

	//
	// Pulse Sensor
	//
	myPulseSensor pulse(AD0);	// signal on AD0
	pulse.start();

	//
	// Fade LED task
	//
	ledTask led(pulse);
	led.start("LED", 50);	// reduce stack size to 58 to increase the heap memory

	//
	//
	//
	CTimeout t1, t2, t3;
	float value;
	uint8_t level;

	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/
		//
		// Heart Rate Measurement
		//
		if ( hrm.isAvailable() ) {
			if ( t1.isExpired(1000) ) {	// interval 1 second
				t1.reset();
				hrm.sendMeasure((uint8_t)pulse.BPM);
			}
		}

		//
		// Battery Level
		//
		if ( t2.isExpired(10000) ) {	// interval 10 seconds
			t2.reset();
			if ( bat.readSystemVoltage(value) ) {
				level = map(value, 2.0, 3.3, 0, 100);	// 0%~100%
				bat.sendBatteryLevel(level);
			}
		}

		//
		// Health Thermometer Service
		//
		if ( t3.isExpired(5000) ) {	// interval 5 seconds
			t3.reset();
			if ( ht.readTemperature(value) ) {
				ht.sendMeasure(value);
			}
		}

	}
    return 0 ;
}

//
// default memory pool
//
static uint8_t mem_pool[DEFAULT_POOL_SIZE-32];

//
// setup before the system startup
//
extern "C" void sys_setup(void) {
	pool_memadd((uint32_t)mem_pool, sizeof(mem_pool));
#if __USE_USB==0
	pool_memadd(USB_MEM_BASE, USB_MEM_SIZE);
#endif
}

