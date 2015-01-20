/*
 * LedShieldDriver2.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: tsasala
 */

#include "LedShieldDriverScaled.h"

LedShieldDriverScaled::LedShieldDriverScaled()
{
	buf1 = 0;
	buf2 = 0;
	rows = 0;
	cols = 0;
	rowDrivers = 0;
	colDrivers = 0;

	frameBuf = 0;
	driveBuf = 0;
	currentCol = 0;

	hsBuffer = 0;
	colMask = 0;
	colDrive = 0;
}


/**
 * Initializes the shield driver
 *
 * NOTE: Must call initialize, initializeHighSideDriver, and initializeLowSideDriver
 *       for this class to work properly
 */
boolean LedShieldDriverScaled::initialize(uint8_t r, uint8_t c)
{
	boolean b = false;
	rows = r;
	cols = c;

	rowDrivers = r/ROWS_PER_DRIVER;
	colDrivers = c/COLS_PER_DRIVER;

	// Initialize high side variables (595)
	pinMode(COL_CLOCK, OUTPUT);
	pinMode(COL_DATA, OUTPUT);
	pinMode(COL_LATCH, OUTPUT);
	pinMode(COL_OE, OUTPUT);

	COL_CLOCK_LOW;
	COL_DATA_LOW;
	COL_LATCH_LOW;
	COL_OUTPUT_DISABLED;
	COL_CLEAR_DISABLED;

	// Initialize low side variables (TLC)
	pinMode(ROW_CLOCK, OUTPUT);
	pinMode(ROW_DATA, OUTPUT);
	pinMode(ROW_LATCH, OUTPUT);
	pinMode(ROW_CLEAR, OUTPUT);

	ROW_CLOCK_LOW;
	ROW_DATA_LOW;
	ROW_LATCH_LOW;
	ROW_CLEAR_ENABLED;

	// Reserve memory
	buf1 = (INTENSITY_TYPE *) calloc(MAX_BUFFER_SIZE, sizeof(INTENSITY_TYPE));
	if( !buf1 )
	{
		return false;
	}

	buf2 = (INTENSITY_TYPE *) calloc(MAX_BUFFER_SIZE, sizeof(INTENSITY_TYPE));
	if(!buf2)
	{
		return false;
	}

	frameBuf = buf1;
	driveBuf = buf2;

	// If # cols 1-8, set drive to 7
	if( c <= (COLS_PER_DRIVER) )
	{
		colDrive = 7;
		colMask = 0x0001;
	}
	else if( c <= (COLS_PER_DRIVER*2) )
	{
		colDrive = 15;
		colMask = 0x0001;
	}
	else
	{
		return false;
	}

	return true;

} // end initialize


/**
 * Interrupt service routine - cycle columns for driving
 *
 */
void LedShieldDriverScaled::execInterrupt()
{
	int8_t i, j;
	INTENSITY_TYPE *buf = 0;

	// set buffer pointer to drive buffer, first row, current column
	buf = (INTENSITY_TYPE *)&driveBuf[INDEX(0,currentCol)];

	// ensure latch and clock are low
	ROW_LATCH_LOW;
	ROW_CLOCK_LOW;
	COL_CLOCK_LOW;
	COL_LATCH_LOW;

	// 24 channels per TLC5974
	for (int8_t c = rows - 1; c >= 0; c--)
	{
		// 12 bits per channel, send MSB first
		for (int8_t b = 11; b >= 0; b--)
		{
			if( ((buf[c] >> b) & 0x01) == 0x01 )
			{
				ROW_DATA_HIGH;
			}
			else
			{
				ROW_DATA_LOW;
			}
			// toggle clock
			ROW_CLOCK_HIGH;
			ROW_CLOCK_LOW;

		} // end row value write

	} // end row write

	// rotate to next column; shifts colMask currentCol times (e.g., 0x0001 x 16 for full 16 columns)
	hsBuffer = (colMask)<<currentCol;

	for (i = colDrive; i >=0 ; i--)
	{
		if( ((hsBuffer >> i) & 0x01) == 0x01 )
		{
			COL_DATA_HIGH;
		}
		else
		{
			COL_DATA_LOW;
		}

		// toggle clock
		COL_CLOCK_HIGH;
		COL_CLOCK_LOW;
	}

	// Clear row and column
	ROW_CLEAR_ENABLED;

	// TODO: test if COL_CLEAR work here
	COL_OUTPUT_DISABLED;

	// Latch data into row
	ROW_LATCH_HIGH;
	ROW_LATCH_LOW;
	ROW_CLEAR_DISABLED;

	// Latch data into col
	COL_LATCH_HIGH;
	COL_LATCH_LOW;
	// TODO: test if COL_CLEAR work here
	COL_OUTPUT_ENABLED;

	// increment column value
	currentCol++;
	if( currentCol >= cols )
	{
		currentCol = 0;
	}

} // end interrupt

/**
 * Writes the frame buffer to the drivers
 *
 */
void LedShieldDriverScaled::write()
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
 * Blanks the display by clearing row driver
 *
 * NOTE: Does not change the underlying data buffers
 */
void LedShieldDriverScaled::blank()
{
	ROW_CLEAR_ENABLED;
	ROW_CLEAR_DISABLED;
}

