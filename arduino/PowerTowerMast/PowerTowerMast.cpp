/*
 * PowerTowerLights.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: tsasala
 */

#include "PowerTowerMast.h"

LedShieldDriverScaled driver = LedShieldDriverScaled();

void isr();

#define WAIT 50

void error(uint8_t errorCode);

void rotateLetters( uint8_t direction, uint16_t onTime, uint16_t offTime, uint8_t repeat, BRIGHTNESS_TYPE onBrightness, BRIGHTNESS_TYPE offBrightness);

void setLetter(uint8_t index, BRIGHTNESS_TYPE value, uint8_t drive);
void sequenceLetter(uint8_t index, BRIGHTNESS_TYPE value, uint16_t delayTime, uint8_t clearBetween);
void ghostSequenceLetter(BRIGHTNESS_TYPE value, uint16_t delayTime);

uint8_t SEGMENT_CACHE[NUM_SEGMENTS][2];

/**
 * Standard arduino setup.  Called once
 *
 */
void setup()
{

	pinMode(13, OUTPUT);

#ifdef __DEBUG
	Serial.begin(115200);
#endif

//#ifdef __DEBUG
//	Serial.print("free=");
//	Serial.println(freeRam());
//#endif

#ifdef __DEBUG
	Serial.println("Initializing shield driver");
#endif
	if( driver.initialize(ROWS, COLS) == false )
	{
		error(10);
	}

//#ifdef __DEBUG
//	Serial.print("free=");
//	Serial.println(freeRam());
//#endif

#ifdef __DEBUG
	Serial.println("Initializing timer");
#endif
	FlexiTimer2::set(4, 0.0001, isr ); // 400us period
	FlexiTimer2::start();

#ifdef __DEBUG
	Serial.println("**** init complete *****");
#endif

}

/**
 * Standard arduino loop.  Called repeatedly
 *
 */
void loop()
{
	uint16_t i,j, q, total;

	// Turn all off
	driver.setAll(0);
	driver.write();

	// random on
	driver.randomize(ROWS, COLS, true, 10, MAX_BRIGHTNESS);
	delay(1000);

	// rotate letters - light through dark
	rotateLetters(0, 150, 0, 3, MAX_BRIGHTNESS, 0);
//	delay(500);

	// set all on
	driver.setAll(MAX_BRIGHTNESS);
	driver.write();
	delay(1000);

	// Random off
	driver.randomize(ROWS, COLS, false, 10, 0);
	delay(1000);

	// rotate letters - dark through light
	rotateLetters(0, 150, 0, 3, 0, MAX_BRIGHTNESS);
	delay(750);

//	// Turn all off
//	driver.setAll(0);
//	driver.write();
//
//	// Sets each segment of each letter on and clears in between
//	for(i=0; i<NUM_LETTERS; i++)
//	{
//		sequenceLetter( i, LETTER_SIZE[i], MAX_BRIGHTNESS, 75, true);
//	}
//	delay(750);


	// turn all on
	driver.setAll(MAX_BRIGHTNESS);
	driver.write();

	// turn off in sequence
	for(j=0; j<NUM_LETTERS; j++)
	{
		setLetter( j, 0, true);
		delay(200);
	}

	// Turns all letters on then off
	for(i=0; i<7; i++)
	{
		driver.setAll(0);
		driver.write();
		delay(250);
		driver.setAll(MAX_BRIGHTNESS);
		driver.write();
		delay(250);
	}

	driver.setAll(0);
	driver.write();

	ghostSequenceLetter(MAX_BRIGHTNESS, 75);

	delay(500);

	// Sets POWER on, then off; sets TOWER on, then off
	for(i=0; i<7; i++)
	{
		setLetter( 0, MAX_BRIGHTNESS, true);
		setLetter( 1, MAX_BRIGHTNESS, true);
		setLetter( 2, MAX_BRIGHTNESS, true);
		setLetter( 3, MAX_BRIGHTNESS, true);
		setLetter( 4, MAX_BRIGHTNESS, true);

		setLetter( 5, MAX_BRIGHTNESS/10, true);
		setLetter( 6, MAX_BRIGHTNESS/10, true);
		setLetter( 7, MAX_BRIGHTNESS/10, true);
		setLetter( 8, MAX_BRIGHTNESS/10, true);
		setLetter( 9, MAX_BRIGHTNESS/10, true);

		delay(350);

		setLetter( 0, MAX_BRIGHTNESS/10, true);
		setLetter( 1, MAX_BRIGHTNESS/10, true);
		setLetter( 2, MAX_BRIGHTNESS/10, true);
		setLetter( 3, MAX_BRIGHTNESS/10, true);
		setLetter( 4, MAX_BRIGHTNESS/10, true);

		setLetter( 5, MAX_BRIGHTNESS, true);
		setLetter( 6, MAX_BRIGHTNESS, true);
		setLetter( 7, MAX_BRIGHTNESS, true);
		setLetter( 8, MAX_BRIGHTNESS, true);
		setLetter( 9, MAX_BRIGHTNESS, true);

		delay(350);
	}

	// Sets all off
	driver.setAll(0);
	driver.write();

	// Sets each segment of each letter on without clearing in between
	for(i=0; i<NUM_LETTERS; i++)
	{
		sequenceLetter( i, MAX_BRIGHTNESS, 75, false);
	}

	// sets all off, then on
	for(i=0; i<7; i++)
	{
		driver.setAll(0);
		driver.write();
		delay(175);
		driver.setAll(MAX_BRIGHTNESS);
		driver.write();
		delay(175);
	}

	// sets all off
	driver.setAll(0);
	driver.write();

	// Sets letter of each word on, then goes to next letter
	for(i=0; i<5; i++)
	{
		for(j=0; j<NUM_LETTERS/2; j++)
		{
			setLetter( j, MAX_BRIGHTNESS, true);
			setLetter( j+(NUM_LETTERS/2), MAX_BRIGHTNESS, true);
			delay(200);
		}
		driver.setAll(0);
		driver.write();
		delay(200);
	}


} // end loop


