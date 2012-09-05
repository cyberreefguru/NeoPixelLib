/*
 * SequencePlayer.cpp
 *
 *  Created on: Jan 22, 2012
 *      Author: tom
 */

#include "SequencePlayer.h"

/** Define the static variable for SequencePlayer */
SequencePlayer Player;


SequencePlayer::SequencePlayer()
{
}

void SequencePlayer::play(Sequence *sequence, uint16_t len)
{
	uint16_t i;

	if( sequence->repeatValue == 0 )
	{
		while(1)
		{
			play( sequence->patterns, len);
		}
	}
	else
	{
		for(i=0; i<sequence->repeatValue; i++)
		{
			play( sequence->patterns, len);
		}
	}
}

void SequencePlayer::play(Pattern *patterns, uint16_t len)
{

	uint16_t i, j;

	for(i=0; i<len; i++)
	{
		for(j=0; j<patterns[i].repeat; j++)
		{
			if( patterns[i].holdBefore > 0 )
			{
				delay(patterns[i].holdBefore);
			}

			CharliePort.setPattern( patterns[i].frameData );

			if( patterns[i].holdAfter > 0 )
			{
				delay(patterns[i].holdAfter);
			}

		} // end for pattern.repeat

	} // end for pattern len

} // end play
