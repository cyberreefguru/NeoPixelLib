// Do not remove the include below
#include "Test5947.h"


Adafruit_TLC5947 tlc = Adafruit_TLC5947(1, ROW_CLOCK, ROW_DATA, ROW_LATCH);
HighSideDriver hsd = HighSideDriver( 1, COL_CLOCK, COL_DATA, COL_LATCH, COL_CLEAR, COL_OE);

void ramp(uint8_t led);
void flash();

//The setup function is called once at startup of the sketch
void setup()
{
//	Serial.begin(115200);
//	Serial.println("TLC5974 test");

	tlc.begin();
	digitalWrite( ROW_CLEAR, HIGH ); // blank TLC

	// Clear all channels
	for(uint8_t i=0; i<24; i++)
	{
		tlc.setPWM(i, 0);
	}
	tlc.write();

	// Clear all channels
	hsd.begin();
	hsd.setValue(0, 0x00);
	hsd.write();

}

// The loop function is called in an endless loop
void loop()
{

	uint8_t i;

	hsd.setValue(0, 0x1f); // turn on all columns
	hsd.write();

	digitalWrite( ROW_CLEAR, LOW ); // unblank TLC


	for(i=0; i<5; i++)
	{
		tlc.setPWM(i, 4095);
		tlc.write();
		delay(500);
	}

	for(i=0; i<5; i++)
	{
		tlc.setPWM(i, 0);
		tlc.write();
		delay(500);
	}

	for(i=0; i<5; i++)
	{
		tlc.setPWM(i, 4095);
		tlc.write();
		delay(500);
	}

	hsd.setValue(0, 0x1e);
	hsd.write();
	delay(500);

	hsd.setValue(0, 0x1c);
	hsd.write();
	delay(500);

	hsd.setValue(0, 0x18);
	hsd.write();
	delay(500);

	hsd.setValue(0, 0x10);
	hsd.write();
	delay(500);

	hsd.setValue(0, 0x00);
	hsd.write();
	delay(500);

}

void lsTest()
{

	for(uint16_t i=0; i<8; i++)
	{
		ramp(i);
		delay(300);
	}

	for(uint16_t i=0; i<8; i++)
	{
		tlc.setPWM(i, 0);
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

	tlc.setPWM(0, 4095);
	tlc.setPWM(1, 4095);
	tlc.setPWM(2, 4095);
	tlc.setPWM(3, 0);
	tlc.setPWM(4, 0);
	tlc.setPWM(5, 0);
	tlc.write();

	delay(100);

	tlc.setPWM(0, 0);
	tlc.setPWM(1, 0);
	tlc.setPWM(2, 0);
	tlc.setPWM(3, 4095);
	tlc.setPWM(4, 4095);
	tlc.setPWM(5, 4095);
	tlc.write();

	delay(500);
}

void ramp(uint8_t led)
{
	uint16_t i;

	for(uint16_t i=0; i<4096; i++)
	{
		tlc.setPWM(led, i);
		tlc.write();
	}
}
