/*
 ===============================================================================
 Name        : eeprom.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2014/1/22
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : EEPROM read and write stack class
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/22	v1.0.0	First Edition for nano11Uxx						Jason
 ===============================================================================
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "class/peripheral.h"

class EEPROM: public CPeripheral {
public:
	static bool read(unsigned addr, void *buf, unsigned size);
	static bool write(unsigned addr, void *buf, unsigned size);
};

#endif /* EEPROM_H_ */
