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
void sparkle(uint16_t runDuration, uint16_t sparkleDuration, uint16_t numSparkle, INTENSITY_TYPE intensity);
void rotateChangeSpeed(uint8_t direction, uint16_t start, uint16_t end, uint16_t increment, INTENSITY_TYPE onBrightness, INTENSITY_TYPE offBrightness);

uint8_t SEGMENT_CACHE[NUM_SEGMENTS][2];

/**
 * Standard arduino setup.  Called once
 *
 */
void setup()
{
	uint8_t index, i, j;
	uint8_t (*letter)[2];

	// set led to output
	pinMode(13, OUTPUT);

#ifdef __DEBUG
	Serial.begin(115200);
//	Serial.print("free=");
//	Serial.println(freeRam());
	Serial.println("Initializing shield driver");
#endif

	if( driver.initialize(ROWS, COLS) == false )
	{
		error(10);
	}

#ifdef __DEBUG
//	Serial.print("free=");
//	Serial.println(freeRam());
#endif


	// Build flat array of letter segment values
	// This allows us to cycle through the segments in order
	index = 0;
	for(i=0; i<NUM_LETTERS; i++)
	{
		letter = (uint8_t (*)[2])LETTERS[i];
		for(j=0; j < LETTER_SIZE[i]; j++)
		{
			SEGMENT_CACHE[index][0] = letter[j][0];
			SEGMENT_CACHE[index++][1] = letter[j][1];
		}
	}

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
	driver.clear();

	// sparkle
	sparkle(3000, 50, 10, MAX_INTENSITY);


	// random on
	driver.randomize(ROWS, COLS, true, 10, MAX_INTENSITY);
	delay(1000);

	// rotate letters right - light through dark
	// speeds up, runs fast, slows down
	rotateChangeSpeed(DIRECTION_RIGHT, 150, 25, 25, MAX_INTENSITY, 0);
	rotateLetters(DIRECTION_RIGHT, 25, 0, 5, MAX_INTENSITY, 0);
	rotateChangeSpeed(DIRECTION_RIGHT, 25, 150, 25, MAX_INTENSITY, 0);

	driver.clear();
	delay(500);

	ghostSequenceLetter(MAX_INTENSITY, 75);

	delay(500);

	// set all on
	driver.setAll(MAX_INTENSITY, true);
	delay(1250);

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

	delay(750);

	// turn all on
	driver.setAll(MAX_INTENSITY, true);

	delay(2000);

	// turn off in sequence
	for(j=0; j<NUM_LETTERS; j++)
	{
		setLetter( j, 0, true);
		delay(200);
	}

	// Turns all letters on then off
	for(i=0; i<7; i++)
	{
		driver.clear();
		delay(250);
		driver.setAll(MAX_INTENSITY, true);
		delay(250);
	}

	// rotate letters - dark through light
	rotateChangeSpeed(DIRECTION_LEFT, 150, 25, 25, 0, MAX_INTENSITY);
	rotateLetters(DIRECTION_LEFT, 25, 0, 5, 0, MAX_INTENSITY);
	rotateChangeSpeed(DIRECTION_LEFT, 25, 150, 25, 0, MAX_INTENSITY);
	delay(750);

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
	driver.clear();

	// Sets each segment of each letter on without clearing in between
	for(i=0; i<NUM_LETTERS; i++)
	{
		sequenceLetter( i, MAX_INTENSITY, 75, false);
	}

	// sets all off, then on
	for(i=0; i<7; i++)
	{
		driver.clear();
		delay(175);
		driver.setAll(MAX_INTENSITY, true);
		delay(175);
	}

	// sets all off
	driver.clear();

	// Sets letter of each word on, then goes to next letter
	for(i=0; i<5; i++)
	{
		for(j=0; j<NUM_LETTERS/2; j++)
		{
			setLetter( j, MAX_INTENSITY, true);
			setLetter( j+(NUM_LETTERS/2), MAX_INTENSITY, true);
			delay(200);
		}
		driver.clear();
		delay(200);
	}


} // end loop

/**
 * rotates the letters with the rotation speed changing each revolution
 * start speed = start; end speed = end.
 *
 * @direction - left or right
 * @start duration to start with (ms)
 * @end duration to end with (ms)
 * @increment value to change each loop iteration; note: abs(start-end)/increment = loop count
 * @onBrightness intensity when on
 * @offBrightness intensity when off
 */
void rotateChangeSpeed(uint8_t direction, uint16_t start, uint16_t end, uint16_t increment, INTENSITY_TYPE onBrightness, INTENSITY_TYPE offBrightness)
{
	uint16_t i;

	if( start > end )
	{
		// Speed up
		for(i=start; i>=end; i=i-increment)
		{
			rotateLetters(direction, i, 0, 1, onBrightness, offBrightness);
		}
	}
	else if( start < end)
	{
		// Slow down
		for(i=start; i<end; i=i+increment)
		{
			rotateLetters(direction, i, 0, 1, onBrightness, offBrightness);
		}
	}
}

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

	driver.setAll(offBrightness, false);
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
			driver.clear();
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
	uint8_t i, j;

	for(i=0; i<(NUM_SEGMENTS+3); i++)
	{
		if( (i >= 0) && (i < NUM_SEGMENTS) )
		{
			driver.setValue( SEGMENT_CACHE[i][0], SEGMENT_CACHE[i][1], value);
		}
		if( (i>0) && (i< NUM_SEGMENTS+1) )
		{
			driver.setValue( SEGMENT_CACHE[i-1][0], SEGMENT_CACHE[i-1][1], value/5);
		}
		if( (i>1) && (i < NUM_SEGMENTS+2) )
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
 * flashes (sparkleDuration) numSparkle random LEDs for runDuration time.
 * Why 25? Because that's what previous code used ;)
 *
 * @runDuration length of time to run routine in ms
 * @sparkleDuration length of time LED will be on in ms
 * @numSparkle number of LEDs to turn on at a time
 * @intensity of LEDs while on
 */
void sparkle(uint16_t runDuration, uint16_t sparkleDuration, uint16_t numSparkle, INTENSITY_TYPE intensity)
{
	uint8_t i;
	uint16_t q;
	uint32_t end;

	// n random on fast
	driver.clear();

	end = millis() + runDuration;
	while( millis() < end )
	{
		for(i=0; i<numSparkle; i++)
		{
			q = random(0, NUM_SEGMENTS);
			driver.setValue( SEGMENT_CACHE[q][0], SEGMENT_CACHE[q][1], intensity);
		}

		driver.write();
		delay(sparkleDuration);
		driver.clear();
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
 *
 * @errorCode - errorCode determines how quickly to flash
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

