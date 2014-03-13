nano11Uxx (LPC11U3x + nRF BLE)
=========

uCXpresso.BLE RTOS C/C++ Framework for Bluetooth Low Energy

v1.0.2 released 13th March 2014 <br/>
	Feature in the version:<br/>
		When host APP crash or other, the BLE will lock in a connection loop. This version add a Watchdog feature on a BLE connection, the watchdog will force to disconnect a invalid connection.<br/>
		We provide a offline manual in the "doc" folder for reference.<br/>
	
	Details:
	1. Add "doc" folder in "/uCXpresso.BLE" framework.<br/>
	2. Add doxygen class manual and getting started document in "doc" folder.<br/>
    3. Add BLE ACI queue buffer (use CMailBox class)for BLE core transaction. (More Speed up & smooth)<br/>
    4. Add non static post() member function in CMailBox class.<br/>
    5. Add watchdog in BLE core, default watchdog timeout is 10 seconds.<br/>
    6. Add onWatchdog Event in bleSerial class, call back when App crash and BLE core WD timeout.<br/>
    7. Add onError Event in bleSerial class to indicate the BLE core error information.<br/>
    8. Move setTxPowerLevel service from bleSerail to bleProxmity class.<br/>
    9. Build by GCC 4.8.3 <br/>
	10. Update ble examples projects XML with LPCXpresso v7.0.0 <br/>
