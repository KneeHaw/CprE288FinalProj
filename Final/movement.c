/*
 * movement.h
 *
 *  Created on: Sep 5, 2023
 *      Author: kneehaw
 */

#include <stdbool.h>
#include "open_interface.h"
#include "movement.h"
#include "Timer.h"
#include "init_imu.h"
#include "imu.h"
#include "bno055.h"
#include "math.h"

#define turn_speed 25
#define bot_radius 18

objs_t saved_object;
int turn_direction = 0;
bool realign_gyro = false;

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

int get_correction_factor(float start_ang, float curr_ang) {
    curr_ang = get_correct_curr(start_ang, curr_ang);
//    return curr_ang / start_ang;

    if ((curr_ang / start_ang) > 1) {
        return 1;
    }
    else if ((curr_ang / start_ang) < 1) {
            return -1;
    }
    return 0;
}

float get_x(int theta, float dist) {
    return dist * cos((double)theta * M_PI / 180.);
}

bool within_range_float(float value, float compare_to, float lower_frac, float upper_frac) {
    if ((value > (compare_to * lower_frac)) && (value < (compare_to * upper_frac))) {
        return true;
    }
    return false;
}

bool within_range_int(int value, int compare_to, float lower_frac, float upper_frac) {
    if (((float)value > ((float)compare_to * lower_frac)) && ((float)value < ((float)compare_to * upper_frac))) {
        return true;
    }
    return false;
}

void handle_close_objs(objs_t *objects, oi_t* self, bool stop_sign) {
    lcd_printf("Handling Objs...");

    int i, j;
    int obj_ang1, obj_ang2;
    float obj_x1, obj_x2;
    objs_t new_scan_temp;
    for (i = 0; i < objects->num_objs; i++) {
        obj_ang1 = objects->start_ang[i] + (objects->tot_ang[i] / 2);
        obj_x1 = get_x(obj_ang1, objects->distance[i]);
        if (obj_x1 > bot_radius) {
            lcd_printf("No Threats");
        }
        else {
            if (stop_sign) {
                timer_waitMillis(10000); //Waiting for pedestrian to pass
            }
            if (obj_x1 >= 0 &&  objects->distance[i] < 25) { //need to turn left and move a bit then turn back
                lcd_printf("Avoiding...\nTurn1");
                turn_deg(self, -90, turn_speed);
                lcd_printf("Avoiding...\nForward");
                move_distance(self, fabsf((fabsf(obj_x1) - 18.) * 10.), 15, 0);
                lcd_printf("Avoiding...\nTurn2");
                turn_deg(self, 90, turn_speed);
            }
            else if (obj_x1 <= 0 &&  objects->distance[i] < 25) {
                lcd_printf("Avoiding...\nTurn1");
                turn_deg(self, 90, turn_speed);
                lcd_printf("Avoiding...\nForward");
                move_distance(self, fabsf((fabsf(obj_x1) - 18.) * 10.), 15, 0);
                lcd_printf("Avoiding...\nTurn2");
                turn_deg(self, -90, turn_speed);
            }
            realign_gyro = true;
        }
    }
}

void turn_deg(oi_t* self, float deg, int16_t speed) {// (deg, mm/s)
    deg = deg * (88./90.);
    get_deg_sign(deg);
    float deg_traveled = 0;
    oi_update(self);
    //rph = xyz, turning on the ground is rotation about z axis
    if (deg > 0) { //turn right
        oi_setWheels(-speed, speed); //in mm/s (right, left)
    }
    else if (deg < 0) { // turn left
        oi_setWheels(speed, -speed); //in mm/s (right, left)
    }


//    struct bno055_euler_float_t starting_angles;
//    struct bno055_euler_float_t current_angles;
//    bno055_convert_float_euler_hpr_deg(&starting_angles);
    do {
        deg_traveled += fabsf((float)self->angle);
        oi_update(self);

//        bno055_convert_float_euler_hpr_deg(&current_angles);
//        deg_traveled = get_correct_difference_orientation(starting_angles.h, current_angles.h);
        lcd_printf("Deg Trav:\n%.2f\nLEnc: %d\nREnc: %d", deg_traveled, self->leftEncoderCount, self->rightEncoderCount);
//        lcd_printf("Deg Trav:\n%.2f\nCurr: %.2f\nStart: %.2f", deg_traveled, current_angles.h, starting_angles.h);
    } while (deg_traveled < fabsf(deg));
    oi_setWheels(0, 0); //in mm/s
}

