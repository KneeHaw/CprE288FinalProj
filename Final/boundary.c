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
    loc->front_left = sensor->cliffFrontLeftSignal;
    loc->front_right = sensor->cliffFrontRightSignal;
    loc->left = sensor->cliffLeftSignal;
    loc->right = sensor->cliffRightSignal;

    return ( (sensor->cliffLeftSignal > 2250) || (sensor->cliffRightSignal > 2250) || (sensor->cliffFrontLeft > 2250) || (sensor->cliffFrontRight > 2250) );
}


void boundary_free(boundary_or_cliff_location* self)
{
    free(self);
}

