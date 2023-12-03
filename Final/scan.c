/*
* Author: Ethan Rogers (kneehaw@iastate.edu)
* Date: 10/3/2023
* Description: Scan Library for Cpre288 CyBot
*/

#include <math.h>
#include <stdbool.h>
#include "scan.h"

#define PI 3.141592653589793
#define E 2.718281828459045


volatile char adc_data_flag = 0;
volatile int adc_data_recv = 0;


float float_min(float compare_to_this1, float compare_to_this2) {
    if (compare_to_this1 > compare_to_this2) {
        return compare_to_this2;
    }
    return compare_to_this1;
}

float float_max(float compare_to_this1, float compare_to_this2) {
    if (compare_to_this1 < compare_to_this2) {
        return compare_to_this2;
    }
    return compare_to_this1;
}

//Convert IR_RAW to ping_sensor similar value using piecewise function
float IR_interpret(float IR_raw) {
    float new_IR = 0;
    new_IR = pow(E, (3.505 / pow(IR_raw, 0.129))) - 17.865; //Values are from scipy curve fit
    return new_IR;
}

float get_botDist(int scanAng, float scanDist) {
    float x = scanDist * cos(scanAng * PI / 180);
    float y = scanDist * sin(scanAng * PI / 180) + 14;

    return pow((pow(x, 2) + pow(y, 2)), .5);
}

int get_botAng(int scanAng, float scanDist) {
    float x = scanDist * cos(scanAng * PI / 180);
    float y = scanDist * sin(scanAng * PI / 180) + 11;
    int output = (int)(atan2f(y, x) * 180 / PI);

    return output;
}

void repeated_ir_scan(cyData *data, float *edge_dist, int num_samples, int degToTurn) {
    int i;
    for (i = 0; i < num_samples; i++) {
        cyScan(degToTurn, data, 0, 1, 6);  //Deg to turn, data struct, ping?, ir?
        *edge_dist += data->IR_dist / num_samples;
        timer_waitMillis(1);
    }
}
////Perform a scan of every N degrees, and then print out all data and code-detected objects
//objs_t* perform_scan_staticObjs(oi_t* self, int deg_inc, int end_deg, int start_deg) {
//
//    int scans = (end_deg - start_deg) / deg_inc;
//
//    char temp[50];
//    int i, k;  // Increments over degrees & iterations per degree
//    int num_samples = 3;
//    int degToTurn;
//    cyData data;
//
//    float previous_edge = 0;
//    float current_edge;
//    float entry_barrier;
//    float exit_barrier;
//    float saved_edge;
//    int num_detections = 0;
//    int previous_detection = 0;
//    int min_detects = 2;
//    int middle_angle;
//    int smallest_width = 10000;
//
//    objs_t* objects;
//    objects->num_objs = 0;
//
//    lcd_printf("Scanning\nSurroundings...");
//    sprintf(temp, "%-10s%-10s\n", "Deg", "IR (cm)");
//    uart_print(temp);
//
//
//    turn_servo_to_deg(start_deg);
//    timer_waitMillis(1000);
//
//    for (i = (start_deg / deg_inc); i < scans; i++) {
//        current_edge = 0;
//        if ((i >= .5 * scans) && (end_deg > 180)) {
//            degToTurn = i * deg_inc - 180;
//        }
//        else {
//            degToTurn = i * deg_inc;
//        }
//        repeated_ir_scan(&data, &current_edge, num_samples, degToTurn);
//        sprintf(temp, "%-10d%-10.3f\n", i * deg_inc, current_edge);
//        uart_print(temp);
//
//        if (previous_edge > 0) {
//            entry_barrier = 0.8  * previous_edge;
//            exit_barrier = 1.2 * previous_edge;
//
//            if ((current_edge < entry_barrier)) { // This would indicate a potential item
//                objects->start_ang[objects->num_objs] = i * deg_inc;
//                num_detections = 1;
//                previous_detection = 1;
//                saved_edge = entry_barrier;
//            }
//
//            else if ((current_edge < exit_barrier) && (current_edge < saved_edge) && (previous_detection == 1)) {  // In case still falling into an object, replace with object edge
//                num_detections += 1;
//            }
//
//            else if ((current_edge > exit_barrier) && (num_detections >= min_detects) && (previous_detection == 1)) { // Detection requirements have been met
//                objects->tot_ang[objects->num_objs] = (i * deg_inc) - objects->start_ang[objects->num_objs];
//                middle_angle = objects->start_ang[objects->num_objs] + (objects->tot_ang[objects->num_objs] / 2);
//                objects->distance[objects->num_objs] = 0;
//                for (k = 0; k < 10; k++) {
//                    if (middle_angle > 180) {
//                        cyScan(middle_angle - 180, &data, 1, 0, 25);
//                    }
//                    else {
//                        cyScan(middle_angle, &data, 1, 0, 500);
//                    }
//                    objects->distance[objects->num_objs] += data.ping_dist / 10;
//                }
//                objects->width[objects->num_objs] = objects->distance[objects->num_objs] * (objects->tot_ang[objects->num_objs] * PI / 180.0);
//
//                if (objects->width[objects->num_objs] < smallest_width) {
//                    smallest_width = objects->width[objects->num_objs];
//                    objects->smlst_width_ang = get_botAng(middle_angle, objects->distance[objects->num_objs]);
//                    objects->smlst_width_dist = objects->distance[objects->num_objs];
//                }
//
//                objects->num_objs += 1;
//                previous_detection = 0;
//            }
//
//            else if ((current_edge > exit_barrier) && (num_detections < min_detects) && (previous_detection == 1)) {
//                previous_detection = 0;
//            }
//        }
//        previous_edge = current_edge;
//
//        if ((i == (.5 * scans) - 1) && (end_deg > 180)) {
//            turn_deg(self, 180, 100);
//        }
//    }
//
//    sprintf(temp, "%-5s%-5s%-5s%-10s%-10s\n", "Obj#", "Deg", "TotA", "Dist", "Width");
//    uart_print(temp);
//    for (i = 0; i < objects->num_objs; i++) {
//        sprintf(temp, "%-5d%-5d%-5d%-10.3f%-10.3f\n", i + 1, objects->start_ang[i], objects->tot_ang[i], objects->distance[i], objects->width[i]);
//        uart_print(temp);
//    }
//    return objects;
//}

