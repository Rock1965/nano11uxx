/*
 ===============================================================================
 Name        : ble_battery.h
 Author      : Jason
 Version     : 1.0.1
 Date		 : 2014/1/22
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : Battery Level Service for BLE
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/19	v1.0.0	Initialize										Jason
 2014/1/22	v1.0.1	Add pie check before the battery to send		Jason
 	 	 	 	 	Add isAvailable() member for service check
 2014/2/5	v1.0.2	Rename getBatteryVoltage() to readSystemVoltage()
 	 	 	 	 	Rename setBatteryLevel() to sendBatteryLevel()
 ===============================================================================
 */

#ifndef BLE_BATTERY_H_
#define BLE_BATTERY_H_

#include "class/peripheral.h"
#include "class/ble_serial.h"

class bleBatteryLevel : virtual public CPeripheral {
public:
	bleBatteryLevel(bleSerial &ble);

	//
	// Read System Voltage from Phy (2.0V ~ 3.6V)
	//
	virtual bool readSystemVoltage(float &voltage);

	//
	// Send Battery Level to Host, (value range: 0 to 100 (0~100%))
	//
	virtual bool  sendBatteryLevel(uint8_t level);

	//
	// check service available or not
	//
	virtual bool isAvailable();

protected:
	bleSerial	*m_ble;	// ble Engine
};


#endif /* BLE_BATTERY_H_ */
