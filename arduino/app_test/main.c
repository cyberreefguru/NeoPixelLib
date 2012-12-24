/*
 * main.c
 *
 *  Created on: Feb 7, 2010
 *      Author: tmsasala
 */
#include "AnimationPlayer.h"


void kitt();
void ramp_brightness(unsigned char pattern, unsigned char count, int delayTime, int direction);
void fade(int delayTime);
void set_output_value(unsigned char value);
void fast_flash();
void printProject(AnimationPlayer *player);
void testPlayer();

unsigned char brightness[BCM_SIZE]; // brightness for each LED

extern const uint8_t dim_curve[] PROGMEM;
const uint8_t dim_curve[] = {
    0,   1,   1,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,
    3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,   4,
    4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   6,   6,   6,
    6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,
    8,   8,   9,   9,   9,   9,   9,   9,   10,  10,  10,  10,  10,  11,  11,  11,
    11,  11,  12,  12,  12,  12,  12,  13,  13,  13,  13,  14,  14,  14,  14,  15,
    15,  15,  16,  16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,
    20,  20,  21,  21,  22,  22,  22,  23,  23,  24,  24,  25,  25,  25,  26,  26,
    27,  27,  28,  28,  29,  29,  30,  30,  31,  32,  32,  33,  33,  34,  35,  35,
    36,  36,  37,  38,  38,  39,  40,  40,  41,  42,  43,  43,  44,  45,  46,  47,
    48,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,
    63,  64,  65,  66,  68,  69,  70,  71,  73,  74,  75,  76,  78,  79,  81,  82,
    83,  85,  86,  88,  90,  91,  93,  94,  96,  98,  99,  101, 103, 105, 107, 109,
    110, 112, 114, 116, 118, 121, 123, 125, 127, 129, 132, 134, 136, 139, 141, 144,
    146, 149, 151, 154, 157, 159, 162, 165, 168, 171, 174, 177, 180, 183, 186, 190,
    193, 196, 200, 203, 207, 211, 214, 218, 222, 226, 230, 234, 238, 242, 248, 255,
};

//struct project {
//	unsigned int rows;
//	unsigned int cols;
//  unsigned int numberSequences;
//	sequence *sequences;
//};
//struct sequence {
//	unsigned int repeatValue;
//	unsigned int numberFrames;
//	frame *frames;
//};
//struct frame {
//	unsigned int holdDuration;
//	unsigned char frameData[][];
//};

