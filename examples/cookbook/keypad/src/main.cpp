/*
 ===============================================================================
 Name        : main.cpp
 Author      : Arik
 Version     : v1.0.0
 Date		 : 2014/6/17
 Copyright   : www.embeda.com.tw
 License	 : MIT
 Description :
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/6/17
 ===============================================================================
 */

#include "uCXpresso.h"
#include "class/serial.h"
#include "class/usb_cdc.h"
#include "class/bus.h"
#include "keypad.h"

#if 1
#include "debug.h"
#define DBG	dbg_printf
#else
#define DBG(...)
#endif

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
	CBus rows(P12, P13, P14, END);
	CBus cols(P19, P18, P17, P16, END);
	CKeypad keypad(rows, cols);
	keypad.enable();

	int key;
	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/
		key = keypad.wait(1000);
		switch(key) {
		case -1:	// timeout (no key press)
			DBG("Timeout\n");
			break;
		default:
			DBG("key=%d\n", key);	// 0~11 (for 4x3)
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

