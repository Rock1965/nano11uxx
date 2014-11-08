/*
 ===============================================================================
 Name        : ble_app_uart_remote_control
 Author      : uCXpresso
 Version     : v1.0.0
 Date		 : 2014/11/8
 Description : BLE UART Test
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/11/8	v1.0.0	First Edition.									LEO
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
#include <class/ble_serial.h>
#include <class/ble_battery.h>
#include <class/timeout.h>
#include <class/pin.h>

//
// TODO: insert other definitions and declarations here
//
CPin ledLeft(LED1);	// for left
CPin ledRight(LED4);	// for right

/**
 * Command String Parse:
 *
 * Command Package:
 * $  : start of the command (for synchronize)
 * xxx: command
 * #  : end of the command
 *
 * Example: @right# (right), @left# (left)
 */
class cmdParse {
public:
	//
	// command input
	//
	void input(uint8_t ch) {
		switch (ch) {
		case '$':	// start of command
			m_cmd.clear();
			m_bStart = true;
			break;

		case '#':	// end of command
			m_cmd.makeLower();
			onCommand(m_cmd);
			m_bStart = false;
			break;

		default:
			if (m_bStart) {
				m_cmd += (char) ch;
			}
			break;
		}
	}

	//
	// Events
	//

	virtual void onRight() {
		ledRight = LED_ON;
		ledLeft = LED_OFF;
	}

	virtual void onLeft() {
		ledLeft = LED_ON;
		ledRight = LED_OFF;
	}

	virtual void onUp() {
		// TODO for up
	}

	virtual void onDown() {
		// TODO for down
	}

	virtual void onPlay() {
		// TODO for play
	}

	virtual void onStop() {
		ledRight = LED_OFF;
		ledLeft = LED_OFF;
	}

protected:
	bool m_bStart;
	CString m_cmd;

	/**
	 * On command events (string parse)
	 */
	void onCommand(CString &cmd) {
		if (cmd == "right") {
			onRight();
		} else if (cmd == "left") {
			onLeft();
		} else if (cmd == "up") {
			onUp();
		} else if (cmd == "down") {
			onDown();
		} else if (cmd == "play") {
			onPlay();
		} else if (cmd == "stop") {
			onStop();
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
	//
	// BLE
	//
	bleSerial ble("nano11u37");
	ble.enable();

	//
	// my command parse class
	//
	cmdParse cmd;

	//
	// LED
	//
	CPin led2(LED2);
	CPin led3(LED3);
	CTimeout tmLED;
	uint8_t ch=0;
	while(1) {
		/**********************************************************************
		 *
		 *                         your loop code here
		 *
		 **********************************************************************/
		//
		// BLE UART to NANO UART
		//
		if ( ble.isConnected() ) {
			led2 = LED_OFF;
			led3 = LED_OFF;
			if ( ble.readable() ) {
				// Also, block will save the power when tickless enabled.
				ch = ble.read();
				if ( ch ) {
					cmd.input(ch);
				}
			}
		} else {

			//
			// alternate LED
			//
			if ( tmLED.isExpired(500) ) {
				tmLED.reset();
				ch = (ch ? 0 : 1);
				led2 = (ch ? LED_ON : LED_OFF);
				led3 = (ch ? LED_OFF : LED_ON);
			}
		}
	}
    return 0 ;
}
