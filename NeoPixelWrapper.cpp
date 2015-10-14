/*
 * NeoPixelWrapper.cpp
 *
 *  Created on: Sep 12, 2015
 *      Author: tsasala
 */

#include "NeoPixelWrapper.h"

#define FRAMES_PER_SECOND			120

uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t sparkleCount = 0;
uint8_t frameWaitTime = 1000/FRAMES_PER_SECOND;
uint8_t gHueUpdateTime = 20;

/**
 * Constructor
 */
NeoPixelWrapper::NeoPixelWrapper()
{
	leds = 0;
}

/**
 * Changes the amount of times per second functions are
 * updated.
 */
void NeoPixelWrapper::setFramesPerSecond(uint8_t fps)
{
	frameWaitTime = 1000/fps;
}

/**
 * Changes the amount of time to wait before updating the hue
 *
 */
void NeoPixelWrapper::setHueUpdateTime(uint8_t updateTime)
{
	gHueUpdateTime = updateTime;
}

/**
 * Initializes the library
 */
boolean NeoPixelWrapper::initialize(uint8_t numLeds, uint8_t ledPin)
{
	boolean status = false;

	leds = (CRGB *) malloc(sizeof(CRGB) * numLeds);
	if (leds != 0)
	{
		FastLED.addLeds<WS2812, LED_PIN>(leds, numLeds).setCorrection(TypicalLEDStrip);
		// set master brightness control
		FastLED.setBrightness(200);
	}

	return status;
}

/**
 * Internal delay function that checks for an available command.
 * If a command is available, it will break out of the delay
 * routine and return true.  If delay times out, false is returned.
 *
 * @time the time to wait in ms
 * @return boolean, true=command available; false=no command available
 */
boolean NeoPixelWrapper::commandDelay(uint32_t time)
{
	boolean cmd = isCommandAvailable();
	if( !cmd )
	{
		for (uint32_t i = 0; i < time; i++)
		{
			delay(1);
			cmd = isCommandAvailable();
			if (cmd)
			{
				break;
			}
		}
	}
	return cmd;
}



/**
 * sets a color to write to all pixels
 *
 * @color - color to set
 * @show - if true, sets color immediately
 */
void NeoPixelWrapper::fill(CRGB color, uint8_t show)
{
    for (uint8_t i = 0; i < FastLED.size(); i++)
    {
        leds[i] = color;
    }
    if (show)
    {
        FastLED.show();
    }
}

/**
 * Sets strip to specified pattern. 1 = on, 0 = off.
 * Repeats every 8 bits.
 *
 */
void NeoPixelWrapper::fillPattern(uint8_t pattern, CRGB onColor, CRGB offColor)
{
    uint16_t j;
    int i;

    j = 0;

    for (i = 0; i < FastLED.size(); i++)
    {
        if ((pattern >> j) & 0x01)
        {
            leds[i] = onColor;
        }
        else
        {
            leds[i] = offColor;
        }
        j = j + 1;
        if (j == 8)
        {
            j = 0;
        }
    }
    FastLED.show();

}



/**
 * Rotates a pattern across the stripe; onTime determines pause between rotation
 */
void NeoPixelWrapper::pattern(uint16_t repeat, uint8_t pattern, uint8_t direction, CRGB onColor, CRGB offColor, uint32_t onTime, uint32_t offTime)
{
	uint16_t i, count;

	i = 0;
	count = 0;
	while (isCommandAvailable() == false)
	{
		fillPattern(pattern, onColor, offColor);
		if (commandDelay(onTime))
			break;
		if (direction == LEFT)
		{
			if (pattern & 0x80)
			{
				i = 0x01;
			}
			else
			{
				i = 0x00;
			}
			pattern = pattern << 1;
			pattern = pattern | i;
		}
		else if (direction == RIGHT)
		{
			if (pattern & 0x01)
			{
				i = 0x80;
			}
			else
			{
				i = 0x00;
			}
			pattern = pattern >> 1;
			pattern = pattern | i;
		}

		count++;
		if( repeat > 0 && count >= repeat )
		{
			break;
		}
	} // end while
}

