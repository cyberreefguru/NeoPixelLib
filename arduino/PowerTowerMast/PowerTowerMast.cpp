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

//#define MAX_LETTER_SIZE	(LETTER_P_SIZE + LETTER_O_SIZE + LETTER_W_SIZE + LETTER_E_SIZE + LETTER_R_SIZE + LETTER_T_SIZE)

void error(uint8_t errorCode);

void rotateLetters( uint8_t direction, uint16_t onTime, uint16_t offTime, uint8_t repeat, BRIGHTNESS_TYPE onBrightness, BRIGHTNESS_TYPE offBrightness);

void setLetter(uint8_t (*letter)[2], uint8_t len, BRIGHTNESS_TYPE value);
void sequenceLetter(uint8_t (*letter)[2], uint8_t len, BRIGHTNESS_TYPE value, uint16_t delayTime, uint8_t clearBetween);


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

#ifdef __DEBUG
	Serial.print("free=");
	Serial.println(freeRam());
#endif

#ifdef __DEBUG
	Serial.println("Initializing shield driver");
#endif
	if( driver.initialize(ROWS, COLS) == false )
	{
		error(10);
	}

#ifdef __DEBUG
	Serial.print("free=");
	Serial.println(freeRam());
#endif

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

//	// Turn all on
//	driver.setAll(MAX_VALUE);
//	driver.write();
//	PORTB |= _BV(5);
//	delay(1000);


	// Turn all off
	driver.setAll(0);
	driver.write();
	delay(750);

	// random on
	driver.randomize(ROWS, COLS, true, 10, MAX_BRIGHTNESS);

//	total = ROWS*COLS;
//	while( total > 0 )
//	{
//		q = random(0, ROWS*COLS);
//		i = q/ROWS; // column
//		j = q%ROWS; // row
//
//		if( driver.getValue(j, i) == 0 )
//		{
//			driver.setValue(j, i, MAX_BRIGHTNESS);
//			driver.write();
//			total--;
//			delay(10);
//		}
//	}

	delay(1000);

	// rotate letters - light through dark
	rotateLetters(0, 150, 0, 3, MAX_BRIGHTNESS, 0);

	delay(500);

	// set all on
	driver.setAll(MAX_BRIGHTNESS);
	driver.write();
	delay(1000);

	// Random off
	driver.randomize(ROWS, COLS, false, 10, 0);

