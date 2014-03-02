/*
 * PowerTowerLights.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: tsasala
 */
#include "PowerTowerLights.h"

//LedShieldDriver driver = LedShieldDriver();
LedShieldDriver2 driver2 = LedShieldDriver2();

void isr();

#define WAIT 50

const int led_pin = 13;			// default to pin 13

void error(uint8_t errorCode);
void LEDscan(float degreeoffset);
void crossfade();

int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


/**
 * Standard arduino setup.  Called once
 *
 */
void setup()
{
//	delay(10000);  // wait for leonardo to fully reset

#ifdef __DEBUG
	Serial.begin(115200);
#endif

#ifdef __DEBUG
	Serial.print("free=");
	Serial.println(freeRam());
#endif

#ifdef __DEBUG
	Serial.println("Initializing shield driver");
#endif
	if( driver2.initialize(ROWS, COLS) == false )
	{
		error(10);
	}

#ifdef __DEBUG
	Serial.print("free=");
	Serial.println(freeRam());
#endif

#ifdef __DEBUG
	Serial.println("Initializing timer");
#endif
	FlexiTimer2::set(8, 0.0001, isr ); // 800us period
	FlexiTimer2::start();

#ifdef __DEBUG
	Serial.println("**** init complete *****");
#endif

	delay(2500);

}

/**
 * Standard arduino loop.  Called repeatedly
 *
 */
void loop()
{
	uint8_t i, j, k;
	int8_t z;

#ifdef __DEBUG
	Serial.print("free=");
	Serial.println(freeRam());
#endif

	//crossfade();

	// rotate 0->n
	for(i=0; i<5; i++)
	{
		for(j=0; j<8; j++)
		{
			driver2.setColumn(j, 4095);
			driver2.write();
			delay(100);
			driver2.setColumn(j, 0);
			driver2.write();
		}
	}

	// rotate n->0
	for(i=0; i<5; i++)
	{
		for(z=7; z>=0; z--)
		{
			driver2.setColumn(z, 4095);
			driver2.write();
			delay(100);
			driver2.setColumn(z, 0);
			driver2.write();
		}
	}


	i = 175;
	while( i !=0 )
	{
		for(j=0; j<12; j++)
		{
			driver2.setRow(j, 4095);
			driver2.write();
			delay(i);
			driver2.setAll(0);
			driver2.write();
		}
		i = i-25;
	}
	for(i=0; i<5; i++)
	{
		for(j=0; j<12; j++)
		{
			driver2.setRow(j, 4095);
			driver2.write();
			delay(25);
			driver2.setAll(0);
			driver2.write();
		}
	}

	for(i=0; i<7; i++)
	{
		for(j=0; j<12; j++)
		{
			driver2.setRow(j, 4095);
			driver2.setRow(11-j, 4095);
			driver2.write();
			delay(75);
			driver2.setAll(0);
			driver2.write();
		}
	}

	for(i=0; i<7; i++)
	{
		for(j=0; j<8; j++)
		{
			driver2.setColumn(j, 4095);
			driver2.setColumn(7-j, 4095);
			driver2.write();
			delay(75);
			driver2.setAll(0);
			driver2.write();
		}
	}


	for(i=0; i<8; i++)
	{
		k = 0;
		j = 0;
		while( k < 12 )
		{
			for(j=11; j>=0; j--)
			{
				driver2.setValue(j, i, 4095);
				driver2.write();
				delay(50);
				if( (j-k) == 0 )
				{
					break;
				}
				driver2.setValue(j, i, 0);
				driver2.write();
			}
			k++;
		}
	}
	driver2.setAll(0);
	driver2.write();


	k = 0;
	while( k < 12 )
	{
		for(j=11; j>=0; j--)
		{
			driver2.setRow(j, 4095);
			driver2.write();
			delay(50);
			if( (j-k) == 0 )
			{
				break;
			}
			driver2.setRow(j, 0);
			driver2.write();
		}
		k++;
	}
	driver2.setAll(0);
	driver2.write();

	k = 12*8;

	while( k > 0 )
	{
		j = random(0, 12*8);
		i = j/12; // column
		j = j%12; // row

		if( driver2.getValue(j, i) == 0 )
		{
			driver2.setValue(j, i, 4095);
			driver2.write();
			k--;
			delay(25);
		}
	}



} // end loop



/**
 * Interrupt service routine
 *
 */
void isr()
{
	driver2.execInterrupt();

	static boolean output = HIGH;
	digitalWrite(led_pin, output);
	output = !output;
}

void crossfade()
{
	float offset = 0;

	while (1)
	{
		for (offset = 0; offset < 360; offset += 0.5)
		{
			LEDscan(offset);
			delay(2);
		}
	}

}


/**
 * Flashes on-board LED to indicate critical failure
 */
void error(uint8_t errorCode)
{
	uint16_t delayValue;

	delayValue = errorCode*10;

	while (1)
	{
		digitalWrite(13, HIGH);
		delay(delayValue);
		digitalWrite(13, LOW);
		delay(delayValue);
	}
}

// Sample function to draw a scanning pattern with fading
void LEDscan(float degreeoffset)
{
	float brightnessfactor = 0;
	float scanindex = (1.0 + sin(degreeoffset * 3.14159 / 180.0)) * (float) 6.0;

	for (uint8_t LEDindex = 0; LEDindex < 12; LEDindex++)
	{
		brightnessfactor = exp(0.0 - fabs(scanindex - ((float) LEDindex + 0.5)) * 1.3);
		driver2.setRow(LEDindex, (uint16_t)(4095*brightnessfactor));
	}
	driver2.write();
}

