/*
 ===============================================================================
 Name        : ble_devinfo.h
 Author      : Jason
 Version     : 1.0.2
 Date		 : 2014/2/26
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : Device Information service for BLE
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/2/16	v1.0.0	Initialize										Jason
 2014/2/24	v1.0.1	Add PnP characteristic.							Jason
 2014/2/26	v1.0.2	Add System Id characteristic.					Jason
 ===============================================================================
 */
#ifndef BLE_DEVINFO_H_
#define BLE_DEVINFO_H_

#include "class/peripheral.h"
#include "class/ble_serial.h"

/**Vendor Id Source for Device Information (BLE).
 * \ingroup Enumerations
 */
typedef enum {
	VS_REV = 0,		///< Reserved for future use
	VS_SIG = 1,		///< Bluetooth SIG assigned Company Identifier value from the Assigned Numbers document
	VS_USB = 2		///< USB Implementer’s Forum assigned Vendor ID value
}VID_SOURCE_T;

/**System Id for Device Information (BLE).
 */
typedef struct {
	uint8_t	mfgId[5];	///< Manufacturer Identifier, 40 bits
	uint8_t orgId[3];	///< Organizationally Unique Identifier, 24 bits;
} PACK_STRUCT SYS_ID_T ;

/**bleDeviceInfo provides the Device Information service in BLE device.
 * \class bleDeviceInfo ble_devinfo.h "class/ble_devinfo.h"
 */
class bleDeviceInfo: public CPeripheral {
public:
	bleDeviceInfo(bleSerial &ble);

	int setManufactureName(LPCTSTR str);			// max 20 bytes
	int setModelNumber(LPCTSTR str);				// max 10 bytes
	int setSerialNumber(LPCTSTR str); 				// max 10 bytes
	int setFirmwareRevison(LPCTSTR str);			// max 10 bytes
	int setHardwareRevision(LPCTSTR str);			// max 2 bytes

	void setSystemId(SYS_ID_T &sysId);				// max 8 bytes (first 40bit=MFG. ID, last 24bit=ORG.UID)
	void setPnP(VID_SOURCE_T src, uint16_t vendorId, uint16_t productId, uint16_t productVersion);

	/// @cond
protected:
	bleSerial	*m_ble;
	/// @endcond
};

#endif /* BLE_DEVINFO_H_ */
