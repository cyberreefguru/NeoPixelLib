/*
 Copyright (c) 2008, Nigel Batten.
 */

#include "bcm.h"

volatile unsigned char current_buffer; // pointer to current timeslice
volatile unsigned char flag_change_buffer; // flag indicating time to update timeslice buffer
volatile uint8_t bcm_timeslice[2][BCM_SIZE]; // one byte for each bit-position being displayed on a port.

volatile uint8_t bcm_bitpos; // which bit position is currently being shown
unsigned char bcm_output_port;

void bcm_initialize_timer();

/**
 * The setup() method runs once, when the sketch starts
 **/
void bcm_initialize(unsigned char port)
{
	// Initialize variables
	//bcm_tick = 0;
	bcm_bitpos = 0;
	bcm_output_port = port;
	current_buffer = 0;
	flag_change_buffer = false;

	for(int i=0; i< BCM_SIZE; i++ )
	{
		bcm_timeslice[0][i] = 0;
		bcm_timeslice[1][i] = 0;
	}

	// Initialize timer
	bcm_initialize_timer();

} // end bcm_initialize

/**
 * Initializes the timer
 *
 */
void bcm_initialize_timer()
{
	TCCR2A = 0x02; // set the timer to CTC mode; disable output compare pin
	TCCR2B = 0x06; // clk/256 = 16us tclk
//	TCCR2B = 0x04; // clk/64 = 4us tclk

	// Set initial compare value
	OCR2A = 0x01;

} // end initialize_timer

/**
 * Enables the timer.  With the Arudino, interrupts are enabled when the program
 * starts, so GIE is meaningless -- we need to ensure the timer interrupt does not
 * fire until we are ready for it.
 *
 * @param timerValue the time value to compare to
 */
void bcm_enable_timer(unsigned char timerValue)
{
	// Set timer count
	TCNT2 = timerValue;

	//Timer2 Compare Match A Interrupt Enable
	TIMSK2 = 0x02;

} // end bcm_enable_timer

/**
 * Encode an array of 8 LED brightness bytes into the pattern
 * to be shown on the port for each of the 8 timeslices.
 * There are 8 LEDs and 8 timeslices.  For the nth timeslide,
 * pull the nth bit from each intensity value and form a full
 * byte.  In effect, turn the intensity array
 * on it's "side" and use the byte to turn on or off LED.
 *
 * If we knew the pattern in advance, we could precompute this.
 * But instead we need to recompute each cycle to capture the
 * newly computed intensity value.
 *
 * @param intensity - the intensity values to encode
 * Sets bcm_timeslice to the encoded intensity value
 *
 * NOTE: This routine modifies bcm_timeslice which is what is being
 * "shown" on the output port.  It should block interrupts until
 * the encoding is complete to avoid writing incomplete values
 * to the output port.  It currently DOES NOT disable interrupts.
 *
 */
void bcm_encode_timeslices(uint8_t intensity[], unsigned char mask)
{
	unsigned char next_buffer;
	uint8_t portbits;
	uint8_t bitvalue;

	// Wait until interrupt clears flag
	while(flag_change_buffer == true )
	{}

	// Compute the next "free" buffer (so we don't update the active buffer)
	next_buffer = (current_buffer+1) & 0x01;

	// portbits represents the value to write to the output port for a specified timeslice
	// bitvalue represents a "floating" 1 used to add a 1 to the portbits output value.
	// ledpos represents the position of the LED on the output port
	// bitpos represent the bit position or time slice index that we are encoding

	// takes the nth bit from every intensity byte (starting at 0), and builds a single byte
	// representing the "on" value for the particular cycle.  There are 8 cycles
	// before the phase is repeated.  The timeslices are repeated for a duration
	// set outside the ISR.

	// Cycle from 0 to BCM_SIZE, setting the internal intensity value
	// to the appropriate value based on the desired "brightness" represented
	// by the intensity byte array (byte = 256 brightness levels)

	for (uint8_t bitpos = 0; bitpos < BCM_SIZE; bitpos++)
	{
		portbits = 0; // value of the port for this timeslice (1, 2, 4, 8, ..., 256)
		bitvalue = 1; // floating 1 representing the current bit position
		for (uint8_t ledpos = 0; ledpos < BCM_SIZE; ledpos++)
		{
			if( mask & bitvalue )
			{
				// if there is a 1 in the led position, set the port value to a 1
				if (intensity[ledpos] & (1 << bitpos))
				{
					portbits |= bitvalue;
				}
			}
			bitvalue = bitvalue << 1;

		} // end ledpos
		bcm_timeslice[next_buffer][bitpos] = portbits;

	} // end bitpos

	// Set flag to true so ISR starts using new value
	flag_change_buffer = true;

} // end bcm_encode_timeslice


///**
// * The interrupt service routine for timer 2
// *
// * Called once per bit position.
// *
// */
//ISR( TIMER2_COMPA_vect )
//{
//	// Increase time slice counter
//	bcm_bitpos++; // NOTE: THIS CAUSES THE FIRST CYCLE TO WRITE OUT TIMESLICE 1 INSTEAD OF 0
//
//	// mask upper bits; if value > 7, it becomes less than 7 (usually zero).
//	// basically rolls the counter from 7 to zero without a compare
//	bcm_bitpos &= (BCM_SIZE-1); // NOTE: this only works if BCM_SIZE is a power of 2.
//
//	IoPort.write_value(bcm_output_port, bcm_timeslice[current_buffer][bcm_bitpos]);
//
//	// now set the delay...
//	TCNT2 = 0; // reset the timer
//	OCR2A <<= 1; // the next delay is just the last delay shifted left once (e.g., ^2)
//
//	if (bcm_bitpos == 0)
//	{
//		OCR2A = 1; // reset the compare match value.
//	}
//
//	// Check if all time slices have been written out
//	if (bcm_bitpos == (BCM_SIZE - 1))
//	{
//		if( flag_change_buffer == true )
//		{
//			// Use next available buffer
//			current_buffer = (current_buffer+1) & 0x01;
//
//			// Reset the flag
//			flag_change_buffer = false;
//		}
//	}
//
//} // end ISR




/**
 * The interrupt service routine for timer 2
 *
 * Called once per bit position.
 *
 */
ISR( TIMER2_COMPA_vect )
{
	IoPort.write_value(bcm_output_port, bcm_timeslice[current_buffer][bcm_bitpos]);

	// now set the delay...
	TCNT2 = 0; // reset the timer
	OCR2A <<= 1; // the next delay is just the last delay shifted left once (e.g., ^2)

	// Increase time slice counter
	bcm_bitpos++;

	// mask upper bits; if value > 7, it becomes less than 7 (usually zero).
	// basically rolls the counter from 7 to zero without a compare
	bcm_bitpos &= (BCM_SIZE-1); // NOTE: this only works if BCM_SIZE is a power of 2.


	if (bcm_bitpos == 0)
	{
		OCR2A = 1; // reset the compare match value.
		if( flag_change_buffer == true )
		{
			// Use next available buffer
			current_buffer = (current_buffer+1) & 0x01;

			// Reset the flag
			flag_change_buffer = false;
		}
	}

} // end ISR
