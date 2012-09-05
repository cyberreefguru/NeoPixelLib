/*
 * main.c
 *
 *  Created on: Dec 26, 2011
 *      Author: Thomas M. Sasala
 *
 *  Controller for Faller Top Spin
 *
 * Copyright (c) 2011 Thomas M. Sasala.  All right reserved.
 *
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 * To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/
 * or send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 *
 */

#include "SequencePlayer.h"

// Defines the number of pins used
#define NUMBER_OF_PINS	4
#define NUMBER_OF_LEDS	(NUMBER_OF_PINS*(NUMBER_OF_PINS -1) )


//define pins used to drive LEDs
byte pins[] = {8,9,10,11};

// Mapping of LED # to PIN #
uint8_t led_to_pin[NUMBER_OF_LEDS][2] = {
		{pins[0], pins[1]}, {pins[1], pins[2]}, {pins[2], pins[3]},
		{pins[3], pins[2]}, {pins[2], pins[1]}, {pins[1], pins[0]},
		{pins[0], pins[2]}, {pins[2], pins[0]}, {pins[0], pins[3]},
		{pins[3], pins[0]}, {pins[1], pins[3]}, {pins[3], pins[1]}
};

#define FRAME_0 	0x0000
#define FRAME_1 	0x0001
#define FRAME_2 	0x0002
#define FRAME_3 	0x0004
#define FRAME_4 	0x0008
#define FRAME_5 	0x0010
#define FRAME_6 	0x0020
#define FRAME_7 	0x0040
#define FRAME_8 	0x0080
#define FRAME_9 	0x0100
#define FRAME_10 	0x0200
#define FRAME_11 	0x0400
#define FRAME_12 	0x0800
#define FRAME_13 	0x1000
#define FRAME_14 	0x2000
#define FRAME_15 	0x4000
#define FRAME_16 	0x8000

#define LED_OFF	0x0000
#define LED1 	0x0001
#define LED2 	0x0002
#define LED3 	0x0004
#define LED4 	0x0008
#define LED5 	0x0010
#define LED6 	0x0020
#define LED7 	0x0040
#define LED8 	0x0080
#define LED9 	0x0100
#define LED10 	0x0200
#define LED11 	0x0400
#define LED12 	0x0800
#define LED13 	0x1000
#define LED14 	0x2000
#define LED15 	0x4000
#define LED16 	0x8000
#define LED_ON	0x0FFF


// {repeat, holdBefore, holdAfter, frameData }
Pattern pattern1 = {1, 0, 250, FRAME_0};
Pattern pattern2 = {1, 0, 250, FRAME_1};
Pattern pattern3 = {1, 0, 250, FRAME_2};
Pattern pattern4 = {1, 0, 250, FRAME_3};
Pattern pattern5 = {1, 0, 1000, FRAME_4};

Pattern hb1 = {1, 0, 200, FRAME_1};
Pattern hb2 = {1, 0, 125, FRAME_0};
Pattern hb3 = {1, 0, 200, FRAME_1};
Pattern hb4 = {1, 0, 1250, FRAME_0};

Pattern heartBeat[4] = { {1, 0, 200, FRAME_1}, {1, 0, 125, FRAME_0}, {1, 0, 200, FRAME_1}, {1, 0, 1250, FRAME_0} };

Sequence heartBeatSeq = { 2, heartBeat };

uint8_t mapPort(uint8_t pattern);
void roll(uint16_t pattern, uint8_t direction, uint16_t wait, uint8_t repeat);
void rotate(uint16_t pattern, uint8_t direction, uint16_t wait, uint8_t numLeds);
void playHeartbeat(uint16_t pattern, uint16_t repeat, uint16_t hbOff, uint16_t hbOn, uint16_t hbPause);
void stack(uint8_t direction, uint16_t wait, uint8_t numLeds);
void set(uint8_t led);
void unset(uint8_t led);
void pattern(uint16_t led);
void enableRow(uint8_t row);
void set(uint8_t row, uint8_t col, boolean flag);
uint8_t mapRow(uint8_t row);


/**
 * Main application
 *
 */
int main(void)
{
	uint8_t i, j;
	uint16_t temp;
	uint8_t row, col, data;

	// Initialize libraries
	init();

	data = 0xFD	;

	DDRD = DDRD & 0x03; // clear direction
	DDRD = DDRD | 0xFC; // most outputs to drive

	for(i=1;i<8; i++)
	{
		enableRow(i);
		delay(250);
	}

	i = 0xff;
	i &= ~(0x02<<2);

	// Turn on 2,1
	PORTD = (PORTD & 0x03) | i;


#ifdef __DEBUG
	Serial.println("PROGRAM END");
#endif

	// Hold forever
	while(1);

} // end main

