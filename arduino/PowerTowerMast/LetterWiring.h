/*
 * LetterWiring.h
 *
 *  Created on: Dec 28, 2014
 *      Author: tsasala
 */

#ifndef LETTERWIRING_H_
#define LETTERWIRING_H_

#define LETTER_P_SIZE	8
#define LETTER_O_SIZE	8
#define LETTER_W_SIZE	7
#define LETTER_E_SIZE	7
#define LETTER_R_SIZE	8
#define LETTER_T_SIZE	5

// POWER TOWER
#define NUM_SEGMENTS	(LETTER_P_SIZE + LETTER_O_SIZE*2 + LETTER_W_SIZE*2 + LETTER_E_SIZE*2 + LETTER_R_SIZE*2 + LETTER_T_SIZE)
#define NUM_LETTERS		10

uint8_t LETTER_P1[LETTER_P_SIZE][2] = { {2,4}, {3,4}, {1,4}, {0,4}, {4,4}, {6,4}, {7,4}, {5,4} };
uint8_t LETTER_O1[LETTER_O_SIZE][2] = { {7,2}, {5,2}, {3,2}, {4,2}, {1,2}, {0,2}, {6,2}, {2,2} };
uint8_t LETTER_W1[LETTER_W_SIZE][2] = { {3,3}, {4,3}, {1,3}, {0,3}, {5,3}, {6,3}, {2,3} };
uint8_t LETTER_E1[LETTER_E_SIZE][2] = { {5,1}, {2,1}, {6,1}, {3,1}, {4,1}, {1,1}, {0,1} };
uint8_t LETTER_R1[LETTER_R_SIZE][2] = { {1,0}, {6,0}, {5,0}, {7,0}, {2,0}, {4,0}, {3,0}, {0,0} };

uint8_t LETTER_T1[LETTER_T_SIZE][2] = { {14,4}, {12,4}, {10,4}, {15,4}, {9,4} };
uint8_t LETTER_O2[LETTER_O_SIZE][2] = { {10,2}, {12,2}, {13,2}, {15,2}, {9,2}, {8,2}, {11,2}, {14,2} };
uint8_t LETTER_W2[LETTER_W_SIZE][2] = { {14,3}, {15,3}, {9,3}, {8,3}, {11,3}, {13,3}, {12,3} };
uint8_t LETTER_E2[LETTER_E_SIZE][2] = { {10,1}, {12,1}, {14,1}, {13,1}, {8,1}, {15,1}, {9,1} };
uint8_t LETTER_R2[LETTER_R_SIZE][2] = { {9,0}, {13,0}, {12,0}, {10,0}, {14,0}, {8,0}, {11,0}, {15,0} };


uint16_t LETTERS[NUM_LETTERS] = { (uint16_t)&LETTER_P1[0][0], (uint16_t)&LETTER_O1[0][0], (uint16_t)&LETTER_W1[0][0],
		(uint16_t)&LETTER_E1[0][0], (uint16_t)&LETTER_R1[0][0], (uint16_t)&LETTER_T1[0][0],
		(uint16_t)&LETTER_O2[0][0], (uint16_t)&LETTER_W2[0][0], (uint16_t)&LETTER_E2[0][0],
		(uint16_t)&LETTER_R2[0][0]};

uint8_t LETTER_SIZE[NUM_LETTERS] = {LETTER_P_SIZE, LETTER_O_SIZE, LETTER_W_SIZE, LETTER_E_SIZE, LETTER_R_SIZE,
		LETTER_T_SIZE, LETTER_O_SIZE, LETTER_W_SIZE, LETTER_E_SIZE, LETTER_R_SIZE};


#endif /* LETTERWIRING_H_ */
