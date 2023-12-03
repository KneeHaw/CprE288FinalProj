/*
 * movement.h
 *
 *  Created on: Sep 5, 2023
 *      Author: kneehaw
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
#include "open_interface.h"
#include "Timer.h"
#include "movement.h"
#include "scan.h"

void turn_deg(oi_t* self, float deg, int16_t speed);

void gyro_turn_deg(float deg, int16_t speed);
/*
 * Given oi_t sensor data and degrees
 * Turns left at speed "speed" left if degrees less than 0
 * Otherwise turns right
 * */

void move_distance(oi_t *self, double dist, int16_t speed, int bump);
/*
 * Given oi_t sensor data and distance (mm)
 * Moves both wheels at speed "speed" until distance has been reached
 * */

#endif /* MOVEMENT_H_ */
