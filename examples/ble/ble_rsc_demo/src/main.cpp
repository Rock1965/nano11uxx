/*
 ===============================================================================
 Name        : main.cpp
 Author      : uCXpresso
 Version     : v1.0.0
 Date		 : 2014/8/8
 Copyright   : www.uexpresso.net
 License	 : MIT
 Description : BLE Running Speed and Cadence Service demo
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/8/8	v1.0.0	First Edition.									Jason
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
#include <class/ble_serial.h>
#include <class/ble_rsc.h>
#include <class/ble_battery.h>

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

	/*************************************************************************
	 *
	 *                         your setup code here
	 *
	 **************************************************************************/
	bleSerial ble("uCX-RSC");
	ble.enable();

	//
	// Running Speed Cadence Service
	//
	bleRSC rsc(ble);
	rsc.features(RSC_SUPPORT_STATUS);

	//
	// Battery Level Service
	//
	bleBatteryLevel bl(ble);	// declare Battery Level Service object

	float 	value;
	uint8_t level;
	CPin led(LED1);
	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/
		if ( rsc.isAvailable() ) {
			// speed = 3m/s, cadence=2rpm, distance=2m
			rsc.send( RSC_SPEED_MS(3), 2, RSC_RUNNING);
		}

		if ( bl.isAvailable() ) {
			if ( bl.readSystemVoltage(value) ) {
				if ( value>=2.4 && value<=3.6 ) {
					level =  map(value, 2.0, 3.3, 0, 100);
					bl.sendBatteryLevel(level);
				}
			}
		}

		led = !led;
		sleep(500);
	}
    return 0 ;
}
