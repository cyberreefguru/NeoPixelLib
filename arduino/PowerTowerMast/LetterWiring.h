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

uint8_t LETTER_P1[LETTER_P_SIZE][2] = { {0,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {1,0}, {2,0} };
uint8_t LETTER_O1[LETTER_O_SIZE][2] = { {1,2}, {2,2}, {3,2}, {6,2}, {4,2}, {5,2}, {7,2}, {0,2} };
uint8_t LETTER_W1[LETTER_W_SIZE][2] = { {3,4}, {6,4}, {4,4}, {5,4}, {2,4}, {7,4}, {0,4} };
uint8_t LETTER_E1[LETTER_E_SIZE][2] = { {2,3}, {0,3}, {7,3}, {3,3}, {6,3}, {4,3}, {5,3} };
uint8_t LETTER_R1[LETTER_R_SIZE][2] = { {2,1}, {7,1}, {4,1}, {5,1}, {3,1}, {6,1}, {0,1}, {1,1} };

uint8_t LETTER_T1[LETTER_T_SIZE][2] = { {14,0}, {12,0}, {15,0}, {11,0}, {9,0} };
uint8_t LETTER_O2[LETTER_O_SIZE][2] = { {15,2}, {12,2}, {8,2}, {11,2}, {9,2}, {13,2}, {10,2}, {14,2} };
uint8_t LETTER_W2[LETTER_W_SIZE][2] = { {14,4}, {11,4}, {9,4}, {13,4}, {10,4}, {8,4}, {12,4} };
uint8_t LETTER_E2[LETTER_E_SIZE][2] = { {15,3}, {12,3}, {14,3}, {8,3}, {13,3}, {11,3}, {9,3} };
uint8_t LETTER_R2[LETTER_R_SIZE][2] = { {12,1}, {8,1}, {9,1}, {10,1}, {11,1}, {13,1}, {14,1}, {15,1} };

#define NUM_LETTERS		10

uint16_t LETTERS[NUM_LETTERS] = { (uint16_t)&LETTER_P1[0][0], (uint16_t)&LETTER_O1[0][0], (uint16_t)&LETTER_W1[0][0],
		(uint16_t)&LETTER_E1[0][0], (uint16_t)&LETTER_R1[0][0], (uint16_t)&LETTER_T1[0][0],
		(uint16_t)&LETTER_O2[0][0], (uint16_t)&LETTER_W2[0][0], (uint16_t)&LETTER_E2[0][0],
		(uint16_t)&LETTER_R2[0][0]};

uint8_t LETTER_SIZE[NUM_LETTERS] = {LETTER_P_SIZE, LETTER_O_SIZE, LETTER_W_SIZE, LETTER_E_SIZE, LETTER_R_SIZE,
		LETTER_T_SIZE, LETTER_O_SIZE, LETTER_W_SIZE, LETTER_E_SIZE, LETTER_R_SIZE};



#endif /* LETTERWIRING_H_ */
