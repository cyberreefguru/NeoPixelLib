/*
 * MotorShield.h
 *
 *  Created on: Apr 15, 2014
 *      Author: tsasala
 */

#ifndef MOTORSHIELD_H_
#define MOTORSHIELD_H_

#include "Arduino.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// This is just general Arduino description; not program specific
//PORTD maps to Arduino digital pins 0 to 7
//    DDRD - The Port D Data Direction Register - read/write
//    PORTD - The Port D Data Register - read/write
//    PIND - The Port D Input Pins Register - read only
//
//PORTB maps to Arduino digital pins 8 to 13 The two high bits (6 & 7) map to the crystal pins and are not usable
//    DDRB - The Port B Data Direction Register - read/write
//    PORTB - The Port B Data Register - read/write
//    PINB - The Port B Input Pins Register - read only
//
//PORTC maps to Arduino analog pins 0 to 5. Pins 6 & 7 are only accessible on the Arduino Mini
//    DDRC - The Port C Data Direction Register - read/write
//    PORTC - The Port C Data Register - read/write
//    PINC - The Port C Input Pins Register - read only

#define LED_PIN		13

#define FORWARD 	0
#define UP		 	FORWARD
#define BACKWARD 	1
#define DOWN		BACKWARD
#define STOP 		2

#define DEBOUNCE_DELAY	5

extern uint8_t leds[4];
extern uint8_t motor[2][3];
extern void sampleInput();


#define NUM_MOTORS	2
#define NUM_LEDS	4
#define NUM_INPUTS	4

#define ON 			true
#define OFF			false
#define DONT_CARE	2

#define LED_1		0
#define LED_2		1
#define LED_3		2
#define LED_4		3


class MotorShield {
public:
	MotorShield();

	/**
	 * Function stereotypes
	 */
	void initialize();

	void execIsr();

	volatile uint8_t getInputValue(uint8_t index);

	void step(uint8_t motorIndex, int8_t direction, uint8_t initialSpeed, uint8_t endSpeed, int8_t speedStep, uint16_t delayValue, uint16_t repeatValue);

	void forward(uint8_t motorIndex, uint8_t speed);
	void backward(uint8_t motorIndex, uint8_t speed);
	void stop(uint8_t motorIndex);

	void setSpeed(uint8_t motorIndex, uint8_t speed);

	void togglePin( uint8_t pin, uint8_t repeat, uint16_t duration );

	void move( uint8_t direction, uint8_t motorIndex, uint8_t speed);
	void moveTimed(uint8_t direction, uint8_t motorIndex, uint8_t speed, uint16_t duration);

	void setLed(uint8_t ledIndex, uint8_t ledValue);
	void updateLeds( uint8_t motorIndex, uint8_t direction );

private:
	volatile uint8_t inputValue[4];
	volatile uint8_t inputCount[4];
	volatile uint8_t inputChange[4];

};




#endif /* MOTORSHIELD_H_ */
