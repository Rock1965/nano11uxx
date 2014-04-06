/*
 ===============================================================================
 Name        : main.cpp
 Author      : uCXpresso
 Version     : v1.0.0
 Date		 : 2014/4/5
 License   	 : MIT
 Description : I2C test for sensor ADXL345
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/4/5	v1.0.0	First Edition.									Jason
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
#include "sensor/adxl345/ADXL345.h"

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

	// class default I2C address is 0x53
	// specific I2C addresses may be passed as a parameter here
	// ALT low = 0x53 (default for SparkFun 6DOF board)
	// ALT high = 0x1D
	ADXL345 accel;

	// initialize device
	accel.initialize();

	int16_t ax, ay, az;

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
		// read raw accel measurements from device
		//
		accel.getAcceleration(&ax, &ay, &az);
		DBG("ax=%d, ay=%d, az=%d\n", ax, ay, az);

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