void gyro_turn_deg(float deg, int16_t speed) {
    deg = deg * (87/90.);
    get_deg_sign(deg);
    float deg_traveled = 0;
    //rph = xyz, turning on the ground is rotation about z axis
    if (deg > 0) { //turn right
        oi_setWheels(-speed, speed); //in mm/s (right, left)
    }
    else if (deg < 0) { // turn left
        oi_setWheels(speed, -speed); //in mm/s (right, left)
    }


    struct bno055_euler_float_t starting_angles;
    struct bno055_euler_float_t current_angles;
    bno055_convert_float_euler_hpr_deg(&starting_angles);
    do {
        bno055_convert_float_euler_hpr_deg(&current_angles);
        deg_traveled = get_correct_difference_orientation(starting_angles.h, current_angles.h);
        lcd_printf("Deg Trav:\n%.2f\nCurr: %.2f\nStart: %.2f", deg_traveled, current_angles.h, starting_angles.h);
    } while (deg_traveled < fabsf(deg));
    oi_setWheels(0, 0); //in mm/s
}

void avoid_left_object(oi_t *self) {
    move_distance(self, 150, -50, 1);
    turn_deg(self, 90, turn_speed);
    move_distance(self, 250, 50, 1);
    turn_deg(self, -90, turn_speed);
    move_distance(self, 150, 50, 1);
}

void avoid_right_object(oi_t *self) {
    move_distance(self, 150, -50, 1);
    turn_deg(self, -90, turn_speed);
    move_distance(self, 250, 50, 1);
    turn_deg(self, 90, turn_speed);
    move_distance(self, 150, 50, 1);
}

void correct_left(oi_t *self) {
    turn_deg(self, 90, turn_speed);
    move_distance(self, 190, 50, 1);
}

void correct_right(oi_t *self) {
    turn_deg(self, -90, turn_speed);
    move_distance(self, 190, 50, 1);
}

void move_distance(oi_t *self, double dist, int16_t speed, int bump) { // (sensor struct, mm, mm/s)
    struct bno055_euler_float_t starting_angles;
    struct bno055_euler_float_t current_angles;
    bno055_convert_float_euler_hpr_deg(&starting_angles);
    int right_wheel_correction_factor = 0;

    double distance_left = 0;
    int left_correction = 0, right_correction = 0;
    oi_update(self);
    while(distance_left <= dist) {
        oi_setWheels(0, 0);
        saved_object = final_ir_only_scan(&saved_object, false, false, 180, 0, 2);
        handle_close_objs(&saved_object, self, false);
        oi_setWheels((int)(speed + right_wheel_correction_factor), speed); // (right, left) in mm/s
        if (realign_gyro) {
            bno055_convert_float_euler_hpr_deg(&starting_angles);
            realign_gyro = false;
        }

//        begin close obj detection

        //Need to have obj_too_close detection inside of the scan?
        //Should bumps throw interrupts, along with boundaries?
        // BEGIN BUMP DETECTION
        if((self->bumpLeft) && (bump == 0)) {
            lcd_printf("LEFT BUMP");
            oi_setWheels(0, 0);
            timer_waitMillis(100);
            avoid_left_object(self);
            left_correction = 1;
            oi_update(self);
        }
        else if((self->bumpRight) && (bump == 0)){
            lcd_printf("RIGHT BUMP");
            oi_setWheels(0, 0);
            timer_waitMillis(100);
            avoid_right_object(self);
            right_correction = 1;
            oi_update(self);
        }
        // END BUMP DETECTION

        bno055_convert_float_euler_hpr_deg(&current_angles);
        right_wheel_correction_factor = get_correction_factor(starting_angles.h, current_angles.h);
        timer_waitMillis(1000);
        oi_update(self);
        distance_left += fabs(self->distance);
//        lcd_printf("Travel Dist: %.3f\nDist: %.8f\nenc: %d", dist - distance_left, self->distance, self->leftEncoderCount);
//        oi_update(self);
    }
    if (left_correction) {
        correct_left(self);
    }
    else if (right_correction) {
        correct_right(self);
    }
    oi_setWheels(0, 0); //in mm/s
}
