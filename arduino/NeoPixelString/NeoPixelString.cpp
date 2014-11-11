#include "NeoPixelString.h"
//#include "RGB.h"


#define NUM_PIXELS 14
#define HALF NUM_PIXELS/2
#define PIN 2
#define DOWN 	0
#define UP		1
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIN);

uint32_t buffer[NUM_PIXELS];
uint8_t mode = 0;

void rainbowFadeStrip(uint32_t runTime, uint32_t fadeTime);
void rainbowFadeOne(uint32_t runTime, uint16_t fadeTime);
void rainbowStrip();
void randomFlash(uint8_t repeat, uint32_t runTime, uint16_t holdTime);
void randomFlashColor(uint32_t runTime, uint16_t holdTime, uint32_t color);
void colorWipe(uint8_t repeat, uint32_t c, uint16_t wait, uint8_t clearAfter);
void car();
void clear();
uint32_t Wheel(byte WheelPos);
void sequenceHalf(uint8_t repeat, uint32_t c, uint16_t waitTime, uint8_t clearAfter);


uint8_t rgbStart[3];
uint8_t rgbEnd[3];
uint8_t rgbDiff[3];
uint8_t rgb[3];

//RGB start;
//RGB end;



void writeColor(uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t color;
	color = 0;
	color = r;
	color = color<<8;
	color = color | g;
	color = color<<8;
	color = color | b;

  for(uint8_t i=0; i<NUM_PIXELS; i++)
  {
	  pixels.setPixelColor(i, color);
  }
  pixels.show();
}


uint32_t add(uint32_t color, uint8_t v)
{
	RGB c;
	c.set(color);
	c.add( v );
	return c.getColor();
}

void writeColor(uint32_t color, uint8_t show)
{
  for(uint8_t i=0; i<NUM_PIXELS; i++)
  {
	  pixels.setPixelColor(i, color);
  }
  if(show)
  {
	  pixels.show();
  }
}


void writeColor(uint32_t color)
{
  for(uint8_t i=0; i<NUM_PIXELS; i++)
  {
	  pixels.setPixelColor(i, color);
  }
  pixels.show();
}

void strobe(uint32_t duration)
{
	uint32_t end = millis() + duration;
	pixels.setBrightness(255);

	while( millis() < end )
	{
		writeColor(0xffffff);
		delay(35);
		writeColor(0x000000);
		delay(35);
	}
}


void lightning()
{

	uint32_t count, duration, wait, large;
	uint8_t i, b;

	b = false;
	pixels.setBrightness(255);

	count = random(2, 6);
	for(i=0; i<count; i++)
	{
		large = random(0,100);
		writeColor(0xffffff);
		if( large > 40 && b == false)
		{
			delay(random(100, 350));
			b = true;
		}
		else
		{
			delay(random(20, 50));
		}
		writeColor(0x000000);
		if( large > 40 && b == false )
		{
			delay(random(200, 500));
		}
		else
		{
			delay(random(30, 70));
		}
	}
}

void fade(uint8_t direction, uint32_t time, uint32_t color)
{
	uint8_t i;

	for(i=0; i<255; i++)
	{
		if( direction == DOWN )
		{
			  pixels.setBrightness(255-i);
		}
		else if( direction == UP)
		{
			  pixels.setBrightness(i);
		}
		writeColor( color );
		delay(time);
	}

}


void crossFade(uint32_t startColor, uint32_t endColor, uint32_t wait)
{

	uint32_t temp, color;
	uint8_t i, index, diff;

	RGB start, end, d;

	pixels.setBrightness(255);
	start.set(startColor);
	end.set(endColor);
	start.difference( &end, &d);
	diff = d.getMax();

	rgbStart[0] = startColor & 0x0ff;
	rgbStart[1] = (startColor>>8) & 0x0ff;
	rgbStart[2] = (startColor>>16) & 0x0ff;

	rgbEnd[0] = 0x0000ff & endColor;
	rgbEnd[1] = 0x0000ff & (endColor>>8);
	rgbEnd[2] = 0x0000ff & (endColor>>16);


	for(i=0; i<3; i++)
	{
		rgb[i] = rgbStart[i];

		if( rgbStart[i] < rgbEnd[i] )
		{
			rgbDiff[i] = rgbEnd[i] - rgbStart[i];
		}
		else
		{
			rgbDiff[i] = rgbStart[i] - rgbEnd[i];
		}
	}

	diff = max(rgbDiff[0], rgbDiff[1]);
	diff = max( diff, rgbDiff[2]);

	for(index = 0; index<diff; index++)
	{

		for(i=0; i<3; i++)
		{
			if(rgbDiff[i] > 0 )
			{
				if( (rgbStart[i] < rgbEnd[i]) )
				{
					rgb[i] = rgbStart[i]+index;
				}
				else if( rgbStart[i] > rgbEnd[i] )
				{
					rgb[i] = rgbStart[i] - index;
				}
			}
		}
		writeColor( rgb[2], rgb[1], rgb[0] );
		delay(wait);
	}

}

