/*
 * instruction_set.h
 *
 *  Created on: Dec 3, 2023
 *      Author: kneehaw
 */
#include "movement.h"
#include "music.h"

#ifndef INSTRUCTION_SET_H_
#define INSTRUCTION_SET_H_

void get_instructions(oi_t* self, char house);
void first_half_initiate_delivery(oi_t* self);
void second_half_arrive_at_destination(oi_t* self, char house, int get_to_destination);
void return_to_street_exit(oi_t* self, char house, int get_back_to_road);
void return_to_charging_station(oi_t *self);


#endif /* INSTRUCTION_SET_H_ */
