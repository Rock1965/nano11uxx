/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.4
 Date		 : 2014/3/2
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description : a BLE Firmat & multiple services Demo
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	Initialize										Jason
 2014/1/20	v1.0.1  Add Battery Level and Health Thermometer demo	Jason
 2014/2/22	v1.0.2	Add tone in myProximity class					Jason
 2014/2/28	v1.0.3	Add Device Information Service					Jason
 2014/3/2	v1.0.4	Add onWatchdog() event in myBLE class.			Jason
 	 	 	 	 	Add onError() event in myBLE class.
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
#include "class/ble_proximity.h"
#include "class/ble_devinfo.h"

#include "MyFirmata.h"
#include "callback.h"
#include "arduino/tone.h"
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
	myBLE(): bleSerial("BLE-Multiple") {
		// nothing
	}

	//
	// Override the bleSerial::onConnected() event
	//
	virtual void onConnected() {
		bleSerial::onConnected();

		// your connected event code here
		ledCON = LED_ON;	// turn ON the Connected LED
	}

	//
	// Override the bleSerial::onDisconnected() event
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
	// Override the bleSerial::onWatchdog() event
	//
	virtual void onWatchdog() {
		bleSerial::onWatchdog();

		// your watchdog event code here
		systemResetCallback();
		ledCON = LED_OFF;	// turn OFF all LEDS
		ledRXD = LED_OFF;
		ledTXD = LED_OFF;
	}

	//
	// Override the bleSerial::onError() event
	//
	virtual void onError(BLE_ERR_T err, LPCTSTR id) {
		DBG("Error %d on %s\n", err, id);
	}

	//
	// Override the bleSerial::onBleRecv(...)
	//
	virtual void onBleRecv(uint8_t *buf, int len) {
		bleSerial::onBleRecv(buf, len);

		// your onBleRecv code here
		ledRXD.invert();	// to invert the RXD LED
	}

	//
	// Override the bleSerial::onBleSend()
	//
	virtual void onBleSend(uint8_t ack) {
		bleSerial::onBleSend(ack);

		// your onBleSend code here
		ledTXD.invert();	// to invert the TXD Ack LED
	}
};

//
// power Save Class
//
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
// myProximity Class
//
class myProximity : public bleProximity {
public:
	myProximity(bleSerial &ble) :
		bleProximity(ble),
		m_ledAlert(P5)
	{
		m_ledAlert.output();	// set ledAlert as output pin
	}

	virtual void onAlert(uint8_t level) {
		DBG("On Alert: %d\n", level);
		switch(level) {
		case 0:
			m_ledAlert = LED_OFF;
			break;
		case 1:
			break;
		case 2:
			m_ledAlert = LED_ON;
			tone(P19, 3800, 150, 3);	// freq=3800Hz, duration=150ms, count=3 times
			break;
		}
	}

	virtual void onLinkLose(uint8_t level) {
		DBG("On Link Lose: %d\n", level);
	}
protected:
	CPin m_ledAlert;
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
#else
	#undef __USE_USB
#endif

	/*************************************************************************
	 *
	 *                         your setup code here
	 *
	 **************************************************************************/
	//
	// BLE Engine (Serial Stream)
	//
	myBLE ble;
	ble.advertising(100, -70, 8);	// set adv. interval = 100ms, calibrater tx power = -70dBm, conn. interval=8ms
	ble.enable();					// start the ble engine first!!

	//
	// Device Information Service
	//
	bleDeviceInfo info(ble);
	info.setManufactureName(u8"英倍達國際");	// u8 mean to use the UTF-8 string
	info.setModelNumber("nano11U37");
	info.setSerialNumber("140226000");
	info.setFirmwareRevison(uCXpresso_VER_STR);
	info.setHardwareRevision("R1");
	info.setPnP(VS_USB, 1, 2, 0x3456);		// vendor Id=1, product Id = 2,  product ver. = 0x3456

