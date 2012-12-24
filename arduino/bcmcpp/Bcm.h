#ifndef Bcm_h
#define Bcm_h

#include <WProgram.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "IoShield.h"


#define BCM_SIZE 8

#ifdef __cplusplus

extern "C" void TIMER2_COMPA_vect(void) __attribute__ ((signal));

class Bcm
{

private:
	volatile unsigned char current_buffer; // pointer to current timeslice
	volatile unsigned char flag_change_buffer; // flag indicating time to update timeslice buffer
	volatile uint8_t bcm_timeslice[2][BCM_SIZE]; // one byte for each bit-position being displayed on a port.

	volatile uint8_t bcm_bitpos; // which bit position is currently being shown
	unsigned char bcm_output_port;

public:
	Bcm();
	void initialize(unsigned char port);
	void initializeTimer();
	void enableTimer(unsigned char timerValue);
	void encodeTimeslices(uint8_t intensity[], unsigned char mask);

	friend void TIMER2_COMPA_vect();

};

#else

typedef
	struct Bcm
		Bcm;

#endif // end if c++

#ifdef __cplusplus
extern "C" {
#endif

	void initialize(unsigned char port);
	void initializeTimer();
	void enableTimer(unsigned char timerValue);
	void encodeTimeslices(uint8_t intensity[], unsigned char mask);

	extern Bcm BcmPwm;


#ifdef __cplusplus
} // end extern C
#endif


#endif // end ifndef Bcm_h

