/*
 * NeoPixelLibTest.cpp
 *
 *  Created on: Sep 12, 2015
 *      Author: tsasala
 */

#include "NeoPixelLibTest.h"

#ifdef __TEST

NeoPixelWrapper controller = NeoPixelWrapper();

uint8_t gPattern = 18;

volatile uint32_t endTime;
void commandDelay(uint32_t time);

void setup()
{

	if( controller.initialize(50, 2) == false )
	{

	}

}
void test()
{
	endTime = millis()+10000;
	switch (gPattern)
	{
	case 1:
		controller.rainbow(0);
		break;
	case 2:
		controller.rainbow(80);
		break;
	case 3:
		controller.confetti(CRGB::Red);
		break;
	case 4:
		controller.sparkle(CRGB::White, 10);
		break;
	case 5:
		controller.cylon(CRGB::Green);
		break;
	case 6:
		controller.juggle();
		break;
	case 7:
		controller.bpm();
		break;
	case 8:
		controller.fadeRainbow();
		break;
	case 9:
		controller.pattern(0x33, LEFT, CRGB::RED, CRGB::White, 500, 500);
		break;
	case 10:
		controller.randomFlash(25,25);
		break;
	case 11:
		controller.randomFlashColor(25, 10, ORANGE, BLACK);
		break;
	case 12:
		controller.middle(IN, CRGB::Red, CRGB::Blue, 50, 50, false, true);
		break;
	case 13:
		controller.middle(IN, CRGB::Red, CRGB::Blue, 50, 50, true, true);
		break;
	case 14:
		controller.middle(OUT, CRGB::Green, CRGB::Yellow, 50, 50, false, true);
		break;
	case 15:
		controller.middle(OUT, CRGB::Green, CRGB::Yellow, 50, 50, true, true);
		break;
	case 16:
		controller.colorBounce(0x01, LEFT, CRGB::White, CRGB::Black, 25, 25, 20, true, true);
		break;
	case 17:
		controller.colorBounce(0x01, LEFT, CRGB::White, CRGB::Black, 25, 25, 20, false, true);
		break;
	case 18:
		controller.outAndBack(0x01, LEFT, CRGB::White, CRGB::Black, 25, 25, 25);
		break;
	case 19:
		controller.outAndBack(0x01, RIGHT, CRGB::White, CRGB::Black, 25, 25, 25);
		break;
	default:
		gPattern = 0;
		break;
	}
	gPattern++;

}


void loop()
{
	test();
}

void commandDelay(uint32_t time)
{
	endTime = millis() + time;
}


boolean isCommandAvailable()
{
	if( millis() > endTime )
	{
		return true;
	}
	else
	{
		return false;
	}
}

#endif