uint8_t frameData11[8] PROGMEM = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t frameData12[8] PROGMEM = {0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t frameData13[8] PROGMEM = {0x43,0xff,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t frameData14[8] PROGMEM = {0x22,0x43,0xff,0x00,0x00,0x00,0x00,0x00};
uint8_t frameData15[8] PROGMEM = {0x11,0x22,0x43,0xff,0x00,0x00,0x00,0x00};
uint8_t frameData16[8] PROGMEM = {0x09,0x11,0x22,0x43,0xff,0x00,0x00,0x00};
uint8_t frameData17[8] PROGMEM = {0x05,0x09,0x11,0x22,0x43,0xff,0x00,0x00};
uint8_t frameData18[8] PROGMEM = {0x02,0x05,0x09,0x11,0x22,0x43,0xff,0x00};
uint8_t frameData19[8] PROGMEM = {0x00,0x02,0x05,0x09,0x11,0x22,0x43,0xff};
uint8_t frameData110[8] PROGMEM = {0x00,0x00,0x02,0x05,0x09,0x11,0x22,0x43};
uint8_t frameData111[8] PROGMEM = {0x00,0x00,0x00,0x02,0x05,0x09,0x11,0x22};
uint8_t frameData112[8] PROGMEM = {0x00,0x00,0x00,0x00,0x02,0x05,0x09,0x11};
uint8_t frameData113[8] PROGMEM = {0x00,0x00,0x00,0x00,0x00,0x02,0x05,0x09};
uint8_t frameData114[8] PROGMEM = {0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x05};
uint8_t frameData115[8] PROGMEM = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02};


extern frame frames1[15] PROGMEM;
frame frames1[15] = {
		{100, frameData11},
		{100, frameData12},
		{100, frameData13},
		{100, frameData14},
		{100, frameData15},
		{100, frameData16},
		{100, frameData17},
		{100, frameData18},
		{100, frameData19},
		{100, frameData110},
		{100, frameData111},
		{100, frameData112},
		{100, frameData113},
		{100, frameData114},
		{100, frameData115},
};

extern frame frames2[15] PROGMEM;
frame frames2[15] = {
		{110, frameData115},
		{110, frameData114},
		{110, frameData113},
		{110, frameData112},
		{110, frameData111},
		{110, frameData110},
		{110, frameData19},
		{110, frameData18},
		{110, frameData17},
		{110, frameData16},
		{110, frameData15},
		{110, frameData14},
		{110, frameData13},
		{110, frameData12},
		{110, frameData11},
};

//		{100, {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}},
//		{100, {{0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}},
//		{100, {{0x11,0x22,0x43,0xff,0x00,0x00,0x00,0x00}}},
//		{100, {{0x09,0x11,0x22,0x43,0xff,0x00,0x00,0x00}}},
//		{100, {{0x05,0x09,0x11,0x22,0x43,0xff,0x00,0x00}}},
//		{100, {{0x02,0x05,0x09,0x11,0x22,0x43,0xff,0x00}}},
//		{100, {{0x00,0x02,0x05,0x09,0x11,0x22,0x43,0xff}}},
//		{100, {{0x00,0x00,0x02,0x05,0x09,0x11,0x22,0x43}}},
//		{100, {{0x00,0x00,0x00,0x02,0x05,0x09,0x11,0x22}}},
//		{100, {{0x00,0x00,0x00,0x00,0x02,0x05,0x09,0x11}}},
//		{100, {{0x00,0x00,0x00,0x00,0x00,0x02,0x05,0x09}}},
//		{100, {{0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x05}}},
//		{100, {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02}}},

//extern frame frames2[4] PROGMEM;
//
//frame frames2[4] = {
//		{1000, {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}},
//		{1000, {{0x55,0x00,0x55,0x00,0x55,0x00,0x55,0x00}}},
//		{1000, {{0x00,0xAA,0x00,0xAA,0x00,0xAA,0x00,0xAA}}},
//		{1000, {{0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00}}}
//};

sequence sequence1 = {10, 15, frames1};
sequence sequence2 = {10, 15, frames2};
sequence sequences1[2] = {sequence1, sequence2};

//project myProject = { 1,8,2, sequences1 };

#define NUM_ROWS 1
#define NUM_COLS 8
#define NUM_SEQ 2

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

//	delay(1000);
	Serial.println("Printing frame data...");
	Serial.print(" frameData11: ");
	Serial.println( (long int)(frameData11), HEX );

	Player.initializePlayer( sequences1, NUM_ROWS, NUM_COLS, NUM_SEQ );
	Player.print();

	Serial.println("Testing Player...");
	Player.play();
	Serial.println("Player Complete.");

//	for(int i=0; i< BCM_SIZE; i++ )
//	{
//		brightness[i] = 0;
//	}
//
//	Serial.println("Initializing BCM...");
//
//	BcmPwm.initialize(OUTPUT_PORT_1);
//
//	//kitt();
//
//	Serial.println("Starting fade...");
//
//	fade(50);



////	fast_flash();
//
////	set_output_value(25);
////	bcm_enable_timer(0);

	// Just in case
	Serial.println("Complete.");
	IoPort.write_value(OUTPUT_PORT_1, 0x00);
	while(1);


}

void testPlayer()
{

	Player.initializePlayer( sequences1, 1, 8, 1 );

}

