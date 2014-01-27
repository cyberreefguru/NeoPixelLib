/*
 * LedShieldDriver.cpp
 *
 *  Created on: Dec 8, 2013
 *      Author: tsasala
 */

#include "LedShieldDriver.h"

Tlc5947Driver lsd = Tlc5947Driver();
HighSideDriver hsd = HighSideDriver();

/**
 * Constructor
 */
LedShieldDriver::LedShieldDriver()
{
	buf1 = 0;
	buf2 = 0;
	rows = 0;
	cols = 0;
	frameBuf = 0;
	driveBuf = 0;
	currentCol = 0;
}

/**
 * Initializes the shield driver
 *
 * NOTE: Must call initialize, initializeHighSideDriver, and initializeLowSideDriver
 *       for this class to work properly
 */
boolean LedShieldDriver::initialize(uint8_t r, uint8_t c)
{
	boolean b = false;
	rows = r;
	cols = c;

	buf1 = (uint16_t *) calloc(MAX_BUFFER_SIZE, sizeof(uint16_t));
	if( !buf1 )
	{
#ifdef __DEBUG
		Serial.print("ERROR: buf1 allocation failed: ");
#endif
		return false;
	}

	buf2 = (uint16_t *) calloc(MAX_BUFFER_SIZE, sizeof(uint16_t));
	if(!buf2)
	{
#ifdef __DEBUG
		Serial.print("ERROR: buf2 allocation failed: ");
#endif
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
void LedShieldDriver::execInterrupt()
{
	// Clear columns
//	hsd.setValue(0);
//	hsd.write();

	// Write row values
	lsd.write((uint16_t *)&driveBuf[INDEX(0,currentCol)]);

	// rotate to next column
	hsd.setValue(0x01<<currentCol); // move to next column
	hsd.write();

	// increment column value
	currentCol++;
	if( currentCol >= cols )
	{
		currentCol = 0;
	}

// Clear timer 4 count
//	TCNT4 = 0;

}




/**
 * Initializes the high side driver
 *
 * NOTE: Must call initialize, initializeHighSideDriver, and initializeLowSideDriver
 *       for this class to work properly
 */
boolean LedShieldDriver::initializeHighSideDriver(uint8_t num, uint8_t clk,
		uint8_t data, uint8_t lat, uint8_t clr, uint8_t oe)
{
	boolean b = hsd.initialize( num, clk, data, lat, clr, oe);

	if(b)
	{
#ifdef __DEBUG
	Serial.println("Clearing columns");
#endif
		hsd.setValue(0x00); // turn off all columns
		hsd.write();
	}
	else
	{
#ifdef __DEBUG
	Serial.println("Error initializing");
#endif
	}

	return b;
}

/**
 * Initializes the low side driver
 *
 * NOTE: Must call initialize, initializeHighSideDriver, and initializeLowSideDriver
 *       for this class to work properly
 */
boolean LedShieldDriver::initializeLowSideDriver(uint8_t num, uint8_t clk,
		uint8_t data, uint8_t lat, uint8_t clr, uint8_t oe)
{
	boolean b = lsd.initialize( num, clk, data, lat, clr, oe);

	// TODO where should turning on row drivers be done? ISR?
	if(b)
	{

#ifdef __DEBUG
		Serial.println("Clearing TLC");
#endif
//		lsd.setBlank(true); // blank
//		lsd.clear(); // clear buffer
//		lsd.setBlank(false); // "unblank"
	}
	else
	{
#ifdef __DEBUG
		Serial.println("Error initializing TLC");
#endif

	}
	return b;
}

/**
 * Writes the frame buffer to the drivers
 *
 */
void LedShieldDriver::write()
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
void LedShieldDriver::clear(boolean b)
{
	if( b )
	{
		hsd.setOutputEnable(false);
		lsd.setBlank(true);
	}
	else
	{
		hsd.setOutputEnable(true);
		lsd.setBlank(false);
	}
}

/**
 * Clears the values driven
 *
 * NOTE: This function clears all underlying data buffers
 */
void LedShieldDriver::clearAll()
{
	lsd.clear();
	hsd.clear();
}

/**
 * Sets the specified value
 *
 * NOTE: this routine does NOT check the row/col values; don't do anything stupid
 */
void LedShieldDriver::setValue(uint8_t row, uint8_t col, uint16_t value)
{
	frameBuf[INDEX(row,col)] = value;
}

/**
 * Returns the specified value.
 *
 * NOTE: this routine does NOT check the row/col values; don't do anything stupid
 */
uint16_t LedShieldDriver::getValue(uint8_t row, uint8_t col)
{
	return frameBuf[INDEX(row,col)];
}

/**
 * Sets the entire row to the value.
 *
 * NOTE: this routine does NOT check the row value; don't do anything stupid
 */
void LedShieldDriver::setRow(uint8_t row, uint16_t value)
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
void LedShieldDriver::setColumn(uint8_t col, uint16_t value)
{
	for(uint8_t i=0; i<rows; i++)
	{
		frameBuf[INDEX(i,col)]= value;
	}
}

uint8_t LedShieldDriver::getRows()
{
	return rows;
}

uint8_t LedShieldDriver::getColumns()
{
	return cols;
}

//Tlc5947Driver LedShieldDriver::getLowSideDriver()
//{
//	return lsd;
//}
//
//HighSideDriver LedShieldDriver::getHighSideDriver()
//{
//	return hsd;
//}

void LedShieldDriver::setAll(uint16_t value)
{
	for(int8_t j=0; j<cols; j++ )
	{
		for(uint8_t i=0; i<rows; i++)
		{
			frameBuf[INDEX(i,j)]= value;
		}
	}

}
