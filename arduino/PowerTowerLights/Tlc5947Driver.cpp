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

#include "Tlc5947Driver.h"

Tlc5947Driver::Tlc5947Driver()
{
}

/**
 * Initializes the chip.
 *
 * NOTE: must be called before using driver
 */
boolean Tlc5947Driver::initialize(uint8_t n, uint8_t c, uint8_t d, uint8_t l, uint8_t b, uint8_t o)
{
	// Allocate buffer to hold intensity values if buf is null
	if( !pwmbuffer)
	{
		// Must allocate 2 bytes per channel = 48 bytes/driver
		pwmbuffer = (uint16_t *) calloc(2, totalChannels);
	}
	if (!pwmbuffer)
	{
		return false;
	}

	maxIntensity = MAX_INTENSITY;
	totalChannels = CHANNELS_PER_DRIVER*n;

	// Set up pins
	pinMode(c, OUTPUT);
	pinMode(d, OUTPUT);
	pinMode(l, OUTPUT);

	digitalWrite(c, LOW);
	digitalWrite(d, LOW);
	digitalWrite(l, LOW);

	if( b != -1 )
	{
		pinMode(b, OUTPUT);
		digitalWrite(b, LOW);
		blankPort = portOutputRegister( digitalPinToPort( b ) );
		blankMask = digitalPinToBitMask( b );
	}
	if( o != -1 )
	{
		pinMode(o, OUTPUT);
		digitalWrite(o, HIGH);
		oePort = portOutputRegister( digitalPinToPort( o ) );
		oeMask = digitalPinToBitMask( o );
	}

	// Calculate data port values
	clockPort = portOutputRegister( digitalPinToPort( c ) );
	latchPort = portOutputRegister( digitalPinToPort( l ) );
	dataPort = portOutputRegister( digitalPinToPort( d ) );

	// Calculate data mask values
	clockMask = digitalPinToBitMask( c );
	latchMask = digitalPinToBitMask( l );
	dataMask = digitalPinToBitMask( d );

//	Serial.print("maxIntensity: ");
//	Serial.println( maxIntensity );
//
//	Serial.print("totalChannels: ");
//	Serial.println( totalChannels );

	return true;

} // end initialize

/**
 * Writes the buffer to the driver.
 *
 * NOTE: Must be called to update outputs with buffer values
 */
void Tlc5947Driver::write(void)
{
	// ensure latch and clock are low
	*latchPort &= ~latchMask; // low
	*clockPort &= ~clockMask; // low

	// 24 channels per TLC5974
	for (int8_t c = totalChannels - 1; c >= 0; c--)
	{
		// 12 bits per channel, send MSB first
		for (int8_t b = 11; b >= 0; b--)
		{
			if (pwmbuffer[c] & (1 << b))
			{
				*dataPort |= dataMask;
			}
			else
			{
				*dataPort &= ~dataMask;
			}
			*clockPort |= clockMask; // clock high
			*clockPort &= ~clockMask; // clock low

		} // end value write

	} // end channel

	// Latch data into chip
	*blankPort |= blankMask; // high
	*latchPort |= latchMask; // high
	*latchPort &= ~latchMask; // low
	*blankPort &= ~blankMask; // low


} // end write

/**
 * Sets the intensity of a specific channel in the buffer
 *
 * NOTE: must call write() to driver output
 *
 * @chan - pwm channel to set intensity
 * @pwm - intensity value to set
 */
void Tlc5947Driver::setIntensity(uint8_t chan, uint16_t value)
{
	if (value > maxIntensity)
	{
		value = maxIntensity;
	}

	if (chan > totalChannels)
	{
		return;
	}

	pwmbuffer[chan] = value;

} // end setIntensity

/**
 * Blanks driver
 *
 * true = output blanked; false = outputs driven
 */
void Tlc5947Driver::setBlank(boolean b)
{
	if( blank != -1 )
	{
		digitalWrite( blank, b ); // blank TLC
	}
}

/**
 * Enables outputs.
 *
 * true = enabled; false = disabled
 *
 */
void Tlc5947Driver::setOutputEnable(boolean b)
{
	if( oe != -1 )
	{
		digitalWrite( oe, !b ); // oe is active low
	}
}

/**
 * Sets all intensity values to 0
 */
void Tlc5947Driver::clear()
{
	setAll( 0 );
}

/**
 * Sets all intensity values to 0
 */
void Tlc5947Driver::setAll(uint16_t value)
{
	// Clear all channels
	for(uint8_t i=0; i<totalChannels; i++)
	{
		pwmbuffer[i] = value;
	}
	write();

}
