/*
 * SequencePlayer.h
 *
 *  Created on: Jan 22, 2012
 *      Author: tom
 */

#ifndef SEQUENCEPLAYER_H_
#define SEQUENCEPLAYER_H_

#include "Charlieplex.h"

#define LEFT 		0
#define RIGHT		1

// Define the frame structure
struct Pattern {
	uint16_t repeat;
	uint16_t holdBefore;
	uint16_t holdAfter;
	uint16_t frameData;
};

// Define the frame structure
struct Sequence {
	uint16_t repeatValue;
	Pattern *patterns;
};


#ifdef __cplusplus


class SequencePlayer
{
public:
	SequencePlayer();
	void initialize();
	void play(Pattern *p, uint16_t len);
	void play(Sequence *s, uint16_t len);
	void stop();
	void resume();
	void print();

private:


};

#else

typedef
	struct SequencePlayer
		SequencePlayer;


#endif // end if c++

#ifdef __cplusplus
extern "C" {
#endif

	// Define static class for C code to access
	extern SequencePlayer Player;


#ifdef __cplusplus
} // end extern C
#endif


#endif /* SEQUENCEPLAYER_H_ */