/**
 * Turns on LEDs one at time in sequence.  LEFT = 0->n; RIGHT = n -> 0
 *
 * @total - number of LEDs to wipe
 * @direction - left (up) or right (down)
 * @color - color to fill LEDs with
 * @waitTime - time to keep LED on
 * @clearAfter - turn LED off after waiting
 * @clearBetween - clear string in between repeats
 */
void NeoPixelWrapper::wipe(uint8_t pattern, uint8_t direction, CRGB onColor, CRGB offColor, uint32_t onTime, uint32_t offTime, uint8_t clearAfter, uint8_t clearEnd)
{
	fill(offColor, true);
	if( direction == LEFT)
	{
		for(uint8_t i=0; i<FastLED.size(); i++)
		{
			setWipeColor(onColor, i, onTime, offTime, clearAfter);
			if(isCommandAvailable()) return;
		}
	}
	else if (direction == RIGHT )
	{
		for(uint8_t i=FastLED.size(); i>0; i--)
		{
			setWipeColor(onColor, i, onTime, offTime, clearAfter);
			if(isCommandAvailable()) return;
		}
	}

	if( clearEnd )
	{
		fill(offColor, true);
	}
}

/**
 *
 */
void NeoPixelWrapper::bounce(uint16_t repeat, uint8_t pattern, uint8_t direction, CRGB onColor, CRGB offColor, uint32_t onTime, uint32_t offTime, uint32_t bounceTime, uint8_t clearAfter, uint8_t clearEnd)
{
	uint16_t count = 0;

	while (isCommandAvailable() == false)
	{
		if( direction == LEFT )
		{
			wipe(pattern, LEFT, onColor, offColor, onTime, offTime, clearAfter, clearEnd);
			if( commandDelay(bounceTime) ) return;
			wipe(pattern, RIGHT, onColor, offColor, onTime, offTime, clearAfter, clearEnd);
			if(isCommandAvailable()) return;
			if( commandDelay(bounceTime) ) return;
		}
		else if (direction == RIGHT )
		{
			wipe(pattern, RIGHT, onColor, offColor, onTime, offTime, clearAfter, clearEnd);
			if( commandDelay(bounceTime) ) return;
			wipe(pattern, LEFT, onColor, offColor, onTime, offTime, clearAfter, clearEnd);
			if( commandDelay(bounceTime) ) return;
		}
		count++;
		if( repeat > 0 && count > repeat )
		{
			break;
		}
	} // end while

}


/**
 * Starts in the middle and works out; or starts in the end and works in
 */
void NeoPixelWrapper::middle(uint16_t repeat, uint8_t direction, CRGB color1, CRGB color2, uint32_t onTime, uint32_t offTime, uint8_t clearAfter, uint8_t clearEnd)
{
	uint16_t count = 0;
	uint8_t numPixels = FastLED.size();
	uint8_t halfNumPixels = numPixels/2;

	if(clearEnd)
	{
		fill(color2, true);
	}
	while (isCommandAvailable() == false)
	{
		if(direction == IN)
		{
			for(uint8_t i=0; i<halfNumPixels; i++)
			{
				leds[i] = color1;
				leds[(numPixels-1)-i] = color1;
				FastLED.show();
				if( commandDelay(onTime) ) return;

				if( clearAfter == true )
				{
					leds[i] = color2;
					leds[(numPixels-1)-i] = color2;
					FastLED.show();
					if( commandDelay(offTime) ) return;
				}
			}
		}
		else if( direction == OUT )
		{
			for(uint8_t i=0; i<halfNumPixels+1; i++)
			{
				leds[halfNumPixels-i] = color1;
				leds[halfNumPixels+i] = color1;
				FastLED.show();
				if( commandDelay(onTime) ) return;

				if( clearAfter == true )
				{
					leds[halfNumPixels-i] = color2;
					leds[halfNumPixels+i] = color2;
					FastLED.show();
					if( commandDelay(offTime) ) return;
				}
			}
		}
		if(clearEnd)
		{
			fill(color2, true);
		}

		count++;
		if( repeat > 0 && count > repeat )
		{
			break;
		}

	} // end while
}

