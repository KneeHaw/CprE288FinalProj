/*
 * custom_math.c
 *
 *  Created on: Oct 18, 2023
 *      Author: Ethan Rogers (kneehaw@iastate.edu)
 */
#include <math.h>
#include <stdbool.h>


float get_difference(float start_ang, float curr_ang) {
    if ((start_ang > 350) && (curr_ang < 10)) {
        return (360 - start_ang + curr_ang);
    }
    else if ((start_ang < 10) && (curr_ang > 350)) {
        return (360 + start_ang - curr_ang);
    }
    else {
        return (curr_ang - start_ang);
    }
}

int get_correction_factor(float start_ang, float curr_ang) {
    float difference;
    difference = get_difference(start_ang, curr_ang);
    if (difference < -0.1) { //if turned left, decrease right wheel speed to turn right
        return -1;
    }
    else if (difference > 0.1) { // if turned right, increase right wheel speed to turn left
        return 1;
    }
    else {
        return 0;
    }
}

int turn_direction = 0; //used to store the turn direction within the file since it's called regularly

void get_deg_sign(float deg) {
    if (deg < 0) {
        turn_direction = -1;
    }
    else {
        turn_direction = 1;
    }
}

float get_correct_difference_orientation(float start_ang, float curr_ang) {
    if ((start_ang > curr_ang) && (turn_direction > 0)) { // right turn detect wrap from 360 to 0 (positive angle!)
        return fabsf(360 - start_ang + curr_ang);
    }
    else if ((start_ang < curr_ang) && (turn_direction < 0)) {
        return fabsf(360 - curr_ang + start_ang); //left turn detect wrap from 0 to 360 (negative angle!)
    }
    else{
        return  fabsf(start_ang - curr_ang);
    }
}

float get_x_from_deg_hypotenuse(int theta, float dist) {
    return dist * cos((double)theta * M_PI / 180.);
}

float get_y_from_deg_hypotenuse(int theta, float dist) {
    return dist * sin((double)theta * M_PI / 180.);
}

float get_theta_from_xy(float x, float y) {
    return atanf((20 - fabsf(x))/(y + 15)) * 180 / M_PI;
}




/*
 * Begin SCAN math
 */

float IR_interpret(int IR_raw) {
    float new_IR = 0;
    new_IR = pow(M_E, (24.2542 / pow((float)IR_raw, 0.27457))) - 7.877; //Values are from scipy curve fit
    return new_IR;
}

float get_botDist(int scanAng, float scanDist) {
    float x = scanDist * cos(scanAng * M_PI / 180);
    float y = scanDist * sin(scanAng * M_PI / 180) + 14;
    return pow((pow(x, 2) + pow(y, 2)), .5);
}

int get_botAng(int scanAng, float scanDist) {
    float x = scanDist * cos(scanAng * M_PI / 180);
    float y = scanDist * sin(scanAng * M_PI / 180) + 11;
    int output = (int)(atan2f(y, x) * 180 / M_PI);
    return output;
}


/*
 * Begin General Math
 */

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

void sort_float_array(float* array, int array_length) {
    int i, j;
    float temp;
    for (i = 0; i < array_length; i++) {
        for (j = 0; j < (array_length - 1); j++)
            if (array[j] > array[j+1]) {
                temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
            }
    }
    //The array is now sorted smallest to largest.
}

float get_median_of_float_array(float array[], int array_length) {
    if (array_length % 2 == 0) {
        return ((array[(array_length - 1) / 2] + array[(array_length + 1) / 2]) / 2.);
    }
    else {
        return array[array_length / 2];
    }
}


