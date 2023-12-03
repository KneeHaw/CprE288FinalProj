/*
 * scan.h
 *
 *  Created on: Oct 3, 2023
 *      Author: kneehaw
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
    float distance[15];
    int start_ang[15];
    int tot_ang[15];
    float width[15];
    int num_objs;
    int smlst_width_ang;
    float smlst_width_dist;
    bool civilian_leftwards;
    bool civilian_rightwards;
    int num_scans;
} objs_t; //Stores up to 15 objects


typedef struct {
    float ping_dist;
    float IR_dist;

}cyData;


float IR_interpret(float IR_raw);

void ir_calibrate();

objs_t perform_detailed_scan(oi_t* sensor_data, int deg_inc, int tot_deg);

void cyScan(int degrees, cyData* data, char ping, char ir, int wait_time);

void ir_calibrate();


#endif /* SCAN_H_ */
