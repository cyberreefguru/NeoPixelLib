/*
 * TopSpinShield.h
 *
 *  Created on: Aug 20, 2013
 *      Author: tsasala
 */

#ifndef TOPSPINSHIELD_H_
#define TOPSPINSHIELD_H_

#include "Arduino.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <FlexiTimer2.h>

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

//#define BASE_STATUS_PIN	6
//#define SIDE_STATUS_PIN	7

#define BASE_INPUT_PIN	11
#define SIDE_INPUT_PIN	12

#define FORWARD 	0
#define BACKWARD 	1
#define STOP 		2

#define DEBOUNCE_DELAY	5

extern uint8_t leds[4];
extern uint8_t motor[2][3];
extern void sampleInput();


// Base -> In2 (D11)
// Side -> In3 (D12)
// Right Motor -> 1Y -> O1.1 -> RED (D2)
// Right Motor -> 2Y -> O1.2 -> BLUE (D3)
// Right Motor -> 1-2EN -> PWM (D9)
// Left Motor  -> 3Y -> O2.1 -> RED (D4)
// Left Motor  -> 4Y -> O2.2 -> BLUE (D5)
// Left Motor  -> 3-4EN -> PWM (D10)


#define NUM_MOTORS	2

#define NUM_LEDS	4
#define ON 			true
#define OFF			false
#define DONT_CARE	2

class TopSpinShield {
public:
	TopSpinShield();
	/**
	 * Function stereotypes
	 */
	volatile uint8_t getCurrentBaseValue();
	volatile uint8_t getCurrentSideValue();
	void step(uint8_t motorIndex, int8_t direction, uint8_t initialSpeed, uint8_t endSpeed, int8_t speedStep, uint16_t delayValue, uint16_t repeatValue);
	void forward(uint8_t motorIndex, uint8_t speed);
	void backward(uint8_t motorIndex, uint8_t speed);
	void stop(uint8_t motorIndex);
	void setSpeed(uint8_t motorIndex, uint8_t speed);
	void initialize();
	void sample();
	void testOutput();
	void flash();
	void debounceInput( uint8_t pin, volatile uint8_t *currentValue, volatile uint8_t *currentCount, volatile uint8_t *change );
	void togglePin( uint8_t pin, uint8_t repeat, uint16_t duration );
	void move( uint8_t direction, uint8_t motorIndex, uint8_t speed);
	void moveTimed(uint8_t direction, uint8_t motorIndex, uint8_t speed, uint16_t duration);
	void setLeds(uint8_t led1, uint8_t led2, uint8_t led3, uint8_t led4);
	void setLed1(uint8_t value);
	void setLed2(uint8_t value);
	void setLed3(uint8_t value);
	void setLed4(uint8_t value);
	void updateLeds( uint8_t motorIndex, uint8_t direction );

private:
	volatile uint8_t led;
	volatile uint8_t currentBaseValue;
	volatile uint8_t currentSideValue;
	volatile uint8_t baseCount;
	volatile uint8_t sideCount;
	volatile uint8_t baseChange;
	volatile uint8_t sideChange;

};

#endif /* TOPSPINSHIELD_H_ */
