// Do not remove the include below
#include "PowerTowerLights.h"

Tlc5947Driver tlc = Tlc5947Driver();
HighSideDriver hsd = HighSideDriver();

void lsTest();
void hsTest();
void ramp(uint8_t led);
void flash();

#define WAIT 50

void LEDscan(float degreeoffset);


//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(115200);
//	Serial.println("TLC5974 test");
//
//	delay(10000);

//	Serial.println("STARTING");

	tlc.initialize(1, ROW_CLOCK, ROW_DATA, ROW_LATCH, ROW_CLEAR, -1);
	tlc.setBlank(true);
	tlc.clear();

	// Clear all channels
	hsd.initialize(1, COL_CLOCK, COL_DATA, COL_LATCH, COL_CLEAR, COL_OE);
	hsd.setValue(0, 0x00);
	hsd.write();

}

// The loop function is called in an endless loop
void loop()
{
	uint8_t i;
	float offset = 0;

	hsd.setValue(0, 0x1f); // turn on all columns
	hsd.write();
	tlc.setBlank(false);
	tlc.clear();

	while(1)
	{
	for (offset = 0; offset < 360; offset += 0.5) {
	    LEDscan(offset);
	    delay(2);
	  }
	}


	for (i = 0; i < 5; i++)
	{
		ramp(i);
		delay(300);
	}

//	Serial.println("turning all on");
	for (i = 0; i < 5; i++)
	{
		tlc.setIntensity(i, 4095);
		tlc.write();
		delay(WAIT);
	}

//	Serial.println("turning all off");
	for (i = 0; i < 5; i++)
	{
		tlc.setIntensity(i, 0);
		tlc.write();
		delay(WAIT);
	}

//	Serial.println("turning all on");
	for (i = 0; i < 5; i++)
	{
		tlc.setIntensity(i, 4095);
		tlc.write();
		delay(WAIT);
	}

	//	Serial.println("flashing");
	for (i = 0; i < 4; i++)
	{
		tlc.setAll(0);
		delay(WAIT);
		tlc.setAll(MAX_INTENSITY);
		delay(WAIT);
	}

//	Serial.println("turning 1e");
	hsd.setValue(0, 0x1e);
	hsd.write();
	delay(WAIT);

//	Serial.println("turning 1c");
	hsd.setValue(0, 0x1c);
	hsd.write();
	delay(WAIT);

//	Serial.println("turning 18");
	hsd.setValue(0, 0x18);
	hsd.write();
	delay(WAIT);

//	Serial.println("turning 10");
	hsd.setValue(0, 0x10);
	hsd.write();
	delay(WAIT);

//	Serial.println("turning 00");
	hsd.setValue(0, 0x00);
	hsd.write();
	delay(WAIT);

	tlc.clear();

} // end loop

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
//		initial = micros();
		tlc.write();
//		final = micros();
//		Serial.print("write time: ");
//		Serial.println( (final-initial) );
	}
}

// Sample function to draw a scanning pattern with fading
void LEDscan(float degreeoffset)
{

	float brightnessfactor = 0;

	float scanindex = (1.0 + sin(degreeoffset * 3.14159 / 180.0)) * (float)2.0;

	for (uint8_t LEDindex = 0; LEDindex < 5; LEDindex++)
	{

		brightnessfactor = exp(
				0.0 - fabs(scanindex - ((float) LEDindex + 0.5)) * 1.3);

		tlc.setIntensity(LEDindex, (uint16_t)(4095 * brightnessfactor) );
	}

	tlc.write();
}

