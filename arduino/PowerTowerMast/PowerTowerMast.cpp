/*
 * PowerTowerLights.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: tsasala
 */

#include "PowerTowerMast.h"

LedShieldDriverScaled driver = LedShieldDriverScaled();

void isr();

void error(uint8_t errorCode);

void rotateLetters( uint8_t direction, uint16_t onTime, uint16_t offTime, uint8_t repeat, INTENSITY_TYPE onBrightness, INTENSITY_TYPE offBrightness);
void setLetter(uint8_t index, INTENSITY_TYPE value, uint8_t drive);
void sequenceLetter(uint8_t index, INTENSITY_TYPE value, uint16_t delayTime, uint8_t clearBetween);
void ghostSequenceLetter(INTENSITY_TYPE value, uint16_t delayTime);

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
	uint16_t i,j;

	// Turn all off
	driver.setAll(0);
	driver.write();

	// random on
	driver.randomize(ROWS, COLS, true, 10, MAX_INTENSITY);
	delay(1000);

	// rotate letters - light through dark
	rotateLetters(DIRECTION_RIGHT, 150, 0, 1, MAX_INTENSITY, 0);
	rotateLetters(DIRECTION_RIGHT, 125, 0, 1, MAX_INTENSITY, 0);
	rotateLetters(DIRECTION_RIGHT, 100, 0, 1, MAX_INTENSITY, 0);
	rotateLetters(DIRECTION_RIGHT, 75, 0, 1, MAX_INTENSITY, 0);
	rotateLetters(DIRECTION_RIGHT, 50, 0, 1, MAX_INTENSITY, 0);
	// rotate letters - light through dark
	rotateLetters(DIRECTION_LEFT, 50, 0, 1, MAX_INTENSITY, 0);
	rotateLetters(DIRECTION_LEFT, 75, 0, 1, MAX_INTENSITY, 0);
	rotateLetters(DIRECTION_LEFT, 100, 0, 1, MAX_INTENSITY, 0);
	rotateLetters(DIRECTION_LEFT, 125, 0, 1, MAX_INTENSITY, 0);
	rotateLetters(DIRECTION_LEFT, 150, 0, 1, MAX_INTENSITY, 0);

	// set all on
	driver.setAll(MAX_INTENSITY);
	driver.write();
	delay(1000);

	// Random off
	driver.randomize(ROWS, COLS, false, 10, 0);