/**
 * Clears the display by clearing memory and writing new values
 */
void LedShieldDriverScaled::clear()
{
	setAll(0, true);
}

/**
 * Sets the specified value
 *
 * @index the index to change
 * @value the value
 *
 * NOTE: this routine does NOT check the row/col values; don't do anything stupid
 */
void LedShieldDriverScaled::setIndexedValue(uint16_t index, INTENSITY_TYPE value)
{
	frameBuf[index] = value;
}

/**
 * Sets the specified value
 *
 * NOTE: this routine does NOT check the row/col values; don't do anything stupid
 *
 * @row row index
 * @col column index
 * @value intensity value for (r,c)
 *
 */
void LedShieldDriverScaled::setValue(uint8_t row, uint8_t col, INTENSITY_TYPE value)
{
	frameBuf[INDEX(row,col)] = value;
}

/**
 * Returns the specified value.
 *
 * NOTE: this routine does NOT check the row/col values; don't do anything stupid
 *
 * @row row index
 * @col column index
 * @return value of (r,c)
 */
INTENSITY_TYPE LedShieldDriverScaled::getValue(uint8_t row, uint8_t col)
{
	return frameBuf[INDEX(row,col)];
}

/**
 * Sets the entire row to the value.
 *
 * NOTE: this routine does NOT check the row value; don't do anything stupid
 *
 * @row row index
 * @value intensity value
 */
void LedShieldDriverScaled::setRow(uint8_t row, INTENSITY_TYPE value)
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
 *
 * @col column index
 * @value intensity value
 */
void LedShieldDriverScaled::setColumn(uint8_t col, INTENSITY_TYPE value)
{
	for(uint8_t i=0; i<rows; i++)
	{
		frameBuf[INDEX(i,col)]= value;
	}
}

/**
 * returns the number of rows the driver was initialized with
 */
uint8_t LedShieldDriverScaled::getRows()
{
	return rows;
}

/**
 * returns the number of columns the driver was initialized with
 */
uint8_t LedShieldDriverScaled::getColumns()
{
	return cols;
}

/**
 * Sets all to value.
 *
 * @value intensity value
 * @w boolean - true=write now; false=no write
 */
void LedShieldDriverScaled::setAll(INTENSITY_TYPE value, uint8_t w)
{
	for(int8_t j=0; j<cols; j++ )
	{
		for(uint8_t i=0; i<rows; i++)
		{
			frameBuf[INDEX(i,j)]= value;
		}
	}
	if(w)
	{
		write();
	}

}

/**
 * rotates teh values in the buffer
 *
 * @direction DIRECTION_LEFT or DIRECTION_RIGHT
 * @rotateAmount number of positions to rotate
 */
void LedShieldDriverScaled::rotate(uint8_t direction, uint8_t rotateAmount)
{
	uint16_t i;
	uint8_t j, k;

	if( direction == DIRECTION_LEFT)
	{
		for(k=0; k<rotateAmount; k++)
		{
			j = frameBuf[0];
			for (i = 0; i < (MAX_BUFFER_SIZE-1); i++)
			{
				frameBuf[i] = frameBuf[i+i];
			}
			frameBuf[i] = j;
		}
	}
	else if( direction == DIRECTION_RIGHT)
	{
		//TODO right code for rotate right
	}
}

/**
 * Rotates an entire row
 *
 * @direction DIRECTION_LEFT or DIRECTION_RIGHT
 * @rotateAmount number of positions to rotate
 */
void LedShieldDriverScaled::rotateRow(uint8_t direction, uint8_t rotateAmount)
{
	int8_t z;
	uint8_t i, j, k;

	if( direction == DIRECTION_LEFT)
	{
		for(k=0; k<rotateAmount; k++)
		{
			j = getValue(0, 0);
			for (i = 0; i < (rows-1); i++)
			{
				setRow(i, getValue(i+1,0) );
			}
			setRow(i, j);
		}
	}
	else if( direction == DIRECTION_RIGHT)
	{
		for(k=0; k<rotateAmount; k++)
		{
			j = getValue((rows-1), 0);
			for(z=(rows-1); z>= 0; z--)
			{
				setRow(z, getValue(z-1,0) );
			}
			setRow(z, j);
		}
	}
}

/**
 * randomly sets a pixel to the specified intensity
 */
void LedShieldDriverScaled::randomize(uint8_t rows, uint8_t cols, uint8_t on, uint16_t delayTime, INTENSITY_TYPE brightness)
{
	uint16_t i,j, q, total;

	if( on == true )
	{
		// set all off
		setAll(0, false);
	}
	else
	{
		// set all on
		setAll(MAX_INTENSITY, false);
	}

	total = rows*cols;
	while( total > 0 )
	{
		q = random(0, rows*cols); // don't use 'total' here since it changes in the loop
		i = q/rows; // column
		j = q%rows; // row

		if( on == true )
		{
			if( getValue(j, i) != brightness )
			{
				setValue(j, i, brightness);
				write();
				total--;
				delay(delayTime);
			}
		}
		else
		{
			if( getValue(j, i) != 0 )
			{
				setValue(j, i, 0);
				write();
				total--;
				delay(delayTime);
			}
		}

	} // end while

} // end random
