/*
 * TopSpinApp.h
 *
 *  Created on: Aug 20, 2013
 *      Author: tsasala
 */

#ifndef TOPSPINAPP_H_
#define TOPSPINAPP_H_

#include "TopSpinShield.h"

#define MAIN_ARM	0 // LEFT
#define CHAIR		1 // RIGHT


class TopSpinApp {
public:
	TopSpinApp();
	void initialize();
	void start();
	void findHome();
	void fineTune();
	void rotateMainArm(uint8_t direction, uint8_t repeat, uint8_t speed);
	void rotateChair(uint8_t direction, uint16_t duration, uint8_t speed);

};

#endif /* TOPSPINAPP_H_ */
