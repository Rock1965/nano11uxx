/*
 ===============================================================================
 Name        : main.cpp
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

#ifdef DEBUG
#define DBG		dbg_printf
#else
#define DBG(...)
#endif

//
// TODO: insert other include files here
//
#include <display/max7219/max7219.h>

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
	CPin led(LED1);

	max7219 dsp(P26);	// use SPI port with P26 (Load/CS) pin
	dsp.enable();

	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/
		led = !led;	// for debug

		dsp.write(1,1);                       //  + - - - - - - -
		dsp.write(2,2);                       //  - + - - - - - -
		dsp.write(3,4);                       //  - - + - - - - -
		dsp.write(4,8);                       //  - - - + - - - -
		dsp.write(5,16);                      //  - - - - + - - -
		dsp.write(6,32);                      //  - - - - - + - -
		dsp.write(7,64);                      //  - - - - - - + -
		dsp.write(8,128);                     //  - - - - - - - +
		sleep(500);

		dsp.write(1,128);                     //  - - - - - - - +
		dsp.write(2,64);                      //  - - - - - - + -
		dsp.write(3,32);                      //  - - - - - + - -
		dsp.write(4,16);                      //  - - - - + - - -
		dsp.write(5,8);                       //  - - - + - - - -
		dsp.write(6,4);                       //  - - + - - - - -
		dsp.write(7,2);                       //  - + - - - - - -
		dsp.write(8,1);                       //  + - - - - - - -
		sleep(500);

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

