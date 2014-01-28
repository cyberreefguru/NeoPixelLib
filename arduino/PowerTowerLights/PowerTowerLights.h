// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef PowerTowerLights_H_
#define PowerTowerLights_H_

#include <Arduino.h>
#include "LedShieldDriver.h"
#include "LedShieldDriver2.h"
#include "FlexiTimer2.h"

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

#define NUM_ROW_DRIVERS 	1
#define CHANNELS_PER_ROW 	24

#define NUM_COL_DRIVERS 	1
#define CHANNELS_PER_COL 	8

#define ROWS	24
#define COLS	8

//#define __DEBUG


//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif


#endif /* PowerTowerLights_H_ */
