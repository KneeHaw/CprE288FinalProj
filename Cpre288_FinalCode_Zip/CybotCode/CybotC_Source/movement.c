/*
 * movement.c
 *
 *  Created on: Sep 5, 2023
 *      Author: Ethan Rogers (kneehaw@iastate.edu)
 *      Editor(s): Adeife Fadahunsi
 */

#include <stdbool.h>
#include "open_interface.h"
#include "movement.h"
#include "Timer.h"
#include "init_imu.h"
#include "imu.h"
#include "bno055.h"
#include "math.h"
#include "boundary.h"
#include "custom_math.h"

#define turn_speed 50
#define bot_radius 20.

objs_t saved_objects;
bool realign_gyro = false;

/*
 * When looking at code, make note of its recursive nature for navigating more complex obstacles!
 */


//BEGIN HANDLING TALL OBJECTS PROTOCOL
bool handle_close_objs(objs_t *objects, oi_t* self, bool stop_sign, float *distance) {
    int i, j;
    int obj_ang;
    float obj_x;
    float obj_y;
    float deg_to_turn;
    objs_t new_scan_temp;
    for (i = 0; i < objects->num_objs; i++) {
        obj_ang = objects->midd_ang[i];
        obj_x = get_x_from_deg_hypotenuse(obj_ang, objects->distance[i]); //x relative to bot center
        obj_y = get_y_from_deg_hypotenuse(obj_ang, objects->distance[i]); //y relative to IR center
        deg_to_turn = fabsf(get_theta_from_xy(obj_x, obj_y));
        lcd_printf("Handling Objs...\n%d", obj_x);

        if ((fabsf(obj_x) > bot_radius) && (obj_y > 20.)) {
            lcd_printf("No Threat");
        }
        else if ((fabsf(obj_x) < bot_radius) && (obj_y < 20.)){
            if (stop_sign) {
                for (j = 0; j < 10; j++) {
                    timer_waitMillis(1000); //Waiting for pedestrian to pass
                }
                new_scan_temp = routine_scan(180, 0, 2);
                handle_close_objs(&new_scan_temp, self, false, distance); //Check again now that pedestrians have moved
            }
            else {
                if (obj_x >= 0) {
                    turn_deg(self, -deg_to_turn, turn_speed);
                    move_distance(self, (powf(powf(bot_radius - fabsf(obj_x) + 1, 2) + powf(obj_y + 15, 2), .5))* 10, 50, 0, 0, 0);
                    turn_deg(self, deg_to_turn, turn_speed);
                }
                else if (obj_x <= 0) {
                    turn_deg(self, deg_to_turn, turn_speed);
                    move_distance(self, (powf(powf(bot_radius - fabsf(obj_x) + 1, 2) + powf(obj_y + 15, 2), .5)) * 10, 50, 0, 0, 0);
                    turn_deg(self, -deg_to_turn, turn_speed);
                }
                *distance += (obj_y + 15) * 10; //Ensure traveled forward distance is accounted for.
                                                // Could lead to overshooting!
            }
            realign_gyro = true;
            return true;
        }
    }
    return false;
}
//END HANDLING TALL OBJECTS PROTOCOL

// BEGIN SHORT OBJECT / BUMP AVOIDANCE PROTOCOLS
void avoid_left_object(oi_t *self) {
    move_distance(self, 60, -25, 1, 0, 0);
    turn_deg(self, 50, turn_speed);
    move_distance(self, 120, 100, 0, 0, 0);
    turn_deg(self, -50, turn_speed);
}

void avoid_right_object(oi_t *self) {
    move_distance(self, 60, -25, 1, 0, 0);
    turn_deg(self, -50, turn_speed);
    move_distance(self, 120, 100, 0, 0, 0);
    turn_deg(self, 50, turn_speed);
}

bool handle_bumpers(oi_t* self, int bump, float *distance) {
    if(self->bumpLeft && (bump == 0)) {
        lcd_printf("LEFT BUMP");
        avoid_left_object(self);
        distance += 32;
        oi_update(self);
        realign_gyro = true;
        return true;
    }
    else if((self->bumpRight && (bump == 0))){
        lcd_printf("RIGHT BUMP");
        avoid_right_object(self);
        distance += 32; //Ensure traveled forward distance is accounted for.
                        // Could lead to overshooting!
        oi_update(self);
        realign_gyro = true;
        return true;
    }
    return false;
}
// END SHORT OBJECT / BUMP AVOIDANCE PROTOCOLS

