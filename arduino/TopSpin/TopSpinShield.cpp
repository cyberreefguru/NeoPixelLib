/*
 * TopSpinShield.cpp
 *
 *  Created on: Aug 20, 2013
 *      Author: tsasala
 */

#include "TopSpinShield.h"

uint8_t leds[4] = {0,1,2,3}; // Analog port (Port C)
uint8_t motor[2][3] = { {2, 3, 9}, {4, 5, 10} }; // Input 1, Input 2, Enable

TopSpinShield::TopSpinShield() {
}


volatile uint8_t TopSpinShield::getCurrentBaseValue() {
	return currentBaseValue;
}

volatile uint8_t TopSpinShield::getCurrentSideValue() {
	return currentSideValue;
}


void TopSpinShield::togglePin( uint8_t pin, uint8_t repeat, uint16_t duration )
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

/**
 * Initializes the hardware for PWM mode
 *
 */
void TopSpinShield::initialize()
{
	 // Set I/O to input
	pinMode( LED_PIN, OUTPUT );
	pinMode( BASE_INPUT_PIN, INPUT );
	pinMode( SIDE_INPUT_PIN, INPUT );

	// Set internal pull up resistor
	digitalWrite(BASE_INPUT_PIN, HIGH);
	digitalWrite(SIDE_INPUT_PIN, HIGH);

	// Set outputs
	digitalWrite(LED_PIN, LOW);

	// Setup motors
	uint8_t i, j;
	for(i = 0; i < NUM_MOTORS; i++)
	{
		for(j = 0; j< 3; j++ )
		{
			pinMode(motor[i][j], OUTPUT);
			digitalWrite(motor[i][j], LOW);
		}
	}

	// Configure LED port
	DDRC = DDRC | 0x3F; // Set PC0..5 to output
	PORTC = PORTC & 0xC0; // Set port to low


	// I don't know if this is needed, but since we
	// initialized the pins as digital above, I
	// figure it won't hurt
	analogWrite(motor[0][2], 0);
	analogWrite(motor[1][2], 0);


	// Initialize variables
	led = LOW;
	currentBaseValue = 0;
	currentSideValue = 0;
	baseCount = 0;
	sideCount = 0;
	baseChange = false;
	sideChange = false;

	// Initialize timer (1ms interval)
	FlexiTimer2::set(1, sampleInput);

	// Start the timer
	FlexiTimer2::start();

#ifdef __DEBUG
	Serial.println("INITIALIZED");
#endif

} // end initialize

/**
 * Steps the output from initial setting n times with specified delay
 * between, where n = repeatValue.  Value set to endValue upon completion
 */
void TopSpinShield::step(uint8_t motorIndex, int8_t direction, uint8_t initialSpeed, uint8_t endSpeed, int8_t speedStep, uint16_t delayValue, uint16_t repeatValue)
{

	uint16_t i = 0;

	int8_t speed = initialSpeed;

	for(i=0; i<repeatValue; i++)
	{
		if(direction == FORWARD)
		{
			forward( motorIndex, speed );

		} else if( direction == BACKWARD)
		{
			backward( motorIndex, speed);

		}
		delay( delayValue );
		speed += speedStep;

	}

	if(direction == FORWARD)
	{
		forward( motorIndex, endSpeed );

	} else if( direction == BACKWARD)
	{
		backward( motorIndex, endSpeed );
	}


}

/**
 * Moves specified motor in specified direction at specified speed for specified duration.
 */
void TopSpinShield::moveTimed(uint8_t direction, uint8_t motorIndex, uint8_t speed, uint16_t duration)
{
	move( direction, motorIndex, speed );
	delay( duration);
	stop( motorIndex );
}

/**
 * Sets output in specific direction and speed.
 *
 */
void TopSpinShield::move( uint8_t direction, uint8_t motorIndex, uint8_t speed)
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
void TopSpinShield::setSpeed(uint8_t motorIndex, uint8_t speed)
{
	analogWrite( motor[motorIndex][2], speed );
}

/**
 * Moves motor forward
 */
void TopSpinShield::forward(uint8_t motorIndex, uint8_t speed)
{
	digitalWrite(motor[motorIndex][0], HIGH);
	digitalWrite(motor[motorIndex][1], LOW);
	analogWrite( motor[motorIndex][2], speed );
	updateLeds( motorIndex, FORWARD );
}

/**
 * Moves motor backwards
 */
void TopSpinShield::backward(uint8_t motorIndex, uint8_t speed)
{
	digitalWrite(motor[motorIndex][0], LOW);
	digitalWrite(motor[motorIndex][1], HIGH);
	analogWrite( motor[motorIndex][2], speed );
	updateLeds( motorIndex, BACKWARD );
}

/**
 * Stops motor
 */
