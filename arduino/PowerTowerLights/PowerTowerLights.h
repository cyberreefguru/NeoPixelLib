// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef PowerTowerLights_H_
#define PowerTowerLights_H_

#include <Arduino.h>
//#include "LedShieldDriver.h"
#include "LedShieldDriver2.h"
#include "FlexiTimer2.h"


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
