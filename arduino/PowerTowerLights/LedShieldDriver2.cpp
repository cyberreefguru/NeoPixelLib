/*
 * LedShieldDriver2.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: tsasala
 */

#include "LedShieldDriver2.h"

LedShieldDriver2::LedShieldDriver2()
{
	buf1 = 0;
	buf2 = 0;
	rows = 0;
	cols = 0;
	frameBuf = 0;
	driveBuf = 0;
	currentCol = 0;

	hsBuffer = 0;
	hsNumDrivers = 0;
	hsTotalChannels = 0;

	hsClockPort = 0;
	hsDataPort = 0;
	hsLatchPort = 0;
	hsOePort = 0;
	hsClrPort = 0;

	hsClockMask = 0xff;
	hsDataMask = 0xff;
	hsLatchMask = 0xff;
	hsOeMask = 0xff;
	hsClrMask = 0xff;



	lsMaxIntensity = MAX_INTENSITY;
	lsNumDrivers = 0;
	lsTotalChannels = 0;

	lsClockPort = 0;
	lsDataPort = 0;
	lsLatchPort = 0;
	lsOePort = 0;
	lsBlankPort = 0;

	lsClockMask = 0xff;
	lsDataMask = 0xff;
	lsLatchMask = 0xff;
	lsOeMask = 0xff;
	lsBlankMask = 0xff;

}


/**
 * Initializes the shield driver
 *
 * NOTE: Must call initialize, initializeHighSideDriver, and initializeLowSideDriver
 *       for this class to work properly
 */
boolean LedShieldDriver2::initialize(uint8_t r, uint8_t c)
{
	boolean b = false;
	rows = r;
	cols = c;

	buf1 = (uint16_t *) calloc(MAX_BUFFER_SIZE, sizeof(uint16_t));
	if( !buf1 )
	{
		return false;
	}

	buf2 = (uint16_t *) calloc(MAX_BUFFER_SIZE, sizeof(uint16_t));
	if(!buf2)
	{
		return false;
	}

	frameBuf = buf1;
	driveBuf = buf2;

	return true;

} // end initialize



/**
 * Interrupt service routine - cycle columns for driving
 *
 */
void LedShieldDriver2::execInterrupt()
{
	int8_t i, j;
	uint16_t *buf = 0;

	buf = (uint16_t *)&driveBuf[INDEX(0,currentCol)];

	// Write row values

	// ensure latch and clock are low
	PORTD &= 0x7F; // rlatch low
	PORTD &= 0xEF; // rclk low

	PORTB &= 0xEF; // cclk low
	PORTB &= 0xBF; // clatch low

	// 24 channels per TLC5974
	for (int8_t c = lsTotalChannels - 1; c >= 0; c--)
	{
		// 12 bits per channel, send MSB first
		for (int8_t b = 11; b >= 0; b--)
		{
			if( ((buf[c] >> b) & 0x01) == 0x01 )
			{
				PORTC |= 0x40; // rdata high
			}
			else
			{
				PORTC &= 0xBF; // rdata low
			}
			// toggle clock
			PORTD |= 0x10; // rclk high
			PORTD &= 0xEF; // rclk low

		} // end value write

	} // end channel

	// rotate to next column
	hsBuffer = 0x01<<currentCol; // move to next column

	for(j=(hsNumDrivers-1); j>=0; j--)
	{
		for (i = 7; i >=0 ; i--)  {

			if( ((hsBuffer >> i) & 0x01) == 0x01 )
			{
				PORTB |= 0x20; // cdata high
			}
			else
			{
				PORTB &= 0xDF; // cdata low
			}

			// toggle clock
			PORTB |= 0x10; // cclk high
			PORTB &= 0xEF; // cclk low

		}
	}

	// Latch data into ls chip
	PORTE |= 0x40; // rblank high = blank

	PORTB |= 0x80; // coe high = off

	PORTD |= 0x80; // rlatch high
	PORTD &= 0x7F; // rlatch low
	PORTE &= 0xBF; // rblank low = unblank

	// toggle latch
	PORTB |= 0x40; // high
	PORTB &= 0xBF; // low

	PORTB &= 0x7E; // coe low = on



	// increment column value
	currentCol++;
	if( currentCol >= cols )
	{
		currentCol = 0;
	}


}

