nano11Uxx (LPC11U3x + nRF BLE)
===============================
uCXpresso.BLE RTOS C/C++ Framework for Bluetooth Low Energy
v1.0.7 released 18th August 2014
--------------------------------
###Features:
	1.Update Stream strategy to save the heap memory and speed up. 
	2.Add setup.cpp in project.
	3.Add sleep & wakeup members for BLE.
	4.Add USB wakeup source and isVBUS member function (usbCDC class).
    5.Add Running Speed and Cadence Service for wearable & fitness applications.
    6.Add Cycling Speed and Cadence Service for fitness applications.
    7.Remove Proximity Service (bleProxmity class removed)!!
    8.Remove kill() member function of CThread class.

uCXpresso.BLE RTOS C/C++ Framework for Bluetooth Low Energy
v1.0.6 released 24th May 2014
--------------------------------
###Features:
	1. Kernel update to v8.0.1
	2. Update PWM precisions.
	3. Add BLE "Direct Test Mode" for ODM customer.
	
###Details:
	1. Update kernel to v8.0.1
	2. Add PID and Kalman algorithms from ARM Limited.
	3. Add "bleTest" class for Bluetooth "Direct Test Mode". 
	4. Add PWM GROUP (GROUP-1: PWM1 & PWM2, GROUP-2: PWM3 & PWM4)
	5. Updata PWM precision. (Frequency range: 20Hz ~ 25KHz)
	6. Update PWM to support the fully LOW level in duty-cycle zero.
	7. Add the parseInt, parseFloat, available member functions in CStream and CDebug classes.

v1.0.5 released 26th April 2014
--------------------------------
###Features:
	1. Update "Low Power" Features. 
	2. Support Virtual COM. port in Windows 7 64-bits.
	3. Add "BLE Notify" for "Battery-Level" service.
	
###Details:
	1. Update BLE driver.
	2. Update Low Power Features ( 60uA in Activily )
	3. Update usbCDC (Virtual COM. port) driver to support the Windows 7 64-bits.
	4. Add "Battery-Level" BLE notify.

v1.0.4 released 13th April 2014
--------------------------------
###Features:
	1. Update I2C driver and compatible with I2Cdevlib.
	2. Add a simple gabrage collector template class.
	3. Update the power-save scheme.

###Details:
	1. Add DCMotor class in "/extensions/lib/motor/h-bridge".
	2. Add ADXL345 and MPU-6050 I2C sensor devices in "/uCXpresso.BLE/extensions/lib/sensor".
	3. Move readwrite member from CI2CMaster to CI2C base class.
	4. Fixed I2C lock problem and shift left address bit for Arduino definition.
	5. Add more read and write member functions in CI2CMaster class.
	6. Add Garbage Collector Handle Template Class : gcHandleT<CType>
	7. Update Doxygen Documents.


v1.0.3 released 20th March 2014
--------------------------------	
###Features:
	1. Use interrupt to handle the BLE core, and puhs BLE in to BLOCK status when BLE is in idle mode.
	2. CSerial (UART) and usbCDC classes supports the semaphore to control the FIFO buffer.

###Details:
	1. Add more operators in CStream class.
	2. Add fifo semaphore control in CSerial and usbCDC classes.
	3. Increase more heap memory
	4. Change BLE polling to interrupt method to speed up the BLE core.
	5. BLE become to a Weakup Source for Power Save Features.


v1.0.2 released 13th March 2014
--------------------------------
###Features:
	1. When host APP crash or other, the BLE will lock in a invalid connection loop.
	   This version add a Watchdog feature on a BLE connection, 
	   the watchdog will force to disconnect a invalid connection.
	2. We provide a offline manual in the "doc" folder for reference.
	
###Details:
	1. Add "doc" folder in "/uCXpresso.BLE" framework.
	2. Add doxygen class manual and getting started document in "doc" folder.
    3. Add BLE ACI queue buffer (use CMailBox class)for BLE core transaction. (More Speed up & smooth)
    4. Add non static post() member function in CMailBox class.
    5. Add watchdog in BLE core, default watchdog timeout is 10 seconds.
    6. Add onWatchdog Event in bleSerial class, call back when App crash and BLE core WD timeout.
    7. Add onError Event in bleSerial class to indicate the BLE core error information.
    8. Move setTxPowerLevel service from bleSerail to bleProxmity class.
    9. Build by GCC 4.8.3 
	10. Update ble examples projects XML with LPCXpresso v7.0.0 
