#ifndef BcmProgMem_h
#define BcmProgMem_h

#include <WProgram.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "IoShield.h"


#define BCM_SIZE 8

#ifdef __cplusplus

extern "C" void TIMER2_COMPA_vect(void) __attribute__ ((signal));

class BcmProgMem
{

private:
	volatile uint8_t current_buffer; // pointer to current timeslice
	volatile uint8_t flag_change_buffer; // flag indicating time to update timeslice buffer
	volatile uint8_t bcm_timeslice[2][BCM_SIZE]; // one byte for each bit-position being displayed on a port.

	volatile uint8_t bcm_bitpos; // which bit position is currently being shown
	uint8_t bcm_output_port;

public:
	BcmProgMem();
	void initialize(uint8_t port);
	void initializeTimer();
	void enableTimer(uint8_t timerValue);
	void encodeTimeslices(uint8_t intensity[], uint8_t mask);

	friend void TIMER2_COMPA_vect();

};

#else

typedef
	struct BcmProgMem
	BcmProgMem;

#endif // end if c++

#ifdef __cplusplus
extern "C" {
#endif

	void initialize(uint8_t port);
	void initializeTimer();
	void enableTimer(uint8_t timerValue);
	void encodeTimeslices(uint8_t intensity[], uint8_t mask);

	extern BcmProgMem BcmPwm;


#ifdef __cplusplus
} // end extern C
#endif


#endif // end ifndef BcmProgMem_h

