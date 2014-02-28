/*
 ===============================================================================
 Name        : ble_ht.h
 Author      : Jason
 Version     : 1.0.3
 Date		 : 2014/2/5
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : Health Thermometer Service for BLE
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/18	v1.0.0	Initialize										Jason
 2014/1/19	v1.0.1	Update float format for IEEE 11073				Jason
 	 	 	 	 	Add measurementInterval() member				Jason
 2014/1/22	v1.0.2	Add pie check before the temp. to send.			Jason
 	 	 	 	 	Add isAvailable() member for service check.
 2014/2/5	v1.0.3	Rename getTemperature() to readTemperature()	Jason
 ===============================================================================
 */

#ifndef BLE_HT_H_
#define BLE_HT_H_

#include "class/peripheral.h"
#include "class/ble_serial.h"

/** Temperature measurement type
 */
typedef enum h_temp_type_t
{
    H_TYPE_NOT_INCLUDED                  = 0,
    H_TYPE_ARMPIT                        = 1,
    H_TYPE_BODY                          = 2,
    H_TYPE_EAR                           = 3,
    H_TYPE_FINGER                        = 4,
    H_TYPE_GASTRO_INTESTINAL_TRACT       = 5,
    H_TYPE_MOUTH                         = 6,
    H_TYPE_RECTUM                        = 7,
    H_TYPE_TOE                           = 8,
    H_TYPE_TYMPANUM                      = 9,
}h_temp_type_t;

/** Temperature measurement structure*/
typedef struct PACK_STRUCT h_thermo_temp_measure_t
{
    uint8_t flags;
    uint8_t  measurement[4];
    uint8_t time_type[8];
}h_thermo_temp_measure_t;

//
// ble Health Thermometer class
//
class bleHealthThermometer: public CPeripheral {
public:
	bleHealthThermometer(bleSerial &ble, h_temp_type_t type=H_TYPE_NOT_INCLUDED);

	// set unit to Celsius.
	void unit_c();

	// set unit to Fahrenheit
	void unit_f();

	// set measurement interval (unit second, default 3 seconds)
	virtual void measurementInterval(uint16_t sec);

	// send temperature measurement
	virtual bool sendMeasure(float temp);

	// set the temperature dynamic type (used in the temperature measurement characteristic field).
	virtual void setDynamicType(h_temp_type_t in_type);

	//
	// check service is available or not
	//
	virtual bool isAvailable();

	// get the temperatuer from PHY sensor
	bool readTemperature(float &temp);

protected:
	h_thermo_temp_measure_t	h_temperature;
	h_temp_type_t 			current_type;
	bleSerial *m_ble;
};

#endif /* BLE_HT_H_ */
