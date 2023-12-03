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

void cyScan(int degrees, cyData* data, char ping, char ir, int wait_time);

objs_t* perform_scan_staticObjs(oi_t* self, int deg_inc, int end_deg, int start_deg);


objs_t final_ir_only_scan(objs_t* prev_objs, bool reset_num_scans, bool check_moving, int end_deg, int start_deg, int deg_inc);


#endif /* SCAN_H_ */
