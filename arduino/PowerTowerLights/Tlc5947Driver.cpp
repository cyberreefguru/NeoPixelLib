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

Tlc5947Driver::Tlc5947Driver(uint8_t n, uint8_t c, uint8_t d, uint8_t l)
{
	Tlc5947Driver(n, c, d, l, -1, -1);
}

Tlc5947Driver::Tlc5947Driver(uint8_t n, uint8_t c, uint8_t d, uint8_t l, uint8_t b, uint8_t o)
{
	numdrivers = n;
	clk = c;
	dat = d;
	lat = l;
	blank = b;
	oe = o;

	pwmbuffer = 0;
	maxIntensity = MAX_INTENSITY;
	totalChannels = CHANNELS_PER_DRIVER*n;

}

/**
 * Initializes the chip.
 *
 * NOTE: must be called before using driver
 */
boolean Tlc5947Driver::initialize()
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

	// Set up pins
	pinMode(clk, OUTPUT);
	pinMode(dat, OUTPUT);
	pinMode(lat, OUTPUT);

	digitalWrite(clk, LOW);
	digitalWrite(dat, LOW);
	digitalWrite(lat, LOW);

	if( blank != -1 )
	{
		pinMode(blank, OUTPUT);
		digitalWrite(blank, LOW);
	}
	if( oe != -1 )
	{
		pinMode(oe, OUTPUT);
		digitalWrite(oe, HIGH);
	}

	Serial.print("maxIntensity: ");
	Serial.println( maxIntensity );

	Serial.print("totalChannels: ");
	Serial.println( totalChannels );


	return true;

} // end initialize

/**
 * Writes the buffer to the driver.
 *
 * NOTE: Must be called to update outputs with buffer values
 */
void Tlc5947Driver::write(void)
{
	// ensure latch is low
	digitalWrite(lat, LOW);

	// 24 channels per TLC5974
	for (int8_t c = totalChannels - 1; c >= 0; c--)
	{
		// 12 bits per channel, send MSB first
		for (int8_t b = 11; b >= 0; b--)
		{
			digitalWrite(clk, LOW);

			if (pwmbuffer[c] & (1 << b))
			{
				digitalWrite(dat, HIGH);
			}
			else
			{
				digitalWrite(dat, LOW);
			}

			digitalWrite(clk, HIGH);

		} // end value write
	} // end channel

	// Set clock low
	digitalWrite(clk, LOW);

	// Latch data into chip
	digitalWrite(lat, HIGH);
	digitalWrite(lat, LOW);

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
