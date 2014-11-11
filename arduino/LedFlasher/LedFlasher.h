// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef LedFlasher_H_
#define LedFlasher_H_

#include "Arduino.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <EEPROM.h>

#define __DEBUG TRUE

#define NUM_OUTPUTS 6
#define NUM_INPUTS 8

#define INTERRUPT_SOURCE	0 // Interrupt Source = INT0

//#define ON 		255
//#define OFF		0

#define UP		1
#define DOWN	0

#define CONFIG_ADDRESS		0x20 	// address where last config is saved
#define CONFIG_UNSET		255 	// value of config if unset

// Hardware Specific Pin Mappings

// Inputs
#define SW1					2
#define SW2					4
#define POT1				A0
#define POT2				A1
#define POT3 				A2
#define POT4				A3
#define POT5				A4
#define POT6				A5

// Outputs
#define OUT1				3
#define OUT2				5
#define OUT3				6
#define OUT4				9
#define OUT5				10
#define OUT6				11

#define IN_LOW				14		// lowest possible input analog value
#define IN_HIGH				255 	// highest possible input analog value

#define OFF					0
#define ON					1

#define MIN_VALUE			0
#define MAX_VALUE			135



//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project LedFlasher here




//Do not add code below this line
#endif /* LedFlasher_H_ */
