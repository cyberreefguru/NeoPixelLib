/*
 * RGB.h
 *
 *  Created on: Oct 22, 2014
 *      Author: tsasala
 */

#ifndef RGB_H_
#define RGB_H_

#include <Arduino.h>

class RGB
{
public:

	RGB();
	RGB(uint32_t x);
	RGB(uint8_t r, uint8_t g, uint8_t b);

	void set(uint32_t x);
	void set(uint8_t r, uint8_t g, uint8_t b);
	void setRed(uint8_t r);
	uint8_t getRed();
	void setGreen(uint8_t g);
	uint8_t getGreen();
	void setBlue(uint8_t b);
	uint8_t getBlue();

	uint32_t getColor();

	void add(RGB *x);
	void add(uint32_t x);
	void add(uint8_t r, uint8_t g, uint8_t b);
	void addRed(uint8_t x);
	void addGreen(uint8_t x);
	void addBlue(uint8_t x);

	void subtract(RGB *x);
	void subtract(uint32_t x);
	void subtract(uint8_t r, uint8_t g, uint8_t b);
	void subtractRed(uint8_t x);
	void subtractGreen(uint8_t x);
	void subtractBlue(uint8_t x);

	uint8_t equals(RGB *x);
	uint8_t equals(uint32_t x);
	uint8_t equals(uint8_t r, uint8_t g, uint8_t b);

	void difference(RGB *x, RGB *result);

	uint8_t getMax();

private:
	uint8_t red,green,blue;


};

#endif /* RGB_H_ */
