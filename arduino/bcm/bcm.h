/*
 * Bcm.h
 *
 *  Created on: Feb 9, 2010
 *      Author: tmsasala
 */
#ifndef BCM_H_
#define BCM_H_

#include <WProgram.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "IoShield.h"

#define BCM_SIZE 8


#ifdef __cplusplus
extern "C"{
#endif

void bcm_initialize(unsigned char port);
void bcm_enable_timer(unsigned char timerValue);
void bcm_encode_timeslices( uint8_t intensity[], unsigned char mask );


//extern volatile uint8_t bcm_timeslice[BCM_SIZE] ; // one byte for each bit-position being displayed on a port.
//extern volatile uint8_t bcm_tick;
//extern volatile uint8_t bcm_bitpos; // which bit position is currently being shown
//extern unsigned char bcm_brightness[BCM_SIZE]; // brightness for each LED
//extern unsigned char bcm_output_port;


#ifdef __cplusplus
} // extern "C"
#endif


#endif /* BCM_H_ */

