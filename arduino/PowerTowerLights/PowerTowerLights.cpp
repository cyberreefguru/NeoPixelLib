/*
 * PowerTowerLights.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: tsasala
 */
#include "PowerTowerLights.h"

LedShieldDriver driver = LedShieldDriver();
void isr();

#define WAIT 50

const int led_pin = 13;			// default to pin 13

void error(uint8_t errorCode);
void LEDscan(float degreeoffset);

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
	if( driver.initialize(ROWS, COLS) == false )
	{
		error(10);
	}

#ifdef __DEBUG
	Serial.print("free=");
	Serial.println(freeRam());
#endif

#ifdef __DEBUG
	Serial.println("Initializing ls driver");
#endif
	if (driver.initializeLowSideDriver(1, ROW_CLOCK, ROW_DATA, ROW_LATCH, ROW_CLEAR, -1) == false)
	{
		error(30);
	}
#ifdef __DEBUG
	Serial.print("free=");
	Serial.println(freeRam());
#endif

#ifdef __DEBUG
	Serial.println("Initializing hs driver");
#endif
	if( driver.initializeHighSideDriver(1, COL_CLOCK, COL_DATA, COL_LATCH, COL_CLEAR, COL_OE) == false)
	{
		error(50);
	}
#ifdef __DEBUG
	Serial.print("free=");
	Serial.println(freeRam());
#endif

	// clear hs and ls drivers; fills buffers with zeros
#ifdef __DEBUG
	Serial.println("Clearing");
#endif
	driver.clearAll();

#ifdef __DEBUG
	Serial.println("Initializing timer");
#endif
	FlexiTimer2::set(8, 0.0001, isr ); // 800us period
	FlexiTimer2::start();

#ifdef __DEBUG
	Serial.println("**** init complete *****");
#endif

	delay(2500);

	driver.setColumn(0, 4095);
	driver.write();

	while(1);
}

/**
 * Standard arduino loop.  Called repeatedly
 *
 */
void loop()
{
	uint8_t i, j;

#ifdef __DEBUG
	Serial.print("free=");
	Serial.println(freeRam());
#endif

//#ifdef __DEBUG
//	Serial.println("Calling crossfade");
//#endif

	for(j=0; j<8; j++)
	{
		driver.setColumn(j, 4095);
		driver.write();
		delay(1000);
		driver.setAll(0);
		driver.write();
//		for(i=0; i<12; i++)
//		{
//			driver.setValue(i,j,4095);
//			driver.write();
//			delay(50);
//		}
//		driver.setColumn(j, 0);
	}


//	for(i=0; i<8; i++)
//	{
//		for(j=0; j<6; j++)
//		{
//			driver.setValue(j, i, 4095);
//			driver.write();
//			delay(250);
//			driver.setValue(j,i,0);
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
	float scanindex = (1.0 + sin(degreeoffset * 3.14159 / 180.0)) * (float) 2.0;

	for (uint8_t LEDindex = 0; LEDindex < 5; LEDindex++)
	{
		brightnessfactor = exp(0.0 - fabs(scanindex - ((float) LEDindex + 0.5)) * 1.3);
//		driver.getLowSideDriver().setIntensity(LEDindex, (uint16_t) (4095 * brightnessfactor) );
		driver.setValue(LEDindex, LEDindex, (uint16_t)(4095*brightnessfactor));
	}

//	driver.getLowSideDriver().write();
	driver.write();
}