void cyScan(int degrees, cyData* data, char ping, char ir, int wait_time) {
    float ping_holder = 0;
    int adc_holder = 0;
    int i;

    turn_servo_to_deg(degrees);
    timer_waitMillis(wait_time);

    if (ping) {
        for (i = 0; i < 20; i++)
            ping_holder += get_ping() / 20;
            timer_waitMicros(15);
    }

    if (ir) {
        ADC0_PSSI_R |= 0x1; // Initiate SS0, this bit is automatically cleared once sampling initiates
        while (adc_data_flag == 0) {} // Wait for ADC flag to be set
        adc_holder = adc_data_recv;
        adc_data_flag = 0; // clear adc flag
        timer_waitMicros(1);
    }

    data->ping_dist = float_min(60, ping_holder);
    data->IR_dist = float_min(60, IR_interpret(ir_int_to_volt(adc_holder)));
}

//int num_scans = 0;
//objs_t static_scan(objs_t prev_objs, bool reset_num_scans) {
//    /* Adeife's current iteration checks the last seen object against the current seen object to detect movement. What if the civilian is crossing leftwards
//     * and there is a static obstacle in more left than the civilian  e.g. [   OBJ        <-CV   ]\
//     * The scan will see the CV, save it in the array, and then save the stationary obj
//     * It will say "stationary obj obstructing view" when there is actually a civilian being scanned; it's just that the stationary obj will be detected last
//     * until the civilian crosses the obj.
//     *
//     */
//    if (reset_num_scans) {
//        num_scans = 0;
//    }
//
//    objs_t current_objs;
//    current_objs.num_objs = 0;
//    current_objs.civilian_leftwards = false;
//    current_objs.civilian_rightwards = false;
//    cyData sensor_data;
//
//    int startAngle = 0;
//    int currentDist = -100;
//    int prev_obj_angle, prev_obj_distance;
//    float current_edge;
//    int i;
//
//    turn_servo_to_deg(50);
//    timer_waitMillis(1000);
//
//    for(i = 52; i <= 136; i+=2){
//        timer_waitMillis(6);
//        repeated_ir_scan(&sensor_data, &current_edge, 5, i);
//
//        if(abs(currentDist - sensor_data.IR_dist) < 20) { }
//        else{
//            // CODE TO DETECT OBJECTS, OBJECTS CROSSING LEFT, RIGHT AND STATIONARY OBJECTS
//            // if ocj seen for > 4degrees, obj less than 50cm away, object more than 20 cm away, and sensor_dist > current_dist (exit condition)
//            //current_dist is actually previous deg's distance in disguise
//            //
//
//            if ((i - startAngle > 4) && (currentDist < 50) && (currentDist != -100) && (sensor_data.IR_dist > currentDist)){
//                current_objs.start_ang[current_objs.num_objs] = startAngle;
//                current_objs.distance[current_objs.num_objs] = currentDist;
//                current_objs.tot_ang[current_objs.num_objs] = i - startAngle;
//                current_objs.width[prev_objs.num_objs] = current_objs.distance[prev_objs.num_objs] * (current_objs.tot_ang[prev_objs.num_objs] * PI / 180.0);
//
//                // compare present object detected with object detected in previous scan
//                current_objs.num_scans ++;
//                if (num_scans > 1){
//                    prev_obj_angle = (prev_objs.start_ang[current_objs.num_objs] + prev_objs.tot_ang[current_objs.num_objs]);
//                    prev_obj_distance = prev_objs.distance[current_objs.num_objs];
//                    // check if object is crossing to the left
//                    if(currentDist < prev_obj_distance && i > prev_obj_angle) {
//                        lcd_printf("CIVILIAN CROSSING LEFTWARDS");
//                        current_objs.civilian_leftwards = true;
//                        oi_setWheels(0, 0);
//                        // add code to determine how long to wait
//                   }
//                   // check if object is crossing to the right
//                   // Should change to if ((current distance > previous_obj_distance * .9) && (current distance < previous_obj_distance * 1.1))?
//                   else if(currentDist < prev_obj_distance && i < prev_obj_angle) {
//                       lcd_printf("CIVILIAN CROSSING RIGHTWARDS");
//                       current_objs.civilian_rightwards = true;
//                       oi_setWheels(0, 0);
//                       // add code to determine how long to wait
//                   }
//                   // check if object is a tall object
//                   else if (currentDist < prev_obj_distance && i == prev_obj_angle) {
//                       lcd_printf("TALL OBJECT OBSTRUCTION");
//                       oi_setWheels(0, 0);
//                       // add code to determine how long to wait
//                   }
//                }
//                current_objs.num_objs += 1;
//                prev_obj_distance = currentDist;
//                prev_obj_angle = i;
//            }
//            startAngle = i;
//            currentDist = sensor_data.IR_dist;
//        }
//    }
//    return current_objs;
//}

