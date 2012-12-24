/*
 * SequencePlayer.cpp
 *
 *  Created on: Dec 31, 2010
 *      Author: tom
 */

#include "SequencePlayer.h"


/** Define the static variable for AnimationPlayer */
SequencePlayer sequencePlayer;


/**
 * Constructor
 */
SequencePlayer::SequencePlayer()
{
}

/**
 * Initialize routine
 */
void SequencePlayer::initializePlayer(pattern sequences[], uint16_t rows, uint16_t cols, uint16_t numberSeqs)
{
	// Save pointer to project data
	pSequences = sequences;
	numberRows = rows;
	numberCols = cols;
	numberSequences = numberSeqs;

} // end initialize
