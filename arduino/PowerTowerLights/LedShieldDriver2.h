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

#define COLS_PER_DRIVER 	8
#define ROWS_PER_DRIVER 	24
#define MAX_INTENSITY 			4095

#define INDEX(row,col) (row+col*MAX_ROWS)

#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328P__) // Uno

#define ROW_CLK_HIGH		PORTD |= 0x10
#define ROW_CLK_LOW			PORTD &= 0xEF
#define ROW_DATA_HIGH		PORTC |= 0x40
#define ROW_DATA_LOW		PORTC &= 0xBF
#define ROW_LATCH_HIGH		PORTD |= 0x80
#define ROW_LATCH_LOW		PORTD &= 0x7F
#define ROW_CLEAR_ENABLE	PORTE |= 0x40
#define ROW_CLEAR_DISABLE	PORTE &= 0xBF

#define COL_CLK_HIGH		PORTB |= 0x10
#define COL_CLK_LOW			PORTB &= 0xEF
#define COL_DATA_HIGH		PORTB |= 0x20
#define COL_DATA_LOW		PORTB &= 0xDF
#define COL_LATCH_HIGH		PORTB |= 0x40
#define COL_LATCH_LOW		PORTB &= 0xBF
#define COL_CLEAR_ENABLE	nop
#define COL_CLEAR_DISABLE	nop
#define COL_OE_ENABLE		PORTB |= 0x80
#define COL_OE_DISABLE		PORTB &= 0x7E

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

#define ROW_CLOCK_HIGH		PORTD |= 0x10
#define ROW_CLOCK_LOW		PORTD &= 0xEF
#define ROW_DATA_HIGH		PORTC |= 0x40
#define ROW_DATA_LOW		PORTC &= 0xBF
#define ROW_LATCH_HIGH		PORTD |= 0x80
#define ROW_LATCH_LOW		PORTD &= 0x7F
#define ROW_CLEAR_ENABLED	PORTE |= 0x40
#define ROW_CLEAR_DISABLED	PORTE &= 0xBF

#define COL_CLOCK_HIGH		PORTB |= 0x10
#define COL_CLOCK_LOW		PORTB &= 0xEF
#define COL_DATA_HIGH		PORTB |= 0x20
#define COL_DATA_LOW		PORTB &= 0xDF
#define COL_LATCH_HIGH		PORTB |= 0x40
#define COL_LATCH_LOW		PORTB &= 0xBF
#define COL_CLEAR_ENABLED	nop
#define COL_CLEAR_DISABLED	nop
#define COL_OUTPUT_ENABLED	PORTB &= 0x7E // active low
#define COL_OUTPUT_DISABLED	PORTB |= 0x80 // active low

#else
#error Unsupported CPU type
#endif


class LedShieldDriver2
{
public:
	LedShieldDriver2();

	boolean initialize(uint8_t rows, uint8_t cols);
//	boolean initializeHighSideDriver(uint8_t num, uint8_t clk, uint8_t data, uint8_t lat, uint8_t clr, uint8_t oe);
//	boolean initializeLowSideDriver(uint8_t num, uint8_t clk, uint8_t data, uint8_t lat, uint8_t clr, uint8_t oe);

	void write();
//	void clear(boolean b);
//	void clearAll();

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
	uint8_t rowDrivers, colDrivers;

	volatile uint8_t currentCol;


	// ls methods
//	void setLsBlank(boolean b);
//	void setLsOutputEnable(boolean b);
//	void lsWriteValue(uint16_t value);
//	void lsWrite(uint16_t *buffer);
//	void lsClear();


	// low side variables
//	uint8_t lsNumDrivers;
//	uint16_t lsMaxIntensity, lsTotalChannels;

//	volatile uint8_t *lsClockPort, *lsDataPort, *lsLatchPort, *lsOePort, *lsBlankPort;
//	uint8_t lsClockMask, lsDataMask, lsLatchMask, lsOeMask, lsBlankMask;

	// hs methods
//	void hsWrite(void);
//	void hsClear(void);
//	void setHsClear(boolean b);
//	void setHsOutputEnable(boolean b);


	// High side variables
	uint16_t hsBuffer;
//	uint8_t hsNumDrivers, hsTotalChannels;

//	volatile uint8_t *hsClockPort, *hsDataPort, *hsLatchPort, *hsOePort, *hsClrPort;
//	uint8_t hsClockMask, hsDataMask, hsLatchMask, hsOeMask, hsClrMask;

};

#endif /* LEDSHIELDDRIVER2_H_ */