void rotateLetters( uint8_t direction, uint16_t onTime, uint16_t offTime, uint8_t repeat, BRIGHTNESS_TYPE onBrightness, BRIGHTNESS_TYPE offBrightness)
{
	uint8_t i, j;

	driver.setAll(offBrightness);
	for(i=0; i<repeat; i++)
	{
		for(j=0; j<NUM_LETTERS; j++)
		{
			setLetter( j, onBrightness, true);
			delay(onTime);
			setLetter( j, offBrightness, true);
			delay(offTime);
		}
	}
}

void setLetter(uint8_t index, BRIGHTNESS_TYPE value, uint8_t drive)
{
	uint8_t i;

	uint8_t (*letter)[2] = (uint8_t (*)[2])LETTERS[index];

	for(i=0; i<LETTER_SIZE[index]; i++)
	{
		driver.setValue( letter[i][0], letter[i][1], value);
	}

	if( drive )
	{
		driver.write();
	}
}

void sequenceLetter(uint8_t index, BRIGHTNESS_TYPE value, uint16_t delayTime, uint8_t clearBetween)
{
	uint8_t i,j;

	uint8_t (*letter)[2] = (uint8_t (*)[2])LETTERS[index];

	for(i=0; i<LETTER_SIZE[index]; i++)
	{
		driver.setValue( letter[i][0], letter[i][1], value);
		driver.write();
		delay(delayTime);
		if( clearBetween == true)
		{
			driver.setAll(0);
			driver.write();
		}
	}
}

void ghostSequenceLetter(BRIGHTNESS_TYPE value, uint16_t delayTime)
{
	uint8_t index, i, j;
	uint8_t (*letter)[2];

	driver.setAll(0);

	// Build flat array of letter segment values
	index = 0;
	for(i=0; i<NUM_LETTERS; i++)
	{
		letter = (uint8_t (*)[2])LETTERS[i];
		for(j=0; j< LETTER_SIZE[i]; j++)
		{
			SEGMENT_CACHE[index][0] = letter[j][0];
			SEGMENT_CACHE[index++][1] = letter[j][1];
		}
	}

	for(i=0; i<(index+3); i++)
	{
		if( (i >= 0) && (i < index) )
		{
			driver.setValue( SEGMENT_CACHE[i][0], SEGMENT_CACHE[i][1], value);
		}
		if( (i>0) && (i< index+1) )
		{
			driver.setValue( SEGMENT_CACHE[i-1][0], SEGMENT_CACHE[i-1][1], value/5);
		}
		if( (i>1) && (i < index+2) )
		{
			driver.setValue( SEGMENT_CACHE[i-2][0], SEGMENT_CACHE[i-2][1], value/10);
		}
		if( (i>2) && (i < NUM_SEGMENTS+3) )
		{
			driver.setValue( SEGMENT_CACHE[i-3][0], SEGMENT_CACHE[i-3][1], 0);
		}
		driver.write();
		delay(delayTime);
	}
}


/**
 * Interrupt service routine
 *
 */
void isr()
{
	driver.execInterrupt();

//	static boolean output = HIGH;
//	digitalWrite(led_pin, output);
//	output = !output;
}


/**
 * Flashes on-board LED to indicate critical failure
 */
void error(uint8_t errorCode)
{
	uint16_t delayValue;

	delayValue = errorCode*10;

	while (1)
	{
		digitalWrite(13, HIGH);
		delay(delayValue);
		digitalWrite(13, LOW);
		delay(delayValue);
	}
}