objs_t final_ir_only_scan(objs_t* prev_objs, bool reset_num_scans, bool check_moving, int end_deg, int start_deg, int deg_inc) {
    char temp[50];
    int i; // Increments over degrees & iterations per degree
    int num_sensor_samples = 1;
    cyData data;
    objs_t objects;

    float previous_edge = 0;
    float current_edge;
    float entry_barrier;
    float exit_barrier;
    float saved_edge;
    int num_detections = 0;
    int previous_detection = 0;
    int min_detects = 2;

    turn_servo_to_deg(start_deg);
    timer_waitMillis(750);
    objects.num_objs = 0;
    for (i = start_deg; i <= end_deg; i+= deg_inc) {

        current_edge = 0;
        repeated_ir_scan(&data, &current_edge, num_sensor_samples, i);
        sprintf(temp, "%-10d%-10.3f\n", i, current_edge);
        uart_print(temp);

        if (previous_edge > 0) {
            entry_barrier = 0.9  * previous_edge;
            exit_barrier = 1.1 * previous_edge;

            if (!check_moving) {
                if ((current_edge < entry_barrier)) { // This would indicate a potential item
                    objects.start_ang[objects.num_objs] = i;
                    num_detections = 1;
                    previous_detection = 1;
                    saved_edge = entry_barrier;
                }

                else if ((current_edge < exit_barrier) && (current_edge < saved_edge) && (previous_detection == 1)) {  // In case still falling into an object, replace with object edge
                    num_detections += 1;
                }

                else if ((current_edge > exit_barrier) && (num_detections >= min_detects) && (previous_detection == 1)) { // Detection requirements have been met
                    objects.tot_ang[objects.num_objs] = i - objects.start_ang[objects.num_objs];
                    objects.distance[objects.num_objs] = previous_edge;
                    objects.num_objs += 1;
                    previous_detection = 0;
                }

                else if ((current_edge > exit_barrier) && (num_detections < min_detects) && (previous_detection == 1)) { //Cancel dection because of early exit
                    previous_detection = 0;
                } 
            }
            else {
                //Code for checking moving object
            }
        }
        previous_edge = current_edge;
    }

    sprintf(temp, "%-5s%-5s%-5s%-10s\n", "Obj#", "Deg", "TotA", "Dist");
    uart_print(temp);
    for (i = 0; i < objects.num_objs; i++) {
        sprintf(temp, "%-5d%-5d%-5d%-10.3f\n", i + 1, objects.start_ang[i], objects.tot_ang[i], objects.distance[i]);
        uart_print(temp);
    }
    return objects;
}

void ir_calibrate() {
    cyData data;
    char temp[50];
    while (1) {
        cyScan(90, &data, 1, 1, 5);
        sprintf(temp, "%-10.3f%-10.3f\n", data.ping_dist, data.IR_dist);
        uart_print(temp);
        timer_waitMillis(75);
    }
}

