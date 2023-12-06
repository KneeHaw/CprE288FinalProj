/*
 * scan.h
 *
 * Author: Ethan Rogers (kneehaw@iastate.edu)
 * Date: 10/3/2023
 * Description: Scan Library for Cpre288 CyBot
 *
 * --Heavily edited Dec 1, 2023 through Dec 5 2023
 */

#ifndef SCAN_H_
#define SCAN_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "uart.h"
#include "lcd.h"
#include "Timer.h"
#include "open_interface.h"
#include "movement.h"
#include "ping.h"
#include "ir_adc.h"
#include "servo.h"
#include <stdbool.h>

typedef struct {
    float distance[5];
    int midd_ang[5];
    int tot_ang[5];
    float width[5];
    int num_objs;

} objs_t; //Stores up to 5 objects (could be increased, but CprE 288 use-case rarely needs more than 5)

typedef struct {
    float ping_dist;
    float IR_dist;

}distance_sensor_data;

void ir_calibrate(int i);
void get_sensor_data(int degrees, distance_sensor_data* data, char ping, char ir, int wait_time);
objs_t routine_scan(int end_deg, int start_deg, int deg_inc);


#endif /* SCAN_H_ */
