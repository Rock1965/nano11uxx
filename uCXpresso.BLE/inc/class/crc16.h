/*
 ===============================================================================
 Name        : crc16.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2014/2/11
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : CRC16
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/2/11	v1.0.0	First Edition for nano11Uxx						Jason
 ===============================================================================
 */

#ifndef CRC16_H_
#define CRC16_H_

#include "object.h"

/**CRC16 class
 * \class CRC16 crc16.h "class/crc16.h"
 * \ingroup Miscellaneous
 */
class CRC16: public CObject {
public:
	//
	// default poly = 0x1021 = x^16 + x^15 + x^5 + 1  (for CCITT-16)
	//
	CRC16(uint16_t init=0xffff, uint16_t poly=0x1021);

	virtual void reset();
	virtual void update(uint8_t data);
	virtual void update(uint8_t *buf, int length);
	virtual uint16_t result();

	inline operator uint16_t () {
		return this->result();
	}

	/// @cond
	virtual ~CRC16();
protected:
	uint16_t m_poly;
	uint16_t m_init;
	uint16_t m_crc;
	/// @endcond
};

#endif /* CRC16_H_ */
