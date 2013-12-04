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
	HighSideDriver();

	boolean initialize(uint8_t n, uint8_t c, uint8_t d, uint8_t l, uint8_t clr, uint8_t o);

	void setValue(uint8_t chan, uint8_t value);
	void write(void);
	void clear(void);
	void setClear(boolean b);
	void setOutputEnable(boolean b);

private:
	uint8_t *buffer;
	uint8_t numdrivers, totalChannels;

	volatile uint8_t *clockPort, *dataPort, *latchPort, *oePort, *clrPort;
	uint8_t clockMask, dataMask, latchMask, oeMask, clrMask;

};

#endif /* _HIGH_SIDE_DRIVER_H_ */
