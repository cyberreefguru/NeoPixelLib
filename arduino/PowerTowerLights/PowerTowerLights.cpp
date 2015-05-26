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
	delay(2000);  // wait for leonardo to fully reset
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
	uint32_t end;

#ifdef __DEBUG
	Serial.print("free=");
	Serial.println(freeRam());
#endif


	driver.clear();


	//crossfade();


	// series 1 on, 2 off, 1 on -> top to bottom
	// series 1 on, 2 off, 1 on -> bottom to top

	// twist up
	// twist down

	// n random on fast
	driver.clear();

	total = ROWS*COLS;
	end = millis() + 5000;
	while( millis() < end )
	{
		for(k=0; k<25; k++)
		{
			q = random(0, total);
			i = q/ROWS; // column
			j = q%ROWS; // row
			driver.setValue(j, i, MAX_VALUE);
		}

		driver.write();
		delay(50);
		driver.clear();
	}


	//
	// BREAK
	//
	driver.clear();

	// Random on
	total = ROWS*COLS;
	while( total > 0 )
	{
		q = random(0, ROWS*COLS);
		i = q/ROWS; // column
		j = q%ROWS; // row

		if( driver.getValue(j, i) == 0 )
		{
			driver.setValue(j, i, MAX_VALUE);
			driver.write();
			total--;
			delay(10);
		}
	}

	delay(100);

	// Random off
	driver.setAll(MAX_VALUE);
	driver.write();
	total = ROWS*COLS;
	while( total > 0 )
	{
		q = random(0, ROWS*COLS);
		i = q/ROWS; // column
		j = q%ROWS; // row

		if( driver.getValue(j, i) == MAX_VALUE )
		{
			driver.setValue(j, i, 0);
			driver.write();
			total--;
			delay(10);
		}
	}


	//
	// BREAK
	//
	driver.clear();

	// sets pattern to be on, off off
	z=0;
	while(z<42)
	{
		driver.setRow(z++, MAX_VALUE);
		driver.setRow(z++, 0);
		driver.setRow(z++, 0);
	}

	// rotates pattern 25 times
	for(i=0; i<25; i++)
	{
		driver.write();
		delay(150);
		driver.rotateRow(DIRECTION_RIGHT, 1);
	}
	// capture last pattern
	driver.write();
	delay(150);

	//
	// BREAK
	//

	driver.clear();

	// sets pattern to be on, off off
	z=0;
	while(z<42)
	{
		driver.setRow(z++, 0);
		driver.setRow(z++, MAX_VALUE);
		driver.setRow(z++, MAX_VALUE);
	}

	// rotates pattern 25 times
	for(i=0; i<25; i++)
	{
		driver.write();
		delay(150);
		driver.rotateRow(DIRECTION_RIGHT, 1);
	}
	// capture last pattern
	driver.write();
	delay(150);


	//
	// BREAK
	//

	// Turn all on
	driver.setAll(MAX_VALUE);
	driver.write();
	delay(50);

	for(i=0; i<2; i++)
	{
		// cycle blank row up to top
		for(z=0; z<ROWS; z++)
		{
			driver.setRow(z, 0);
			driver.write();
			delay(50);
			driver.setRow(z, MAX_VALUE);
			driver.write();
		}

		delay(50);

		// cycle blank row down to bottom
		for(z=(ROWS-1); z>=0; z--)
		{
			driver.setRow(z, 0);
			driver.write();
			delay(50);
			driver.setRow(z, MAX_VALUE);
			driver.write();
		}
		delay(50);

	}

	//
	// BREAK
	//

	// moves blank row from middle out to top and bottom
	for(i=0; i<4; i++)
	{
		j = 21;
		k = 21;
		// Turn all on
		driver.setAll(MAX_VALUE);
		driver.write();
		delay(35);

		// cycle blank from middle up, and middle down
		for(z=0; z<ROWS/2; z++)
		{
			driver.setRow(j, 0);
			driver.setRow(k, 0);
			driver.write();
			delay(35);
			driver.setRow(j--, MAX_VALUE);
			driver.setRow(k++, MAX_VALUE);
			driver.write();
		}

		delay(35);

		// cycle blank row from top to middle, bottom to middle
		j = 41;
		k = 0;
		for(z=(ROWS-1); z>=0; z--)
		{
			driver.setRow(j, 0);
			driver.setRow(k, 0);
			driver.write();
			delay(35);
			driver.setRow(j--, MAX_VALUE);
			driver.setRow(k++, MAX_VALUE);
			driver.write();
		}
		delay(35);
	}

	//
	// BREAK
	//

	delay(50);
	driver.clear();

	// grow each color up
	for(i=0; i<4; i++)
	{
		for(z=0; z<7; z++)
		{
			driver.setRow(z, MAX_VALUE);
			driver.setRow(z+6, MAX_VALUE);
			driver.setRow(z+12, MAX_VALUE);
			driver.setRow(z+18, MAX_VALUE);
			driver.setRow(z+24, MAX_VALUE);
			driver.setRow(z+30, MAX_VALUE);
			driver.setRow(z+36, MAX_VALUE);
			driver.write();
			delay(150);
		}
		driver.clear();
	}

	//
	// BREAK
	//

	// grow each color down
	for(i=0; i<4; i++)
	{
		for(z=7; z>=0; z--)
		{
			driver.setRow(z, MAX_VALUE);
			driver.setRow(z+6, MAX_VALUE);
			driver.setRow(z+12, MAX_VALUE);
			driver.setRow(z+18, MAX_VALUE);
			driver.setRow(z+24, MAX_VALUE);
			driver.setRow(z+30, MAX_VALUE);
			driver.setRow(z+36, MAX_VALUE);
			driver.write();
			delay(150);
		}
		driver.clear();
	}





	//
	// BREAK
	//
	driver.clear();

	// rotate columns 0->n
	for(i=0; i<2; i++)
	{
		for(j=0; j<COLS; j++)
		{
			driver.setColumn(j, MAX_VALUE);
			driver.write();
			delay(150);
			driver.setColumn(j, 0);
			driver.write();
		}
	}

	delay(100);

	//
	// BREAK
	//

	// rotate columns n->0
	for(i=0; i<2; i++)
	{
		for(z=(COLS-1); z>=0; z--)
		{
			driver.setColumn(z, MAX_VALUE);
			driver.write();
			delay(150);
			driver.setColumn(z, 0);
			driver.write();
		}
	}

	delay(100);


	// stack from bottom to top
	k = 0;
	while( k < ROWS )
	{
		for(j=(ROWS-1); j>=0; j--)
		{
			driver.setRow(j, MAX_VALUE);
			driver.write();
			delay(15);
			if( (j-k) == 0 )
			{
				break;
			}
			driver.setRow(j, 0);
			driver.write();
		}
		k++;
	}

	driver.clear();


	// stack from top to bottom
	k = ROWS-1;
	while( k != 0 )
	{
		for(j=0; j<=ROWS; j++)
		{
			driver.setRow(j, MAX_VALUE);
			driver.write();
			delay(15);
			if( (k-j) == 0 )
			{
				break;
			}
			driver.setRow(j, 0);
			driver.write();
		}
		k--;
	}

	driver.clear();


	// progressively go faster up
	i = 100;
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
		i = i-20;
	}
	i = 20;
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
		i = i-5;
	}
	for(k=0; k<10; k++)
	{
		for(j=0; j<ROWS; j++)
		{
			driver.setRow(j, MAX_VALUE);
			driver.write();
			delay(2);
			driver.setAll(0);
			driver.write();
		}
	}

	driver.setAll(MAX_VALUE);
	driver.write();

	delay(5000);

