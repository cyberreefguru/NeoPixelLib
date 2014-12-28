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

#define LETTER_SIZE	(LETTER_P_SIZE + LETTER_O_SIZE + LETTER_W_SIZE + LETTER_E_SIZE + LETTER_R_SIZE + LETTER_T_SIZE)

void error(uint8_t errorCode);

void setLetter(uint8_t (*letter)[2], uint8_t len, uint8_t value);
void sequenceLetter(uint8_t (*letter)[2], uint8_t len, uint8_t value, uint16_t delayTime, uint8_t clearBetween);


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
	uint16_t i,j;

//	// Turn all on
//	driver.setAll(MAX_VALUE);
//	driver.write();
//	PORTB |= _BV(5);
//	delay(1000);

	// Turn all off
	driver.setAll(0);
	driver.write();

	sequenceLetter((uint8_t (*)[2])&LETTER_P1[0][0], LETTER_P_SIZE, 255, 75, true);
	sequenceLetter((uint8_t (*)[2])&LETTER_O1[0][0], LETTER_O_SIZE, 255, 75, true);
	sequenceLetter((uint8_t (*)[2])&LETTER_W1[0][0], LETTER_W_SIZE, 255, 75, true);
	sequenceLetter((uint8_t (*)[2])&LETTER_E1[0][0], LETTER_E_SIZE, 255, 75, true);
	sequenceLetter((uint8_t (*)[2])&LETTER_R1[0][0], LETTER_R_SIZE, 255, 75, true);
	sequenceLetter((uint8_t (*)[2])&LETTER_T1[0][0], LETTER_T_SIZE, 255, 75, true);
	sequenceLetter((uint8_t (*)[2])&LETTER_O2[0][0], LETTER_O_SIZE, 255, 75, true);
	sequenceLetter((uint8_t (*)[2])&LETTER_W2[0][0], LETTER_W_SIZE, 255, 75, true);
	sequenceLetter((uint8_t (*)[2])&LETTER_E2[0][0], LETTER_E_SIZE, 255, 75, true);
	sequenceLetter((uint8_t (*)[2])&LETTER_R2[0][0], LETTER_R_SIZE, 255, 75, true);

	driver.setAll(0);
	driver.write();


	setLetter( (uint8_t (*)[2])&LETTER_P1[0][0], LETTER_P_SIZE, 255);
	delay(300);
	setLetter( (uint8_t (*)[2])&LETTER_O1[0][0], LETTER_O_SIZE, 255);
	delay(300);
	setLetter( (uint8_t (*)[2])&LETTER_W1[0][0], LETTER_W_SIZE, 255);
	delay(300);
	setLetter( (uint8_t (*)[2])&LETTER_E1[0][0], LETTER_E_SIZE, 255);
	delay(300);
	setLetter( (uint8_t (*)[2])&LETTER_R1[0][0], LETTER_R_SIZE, 255);
	delay(300);

	setLetter( (uint8_t (*)[2])&LETTER_T1[0][0], LETTER_T_SIZE, 255);
	delay(300);
	setLetter( (uint8_t (*)[2])&LETTER_O2[0][0], LETTER_O_SIZE, 255);
	delay(300);
	setLetter( (uint8_t (*)[2])&LETTER_W2[0][0], LETTER_W_SIZE, 255);
	delay(300);
	setLetter( (uint8_t (*)[2])&LETTER_E2[0][0], LETTER_E_SIZE, 255);
	delay(300);
	setLetter( (uint8_t (*)[2])&LETTER_R2[0][0], LETTER_R_SIZE, 255);
	delay(300);

	for(i=0; i<7; i++)
	{
		driver.setAll(0);
		driver.write();
		delay(250);
		driver.setAll(255);
		driver.write();
		delay(250);
	}

	for(i=0; i<7; i++)
	{
		driver.setAll(0);
		driver.write();

		setLetter( (uint8_t (*)[2])&LETTER_P1[0][0], LETTER_P_SIZE, 255);
		setLetter( (uint8_t (*)[2])&LETTER_O1[0][0], LETTER_O_SIZE, 255);
		setLetter( (uint8_t (*)[2])&LETTER_W1[0][0], LETTER_W_SIZE, 255);
		setLetter( (uint8_t (*)[2])&LETTER_E1[0][0], LETTER_E_SIZE, 255);
		setLetter( (uint8_t (*)[2])&LETTER_R1[0][0], LETTER_R_SIZE, 255);
		delay(350);

		driver.setAll(0);
		driver.write();

		setLetter( (uint8_t (*)[2])&LETTER_T1[0][0], LETTER_T_SIZE, 255);
		setLetter( (uint8_t (*)[2])&LETTER_O2[0][0], LETTER_O_SIZE, 255);
		setLetter( (uint8_t (*)[2])&LETTER_W2[0][0], LETTER_W_SIZE, 255);
		setLetter( (uint8_t (*)[2])&LETTER_E2[0][0], LETTER_E_SIZE, 255);
		setLetter( (uint8_t (*)[2])&LETTER_R2[0][0], LETTER_R_SIZE, 255);
		delay(350);
	}

	driver.setAll(0);
	driver.write();

	sequenceLetter((uint8_t (*)[2])&LETTER_P1[0][0], LETTER_P_SIZE, 255, 75, false);
	sequenceLetter((uint8_t (*)[2])&LETTER_O1[0][0], LETTER_O_SIZE, 255, 75, false);
	sequenceLetter((uint8_t (*)[2])&LETTER_W1[0][0], LETTER_W_SIZE, 255, 75, false);
	sequenceLetter((uint8_t (*)[2])&LETTER_E1[0][0], LETTER_E_SIZE, 255, 75, false);
	sequenceLetter((uint8_t (*)[2])&LETTER_R1[0][0], LETTER_R_SIZE, 255, 75, false);
	sequenceLetter((uint8_t (*)[2])&LETTER_T1[0][0], LETTER_T_SIZE, 255, 75, false);
	sequenceLetter((uint8_t (*)[2])&LETTER_O2[0][0], LETTER_O_SIZE, 255, 75, false);
	sequenceLetter((uint8_t (*)[2])&LETTER_W2[0][0], LETTER_W_SIZE, 255, 75, false);
	sequenceLetter((uint8_t (*)[2])&LETTER_E2[0][0], LETTER_E_SIZE, 255, 75, false);
	sequenceLetter((uint8_t (*)[2])&LETTER_R2[0][0], LETTER_R_SIZE, 255, 75, false);


	for(i=0; i<7; i++)
	{
		driver.setAll(0);
		driver.write();
		delay(175);
		driver.setAll(255);
		driver.write();
		delay(175);
	}

	driver.setAll(0);
	driver.write();

	for(i=0; i<5; i++)
	{
		setLetter( (uint8_t (*)[2])&LETTER_P1[0][0], LETTER_P_SIZE, 255);
		setLetter( (uint8_t (*)[2])&LETTER_T1[0][0], LETTER_T_SIZE, 255);
		delay(200);
		setLetter( (uint8_t (*)[2])&LETTER_O1[0][0], LETTER_O_SIZE, 255);
		setLetter( (uint8_t (*)[2])&LETTER_O2[0][0], LETTER_O_SIZE, 255);
		delay(200);
		setLetter( (uint8_t (*)[2])&LETTER_W1[0][0], LETTER_W_SIZE, 255);
		setLetter( (uint8_t (*)[2])&LETTER_W2[0][0], LETTER_W_SIZE, 255);
		delay(200);
		setLetter( (uint8_t (*)[2])&LETTER_E1[0][0], LETTER_E_SIZE, 255);
		setLetter( (uint8_t (*)[2])&LETTER_E2[0][0], LETTER_E_SIZE, 255);
		delay(200);
		setLetter( (uint8_t (*)[2])&LETTER_R1[0][0], LETTER_R_SIZE, 255);
		setLetter( (uint8_t (*)[2])&LETTER_R2[0][0], LETTER_R_SIZE, 255);
		delay(200);
		driver.setAll(0);
		driver.write();
	}


} // end loop

void setLetter(uint8_t (*letter)[2], uint8_t len, uint8_t value)
{
	uint8_t i,j;

	for(i=0; i<len; i++)
	{
		driver.setValue( letter[i][0], letter[i][1], value);
	}
	driver.write();
}

void sequenceLetter(uint8_t (*letter)[2], uint8_t len, uint8_t value, uint16_t delayTime, uint8_t clearBetween)
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

