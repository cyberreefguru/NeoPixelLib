/*
 * LedShieldDriver.h
 *
 *  Created on: Dec 8, 2013
 *      Author: tsasala
 */

#ifndef LEDSHIELDDRIVER_H_
#define LEDSHIELDDRIVER_H_

#include <Arduino.h>
#include "Tlc5947Driver.h"
#include "HighSideDriver.h"
#include "FlexiTimer2.h";

//#define __DEBUG

#define MAX_ROWS			24
#define MAX_COLS			8
#define MAX_BUFFER_SIZE		MAX_ROWS*MAX_COLS

#define INDEX(row,col) (row+col*MAX_ROWS)


extern int freeRam();


class LedShieldDriver
{
public:
	LedShieldDriver();
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
	uint8_t currentCol;

};

#endif /* LEDSHIELDDRIVER_H_ */
