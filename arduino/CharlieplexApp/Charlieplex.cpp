/*
 * Charlieplex.cpp
 *
 *  Created on: Jan 16, 2012
 *      Author: tom
 */

#include "Charlieplex.h"

///** Define the static variable for Charlieplex */
Charlieplex CharliePort;

static uint8_t *dataPort;
static uint8_t *controlPort;
static uint8_t pinMask;
static uint8_t (*pinMap)[2];
static uint8_t numPins;
static uint8_t numLeds;

static uint16_t pattern; // pattern to be written
static uint8_t enableOutput;
static volatile uint16_t patternBuffer; // buffered copy of pattern
static volatile uint8_t patternIndex; // current bit that is being driven
static volatile uint8_t patternChanged; // flag indicating pattern copy needs updating
static volatile uint16_t patternMask;

/**
 * Constructor
 */
Charlieplex::Charlieplex()
{
}

/**
 * Initializes the library to use a specific port and pin mapping
 */
void Charlieplex::initialize(uint8_t *dp, uint8_t *cp, uint8_t pm, uint8_t (*p)[2], uint8_t np)
{
#ifdef __DEBUG
	// Initialize hardware serial port
	Serial.begin(115200);
	Serial.println("INITIALIZING");
#endif

	 // Set pin 13 to output
	pinMode( LED_PIN, OUTPUT );
	digitalWrite(LED_PIN, LOW);

	// Set local variables
	dataPort = dp;
	controlPort = cp;
	pinMask = pm;
	pinMap = p;
	numPins = np;
	numLeds = numPins*(numPins-1);

	// Set pins as inputs
	clear();

	pattern = 0;
	patternBuffer = 0;
	patternIndex = 0;
	patternMask = 0x01;
	patternChanged = false;
	enableOutput = false;

#ifdef __DEBUG
	uint8_t i;
	for(i=0; i<numLeds; i++)
	{
		Serial.print( pinMap[i][0], HEX);
		Serial.print( " " );
		Serial.println( pinMap[i][1], HEX);
	}
#endif

	// Initialize timer (500us interval)
	FlexiTimer2::set(1, Charlieplex::drivePattern);

	// Start the timer
	FlexiTimer2::start();

#ifdef __DEBUG
	Serial.println("INITIALIZED");
#endif

}

/**
 * Enables or disables display of the pattern
 */
void Charlieplex::enable(uint8_t flag)
{
	if( flag == false)
	{
		clear();
	}
	enableOutput = flag;
}

/**
 * Clears the output port
 */
void Charlieplex::clear()
{
	// Set pins as inputs
	*controlPort &= ~pinMask;
}

/**
 * Sets the pattern
 *
 */
void Charlieplex::setPattern(uint16_t value)
{
	pattern = value;
	patternChanged = true;
}

/**
 * Returns the pattern
 *
 */
uint16_t Charlieplex::getPattern()
{
	return pattern;
}

/**
 * Interrupt service routine that drives the
 * specified pattern to the output port
 *
 */
void Charlieplex::drivePattern()
{
	uint8_t bit;

	if( enableOutput == true )
	{
#ifdef __DEBUG
	PORTB |= 0x20;
#endif

		// See if pattern has changed and update copy when appropriate
		if( patternChanged == true && patternIndex == 0 )
		{
			patternBuffer = pattern;
			patternChanged = false;
		}

		// Clear current output value
		*controlPort &= ~pinMask;

		// If our current bit is a 1, drive it, otherwise, skip
		if( (patternBuffer&patternMask) == patternMask )
		{
			// Drive LED
			bit = digitalPinToBitMask(pinMap[patternIndex][0]);
			*controlPort |= bit;
			*dataPort |= bit;

			bit = digitalPinToBitMask(pinMap[patternIndex][1]);
			*controlPort |= bit;
			*dataPort &= ~bit;
		}

		// Increment counter
		patternIndex=patternIndex+1;
		patternMask = patternMask << 1;

		// Reset index counter
		if( patternIndex == numLeds )
		{
			patternIndex = 0;
			patternMask = 0x01;
		}

#ifdef __DEBUG
	PORTB &= 0xDF;
#endif

	} // end enableOutput true

} // end drivePattern