/**
 * Flashes random LED with specified color
 */
void NeoPixelWrapper::randomFlash(uint32_t runTime, uint32_t onTime, uint32_t offTime, CRGB onColor, CRGB offColor)
{
	uint8_t i;

	fill(offColor, true);

	while (isCommandAvailable() == false)
	{
		i = random(FastLED.size());
		leds[i] = onColor;
		FastLED.show();
		if (commandDelay(onTime))
			return;
		leds[i] = offColor;
		if (commandDelay(offTime))
			return;
	}

	fill(offColor, true);

} // randomFlash

/**
 * Fills strip with rainbow pattern
 *
 * @glitter if true, randomly pops white into rainbow pattern
 */
void NeoPixelWrapper::rainbow(uint32_t runTime, uint8_t glitterProbability, CRGB glitterColor)
{
    while(isCommandAvailable() == false )
    {
        // FastLED's built-in rainbow generator
        fill_rainbow(leds, FastLED.size(), gHue, 7);
        if (glitterProbability > 0)
        {
            if (random8() < glitterProbability)
            {
                leds[random16(FastLED.size())] += glitterColor;
            }

        }
        FastLED.show();
        FastLED.delay( frameWaitTime );

        // do some periodic updates
        EVERY_N_MILLISECONDS(gHueUpdateTime)
        {
            gHue++;
        } // slowly cycle the "base color" through the rainbow
    }

} // end rainbow

/**
 * This function draws rainbows with an ever-changing,widely-varying set of parameters.
 * https://gist.github.com/kriegsman/964de772d64c502760e5
 *
 */
void NeoPixelWrapper::rainbowFade(uint32_t runTime)
{

    while(isCommandAvailable() == false )
    {

        static uint16_t sPseudotime = 0;
        static uint16_t sLastMillis = 0;
        static uint16_t sHue16 = 0;

        uint8_t sat8 = beatsin88(87, 220, 250);
        uint8_t brightdepth = beatsin88(341, 96, 224);
        uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
        uint8_t msmultiplier = beatsin88(147, 23, 60);

        uint16_t hue16 = sHue16; //gHue * 256;
        uint16_t hueinc16 = beatsin88(113, 1, 3000);

        uint16_t ms = millis();
        uint16_t deltams = ms - sLastMillis;
        sLastMillis = ms;
        sPseudotime += deltams * msmultiplier;
        sHue16 += deltams * beatsin88(400, 5, 9);
        uint16_t brightnesstheta16 = sPseudotime;

        for (uint16_t i = 0; i < (uint16_t) FastLED.size(); i++)
        {
            hue16 += hueinc16;
            uint8_t hue8 = hue16 / 256;

            brightnesstheta16 += brightnessthetainc16;
            uint16_t b16 = sin16(brightnesstheta16) + 32768;

            uint16_t bri16 = (uint32_t) ((uint32_t) b16 * (uint32_t) b16) / 65536;
            uint8_t bri8 = (uint32_t) (((uint32_t) bri16) * brightdepth) / 65536;
            bri8 += (255 - brightdepth);

            CRGB newcolor = CHSV(hue8, sat8, bri8);

            uint16_t pixelnumber = i;
            pixelnumber = (FastLED.size() - 1) - pixelnumber;

            nblend(leds[pixelnumber], newcolor, 64);
        }

        FastLED.show();
        FastLED.delay( frameWaitTime );
    }

} // end rainbow fade


/**
 * Creates random speckles of the specified color.
 *
 * 5-10 LEDs makes a nice effect
 *
 */
