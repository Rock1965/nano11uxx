nano11Uxx (LPC11U3x + nRF BLE)
=========

uCXpresso.BLE RTOS C/C++ Framework for Bluetooth Low Energy

v1.0.2 rc1 2th March 2014<br/>
    1. Add BLE ACI queue buffer (use CMailBox class)for BLE core transaction. (More Speed up & smooth)<br/>
    2. Add non static post() member function in CMailBox class.<br/>
    3. Add watchdog in BLE core, default watchdog timeout is 10 seconds.<br/>
    4. Add onWatchdog Event in bleSerial class, call back when App crash and BLE core WD timeout.<br/>
    5. Add onError Event in bleSerial class to indicate the BLE core error information.<br/>
    6. Move setTxPowerLevel service from bleSerail to bleProxmity class.<br/>

v1.0.2 rc0 1th March 2014<br/>
    1. Build by GCC 4.8.3 <br/>
    2. Update ble examples projects XML with LPCXpresso v7.0.0 <br/>
