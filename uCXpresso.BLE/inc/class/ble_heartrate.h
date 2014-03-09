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

/**Body Sensor Location
 * \ingroup BLE
 */
typedef enum {
	HRSL_OTHER = 0,		///< Other
	HRSL_CHEST,			///< Chest
	HRSL_WRIST,			///< Wrist
	HRSL_FINGER,		///< Finger
	HRSL_HAND,			///< Hand
	HRSL_EAR_LOBE,		///< Ear Lobe
	HRSL_FOOT			///< Foot
} hrsl_code_t;

/**Possible Heart Rate Control Point Opcodes
 * \ingroup BLE
 */
typedef enum {
  HRCP_OPCODE_NULL                     = 0x00, ///< Reserved for future use (Operator:N/A)
  HRCP_OPCODE_RESET_ENERY_EXPENDED     = 0x01  ///< Resets the value of the Energy Expended field in the Heart Rate Measurement characteristic to 0
} hrcp_op_codes_t;

/**Attribute Protocol Application Error codes
 * \ingroup BLE
 */
typedef enum
{
  HRCP_ERR_OK                          = 0x00, ///< No error in control point command.
  HRCP_ERR_CONTROL_POINT_NOT_SUPPORTED = 0x80  ///< The received Heart Rate Control Point value is not supported.
} hrcp_error_codes_t;

/**The bleHearRate class exposes heart rate and other data from a Heart Rate Sensor intended for fitness applications.
 * \class bleHeartRate ble_heartrate "class/ble_heartrate.h"
 * \ingroup BLE
 */
class bleHeartRate: public CPeripheral {
public:
	/**bleHeartRate constructor
	 * \code
	 * int main() {
	 * ...
	 * 	bleSerial ble("myBLE");
	 * 	ble.enable();
	 *
	 * 	bleHeartRate hr(ble);
	 * 	hr.sendMeasure(BPM);
	 *  ...
	 * \endcode
	 * \param ble is a bleSerial class object.
	 */
	bleHeartRate(bleSerial &ble);

	/**Use isAvailable to check the service whether opened by remote (App).
	 * \return true, if service is available. otherwise, the service is not in used.
	 */
	virtual bool isAvailable();

	/**Set supported Contact sensor feature
	 * \param enable true, if support the contact sensor. otherwise, no contact sensor supported.
	 */
	void supportContact(bool enable);

	/**Set contact sensor status.
	 * \param enable is a boolean value, set true, if contact is detected. otherwise, contact is not detected.
	 */
	void contactStatus(bool enable);

	/**Set sensor location.
	 * \param code is a hrsl_code_t enumeration.
	 */
	void setSensorLocation(hrsl_code_t code);

	//
	// Overload functions
	//

	/** @brief Overload function to send a heart rate measurement (8 bits).
	 *  @param meas_hr Measured heart_rate to send.
	 *  @return : True when the command send successfully.
	 */
	bool sendMeasure(uint8_t meas_hr);

	/** @brief Function to send a heart rate measurement (16 bits).
	 *  @param meas_hr Measured heart_rate to send.
	 *  @return : True when the command send successfully.
	 */
	bool sendMeasure(uint16_t meas_hr);

	/** @brief Overload function to send a heart rate measurement (8 bits) with expended energy.
	 *  @param meas_hr Measured heart_rate to send.
	 *  @param expended_energy Measured expended energy.
	 *  @return : True when the command send successfully.
	 */
	bool sendMeasure(uint8_t meas_hr, uint16_t expended_energy);

	/** @brief Overload function to send a heart rate measurement (16 bits) with expended energy.
	 *  @param meas_hr Measured heart_rate to send.
	 *  @param expended_energy Measured expended energy.
	 *  @return : True when the command send successfully.
	 */
	bool sendMeasure(uint16_t meas_hr, uint16_t expended_energy);

	/** @brief Overload function to send a heart rate measurement (8 bits) with rr_intervals
	 *  @param meas_hr Measured heart_rate to send.
	 *  @param p_rr_intervals Pointer to rr_intervals values.
	 *  @param nb_intervals Number of rr_intervals.
	 *  @return : True when the command send successfully.
	 */
	bool sendMeasure(uint8_t meas_hr, uint16_t *p_rr_intervals, uint8_t nb_intervals);

	/** @brief Overload function to send a heart rate measurement (16 bits) with rr_intervals
	 *  @param meas_hr Measured heart_rate to send.
	 *  @param p_rr_intervals Pointer to rr_intervals values.
	 *  @param nb_intervals Number of rr_intervals.
	 *  @return : True when the command send successfully.
	 */
	bool sendMeasure(uint16_t meas_hr, uint16_t *p_rr_intervals, uint8_t nb_intervals);

	/** @brief Overload function to send a heart rate measurement (8 bits) with expended energy with rr_intervals
	 *  @param meas_hr Measured heart_rate to send.
	 *  @param expended_energy Measured expended energy.
	 *  @param p_rr_intervals Pointer to rr_intervals values.
	 *  @param nb_intervals Number of rr_intervals.
	 *  @return : True when the command send successfully.
	 */
	bool sendMeasure(uint8_t meas_hr, uint16_t expended_energy, uint16_t *p_rr_intervals, uint8_t nb_intervals);

	/** @brief Overload function to send a heart rate measurement (16 bits) with expended energy with rr_intervals
	 *  @param meas_hr measured heart_rate to send.
	 *  @param expended_energy Measured expended energy.
	 *  @param p_rr_intervals Pointer to rr_intervals values.
	 *  @param nb_intervals Number of rr_intervals.
	 *  @return : True when the command send successfully.
	 */
	bool sendMeasure(uint16_t meas_hr, uint16_t expended_energy, uint16_t *p_rr_intervals, uint8_t nb_intervals);

	/**On reset energy expended event call by BLE task when receive remote (App) command.
	 * \note The onResetEnergyExpended is a virtual empty function, and implement by child class.
	 */
	virtual void onResetEnergyExpended() {}

	//
	/// @cond PRIVATE
	//
	virtual void onRemoteControl(hrcp_op_codes_t ctrl);
	virtual void sendControlAck(hrcp_error_codes_t code);

protected:
	bleSerial *m_ble;	// ble Engine
	uint8_t current_heart_rate_data[HR_MAX_PAYLOAD];
	bool sendMeasure(uint8_t *meas, uint8_t length);
	/// @endcond
};

#endif /* BLE_HEARTRATE_H_ */