	SYS_ID_T sysId = {
		{0x00, 0x01, 0x02, 0x03, 0x04}, 	// Manufacturer Identifier
		{0x05, 0x06, 0x07}					// Organizationally Unique Identifier
	};
	info.setSystemId(sysId);

	//
	// Proximity Service
	//
	myProximity alert(ble);	// declare Proximity Service (Immediate alert + Lose Link)

	//
	// Battery Level Service
	//
	bleBatteryLevel bl(ble);	// declare Battery Level Service object

	//
	// Health Thermometer Service
	//
	bleHealthThermometer ht(ble);	// declare Health Thermometer Service object
	ht.measurementInterval(1);			// set measurement interval = 1 second

	//
	// Arduino Firmata
	//
	myFirmata.begin(ble);				// begin the Firmata Object with the ble serial stream.
	callback_init();					// initialize the callback functions for myFirmata Object.

	//
	// A key input for Alert (for Proximity)
	//
	CPin keyAlert(P8);					// define P8 as a push button
	keyAlert.input();
	PIN_LEVEL_T	pinVal = keyAlert;

	//
	// Timeout for time interval
	//
	CTimeout t1, t2, t3;				// t1=analog input check, t2=temperature check, t3=battery check

#ifndef DEBUG
	//
	// Power Save Feature
	//
	myPowerSave ps;						// use power Save feature
#endif

	float 	value;
	uint8_t level;

	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/
		if ( ble.isConnected() ) {
#ifndef DEBUG
			//
			// Power On
			//
			ps.disable(); // disable power save mode
#else
			ledACT = !ledACT;
#endif
			//
			// UART Service
			//
			if ( ble.isAvailable() ) {

				//
				// BLE Firmata Demo
				//
				if ( myFirmata.available() ) {
					do {
						myFirmata.processInput();
					} while(myFirmata.available());
				} else {
					//
					// check the Digital Input
					//
					checkDigitalInputs();

					//
					// check the Analog Input with a sampling Interval
					//
					if (t1.isExpired(samplingInterval) ) {
						t1.reset();
						checkAnalogInputs();
					}
				}
			}

			//
			// Proximity Service
			//
			if ( alert.isAvailable() ) {
					//
					// push button check (Proximity Service)
					//
					if ( keyAlert != pinVal ) {
						pinVal = keyAlert;	// keep last status of key
						alert.sendEvent(keyAlert.read()==LOW ? 1 : 0);
					}
			}

			//
			// Health Thermometer Service
			//
			if ( ht.isAvailable() ) {
				//
				// check temperature
				//
				if ( t2.isExpired(1000) ) {
					t2.reset();
					if ( ht.readTemperature(value) ) {
						ht.sendMeasure(value);
						DBG("temp=%0.2f\n", value);
					} // */
				}
			}

			//
			// Battery Service
			//
			if ( bl.isAvailable() ) {
				//
				// update Battery Level
				//
				if ( t3.isExpired(3000) ) {
					t3.reset();
					if ( bl.readSystemVoltage(value) ) {
						if ( value>=2.4 && value<=3.6 ) {
							level =  map(value, 2.0, 3.3, 0, 100);
							bl.sendBatteryLevel(level);
							DBG("battery:%0.2fv %d%c\n", value, level, '%');
						}
					}
				}
			}

		}	// isConnected
		else {
#ifndef DEBUG
			//
			// Power Save
			//
			ps.enable(POWER_DOWN);	// enable power save mode
#else
			ledACT = LED_ON;
#endif
			sleep(200);
		}
	}
    return 0 ;
}

//
// default memory pool
//
static uint8_t mem_pool[DEFAULT_POOL_SIZE-256];	// reduce pool size to increase global stack

//
// setup before the system startup
//
extern "C" void sys_setup(void) {
	pool_memadd((uint32_t)mem_pool, sizeof(mem_pool));
#if __USE_USB==0
	pool_memadd(USB_MEM_BASE, USB_MEM_SIZE);
#endif
}

