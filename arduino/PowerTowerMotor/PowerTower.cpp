/*
 * PowerTower.cpp
 *
 *  Created on: Apr 15, 2014
 *      Author: tsasala
 */

#include "PowerTower.h"

MotorShield shield;
InterBoardComm comm;

void sample();
void error(uint8_t code);
void moveToTop(uint8_t speed);
void moveToBottom(uint8_t speed);

//The setup function is called once at startup of the sketch
void setup()
{
	uint8_t home = false;

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

	// Initialize communication
	comm.initialize( MASTER_ADDRESS );

	// Initialize timer (1ms interval)
	FlexiTimer2::set(1, sample);

	// Start the timer
	FlexiTimer2::start();

	// pause - let timer start and gather some data before
	// trying to use the sensor data
	delay(250);

	// if craddle not at bottom, error out
	if( shield.getInputValue(BOTTOM_SENSOR) == true )
	{
		shield.moveTimed( UP, 0, 200, 200);
		moveToBottom(60);

		if( shield.getInputValue(BOTTOM_SENSOR) == true )
		{
			error(0x01);
		}
	}

//	comm.waitForResponse(TOWER_LIGHT_ADDRESS, RESPONSE_RESET_COMPLETE, 1, buffer);


#ifdef __DEBUG
	Serial.println("INITIALIZE END");
#endif

}

// The loop function is called in an endless loop
void loop()
{
	delay(2000);

	moveToTop(230);

	delay(3500);

	// down
	shield.moveTimed(DOWN, 0, 120, 550);
	delay(1250);

	// up
	shield.moveTimed(UP, 0, 255, 400);
	delay(500);

	// down
	shield.moveTimed(DOWN, 0, 150, 400);
	delay(500);


	shield.moveTimed(UP, 0, 255, 400);
	delay(1500);

	// down
	shield.moveTimed(DOWN, 0, 150, 500);
	delay(250);

	shield.moveTimed(UP, 0, 255, 750);
	delay(250);

	// down
	shield.moveTimed(DOWN, 0, 150, 500);
	delay(1000);


	moveToTop(255);
	delay(1500);

	shield.moveTimed(DOWN, 0, 150, 700);
	delay(1000);

	moveToBottom(40);

	delay(10000); // wait 10 seconds

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
		if( code && (0x1<<i))
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




