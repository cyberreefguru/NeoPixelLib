// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef Test5947_H_
#define Test5947_H_
#include "Arduino.h"
#include "Adafruit_TLC5947.h"
#include "HighSideDriver.h"


// ROW_CLK D4, ROW_DATA D5, ROW_LATCH D6, ROW_CLEAR D7
#define ROW_CLOCK	4
#define ROW_DATA	5
#define ROW_LATCH	6
#define ROW_OE  	-1
#define ROW_CLEAR 	7


#define COL_CLOCK 	8
#define COL_DATA	9
#define COL_LATCH	10
#define COL_CLEAR	-1
#define COL_OE		11




//end of add your includes here
#ifdef __cplusplus
	extern "C" {
#endif
	void loop();
	void setup();
#ifdef __cplusplus
	} // extern "C"
#endif

//add your function definitions for the project Test5947 here




//Do not add code below this line
#endif /* Test5947_H_ */
