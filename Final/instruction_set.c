/*
 * instruction_set.c
 *
 *  Created on: Dec 2, 2023
 *      Author: kneehaw
 */

/*1. Use both OI and Gyro to track turned degree; hwoever, use a validity check
 *      such that the gyro cannot move X degrees at one interval without being thrown
 *      out in favor of using OI for turn validity (need flag, logic check, store previous deg_traveled)
 * 2. Boundary and lane adherence (helps stay in the center path,
 *      especially between houses if we can get some tape
 * 3. Use ping for getting obj distance per perform_detailed_scan
 *      (integrate into final_ir_only_scan) should be easy, looka t existing code
 * 4. Stop sign - complete if stop_sign in checker function
 * 5. Create BUMP interrupt?
 *      Sometimes scanning happens at a bad time and bumps aren't detected mid movement
 *      Or implement another fix!
 */

#include "instruction_set.h"
#define forward_speed 100


void get_instructions(oi_t* self, char house) {
   if (house == 'A') {
        first_half_initiate_delivery(self);
        second_half_arrive_at_destination(self, 'A', 1);
        return_to_street_exit(self, 'A', 3);
        return_to_charging_station(self);
   }
   else if (house == 'B'){
        first_half_initiate_delivery(self);
        second_half_arrive_at_destination(self, 'B', 2);
        return_to_street_exit(self, 'B', 2);
        return_to_charging_station(self);
   }
   else if (house == 'C'){
        first_half_initiate_delivery(self);
        second_half_arrive_at_destination(self, 'C', 3);
        return_to_street_exit(self, 'C', 1);
        return_to_charging_station(self);
   }
   else if (house == 'D'){
        first_half_initiate_delivery(self);
        second_half_arrive_at_destination(self, 'D', 3);
        return_to_street_exit(self, 'D', 1);
        return_to_charging_station(self);
   }
   else if(house == 'E'){
        first_half_initiate_delivery(self);
        second_half_arrive_at_destination(self, 'E', 2);
        return_to_street_exit(self, 'E', 2);
        return_to_charging_station(self);
   }
   else if(house == 'F'){
        first_half_initiate_delivery(self);
        second_half_arrive_at_destination(self, 'F', 1);
        return_to_street_exit(self, 'F', 3);
        return_to_charging_station(self);
   }
}

// DEFAULT MOVE SEQUENCE TO THE STOP SIGN
void first_half_initiate_delivery(oi_t* self){
    // MOVE 5 TILES
    move_distance(self, 3050, forward_speed, 0); // should be 5 x 61cm , speed = forward_speed, bump = 0
    turn_deg(self, -90, 25); // Should be deg = -90, speed = 25
    // ENCOUTERS STOP SIGN
    oi_setWheels(0, 0);
    timer_waitMillis(3000); // waits 3 seconds
//    scan_function();
    // MOVE TO STREET ENTRANCE
    move_distance(self, 610, forward_speed, 0); // should be 1 x 61cm, speed = forward_speed, bump = 0
    move_distance(self, 457 + 50, forward_speed, 0); // should be 1 x 45.7cm , speed = forward_speed, bump = 0
    turn_deg(self, -90, 25); // Should be deg = -90, speed = 25
}

// ACTION TO DESTINATION
void second_half_arrive_at_destination(oi_t* self, char house, int get_to_destination){
    if (house == 'A' || house == 'B' || house == 'C'){
        // MOVE DEPENDING ON HOUSE A, B, C
        move_distance(self, get_to_destination * 610, forward_speed, 0); // should be get_to_destination x 61cm , speed = forward_speed, bump = 0
        // PLAY SONG TO INDICATE ITEM DELIVERED
        play_songs(self); // should be sensor_data or smth
    }
    else {
        // MOVE DEPENDING ON HOUSE D, E, F
        move_distance(self, 2440, forward_speed, 0); // should be 4 x 61cm , speed = forward_speed, bump = 0
        turn_deg(self, -90, 25); // Should be deg = -90, speed = 25
        move_distance(self, 457, forward_speed, 0); // should be 1 x 61cm , speed = forward_speed, bump = 0
        turn_deg(self, -90, 25); // Should be deg = -90, speed = 25
        move_distance(self, get_to_destination * 610, forward_speed, 0); // should be get_to_destination x 61cm , speed = forward_speed, bump = 0
        play_songs(self); // should be sensor_data or smth
    }
}

// MOVE BACK TO STREET EXIT
void return_to_street_exit(oi_t* self, char house, int get_back_to_road){
    if (house == 'A' || house == 'B' || house == 'C'){
        // MOVE DEPENDING ON HOUSE A, B, C
        move_distance(self, get_back_to_road * 610, forward_speed, 0); // should be get_back_to_road x 61cm , speed = forward_speed, bump = 0
        turn_deg(self, -90, 25); // Should be deg = -90, speed = 25
        move_distance(self, 457, forward_speed, 0); // should be 1 x 61cm , speed = forward_speed, bump = 0
        turn_deg(self, -90, 25); // Should be deg = -90, speed = 25
        move_distance(self, 2440, forward_speed, 0); // should be 4 x 61cm , speed = forward_speed, bump = 0
        turn_deg(self, -90, 25); // Should be deg = -90, speed = 25
    }
    else {
        // MOVE DEPENDING ON HOUSE D, E, F
        move_distance(self, get_back_to_road * 610, forward_speed, 0); // should be get_back_to_road x 61cm , speed = forward_speed, bump = 0

        turn_deg(self, -90, 25); // Should be deg = -90, speed = 25
    }
}

// EXIT SEQUENCE
void return_to_charging_station(oi_t *self){
    move_distance(self, 1220, forward_speed, 0); // should be 2 x 61cm , speed = forward_speed, bump = 0
    turn_deg(self, -90, 25); // Should be deg = -90, speed = 25
    move_distance(self, 3660, forward_speed, 0); // should be 6 x 61cm , speed = forward_speed, bump = 0
    turn_deg(self, -90, 25); // Should be deg = -90, speed = 25
    move_distance(self, 610, forward_speed, 0); // should be 1 x 61cm , speed = forward_speed, bump = 0
    // ENCOUTERS STOP SIGN
    oi_setWheels(0, 0);
    timer_waitMillis(3000); // waits 3 seconds
//    scan_function();
    move_distance(self, 1220, forward_speed, 0); // should be 2 x 61cm , speed = forward_speed, bump = 0
    turn_deg(self, -90, 25); // Should be deg = -90, speed = 25
    move_distance(self, 610, forward_speed, 0); // should be 1 x 61cm , speed = forward_speed, bump = 0
    play_songs(self); // should be sensor_data or smth
}
