/*
 ===============================================================================
 Name        : main.cpp
 Author      : uCXpresso
 Version     : v1.0.0
 Date		 : 2014/5/21
 Copyright   :
 Description : Bluetooth Core Specification v4.0, Vol. 6, Part F,
 	 	 	   ‘Direct Test Mode’
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/5/21	v1.0.0	First Edition.									Jason
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
#include "class/ble_test.h"

//
// TODO: insert other definitions and declarations here
//
CPin ledStandby(LED2);
CPin ledTest(LED3);

class myBLE: public bleSerial {
public:
	virtual void onTestMode() {
		ledStandby = LED_OFF;
		ledTest = LED_ON;		// turn on Test Mode LED
	}

	virtual void onStandbyMode() {
		ledStandby = LED_ON;	// turn on Standby Mode LED
		ledTest = LED_OFF;
	}
};

class testMenu: public CThread {
public:
	bleTest *m_test;

protected:
	usbCDC	m_usb;
	Console m_con;
	virtual void run() {
		m_con.assign(m_usb, m_usb);
		m_usb.enable();

		while(isAlive()) {
			if ( m_usb.isConnected() ) {
				main_menu();
			}
			sleep(3000);
		}
	}

	void main_menu() {
		int freq = 0;
		uint8_t msb, lsb, cmd;
		bool result = false;
		while( m_usb.isConnected() ) {
			m_con.clear();
			m_con << "****************************************" << endl;
			m_con << "*       Welcome to Test Mode           *" << endl;
			m_con << "****************************************" << endl;
			m_con.printf("[0] Set Channel 0~39 (ch%d = %dMHz)\n", freq, freq*2+2402);
			m_con.printf("[1] Reset\n");
			m_con.printf("[2] Receiver Test\n");
			m_con.printf("[3] Transmitter Test\n");
			m_con.printf("[4] Test End\n");

			cmd = m_con.getc();
			switch(cmd) {
			case '0':
				m_con << "Input new Channel:" << flush;
				freq = m_usb.parseInt(true);
				result = true;
				break;

			case '1':	// reset
				msb = (0x00 << 6) | freq;
				lsb = 0x00;
				result = m_test->command(msb, lsb);
				break;

			case '2':	// receiver
				msb = (0x01 << 6) | freq;
				lsb = 0x00;
				result = m_test->command(msb, lsb);
				break;

			case '3':	// transmitter
				msb = (0x02 << 6) | freq;
				lsb = 0x00;
				result = m_test->command(msb, lsb);
				break;

			case '4':	// test end
				msb = (0x03 << 6) | freq;
				lsb = 0x00;
				result = m_test->command(msb, lsb);
				break;

			case 'h':
				m_con.printf("Stack High-Water Mark:%d", getStackHighWaterMark());
				m_con.getc();
				break;
			}

			if ( cmd>='1' && cmd<='4' ) {
				if ( result ) {
					m_con.printf("MSB=%02X, LSB=%02X", msb, lsb);
				} else {
					m_con << "command failed" << endl;
				}
				m_con.getc();
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
	myBLE	ble;
	ble.enable();

	bleTest test(ble);
	test.interface(BLE_TEST_OVER_ACI);

	testMenu menu;
	menu.m_test = &test;
	menu.start("menu", 168);

	CPin led(LED1);


	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/
		led = !led;
		sleep(200);
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
