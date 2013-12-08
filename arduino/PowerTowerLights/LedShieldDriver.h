/*
 * LedShieldDriver.h
 *
 *  Created on: Dec 8, 2013
 *      Author: tsasala
 */

#ifndef LEDSHIELDDRIVER_H_
#define LEDSHIELDDRIVER_H_

#include <Arduino.h>
#include "FlexiTimer2.h"
#include "Tlc5947Driver.h"
#include "HighSideDriver.h"

class LedShieldDriver
{
public:
	LedShieldDriver();
	boolean initialize(uint8_t rows, uint8_t cols);
	uint16_t* getBuffer();

	void write();
	void clear();
	void clearAll();

	void setValue(uint8_t row, uint8_t col, uint16_t value);
	uint16_t getValue(uint8_t row, uint8_t col);

private:
	uint16_t *buffer;

};

#endif /* LEDSHIELDDRIVER_H_ */
