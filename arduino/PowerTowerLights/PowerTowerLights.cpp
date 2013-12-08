/*
 * PowerTowerLights.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: tsasala
 */
#include "PowerTowerLights.h"

Tlc5947Driver tlc = Tlc5947Driver();
HighSideDriver hsd = HighSideDriver();

void lsTest();
void hsTest();
void ramp(uint8_t led);
void flash();

#define WAIT 50

void error();

void LEDscan(float degreeoffset);

/**
 * Standard arduino setup.  Called once
 *
 */
void setup()
{
#ifdef __DEBUG
	Serial.begin(115200);
	Serial.println("Initializing TLC");
#endif

	if (tlc.initialize(1, ROW_CLOCK, ROW_DATA, ROW_LATCH, ROW_CLEAR,
			-1) == false)
	{
		error();
	}
	tlc.clear();

#ifdef __DEBUG
	Serial.println("Initializing 595");
#endif
	if (hsd.initialize(1, COL_CLOCK, COL_DATA, COL_LATCH, COL_CLEAR,
			COL_OE) == false)
	{
		error();
	}
	// Clear all channels
	hsd.clear();

}

/**
 * Standard arduino loop.  Called repeatedly
 *
 */
void loop()
{
	uint8_t i;
	float offset = 0;

#ifdef __DEBUG
	Serial.println("Enabling columns");
#endif
	hsd.setValue(0, 0x1f); // turn on all columns
	hsd.write();

#ifdef __DEBUG
	Serial.println("Clearing TLC");
#endif
	tlc.setBlank(false);
	tlc.clear();

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

	for (i = 0; i < 5; i++)
	{
		ramp(i);
		delay(300);
	}

#ifdef __DEBUG
	Serial.println("turning all on");
#endif
	for (i = 0; i < 5; i++)
	{
		tlc.setIntensity(i, 4095);
		tlc.write();
		delay(WAIT);
	}

#ifdef __DEBUG
	Serial.println("turning all on");
#endif
	for (i = 0; i < 5; i++)
	{
		tlc.setIntensity(i, 0);
		tlc.write();
		delay(WAIT);
	}

#ifdef __DEBUG
	Serial.println("turning all on");
#endif
	for (i = 0; i < 5; i++)
	{
		tlc.setIntensity(i, 4095);
		tlc.write();
		delay(WAIT);
	}

#ifdef __DEBUG
	Serial.println("flashing");
#endif
	for (i = 0; i < 4; i++)
	{
		tlc.setAll(0);
		delay(WAIT);
		tlc.setAll(MAX_INTENSITY);
		delay(WAIT);
	}

#ifdef __DEBUG
	Serial.println("turning 1e");
#endif
	hsd.setValue(0, 0x1e);
	hsd.write();
	delay(WAIT);

#ifdef __DEBUG
	Serial.println("turning 1c");
#endif
	hsd.setValue(0, 0x1c);
	hsd.write();
	delay(WAIT);

#ifdef __DEBUG
	Serial.println("turning 18");
#endif
	hsd.setValue(0, 0x18);
	hsd.write();
	delay(WAIT);

#ifdef __DEBUG
	Serial.println("turning 10");
#endif
	hsd.setValue(0, 0x10);
	hsd.write();
	delay(WAIT);

#ifdef __DEBUG
	Serial.println("turning 00");
#endif
	hsd.setValue(0, 0x00);
	hsd.write();
	delay(WAIT);

	tlc.clear();

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
	}
}

void lsTest()
{

	for (uint16_t i = 0; i < 8; i++)
	{
		ramp(i);
		delay(300);
	}

	for (uint16_t i = 0; i < 8; i++)
	{
		tlc.setIntensity(i, 0);
		tlc.write();
		delay(300);
	}
}

void hsTest()
{
	hsd.setValue(0, 0x01);
	hsd.write();
	delay(1000);

	hsd.setValue(0, 0x03);
	hsd.write();
	delay(1000);

	hsd.setValue(0, 0x07);
	hsd.write();
	delay(1000);

	hsd.setValue(0, 0x0F);
	hsd.write();
	delay(1000);

	hsd.setValue(0, 0);
	hsd.write();
	delay(1000);

}

void flash()
{
	digitalWrite(ROW_CLEAR, LOW);

	tlc.setIntensity(0, 4095);
	tlc.setIntensity(1, 4095);
	tlc.setIntensity(2, 4095);
	tlc.setIntensity(3, 0);
	tlc.setIntensity(4, 0);
	tlc.setIntensity(5, 0);
	tlc.write();

	delay(100);

	tlc.setIntensity(0, 0);
	tlc.setIntensity(1, 0);
	tlc.setIntensity(2, 0);
	tlc.setIntensity(3, 4095);
	tlc.setIntensity(4, 4095);
	tlc.setIntensity(5, 4095);
	tlc.write();

	delay(500);
}

void ramp(uint8_t led)
{
	uint16_t i;
	int initial = 0;
	int final = 0;

	for (uint16_t i = 0; i < 4096; i++)
	{
		tlc.setIntensity(led, i);

#ifdef __DEBUG
		initial = micros();
#endif

		tlc.write();

#ifdef __DEBUG
		final = micros();
		Serial.print("write time: ");
		Serial.println( (final-initial) );
#endif

	}
}

// Sample function to draw a scanning pattern with fading
void LEDscan(float degreeoffset)
{

	float brightnessfactor = 0;

	float scanindex = (1.0 + sin(degreeoffset * 3.14159 / 180.0)) * (float) 2.0;

	for (uint8_t LEDindex = 0; LEDindex < 5; LEDindex++)
	{

		brightnessfactor = exp(
				0.0 - fabs(scanindex - ((float) LEDindex + 0.5)) * 1.3);

		tlc.setIntensity(LEDindex, (uint16_t) (4095 * brightnessfactor));
	}

	tlc.write();
}

