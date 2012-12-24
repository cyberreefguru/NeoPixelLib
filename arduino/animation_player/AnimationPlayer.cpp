/*
 * AnimationPlayer.cpp
 *
 *  Created on: Apr 18, 2010
 *      Author: tmsasala
 */

#include "AnimationPlayer.h"

/** Define the static variable for AnimationPlayer */
AnimationPlayer Player;

AnimationPlayer::AnimationPlayer()
{
}

void AnimationPlayer::initializePlayer(sequence* sequences, uint16_t rows, uint16_t cols, uint16_t numberSeqs)
{
	// Save pointer to project data
	pSequences = sequences;
	numberRows = rows;
	numberCols = cols;
	numberSequences = numberSeqs;

} // end initialize

void AnimationPlayer::play()
{
	uint16_t seqCnt = 0;
	uint16_t frameCnt = 0;
//	uint8_t buf[numberRows*numberCols];

	boolean timerRunning = false;

	// loop over frame data
	while(seqCnt < numberSequences )
	{
		uint16_t numFrames = pSequences[seqCnt].numberFrames;
		uint16_t repeatValue = pSequences[seqCnt].repeatValue;

		for(uint16_t i=0; i<repeatValue; i++ )
		{
			while( frameCnt < numFrames )
			{
				uint16_t delayValue = pgm_read_word(&pSequences[seqCnt].frames[frameCnt].holdDuration);
				uint8_t *fd = (uint8_t *)pgm_read_word(&pSequences[seqCnt].frames[frameCnt].frameData);

//				for(uint8_t k=0; k<numberCols; k++)
//				{
//					buf[k] = pgm_read_byte(&fd[k]); // TODO: Needs to be row/col
//				}
				// Encode next set of values
				BcmPwm.encodeTimeslices( fd, 0xFF ); // TODO: BCM needs to be row/col

				if( timerRunning == false )
				{
					timerRunning = true;

					// Start the timer
					BcmPwm.enableTimer(0);
				}

				// Pause the specified amount of time
				delay( delayValue );

				// Advance frame counter
				frameCnt++;

			} // end while frameCnt

			// Reset frame counter
			frameCnt = 0;

		} // end repeat

		// Advance sequence counter
		seqCnt++;

	} // end while seqCnt

	// Reset sequence counter
	seqCnt = 0;
	//Timer2 Compare Match A Interrupt Enable
	TIMSK2 = 0x00;


} // end play


void AnimationPlayer::stop()
{

}

void AnimationPlayer::resume()
{

}

void AnimationPlayer::print()
{
	uint8_t c;
	uint8_t *fd;

	Serial.print("Project Rows: ");
	Serial.println( numberRows );
	Serial.print("Project Columns: ");
	Serial.println( numberCols );
	Serial.print("Number Sequences: ");
	Serial.println( numberSequences );

	for(uint16_t k=0; k<numberSequences; k++ )
	{
		Serial.print("Sequence: ");
		Serial.println( k );
		Serial.print("--Number Frames: ");
		Serial.println( pSequences[k].numberFrames );

		for(uint16_t j=0; j < pSequences[k].numberFrames; j++ )
		{
			Serial.print("--Frame: ");
			Serial.print( k );
			Serial.print(":");
			Serial.print( j );
			Serial.print(" (0x");
			Serial.print( (long int)(&pSequences[k].frames[j]), HEX );
			Serial.print(")\n----Duration: ");
			Serial.print( pgm_read_word(&pSequences[k].frames[j].holdDuration) );
			Serial.print( " (0x");
			Serial.print( (long int)(&pSequences[k].frames[j].holdDuration), HEX );
			Serial.print(")\n");

			Serial.print("----Frame Data: { ");

			fd = (uint8_t *)pgm_read_word(&pSequences[k].frames[j].frameData);
			for(uint16_t row=0; row < numberRows; row++)
			{
				Serial.print("{");
				for(uint16_t col=0; col<numberCols; col++)
				{
					c = pgm_read_byte(&fd[row+col]);

					if(c < 0xf )
					{
						Serial.print("0");
					}
					Serial.print( c, HEX );
					Serial.print(" ");
				}
				Serial.print("} ");

			} // end for frame data

			Serial.println("};");
		} //end for frames
	} // end for seqs
} // end print

