/*
 * boundary.c
 *
 *  Created on: Dec 1, 2023
 *      Author: Jesutofunmi Obimakinde
 *
 *      EDITED BY: Ethan Rogers - Dec 2, 2023
 *          1. Fixed ranges of detection and merged cliff with boundary detection
 */

#include "boundary.h"

//BEGIN UPDATE BOUNDARY STRUCT
bool isBoundary(oi_t* sensor, boundary_or_cliff_location* loc)
{
    loc->left = (sensor->cliffLeftSignal > 2500 || sensor->cliffLeftSignal < 800) ? 1 : 0;
    loc->right = (sensor->cliffRightSignal > 2500 || sensor->cliffRightSignal < 800) ? 1 : 0;
    loc->front_left = (sensor->cliffFrontLeftSignal > 2500 || sensor->cliffFrontLeftSignal < 800) ? 1 : 0;
    loc->front_right = (sensor->cliffFrontRightSignal > 2500 || sensor->cliffFrontRightSignal < 800) ? 1 : 0;

    return loc->left || loc->front_left || loc->front_right || loc->right;
}
//END UPDATE BOUNDARY STRUCT
