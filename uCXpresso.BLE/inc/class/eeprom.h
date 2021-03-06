/*
 ===============================================================================
 Name        : eeprom.h
 Author      : uCXpresso
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

/**EEPROM class provides to read and write the EEPROM memory.
 * \class EEPROM eeprom.h "class/eeprom.h"
 * \ingroup Peripherals
 */
class EEPROM: public CPeripheral {
public:
	static bool read(unsigned addr, void *buf, unsigned size);
	static bool write(unsigned addr, void *buf, unsigned size);
};

/**\example /peripheral/eeprom/src/main.cpp
 * \brief This is an example of how to use the EEPROM class.
 * More details about this example.
 */

#endif /* EEPROM_H_ */
