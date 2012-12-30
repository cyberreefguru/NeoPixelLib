/*
 * main.c
 *
 *  Created on: Dec 26, 2011
 *      Author: tom
 *
 *  Test motor controller.
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

#include <WProgram.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

/**
 * Main routine
 */

int motor[] = {2, 3, 9}; // Input 1, Input 2, Enable
#define FORWARD 	0
#define BACKWARD 	1

void step(int8_t direction, uint8_t initialSpeed, uint8_t endSpeed, int8_t speedStep, uint16_t delayValue, uint16_t repeatValue);
void forward(uint8_t speed);
void backward(uint8_t speed);
void stop();
void setSpeed(uint8_t speed);

#define LED_PIN	13

int main(void)
{
	// Initialize libraries
	init();

	// Initialize hardware serial port
	Serial.begin(115200);

	// Print start as FYI
	Serial.println("PROGRAM START");

	pinMode(LED_PIN, OUTPUT);

	// Setup motors
	int i;
	for(i = 0; i < 2; i++)
	{
		pinMode(motor[i], OUTPUT);
		digitalWrite(motor[i], LOW);
	}
	pinMode(motor[2], OUTPUT);
	analogWrite(motor[2], 0);

	Serial.println("INITIALIZED");

	while(1)
	{
//		Serial.println("START SEQUENCE");

		step(FORWARD, 0, 255, 1, 1, 255);
		step(FORWARD, 255, 0, -1, 1, 255);
		step(BACKWARD, 0, 255, 1, 1, 255);
		step(BACKWARD, 255, 0, -1, 1, 255);

//		Serial.println("END SEQUENCE");

	} // end while

	// never get here, but...
	Serial.println("PROGRAM END");

	// Just in case
	while(1);

} // end main

void step(int8_t direction, uint8_t initialSpeed, uint8_t endSpeed, int8_t speedStep, uint16_t delayValue, uint16_t repeatValue)
{

	uint16_t i = 0;

	int8_t speed = initialSpeed;

	for(i=0; i<repeatValue; i++)
	{
		if(direction == FORWARD)
		{
			forward( speed );

		} else if( direction == BACKWARD)
		{
			backward( speed);

		}
		delay( delayValue );
		speed += speedStep;

	}

	if(direction == FORWARD)
	{
		forward( endSpeed );

	} else if( direction == BACKWARD)
	{
		backward( endSpeed );
	}


}

void setSpeed(uint8_t speed)
{
	analogWrite( motor[2], speed );
}

void forward(uint8_t speed)
{
	digitalWrite(motor[0], HIGH);
	digitalWrite(motor[1], LOW);
	analogWrite( motor[2], speed );
}

void backward(uint8_t speed)
{
	digitalWrite(motor[0], LOW);
	digitalWrite(motor[1], HIGH);
	analogWrite( motor[2], speed );
}

void stop()
{
	digitalWrite(motor[0], LOW);
	digitalWrite(motor[1], LOW);
	analogWrite( motor[2], 0 );
}


extern "C" void __cxa_pure_virtual()
{
	cli();
	for (;;);
}