//BEGIN MOVEMENT FUNCTIONS
void turn_deg(oi_t* self, float deg, int16_t speed) {// (deg, mm/s)
    // BEGIN OPEN INTERFACE VERSION
//    deg = deg * 0.9878791; //deg * .98 bot 2, deg * 0.9878791 bot 0
//    get_deg_sign(deg);
//    float deg_traveled = 0;
//    oi_update(self);
//    if (deg > 0) { //turn right
//        oi_setWheels(-speed, speed); //in mm/s (right, left)
//    }
//    else if (deg < 0) { // turn left
//        oi_setWheels(speed, -speed); //in mm/s (right, left)
//    }
//
//    do {
//        oi_update(self);
//        deg_traveled += fabsf((float)self->angle);
//        lcd_printf("Deg Trav:\n%.2f\nLEnc: %d\nREnc: %d", deg_traveled, self->leftEncoderCount, self->rightEncoderCount);
//    } while (deg_traveled < fabsf(deg));
//    oi_setWheels(0, 0); //in mm/s
    //END OPEN INTERFACE VERSION

    //BEGIN GYRO VERSION
    deg = deg * .986; //Calculated via multiple 360deg turns and taking average error. This is per cybot (cy0 in demo)
    get_deg_sign(deg);
    float deg_traveled = 0;
    float incrementer;
    //rph = xyz, turning on the ground is rotation about z axis
    if (deg > 0) { //turn right
        oi_setWheels(-speed, speed); //in mm/s (right, left)
    }
    else if (deg < 0) { // turn left
        oi_setWheels(speed, -speed); //in mm/s (right, left)
    }
    struct bno055_euler_float_t last_angle;
    struct bno055_euler_float_t current_angles;
    bno055_convert_float_euler_hpr_deg(&last_angle);
    do {
        lcd_printf("Deg Trav: %.2f\nCurr: %.2f\nStart: %.2f\ninc: %.2f", deg_traveled, current_angles.h, last_angle.h, incrementer);
        bno055_convert_float_euler_hpr_deg(&current_angles);
        incrementer = get_correct_difference_orientation(last_angle.h, current_angles.h);
        //checking for gyroscope error via a jump in values (normal movement only sees ~0.6 deg of turn at a time
        if (incrementer > 2) {
            bno055_convert_float_euler_hpr_deg(&last_angle);
            continue;
        }
        else {
            deg_traveled += incrementer;
            last_angle.h = current_angles.h;
        }
    } while (deg_traveled < fabsf(deg));
    oi_setWheels(0, 0); //in mm/s
    //END GYRO VERSION
}

void move_distance(oi_t *self, float dist, int16_t speed, int bump, int check_objs, bool handling_bocl) { // (sensor struct, mm, mm/s)
    struct bno055_euler_float_t starting_angles;
    struct bno055_euler_float_t current_angles;
    bno055_convert_float_euler_hpr_deg(&starting_angles);
    int right_wheel_correction_factor = 0;
    int initial_time = 0;
    bool just_bumped = false;
    bool just_handled = false;
    boundary_or_cliff_location bocl;

    float distance_traveled = 0;
    oi_update(self); //Ensure traveled distances will be relative to starting "0"
    while(distance_traveled <= dist) {
        //If the bot has avoided a short or tall object, reset the definition of straight
        if (realign_gyro) {
            bno055_convert_float_euler_hpr_deg(&starting_angles);
            realign_gyro = false;
        }

        //Before moving forward about 10-15 cm, check if there is an object within (bot_radius)cm x/y
        if (check_objs) {
            saved_objects = routine_scan(180, 0, 2);
            just_handled = handle_close_objs(&saved_objects, self, false, &distance_traveled);
            if (just_handled) {
                just_handled = false;
                continue; //Ensure a new scan before moving forward after detecting an object
            }
        }

        /*
         * Start a timer for moving forward, increment distance traveled throughout, checking for
         * boundary & bumps as you move. Ensures instant handling of immediate obstacles (tall objects are not in need
         * of immediate handling)
         */
        initial_time = timer_getMillis();
        do {
            //BEGIN BOUNDARY DETECTION
            isBoundary(self, &bocl);
            if (bocl.left && !((bocl.front_left) && (bocl.front_right))) {
                oi_setWheels(speed - 3, speed + 3); //Turn slightly right
            }

            else if (bocl.right && !((bocl.front_left) && (bocl.front_right))) {
                oi_setWheels(speed + 3, speed - 3); //Turn slightly left
            }
            else if ((bocl.front_left) && (bocl.front_right) && (!handling_bocl)) {
                move_distance(self, 100, -25, 0, 0, 1); //Back 'er up
                distance_traveled = dist; //End current movement if run into boundary
                oi_update(self);
            }
            else {
                oi_setWheels(speed + right_wheel_correction_factor, speed);
            }
            // END BOUNDARY DETECTION

            // BEGIN BUMP DETECTION
            just_bumped = handle_bumpers(self, bump, &distance_traveled);
            if ((just_bumped) && (bump == 0)) {
                just_bumped = false;
                break; //Ensure new scan before moving again
            }
            // END BUMP DETECTION

            //Get the current angles of the bot during linear movement
            bno055_convert_float_euler_hpr_deg(&current_angles);
            //Increase right wheel speed according to a positive or negative difference to the 'straight angle'
            right_wheel_correction_factor = get_correction_factor(starting_angles.h, current_angles.h);
            oi_update(self);

            //Increment distance traveled
            distance_traveled += fabs((float)self->distance);

            //Give an update on the LCD of what's occuring
            lcd_printf("Dist: %.2fcm\n\nBattery: %.1f\n%x",
                       dist - distance_traveled, (100 * (float)self->batteryCharge / (float)self->batteryCapacity),
                       bocl.left, bocl.front_left, bocl.front_right, bocl.right);
        } while (((timer_getMillis() - initial_time) < 1000) && distance_traveled <= dist); //Only move for 1sec, ensure
                                                                                            //not crossing set dist
        oi_setWheels(0, 0);
    }
    oi_setWheels(0, 0); //in mm/s
}
//END MOVEMENT FUNCTIONS

//BEGIN GENERAL STOP SIGN CASE
float do_stop_sign(oi_t* self) {
    float distance = 0;
    objs_t stop_sign_scan = routine_scan(180, 0, 2);
    handle_close_objs(&stop_sign_scan, self, true, &distance);
    return distance;
}
//END GENERAL STOP SIGN CASE
