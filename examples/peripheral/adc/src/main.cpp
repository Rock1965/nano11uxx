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

#if DEBUG
#include "debug.h"
#define DBG		dbg_printf
#else
#define DBG(...)
#endif

//
// TODO: insert other include files here
//
#include "class/adc.h"

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

	//
	// LED Demo (can be removed)
	//
	DBG("Hello I'm in debug mode\n");
	uint8_t i = 0;
	CBus port(LED1, LED2, LED3, LED4, END);
	port.output();	// set all pins as output

	//
	// your setup code here
	//
	CAdc a0(AD0);	a0.enable();
	CAdc a1(AD1);	a1.enable();
	CAdc a2(AD2);	a2.enable();
	CAdc a3(AD3);	a3.enable();
	CAdc a4(AD4);	a4.enable();
	CAdc a5(AD5);	a5.enable();


	while(1) {
		//
		// LED Demo (can be removed)
		//
		port = led_scripts[i];
		i = (i+1) < sizeof(led_scripts) ? i+1 : 0;

		//
		// your loop code here
		//
		DBG("a0=%d, a1=%d, a2=%d, a3=%d, a4=%d, a5=%d\n",
				a0.read(),
				a1.read(),
				a2.read(),
				a3.read(),
				a4.read(),
				a5.read());
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

