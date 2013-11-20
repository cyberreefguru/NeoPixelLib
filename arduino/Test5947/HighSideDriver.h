/*
 * HighSideDriver.h
 *
 *  Created on: Nov 11, 2013
 *      Author: tsasala
 */

#ifndef HIGHSIDEDRIVER_H_
#define HIGHSIDEDRIVER_H_

#include <Arduino.h>

class HighSideDriver
{

public:
	HighSideDriver(uint8_t num, uint8_t clk, uint8_t data, uint8_t latch, uint8_t clr, uint8_t oe);

	boolean begin(void);

	void setValue(uint8_t chan, uint8_t value);
	void write(void);
	void clear(void);
	void setClear(boolean b);
	void setEnable(boolean b);

private:
	uint8_t *buffer;
	uint8_t numdrivers, clock, data, latch, oe, clr;

};



#endif /* HIGHSIDEDRIVER_H_ */
