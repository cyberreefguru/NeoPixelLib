/*
 * SequencePlayer.h
 *
 *  Created on: Dec 31, 2010
 *      Author: tom
 */

#ifndef SEQUENCEPLAYER_H_
#define SEQUENCEPLAYER_H_

#include "IoShield.h"

// Define the frame structure
struct pattern {
	uint16_t repeatValue;
	uint16_t holdValue;
	uint8_t *frameData;
};

#ifdef __cplusplus


class SequencePlayer
{
public:
	SequencePlayer();
	void initializePlayer(pattern* sequences, uint16_t numberRows, uint16_t numberCols, uint16_t numberSequences);
	void play();
	void stop();
	void resume();
	void print();

private:
	pattern* pSequences;
	uint16_t numberRows;
	uint16_t numberCols;
	uint16_t numberSequences;

};


#else

typedef
	struct SequencePlayer
		SequencePlayer;


#endif // end if c++


#ifdef __cplusplus
extern "C" {
#endif

	// Define class methods for C code to access
	void initializePlayer(pattern *sequences, uint16_t numberRows, uint16_t numberCols, uint16_t numberSequences);
	void play();
	void stop();
	void resume();
	void print();

	// Define static class for C code to access
	extern SequencePlayer sequencePlayer;


#ifdef __cplusplus
} // end extern C
#endif



#endif /* SEQUENCEPLAYER_H_ */
