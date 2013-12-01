/*
 * HighSideDriver.h
 *
 *  Created on: Nov 11, 2013
 *      Author: tsasala
 */

#ifndef _HIGH_SIDE_DRIVER_H_
#define _HIGH_SIDE_DRIVER_H_

#include <Arduino.h>

#define CHANNELS_PER_DRIVER 8

class HighSideDriver
{

public:
	HighSideDriver(uint8_t n, uint8_t c, uint8_t d, uint8_t l, uint8_t clr, uint8_t o);

	boolean initialize(void);

	void setValue(uint8_t chan, uint8_t value);
	void write(void);
	void clear(void);
	void setClear(boolean b);
	void setOutputEnable(boolean b);

private:
	uint8_t *buffer;
	uint8_t numdrivers, clock, data, latch, oe, clr, totalChannels;

};

#endif /* _HIGH_SIDE_DRIVER_H_ */