void TopSpinShield::stop(uint8_t motorIndex)
{
	digitalWrite(motor[motorIndex][0], LOW);
	digitalWrite(motor[motorIndex][1], LOW);
	analogWrite( motor[motorIndex][2], 0 );
	updateLeds( motorIndex, STOP );
}

/**
 * Tests output
 */
void TopSpinShield::testOutput()
{
	step(0, FORWARD, 0, 255, 1, 1, 255);
	step(1, FORWARD, 0, 255, 1, 1, 255);
	step(0, FORWARD, 255, 0, -1, 1, 255);
	step(1, FORWARD, 255, 0, -1, 1, 255);
	step(0, BACKWARD, 0, 255, 1, 1, 255);
	step(1, BACKWARD, 0, 255, 1, 1, 255);
	step(0, BACKWARD, 255, 0, -1, 1, 255);
	step(1, BACKWARD, 255, 0, -1, 1, 255);

}

/**
 * Samples the switch input and writes status to status pins
 */
void TopSpinShield::sample()
{
	debounceInput( BASE_INPUT_PIN, (uint8_t *)&currentBaseValue, (uint8_t *)&baseCount, (uint8_t *)&baseChange );
	debounceInput( SIDE_INPUT_PIN, (uint8_t *)&currentSideValue, (uint8_t *)&sideCount, (uint8_t *)&sideChange );
}

/**
 * Debounces the input
 *
 */
void TopSpinShield::debounceInput( uint8_t pin, volatile uint8_t *currentValue, volatile uint8_t *currentCount, volatile uint8_t *change )
{
	uint8_t input = 0;
	input = digitalRead(pin);

    if( (input == *currentValue) && (*currentCount > 0) )
    {
    	*currentCount = *currentCount-1;
    }

    if(input != *currentValue)
    {
    	*currentCount = *currentCount + 1;
    }

    // If the Input has shown the same value for long enough let's switch it
    if(*currentCount >= DEBOUNCE_DELAY )
    {
    	// Trap transitions from high to low
    	if( *currentValue == 1 && input == 0 )
    	{
    		*change = true;
    	}
    	*currentCount = 0;
    	*currentValue = input;
    }
}

/**
 * Updates LEDs with current motor information
 */
void TopSpinShield::updateLeds( uint8_t motorIndex, uint8_t direction )
{
	switch(direction)
	{
		case FORWARD:
			if( motorIndex == 0 )
			{
				setLed1( ON );
				setLed2( OFF );
			}
			else if( motorIndex == 1 )
			{
				setLed3( ON );
				setLed4( OFF );
			}
			break;
		case BACKWARD:
			if( motorIndex == 0 )
			{
				setLed1( OFF );
				setLed2( ON );
			}
			else if( motorIndex == 1 )
			{
				setLed3( OFF );
				setLed4( ON );
			}
			break;
		case STOP:
			if( motorIndex == 0 )
			{
				setLed1( OFF );
				setLed2( OFF );
			}
			else if( motorIndex == 1 )
			{
				setLed3( OFF );
				setLed4( OFF );
			}
			break;

		default:
			break;
	}
}

/**
 * Sets values of LEDs
 */
void TopSpinShield::setLeds(uint8_t led1, uint8_t led2, uint8_t led3, uint8_t led4)
{
	uint8_t value = PORTC;

	if(led1 == OFF )
	{
		value = value & 0xFE;
	}
	else if (led1 == ON )
	{
		value = value | 0x01;
	}

	if(led2 == OFF )
	{
		value = value & 0xFD;
	}
	else if (led2 == ON )
	{
		value = value | 0x02;
	}

	if(led3 == OFF )
	{
		value = value & 0xFB;
	}
	else if (led3 == ON )
	{
		value = value | 0x04;
	}

	if(led4 == OFF )
	{
		value = value & 0xF7;
	}
	else if (led4 == ON )
	{
		value = value | 0x08;
	}

	PORTC = value;

}


void TopSpinShield::setLed1(uint8_t value)
{
	if(value == OFF )
	{
		PORTC = PORTC & 0xFE;
	}
	else if (value == ON )
	{
		PORTC = PORTC | 0x01;
	}
}

void TopSpinShield::setLed2(uint8_t value)
{
	if(value == OFF )
	{
		PORTC = PORTC & 0xFD;
	}
	else if (value == ON )
	{
		PORTC = PORTC | 0x02;
	}
}

void TopSpinShield::setLed3(uint8_t value)
{
	if(value == OFF )
	{
		PORTC = PORTC & 0xFB;
	}
	else if (value == ON )
	{
		PORTC = PORTC | 0x04;
	}
}

void TopSpinShield::setLed4(uint8_t value)
{
	if(value == OFF )
	{
		PORTC = PORTC & 0xF7;
	}
	else if (value == ON )
	{
		PORTC = PORTC | 0x08;
	}
}
