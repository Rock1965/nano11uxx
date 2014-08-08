/*
 ===============================================================================
 Name        : ble_running_speed.h
 Author      : uCXpresso
 Version     : 1.0.0
 Date		 : 2014/8/8
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : Running Speed and Cadence
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/8/8	v1.0.0	Initialize										Jason
 ===============================================================================
 */

#ifndef BLE_RSC_H_
#define BLE_RSC_H_

#include <class/peripheral.h>
#include <class/ble_serial.h>

/**RSC Status Enumeration
 * \ingroup Enumerations
 */
typedef enum {
	RSC_WALKING = 0,	///< WALKING
	RSC_RUNNING	= 1		///< RUNNING
}RSC_STATUS_T;

//
// RSC Features
//
#define RSC_SUPPORT_STRIDE_LENGTH	(1<<0)
#define RSC_SUPPORT_DISTANCE		(1<<1)
#define RSC_SUPPORT_STATUS			(1<<2)	// for walking or running status
//#define RSC_SUPPORT_CALIBRATION		(1<<3)
//#define RSC_SUPPORT_MULTIPLE_SENSOR	(1<<4)

//
// RSC Speed
//
#define RSC_SPEED_KMH(x)	((uint16_t)(x*71.1f))	// speed for Km/h
#define RSC_SPEED_MS(x)		((uint16_t)(x*256.0f))	// speed for m/s

/**The bleRSC class exposes speed, cadence and other data from a Running Speed and Cadence Sensor intended for fitness applications.
 * \class bleRSC ble_rsc.h "class/ble_rsc.h"
 * \ingroup BLE
 */
class bleRSC: public CPeripheral {
public:
	/**\brief bleRSC constructor
	 */
	bleRSC(bleSerial &ble);

	/**\brief Function to set the RSC features
	 * \param value a bit wide value by RSC_SUPPOT_XXXX defines.
	 */
	virtual void features(uint16_t value);

	/**Use isAvailable to check the service whether opened by remote (App).
	 * \return true, if service is available. otherwise, the service is not in used.
	 */
	virtual bool isAvailable();

	/**\brief Function	send the RSC Measurement data
	 * \param status, RSC_WALKING or RSC_RUNNIG.
	 * \param speed, Unit is in m/s with a resolutions of 1/256 s
	 * \param cadence, Unit is in 1/minute (or RPM) with a resolutions of 1 1/min (or 1RPM)
	 * \param distance (optional), Unit is in meter with a resolution of 1/10 m (or decimeter).
	 * \param stride_length (optional), Unit is in meter with a resolution of 1/100 m (or centimeter).
	 * \return true, if send the data successful.
	 */
	virtual bool send(uint16_t speed, uint8_t cadence, RSC_STATUS_T status=RSC_WALKING, uint32_t distance=0, uint16_t stride_length_cm=0);

	//
	/// @cond PRIVATE
	//
	virtual ~bleRSC();
protected:
	bleSerial *m_ble;
	/// @endcond
};

#endif /* BLE_RUNNING_SPEED_H_ */
