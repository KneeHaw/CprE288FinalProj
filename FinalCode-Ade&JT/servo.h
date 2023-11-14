/*
 * servo.h
 *
 *  Created on: Oct 29, 2023
 *      Author: joo
 */

#ifndef SERVO_H_
#define SERVO_H_


#include "lcd.h"
#include "buttons.h"


void servo_init(void);
int servo_move(float degrees);
void servo_calibrate();


#endif /* SERVO_H_ */
