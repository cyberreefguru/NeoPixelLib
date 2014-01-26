/*
 * HighSideDriver.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: tsasala
 */

#include "HighSideDriver.h"

HighSideDriver::HighSideDriver()
{
	buffer = 0;
	numdrivers = 0;
	totalChannels = 0;

	clockPort = 0;
	dataPort = 0;
	latchPort = 0;
	oePort = 0;
	clrPort = 0;

	clockMask = 0xff;
	dataMask = 0xff;
	latchMask = 0xff;
	oeMask = 0xff;
	clrMask = 0xff;
}

/**
 * Initializes the driver
 *
 */
boolean HighSideDriver::initialize(uint8_t num, uint8_t clk, uint8_t d, uint8_t l, uint8_t c, uint8_t o)
{
	// initialize variables
	numdrivers = num;
	totalChannels = num*CHANNELS_PER_DRIVER;

	// Calculate data port values
	clockPort = portOutputRegister( digitalPinToPort( clk ) );
	latchPort = portOutputRegister( digitalPinToPort( l ) );
	dataPort = portOutputRegister( digitalPinToPort( d ) );

	// Calculate data mask values
	clockMask = digitalPinToBitMask( clk );
	latchMask = digitalPinToBitMask( l );
	dataMask = digitalPinToBitMask( d );

	pinMode(clk, OUTPUT);
	pinMode(d, OUTPUT);
	pinMode(l, OUTPUT);

	*clockPort &= ~clockMask;
	*latchPort &= ~latchMask;
	*dataPort &= ~dataMask;

	if (c != -1)
	{
		pinMode(c, OUTPUT);
		clrPort = portOutputRegister( digitalPinToPort( c ) );
		clrMask = digitalPinToBitMask( c );

		*clrPort &= ~clrMask; // low = clear
		*clrPort |= clrMask; // high = not clear
	}

	if (o != -1)
	{
		pinMode(o, OUTPUT);
		oePort = portOutputRegister( digitalPinToPort( o ) );
		oeMask = digitalPinToBitMask( o );

		*oePort &= ~oeMask; // low = enabled
	}

	return true;

} // end initialize


/**
 * Sets the value of the channel in the buffer
 *
 * NOTE: must call write to take affect
 */
void HighSideDriver::setValue(uint16_t value)
{
	buffer = value;
}

/**
 * Writes value of buffer to shift register
 *
 */
void HighSideDriver::write(void)
{
	int8_t i, j;

	*latchPort &= ~latchMask; // low

	for(j=(numdrivers-1); j>=0; j--)
	{
		for (i = 7; i >=0 ; i--)  {

			if( ((buffer >> i) & 0x01) == 0x01 )
//			if( buffer & (1 << i) )
			{
				*dataPort |= dataMask;
			}
			else
			{
				*dataPort &= ~dataMask;
			}

			// toggle clock
			*clockPort |= clockMask; // high
			*clockPort &= ~clockMask; // low
		}
	}

	// toggle latch
	*latchPort |= latchMask; // high
	*latchPort &= ~latchMask; // low

} // end write

uint16_t HighSideDriver::getValue()
{
	return buffer;
}

//void HighSideDriver::setAll(uint8_t value)
//{
//	buffer = value;
//	buffer = buffer << 8;
//	buffer |= value;
//}

/**
 * Sets output enable pin
 *
 * true = outputs on
 * false = outputs off
 *
 */
void HighSideDriver::setOutputEnable(boolean b)
{
	if (oePort == 0)
		return;

	if( b )
	{
		*oePort &= ~oeMask; // low = on
	}
	else
	{
		*oePort |= oeMask; // high = off
	}
}

/**
 * Toggles clr pin to clear shift register
 *
 */
void HighSideDriver::clear()
{
	if (clrPort == 0)
		return;

	setValue(0);

	// no need to call write because toggling clr performs the same action
	*clrPort &= ~clrMask; // low = clear
	*clrPort |= clrMask; // high = not clear


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
	if (clrPort == 0)
		return;

	if( b )
	{
		*clrPort &= ~clrMask; // low = on
	}
	else
	{
		*clrPort |= clrMask; // high = off
	}

} // end setClear

