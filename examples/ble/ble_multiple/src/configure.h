/*
 * configure.h
 *
 *  Created on: Jun 22, 2014
 *      Author: Jason
 */

#ifndef CONFIGURE_H_
#define CONFIGURE_H_

#include <class/thread.h>
#include <class/console.h>
#include <class/usb_cdc.h>
#include <class/ble_serial.h>

typedef struct _ble_cfg_ {
	int				length;
	char 			name[13];
	int8_t 			txPowerLevel;
	uint16_t 		advInterval;
	uint16_t		conInterval;
	BLE_TX_POWER_T 	power;
	uint32_t		mfgCode;
}BLE_CFG_T;


class Configure: public CThread {
public:
	BLE_CFG_T	m_ble;
public:
	Configure();

	void setToDefault();
	void save();

	virtual bool start();

	inline bool isConnected() {
		return m_usb.isConnected();
	}

protected:
	void main_menu();

	usbCDC	m_usb;
	Console m_con;
	virtual void run();
};

#endif /* CONFIGURE_H_ */
