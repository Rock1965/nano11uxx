/*
 ===============================================================================
 Name        : ble_battery.h
 Author      : uCXpresso
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

/**The bleBatteryLevel class exposes the state of a battery within a device.
 * \class bleBatteryLevel ble_battery.h "class/ble_battery.h"
 * \ingroup BLE
 */
class bleBatteryLevel : public CPeripheral {
public:
	/**bleBatteryLevel constructor.
	 * \code
	 * int main() {
	 * 		...
	 * 		bleSerial ble("myBLE");
	 * 		ble.enable();
	 * 		...
	 * 		bleBatteryLevel bat(ble);
	 * 		...
	 * 		float val;
	 * 		uint8_t level;
	 *
	 * 		if ( bat.readSystemVoltage(val) ) {
	 *  		 level = map(val, 2.0f, 3.3f, 0, 100);
	 *  		 bat.sendBatteryLevel(level);
	 * 		}
	 * 		...
	 * \endcode
	 * \param[in] ble is a bleSerial class object.
	 */
	bleBatteryLevel(bleSerial &ble);

	/**Use readSystemVoltage to retrieve the voltage of system (V3.3).
	 * \param[out] voltage is a float type data to receive the system voltage.
	 * \return true, if read system voltage successful. otherwise, read failed.
	 */
	virtual bool readSystemVoltage(float &voltage);

	/**Use sendBatteryLevel to send the battery level to remote (App).
	 * \param[in] level is an unit8_t type integer to indicate the battery level percentage. (0~100%)
	 * \return true, if send battery level successful. otherwise, if send failed.
	 */
	virtual bool  sendBatteryLevel(uint8_t level);

	/**Use isAvailable to check the service whether opened by remote (App).
	 * \return true, if service is available. otherwise, the service is not in used.
	 */
	virtual bool isAvailable();

	/// @cond
protected:
	bleSerial	*m_ble;	// ble Engine
	/// @endcond
};


#endif /* BLE_BATTERY_H_ */
