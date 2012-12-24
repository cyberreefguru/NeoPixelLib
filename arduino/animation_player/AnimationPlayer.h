/*
 * AnimationPlayer.h
 *
 *  Created on: Apr 18, 2010
 *      Author: tmsasala
 */

#ifndef ANIMATION_PLAYER_H_
#define ANIMATION_PLAYER_H_

#include "BcmProgMem.h"

// Define the frame structure
struct frame {
	// TODO Define repeat value for frame?
	prog_uint16_t holdDuration;
	prog_uint8_t *frameData;
};

// Define the sequence structure
struct sequence {
	prog_uint16_t repeatValue;
	prog_uint16_t numberFrames;
	frame *frames;
};

#ifdef __cplusplus

class AnimationPlayer
{
public:
	AnimationPlayer();
	void initializePlayer(sequence* sequences, uint16_t numberRows, uint16_t numberCols, uint16_t numberSequences);
	void play();
	void stop();
	void resume();
	void print();

private:
	sequence* pSequences;
	uint16_t numberRows;
	uint16_t numberCols;
	uint16_t numberSequences;


};

#else

typedef
	struct AnimationPlayer
		AnimationPlayer;

#endif // end if c++

#ifdef __cplusplus
extern "C" {
#endif

	// Define class methods for C code to access
	void initializePlayer(sequence* sequences, uint16_t numberRows, uint16_t numberCols, uint16_t numberSequences);
	void play();
	void stop();
	void resume();
	void print();

	// Define static class for C code to access
	extern AnimationPlayer Player;


#ifdef __cplusplus
} // end extern C
#endif


#endif /* ANIMATION_PLAYER_H_ */
