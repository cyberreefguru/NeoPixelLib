// Do not remove the include below
#include "PowerTowerLights.h"


Tlc5947Driver tlc = Tlc5947Driver(1, ROW_CLOCK, ROW_DATA, ROW_LATCH, ROW_CLEAR, -1);
HighSideDriver hsd = HighSideDriver(1, COL_CLOCK, COL_DATA, COL_LATCH, COL_CLEAR, COL_OE);

void lsTest();
void hsTest();
void ramp(uint8_t led);
void flash();


//The setup function is called once at startup of the sketch
void setup()
{
//	Serial.begin(115200);
//	Serial.println("TLC5974 test");
//
//	delay(10000);

//	Serial.println("STARTING");

	tlc.initialize();
	tlc.setBlank(true);
	tlc.clear();

	// Clear all channels
	hsd.initialize();
	hsd.setValue(0, 0x00);
	hsd.write();

}

// The loop function is called in an endless loop
void loop()
{
	uint8_t i;

	hsd.setValue(0, 0x1f); // turn on all columns
	hsd.write();
	tlc.setBlank(false);

//	Serial.println("turning all on");
	for (i = 0; i < 5; i++)
	{
		tlc.setIntensity(i, 4095);
		tlc.write();
		delay(500);
	}

//	Serial.println("turning all off");
	for (i = 0; i < 5; i++)
	{
		tlc.setIntensity(i, 0);
		tlc.write();
		delay(500);
	}

//	Serial.println("turning all on");
	for (i = 0; i < 5; i++)
	{
		tlc.setIntensity(i, 4095);
		tlc.write();
		delay(500);
	}

	//	Serial.println("flashing");
	for(i=0; i<4; i++)
	{
		tlc.setAll(0);
		delay(200);
		tlc.setAll(MAX_INTENSITY);
		delay(200);
	}


//	Serial.println("turning 1e");
	hsd.setValue(0, 0x1e);
	hsd.write();
	delay(500);

//	Serial.println("turning 1c");
	hsd.setValue(0, 0x1c);
	hsd.write();
	delay(500);

//	Serial.println("turning 18");
	hsd.setValue(0, 0x18);
	hsd.write();
	delay(500);

//	Serial.println("turning 10");
	hsd.setValue(0, 0x10);
	hsd.write();
	delay(500);

//	Serial.println("turning 00");
	hsd.setValue(0, 0x00);
	hsd.write();
	delay(500);

	tlc.clear();

} // end loop

void lsTest()
{

	for(uint16_t i=0; i<8; i++)
	{
		ramp(i);
		delay(300);
	}

	for(uint16_t i=0; i<8; i++)
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

	for(uint16_t i=0; i<4096; i++)
	{
		tlc.setIntensity(led, i);
		tlc.write();
	}
}

