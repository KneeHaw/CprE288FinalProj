/*
 *  movement.c
 *
 *  Created on: Sep 3, 2023
 *      Author: joo & AdeIfe
 */


#include "movement.h"


/**
 * function to move cybot forwared by particular dist
 * @param oi_t* sensor struct
 * @param int centimeters
 */
void move_forward(oi_t *sensor, int centimeters)
{
    double sum = 0;
    oi_update(sensor); // clear sensors
    oi_setWheels(250, 250); // move forward; full speed

    while (sum < (centimeters * 10)) {
        oi_update(sensor);
        sum += sensor->distance;
    }

    oi_setWheels(0, 0); // stop
}


/**
 * function to move cybot backwards by particular dist
 * @param oi_t* sensor struct
 * @param int centimeters
 */
void move_backward(oi_t *sensor, int centimeters)
{
    double sum = 0;
    oi_update(sensor); // clear sensors
    oi_setWheels(-250, -250); // move backward; full speed

    while (sum > (centimeters * -10)) {
        oi_update(sensor);
        sum += sensor->distance;
    }

    oi_setWheels(0, 0); // stop
}


/**
 * function to move cybot forward by particular distance, also accounting for bumps as it tries to avoid them.
 * @param oi_t* sensor struct
 * @param int centimeters
 */
bool move_forward_with_bumps(oi_t *sensor, int centimeters)
{
    double sum = 0;
    bool ans = false;
    oi_update(sensor); // clear sensors
    oi_setWheels(250, 250); // move forward; full speed

    while (sum < (centimeters * 10)) {
        oi_update(sensor);
        sum += sensor->distance;

        if(sensor->bumpLeft == 1){
            move_backward(sensor, 10);    //back up 15cm
            turn_clockwise(sensor, MOVE_RIGHT);  //turn right
            move_forward(sensor, 20);     //forward 25cm
            turn_clockwise(sensor, MOVE_LEFT);   //turn left
            centimeters -= 10;
            oi_update(sensor);
            ans = true;
        }
        else if(sensor->bumpRight == 1){
             move_backward(sensor, 10);    //back up 15cm
             turn_clockwise(sensor, MOVE_LEFT);  //turn left
             move_forward(sensor, 20);     //forward 25cm
             turn_clockwise(sensor, MOVE_RIGHT);   //turn right
             centimeters -= 10;
             oi_update(sensor);
             ans = true;
        }

        oi_setWheels(250, 250);
    }

    oi_setWheels(0, 0); // stop
    return ans;
}


/**
 * function to move cybot forwared by particular dist but stop and returns true if any bumps
 * @param oi_t* sensor struct
 * @param int centimeters
 */
bool move_forward_stop_bumps(oi_t *sensor, int centimeters)
{
    double sum = 0;
    oi_update(sensor); // clear sensors
    oi_setWheels(250, 250); // move forward; full speed

    while (sum < (centimeters * 10)) {
        oi_update(sensor);
        sum += sensor->distance;

        if(sensor->bumpLeft == 1 || sensor->bumpRight == 1) return true;
    }

    oi_setWheels(0, 0); // stop
    return false;
}


/**
 * function to turn cybot by degrees.
 * @param oi_t* sensor struct
 * @param double degrees,  negative means right, positive means left
 */
void turn_clockwise(oi_t *sensor, double degrees)
{
    double turn = 0;
    oi_update(sensor); // clear sensors

    if (degrees > 0){
        oi_setWheels(100, -100);
        while (turn <= degrees){
                oi_update(sensor);
                turn += sensor->angle;
        }
    } else {
        oi_setWheels(-100, 100);
        while (turn >= degrees){
                oi_update(sensor);
                turn += sensor->angle;
        }
    }

    oi_setWheels(0, 0); //STOP
}



