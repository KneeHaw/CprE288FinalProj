/*
 * movement.h
 *
 *  Created on: Sep 3, 2023
 *      Author: joo
 */
#ifndef MOVEMENT_H_
#define MOVEMENT_H_


#include "open_interface.h"
#include "stdbool.h"


#define MOVE_RIGHT -85
#define MOVE_LEFT 85
#define MOVE_RIGHT_SMALL -5
#define MOVE_LEFT_SMALL 5


/**
 * function to move cybot forwared by particular dist
 * @param oi_t* sensor struct
 * @param int centimeters
 */
void move_forward(oi_t *sensor, int centimeters);
/**
 * function to move cybot backwards by particular dist
 * @param oi_t* sensor struct
 * @param int centimeters
 */
void move_backward(oi_t *sensor, int centimeters);
/**
 * function to move cybot forward by particular distance, also accounting for bumps as it tries to avoid them.
 * @param oi_t* sensor struct
 * @param int centimeters
 */
bool move_forward_with_bumps(oi_t *sensor, int centimeters);
/**
 * function to move cybot forwared by particular dist but stop if any bumps
 * @param oi_t* sensor struct
 * @param int centimeters
 */
bool move_forward_stop_bumps(oi_t *sensor, int centimeters);
/**
 * function to turn cybot by degrees.
 * @param oi_t* sensor struct
 * @param double degrees,  negative means right, positive means left
 */
void turn_clockwise(oi_t *sensor, double degrees);


#endif /* MOVEMENT_H_ */


