/*
 ===============================================================================
 Name        : main.cpp
 Author      :
 Version     : v1.0.0
 Date		 : 2014/2/28
 Copyright   :
 Description : UART Echo to USB (Short the Pin 16 (RXD) to Pin 17 (TXD))
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/2/28 v1.0.0	First Edition									Jason
 ===============================================================================

 Echo Loop :

 PC -> USB RX -> UART TX --
                          |
 PC <- USB TX <- UART RX __

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

//
// TODO: insert other definitions and declarations here
//

//
// LED Demo (can be removed)
//
#include "class/bus.h"
static const uint8_t led_scripts[] = { // bit0=LED1, bit1=LED2, bit2=LED3, bit4=LED4
	1, 2, 4, 8, 4, 2
};

//
// main task
//
int main(void) {

	/*************************************************************************
	 *
	 *                         your setup code here
	 *
	 **************************************************************************/
	//
	// LED Demo (can be removed)
	//
	DBG("Hello I'm in debug mode\n");
	uint8_t i = 0;
	CBus leds(LED1, LED2, LED3, LED4, END);
	leds.output();	// set all pins as output


	usbCDC usb;
	usb.enable();			// enable USB core

	CSerial uart;
	uart.enable(115200);	// enable serial port

	int ch = 0;

	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/

		//
		// check uart input
		//
		if ( uart.readable() ) {
			//
			//
			// show led scripts
			leds = led_scripts[i];
			i = (i+1) < sizeof(led_scripts) ? i+1 : 0;

			//
			// read from uart
			//
			if ( uart.read(&ch, 1) ) {
				usb.write(&ch, 1);	 // output to usb
			}
		}

		//
		// check usb input
		//
		if ( usb.readable() ) {
			if ( usb.read(&ch, 1) ) {
				uart.write(&ch, 1);	// output to uart
			}
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

