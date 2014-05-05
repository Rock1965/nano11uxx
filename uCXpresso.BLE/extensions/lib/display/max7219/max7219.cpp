/*
 * max7219.cpp
 *
 */

#include "max7219.h"

#if 1
#include <debug.h>
#define DBG	dbg_printf
#else
#define DBG(...)
#endif

#define DEF_MAX7219_SPI_FREQ	MHZ(2)	// Default 2MHz,  Max. 10MHz


max7219::max7219(PIN_NAME_T cs) :
m_spi(SPI0, false),
m_cs(cs)
{
	m_spi.format(SPI_DATABIT_8);
	m_spi.frequency(DEF_MAX7219_SPI_FREQ);
	m_cs.output(NOT_OPEN, HIGH);
}

max7219::~max7219() {
	shutdown();
	disable();
}

void max7219::enable() {
	uint8_t e;

	m_spi.enable();

	// initialize
	write(max7219_reg_scanLimit, 0x07);
	write(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
	write(max7219_reg_shutdown, 0x01);    // not in shutdown mode
	write(max7219_reg_displayTest, 0x00); // no display test
	for (e=1; e<=8; e++) {    // empty registers, turn all LEDs off
		write(e,0);
	}
	write(max7219_reg_intensity, 0x0f & 0x0f);    // the first 0x0f is the value you can set
}

void max7219::disable() {
	m_spi.disable();
}

void max7219::write(uint8_t reg, uint8_t data) {
	m_cs = LOW;
	m_spi << reg;
	m_spi << data;
	m_cs = HIGH;
}

void max7219::bitmap(const uint8_t *bmp) {
	for (int i=1; i<=8; i++) {
		write(i, bmp[i-1]);
	}
}
