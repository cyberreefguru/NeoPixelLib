/*
 * TopSpinApp.cpp
 *
 *  Created on: Aug 20, 2013
 *      Author: tsasala
 */

#include "TopSpinApp.h"

// Declare an instance of the TopSpinShield
TopSpinShield topSpinShield;


TopSpinApp::TopSpinApp() {

}

void TopSpinApp::initialize() {

#ifdef __DEBUG
	Serial.println("TopSpinApp.initialize: BEGIN");
#endif

	// Initialize hardware
	topSpinShield.initialize();

#ifdef __DEBUG
	// Notify of program start
	Serial.println("PROGRAM START");
	Serial.println("Starting Motor");
	topSpinShield.setLeds(OFF, OFF, OFF, OFF);
	delay(100);
	topSpinShield.setLed1(ON);
	delay(100);
	topSpinShield.setLed2(ON);
	delay(100);
	topSpinShield.setLed3(ON);
	delay(100);
	topSpinShield.setLed4(ON);
	delay(100);
	topSpinShield.setLed1(OFF);
	delay(100);
	topSpinShield.setLed2(OFF);
	delay(100);
	topSpinShield.setLed3(OFF);
	delay(100);
	topSpinShield.setLed4(OFF);
	delay(100);
#endif

#ifdef __DEBUG
	Serial.println("TopSpinApp.initialize: END");
#endif

} // end initialize


/**
 * Special function to call class level method
 * Function needs to be type
 */
void sampleInput()
{
	topSpinShield.sample();
}


void TopSpinApp::start() {

#ifdef __DEBUG
	Serial.println("TopSpinApp.start: BEGIN");
#endif

	rotateMainArm( FORWARD, 5, 255 );
	delay(2000);
	rotateMainArm( BACKWARD, 5, 255 );
	delay(2000);
	topSpinShield.moveTimed( FORWARD, MAIN_ARM, 255, 1600 );
	delay(4000);
	rotateChair( FORWARD, 5000, 255);
	delay(4000);
	rotateChair( BACKWARD, 5200, 255);
	delay(3000);
	rotateChair( FORWARD, 6200, 100);
	delay(4000);
	rotateChair( BACKWARD, 5900, 75);
	delay(6000);
	rotateChair( FORWARD, 5500, 75);
	delay(4000);
	rotateMainArm( BACKWARD, 5, 255 );
	topSpinShield.moveTimed( BACKWARD, MAIN_ARM, 255, 500 );
	delay(3000);
	rotateChair( BACKWARD, 6200, 60);
	delay(1000);
	rotateChair( FORWARD, 5200, 255);
	delay(4000);
	rotateMainArm( BACKWARD, 2, 255 );
	topSpinShield.moveTimed( BACKWARD, MAIN_ARM, 255, 350 );
	delay(1000);
	rotateChair( FORWARD, 6000, 75);
	delay(500);
	rotateChair( FORWARD, 5200, 255);
	delay(4000);

	findHome();
	delay(500);
	fineTune();

#ifdef __DEBUG
	Serial.println("TopSpinApp.start: END");
#endif



} // end start



/**
 * Rotates the main arm in the direction specified at the specified
 * speed for the number of times specified.
 *
 */
void TopSpinApp::rotateMainArm(uint8_t direction, uint8_t repeat, uint8_t speed)
{
	uint8_t i;

	// Start moving
	topSpinShield.move(direction, MAIN_ARM, speed);
	delay(200); // wait to clear tower in case we're in home position

	// Wait until main arm passes base to start counting
	while( topSpinShield.getCurrentSideValue() == 1 );
	while( topSpinShield.getCurrentSideValue() == 0 );
	repeat = repeat-1;

	for(i=0; i<repeat; i++ )
	{
		// Wait until arm passes base
		while( topSpinShield.getCurrentSideValue() == 1 );
		while( topSpinShield.getCurrentSideValue() == 0 );
	}
	topSpinShield.stop( MAIN_ARM );
}

/**
 * Rotates the chair in the direction specified at the specified
 * speed for the specified duration.
 *
 */
void TopSpinApp::rotateChair(uint8_t direction, uint16_t duration, uint8_t speed)
{
	// Start moving
	topSpinShield.move(direction, CHAIR, speed);
	delay(duration);
	topSpinShield.stop( CHAIR );
}


/**
 * Routine to put main arm in home position
 *
 */
void TopSpinApp::findHome()
{
	// No telling where we are, rotate forward
	topSpinShield.forward(MAIN_ARM, 255);
	while( topSpinShield.getCurrentSideValue() == 1 );
	while( topSpinShield.getCurrentSideValue() == 0 );
	topSpinShield.stop(MAIN_ARM);

	// Minor delay for dramatic effect
	delay(250);

	topSpinShield.backward(MAIN_ARM, 100);
	while(topSpinShield.getCurrentSideValue() == 1 );
	topSpinShield.stop(MAIN_ARM);
}

void TopSpinApp::fineTune()
{
	// fine tune location of chair
	if( topSpinShield.getCurrentBaseValue() == 1 )
	{
		// fine tune base
		topSpinShield.forward(CHAIR, 125);
		while( topSpinShield.getCurrentBaseValue() == 1 );
		topSpinShield.stop(CHAIR);
		topSpinShield.togglePin( LED_PIN, 5, 150 );
	}
	else
	{
		// fine tune base
		topSpinShield.backward(CHAIR, 200);
		while( topSpinShield.getCurrentBaseValue() == 0 );
		topSpinShield.stop(CHAIR);
		topSpinShield.togglePin( LED_PIN, 4, 350 );
		topSpinShield.forward(CHAIR, 150);
		while( topSpinShield.getCurrentBaseValue() == 1 );
		topSpinShield.stop(CHAIR);
	}

}

