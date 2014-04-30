/*
 ===============================================================================
 * PID Basic Example (refer from Arduino.CC)
 * Reading analog input 0 to control analog PWM output 3
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
#include <class/pin.h>
#include <class/adc.h>
#include <class/thread.h>
#include <algorithm/pid/PID_v1.h>

//
// TODO: insert other definitions and declarations here
//
class pidBasic:public CThread {
public:
	//Define Variables we'll be connecting to
	double Setpoint, Input, Output;

protected:
	virtual void run() {
		//Specify the links and initial tuning parameters
		PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

		//initialize the variables we're linked to
		CAdc ad(AD0);
		ad.enable();

		Input = ad.read();
		Setpoint = 100;

		//turn the PID on
		myPID.SetMode(AUTOMATIC);
		myPID.SetOutputLimits(0, 100);

		CPin led(LED2);
		while(1) {

			Input = ad.read();
			if ( myPID.Compute() ) {
				led = !led;
			}
		}
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
	pidBasic t;
	t.start("pid", 128);

	CPin led(LED1);

	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/

		DBG("Input=%0.4f / Output=%0.4f\n", t.Input, t.Output);

		led = !led;
		sleep(100);
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