//	// Turn all off
//	driver.setAll(0);
//	driver.write();
//
//	// Sets each segment of each letter on and clears in between
//	for(i=0; i<NUM_LETTERS; i++)
//	{
//		sequenceLetter( i, LETTER_SIZE[i], MAX_INTENSITY, 75, true);
//	}

	delay(500);

	// turn all on
	driver.setAll(MAX_INTENSITY);
	driver.write();

	delay(1000);

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
		driver.setAll(MAX_INTENSITY);
		driver.write();
		delay(250);
	}

	// rotate letters - dark through light
	rotateLetters(DIRECTION_RIGHT, 150, 0, 1, 0, MAX_INTENSITY);
	rotateLetters(DIRECTION_RIGHT, 125, 0, 1, 0, MAX_INTENSITY);
	rotateLetters(DIRECTION_RIGHT, 100, 0, 1, 0, MAX_INTENSITY);
	rotateLetters(DIRECTION_RIGHT, 75, 0, 1, 0, MAX_INTENSITY);
	rotateLetters(DIRECTION_RIGHT, 50, 0, 1, 0, MAX_INTENSITY);
	rotateLetters(DIRECTION_LEFT, 50, 0, 1, 0, MAX_INTENSITY);
	rotateLetters(DIRECTION_LEFT, 75, 0, 1, 0, MAX_INTENSITY);
	rotateLetters(DIRECTION_LEFT, 100, 0, 1, 0, MAX_INTENSITY);
	rotateLetters(DIRECTION_LEFT, 125, 0, 1, 0, MAX_INTENSITY);
	rotateLetters(DIRECTION_LEFT, 150, 0, 1, 0, MAX_INTENSITY);

	delay(750);

	driver.setAll(0);
	driver.write();
	delay(750);

	ghostSequenceLetter(MAX_INTENSITY, 75);

	delay(500);

	// Sets POWER on, then off; sets TOWER on, then off
	for(i=0; i<7; i++)
	{
		setLetter( 0, MAX_INTENSITY, true);
		setLetter( 1, MAX_INTENSITY, true);
		setLetter( 2, MAX_INTENSITY, true);
		setLetter( 3, MAX_INTENSITY, true);
		setLetter( 4, MAX_INTENSITY, true);

		setLetter( 5, MAX_INTENSITY/10, true);
		setLetter( 6, MAX_INTENSITY/10, true);
		setLetter( 7, MAX_INTENSITY/10, true);
		setLetter( 8, MAX_INTENSITY/10, true);
		setLetter( 9, MAX_INTENSITY/10, true);

		delay(350);

		setLetter( 0, MAX_INTENSITY/10, true);
		setLetter( 1, MAX_INTENSITY/10, true);
		setLetter( 2, MAX_INTENSITY/10, true);
		setLetter( 3, MAX_INTENSITY/10, true);
		setLetter( 4, MAX_INTENSITY/10, true);

		setLetter( 5, MAX_INTENSITY, true);
		setLetter( 6, MAX_INTENSITY, true);
		setLetter( 7, MAX_INTENSITY, true);
		setLetter( 8, MAX_INTENSITY, true);
		setLetter( 9, MAX_INTENSITY, true);

		delay(350);
	}

	// Sets all off
	driver.setAll(0);
	driver.write();

	// Sets each segment of each letter on without clearing in between
	for(i=0; i<NUM_LETTERS; i++)
	{
		sequenceLetter( i, MAX_INTENSITY, 75, false);
	}

	// sets all off, then on
	for(i=0; i<7; i++)
	{
		driver.setAll(0);
		driver.write();
		delay(175);
		driver.setAll(MAX_INTENSITY);
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
			setLetter( j, MAX_INTENSITY, true);
			setLetter( j+(NUM_LETTERS/2), MAX_INTENSITY, true);
			delay(200);
		}
		driver.setAll(0);
		driver.write();
		delay(200);
	}


} // end loop

/**
 * Rotates through all letters; on @ onBrightness for onTime; off @offBrightness for offTime, then next letter
 *
 * @direction - left or right; DIRECTION_LEFT (high index to low index), DIRECTION_RIGHT (low index to high index)
 * @onTime time letter will be on
 * @offTime time letter will be off ("between" segments)
 * @repeat number of times to repeat
 * @onBrightness intensity of on value
 * @offBrightness intensity of off value
 */
void rotateLetters( uint8_t direction, uint16_t onTime, uint16_t offTime, uint8_t repeat, INTENSITY_TYPE onBrightness, INTENSITY_TYPE offBrightness)
{
	int8_t i, j;

	driver.setAll(offBrightness);
	for(i=0; i<repeat; i++)
	{
		if( direction == DIRECTION_LEFT )
		{
			for(j=(NUM_LETTERS-1); j>=0; j--)
			{
				setLetter( j, onBrightness, true);
				delay(onTime);
				setLetter( j, offBrightness, true);
				delay(offTime);
			}
		}
		else if( direction == DIRECTION_RIGHT )
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
}

/**
 * Turns a single letter on.
 *
 * @index letter index
 * @value intensity value - 0 to MAX_INTENSITY
 * @drive true - call drive and display; false - do not call drive
 */
void setLetter(uint8_t index, INTENSITY_TYPE value, uint8_t drive)
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

/**
 * Turns letter segments on one after another
 *
 * @index index of letter
 * @value intensity of segment, 0-MAX_INTENSITY
 * @delayTime time to leave on before moving to next segment
 * @clearBetween - true/false; if true, turns segment off before going to next segment
 */
void sequenceLetter(uint8_t index, INTENSITY_TYPE value, uint16_t delayTime, uint8_t clearBetween)
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

/**
 * Turns segments on one after another; leaves trail behind of lower brightness values
 *
 * @value intensity of segment; 0-MAX_INTENSITY
 * @delaytTime time on before moving to next segment
 */
void ghostSequenceLetter(INTENSITY_TYPE value, uint16_t delayTime)
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