/**
 * Writes the frame buffer to the drivers
 *
 */
void LedShieldDriver2::write()
{
	// update low side buffer
	noInterrupts(); // turn off interrupts
	if( driveBuf == buf1 )
	{
		frameBuf = buf1;
		driveBuf = buf2;
	}
	else
	{
		frameBuf = buf2;
		driveBuf = buf1;
	}
	interrupts(); // turn on interrupts

	// copy current drive buffer to frame buffer to ensure they are the same
	for(uint16_t i=0; i<MAX_BUFFER_SIZE; i++)
	{
		frameBuf[i] = driveBuf[i];
	}

}

/**
 * Clears all drivers
 *
 * true = clear; disables high and low side drives
 * false = unclear; return last pattern driven
 *
 * NOTE: this function does NOT change the underlying data buffers
 */
void LedShieldDriver2::clear(boolean b)
{
	if( b )
	{
		setHsOutputEnable(false);
		setLsBlank(true);
	}
	else
	{
		setHsOutputEnable(true);
		setLsBlank(false);
	}
}

/**
 * Clears the values driven
 *
 * NOTE: This function clears all underlying data buffers
 */
void LedShieldDriver2::clearAll()
{
	lsClear();
	hsClear();
}

/**
 * Sets the specified value
 *
 * NOTE: this routine does NOT check the row/col values; don't do anything stupid
 */
void LedShieldDriver2::setValue(uint8_t row, uint8_t col, uint16_t value)
{
	frameBuf[INDEX(row,col)] = value;
}

/**
 * Returns the specified value.
 *
 * NOTE: this routine does NOT check the row/col values; don't do anything stupid
 */
uint16_t LedShieldDriver2::getValue(uint8_t row, uint8_t col)
{
	return frameBuf[INDEX(row,col)];
}

/**
 * Sets the entire row to the value.
 *
 * NOTE: this routine does NOT check the row value; don't do anything stupid
 */
void LedShieldDriver2::setRow(uint8_t row, uint16_t value)
{
	for(uint8_t i=0; i<cols; i++)
	{
		frameBuf[INDEX(row,i)]= value;
	}
}

/**
 * Sets the entire column to the value.
 *
 * NOTE: this routine does NOT check the col value; don't do anything stupid
 */
void LedShieldDriver2::setColumn(uint8_t col, uint16_t value)
{
	for(uint8_t i=0; i<rows; i++)
	{
		frameBuf[INDEX(i,col)]= value;
	}
}

uint8_t LedShieldDriver2::getRows()
{
	return rows;
}

uint8_t LedShieldDriver2::getColumns()
{
	return cols;
}


void LedShieldDriver2::setAll(uint16_t value)
{
	for(int8_t j=0; j<cols; j++ )
	{
		for(uint8_t i=0; i<rows; i++)
		{
			frameBuf[INDEX(i,j)]= value;
		}
	}

}








/**
 * Initializes the driver
 *
 */
