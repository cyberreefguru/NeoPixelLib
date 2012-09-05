/*
 * TopSpin.h
 *
 *  Created on: Dec 26, 2011
 *      Author: tom
 */

#ifndef CHARLIEPLEX_H_
#define CHARLIEPLEX_H_

#include <WProgram.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <pins_arduino.h>
#include <FlexiTimer2.h>

#define LED_PIN		13

#ifdef __cplusplus

class Charlieplex
{
public:
	Charlieplex();

	void clear();
	void initialize(uint8_t *dataPort, uint8_t *controlPort, uint8_t portMask,
					uint8_t (*pins)[2], uint8_t numPins);
	void enable(uint8_t flag);
	void setPattern(uint16_t value);
	uint16_t getPattern();
	static void drivePattern();

};

#else

typedef
	struct Charlieplex
		Charlieplex;

#endif // end if c++

#ifdef __cplusplus
extern "C" {
#endif

void clear();
void initialize(uint8_t *dataPort, uint8_t *controlPort, uint8_t portMask,
				uint8_t (*pins)[2], uint8_t numPins);
void enable(uint8_t flag);
void setPattern(uint16_t value);
uint16_t getPattern();
void drivePattern();

extern Charlieplex CharliePort;


#ifdef __cplusplus
} // end extern C
#endif


#endif /* CHARLIEPLEX_H_ */
