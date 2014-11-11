/*
 * RGB.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: tsasala
 */

#include "RGB.h"

RGB::RGB()
{
	red = 0;
	green = 0;
	blue = 0;
}

RGB::RGB(uint32_t x)
{
	red = (x >> 16) & 0x0ff;
	green = (x >> 8) & 0x0ff;
	blue = x & 0x0ff;
}

RGB::RGB(uint8_t r, uint8_t g, uint8_t b)
{
	red = r;
	green = g;
	blue = b;
}

void RGB::set(uint32_t x)
{
	red = (x >> 16) & 0x0ff;
	green = (x >> 8) & 0x0ff;
	blue = x & 0x0ff;
}

void RGB::set(uint8_t r, uint8_t g, uint8_t b)
{
	red = r;
	green = g;
	blue = b;
}

uint8_t RGB::getBlue()
{
	return blue;
}

void RGB::setBlue(uint8_t b)
{
	blue = b;
}

uint8_t RGB::getGreen()
{
	return green;
}

void RGB::setGreen(uint8_t g)
{
	green = g;
}

uint8_t RGB::getRed()
{
	return red;
}

void RGB::setRed(uint8_t r)
{
	red = r;
}

uint32_t RGB::getColor()
{
	uint32_t color = 0;
	color = red;
	color = (color << 8) | green;
	color = (color << 8) | blue;
	return color;
}

void RGB::add(RGB *x)
{
	if( red <= 255-x->red )
	{
		red += x->red;
	}
	if( green <= 255-x->green)
	{
		green += x->green;
	}
	if( blue <= 255-x->blue)
	{
		blue += x->blue;
	}
}

void RGB::add(uint8_t r, uint8_t g, uint8_t b)
{
	if( red <= 255-r )
	{
		red += r;
	}
	if( green <= 255-g)
	{
		green += g;
	}
	if( blue <= 255-b)
	{
		blue += b;
	}
}

void RGB::addRed(uint8_t r)
{
	if( red <= 255-r )
	{
		red += r;
	}
}

void RGB::addGreen(uint8_t g)
{
	if( green <= 255-g)
	{
		green += g;
	}
}
void RGB::addBlue(uint8_t b)
{
	if( blue <= 255-b)
	{
		blue += b;
	}
}

void RGB::subtract(RGB *x)
{
	if( red >= x->red)
	{
		red -= x->red;
	}
	if( green >= x->green )
	{
		green -= x->green;
	}
	if( blue >= x->blue )
	{
		blue -= x->blue;
	}
}

void RGB::subtract(uint8_t r, uint8_t g, uint8_t b)
{
	if( red >= r)
	{
		red -= r;
	}
	if( green >= g )
	{
		green -= g;
	}
	if( blue >= b )
	{
		blue -= b;
	}
}

void RGB::subtractRed(uint8_t r)
{
	if( red >= r)
	{
		red -= r;
	}
}

void RGB::subtractGreen(uint8_t g)
{
	if( green >= g )
	{
		green -= g;
	}
}
void RGB::subtractBlue(uint8_t b)
{
	if( blue >= b )
	{
		blue -= b;
	}
}

uint8_t RGB::equals(uint32_t x)
{
	return (red == ((x >> 16) & 0x0ff)) && (green == ((x >> 8) & 0x0ff)) && (blue == (x & 0x0ff));
}

uint8_t RGB::equals(uint8_t r, uint8_t g, uint8_t b)
{
	return (red == r) && (green==g) && (blue == b);
}

uint8_t RGB::equals(RGB *x)
{
	return (red == x->red) && (green == x->green) && (blue == x->blue);
}

void RGB::difference(RGB *x, RGB *result)
{
	if( red < x->red )
	{
		result->red = x->red - red;
	}
	else
	{
		result->red = red - x->red;
	}
	if( green < x->green )
	{
		result->green = x->green - green;
	}
	else
	{
		result->green = green - x->green;
	}
	if( blue < x->blue )
	{
		result->blue = x->blue - blue;
	}
	else
	{
		result->blue = blue - x->blue;
	}
}

uint8_t RGB::getMax()
{
	uint8_t max = 0;

	max = max(red, green);
	max = max(max, blue);

	return max;
}

