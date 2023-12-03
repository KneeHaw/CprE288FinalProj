/*
 * instruction_set.c
 *
 *  Created on: Dec 2, 2023
 *      Author: kneehaw
 */

#include "movement.h"



void get_instructions(char house) {
   if (house == 'A') {
        first_half_initiate_delivery();
        second_half_arrive_at_destination('A', 1);
        return_to_street_exit('A', 3);
        return_to_charging_station();
   } 
   else if (house == 'B'){
        first_half_initiate_delivery();
        second_half_arrive_at_destination('B', 2);
        return_to_street_exit('B', 2);
        return_to_charging_station();
   }
   else if (house == 'C'){
        first_half_initiate_delivery();
        second_half_arrive_at_destination('C', 3);
        return_to_street_exit('C', 1);
        return_to_charging_station();
   }
   else if (house == 'D'){
        first_half_initiate_delivery();
        second_half_arrive_at_destination('D', 3);
        return_to_street_exit('D', 1);
        return_to_charging_station();
   }
   else if(house == 'E'){
        first_half_initiate_delivery();
        second_half_arrive_at_destination('E', 2);
        return_to_street_exit('E', 2);
        return_to_charging_station();
   }
   else if(house == 'F'){
        first_half_initiate_delivery();
        second_half_arrive_at_destination('F', 1);
        return_to_street_exit('F', 3);
        return_to_charging_station();
   }
}
// DEFAULT MOVE SEQUENCE TO THE STOP SIGN
void first_half_initiate_delivery(){
    // MOVE 5 TILES
    move_distance(oi_t *self, double dist, int16_t speed, int bump); // should be 5 x 61cm , speed = 15, bump = 0
    turn_deg(float deg, int16_t speed); // Should be deg = -90, speed = 25
    // ENCOUTERS STOP SIGN
    timer_waitMicros(3000); // waits 3 seconds
    scan_function();
    // MOVE TO STREET ENTRANCE
    move_distance(oi_t *self, double dist, int16_t speed, int bump); // should be 1 x 61cm, speed = 15, bump = 0
    move_distance(oi_t *self, double dist, int16_t speed, int bump); // should be 1 x 45.7cm , speed = 15, bump = 0
    turn_deg(float deg, int16_t speed); // Should be deg = -90, speed = 25
}

// ACTION TO DESTINATION
void second_half_arrive_at_destination(char house, int get_to_destination){
    if (house == 'A' || house == 'B' || house == 'C'){
        // MOVE DEPENDING ON HOUSE A, B, C
        move_distance(oi_t *self, double dist, int16_t speed, int bump); // should be get_to_destination x 61cm , speed = 15, bump = 0
        // PLAY SONG TO INDICATE ITEM DELIVERED
        play_songs(oi_t *self); // should be sensor_data or smth
    }
    else {
        // MOVE DEPENDING ON HOUSE D, E, F
        move_distance(oi_t *self, double dist, int16_t speed, int bump); // should be 4 x 61cm , speed = 15, bump = 0
        turn_deg(float deg, int16_t speed); // Should be deg = -90, speed = 25
        move_distance(oi_t *self, double dist, int16_t speed, int bump); // should be 1 x 61cm , speed = 15, bump = 0
        turn_deg(float deg, int16_t speed); // Should be deg = -90, speed = 25
        move_distance(oi_t *self, double dist, int16_t speed, int bump); // should be get_to_destination x 61cm , speed = 15, bump = 0
        play_songs(oi_t *self); // should be sensor_data or smth
    }
}

// MOVE BACK TO STREET EXIT
void return_to_street_exit(char house, int get_back_to_road){
    if (house == 'A' || house == 'B' || house == 'C'){
        // MOVE DEPENDING ON HOUSE A, B, C
        move_distance(oi_t *self, double dist, int16_t speed, int bump); // should be get_back_to_road x 61cm , speed = 15, bump = 0
        turn_deg(float deg, int16_t speed); // Should be deg = -90, speed = 25
        move_distance(oi_t *self, double dist, int16_t speed, int bump); // should be 1 x 61cm , speed = 15, bump = 0
        turn_deg(float deg, int16_t speed); // Should be deg = -90, speed = 25
        move_distance(oi_t *self, double dist, int16_t speed, int bump); // should be 4 x 61cm , speed = 15, bump = 0
        turn_deg(float deg, int16_t speed); // Should be deg = -90, speed = 25
    }
    else {
        // MOVE DEPENDING ON HOUSE D, E, F
        move_distance(oi_t *self, double dist, int16_t speed, int bump); // should be get_back_to_road x 61cm , speed = 15, bump = 0
        
        turn_deg(float deg, int16_t speed); // Should be deg = -90, speed = 25
    }
}

// EXIT SEQUENCE
void return_to_charging_station(){
    move_distance(oi_t *self, double dist, int16_t speed, int bump); // should be 2 x 61cm , speed = 15, bump = 0
    turn_deg(float deg, int16_t speed); // Should be deg = -90, speed = 25
    move_distance(oi_t *self, double dist, int16_t speed, int bump); // should be 6 x 61cm , speed = 15, bump = 0
    turn_deg(float deg, int16_t speed); // Should be deg = -90, speed = 25
    move_distance(oi_t *self, double dist, int16_t speed, int bump); // should be 1 x 61cm , speed = 15, bump = 0
    // ENCOUTERS STOP SIGN
    timer_waitMicros(3000); // waits 3 seconds
    scan_function();
    move_distance(oi_t *self, double dist, int16_t speed, int bump); // should be 2 x 61cm , speed = 15, bump = 0
    turn_deg(float deg, int16_t speed); // Should be deg = -90, speed = 25
    move_distance(oi_t *self, double dist, int16_t speed, int bump); // should be 1 x 61cm , speed = 15, bump = 0
    play_songs(oi_t *self); // should be sensor_data or smth
}
