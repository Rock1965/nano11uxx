/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.1
 Date		 : 2014/1/20
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description : a BLE Firmata Demo
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	Initialize										Jason
 2014/1/20	v1.0.1  Add Battery Level and Health Thermometer demo	Jason
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
#include "class/thread.h"
#include "class/bus.h"
#include "class/timeout.h"
#include "class/power.h"

#include "class/ble_serial.h"
#include "class/ble_battery.h"
#include "class/ble_ht.h"

#include "MyFirmata.h"
#include "callback.h"

//
// TODO: insert other definitions and declarations here
//

CBus leds(LED1, LED2, LED3, LED4, END);	// debug leds

#define ledACT	leds[0]		// RF active led
#define ledCON	leds[1]		// Connected led
#define ledRXD	leds[2]		// RXD Event led
#define ledTXD	leds[3]		// TXD Ack	 led

//
// MyFirmataClass Object
//
MyFirmataClass myFirmata;

//
// my BLE class
//
class myBLE: public bleSerial {
public:
	//
	// Construct for myBLE class
	//     set the Vendor Name, Device Name, Device Version
	//
	myBLE(): bleSerial("uC-Firmata") {
		// nothing
	}

	//
	// Override the CSerialBLE::onConnected()
	//
	virtual void onConnected() {
		bleSerial::onConnected();

		// your connected event code here
		ledCON = LED_ON;	// turn ON the Connected LED
	}

	//
	// Override the CSerialBLE::onDisconnected()
	//
	virtual void onDisconnected() {
		bleSerial::onDisconnected();

		// your disconnect event code here
		systemResetCallback();
		ledCON = LED_OFF;	// turn OFF all LEDS
		ledRXD = LED_OFF;
		ledTXD = LED_OFF;
	}

	//
	// Override the CSerialBLE::onBleRecv(...)
	//
	virtual void onBleRecv(uint8_t *buf, int len) {
		bleSerial::onBleRecv(buf, len);

		// your onBleRecv code here
		ledRXD.invert();	// to invert the RXD LED
	}

	//
	// Override the CSerialBLE::onBleSend()
	//
	virtual void onBleSend(uint8_t ack) {
		bleSerial::onBleSend(ack);

		// your onBleSend code here
		ledTXD.invert();	// to invert the TXD Ack LED
	}
};

class myPowerSave: public CPowerSave {
public:
	virtual void onSleep(uint32_t ulExpectedIdleTime) {
		ledACT = LED_OFF;
		CPowerSave::onSleep(ulExpectedIdleTime);
	}

	virtual void onWakeup(uint32_t *pExpectedIdleTime) {
		CPowerSave::onWakeup(pExpectedIdleTime);
		ledACT = LED_ON;
	}
};

//
// main loop
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
	myBLE ble;
	ble.enable(128);

	bleBatteryLevel 	 bl(ble);	// declare Battery Level Service object
	bleHealthThermometer ht(ble);	// declare Health Thermometer Service object

	myFirmata.begin(ble);		// begin the Firmata Object with the ble serial stream.
	callback_init();			// initialize the callback functions for myFirmata Object.

	CTimeout t1, t2;			// t1=battery level update, t2=analog-input interval

#ifndef DEBUG
	myPowerSave ps;				// use power Save
#endif
	ledACT = LED_ON;

	float 	value;
	uint8_t level;

	//
	// your loop code here
	//
	while(1) {
		if ( ble.isConnected() ) {

#ifndef DEBUG
			ps.disable(); // disable power save mode
#endif
			//
			// myFirmataClass Check
			//

			if ( myFirmata.available() ) {
				do {
					myFirmata.processInput();
				} while(myFirmata.available());
			} else {

				if ( t1.isExpired(3000)==false ) {

					//
					// check the Digital Input
					//
					checkDigitalInputs();

					//
					// check the Analog Input with a sampling Interval
					//
					if (t2.isExpired(samplingInterval) ) {
						t2.reset();
						checkAnalogInputs();
					}
				} else {

					t1.reset();


					// update Battery Level
					//
					if ( bl.readSystemVoltage(value) ) {
						if ( value>=2.0 && value<=3.6 ) {
							level =  map(value, 2.0, 3.3, 0, 100);
							bl.sendBatteryLevel(level);
							DBG("battery:%0.2f %d\n", value, level);
						}
					}

					//
					// update Temperature
					//
					if ( ht.readTemperature(value) ) {
						ht.sendMeasure(value);
						DBG("temp=%0.2f\n", value);
					} // */
				}
			}// */
		} else {
#ifndef DEBUG
			ps.enable(POWER_DOWN);	// enable power save mode
#endif
			sleep(200);
		}
	}
    return 0 ;
}

//
// default memory pool
//
static uint8_t mem_pool[DEFAULT_POOL_SIZE-128];

//
// setup before the system startup
//
extern "C" void sys_setup(void) {
	pool_memadd((uint32_t)mem_pool, sizeof(mem_pool));
#if __USE_USB==0
	pool_memadd(USB_MEM_BASE, USB_MEM_SIZE);
#endif
}

