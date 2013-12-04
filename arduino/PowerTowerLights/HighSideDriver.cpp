/*
 * HighSideDriver.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: tsasala
 */

#include "HighSideDriver.h"

HighSideDriver::HighSideDriver()
{
}

/**
 * Initializes the driver
 *
 */
boolean HighSideDriver::initialize(uint8_t num, uint8_t clk, uint8_t d, uint8_t l, uint8_t c, uint8_t o)
{
	numdrivers = num;
	clock = clk;
	data = d;
	latch = l;
	clr = c;
	oe = o;
	buffer = 0;

	totalChannels = num*CHANNELS_PER_DRIVER;


	// Allocate buffer; 1 byte per driver
	buffer = (uint8_t *) calloc(1, numdrivers);
	if (!buffer)
	{
		return false;
	}

	pinMode(clock, OUTPUT);
	pinMode(data, OUTPUT);
	pinMode(latch, OUTPUT);

	digitalWrite(clock, LOW);
	digitalWrite(data, LOW);
	digitalWrite(latch, LOW);

	if (clr != -1)
	{
		pinMode(clr, OUTPUT);
		digitalWrite(clr, LOW);
		digitalWrite(clr, HIGH);
	}

	if (oe != -1)
	{
		pinMode(oe, OUTPUT);
		digitalWrite(oe, LOW);
	}

	return true;

} // end initialize


/**
 * Sets the value of the channel in the buffer
 *
 * NOTE: must call write to take affect
 */
void HighSideDriver::setValue(uint8_t chan, uint8_t value)
{
	buffer[chan] = value;
}

/**
 * Writes value of buffer to shift register
 *
 */
void HighSideDriver::write(void)
{
	int8_t i;

//	setOutputEnable( false );
	digitalWrite(latch, LOW);
	for(i=(numdrivers-1); i>=0; i--)
	{
		shiftOut(data, clock, MSBFIRST, buffer[i]);
	}
	digitalWrite(latch, HIGH);
	digitalWrite(latch, LOW);
//	setOutputEnable( true );

} // end write

/**
 * Sets output enable pin
 *
 * true = outputs on
 * false = outputs off
 *
 */
void HighSideDriver::setOutputEnable(boolean b)
{
	if (oe == -1)
		return;

	// oe is active low, so "enabled" = low
	digitalWrite(oe, !b);

}

/**
 * Toggles clr pin to clear shift register
 *
 */
void HighSideDriver::clear()
{
	if (clr == -1)
		return;

	// Toggle clear signal, low then high
	setClear(true);
	setClear(false);

} // end clear

/**
 * Clears shift register
 *
 * true = clear
 * false = not clear
 *
 * Use clear() to toggle
 */
void HighSideDriver::setClear(boolean b)
{
	if (clr == -1)
		return;

	// clr is active low, so "clear" = low
	digitalWrite(clr, !b);

} // end setClear

