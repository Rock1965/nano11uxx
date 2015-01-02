/*
 ===============================================================================
 Name        : main.cpp
 Author      :
 Version     : v1.0.0
 Date		 :
 Copyright   :
 License	 :
 Description :
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------

 ===============================================================================
 */

#include "uCXpresso.h"
#include "class/serial.h"
#include "class/usb_cdc.h"
#include "debug.h"

#ifdef DEBUG
#define DBG		dbg_printf
#define ASSERT	dbg_assert
#else
#define DBG(...)
#define ASSERT(...)
#endif

//
// TODO: insert other include files here
//
#include <class/thread.h>

//
// TODO: insert other definitions and declarations here
//
void tskProbe(CThread *p_thread, xHandle p_param) {
	CPin pin(P20);
	pin.output();
	while( p_thread->isAlive() ) {
		pin = HIGH;
		delay_us(10);
		pin = LOW;
		delay_us(50);
	}
}

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
	// probe task for delay
	//
	CThread t(tskProbe);
	t.start("probe");

	//
	// LED Demo (can be removed)
	//
	CPin led(LED1);

	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/
		//
		// LED Demo (can be removed)
		//
		led = !led;
		sleep(100);
	}
    return 0 ;
}
