/*
 ===============================================================================
 Name        : ble_heartrate.h
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2014/2/16
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : Heart rate measurement service for BLE
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/2/16	v1.0.0	Initialize										Jason
 ===============================================================================
 */

#ifndef BLE_HEARTRATE_H_
#define BLE_HEARTRATE_H_

#include "class/peripheral.h"
#include "class/ble_serial.h"

#define HR_MAX_PAYLOAD	19

/** Body Sensor Location */
typedef enum {
	HRSL_OTHER = 0,
	HRSL_CHEST,
	HRSL_WRIST,
	HRSL_FINGER,
	HRSL_HAND,
	HRSL_EAR_LOBE,
	HRSL_FOOT
} hrsl_code_t;

/** Possible Heart Rate Control Point Opcodes */
typedef enum hrcp_op_codes_t{
  HRCP_OPCODE_NULL                     = 0x00, /**<Reserved for future use (Operator:N/A)*/
  HRCP_OPCODE_RESET_ENERY_EXPENDED     = 0x01  /**<Resets the value of the Energy Expended field in the Heart Rate Measurement characteristic to 0*/
} hrcp_op_codes_t;

/**Attribute Protocol Application Error codes*/
typedef enum
{
  HRCP_ERR_OK                          = 0x00, /**<No error in control point command.*/
  HRCP_ERR_CONTROL_POINT_NOT_SUPPORTED = 0x80  /**<The received Heart Rate Control Point value is not supported.*/
} hrcp_error_codes_t;

//
// Heart Rate Service class
//
class bleHeartRate: public CPeripheral {
public:
	bleHeartRate(bleSerial &ble);

	virtual bool isAvailable();

	void supportContact(bool enable);
	void contactStatus(bool enable);
	void setSensorLocation(hrsl_code_t code);

	bool sendMeasure(uint8_t meas_hr);
	bool sendMeasure(uint16_t meas_hr);
	bool sendMeasure(uint8_t meas_hr, uint16_t expended_energy);
	bool sendMeasure(uint16_t meas_hr, uint16_t expended_energy);
	bool sendMeasure(uint8_t meas_hr, uint16_t *p_rr_intervals, uint8_t nb_intervals);
	bool sendMeasure(uint16_t meas_hr, uint16_t *p_rr_intervals, uint8_t nb_intervals);
	bool sendMeasure(uint8_t meas_hr, uint16_t expended_energy, uint16_t *p_rr_intervals, uint8_t nb_intervals);
	bool sendMeasure(uint16_t meas_hr, uint16_t expended_energy, uint16_t *p_rr_intervals, uint8_t nb_intervals);

	//
	// Hear Rate Remote Control Event
	//
	virtual void onResetEnergyExpended() {}

	//
	// Private (internal use)
	//
	virtual void onRemoteControl(hrcp_op_codes_t ctrl);
	virtual void sendControlAck(hrcp_error_codes_t code);

protected:
	bleSerial *m_ble;	// ble Engine
	uint8_t current_heart_rate_data[HR_MAX_PAYLOAD];
	bool sendMeasure(uint8_t *meas, uint8_t length);
};

#endif /* BLE_HEARTRATE_H_ */
