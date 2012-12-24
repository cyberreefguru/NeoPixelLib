/*
 * main.c
 *
 *  Created on: Feb 7, 2010
 *      Author: tmsasala
 */
#include "SequencePlayerApp.h"


uint8_t frameData1[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t frameData2[8] = {0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff};
uint8_t frameData3[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t frameData4[8] = {0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff};
uint8_t frameData5[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t frameData6[8] = {0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff};
uint8_t frameData7[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t frameData8[8] = {0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff};

extern pattern patterns1[8];
pattern patterns1[8] = {
		{1, 100, frameData1},
		{1, 100, frameData2},
		{1, 100, frameData3},
		{1, 100, frameData4},
		{1, 100, frameData5},
		{1, 100, frameData6},
		{1, 100, frameData7},
		{1, 100, frameData8},
};

void flash_output(int count);


#define NUM_ROWS 1
#define NUM_COLS 8

int main(void)
{
	init();

	Serial.begin(57600);

	Serial.println("Initializing IO ports...");

	IoPort.initialize_ports();

	Serial.println("Testing IO ports...");

	IoPort.test_output_ports();

	Serial.println("Clearing IO ports...");

	IoPort.write_value(OUTPUT_PORT_1, 0x00);

	delay(1000);



	// Just in case
	Serial.println("Complete.");
	IoPort.write_value(OUTPUT_PORT_1, 0x00);
	while(1);


}


void flash_output(int count)
{
	int i = 0;
	while( i<count )
	{
		// Flash here
		i++;
	}
}


extern "C" void __cxa_pure_virtual()
{
  cli();
  for (;;);
}

