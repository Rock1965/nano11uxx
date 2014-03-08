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

/**The bleHealthThermometer class exposes temperature and other data from a thermometer intended for healthcare and fitness applications.
 * \class bleHealthThermometer ble_ht.h "class/ble_ht.h"
 * \ingroup BLE
 */
class bleHealthThermometer: public CPeripheral {
public:
	/**Temperature measurement type */
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
	    uint8_t measurement[4];
	    uint8_t time_type[8];
	}h_thermo_temp_measure_t;

public:
	/**bleHealthThermometer constructor.
	 * \code
	 * int main() {
	 * 	...
	 * 	bleSerial ble("myBLE");
	 * 	ble.enable();
	 * 	...
	 * 	bleHealthThermometer ht(ble);
	 * 	ht.unit_c();
	 * 	ht.measurementInterval(3);	// set measurement interval 3 seconds
	 * 	...
	 * 	ht.sendMeasure(temp);
	 * 	...
	 * \endcode
	 * \param[in] ble is a bleSerial class object.
	 * \param[in] type is a h_temp_type_t enumeration.
	 */
	bleHealthThermometer(bleSerial &ble, h_temp_type_t type=H_TYPE_NOT_INCLUDED);

	/**Set temperature unit to Celsius. */
	void unit_c();

	/**Set temperature unit to Fahrenheit. */
	void unit_f();

	/**Set measurement interval (unit second, default 3 seconds)
	 * \param sec is an uint16_t integer to indicate the measurement interval.
	 */
	virtual void measurementInterval(uint16_t sec);

	/**send temperature measurement
	 * \param temp is a floating value to indicate the temperature.
	 * \return true, if send measure successful. otherwise, send failed.
	 */
	virtual bool sendMeasure(float temp);

	/**Set the temperature dynamic type
	 * \param[in] in_type is a h_temp_type enumeration.
	 */
	virtual void setDynamicType(h_temp_type_t in_type);

	/**Use isAvailable to check the service whether opened by remote (App).
	 * \return true, if service is available. otherwise, the service is not in used.
	 */
	virtual bool isAvailable();

	/**Use readTemperature to read the temperature from BLE core sensor
	 * \param[out] temp is a floating value to receive the temperature.
	 * \return true, if read temperature successful. otherwise, read failed.
	 */
	bool readTemperature(float &temp);

	/// @cond PRIVATE
protected:
	h_thermo_temp_measure_t	h_temperature;
	h_temp_type_t 			current_type;
	bleSerial *m_ble;
	/// @endcond
};

#endif /* BLE_HT_H_ */
