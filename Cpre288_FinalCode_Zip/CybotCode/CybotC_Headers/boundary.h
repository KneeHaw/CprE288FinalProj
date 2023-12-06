/*
 * boundary.h
 *
 *  Created on: Dec 1, 2023
 *      Author: Jesutofunmi Obimakinde
 *
 *      EDITED BY: Ethan Rogers - Dec 4, 2023
 *          1. Deleted separate cliff and boundary detection (merged)
 *          2. Deleted unnecessary malloc and free
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
 * function to check if cybot is at a boundary
 * @param oi_t* sensor struct
 */
bool isBoundary(oi_t* sensor, boundary_or_cliff_location* loc);

#endif /* BOUNDARY_H_ */

