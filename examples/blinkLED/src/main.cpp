/*
 ===============================================================================
 Name        :
 Author      :
 Version     : v1.0.0
 Date		 :
 Copyright   :
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

//
// TODO: insert other include files here
//

//
// TODO: insert other definitions and declarations here
//

//
// LED Demo (can be removed)
//
#include "class/pin.h"

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
	// LED Demo (can be removed)
	//
	CPin led(LED1);
	led.output();	// set the pin as output

	//
	// your setup code here
	//

	while(1) {
		//
		// LED Demo (can be removed)
		//
		led = !led;
		sleep(500);

		//
		// your loop code here
		//

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

