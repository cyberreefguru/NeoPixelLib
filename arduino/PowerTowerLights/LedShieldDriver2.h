/*
 * LedShieldDriver2.h
 *
 *  Created on: Jan 27, 2014
 *      Author: tsasala
 */

#ifndef LEDSHIELDDRIVER2_H_
#define LEDSHIELDDRIVER2_H_


#include <Arduino.h>


#define MAX_ROWS				24
#define MAX_COLS				8
#define MAX_BUFFER_SIZE			MAX_ROWS*MAX_COLS

#define HS_CHANNELS_PER_DRIVER 	8
#define LS_CHANNELS_PER_DRIVER 	24
#define MAX_INTENSITY 			4095

#define INDEX(row,col) (row+col*MAX_ROWS)


class LedShieldDriver2
{
public:
	LedShieldDriver2();

	boolean initialize(uint8_t rows, uint8_t cols);
	boolean initializeHighSideDriver(uint8_t num, uint8_t clk, uint8_t data, uint8_t lat, uint8_t clr, uint8_t oe);
	boolean initializeLowSideDriver(uint8_t num, uint8_t clk, uint8_t data, uint8_t lat, uint8_t clr, uint8_t oe);

	void write();
	void clear(boolean b);
	void clearAll();

	uint8_t getRows();
	uint8_t getColumns();

	void setValue(uint8_t row, uint8_t col, uint16_t value);
	uint16_t getValue(uint8_t row, uint8_t col);

	void setRow(uint8_t row, uint16_t value);
	void setColumn(uint8_t col, uint16_t value);
	void setAll(uint16_t value);

	void execInterrupt();


private:

	// Use single dimension arrays to make compilation easier
	// access by col + 8*row (or row + 8*col)
	uint16_t *buf1, *buf2;
	uint16_t *frameBuf, *driveBuf;
	uint8_t rows, cols;
	volatile uint8_t currentCol;

	// ls methods
	void setLsBlank(boolean b);
	void setLsOutputEnable(boolean b);
	void lsWriteValue(uint16_t value);
	void lsWrite(uint16_t *buffer);
	void lsClear();


	// low side variables
	uint8_t lsNumDrivers;
	uint16_t lsMaxIntensity, lsTotalChannels;

	volatile uint8_t *lsClockPort, *lsDataPort, *lsLatchPort, *lsOePort, *lsBlankPort;
	uint8_t lsClockMask, lsDataMask, lsLatchMask, lsOeMask, lsBlankMask;

	// hs methods
	void hsWrite(void);
	void hsClear(void);
	void setHsClear(boolean b);
	void setHsOutputEnable(boolean b);


	// High side variables
	uint16_t hsBuffer;
	uint8_t hsNumDrivers, hsTotalChannels;

	volatile uint8_t *hsClockPort, *hsDataPort, *hsLatchPort, *hsOePort, *hsClrPort;
	uint8_t hsClockMask, hsDataMask, hsLatchMask, hsOeMask, hsClrMask;

};

#endif /* LEDSHIELDDRIVER2_H_ */
