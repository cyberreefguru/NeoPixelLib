/*
 * PowerTowerLights.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: tsasala
 */
#include "PowerTowerLights.h"

LedShieldDriver driver = LedShieldDriver();

#define WAIT 50

void error();

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

	delay(10000);

	Serial.print("free=");
	Serial.println(freeRam());

#ifdef __DEBUG
	Serial.begin(115200);
#endif

	Serial.print("free=");
	Serial.println(freeRam());

#ifdef __DEBUG
	Serial.println("Initializing shield driver");
#endif
	if( driver.initialize(ROWS, COLS) == false )
	{
		error();
	}

	Serial.print("free=");
	Serial.println(freeRam());

#ifdef __DEBUG
	Serial.println("Initializing ls driver");
#endif
	if (driver.initializeLowSideDriver(1, ROW_CLOCK, ROW_DATA, ROW_LATCH, ROW_CLEAR, -1) == false)
	{
		error();
	}

	Serial.print("free=");
	Serial.println(freeRam());

#ifdef __DEBUG
	Serial.println("Initializing hs driver");
#endif
	if( driver.initializeHighSideDriver(1, COL_CLOCK, COL_DATA, COL_LATCH, COL_CLEAR, COL_OE) == false)
	{
		error();
	}
	Serial.print("free=");
	Serial.println(freeRam());

	// clear hs and ls drivers; fills buffers with zeros
#ifdef __DEBUG
	Serial.println("Clearing");
#endif
	driver.clearAll();

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
	uint8_t i;
	float offset = 0;

//#ifdef __DEBUG
//	Serial.println("Enabling columns");
//#endif
//	hsd.setValue(0, 0x1f); // turn on all columns
//	hsd.write();
//
//#ifdef __DEBUG
//	Serial.println("Clearing TLC");
//#endif
//	tlc.setBlank(false);
//	tlc.clear();

	Serial.print("free=");
	Serial.println(freeRam());


	driver.getLowSideDriver().setAll(4095);
	Serial.println("Printing buffer");
	driver.getLowSideDriver().printValues();
	Serial.println("Writing values");
	driver.getLowSideDriver().write();
	Serial.println("Complete");

	Serial.print("free=");
	Serial.println(freeRam());


//	while(1);


#ifdef __DEBUG
	Serial.println("Calling crossfade");
#endif
	while (1)
	{
		for (offset = 0; offset < 360; offset += 0.5)
		{
			LEDscan(offset);
			delay(2);
		}
	}

} // end loop

/**
 * Flashes on-board LED to indicate critical failure
 */
void error()
{
	while (1)
	{
		digitalWrite(13, HIGH);
		delay(100);
		digitalWrite(13, LOW);
		delay(100);
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

