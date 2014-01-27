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

/**
 * Constructor
 */
Tlc5947Driver::Tlc5947Driver()
{
//	buffer = 0;
	maxIntensity = MAX_INTENSITY;
	numdrivers = 0;
	totalChannels = 0;

	clockPort = 0;
	dataPort = 0;
	latchPort = 0;
	oePort = 0;
	blankPort = 0;

	clockMask = 0xff;
	dataMask = 0xff;
	latchMask = 0xff;
	oeMask = 0xff;
	blankMask = 0xff;
}

/**
 * Initializes the chip.
 *
 * NOTE: must be called before using driver
 */
boolean Tlc5947Driver::initialize(uint8_t n, uint8_t c, uint8_t d, uint8_t l, uint8_t b, uint8_t o)
{
#ifdef __DEBUG
	Serial.println("tlc.initialize: BEGIN");
#endif

	// initialize variables
	numdrivers = n;
	totalChannels = CHANNELS_PER_DRIVER*n;

//	// TODO we can point this to the driveBuf rather than using more memory here. It will avoid the copy too.
//	// Allocate buffer to hold intensity values if buf is null
//	if( !buffer)
//	{
//		// Must allocate 2 bytes per channel = 48 bytes/driver
//		buffer = (uint16_t *) calloc(totalChannels, sizeof(uint16_t));
//	}
//
//	if (!buffer)
//	{
//		return false;
//	}

	// Set up pins
	pinMode(c, OUTPUT);
	pinMode(d, OUTPUT);
	pinMode(l, OUTPUT);

	// Calculate data port values
	clockPort = portOutputRegister( digitalPinToPort( c ) );
	latchPort = portOutputRegister( digitalPinToPort( l ) );
	dataPort = portOutputRegister( digitalPinToPort( d ) );

	// Calculate data mask values
	clockMask = digitalPinToBitMask( c );
	latchMask = digitalPinToBitMask( l );
	dataMask = digitalPinToBitMask( d );

	*clockPort &= ~clockMask; // low
	*latchPort &= ~latchMask; // low
	*dataPort &= ~dataMask; // low

	if( b != -1 )
	{
		pinMode(b, OUTPUT);
		blankPort = portOutputRegister( digitalPinToPort( b ) );
		blankMask = digitalPinToBitMask( b );

		*blankPort |= blankMask; // high = blank
		*blankPort &= ~blankMask; // low = no blank
	}

	if( o != -1 )
	{
		pinMode(o, OUTPUT);
		oePort = portOutputRegister( digitalPinToPort( o ) );
		oeMask = digitalPinToBitMask( o );

		*oePort &= ~oeMask; // low = enabled
	}

#ifdef __DEBUG
	Serial.print("maxIntensity: ");
	Serial.println( maxIntensity );

	Serial.print("totalChannels: ");
	Serial.println( totalChannels );
#endif

#ifdef __DEBUG
	Serial.println("tlc.initialize: END");
#endif

	return true;

} // end initialize


/**
 * Writes the buffer to the driver.
 *
 * NOTE: Must be called to update outputs with buffer values
 */
void Tlc5947Driver::write(uint16_t *buf)
{
#ifdef __DEBUG
	Serial.println("write: BEGIN");
#endif

	// ensure latch and clock are low
//	*latchPort &= ~latchMask; // low
//	*clockPort &= ~clockMask; // low

	PORTD &= 0x7F; // latch low
	PORTD &= 0xEF; // clk low

	// 24 channels per TLC5974
	for (int8_t c = totalChannels - 1; c >= 0; c--)
	{
		// 12 bits per channel, send MSB first
		for (int8_t b = 11; b >= 0; b--)
		{
			if( ((buf[c] >> b) & 0x01) == 0x01 )
//			if (buf[c] & (1 << b))
			{
				PORTC |= 0x40;
//				*dataPort |= dataMask;
			}
			else
			{
				PORTC &= 0xBF;
//				*dataPort &= ~dataMask;
			}
//			*clockPort |= clockMask; // clock high
//			*clockPort &= ~clockMask; // clock low
			PORTD |= 0x10; // clk high
			PORTD &= 0xEF; // clk low

		} // end value write

	} // end channel

	// Latch data into chip
//	*blankPort |= blankMask; // high
//	*latchPort |= latchMask; // high
//	*latchPort &= ~latchMask; // low
//	*blankPort &= ~blankMask; // low

//	PORTB &= 0x7F; // clear col
//	PORTB |= 0x80; //

	PORTE |= 0x40; // blank high
	PORTD |= 0x80; // latch high
	PORTD &= 0x7F; // latch low
	PORTE &= 0xBF; // blank low



#ifdef __DEBUG
	Serial.println("write: COMPLETE");
#endif

} // end write



