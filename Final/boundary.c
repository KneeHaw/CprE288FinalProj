#include "boundary.h"


boundary_or_cliff_location* boundary_alloc()
{
    return malloc(sizeof(boundary_or_cliff_location));
}


bool isCliff(oi_t* sensor, boundary_or_cliff_location* loc)
{
    oi_update(sensor);
    if(sensor->cliffLeft || sensor->cliffRight || sensor->cliffFrontLeft || sensor->cliffFrontRight) return true;

    loc->front_left = sensor->cliffFrontLeft;
    loc->front_right = sensor->cliffFrontRight;
    loc->left = sensor->cliffLeft;
    loc->right = sensor->cliffRight;
    return false;
}


bool isBoundary(oi_t* sensor, boundary_or_cliff_location* loc)
{
    oi_update(sensor);
    loc->front_left = (sensor->cliffLeftSignal > 2500) ? 1 : 0;
    loc->front_right = (sensor->cliffRightSignal > 2500) ? 1 : 0;
    loc->left = (sensor->cliffFrontLeftSignal > 2500) ? 1 : 0;
    loc->right = (sensor->cliffFrontRightSignal > 2500) ? 1 : 0;

    return ( (sensor->cliffLeftSignal > 2500) || (sensor->cliffRightSignal > 2500) || (sensor->cliffFrontLeftSignal > 2500) || (sensor->cliffFrontRightSignal > 2500) );
}


void boundary_free(boundary_or_cliff_location* self)
{
    free(self);
}

