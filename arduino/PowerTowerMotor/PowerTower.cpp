/*
 * PowerTower.cpp
 *
 *  Created on: Apr 15, 2014
 *      Author: tsasala
 */

#include "PowerTower.h"

MotorShield shield;

void sample();
void error(uint8_t code);
void moveToTop(uint8_t speed);
void moveToBottom(uint8_t speed);


//The setup function is called once at startup of the sketch
void setup()
{

#ifdef __DEBUG
	// Initialize hardware serial port
	Serial.begin(115200);
	Serial.println("INITIALIZING");
#endif

	shield.initialize();

	for(uint8_t i=0; i<NUM_LEDS; i++)
	{
		shield.setLed(i, ON);
		delay(100);
	}
	for(uint8_t i=0; i<NUM_LEDS; i++)
	{
		shield.setLed(i, OFF);
		delay(100);
	}
	for(uint8_t j=0; j<4; j++)
	{
		for(uint8_t i=0; i<NUM_LEDS; i++)
		{
			shield.setLed(i, ON);
		}
		delay(100);
		for(uint8_t i=0; i<NUM_LEDS; i++)
		{
			shield.setLed(i, OFF);
		}
		delay(100);
	}

	// Initialize timer (1ms interval)
	FlexiTimer2::set(1, sample);

	// Start the timer
	FlexiTimer2::start();

	// pause
	delay(250);

	// if craddle not at bottom, error out
	if( shield.getInputValue(BOTTOM_SENSOR) == true )
	{
		error(0x1);
	}


#ifdef __DEBUG
	Serial.println("INITIALIZE END");
#endif

}

// The loop function is called in an endless loop
void loop()
{

	delay(2000);

	moveToTop(225);

	delay(5000);

	// down
	shield.moveTimed(DOWN, 0, 100, 400);

	delay(750);
	shield.moveTimed(UP, 0, 250, 250);
	delay(100);
	shield.moveTimed(DOWN, 0, 125, 250);
	delay(100);
	shield.moveTimed(UP, 0, 250, 400);
	delay(100);
	moveToTop(225);
	delay(1500);
	shield.moveTimed(DOWN, 0, 125, 700);
	delay(1000);
	moveToBottom(50);

	while(1);
}

/**
 * Calls ISR to keep shield sensors up to date
 */
void sample()
{
	shield.execIsr();
}

/**
 * moves craddle to the top; stops when top sensor is triggered
 */
void moveToTop(uint8_t speed)
{
	shield.move(UP, 0, speed);
	while( shield.getInputValue(TOP_SENSOR) == true ); // wait until sensor triggered
	shield.stop(0);
}

/**
 * moves craddle to the bottom; stops when bottom sensor is triggered
 */
void moveToBottom(uint8_t speed)
{
	shield.move(DOWN, 0, speed);
	while(shield.getInputValue(BOTTOM_SENSOR) == true); // wait until sensor triggered
	shield.stop(0);
}

/**
 * flashes an error pattern
 */
void error(uint8_t code)
{
	uint8_t led[4];
	for(uint8_t i=0; i<NUM_LEDS; i++)
	{
		if( led[i] && (0x1<<i))
		{
			led[i] = ON;
		}
		else
		{
			led[i] = OFF;
		}
	}

	while(1)
	{
		for(uint8_t i=0; i<NUM_LEDS; i++)
		{
			shield.setLed(i, led[i]);
		}
		delay(100);
		for(uint8_t i=0; i<NUM_LEDS; i++)
		{
			shield.setLed(i, OFF);
		}
		delay(100);
	}
}




