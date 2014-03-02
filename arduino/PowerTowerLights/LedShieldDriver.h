/*
 * LedShieldDriver.h
 *
 *  Created on: Jan 27, 2014
 *      Author: tsasala
 */

#ifndef LEDSHIELDDRIVER_H_
#define LEDSHIELDDRIVER_H_


#include <Arduino.h>


#define MAX_ROWS				24
#define MAX_COLS				8
#define MAX_BUFFER_SIZE			MAX_ROWS*MAX_COLS

#define COLS_PER_DRIVER 		8
#define ROWS_PER_DRIVER 		24
#define MAX_INTENSITY 			4095

#define INDEX(row,col) (row+col*MAX_ROWS)

// VERSION 1.0
// ROW_CLK = D4		COL_CLK = D8
// ROW_DATA = D5	COL_DATA = D9
// ROW_LATCH = D6	COL_LATCH = D10
// ROW_CLEAR = D7	COL_CLEAR = D11

#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328P__) // Uno

#define ROW_CLOCK	4
#define ROW_DATA	5
#define ROW_LATCH	6
#define ROW_OE  	-1
#define ROW_CLEAR 	7

#define COL_CLOCK 	8
#define COL_DATA	9
#define COL_LATCH	10
#define COL_CLEAR	-1
#define COL_OE		11

#define ROW_CLOCK_HIGH			PORTD |= _BV(4)
#define ROW_CLOCK_LOW			PORTD &= ~_BV(4)
#define ROW_DATA_HIGH			PORTD |= _BV(5)
#define ROW_DATA_LOW			PORTD &= ~_BV(5)
#define ROW_LATCH_HIGH			PORTD |= _BV(6)
#define ROW_LATCH_LOW			PORTD &= ~_BV(6)
#define ROW_CLEAR_ENABLED		PORTD |= _BV(7)
#define ROW_CLEAR_DISABLED		PORTD &= ~_BV(7)

#define COL_CLOCK_HIGH			PORTB |= _BV(0)
#define COL_CLOCK_LOW			PORTB &= ~_BV(0)
#define COL_DATA_HIGH			PORTB |= _BV(1)
#define COL_DATA_LOW			PORTB &= ~_BV(1)
#define COL_LATCH_HIGH			PORTB |= _BV(2)
#define COL_LATCH_LOW			PORTB &= ~_BV(2)
#define COL_CLEAR_ENABLED		nop
#define COL_CLEAR_DISABLED		nop
#define COL_OUTPUT_ENABLED		PORTB &= !_BV(3) // active low
#define COL_OUTPUT_DISABLED		PORTB |= _BV(3) // active low

#elif defined (__AVR_ATmega32U4__) // Leonardo

// ROW_CLK D4, ROW_DATA D5, ROW_LATCH D6, ROW_CLEAR D7
#define ROW_CLOCK	4
#define ROW_DATA	5
#define ROW_LATCH	6
#define ROW_OE  	-1
#define ROW_CLEAR 	7

#define COL_CLOCK 	8
#define COL_DATA	9
#define COL_LATCH	10
#define COL_CLEAR	-1
#define COL_OE		11

#define ROW_CLOCK_HIGH		PORTD |= _BV(4)
#define ROW_CLOCK_LOW		PORTD &= ~_BV(4)
#define ROW_DATA_HIGH		PORTC |= _BV(6)
#define ROW_DATA_LOW		PORTC &= ~_BV(6)
#define ROW_LATCH_HIGH		PORTD |= _BV(7)
#define ROW_LATCH_LOW		PORTD &= ~_BV(7)
#define ROW_CLEAR_ENABLED	PORTE |= _BV(6)
#define ROW_CLEAR_DISABLED	PORTE &= ~_BV(6)

#define COL_CLOCK_HIGH		PORTB |= _BV(4)
#define COL_CLOCK_LOW		PORTB &= ~_BV(4)
#define COL_DATA_HIGH		PORTB |= _BV(5)
#define COL_DATA_LOW		PORTB &= ~_BV(5)
#define COL_LATCH_HIGH		PORTB |= _BV(6)
#define COL_LATCH_LOW		PORTB &= ~_BV(6)
#define COL_CLEAR_ENABLED	nop
#define COL_CLEAR_DISABLED	nop
#define COL_OUTPUT_ENABLED	PORTB &= ~_BV(7) // active low
#define COL_OUTPUT_DISABLED	PORTB |= _BV(7) // active low

#else
#error Unsupported CPU type
#endif


class LedShieldDriver
{
public:
	LedShieldDriver();

	boolean initialize(uint8_t rows, uint8_t cols);

	void write();
	void blank();
	void clear();

	uint8_t getRows();
	uint8_t getColumns();

	void setIndexedValue(uint16_t index, uint16_t value);
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
	uint8_t rowDrivers, colDrivers;
	uint16_t hsBuffer;

	volatile uint8_t currentCol;

};

#endif /* LEDSHIELDDRIVER_H_ */
