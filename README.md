nano11uxx
=========

uCXpresso BLE Framework for LPC11U3x (ARM Cortex-M0)

2014/3/2:<br/>
    uCXpresso.BLE update to v1.0.2 rc1<br/>
    1. Add BLE ACI Queue (use CMailBox class) buffer for Hardware. (More Speed up & smooth)<br/>
    2. Add post member function in CMailBox class.<br/>
    3. Add watch-dog in BLE core, default watch-dog timeout is 10 seconds.<br/>
    4. Add onWatchdog Event in bleSerial class, call back then onWatchdog event when App crash.<br/>

2014/3/1:<br/>
    1. uCXpresso.BLE framework build by GCC 4.8.3 <br/>
    2. Update ble examples projects XML with LPCXpresso v7.0.0 <br/>