void selectCrossover()
{
	uint32_t x, high, low;

	x = random(0,100);
	high = 85;
	low= 20;

	if( x > high )
	{
		lightning();
	}
	else if( x < low )
	{
		strobe(random(750,2000));
	}
}

void setup()
{
	randomSeed(analogRead(0));
	pixels.begin();
	pixels.setBrightness(255);
	writeColor(0x00ffffff, false);
}

void loop()
{
	uint8_t i, b;

	uint32_t x, high, low;

	b = false;

	fade(UP, 10, ORANGE);
	delay(5000);
	fade(DOWN, 10, ORANGE);

	selectCrossover();

	fade(UP, 10, PURPLE);
	delay(5000);
	fade(DOWN, 10, PURPLE);

	fade(UP, 10, RED);
	delay(5000);
	selectCrossover();
	fade(DOWN, 10, RED);

	fade(UP, 10, GREEN);
	delay(5000);
	fade(DOWN, 10, GREEN);
	selectCrossover();

	fade(UP, 10, BLUE);
	delay(5000);
	selectCrossover();
	fade(DOWN, 10, BLUE);

	selectCrossover();



}


/** makes whole strip the same color rainbox **/
void rainbowFadeStrip(uint32_t runTime, uint32_t fadeTime)
{
  uint32_t endTime  = millis() + runTime;
  uint16_t i, j;

  while( millis() < endTime )
  {

    for(j=0; j<256; j++)
    {
      for(i=0; i<NUM_PIXELS; i++)
      {
        pixels.setPixelColor(i, Wheel((j) & 255));
      }
      pixels.show();
      delay(fadeTime);
    } // end for
  }

  clear();

}

void rainbowFadeOne(uint32_t runTime, uint16_t fadeTime)
{
  uint32_t endTime  = millis() + runTime;
  uint16_t i, j;

  while( millis() < endTime )
  {
    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
      for(i=0; i< pixels.numPixels(); i++) {
        pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
      }
      pixels.show();
      delay(fadeTime);
    }
  }
  clear();
}

/**
* Flashes random LED with random color
*/
void randomFlash(uint8_t repeat, uint32_t runTime, uint16_t holdTime)
{
  for(uint8_t i=0; i<repeat; i++)
  {
    randomFlashColor(runTime, holdTime, Wheel( random(0xff) ));
  }

} // randomFlash


/**
* Flashes random LED with specified color
*/
void randomFlashColor(uint32_t runTime, uint16_t holdTime, uint32_t color)
{
  uint32_t endTime  = millis() + runTime;
  uint8_t i;

  // loop for duration
  while( millis() < endTime )
  {
    i = random(NUM_PIXELS);
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(holdTime);
    pixels.setPixelColor(i, 0);
  }

} // randomFlash

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint16_t waitTime, uint8_t clearAfter)
{
  for(uint8_t i=0; i<pixels.numPixels(); i++)
  {
      if(clearAfter==true && i !=0)
      {
        pixels.setPixelColor(i-1,0);
      }
      else if(clearAfter == true && i==0)
      {
        pixels.setPixelColor(pixels.numPixels()-1,0);
      }
      pixels.setPixelColor(i, c);
      pixels.show();
      delay(waitTime);
  }
}

// Fill the dots one after the other with a color
void sequenceHalf(uint8_t repeat, uint32_t c, uint16_t waitTime, uint8_t clearAfter)
{
  uint8_t numPixels = pixels.numPixels();
  uint8_t halfNumPixels = numPixels/2;

  for(uint8_t r=0; r< repeat; r++)
  {
    for(uint8_t i=0; i<halfNumPixels; i++)
    {
        pixels.setPixelColor(i, c);
        pixels.setPixelColor((numPixels-1)-i, c);
        pixels.show();
        delay(waitTime);

        if( clearAfter == true )
        {
          pixels.setPixelColor(i, 0);
          pixels.setPixelColor((numPixels-1)-i, 0);
        }
    }
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
  if(WheelPos < 85) {
   return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void clear()
{
  for(uint8_t i=0; i<NUM_PIXELS; i++)
  {
     pixels.setPixelColor(i, 0);
  }
  pixels.show();

}