uint8_t mapPort(uint8_t pattern)
{
	return (pattern << 2);
}

uint8_t mapRow(uint8_t row)
{
	uint8_t rowPort = 0x00;

	switch( row )
	{
		case 1:
			rowPort = 0x01<<2;
			break;
		case 2:
			rowPort = 0x02<<2;
			break;
		case 3:
			rowPort = 0x04<<2;
			break;
		case 4:
			rowPort = 0x08<<2;
			break;
		case 5:
			rowPort = 0x10<<2;
			break;
		case 6:
			rowPort = 0x20<<2;
			break;
	}
	return rowPort;
}



void set(uint8_t row, uint8_t col, boolean flag)
{
	uint8_t matrix[6][5];
	uint8_t rows[6];
	uint8_t port;

	uint8_t rowPort, colPort;
	uint8_t i,j;

	port = 0x00;
	rowPort = 0x00;
	rowPort = mapRow( row );
	colPort = mapRow( col );

	if( row == col )
	{
		colPort = colPort < 1;
	}

	port = 0xff & ~colPort;
	PORTD = (PORTD & 0x03) | port;
}

void enableRow(uint8_t row)
{
	uint8_t rowPort = 0x00;
	uint8_t colPort = 0x00;

	rowPort = mapRow( row );
	colPort = ~rowPort;

	PORTD = PORTD & (colPort | 0x03);
	PORTD = PORTD | colPort;
}

void pattern(uint16_t led)
{
	CharliePort.setPattern( led );
}


void set(uint8_t led)
{
	CharliePort.setPattern( CharliePort.getPattern() | (1<< (led-1)) );
}

void unset(uint8_t led)
{
	CharliePort.setPattern( CharliePort.getPattern() & ~(1<< (led-1)) );
}


void roll(uint16_t pattern, uint8_t direction, uint16_t wait, uint8_t repeat)
{
	uint16_t i, temp;

	CharliePort.setPattern(pattern);
	for(i=0; i<repeat; i++)
	{
		delay(wait);
		if( direction == LEFT )
		{
			temp = CharliePort.getPattern() << 1;
			if( (temp & 0x1000) == 0x1000)
			{
				temp |= 0x0001;
			}
			CharliePort.setPattern(temp);
		}
		else
		{
			temp = CharliePort.getPattern() >> 1;
			if( (CharliePort.getPattern() & 0x0001) == 0x01)
			{
				temp |= 0x0800;
			}
			CharliePort.setPattern(temp);
		}
	}
}

void rotate(uint16_t pattern, uint8_t direction, uint16_t wait, uint8_t repeat)
{
	uint16_t i;
	CharliePort.setPattern(pattern);
	for( i=0; i<repeat; i++)
	{
		if( direction == LEFT )
		{
			CharliePort.setPattern( CharliePort.getPattern() << 1 );
		}
		else
		{
			CharliePort.setPattern( CharliePort.getPattern() >> 1 );
		}
		delay(wait);
	}
}


void playHeartbeat(uint16_t pattern, uint16_t repeat, uint16_t hbOff, uint16_t hbOn, uint16_t hbPause)
{
	uint16_t i;

	if( repeat > 0 )
	{
		for(i=0; i<repeat; i++)
		{
			CharliePort.setPattern(pattern);
			delay( hbOn );
			CharliePort.setPattern(0x00);
			delay(hbPause);
			CharliePort.setPattern(pattern);
			delay( hbOn );
			CharliePort.setPattern( 0x00 );
			delay( hbOff );
		}
	}
}

void stack(uint8_t direction, uint16_t wait, uint8_t numLeds)
{
	uint16_t i, j, k, temp, pattern;

	pattern = 0x0000;
	j = numLeds;

	for(i=0; i<numLeds; i++)
	{
		temp = 0x0001;
		for(k=0; k<j; k++)
		{
			if( direction == RIGHT )
			{
				temp = 0x01 << ( (numLeds-1) - k);
			}
			else
			{
				temp = 0x01 << k;
			}

			CharliePort.setPattern( (temp | pattern) );
			delay( wait );
		}
		pattern |= temp;
		j--;
//		delay( wait );
	}
}

extern "C" void __cxa_pure_virtual()
{
	cli();
	for (;;);
}

