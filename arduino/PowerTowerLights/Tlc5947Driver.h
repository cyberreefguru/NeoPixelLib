/***************************************************
 * Author: Thomas M. Sasala
 * Date: 12/1/2013
 * Purpose: TLC5947, 24 channel PWM, driver.  Based
 * on the Adafruit driver.  Heavily customized to
 * suit my needs.
 *
 * Original text:
 *
 * This is a library for our Adafruit 24-channel PWM/LED driver
 *
 * Pick one up today in the adafruit shop!
 * ------> http://www.adafruit.com/products/1429
 *
 * These drivers uses SPI to communicate, 3 pins are required to
 * interface: Data, Clock and Latch. The boards are chainable
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 * BSD license, all text above must be included in any redistribution
 *
 ****************************************************/

#ifndef _TLC5947_DRIVER_H
#define _TLC5947_DRIVER_H

#include <Arduino.h>

#define CHANNELS_PER_DRIVER 24
#define MAX_INTENSITY 4095

class Tlc5947Driver
{

public:
	Tlc5947Driver();

	boolean initialize(uint8_t n, uint8_t c, uint8_t d, uint8_t l, uint8_t b, uint8_t o);

	void setIntensity(uint8_t chan, uint16_t value);
	void setAll(uint16_t value);
	void setBlank(boolean b);
	void setOutputEnable(boolean b);
	void write(void);
	void clear(void);

private:
	uint16_t *pwmbuffer;

	uint8_t numdrivers, clk, dat, lat, oe, blank;
	// we sacrifice memory size for speed; we pre-calculate and save
	// the port mappings and masks, then we can use port manipulation
	// instead of digitalWrite
	uint16_t maxIntensity, totalChannels;
	volatile uint8_t *clockPort, *dataPort, *latchPort, *oePort, *blankPort;
	uint8_t clockMask, dataMask, latchMask, oeMask, blankMask;

};

#endif
