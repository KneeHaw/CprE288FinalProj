/*
 * movement.h
 *
 *  Created on: Sep 3, 2023
 *      Author: joo
 */
#ifndef BOUNDARY_H_
#define BOUNDARY_H_


#include "open_interface.h"
#include "stdbool.h"


typedef struct {
    uint32_t left;
    uint32_t right;
    uint32_t front_left;
    uint32_t front_right;
} boundary_or_cliff_location;


/**
 * function to create loc structur for use in boundary functions
 */
boundary_or_cliff_location* boundary_alloc();


/**
 * function to check if cybot is at a cliff
 * @param oi_t* sensor struct
 */
bool isCliff(oi_t* sensor, boundary_or_cliff_location* loc);


/**
 * function to check if cybot is at a boundary
 * @param oi_t* sensor struct
 */
bool isBoundary(oi_t* sensor, boundary_or_cliff_location* loc);


/**
 * Free memory from pointer to Open Interface Struct
 */
void boundary_free(boundary_or_cliff_location* self);


#endif /* BOUNDARY_H_ */

