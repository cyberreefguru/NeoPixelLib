/*
 * MotorShield.cpp
 *
 *  Created on: Apr 15, 2014
 *      Author: tsasala
 */

#include "MotorShield.h"


// Base -> In2 (D11)
// Side -> In3 (D12)
// Right Motor -> 1Y -> O1.1 -> D2
// Right Motor -> 2Y -> O1.2 -> D3
// Right Motor -> 1-2EN -> D9
// Left Motor  -> 3Y -> O2.1 -> D4
// Left Motor  -> 4Y -> O2.2 -> D5
// Left Motor  -> 3-4EN -> D10

uint8_t leds[4] = {A0,A1,A2,A3}; // Analog port
uint8_t inputs[4] = {8, 11, 12, 13}; // Digital port
uint8_t motor[2][3] = { {2, 3, 9}, {4, 5, 10} }; // Input 1, Input 2, Enable



/**
 * Constructor
 */
MotorShield::MotorShield()
{
}


/**
 * Initializes the hardware for PWM mode
 *
 */
void MotorShield::initialize()
{
	 // Set I/O to input
	uint8_t i, j;

	// set up leds outputs
	for(i=0; i<NUM_LEDS; i++)
	{
		pinMode( leds[i], OUTPUT );
	}

	// set up inputs
	for(i=0; i<NUM_INPUTS; i++)
	{
		pinMode( inputs[i], INPUT ); // set mode to input
		digitalWrite(inputs[i], HIGH ); // enable internal pull up
		inputValue[i] = 0;
		inputCount[i] = 0;
		inputChange[i] = 0;
	}

	// Setup motors
	for(i = 0; i < NUM_MOTORS; i++)
	{
		for(j = 0; j< 3; j++ )
		{
			pinMode(motor[i][j], OUTPUT); // set mode to output
			digitalWrite(motor[i][j], LOW); // set output to low
		}
	}

	// I don't know if this is needed, but since we
	// initialized the pins as digital above, I
	// figure it won't hurt
	analogWrite(motor[0][2], 0);
	analogWrite(motor[1][2], 0);


#ifdef __DEBUG
	Serial.println("INITIALIZED");
#endif

} // end initialize



/**
 * Samples the switch input and writes status to status pins
 */
void MotorShield::execIsr()
{
	uint8_t input = 0;

	for(uint8_t i=0; i<NUM_INPUTS; i++)
	{
		input = digitalRead(inputs[i]);

	    if( (input == inputValue[i]) && (inputCount[i] > 0) )
	    {
	    	inputCount[i] = inputCount[i]-1;
	    }

	    if(input != inputValue[i])
	    {
	    	inputCount[i] = inputCount[i] + 1;
	    }

	    // If the Input has shown the same value for long enough let's switch it
	    if(inputCount[i] >= DEBOUNCE_DELAY )
	    {
	    	// Trap transitions from high to low
	    	if( inputValue[i] == 1 && input == 0 )
	    	{
	    		inputChange[i] = true;
	    	}
	    	inputCount[i] = 0;
	    	inputValue[i] = input;
	    }
	}
}



/**
 * Returns current value of the specified sensor.
 *
 * Value is true or false.
 *
 */
volatile uint8_t MotorShield::getInputValue(uint8_t index)
{
	return inputValue[index];
}


//void MotorShield::togglePin( uint8_t pin, uint8_t repeat, uint16_t duration )
//{
//	uint8_t i;
//
//	for(i=0; i<repeat; i++)
//	{
//		digitalWrite(pin, LOW );
//		delay( duration );
//		digitalWrite( pin, HIGH );
//		delay( duration);
//	}
//	digitalWrite(pin, LOW );
//}


/**
 * Steps the output from initial setting n times with specified delay
 * between, where n = repeatValue.  Value set to endValue upon completion
 */
void MotorShield::step(uint8_t motorIndex, int8_t direction, uint8_t initialSpeed, uint8_t endSpeed, int8_t speedStep, uint16_t delayValue, uint16_t repeatValue)
{

	uint16_t i = 0;

	int8_t speed = initialSpeed;

	for(i=0; i<repeatValue; i++)
	{
		move(direction, motorIndex, speed);
		delay( delayValue );
		speed += speedStep;

	}

	move(direction, motorIndex, endSpeed);
}

/**
 * Moves specified motor in specified direction at specified speed for specified duration
 * and then stops the motor.
 */
void MotorShield::moveTimed(uint8_t direction, uint8_t motorIndex, uint8_t speed, uint16_t duration)
{
	move( direction, motorIndex, speed );
	delay( duration);
	stop( motorIndex );
}

/**
 * Sets output in specific direction and speed.
 *
 */
void MotorShield::move( uint8_t direction, uint8_t motorIndex, uint8_t speed)
{
	if( direction == FORWARD )
	{
		digitalWrite(motor[motorIndex][0], HIGH);
		digitalWrite(motor[motorIndex][1], LOW);
		analogWrite( motor[motorIndex][2], speed );
	}
	else if( direction == BACKWARD )
	{
		digitalWrite(motor[motorIndex][0], LOW);
		digitalWrite(motor[motorIndex][1], HIGH);
		analogWrite( motor[motorIndex][2], speed );
	}
	updateLeds( motorIndex, direction );
}

/**
 * Sets motor speed
 */
void MotorShield::setSpeed(uint8_t motorIndex, uint8_t speed)
{
	analogWrite( motor[motorIndex][2], speed );
}

/**
 * Stops motor
 */
void MotorShield::stop(uint8_t motorIndex)
{
	digitalWrite(motor[motorIndex][0], LOW);
	digitalWrite(motor[motorIndex][1], LOW);
	analogWrite( motor[motorIndex][2], 0 );
	updateLeds( motorIndex, STOP );
}



/**
 * Updates LEDs with current motor information.
 *
 * Forward means + on O1/3, - on O2/4; led1/2=on, led3/4=off
 * Backwards means - on O1/3, + on O2/4; led1/2=off, led3/4=on
 */
void MotorShield::updateLeds( uint8_t motorIndex, uint8_t direction )
{
	switch(direction)
	{
		case FORWARD:
			if( motorIndex == 0 )
			{
				setLed(LED_1, ON);
				setLed(LED_2, OFF);
			}
			else if( motorIndex == 1 )
			{
				setLed(LED_3, ON);
				setLed(LED_4, OFF);
			}
			break;
		case BACKWARD:
			if( motorIndex == 0 )
			{
				setLed(LED_1, OFF);
				setLed(LED_2, ON);
			}
			else if( motorIndex == 1 )
			{
				setLed(LED_3, OFF);
				setLed(LED_4, ON);
			}
			break;
		case STOP:
			if( motorIndex == 0 )
			{
				setLed(LED_1, OFF);
				setLed(LED_2, OFF);
			}
			else if( motorIndex == 1 )
			{
				setLed(LED_3, OFF);
				setLed(LED_4, OFF);
			}
			break;
		default:
			break;
	}
}

/**
 * sets specified LED to specified value
 *
 */
void MotorShield::setLed(uint8_t ledIndex, uint8_t value)
{
	if(ledIndex >= 0 && ledIndex < NUM_LEDS )
	{
		if( value == OFF )
		{
			digitalWrite(leds[ledIndex], false);
		}
		else if(value == ON)
		{
			digitalWrite(leds[ledIndex], true);
		}
	}
}
