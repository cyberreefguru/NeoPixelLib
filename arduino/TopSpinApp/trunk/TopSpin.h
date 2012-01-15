/*
 * TopSpin.h
 *
 *  Created on: Dec 26, 2011
 *      Author: tom
 */

#ifndef TOPSPIN_H_
#define TOPSPIN_H_

#include <WProgram.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <FlexiTimer2.h>

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

#define MAIN_ARM	0 // LEFT
#define CHAIR		1 // RIGHT

#define BASE_INPUT_PIN	11
#define SIDE_INPUT_PIN	12

//#define BASE_STATUS_PIN	6
//#define SIDE_STATUS_PIN	7

#define FORWARD 	0
#define BACKWARD 	1
#define STOP 		2

// Base -> In2
// Side -> In3
// Right Motor -> 1Y -> O1.1 -> RED
// Right Motor -> 2Y -> O1.2 -> BLUE
// Left Motor  -> 3Y -> O2.1 -> RED
// Left Motor  -> 4Y -> O2.2 -> BLUE


#define NUM_MOTORS	2
uint8_t motor[2][3] = { {2, 3, 9}, {4, 5, 10} }; // Input 1, Input 2, Enable

#define NUM_LEDS	4
#define ON 			true
#define OFF			false
#define DONT_CARE	2

uint8_t leds[4] = {0,1,2,3}; // Analog port (Port C)



#endif /* TOPSPIN_H_ */
