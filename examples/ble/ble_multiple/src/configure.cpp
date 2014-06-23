/*
 * configure.cpp
 *
 *  Created on: Jun 22, 2014
 *      Author: Jason
 */

#include "configure.h"
#include <class/eeprom.h>
#include <string.h>

#define VERSION	"V1.0.0"

Configure::Configure() {
	EEPROM::read(0, &m_ble, sizeof(m_ble));
	if ( m_ble.length!=sizeof(m_ble) ) {
		setToDefault();
		save();
	}
}

void Configure::setToDefault() {
	strcpy(m_ble.name, "uCXpresso");
	m_ble.txPowerLevel = - 59;
	m_ble.advInterval = 500;
	m_ble.conInterval = 10;
	m_ble.mfgCode = 0x123456;
	m_ble.power = BLE_TX_0dBm;
	m_ble.length = sizeof(m_ble);
}

void Configure::save() {
	EEPROM::write(0, &m_ble, sizeof(m_ble));
}

bool Configure::start() {
	return CThread::start("Config", 148);
}

void Configure::run() {
	m_con.assign(m_usb, m_usb);
	m_usb.enable();

	while(isAlive()) {
		if ( m_usb.isConnected() ) {
			main_menu();
		}
		sleep(1000);
	}
}

void Configure::main_menu() {
	while( m_usb.isConnected() ) {
		m_con.clear();
		m_con << "****************************************" << endl;
		m_con << "*     Welcome to BLE_Multiple          *" << endl;
		m_con << "*                 " VERSION "               *" << endl;
		m_con << "****************************************" << endl;
		m_con.printf("[1] Device Name (%s)\n", m_ble.name);
		m_con.printf("[2] Calibrate Tx Power Level (%d)\n", m_ble.txPowerLevel);
		m_con.printf("[3] Adv. Interval (%d ms)\n", m_ble.advInterval);
		m_con.printf("[4] Con. Interval (%d ms)\n", m_ble.conInterval);
		m_con.printf("[5] Manufacture Code (%d)\n", m_ble.mfgCode);
		m_con.printf("[6] Radio Power 0~3 (%d)\n", m_ble.power);
		m_con.printf("[S] Save current settings\n");

		switch(m_con.getc()) {
		case '1':
			m_con << "Input the BLE Device name:" << flush;
			m_usb.parseString(m_ble.name, sizeof(m_ble.name), true);
			break;

		case '2':
			m_con << "Calibrate Tx Power at 1 Meter:" << flush;
			m_ble.txPowerLevel = m_usb.parseInt(true);
			break;

		case '3':
			m_con << "Input the Adv. Interval (ms):" << flush;
			m_ble.advInterval = m_usb.parseInt(true);
			break;

		case '4':
			m_con << "Input the Con. Interval (ms):" << flush;
			m_ble.conInterval= m_usb.parseInt(true);
			break;

		case '5':
			m_con << "Input the Manufacture Code:" << flush;
			m_ble.mfgCode = m_usb.parseInt(true);
			break;

		case '6':
			m_con << "Input Radio Power:" << flush;
			m_ble.power = (BLE_TX_POWER_T) m_usb.parseInt(true);
			break;

		case 'S':
		case 's':
			save();
			m_con << "Saved!!" << endl;
			m_con.getc();
			break;

		case 'h':
			m_con.printf("Stack High-Water Mark:%d", getStackHighWaterMark());
			m_con.getc();
			break;
		}
	}
}
