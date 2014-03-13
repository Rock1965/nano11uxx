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
#include "class/spi.h"

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

	CSpiMaster spi(P26);		// spi master with selPin=P26
	spi.format(SPI_DATABIT_8);	// data bit =8, default mode=0
	spi.frequency(MHZ(2));		// frequency 2MHz
	spi.enable();

	uint8_t txbuf[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
	uint8_t rxbuf[sizeof(txbuf)];
	uint8_t tx, rx;
	tx = 0xAA;

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
		i = (i+1) <(uint8_t)sizeof(led_scripts) ? i+1 : 0;
		sleep(100);

		//
		// transmit the txbuf only
		//
		spi.readwrite(txbuf, NULL, sizeof(txbuf));

		//
		// receive to rxbuf only
		//
		spi.readwrite(NULL, rxbuf, sizeof(rxbuf));

		//
		// transmit & receive both txbuf and rxbuf.
		//
		spi.readwrite(txbuf, rxbuf, sizeof(txbuf));

		//
		// write to read a byte
		//
		rx = spi.write(tx);

		(void) rx;
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

