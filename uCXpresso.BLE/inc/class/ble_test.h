/*
 ===============================================================================
 Name        : ble_test.h
 Author      : uCXpresso
 Version     : 1.0.0
 Date		 : 2014/5/20
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : BLE Test Mode
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/5/20	v1.0.0	Initialize										Jason
 ===============================================================================
 */

#ifndef BLE_TEST_H_
#define BLE_TEST_H_

#include <class/peripheral.h>
#include <class/ble_serial.h>

typedef enum {
	BLE_TEST_OFF = 0xFF,
	BLE_TEST_OVER_UART = 0x01,
	BLE_TEST_OVER_ACI = 0x02
}BLE_TEST_IF;

class bleTest: public CPeripheral {
public:
	bleTest(bleSerial &ble);

	bool interface(BLE_TEST_IF If);	// select interface
	bool command(uint16_t dtm);		// send DTM command when IF=ACI !!

	/// @cond PRIVATE
protected:
	bleSerial *m_ble;
	/// @endcond
};

#endif /* BLE_TEST_H_ */
