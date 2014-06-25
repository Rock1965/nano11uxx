/*
 ===============================================================================
 Name        : main.cpp
 Author      : uCXpresso
 Version     : v1.0.0
 Date		 : 2014/5/21
 Copyright   :
 Description : Bluetooth Core Specification v4.0, Vol. 6, Part F,
 	 	 	   ‘Direct Test Mode’
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/5/21	v1.0.0	First Edition.									Jason
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
#include "class/ble_test.h"

//
// TODO: insert other definitions and declarations here
//
CPin ledStandby(LED3);
CPin ledTest(LED4);

class myBLE: public bleSerial {
public:
	virtual void onTestMode() {
		ledStandby = LED_OFF;
		ledTest = LED_ON;		// turn on Test Mode LED
	}

	virtual void onStandbyMode() {
		ledStandby = LED_ON;	// turn on Standby Mode LED
		ledTest = LED_OFF;
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
	myBLE	ble;
	ble.enable();

	bleTest test(ble);
	test.interface(BLE_TEST_OVER_ACI);

	CSerial ser;
	ser.enable(19200);

	CPin ledDTM(LED1);

	uint8_t dtm_buf[2];

	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/
		if ( ser.available() ) {
			ledDTM = LED_ON;
			ser.read(dtm_buf, 2);
			test.command(dtm_buf[0], dtm_buf[1]);
			ser.write(dtm_buf, 2);
			ledDTM = LED_OFF;
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
