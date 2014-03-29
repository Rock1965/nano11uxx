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
#include <class/i2c.h>

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
	// LED Demo (can be removed)
	//
	DBG("Hello I'm in debug mode\n");
	uint8_t i = 0;
	CBus port(LED1, LED2, LED3, LED4, END);
	port.output();	// set all pins as output

	uint8_t data;
	int devAddr=0;
	CI2CMaster i2c;
	i2c.enable();

	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/
		//
		// LED Demo (can be removed)
		//
		port = led_scripts[i];
		i = (i+1) < (int)sizeof(led_scripts) ? i+1 : 0;
		sleep(100);

		//
		// Scan I2C Device
		//
#ifdef DEBUG
		switch( dbg.isAnyKey() ) {
		case 't':
		case 'T':
			devAddr = 1;	// set address =1 to start the scnner.
			break;
		}

		//
		// start to scan when devAddr >0 with timeout 200ms
		//
		if ( devAddr ) {
			if ( i2c.readByte(devAddr, 0, &data)==I2C_OK ) {
				DBG("I2C device found at address 0x%02X\n", devAddr);
			}
			devAddr = (devAddr+1)<127 ? devAddr+1 : 0;
		}
#endif

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

