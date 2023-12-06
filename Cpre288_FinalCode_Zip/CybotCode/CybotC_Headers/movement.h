/*
 * movement.h
 *
 *  Created on: Sep 5, 2023
 *      Author: Ethan Rogers (kneehaw@iastate.edu)
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcd.h"
#include "scan.h"
#include "open_interface.h"
#include "Timer.h"

void turn_deg(oi_t* self, float deg, int16_t speed);

float do_stop_sign(oi_t* self);

void move_distance(oi_t* self, float dist, int16_t speed, int bump, int check_objs, bool handling_bocl);

#endif /* MOVEMENT_H_ */
