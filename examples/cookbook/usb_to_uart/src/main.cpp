/*
 ===============================================================================
 Name        : USB to UART
 Author      : uCXpresso
 Version     : v1.0.0
 Date		 : 2015/1/15
 License	 : Free
 Description : Convert the USB to UART
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2015/1/15	v1.0.0	First Edition.									LEO
 ===============================================================================
 */

#include "uCXpresso.h"
#include "class/serial.h"
#include "class/usb_cdc.h"
#include "class/timeout.h"
#include "debug.h"

#if 0
#define DBG		dbg_printf
#define ASSERT	dbg_assert
#else
#define DBG(...)
#define ASSERT(...)
#endif

//
// TODO: insert other include files here
//

//
// TODO: insert other definitions and declarations here
//
#define SERIAL_BUF_SIZE	16
//
// main task
//
int main(void) {

// Disable DEBUG
#if 0
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
	usbCDC	cdc;
	cdc.enable();

	CSerial	ser;
	ser.enable(115200);	// default baud-rate 115200

	//
	// LED Demo (can be removed)
	//
	CPin ledUSB(LED1);
	CPin ledSER(LED2);
	CPin ledACT(LED4);

	CTimeout tmLED;
	uint8_t buf[SERIAL_BUF_SIZE];
	int len;
	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/
		if ( cdc.isConnected() ) {
			// read from USB to Serial
			if ( cdc.readable() ) {
				ledUSB = LED_ON;
				len = cdc.read(buf, SERIAL_BUF_SIZE, 0);
				if ( len ) {
					ser.write(buf, len);
				}
				ledUSB = LED_OFF;
			}

			// read from Serial to USB
			if ( ser.readable() ) {
				ledSER = LED_ON;
				len = ser.read(buf, SERIAL_BUF_SIZE, 0);
				if ( len ) {
					cdc.write(buf, len);
				}
				ledSER = LED_OFF;
			}
		}

		//
		// LED Demo (can be removed)
		//
		if ( tmLED.isExpired(200) ) {
			tmLED.reset();
			ledACT.toggle();
		}
	}
    return 0 ;
}
