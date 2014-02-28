/*
 ===============================================================================
 Name        : startup.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/9/5
 License	 : MIT
 Description : arduino startup
 History	 :
 ===============================================================================
 */
#include "uCXpresso.h"
#include "class/serial.h"
#include "class/usb_cdc.h"
#include "debug.h"

/* ==============================================
 Arduino Class
 ============================================== */
extern void setup();
extern void loop();

class Arduino: public CThread {
protected:
	virtual void run() {
		setup();		// call user's setup code
		while(1) {
			loop();		// call user's loop code
		}
	}
};

/* ==============================================
 main task routine
 ============================================== */
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

	// Start arduino object
	Arduino arduino;
	arduino.start("arduino", 256);

	// Simple demo Code (removable)
	CPin led(LED1);	// on board LED

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);
	}
	return 0;
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
