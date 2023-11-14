/*
 * movement.h
 *
 *  Created on: Sep 5, 2023
 *      Author: kneehaw
 */

#include "open_interface.h"
#include "movement.h"
#include "Timer.h"

void turn_deg(oi_t *self, double deg, int16_t speed) {// (sensor struct, deg, mm/s)
    double deg_traveled = 0;
    oi_update(self);
//    deg = deg * .95; //Provide an error rate for turning (friction? why error?)
    if (deg > 0) { //turn left
        oi_setWheels(speed, -speed); //in mm/s (right, left)
        while(deg_traveled <= deg) {
            oi_update(self);
            deg_traveled += self->angle;
            lcd_printf("Travel Deg: %.3f\nang: %.8f\nenc: %d", deg_traveled, self->angle, self->leftEncoderCount);
        }
    }
    else if (deg < 0) { // turn right
        oi_setWheels(-speed, speed); //in mm/s
        while(deg_traveled >= deg) {
            oi_update(self);
            deg_traveled += self->angle;
            lcd_printf("Travel Deg: %.3f\nang: %.8f\nenc: %d", deg_traveled, self->angle, self->leftEncoderCount);
        }
    }
    oi_setWheels(0, 0); //in mm/s
}


void avoid_left_object(oi_t *self) {
    int bump = 1;
    move_distance(self, 150, -50, bump);
    turn_deg(self, -90, 100);
    move_distance(self, 250, 50, bump);
    turn_deg(self, 90, 100);
    move_distance(self, 150, 50, bump);
}

void avoid_right_object(oi_t *self) {
    int bump = 1;
    move_distance(self, 150, -50, bump);
    turn_deg(self, 90, 100);
    move_distance(self, 250, 50, bump);
    turn_deg(self, -90, 100);
    move_distance(self, 150, 50, bump);
}

void correct_left(oi_t *self) {
    int bump = 1;
    turn_deg(self, 90, 100);
    move_distance(self, 190, 50, bump);
}

void correct_right(oi_t *self) {
    int bump = 1;
    turn_deg(self, -90, 100);
    move_distance(self, 190, 50, bump);
}


void move_distance(oi_t *self, double dist, int16_t speed, int bump) { // (sensor struct, mm, mm/s)
    double distance_left = 0;
    int left_correction = 0, right_correction = 0;
    oi_update(self);
    while(distance_left <= dist) {
        oi_setWheels(speed, speed); //in mm/s

        //BEGIN BUMP DETECTION
        if((self->bumpLeft) && (bump == 0)) {
            lcd_init();
            lcd_printf("LEFT BUMP");
            oi_setWheels(0, 0);
            timer_waitMillis(500);
            avoid_left_object(self);
            left_correction = 1;
            oi_update(self);
        }
        else if((self->bumpRight) && (bump == 0)){
            lcd_init();
            lcd_printf("RIGHT BUMP");
            oi_setWheels(0, 0);
            timer_waitMillis(500);
            avoid_right_object(self);
            right_correction = 1;
            oi_update(self);
        }
        //END BUMP DETECTION

        oi_update(self);
        distance_left += fabs(self->distance);
        lcd_init();
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
