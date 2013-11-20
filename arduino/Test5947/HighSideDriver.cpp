/*
 * HighSideDriver.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: tsasala
 */

#include "HighSideDriver.h"

HighSideDriver::HighSideDriver(uint8_t num, uint8_t clk, uint8_t d, uint8_t l,
		uint8_t c, uint8_t o)
{
	  numdrivers = num;
	  this->clock = clk;
	  this->data = d;
	  this->latch = l;
	  this->clr = c;
	  this->oe = o;

	  // Allocate buffer
	  buffer = (uint8_t *)calloc(1, 8*numdrivers);

}

boolean HighSideDriver::begin(void)
{
  if (!buffer) return false;

  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(oe, OUTPUT);

  digitalWrite(clock, LOW);
  digitalWrite(data, LOW);
  digitalWrite(latch, LOW);
  digitalWrite( oe, HIGH );

//  setEnable( false );
//  setClear ( false );

  return true;

}

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

	setEnable( false );
	digitalWrite(10, LOW);
	shiftOut(9, 8, MSBFIRST, buffer[0]);
	digitalWrite(10, HIGH);
	digitalWrite(10, LOW);
	setEnable( true );



//	// 8 channels per chip
//	for (int8_t c=8*numdrivers - 1; c >= 0 ; c--)
//	{
//		// 8 bits per channel, send MSB first
//		for (int8_t b=7; b>=0; b--)
//		{
//			digitalWrite(clock, LOW);
//
//			if (buffer[c] & (1 << b))
//				digitalWrite(data, HIGH);
//			else
//				digitalWrite(data, LOW);
//
//			digitalWrite(clock, HIGH);
//		}
//	}
//	digitalWrite(clock, LOW);
//
//	digitalWrite(latch, HIGH);
//	digitalWrite(latch, LOW);

}

/**
 * Sets output enable pin
 *
 * true = outputs on
 * false = outputs off
 *
 */
void HighSideDriver::setEnable(boolean b)
{
	if( oe == -1 ) return;

	// oe is active low, so "enabled" = low
	if( b )
	{
		digitalWrite(oe, LOW );
	}
	else
	{
		digitalWrite(oe, HIGH );
	}
}

/**
 * Toggles clr pin to clear shift register
 *
 */
void HighSideDriver::clear()
{
	if( clr == -1 ) return;

	// Toggle clear signal, low then high
	setClear(true);
	setClear(false);
}

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
	if( clr == -1 ) return;

	// clr is active low, so "clear" = low
	if( b )
	{
		digitalWrite(clr, LOW);
	}
	else
	{
		digitalWrite(clr, HIGH);
	}
}

