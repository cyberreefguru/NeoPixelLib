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

#include <FlexiTimer2.h>

#define LED_PIN		13

#define NUMBER_OF_PINS	4
#define NUMBER_OF_LEDS	(NUMBER_OF_PINS*(NUMBER_OF_PINS -1) )

#define BIT_1_MASK	b01

//define pins used to drive LEDs
byte pins[] = {8,9,10,11};

uint16_t pattern; // pattern to be written
uint8_t enableOutput;
volatile uint16_t patternBuffer; // buffered copy of pattern
volatile uint8_t patternIndex; // current bit that is being driven
volatile uint8_t patternChanged; // flag indicating pattern copy needs updating

uint8_t led_drive_pattern[NUMBER_OF_LEDS][2] = {
		{pins[0], pins[1]}, {pins[1], pins[2]}, {pins[2], pins[3]},
		{pins[3], pins[2]}, {pins[2], pins[1]}, {pins[1], pins[0]},
		{pins[0], pins[2]}, {pins[2], pins[0]}, {pins[0], pins[3]},
		{pins[3], pins[0]}, {pins[1], pins[3]}, {pins[3], pins[1]}
};


void initialize();
void drive(uint8_t flag);
void drivePattern();
void drivePattern2();
void setPattern(uint16_t value);

/**
 * Main application
 *
 */
int main(void)
{
	uint8_t i, j;
	uint16_t temp;

	// Initialize libraries
	init();

	// Initialize hardware
	initialize();

#ifdef __DEBUG
	// Notify of program start
	Serial.println("PROGRAM START");
#endif

	setPattern(0x01);
	drive(true);

	for( i=0; i<NUMBER_OF_LEDS; i++)
	{
		setPattern( pattern << 1 );
		delay(100);
	}
	setPattern( 0x1000);
	for( i=0; i<NUMBER_OF_LEDS; i++)
	{
		setPattern( pattern >> 1 );
		delay(100);
	}

	for( i=0; i<6; i++)
	{
		setPattern( 0x5555);
		delay(125);
		setPattern( 0xAAAA);
		delay(125);
	}

	for(i=0; i<5; i++)
	{
		for(j=0; j<NUMBER_OF_LEDS; j++)
		{
			setPattern( 0x01 << j );
			delay(20);
		}
	}

	for(i=0; i<5; i++)
	{
		for(j=0; j<NUMBER_OF_LEDS; j++)
		{
			setPattern( 0x0800 >> j );
			delay(20);
		}
	}

	setPattern(0x0249);
	for(i=0; i<50; i++)
	{
		for(j=0; j<3; j++)
		{
			delay(125);
			temp = pattern << 1;
			if( (temp & 0x1000) == 0x1000)
			{
				temp |= 0x0001;
			}
			setPattern(temp);
		}
	}

	setPattern(0x0249);
	for(i=0; i<50; i++)
	{
		for(j=0; j<3; j++)
		{
			delay(125);
			temp = pattern >> 1;
			if( (pattern & 0x0001) == 0x01)
			{
				temp |= 0x0800;
			}
			setPattern(temp);
		}
	}

	drive(false);

//	setPattern(0x0);

#ifdef __DEBUG
	// never get here, but...
	Serial.println("PROGRAM END");
#endif

	// Just in case
	while(1);

} // end main


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

	 // Set pin 13 to output
	pinMode( LED_PIN, OUTPUT );
	digitalWrite(LED_PIN, LOW);

	// Setup port
	DDRB &= 0xF0;

	pattern = 0;
	patternBuffer = 0;
	patternIndex = 0;
	patternChanged = false;
	enableOutput = false;

	// Initialize timer (1ms interval)
	FlexiTimer2::set(1, drivePattern2);

	// Start the timer
//	FlexiTimer2::start();

#ifdef __DEBUG
	Serial.println("INITIALIZED");
#endif

} // end initialize

/**
 * Enables or disables driving of pattern.
 * NOTE: Blanks pattern before stopping
 */
void drive(uint8_t flag)
{
	if( flag == false)
	{
		DDRB &= 0xF0;
	}
	enableOutput = flag;
}


/**
 * Sets the pattern
 */
void setPattern(uint16_t value)
{
	pattern = value;
	patternChanged = true;
}

/**
 * Drives the pattern to LEDs
 */
void drivePattern()
{
//	uint8_t i;

	if( enableOutput == true )
	{
		// See if pattern has changed and update copy when appropriate
		if( patternChanged == true && patternIndex == 0 )
		{
			patternBuffer = pattern;
			patternChanged = false;
		}

		// Clear current output value
		DDRB &= 0xF0;

		// If our current bit is a 1, drive it, otherwise, skip
		if( ((patternBuffer>>patternIndex) & 0x01) == 0x01 )
		{
			// Drive LED
			pinMode( led_drive_pattern[patternIndex][0], OUTPUT);
			pinMode( led_drive_pattern[patternIndex][1], OUTPUT);
			digitalWrite( led_drive_pattern[patternIndex][0], HIGH );
			digitalWrite( led_drive_pattern[patternIndex][1], LOW );
		}

		// Increment counter
		patternIndex=patternIndex+1;

		// Reset index counter
		if( patternIndex == NUMBER_OF_LEDS )
		{
			patternIndex = 0;
		}

	} // end enableOutput true

} // end drivePattern


extern "C" void __cxa_pure_virtual()
{
	cli();
	for (;;);
}

