/*
 * PowerTowerLights.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: tsasala
 */
#include "PowerTowerLights.h"

LedShieldDriverScaled driver = LedShieldDriverScaled();

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
#if defined (__AVR_ATmega32U4__) // Leonardo
	delay(10000);  // wait for leonardo to fully reset
#endif

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
	if( driver.initialize(ROWS, COLS) == false )
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
	FlexiTimer2::set(4, 0.0001, isr ); // 400us period
	FlexiTimer2::start();

#ifdef __DEBUG
	Serial.println("**** init complete *****");
#endif

}

/**
 * Standard arduino loop.  Called repeatedly
 *
 */
void loop()
{
	uint8_t i, j, k;
	int8_t z, w;
	uint16_t total, q;


#ifdef __DEBUG
	Serial.print("free=");
	Serial.println(freeRam());
#endif

	//crossfade();
	driver.clear();

//	for(i=0; i<48; i++)
//	{
//		driver.setRow(i, 255);
//		driver.write();
//		delay(250);
//		driver.clear();
//		delay(250);
//	}
//	while(1);
//
//
//	driver.setRow(0, 255);
//	driver.write();
//	while(1);
//
//
//	driver.setColumn(7, 255);
//	driver.write();
//	while(1);

//
//	for(i=0; i<8; i++)
//	{
//		driver.setColumn(i, 255);
//		driver.write();
//		delay(250);
//	}
//
//	while(1);


//	for(i=0; i<10; i++)
//	{
//		for(j=0; j<8; j+2)
//		{
//			driver.setColumn(j, 4095);
//			driver.setColumn(j+1, 4095);
//			driver.write();
//			delay(100);
//			driver.setColumn(j, 0);
//			driver.setColumn(j+1, 0);
//		}
//	}




	driver.clear();

	//
//	k = 0x03;
//	for(i=0; i<32; i++)
//	{
//		for(j=0; j<8; j++)
//		{
//			if( k & (0x01<<j) )
//			{
//				driver.setColumn(j, 4095);
//			}
//			else
//			{
//				driver.setColumn(j, 0);
//			}
//		}
//		driver.write();
//		delay(150);
//		if( k & 0x80 )
//		{
//			k = k << 0x01;
//			k |= 0x01;
//		}
//		else
//		{
//			k = k << 0x01;
//		}
//	}
//
//	driver.clear();

//	// bottom up
//	for(i=0; i<ROWS; i++)
//	{
//		for(j=0; j<COLS; j++)
//		{
//			driver.setValue(i, j, MAX_VALUE);
//			driver.write();
//			delay(75);
//			driver.setValue(i,j,0);
//			driver.write();
//		}
//	}
//
//
//
//	// bottom up
//	for(i=0; i<ROWS; i++)
//	{
//		for(j=0; j<COLS; j++)
//		{
//			driver.setValue(i, j, MAX_VALUE);
//			driver.write();
//			delay(50);
//		}
//	}
//
//	driver.clear();
//
//	// up down
//	for(z=(ROWS-1); z>=0; z--)
//	{
//		for(w=(COLS-1); w>=0; w--)
//		{
//			driver.setValue(z, w, MAX_VALUE);
//			driver.write();
//			delay(50);
//		}
//	}



	// rotate 0->n
	for(i=0; i<5; i++)
	{
		for(j=0; j<COLS; j++)
		{
			driver.setColumn(j, MAX_VALUE);
			driver.write();
			delay(100);
			driver.setColumn(j, 0);
			driver.write();
		}
	}

	// rotate n->0
	for(i=0; i<5; i++)
	{
		for(z=(COLS-1); z>=0; z--)
		{
			driver.setColumn(z, MAX_VALUE);
			driver.write();
			delay(100);
			driver.setColumn(z, 0);
			driver.write();
		}
	}


	i = 125;
	while( i !=0 )
	{
		for(j=0; j<ROWS; j++)
		{
			driver.setRow(j, MAX_VALUE);
			driver.write();
			delay(i);
			driver.setAll(0);
			driver.write();
		}
		i = i-25;
	}
	for(i=0; i<5; i++)
	{
		for(j=0; j<ROWS; j++)
		{
			driver.setRow(j, MAX_VALUE);
			driver.write();
			delay(25);
			driver.setAll(0);
			driver.write();
		}
	}

	for(i=0; i<7; i++)
	{
		for(j=0; j<ROWS; j++)
		{
			driver.setRow(j, MAX_VALUE);
			driver.setRow(COLS-1-j, MAX_VALUE);
			driver.write();
			delay(75);
			driver.setAll(0);
			driver.write();
		}
	}

	for(i=0; i<7; i++)
	{
		for(j=0; j<COLS; j++)
		{
			driver.setColumn(j, MAX_VALUE);
			driver.setColumn(COLS-1-j, MAX_VALUE);
			driver.write();
			delay(75);
			driver.setAll(0);
			driver.write();
		}
	}


//	for(i=0; i<8; i++)
//	{
//		k = 0;
//		j = 0;
//		while( k < ROWS )
//		{
//			for(j=(ROWS-1); j>=0; j--)
//			{
//				driver.setValue(j, i, MAX_VALUE);
//				driver.write();
//				delay(50);
//				if( (j-k) == 0 )
//				{
//					break;
//				}
//				driver.setValue(j, i, 0);
//				driver.write();
//			}
//			k++;
//		}
//	}
//	driver.setAll(0);
//	driver.write();


	k = 0;
	while( k < ROWS )
	{
		for(j=(ROWS-1); j>=0; j--)
		{
			driver.setRow(j, MAX_VALUE);
			driver.write();
			delay(25);
			if( (j-k) == 0 )
			{
				break;
			}
			driver.setRow(j, 0);
			driver.write();
		}
		k++;
	}
	driver.setAll(0);
	driver.write();

	k = ROWS*COLS;

	while( k > 0 )
	{
		j = random(0, ROWS*COLS);
		i = j/ROWS; // column
		j = j%ROWS; // row

		if( driver.getValue(j, i) == 0 )
		{
			driver.setValue(j, i, MAX_VALUE);
			driver.write();
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
	driver.execInterrupt();

//	static boolean output = HIGH;
//	digitalWrite(led_pin, output);
//	output = !output;
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
		driver.setRow(LEDindex, (uint16_t)(4095*brightnessfactor));
	}
	driver.write();
}

