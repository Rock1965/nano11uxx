/*
 ===============================================================================
 Name        : ble_app_uart_v2
 Author      : uCXpresso
 Version     : v1.0.0
 Date		 : 2014/11/8
 Description : BLE UART Test
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/11/8	v1.0.0	First Edition.									LEO
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
#include <class/ble_battery.h>
#include <class/timeout.h>
#include <class/pin.h>

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

#else
	#if __USE_USB
		usbCDC ser;
		ser.enable();
	#endif
#endif

	/*************************************************************************
	 *
	 *                         your setup code here
	 *
	 **************************************************************************/
	bleSerial ble("nano11u37");
	ble.enable();

	CPin led(LED1);
	CTimeout tmLED;
	uint8_t ch;
	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/
		//
		// BLE UART to NANO UART
		//
		if ( ble.isConnected() ) {
			led = LED_ON;
#ifdef DEBUG
			if ( dbg.isDebugMode() ) {
				if ( ble.readable() ) {
					ch = ble.read();
					dbg.putc(ch);
				}

				if ( dbg.available() ) {
					ch = dbg.read();
					ble.write(ch);
				}
			}
#else
			if ( ble.readable() ) {
				ch = ble.read();
				ser.write(ch);
			}

			if ( ser.readable() ) {
				ch = ser.read();
				ble.write(ch);
			}
#endif
		} else {

			//
			// blink LED
			//
			if ( tmLED.isExpired(500) ) {
				tmLED.reset();
				led = !led;
			}
		}
	}
    return 0 ;
}
