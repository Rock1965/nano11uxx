/*
 ===============================================================================
 Name        : ble_running_speed.h
 Author      : uCXpresso
 Version     : 1.0.0
 Date		 : 2014/8/8
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : Cycling Speed and Cadence Service
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/8/8	v1.0.0	Initialize										Jason
 ===============================================================================
 */

#ifndef BLE_CSC_H_
#define BLE_CSC_H_

#include <class/peripheral.h>
#include <class/ble_serial.h>

//
// CSC Flasg
//
#define CSC_FLAG_WHEEL	(1<<0)
#define CSC_FLAG_CRANK	(1<<1)

//
// CSC Features
//
#define CSC_SUPPORT_WHEEL_DATA		(1<<0)	///< CSC feature support the wheel data
#define CSC_SUPPORT_CRANK_DATA		(1<<1)	///< CSC feature support the crank data
//#define CSC_SUPPORT_MULTIPLE_SENSOR	(1<<2)

/**The bleCSC class exposes speed-related and cadence-related data from a Cycling Speed and Cadence sensor intended for fitness applications.
 * \class bleCSC ble_csc.h "class/ble_csc.h"
 * \ingroup BLE
 */
class bleCSC: public CPeripheral {
public:
	/**\brief bleCSC constructor
	 */
	bleCSC(bleSerial &ble);

	/**\brief Function to set the CSC features
	 * \param value a bit wide value by CSC_SUPPOT_XXXX defines.
	 */
	virtual void features(uint16_t value);

	/**Use isAvailable to check the service whether opened by remote (App).
	 * \return true, if service is available. otherwise, the service is not in used.
	 */
	virtual bool isAvailable();

	/**\brief Overload Function, send the CSC Measurement data for wheel.
	 * \param cumulative_wheel_revolutions
	 * \param last_wheel_event_time	Unit has a resolution of 1/1024s.
	 * \return true, if send the data successful.
	 */
	inline bool send(uint32_t cumulative_wheel_revolutions, uint16_t last_wheel_event_time) {
		return send(CSC_FLAG_WHEEL, cumulative_wheel_revolutions, last_wheel_event_time, 0, 0);
	}

	/**\brief Overload function, send the CSC Measurement data for crank.
	 * \param cumulative_crank_revolutions
	 * \param last_crank_event_time	Unit has a resolution of 1/1024s.
	 * \return true, if send the data successful.
	 */
	inline bool send(uint16_t cumulative_crank_revolutions, uint16_t last_crank_event_time) {
		return send(CSC_FLAG_CRANK, 0, 0, cumulative_crank_revolutions, last_crank_event_time);
	}

	/**\brief Overload function, send the CSC Measurement data for wheel and crank.
	 * \param cumulative_wheel_revolutions
	 * \param last_wheel_event_time	Unit has a resolution of 1/1024s.
	 * \param cumulative_crank_revolutions
	 * \param last_crank_event_time	Unit has a resolution of 1/1024s.
	 * \return true, if send the data successful.
	 */
	inline bool send(uint32_t cumulative_wheel_revolutions,
					 uint16_t last_wheel_event_time,
					 uint16_t cumulative_crank_revolutions,
					 uint16_t last_crank_event_time) {

		return send(CSC_FLAG_WHEEL | CSC_FLAG_CRANK,
					cumulative_wheel_revolutions, last_wheel_event_time,
					cumulative_crank_revolutions, last_crank_event_time);
	}

	//
	/// @cond PRIVATE
	//
	virtual ~bleCSC();

protected:
	virtual bool send(uint8_t flag,
					  uint32_t cumulative_wheel_revolutions,
					  uint16_t last_wheel_event_time,
					  uint16_t cumulative_crank_revolutions,
					  uint16_t last_crank_event_time);

	bleSerial *m_ble;
	/// @endcond
};

#endif /* BLE_RUNNING_SPEED_H_ */