boolean LedShieldDriver2::initializeHighSideDriver(uint8_t num, uint8_t clk, uint8_t d, uint8_t l, uint8_t c, uint8_t o)
{
	// initialize variables
	hsNumDrivers = num;
	hsTotalChannels = num*HS_CHANNELS_PER_DRIVER;

	// Calculate data port values
	hsClockPort = portOutputRegister( digitalPinToPort( clk ) );
	hsLatchPort = portOutputRegister( digitalPinToPort( l ) );
	hsDataPort = portOutputRegister( digitalPinToPort( d ) );

	// Calculate data mask values
	hsClockMask = digitalPinToBitMask( clk );
	hsLatchMask = digitalPinToBitMask( l );
	hsDataMask = digitalPinToBitMask( d );

	pinMode(clk, OUTPUT);
	pinMode(d, OUTPUT);
	pinMode(l, OUTPUT);

	*hsClockPort &= ~hsClockMask;
	*hsLatchPort &= ~hsLatchMask;
	*hsDataPort &= ~hsDataMask;

	if (c != -1)
	{
		pinMode(c, OUTPUT);
		hsClrPort = portOutputRegister( digitalPinToPort( c ) );
		hsClrMask = digitalPinToBitMask( c );

		*hsClrPort &= ~hsClrMask; // low = clear
		*hsClrPort |= hsClrMask; // high = not clear
	}

	if (o != -1)
	{
		pinMode(o, OUTPUT);
		hsOePort = portOutputRegister( digitalPinToPort( o ) );
		hsOeMask = digitalPinToBitMask( o );

		*hsOePort &= ~hsOeMask; // low = enabled
	}

	return true;

} // end initialize

/**
 * Writes value of buffer to shift register
 *
 */
void LedShieldDriver2::hsWrite(void)
{
	int8_t i, j;

	PORTB &= 0xBF; // low

	for(j=(hsNumDrivers-1); j>=0; j--)
	{
		for (i = 7; i >=0 ; i--)  {

			if( ((hsBuffer >> i) & 0x01) == 0x01 )
			{
				PORTB |= 0x20; // high
			}
			else
			{
				PORTB &= 0xDF; // low
			}

			// toggle clock
			PORTB |= 0x10; // high
			PORTB &= 0xEF; // low

		}
	}

	// toggle latch
	PORTB |= 0x40; // high
	PORTB &= 0xBF; // low


} // end write

/**
 * Sets output enable pin
 *
 * true = outputs on
 * false = outputs off
 *
 */
void LedShieldDriver2::setHsOutputEnable(boolean b)
{
	if (hsOePort == 0)
		return;

	if( b )
	{
		*hsOePort &= ~hsOeMask; // low = on
	}
	else
	{
		*hsOePort |= hsOeMask; // high = off
	}
}

/**
 * Toggles clr pin to clear shift register
 *
 */
void LedShieldDriver2::hsClear()
{
	if (hsClrPort == 0)
		return;

	hsBuffer = 0;

	// no need to call write because toggling clr performs the same action
	*hsClrPort &= ~hsClrMask; // low = clear
	*hsClrPort |= hsClrMask; // high = not clear


} // end clear

/**
 * Clears shift register
 *
 * true = clear
 * false = not clear
 *
 * Use clear() to toggle
 */
void LedShieldDriver2::setHsClear(boolean b)
{
	if (hsClrPort == 0)
		return;

	if( b )
	{
		*hsClrPort &= ~hsClrMask; // low = on
	}
	else
	{
		*hsClrPort |= hsClrMask; // high = off
	}

} // end setClear










/**
 * Initializes the chip.
 *
 * NOTE: must be called before using driver
 */
boolean LedShieldDriver2::initializeLowSideDriver(uint8_t n, uint8_t c, uint8_t d, uint8_t l, uint8_t b, uint8_t o)
{
	// initialize variables
	lsNumDrivers = n;
	lsTotalChannels = LS_CHANNELS_PER_DRIVER*n;

	// Set up pins
	pinMode(c, OUTPUT);
	pinMode(d, OUTPUT);
	pinMode(l, OUTPUT);

	// Calculate data port values
	lsClockPort = portOutputRegister( digitalPinToPort( c ) );
	lsLatchPort = portOutputRegister( digitalPinToPort( l ) );
	lsDataPort = portOutputRegister( digitalPinToPort( d ) );

	// Calculate data mask values
	lsClockMask = digitalPinToBitMask( c );
	lsLatchMask = digitalPinToBitMask( l );
	lsDataMask = digitalPinToBitMask( d );

	*lsClockPort &= ~lsClockMask; // low
	*lsLatchPort &= ~lsLatchMask; // low
	*lsDataPort &= ~lsDataMask; // low

	if( b != -1 )
	{
		pinMode(b, OUTPUT);
		lsBlankPort = portOutputRegister( digitalPinToPort( b ) );
		lsBlankMask = digitalPinToBitMask( b );

		*lsBlankPort |= lsBlankMask; // high = blank
		*lsBlankPort &= ~lsBlankMask; // low = no blank
	}

	if( o != -1 )
	{
		pinMode(o, OUTPUT);
		lsOePort = portOutputRegister( digitalPinToPort( o ) );
		lsOeMask = digitalPinToBitMask( o );

		*lsOePort &= ~lsOeMask; // low = enabled
	}

	return true;

} // end initialize


