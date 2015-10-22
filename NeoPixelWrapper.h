/*
 * NeoPixelWrapper.h
 *
 *  Created on: Sep 12, 2015
 *      Author: tsasala
 */

#ifndef NEOPIXELWRAPPER_H_
#define NEOPIXELWRAPPER_H_

#include <Arduino.h>
#include "NeoPixelLib.h"
#include <FastLed.h>

#define LED_PIN		3

#define WHITE	CRGB::White
#define BLACK	CRGB::Black
#define RED		CRGB::Red
#define BLUE	CRGB::Blue
#define GREEN	CRGB::Green
#define MAGENTA	CRGB::Magenta
#define CYAN	CRGB::Cyan
#define YELLOW	CRGB::Yellow
#define ORANGE	CRGB::Orange
#define PURPLE	CRGB::Purple
#define RAINBOW	CRGB::Black

#define DOWN 	0
#define UP		1
#define RIGHT	0
#define LEFT	1
#define IN		0
#define OUT		1



class NeoPixelWrapper
{
public:
	NeoPixelWrapper();
	boolean initialize(uint8_t numLeds, uint8_t intensity);
	void setFramesPerSecond(uint8_t fps);
	uint8_t getFramesPerSecond();
	void setHueUpdateTime(uint8_t updateTime);
	uint8_t getHueUpdateTime();
	void setIntensity(uint8_t i);
	uint8_t getIntensity();

    void fill(CRGB color, uint8_t show);
    void fillPattern(uint8_t pattern, CRGB onColor, CRGB offColor);

    void pattern(uint16_t repeat, uint8_t pattern, uint8_t direction, CRGB onColor, CRGB offColor, uint32_t onTime, uint32_t offTime);
    void wipe(uint8_t pattern, uint8_t direction, CRGB onColor, CRGB offColor, uint32_t onTime, uint32_t offTime, uint8_t clearAfter, uint8_t clearEnd);
    void bounce(uint16_t repeat, uint8_t pattern, uint8_t direction, CRGB onColor, CRGB offColor, uint32_t onTime, uint32_t offTime, uint32_t bounceTime, uint8_t clearAfter, uint8_t clearEnd);
    void middle(uint16_t repeat, uint8_t direction, CRGB color1, CRGB color2, uint32_t onTime, uint32_t offTime, uint8_t clearAfter, uint8_t clearEnd);
    void randomFlash(uint32_t runTime, uint32_t onTime, uint32_t offTime, CRGB onColor, CRGB offColor);
    void rainbow(uint32_t runTime,uint8_t glitterProbability, CRGB glitterColor);
    void rainbowFade(uint32_t runTime);

	void confetti(uint32_t runTime,CRGB color, uint8_t numOn);
	void cylon(uint16_t repeat, CRGB color);
	void bpm(uint32_t runTime);
	void juggle(uint32_t runTime);

	boolean commandDelay(uint32_t waitTime);

protected:
	CRGB *leds;
	uint8_t intensity;

	void setWipeColor(CRGB newColor, uint16_t index, uint32_t onTime, uint32_t offTime, uint8_t clearAfter);


};

//end of add your includes here
#ifdef __cplusplus
extern "C"
{
#endif

extern boolean isCommandAvailable();

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* NEOPIXELWRAPPER_H_ */
