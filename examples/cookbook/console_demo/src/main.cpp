/*
 ===============================================================================
 Name        : main.cpp
 Author      : uCXpresso
 Version     : v1.0.0
 Date		 : 2014/5/22
 Description : Console Demo
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/5/22	v1.0.0	First Edition.									LEO
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
#include <class/thread.h>
#include <class/console.h>

//
// TODO: insert other definitions and declarations here
//
class myMenu: public CThread {
public:
	//
	// show welcoem and main-menu
	//
	void show_welcome() {
		while( isAlive() && m_usb.isConnected() ) {
			m_con.clear();
			m_con << "Welcome to Main Menu" << endl;
			m_con << "[1] Menu1" << endl;
			m_con << "[2] Menu2" << endl;
			m_con << "[3] Stack Highwater Mark" << endl;
			switch( m_con.getc() ) {
			case '1':
				menu1();
				break;
			case '2':
				menu2();
				break;
			case '3':
				m_con.printf("Stack Highwater Mark = %d\n", getStackHighWaterMark());
				m_con.getc();
				break;
			}
		}
	}

	//
	// sub-menu (menu-1)
	//
	void menu1() {
		while( isAlive() && m_usb.isConnected() ) {
			m_con.clear();
			m_con << "Menu-1" << endl;
			m_con << "[1] Say Hello" << endl;
			m_con << "[2] Go Back" << endl;
			switch(m_con.getc() ) {
			case '1' :
				m_con << "Hello" << endl;
				m_con.getc(); // wait a key
				break;
			case '2' :
				return;
			}
		}
	}

	//
	// sub-menu (menu-2)
	//
	void menu2() {
		while( isAlive() && m_usb.isConnected() ) {
			m_con.clear();
			m_con << "Menu-2" << endl;
			m_con << "[1] Say Awesome" << endl;
			m_con.printf("[2] Input Integer (%d)\n", nVal);
			m_con.printf("[3] Input Float (%0.2f)\n", fVal);
			m_con << "[4] Go Back" << endl;
			switch( m_con.getc() ) {
			case '1' :
				m_con << "Awesome" << endl;
				m_con.getc();	// wait a key
				break;
			case '2' :
				m_con << "Input integer:" << flush;
				nVal = m_usb.parseInt(true);	// input & parse integer with echo
				break;
			case '3' :
				m_con << "Input floating:" << flush;
				fVal = m_usb.parseFloat(true);	// input & parse floating with echo
				break;
			case '4' :
				return;
			}
		}
	}

protected:
	usbCDC 	m_usb;
	Console m_con;

	int nVal;
	float fVal;

	//
	// implement CThread::run() loop
	//
	virtual void run() {
		m_usb.enable();
		m_con.assign(m_usb, m_usb);

		nVal = 0;
		fVal = 0;
		while(isAlive()) {
			if ( m_usb.isConnected() ) {
				show_welcome();
			}
		}
	}
};

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

//#ifdef DEBUG
//	#if __USE_USB
//		usbCDC ser;
//		ser.connect();
//	#else
//		CSerial ser;
//		ser.settings(115200);
//	#endif
//	CDebug dbg(ser);
//	dbg.start();
//#endif

	/*************************************************************************
	 *
	 *                         your setup code here
	 *
	 **************************************************************************/
	//
	// LED Demo (can be removed)
	//
	uint8_t i = 0;
	CBus port(LED1, LED2, LED3, LED4, END);
	port.output();	// set all pins as output

	//
	// start menu task
	//
	myMenu menu;
	menu.start("Menu", 142);

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

