/*
 ===============================================================================
 Name        : main.c
 Author      : Jason
 Version     : v1.0.3
 Date		 : 2014/3/10
 Copyright   :
 Description : proximity service for BLE
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/20 v1.0.0	First Edition									Jason
 2014/2/20 v1.0.1	Add tone										Jason
 2014/2/28 v1.0.2	Add Device Information Service					Jason
 2014/3/10 v1.0.3	Rename onLinkLose to onLinkLoss.				Jason
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
#include "class/timeout.h"
#include "class/ble_serial.h"
#include "class/ble_proximity.h"
#include "class/ble_battery.h"
#include "class/ble_ht.h"		// health thermometer service
#include "class/ble_devinfo.h"
#include "class/power.h"
#include "arduino/tone.h"

//
// TODO: insert other definitions and declarations here
//

//
// Alert definitions
//
#define ALERT_FREQ		3800	// alarm 3800Hz
#define ALERT_PIN		P19		// p19 can driver a 40mA for buzzer

CPin ledPower(LED1);
CPin ledAlertH(LED2);
CPin ledAlertL(LED3);

//
// power Save Class
//
class myPowerSave: public CPowerSave {
public:
	virtual void onSleep(uint32_t ulExpectedIdleTime) {
		ledPower = LED_OFF;
		CPowerSave::onSleep(ulExpectedIdleTime);
	}

	virtual void onWakeup(uint32_t *pExpectedIdleTime) {
		CPowerSave::onWakeup(pExpectedIdleTime);
		ledPower = LED_ON;
	}
};

//
// myProximity Class
//
class myProximity : public bleProximity {
public:
	myProximity(bleSerial &ble) : bleProximity(ble) {
		// nothing
	}

	virtual void onAlert(uint8_t level) {
		DBG("On Alert: %d\n", level);
		switch(level) {
		case 0:
			ledAlertH = LED_OFF;
			ledAlertL = LED_OFF;
			break;
		case 1:
			ledAlertL = LED_ON;
			break;
		case 2:
			ledAlertH = LED_ON;
			tone(ALERT_PIN, ALERT_FREQ, 150, 3);	//  duration=150ms, count=3 times
			break;
		}
	}

	virtual void onLinkLoss(uint8_t level) {
		DBG("On Link Lose: %d\n", level);
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

	//
	// your setup code here
	//
	bleSerial ble("myProximity");
	ble.advertising(50, -67);	// set Advertising interval=100ms, txPowerLevel=-67dBm @ 1M
	ble.enable();

	//
	// Device Information Service
	//
	bleDeviceInfo info(ble);
	info.setManufactureName(u8"英倍達國際");	// u8 mean to use the UTF-8 string
	info.setModelNumber("nano11U37");
	info.setSerialNumber("140226000");
	info.setFirmwareRevision(uCXpresso_VER_STR);
	info.setHardwareRevision("R1");
	info.setPnP(VS_USB, 1, 2, 0x3456);		// vendor Id=1, product Id = 2,  product ver. = 0x3456 */

	//
	// Proximity Service
	//
	myProximity 			alert(ble);
	bleBatteryLevel			bat(ble);
	bleHealthThermometer	ht(ble);
	ht.measurementInterval(1);	// set measurement interval = 1 second

	CTimeout t1, t2;
	float 	 value;
	uint8_t	 level;

	CPin		key(P18);
	key.input();

	PIN_LEVEL_T pinVal = key;

#ifndef DEBUG
	myPowerSave ps;						// use power Save feature
#endif

	while(1) {
		if ( ble.isConnected() ) {
#ifndef DEBUG
			ps.disable();
#endif
			//
			// Battery Level
			//
			if ( t1.isExpired(3000) ) {	// interval 3 seconds
				t1.reset();
				if ( bat.readSystemVoltage(value) ) {
					level = map(value, 2.0, 3.3, 0, 100);	// 0%~100%
					bat.sendBatteryLevel(level);
				}
			}

			//
			// Health Thermometer Service
			//
			if ( t2.isExpired(1000) ) {	// interval 1 seconds
				t2.reset();
				if ( ht.readTemperature(value) ) {
					ht.sendMeasure(value);
				}
			}

			//
			// Proximity Service
			//
			if ( key != pinVal ) {
				pinVal = key;
				alert.sendEvent(key.read()==LOW ? 1 : 0);
			}

		} else {
#ifndef DEBUG
			ps.enable();
#endif
			ledAlertH = LED_OFF;
			ledAlertL = LED_OFF;
			sleep(500);
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

