/*
 ===============================================================================
 Name        : main.cpp
 Author      : uCXpresso
 Version     : v1.0.1
 Date		 : 2014/5/5
 License   	 : MIT
 Description : MAX7219 control
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/5/3	v1.0.0	Initialization									Jason
 2014/5/5	v1.0.1	Add bitmap member function						Jason
 ===============================================================================
 */

#ifndef MAX7219_H_
#define MAX7219_H_

#include <class/spi.h>

#define max7219_reg_noop		0x00
#define max7219_reg_digit0		0x01
#define max7219_reg_digit1		0x02
#define max7219_reg_digit2		0x03
#define max7219_reg_digit3		0x04
#define max7219_reg_digit4		0x05
#define max7219_reg_digit5		0x06
#define max7219_reg_digit6		0x07
#define max7219_reg_digit7		0x08
#define max7219_reg_decodeMode 	0x09
#define max7219_reg_intensity 	0x0a
#define max7219_reg_scanLimit	0x0b
#define max7219_reg_shutdown	0x0c
#define max7219_reg_displayTest	0x0f

class max7219: public CPeripheral {
public:
	max7219(PIN_NAME_T cs);
	void enable();
	void disable();

	void write(uint8_t reg, uint8_t data);

	void bitmap(const uint8_t *bmp);

	inline void shutdown() {
		write(max7219_reg_shutdown, 0x00);
	}

	virtual ~max7219();
protected:
	CSPI m_spi;
	CPin m_cs;
};

#endif /* MAX7219_H_ */
