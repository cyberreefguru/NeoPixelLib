/*
 * LedTest.h
 *
 *  Created on: Dec 26, 2011
 *      Author: tom
 */

#ifndef LED_TEST_H_
#define LED_TEST_H_

#include <WProgram.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
//#include <FlexiTimer2.h>


#define LED_PIN		13

uint8_t led[3] = { 9,10,11 };
#define RED 	0x02
#define GREEN	0x04
#define YELLOW	0x08

#define OFF 	0x00
#define ON		RED | GREEN | YELLOW

typedef struct {
  uint8_t pattern;
  uint16_t duration;
} PATTERN;

typedef struct {
  uint8_t len;
  uint8_t repeat;
  PATTERN initialPattern;
  PATTERN finalPattern;
  PATTERN *patterns;

} SEQUENCE;



#endif /* LED_TEST_H_ */
