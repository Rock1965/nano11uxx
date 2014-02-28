/*
 ===============================================================================
 Name        : ble_proximity.h
 Author      : Jason
 Version     : 1.0.1
 Date		 : 2014/1/22
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : Proximity Service for Bluetooth Low Energy
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/19	v1.0.0	Initialize										Jason
 2014/1/22	v1.0.1	Add pie check before the alert to send.			Jason
 	 	 	 	 	Add isAvailable() member for service check
 ===============================================================================
 */
#ifndef BLE_PROXIMITY_H_
#define BLE_PROXIMITY_H_

#include "class/peripheral.h"
#include "class/ble_serial.h"

class bleProximity: public CPeripheral {
public:
	bleProximity(bleSerial &ble);

	//
	// Alert from Host
	//
	virtual void onAlert(uint8_t level);
	virtual void onLoseLink(uint8_t level);

	//
	// Event to Host
	//
	virtual bool sendEvent(uint8_t level);

	//
	// check service available
	//
	virtual bool isAvailable();

	//
	// PRIVATE (internal use)
	//
	virtual ~bleProximity();

protected:
	bleSerial *m_ble;
};

#endif /* BLE_PROXIMITY_H_ */
