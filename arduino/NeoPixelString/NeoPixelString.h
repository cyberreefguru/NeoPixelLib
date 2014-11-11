// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef NeoTest_H_
#define NeoTest_H_

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include "RGB.h"


#define RED		0x00FF0000
#define BLUE	0x000000FF
#define GREEN	0x0000FF00
#define MAGENTA	RED | BLUE
#define CYAN	BLUE | GREEN
#define YELLOW	RED | GREEN
#define ORANGE	0xFF3300
#define PURPLE	0x990099


//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project NeoTest here




//Do not add code below this line
#endif /* NeoTest_H_ */
