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
#include "class/bus.h"
#include "keypad.h"

//
// Configuration
//
struct _config_{
	uint8_t pin_code;

}PACK_STRUCT;
typedef struct _config_ CONFIG_T;


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

// Key code
static const uint8_t key_code[] = { // key0=28, key1=11.....key9=44
	28, 11, 21, 41, 12, 22, 42, 14, 24, 44
};
#define SHARP	48
#define STAR	18


#define VERSION	"v0.0.1"

////
//// Menu for Configuration
////
//#ifndef DEBUG
//#include <class/console.h>
//class myMenu: public CThread {
//public:
//	myMenu() {
//	}
//
//	virtual bool start() {
//		return CThread::start("menu", 168);
//	}
//
//protected:
//	usbCDC	m_usb;
//	Console m_con;
//
//	//
//	// Implement the CThread::run() virtual function
//	//
//	virtual void run() {
//		m_con.assign(m_usb, m_usb);
//		m_usb.enable();
//
//		while(1) {
//			if ( m_usb.isConnected() ) {
//				show_welcome();
//			}
//			LEDs[3] = !LEDs[3];
//			sleep(500);
//		}
//	}
//
//	void show_welcome() {
//		while(m_usb.isConnected()) {
//			m_con.clear();
//			m_con << "****************************************" << endl;
//			m_con << "*     Welcome to VLOCK-BLE             *" << endl;
//			m_con << "*                 " VERSION "               *" << endl;
//			m_con << "****************************************" << endl;
//			m_con << "[1] Set Pin Code" << endl;
//			m_con << "[2] Save changed" << endl;
//			switch(m_con.getc()) {
//			case '1' :
//				set_pincode();
//				break;
//			case '2' :
//				saveConfigure();
//				break;
//			}
//		}
//	}
//
//	void show_mpu6050() {
//		while( m_usb.isConnected() ) {
//			m_con.clear();
//			m_con << "****************************************" << endl;
//			m_con << "*            Calibrations              *" << endl;
//			m_con << "****************************************" << endl;
//#if USE_AUTO_TUNING
//			m_con.printf("[1] Set roll angle (%0.4f)\n", config.roll_offset);
//#else
//			m_con.printf("[1] Set roll offset (%0.4f)\n", config.roll_offset);
//#endif
//			m_con.printf("[2] Set left motor power (%0.2f)\n", config.left_power);
//			m_con.printf("[3] Set right motor power (%0.2f)\n", config.right_power);
//			m_con << "[ESC] Return" << endl;
//
//			switch( m_con.getc() ) {
//			case '1':
//#if USE_AUTO_TUNING
//				m_con << "Input roll angle:" << flush;
//#else
//				m_con << "Input roll offset:" << flush;
//#endif
//				config.roll_offset = m_usb.parseFloat(true);
//				break;
//			case '2':
//				m_con << "Input left power:" << flush;
//				config.left_power = m_usb.parseFloat(true);
//				break;
//			case '3':
//				m_con << "Input right power:" << flush;
//				config.right_power = m_usb.parseFloat(true);
//				break;
//			case 0x1B:
//				return;
//			}
//		}
//	}
//
//	void show_pid() {
//		while( m_usb.isConnected() ) {
//			m_con.clear();
//			m_con << "****************************************" << endl;
//			m_con << "*          PID Control tuning          *" << endl;
//			m_con << "****************************************" << endl;
//			m_con.printf("[1] Kp (%0.3f)\n", config.kp);
//			m_con.printf("[2] Ki (%0.3f)\n", config.ki);
//			m_con.printf("[3] Kd (%0.3f)\n", config.kd);
//			m_con.printf("[4] Min. PWM (%0.2f)\n", config.pwm_min);
//			m_con.printf("[5] Max. PWM (%0.2f)\n", config.pwm_max);
//			m_con.printf("[6] Skip Interval (%0.4f)\n", config.skip_interval);
//			m_con << "[ESC] Return" << endl;
//			switch(m_con.getc()) {
//			case '1':
//				m_con << "Input Kp:" << flush;
//				config.kp = m_usb.parseFloat(true);
//				m_robot->tuings(config.kp, config.ki, config.kd);
//				break;
//			case '2':
//				m_con << "Input Ki:" << flush;
//				config.ki = m_usb.parseFloat(true);
//				m_robot->tuings(config.kp, config.ki, config.kd);
//				break;
//			case '3':
//				m_con << "Input Kd:" << flush;
//				config.kd = m_usb.parseFloat(true);
//				m_robot->tuings(config.kp, config.ki, config.kd);
//				break;
//			case '4':
//				m_con << "Input Min. PWM:" << flush;
//				config.pwm_min = m_usb.parseFloat(true);
//				break;
//			case '5':
//				m_con << "Input Max. PWM:" << flush;
//				config.pwm_max = m_usb.parseFloat(true);
//				break;
//			case '6':
//				m_con << "Input Skip Interval:" << flush;
//				config.skip_interval = m_usb.parseFloat(true);
//				break;
//			case 0x1B:
//				return;
//			}
//		}
//	}
//
//	void dashboard() {
//		while(m_usb.isConnected() ) {
//			m_con.clear();
//			m_con << "SetPoint Patch   Roll    PWM\%" << endl;
//			m_con.printf("%0.3f\t %0.3f\t %0.3f\t%0.3f", m_robot->m_setpoint, m_robot->m_pitch, m_robot->m_roll, m_robot->m_output);
//			if ( m_usb.available() ) {
//				if ( m_con.getc()==0x1B ) return;
//			}
//			sleep(250);
//		}
//	}
//
//	void saveConfigure() {
//		EEPROM::write(0, &config, sizeof(config));
//		m_con << "Save finished." << endl;
//		m_con.getc();
//	}
//
//};
//
//#endif


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

	CBus rows(P12, P13, P14, END);
	CBus cols(P19, P18, P17, P16, END);
	CKeypad keypad(rows, cols);
	keypad.enable();

	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/


		DBG("keycode %d\n",keypad.wait(1000));
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

