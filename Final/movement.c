/*
 * movement.h
 *
 *  Created on: Sep 5, 2023
 *      Author: kneehaw
 */

#include "open_interface.h"
#include "movement.h"
#include "Timer.h"
#include "init_imu.h"
#include "imu.h"
#include "bno055.h"
#include "math.h"
#include "boundary.h"

#define turn_speed 30

int turn_direction = 0;

void get_deg_sign(float deg) {
    if (deg < 0) {
        turn_direction = -1;
    }
    else {
        turn_direction = 1;
    }
}

float get_correct_curr(float start_ang, float curr_ang) {
    if ((start_ang > 350) && (curr_ang < 10)) {
        return (start_ang - (360 + curr_ang));
    }
    else if ((start_ang < 10) && (curr_ang > 350)) {
        return (start_ang + (360 - curr_ang));
    }
    else {
        return (curr_ang);
    }
}

float get_correct_difference_orientation(float start_ang, float curr_ang) {
    if ((start_ang > curr_ang) && (turn_direction > 0)) { // right turn detect wrap from 360 to 0 (positive angle!)
        return fabsf(360 - start_ang + curr_ang);
    }
    else if ((start_ang < curr_ang) && (turn_direction < 0)) {
        return fabsf(curr_ang - 360 - start_ang); //left turn detect wrap from 0 to 360 (negative angle!)
    }
    else{
        return  fabsf(start_ang - curr_ang);
    }
}

float get_correction_factor(float start_ang, float curr_ang) {
    curr_ang = get_correct_curr(start_ang, curr_ang);

    return fabsf(curr_ang / start_ang);
}

void turn_deg(float deg, int16_t speed) {// (deg, mm/s)
    deg = deg * 89/90;
    get_deg_sign(deg);
    float deg_traveled;
    struct bno055_euler_float_t starting_angles;
    struct bno055_euler_float_t current_angles;
    bno055_convert_float_euler_hpr_deg(&starting_angles);

    //rph = xyz, turning on the ground is rotation about z axis
    if (deg > 0) { //turn right
        oi_setWheels(-speed, speed); //in mm/s (right, left)
    }
    else if (deg < 0) { // turn left
        oi_setWheels(speed, -speed); //in mm/s (right, left)
    }
    do {
        bno055_convert_float_euler_hpr_deg(&current_angles);
        deg_traveled = get_correct_difference_orientation(starting_angles.h, current_angles.h);
        lcd_printf("Deg Trav:\n%.2f\nCurr: %.2f\nStart: %.2f", deg_traveled, current_angles.h, starting_angles.h);
    } while (deg_traveled < fabsf(deg));
    oi_setWheels(0, 0); //in mm/s
}


void avoid_left_object(oi_t *self) {
    move_distance(self, 150, -50, 1);
    turn_deg(90, turn_speed);
    move_distance(self, 250, 50, 1);
    turn_deg(-90, turn_speed);
    move_distance(self, 150, 50, 1);
}

void avoid_right_object(oi_t *self) {
    move_distance(self, 150, -50, 1);
    turn_deg(-90, turn_speed);
    move_distance(self, 250, 50, 1);
    turn_deg(90, turn_speed);
    move_distance(self, 150, 50, 1);
}

void correct_left(oi_t *self) {
    turn_deg(90, turn_speed);
    move_distance(self, 190, 50, 1);
}

void correct_right(oi_t *self) {
    turn_deg(-90, turn_speed);
    move_distance(self, 190, 50, 1);
}

void move_distance(oi_t *self, double dist, int16_t speed, int bump) { // (sensor struct, mm, mm/s)
    struct bno055_euler_float_t starting_angles;
    struct bno055_euler_float_t current_angles;
    boundary_or_cliff_location* bocl = boundary_alloc();
    bno055_convert_float_euler_hpr_deg(&starting_angles);
    float right_wheel_correction_factor = 1;


    double distance_left = 0;
    int left_correction = 0, right_correction = 0;
    oi_update(self);
    while(distance_left <= dist) {
        oi_setWheels(speed * right_wheel_correction_factor, speed); // (right, left) in mm/s

        //BEGIN BUMP DETECTION
        if((self->bumpLeft) && (bump == 0)) {
            lcd_init();
            lcd_printf("LEFT BUMP");
            oi_setWheels(0, 0);
            timer_waitMillis(100);
            avoid_left_object(self);
            left_correction = 1;
            oi_update(self);
        }
        else if((self->bumpRight) && (bump == 0)){
            lcd_init();
            lcd_printf("RIGHT BUMP");
            oi_setWheels(0, 0);
            timer_waitMillis(100);
            avoid_right_object(self);
            right_correction = 1;
            oi_update(self);
        }
        //END BUMP DETECTION

        //BEGIN CLIFF DETECTION
        if( isCliff(self, bocl) ) {
            oi_setWheels(0, 0);
            lcd_init();
            lcd_printf("AT CLIFF");
            if(bocl.front_left || bocl.front_right){
                move_distance(self, -140, speed, bump);
                return;
            }
            else{
                // handle side boundary
            }
            oi_update(self);
        }
        //END CLIFF DETECTION

         //BEGIN BOUNDARY DETECTION
        if( isBoundary(self, bocl) ){
            oi_setWheels(0, 0);
            lcd_init();
            lcd_printf("AT BOUNDARY");
            if(bocl.front_left || bocl.front_right){
                move_distance(self, -140, speed, bump);
                return;
            }
            else{
                // handle side boundary
            }
            oi_update(self);
        }
        //END BOUNDARY DETECTION

        bno055_convert_float_euler_hpr_deg(&current_angles);
        right_wheel_correction_factor = get_correction_factor(starting_angles.h, current_angles.h);
        oi_update(self);
        distance_left += fabs(self->distance);
        lcd_printf("Travel Dist: %.3f\nDist: %.8f\nenc: %d", dist -distance_left, self->distance, self->leftEncoderCount);
    }
    if (left_correction) {
        correct_left(self);
    }
    else if (right_correction) {
        correct_right(self);
    }
    oi_setWheels(0, 0); //in mm/s
}