void NeoPixelWrapper::confetti(uint32_t runTime, CRGB color, uint8_t numLeds)
{
    while(isCommandAvailable() == false )
    {
        // random colored speckles that blink in and fade smoothly
        fadeToBlackBy(leds, FastLED.size(), numLeds);
        int pos = random16(FastLED.size());
        if (color == (CRGB)RAINBOW)
        {
            leds[pos] += CHSV(gHue + random8(64), 200, 255);
            // do some periodic updates
            EVERY_N_MILLISECONDS(gHueUpdateTime)
            {
                gHue++;
            } // cycle the "base color" through the rainbow

        }
        else
        {
            leds[pos] += color;
        }
        FastLED.show();
        FastLED.delay( frameWaitTime );

    }

} // end confetti


///**
// * Creates random speckles of the specified color.  Nearly the same as
// * confetti but fewer simulateous LEDs
// *
// */
//void NeoPixelWrapper::sparkle(CRGB color, uint8_t sparkles)
//{
//  while(isCommandAvailable() == false )
//  {
//      sparkleCount++;
//      fadeToBlackBy(leds, FastLED.size(), 5);
//      if( sparkleCount > sparkles )
//      {
//          int index = random16(FastLED.size());
//          while( leds[index] != (CRGB)CRGB::Black )
//          {
//              index = random16(FastLED.size());
//          }
//          leds[index] = color;
//          sparkleCount=0;
//      }
//      FastLED.show();
//      FastLED.delay( frameWaitTime );
//  }
//}



/**
 * Creates "cylon" pattern - bright led followed up dimming LEDs back and forth
 *
 */
void NeoPixelWrapper::cylon(uint16_t repeat, CRGB color)
{
	uint16_t count = 0;
	repeat = FastLED.size()*repeat;

    while(isCommandAvailable() == false )
    {
        fadeToBlackBy(leds, FastLED.size(), 20);
        int pos = beatsin16(10, 0, FastLED.size());
        if (color == (CRGB)RAINBOW)
        {
            leds[pos] += CHSV(gHue, 255, 192);
            // do some periodic updates
            EVERY_N_MILLISECONDS(gHueUpdateTime)
            {
                // slowly cycle the "base color" through the rainbow
                gHue++;
            }
        }
        else
        {
            leds[pos] += color;
        }

        FastLED.show();
        FastLED.delay( frameWaitTime );

		count++;
		if( repeat > 0 && count > repeat )
		{
			break;
		}

    }
} // end cylon

/**
 * No clue how to explain this one...
 *
 */
void NeoPixelWrapper::bpm(uint32_t runTime)
{
    while(isCommandAvailable() == false )
    {
        // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
        uint8_t BeatsPerMinute = 62;
        CRGBPalette16 palette = PartyColors_p;
        uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
        for (int i = 0; i < FastLED.size(); i++)
        { //9948
            leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
        // do some periodic updates
        EVERY_N_MILLISECONDS(gHueUpdateTime)
        {
            gHue++;
        } // slowly cycle the "base color" through the rainbow

        FastLED.show();
        FastLED.delay( frameWaitTime );
    }

}

/**
 * No clue how to explain this one
 *
 */
void NeoPixelWrapper::juggle(uint32_t runTime)
{
    while(isCommandAvailable() == false )
    {
        // eight colored dots, weaving in and out of sync with each other
        fadeToBlackBy(leds, FastLED.size(), 20);
        byte dothue = 0;
        for (int i = 0; i < 8; i++)
        {
            leds[beatsin16(i + 7, 0, FastLED.size())] |= CHSV(dothue, 200, 255);
            dothue += 32;
        }

        FastLED.show();
        FastLED.delay( frameWaitTime );
    }
}

////////////////////////////////////////
// BEGIN PRIVATE FUNCTIONS
////////////////////////////////////////

/**
 * Sets the color of the specified LED for onTime time.  If clearAfter
 * is true, returns color to original color and waits offTime before returning.
 */
void NeoPixelWrapper::setWipeColor(CRGB newColor, uint16_t index, uint32_t onTime, uint32_t offTime, uint8_t clearAfter)
{
    uint32_t curColor;

    curColor = leds[index];
    leds[index] = newColor;
    FastLED.show();
    commandDelay(onTime);
    if(clearAfter == true)
    {
        leds[index] = curColor;
        FastLED.show();
        commandDelay(offTime);
    }

}
