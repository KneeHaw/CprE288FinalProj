/*
 * servo.h
 *
 *  Created on: Oct 25, 2023
 *      Author: kneehaw
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "button.h"

void servo_init_wrapper();

void servo_calibrate();

void turn_servo_to_deg(int degrees);

#endif /* SERVO_H_ */