//	while(1);









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

//
//
//
//	driver.clear();

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



//
//
//
//	// each row on then off, 5 times, from bottom to top
//	for(i=0; i<5; i++)
//	{
//		for(j=0; j<ROWS; j++)
//		{
//			driver.setRow(j, MAX_VALUE);
//			driver.write();
//			delay(25);
//			driver.setAll(0);
//			driver.write();
//		}
//	}
//
//	// each row on then off, 7 times, from top to bottom
//	for(i=0; i<7; i++)
//	{
//		for(j=0; j<ROWS; j++)
//		{
//			driver.setRow(j, MAX_VALUE);
//			driver.setRow(COLS-1-j, MAX_VALUE);
//			driver.write();
//			delay(75);
//			driver.setAll(0);
//			driver.write();
//		}
//	}
//
//	for(i=0; i<7; i++)
//	{
//		for(j=0; j<COLS; j++)
//		{
//			driver.setColumn(j, MAX_VALUE);
//			driver.setColumn(COLS-1-j, MAX_VALUE);
//			driver.write();
//			delay(75);
//			driver.setAll(0);
//			driver.write();
//		}
//	}


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


//	k = 0;
//	while( k < ROWS )
//	{
//		for(j=(ROWS-1); j>=0; j--)
//		{
//			driver.setRow(j, MAX_VALUE);
//			driver.write();
//			delay(25);
//			if( (j-k) == 0 )
//			{
//				break;
//			}
//			driver.setRow(j, 0);
//			driver.write();
//		}
//		k++;
//	}
//
//
//
//	driver.setAll(0);
//	driver.write();
//
//	k = ROWS*COLS;
//
//	while( k > 0 )
//	{
//		j = random(0, ROWS*COLS);
//		i = j/ROWS; // column
//		j = j%ROWS; // row
//
//		if( driver.getValue(j, i) == 0 )
//		{
//			driver.setValue(j, i, MAX_VALUE);
//			driver.write();
//			k--;
//			delay(25);
//		}
//	}



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