/**
 * Writes the buffer to the driver.
 *
 * NOTE: Must be called to update outputs with buffer values
 */
void Tlc5947Driver::writeValue(uint16_t value)
{
#ifdef __DEBUG
	Serial.println("write: BEGIN");
#endif

	// ensure latch and clock are low
	*latchPort &= ~latchMask; // low
	*clockPort &= ~clockMask; // low

	// 24 channels per TLC5974
	for (int8_t c = totalChannels - 1; c >= 0; c--)
	{
		// 12 bits per channel, send MSB first
		for (int8_t b = 11; b >= 0; b--)
		{
			if (value & (1 << b))
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

#ifdef __DEBUG
	Serial.println("write: COMPLETE");
#endif

} // end write

///**
// * Sets the intensity of a specific channel in the buffer
// *
// * NOTE: must call write() to driver output
// *
// * @chan - pwm channel to set intensity
// * @pwm - intensity value to set
// */
//void Tlc5947Driver::setIntensity(uint8_t chan, uint16_t value)
//{
//	if (value > maxIntensity)
//	{
//		value = maxIntensity;
//	}
//
//	if (chan > totalChannels)
//	{
//		return;
//	}
//	buffer[chan] = value;
//
//#ifdef __DEBUG
//	Serial.print("Intensity ");
//	Serial.print( chan );
//	Serial.print( ":");
//	Serial.println( buffer[chan]);
//#endif
//
//} // end setIntensity

/**
 * Blanks driver
 *
 * true = output blanked; false = outputs driven
 */
void Tlc5947Driver::setBlank(boolean b)
{
	if( blankPort != 0 )
	{
		if( b )
		{
			*blankPort &= ~blankMask; // low
		}
		else
		{
			*blankPort |= blankMask; // high
		}
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
	if( oePort != 0 )
	{
		if( b )
		{
			*oePort &= ~oeMask; // low = enabled
		}
		else
		{
			*oePort |= oeMask; // high = disabled
		}
	}
}

/**
 * Sets all intensity values to 0
 *
 * NOTE: calls write()
 */
void Tlc5947Driver::clear()
{
#ifdef __DEBUG
	Serial.println("clear: BEGIN");
#endif

//	setAll( 0 );
	writeValue(0);

#ifdef __DEBUG
	Serial.println("clear: END");
#endif
}

///**
// * Sets all intensity values to 0
// *
// * NOTE: does not write the values
// */
//void Tlc5947Driver::setAll(uint16_t value)
//{
//#ifdef __DEBUG
//	Serial.println("setAll: BEGIN");
//#endif
//
//	// Clear all channels
//
//#ifdef __DEBUG
//	Serial.print("v=");
//	Serial.println(value);
//#endif
//
//	for(uint8_t i=0; i<24; i++)
//	{
//		buffer[i] = value;
//	}
//
//#ifdef __DEBUG
//	Serial.println("setAll: END");
//#endif
//
//}

void Tlc5947Driver::printValues()
{
#ifdef __DEBUG
	Serial.println("printValues:BEGIN");
#endif

	for(uint8_t i=0; i<24; i++)
	{
		Serial.print("buffer[");
		Serial.print( i );
		Serial.print( "]=");
//		Serial.print( buffer[i]);
	}

#ifdef __DEBUG
	Serial.println("printValues:END");
#endif
}