//
//	total = ROWS*COLS;
//	while( total > 0 )
//	{
//		q = random(0, ROWS*COLS);
//		i = q/ROWS; // column
//		j = q%ROWS; // row
//
//		if( driver.getValue(j, i) == MAX_BRIGHTNESS )
//		{
//			driver.setValue(j, i, 0);
//			driver.write();
//			total--;
//			delay(10);
//		}
//	}
	delay(750);

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
//		sequenceLetter( (uint8_t (*)[2])LETTERS[i], LETTER_SIZE[i], MAX_BRIGHTNESS, 75, true);
//	}
//	delay(750);


	// turn all off
	driver.setAll(0);
	driver.write();

	// Sets each letter on in sequence
	for(i=0; i<NUM_LETTERS; i++)
	{
		setLetter( (uint8_t (*)[2])LETTERS[i], LETTER_SIZE[i], MAX_BRIGHTNESS );
		delay(300);
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

	// Sets POWER on, then off; sets TOWER on, then off
	for(i=0; i<7; i++)
	{
//		driver.setAll(0);
//		driver.write();

		setLetter( (uint8_t (*)[2])&LETTER_P1[0][0], LETTER_P_SIZE, MAX_BRIGHTNESS);
		setLetter( (uint8_t (*)[2])&LETTER_O1[0][0], LETTER_O_SIZE, MAX_BRIGHTNESS);
		setLetter( (uint8_t (*)[2])&LETTER_W1[0][0], LETTER_W_SIZE, MAX_BRIGHTNESS);
		setLetter( (uint8_t (*)[2])&LETTER_E1[0][0], LETTER_E_SIZE, MAX_BRIGHTNESS);
		setLetter( (uint8_t (*)[2])&LETTER_R1[0][0], LETTER_R_SIZE, MAX_BRIGHTNESS);

		setLetter( (uint8_t (*)[2])&LETTER_T1[0][0], LETTER_T_SIZE, MAX_BRIGHTNESS/10);
		setLetter( (uint8_t (*)[2])&LETTER_O2[0][0], LETTER_O_SIZE, MAX_BRIGHTNESS/10);
		setLetter( (uint8_t (*)[2])&LETTER_W2[0][0], LETTER_W_SIZE, MAX_BRIGHTNESS/10);
		setLetter( (uint8_t (*)[2])&LETTER_E2[0][0], LETTER_E_SIZE, MAX_BRIGHTNESS/10);
		setLetter( (uint8_t (*)[2])&LETTER_R2[0][0], LETTER_R_SIZE, MAX_BRIGHTNESS/10);

		delay(350);

//		driver.setAll(0);
//		driver.write();

		setLetter( (uint8_t (*)[2])&LETTER_P1[0][0], LETTER_P_SIZE, MAX_BRIGHTNESS/10);
		setLetter( (uint8_t (*)[2])&LETTER_O1[0][0], LETTER_O_SIZE, MAX_BRIGHTNESS/10);
		setLetter( (uint8_t (*)[2])&LETTER_W1[0][0], LETTER_W_SIZE, MAX_BRIGHTNESS/10);
		setLetter( (uint8_t (*)[2])&LETTER_E1[0][0], LETTER_E_SIZE, MAX_BRIGHTNESS/10);
		setLetter( (uint8_t (*)[2])&LETTER_R1[0][0], LETTER_R_SIZE, MAX_BRIGHTNESS/10);

		setLetter( (uint8_t (*)[2])&LETTER_T1[0][0], LETTER_T_SIZE, MAX_BRIGHTNESS);
		setLetter( (uint8_t (*)[2])&LETTER_O2[0][0], LETTER_O_SIZE, MAX_BRIGHTNESS);
		setLetter( (uint8_t (*)[2])&LETTER_W2[0][0], LETTER_W_SIZE, MAX_BRIGHTNESS);
		setLetter( (uint8_t (*)[2])&LETTER_E2[0][0], LETTER_E_SIZE, MAX_BRIGHTNESS);
		setLetter( (uint8_t (*)[2])&LETTER_R2[0][0], LETTER_R_SIZE, MAX_BRIGHTNESS);
		delay(350);
	}

	// Sets all off
	driver.setAll(0);
	driver.write();

	// Sets each segment of each letter on without clearing in between
	for(i=0; i<NUM_LETTERS; i++)
	{
		sequenceLetter( (uint8_t (*)[2])LETTERS[i], LETTER_SIZE[i], MAX_BRIGHTNESS, 75, false);
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
			setLetter( (uint8_t (*)[2])LETTERS[j], LETTER_SIZE[j], MAX_BRIGHTNESS );
			setLetter( (uint8_t (*)[2])LETTERS[j+(NUM_LETTERS/2)], LETTER_SIZE[j+(NUM_LETTERS/2)], MAX_BRIGHTNESS );
			delay(200);
		}
//		setLetter( (uint8_t (*)[2])&LETTER_P1[0][0], LETTER_P_SIZE, MAX_BRIGHTNESS);
//		setLetter( (uint8_t (*)[2])&LETTER_T1[0][0], LETTER_T_SIZE, MAX_BRIGHTNESS);
//		delay(200);
//		setLetter( (uint8_t (*)[2])&LETTER_O1[0][0], LETTER_O_SIZE, MAX_BRIGHTNESS);
//		setLetter( (uint8_t (*)[2])&LETTER_O2[0][0], LETTER_O_SIZE, MAX_BRIGHTNESS);
//		delay(200);
//		setLetter( (uint8_t (*)[2])&LETTER_W1[0][0], LETTER_W_SIZE, MAX_BRIGHTNESS);
//		setLetter( (uint8_t (*)[2])&LETTER_W2[0][0], LETTER_W_SIZE, MAX_BRIGHTNESS);
//		delay(200);
//		setLetter( (uint8_t (*)[2])&LETTER_E1[0][0], LETTER_E_SIZE, MAX_BRIGHTNESS);
//		setLetter( (uint8_t (*)[2])&LETTER_E2[0][0], LETTER_E_SIZE, MAX_BRIGHTNESS);
//		delay(200);
//		setLetter( (uint8_t (*)[2])&LETTER_R1[0][0], LETTER_R_SIZE, MAX_BRIGHTNESS);
//		setLetter( (uint8_t (*)[2])&LETTER_R2[0][0], LETTER_R_SIZE, MAX_BRIGHTNESS);
//		delay(200);
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
		for(j=0; j<10; j++)
		{
			setLetter( (uint8_t (*)[2])LETTERS[j], LETTER_SIZE[j], onBrightness);
			delay(onTime);
			setLetter( (uint8_t (*)[2])LETTERS[j], LETTER_SIZE[j], offBrightness);
			delay(offTime);
		}
	}
}

void setLetter(uint8_t (*letter)[2], uint8_t len, BRIGHTNESS_TYPE value)
{
	uint8_t i,j;

	for(i=0; i<len; i++)
	{
		driver.setValue( letter[i][0], letter[i][1], value);
	}
	driver.write();
}

void sequenceLetter(uint8_t (*letter)[2], uint8_t len, BRIGHTNESS_TYPE value, uint16_t delayTime, uint8_t clearBetween)
{
	uint8_t i,j;

	for(i=0; i<len; i++)
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