/**
 * Writes the buffer to the driver.
 *
 * NOTE: Must be called to update outputs with buffer values
 */
void LedShieldDriver2::lsWrite(uint16_t *buf)
{
	// ensure latch and clock are low
	PORTD &= 0x7F; // latch low
	PORTD &= 0xEF; // clk low

	// 24 channels per TLC5974
	for (int8_t c = lsTotalChannels - 1; c >= 0; c--)
	{
		// 12 bits per channel, send MSB first
		for (int8_t b = 11; b >= 0; b--)
		{
			if( ((buf[c] >> b) & 0x01) == 0x01 )
			{
				PORTC |= 0x40;
			}
			else
			{
				PORTC &= 0xBF;
			}
			// toggle clock
			PORTD |= 0x10; // clk high
			PORTD &= 0xEF; // clk low

		} // end value write

	} // end channel

	// Latch data into chip
	PORTE |= 0x40; // blank high
	PORTD |= 0x80; // latch high
	PORTD &= 0x7F; // latch low
	PORTE &= 0xBF; // blank low

} // end write



/**
 * Writes the buffer to the driver.
 *
 * NOTE: Must be called to update outputs with buffer values
 */
void LedShieldDriver2::lsWriteValue(uint16_t value)
{
	// ensure latch and clock are low
	*lsLatchPort &= ~lsLatchMask; // low
	*lsClockPort &= ~lsClockMask; // low

	// 24 channels per TLC5974
	for (int8_t c = lsTotalChannels - 1; c >= 0; c--)
	{
		// 12 bits per channel, send MSB first
		for (int8_t b = 11; b >= 0; b--)
		{
			if (value & (1 << b))
			{
				*lsDataPort |= lsDataMask;
			}
			else
			{
				*lsDataPort &= ~lsDataMask;
			}
			*lsClockPort |= lsClockMask; // clock high
			*lsClockPort &= ~lsClockMask; // clock low

		} // end value write

	} // end channel

	// Latch data into chip
	*lsBlankPort |= lsBlankMask; // high
	*lsLatchPort |= lsLatchMask; // high
	*lsLatchPort &= ~lsLatchMask; // low
	*lsBlankPort &= ~lsBlankMask; // low

} // end write

/**
 * Blanks driver
 *
 * true = output blanked; false = outputs driven
 */
void LedShieldDriver2::setLsBlank(boolean b)
{
	if( lsBlankPort != 0 )
	{
		if( b )
		{
			*lsBlankPort &= ~lsBlankMask; // low
		}
		else
		{
			*lsBlankPort |= lsBlankMask; // high
		}
	}
}

/**
 * Enables outputs.
 *
 * true = enabled; false = disabled
 *
 */
void LedShieldDriver2::setLsOutputEnable(boolean b)
{
	if( lsOePort != 0 )
	{
		if( b )
		{
			*lsOePort &= ~lsOeMask; // low = enabled
		}
		else
		{
			*lsOePort |= lsOeMask; // high = disabled
		}
	}
}

/**
 * Sets all intensity values to 0
 *
 * NOTE: calls write()
 */
void LedShieldDriver2::lsClear()
{
	lsWriteValue(0);
}