void fast_flash()
{
	BcmPwm.enableTimer(0);
	while( 1 )
	{
//		set_output_value( 255 );
//
//		delay(50);
//
//		set_output_value(128);
//
//		delay(50);
//
//		set_output_value(20);
//
//		delay(50);
//
//		set_output_value(4);
//
//		delay(50);
		set_output_value(5);
		delay(2000);
		set_output_value(4);
		delay(2000);
		set_output_value(3);
		delay(2000);
		set_output_value(2);
		delay(2000);
		set_output_value(1);
		delay(2000);
		set_output_value(0);
		delay(2000);
	}
}

void set_output_value(unsigned char value)
{
	for(int i=0; i< BCM_SIZE; i++ )
	{
		brightness[i] = value;
	}

	BcmPwm.encodeTimeslices( brightness, 0xff );  // Writes brightness values to time slices
}

void fade(int delayTime)
{
	unsigned char mask = 0xff;
	unsigned char count = 255;

	// Load initial value
	for(int i=0; i<BCM_SIZE; i++)
	{
		brightness[i] = 255;
	}
	BcmPwm.encodeTimeslices( brightness, mask );  // Writes brightness values to time slices

	BcmPwm.enableTimer(0);

	Serial.println("Fully encoded - starting fade");

	while(1)
	{
		ramp_brightness(mask, count, delayTime, -1);
		ramp_brightness(mask, count, delayTime, 1);
//		mask = mask ^ 0xff;
	}

}

void ramp_brightness_value(unsigned char mask, int delayTime, unsigned char startVal, unsigned char endVal, unsigned char direction)
{
	unsigned char currVal = startVal;

	// Load initial value
	for(int i=0; i<BCM_SIZE; i++)
	{
		brightness[i] = currVal;
	}
	BcmPwm.encodeTimeslices( brightness, mask );  // Writes brightness values to time slices
	BcmPwm.enableTimer(0);

	while(currVal != endVal)
	{
		delay(delayTime);
		currVal += direction;
		for(int i=0; i<BCM_SIZE; i++)
		{
			brightness[i] = currVal;
		}
		BcmPwm.encodeTimeslices( brightness, mask );  // Writes brightness values to time slices
	}


}


void ramp_brightness(unsigned char mask, unsigned char count, int delayTime, int direction)
{
	for(int cnt = 0; cnt < count; cnt++)
	{
		delay(delayTime);
		for(int i=0; i<BCM_SIZE; i++)
		{
			brightness[i] += direction;
		}
		BcmPwm.encodeTimeslices( brightness, mask );  // Writes brightness values to time slices
	}
}

void ramp_brightness_curve(unsigned char mask, unsigned char count, int delayTime, int direction)
{
	unsigned char index = 255;

	if( direction > 0 )
	{
		index += direction;
	}

	for(int cnt = 0; cnt < count; cnt++)
	{
		delay(delayTime);
		for(int i=0; i<BCM_SIZE; i++)
		{
			brightness[i] = pgm_read_byte(&dim_curve[index]);
		}
		BcmPwm.encodeTimeslices( brightness, mask );  // Writes brightness values to time slices
		index += direction;
	}
}

/**
 *
 */
void kitt()
{

	uint8_t slowtick = 30;
	uint8_t position = 0 ;

	BcmPwm.enableTimer(0);

	while(1)
	{
		delay(5);

		// make each of the LEDs slightly dimmer...
		for ( uint8_t index = 0 ; index < BCM_SIZE ; index++ )
		{
			if (brightness[ index ] > 0) brightness[ index ]-- ;
		}

		// once every 50 ticks, advance the head of the sweep...
		slowtick-- ;
		if (slowtick==0)
		{
			slowtick = 50;
			position++ ;
			position &= (BCM_SIZE-1); // this only works if BCM_SIZE is a power of 2
			brightness[ position ] = 255 ;
		}

		// and now re-encode all the timeslices...
		BcmPwm.encodeTimeslices( brightness, 0xff );  // Writes brightness values to time slices
	}

}

extern "C" void __cxa_pure_virtual()
{
  cli();
  for (;;);
}

