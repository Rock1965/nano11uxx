/*
 ===============================================================================
 Name        : ble_proximity.h
 Author      : uCXpresso
 Version     : 1.0.2
 Date		 : 2014/3/2
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
 2014/3/2	v1.0.2	Add setTxPowerLevel member						Jason
 ===============================================================================
 */
#ifndef BLE_PROXIMITY_H_
#define BLE_PROXIMITY_H_

#include "class/peripheral.h"
#include "class/ble_serial.h"


/**bleProximity provide "Immediate Alert" and "Link Loss" services.
 * The bleProximity enables proximity monitoring between two devices.<br/>
 * See Also:<br/>
 * <a href="https://developer.bluetooth.org/TechnologyOverview/Pages/PXP.aspx">Proximity Profile (PXP)</a>
 * \class bleProximity ble_proximity.h "class/ble_proximity.h"
 * \ingroup BLE
 */
class bleProximity: public CPeripheral {
public:
	/**bleProximity constructor.
	 * \param[in] ble is a bleSerial class object.
	 */
	bleProximity(bleSerial &ble);

	/**onAlert event is call by BLE task.
	 * \param[in] level is an uint8_t type value, 0=No Alert, 1=Mild Alert, 2=High Alert, 3-255 reserved.
	 * \remark The onAlert will be implemented by the bleProximity class.
	 * \note The event is defined in the Immediate service of BLE.
	 * The Alert Level characteristic is a control point that allows a peer to command this device to alert to a given level.
	 */
	virtual void onAlert(uint8_t level);

	/**onLinkLoss event is call by BLE task.
	 * \param level is an uint8_t type value, 0=No Alert, 1=Mild Alert, 2=High Alert, 3-255 reserved.
	 * \remark The onLinkLose will be implemented by the bleProxmity class.
	 * \note The event is defined in the Link-Lose service of BLE.
	 * The Alert Level characteristic is used to expose
	 * the current link loss alert level that is used to determine how the device alerts when the link is lost.
	 */
	virtual void onLinkLoss(uint8_t level);

	/**Send alert event to remote (App).
	 * \param[in] level is an uint8_t type value to indicate BLE devie alert level.
	 * \return true, if send alert event successful. otherwise, if send alert event failed.
	 */
	virtual bool sendEvent(uint8_t level);

	/**setTxPowerLevel exposes a device’s current transmit power level when in a connection.
	 * \param[in] dBm is a signed integer and range from +20dBm to -120dBm
	 */
	virtual void setTxPowerLevel(int8_t dBm);	// range: -120 ~ +20 dBm

	/**Use isAvailable to check the service whether opened by remote (App).
	 * \return true, if service is available. otherwise, the service is not in used.
	 */
	virtual bool isAvailable();

	//
	// PRIVATE (internal use)
	//
	/*! \cond PRIVATE */
	virtual ~bleProximity();

protected:
	bleSerial *m_ble;

	/*! \endcond */
};

#endif /* BLE_PROXIMITY_H_ */
