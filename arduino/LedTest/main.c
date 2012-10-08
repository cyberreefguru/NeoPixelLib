/*
 * main.c
 *
 *  Created on: Dec 26, 2011
 *      Author: Thomas M. Sasala
 *
 *  Test for LED sequencing.  Connection to Port B, pins 9,10,11 (red, green, yellow)
 *  Active low signal; black=VCC, R/G/Y = GND
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

#include "LedTest.h"

/**
 * Function stereotypes
 */
void initialize();
void testOutput();
void flash();
void sample();
void debounceInput( uint8_t pin, volatile uint8_t *currentValue, volatile uint8_t *currentCount, volatile uint8_t *change );
void togglePin( uint8_t pin, uint8_t repeat, uint16_t duration );
void playSequence(uint8_t repeat, uint8_t seqLen, uint16_t delayValue, uint8_t* seq);
void playSeq(SEQUENCE *seq);


volatile uint8_t currentBaseValue;
volatile uint8_t currentSideValue;
volatile uint8_t baseCount;
volatile uint8_t sideCount;
volatile uint8_t baseChange;
volatile uint8_t sideChange;

#define DEBOUNCE_DELAY	5
#define ROTATE_DELAY 	250

PATTERN p1 = { OFF, 250 };
PATTERN p2 = { RED, 500 };
PATTERN p3 = { GREEN, 500 };
PATTERN p4 = { YELLOW, 500 };

PATTERN p5 = { RED|GREEN, 500 };
PATTERN p6 = { GREEN|YELLOW, 500 };
PATTERN p7 = { RED|YELLOW, 500 };

PATTERN s01[] = { p2, p3, p4 };
PATTERN s02[] = { p5, p6, p7 };

SEQUENCE s1 = { 3, 10, p1, p1, s01 };
SEQUENCE s2 = { 3, 10, p1, p1, s02 };

// Format: Length, Repeat, {initial patter, duration}, {pattern, duration} x Length, {end pattern, duration}
//uint8_t seq0[] = { 3, 1, OFF, 250, RED, 500, GREEN, 500, YELLOW, 500, OFF, 250 };
//uint8_t seq1[] = { 3, 10, OFF, 0, RED|GREEN, 1000, GREEN|YELLOW, 1000, RED|YELLOW, 1000, OFF, 0 };
//uint8_t seq2[] = { 0x05, 0x03, 0x09 };
//uint8_t seq3[] = { 0x0d, 0x0b, 0x07 };
//uint8_t seq4[] = { 0x07, 0x0b, 0x0d };
//uint8_t seq5[] = { 0x00, 0x0F };
//uint8_t seq7[] = { 0x0D, 0x0B };
//uint8_t seq8[] = { 0x0D, 0x07 };
//uint8_t seq9[] = { 0x0B, 0x07 };

/**
 * Main application
 *
 */
int main(void)
{
	uint8_t led_pattern;

	// Initialize libraries
	init();

	// Initialize hardware
	initialize();

#ifdef __DEBUG
	// Notify of program start
	Serial.println("PROGRAM START");
#endif

	playSeq( &s1);
	playSeq( &s2);
	while(1);
//	playSequence( 10, 3, ROTATE_DELAY, seq1);
//	playSequence( 10, 3, ROTATE_DELAY, seq2);
//	playSequence( 10, 3, ROTATE_DELAY, seq3);
//	playSequence( 10, 3, ROTATE_DELAY, seq4);
//	playSequence( 10, 2, ROTATE_DELAY, seq5);
//	playSequence( 10, 2, ROTATE_DELAY, seq7);
//	playSequence( 10, 2, ROTATE_DELAY, seq8);
//	playSequence( 10, 2, ROTATE_DELAY, seq9);
//
//	while(1);

#ifdef __DEBUG
	// never get here, but...
	Serial.println("PROGRAM END");
#endif

	// Just in case
	while(1);

} // end main


void togglePin( uint8_t pin, uint8_t repeat, uint16_t duration )
{
	uint8_t i;

	for(i=0; i<repeat; i++)
	{
		digitalWrite(pin, LOW );
		delay( duration );
		digitalWrite( pin, HIGH );
		delay( duration);
	}
	digitalWrite(pin, LOW );
}

void playSequence(uint8_t repeat, uint8_t seqLen, uint16_t delayValue, uint8_t* seq)
{
	uint8_t i,j;

	uint8_t len;
	uint8_t repeatValue;
//	uint8_t returnIndex;
//	uint8_t duration;
	uint8_t index = 2;

	len = seq[0];
	repeatValue = seq[1];

	// Initial sequence
	PORTB = ~seq[index++];
	delay(seq[index++]);

	for(i=0; i<repeatValue; i++)
	{
		for(j=0; j<len; j++)
		{
			PORTB = ~seq[index++];
			delay(seq[index++]);
		}
		index = 4;
	}

	// Compute end index
	index = (len*2)+4;

	// End sequence
	PORTB = ~seq[index++];
	delay(seq[index++]);

}

void playSeq(SEQUENCE *seq)
{
	uint8_t i,j;

	// Initial sequence
	PORTB = ~seq->initialPattern.pattern;
	delay(seq->initialPattern.duration);

	for(i=0; i<seq->repeat; i++)
	{
		for(j=0; j<seq->len; j++)
		{
			PORTB = ~seq->patterns[j].pattern;
			delay(seq->patterns[j].duration);
		}
	}

	// End sequence
	PORTB = ~seq->finalPattern.pattern;
	delay(seq->finalPattern.duration);

}


/**
 * Initializes the hardware for PWM mode
 *
 */
void initialize()
{
#ifdef __DEBUG
	// Initialize hardware serial port
	Serial.begin(115200);
	Serial.println("INITIALIZING");
#endif

	 // Set I/O to input
	pinMode( LED_PIN, OUTPUT );

	// Set outputs
	digitalWrite(LED_PIN, LOW);

	// Setup leds
	uint8_t i;
	for(i = 0; i < 3; i++)
	{
		pinMode(led[i], OUTPUT);
		digitalWrite(led[i], 255);
	}


#ifdef __DEBUG
	Serial.println("INITIALIZED");
#endif

} // end initialize

extern "C" void __cxa_pure_virtual()
{
	cli();
	for (;;);
}

