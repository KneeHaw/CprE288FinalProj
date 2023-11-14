/*
* Author: Ethan Rogers (kneehaw@iastate.edu)
* Date: 10/3/2023
* Description: Scan Library for Cpre288 CyBot
*/

#include <math.h>
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
    new_IR = pow(E, (3.6934023 / pow(IR_raw, 0.21835123))) - 13.58386622; //Values are from scipy curve fit
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

//Perform a scan of every N degrees, and then print out all data and code-detected objects
objs_t perform_detailed_scan(oi_t* sensor_data, int deg_inc, int tot_deg)
{

    int scans = tot_deg / deg_inc;

    char temp[50];
    int i, j, k;  // Increments over degrees & iterations per degree
    int num_samples = 3;
    int degToTurn;
    cyData data;

    float previous_edge = 0;
    float current_edge;
    float entry_barrier;
    float exit_barrier;
    float saved_edge;
    int num_detections = 0;
    int previous_detection = 0;
    int min_detects = 3;
    int middle_angle;
    int smallest_width = 10000;

    objs_t objects;
    objects.num_objs = 0;

    lcd_printf("Scanning Terrain...");
    sprintf(temp, "%-10s%-10s\n", "Deg", "IR (cm)");
    uart_print(temp);

    turn_servo_to_deg(0);
    timer_waitMillis(1000);
    // Begin 360 degree scan
    for (i = 0; i < scans; i++) {
        current_edge = 0;
        if ((i >= .5 * scans) && (tot_deg > 180)) {
            degToTurn = i * deg_inc - 180;
        }
        else {
            degToTurn = i * deg_inc;
        }
        for (j = 0; j < num_samples; j++) {
            cyScan(degToTurn, &data, 0, 1, 6);  //Deg to turn, data struct, ping?, ir?
            current_edge += data.IR_dist / num_samples;
            timer_waitMillis(1);
        }
        sprintf(temp, "%-10d%-10.3f\n", i * deg_inc, current_edge);
        uart_print(temp);

        if (previous_edge > 0) {
            entry_barrier = 0.8  * previous_edge;
            exit_barrier = 1.2 * previous_edge;

            if ((current_edge < entry_barrier)) { // This would indicate a potential item
                objects.start_ang[objects.num_objs] = i * deg_inc;
                num_detections = 1;
                previous_detection = 1;
                saved_edge = entry_barrier;
            }

            else if ((current_edge < exit_barrier) && (current_edge < saved_edge) && (previous_detection == 1)) {  // In case still falling into an object, replace with object edge
                num_detections += 1;
            }

            else if ((current_edge > exit_barrier) && (num_detections >= min_detects) && (previous_detection == 1)) { // Detection requirements have been met
                objects.tot_ang[objects.num_objs] = (i * deg_inc) - objects.start_ang[objects.num_objs];
                middle_angle = objects.start_ang[objects.num_objs] + (objects.tot_ang[objects.num_objs] / 2);
                objects.distance[objects.num_objs] = 0;
                for (k = 0; k < 10; k++) {
                    if (middle_angle > 180) {
                        cyScan(middle_angle - 180, &data, 1, 0, 25);
                    }
                    else {
                        cyScan(middle_angle, &data, 1, 0, 500);
                    }
                    objects.distance[objects.num_objs] += data.ping_dist / 10;
                }
                objects.width[objects.num_objs] = objects.distance[objects.num_objs] * (objects.tot_ang[objects.num_objs] * PI / 180.0);

                //Store where the smallest obj is via a check and stuct-stored index
                if (objects.width[objects.num_objs] < smallest_width) {
                    smallest_width = objects.width[objects.num_objs];
                    objects.smlst_width_ang = get_botAng(middle_angle, objects.distance[objects.num_objs]);
                    //objects.smlst_width_dist = get_botDist(middle_angle, objects.distance[objects.num_objs]);
                    objects.smlst_width_dist = objects.distance[objects.num_objs];
                }

                objects.num_objs += 1;
                previous_detection = 0;
            }

            else if ((current_edge > exit_barrier) && (num_detections < min_detects) && (previous_detection == 1)) {
                previous_detection = 0;
            } 
        }
        previous_edge = current_edge;

        if ((i == (.5 * scans) - 1) && (tot_deg > 180)) {
            turn_deg(sensor_data, 180, 100);
        }
    }

    sprintf(temp, "%-5s%-5s%-5s%-10s%-10s\n", "Obj#", "Deg", "TotA", "Dist", "Width");
    uart_print(temp);
    for (i = 0; i < objects.num_objs; i++) {
        sprintf(temp, "%-5d%-5d%-5d%-10.3f%-10.3f\n", i + 1, objects.start_ang[i], objects.tot_ang[i], objects.distance[i], objects.width[i]);
        uart_print(temp);
    }
    return objects;
}

void cyScan(int degrees, cyData* data, char ping, char ir, int wait_time) {
    float ping_holder = 0;
    int adc_holder = 0;

    turn_servo_to_deg(degrees);
    timer_waitMillis(wait_time);
    if (ping) {
        ping_holder = get_ping();
        timer_waitMicros(150);
    }

    if (ir) {
        ADC0_PSSI_R |= 0x1; // Initiate SS0, this bit is automatically cleared once sampling initiates
        while (adc_data_flag == 0) {} // Wait for ADC flag to be set
        adc_holder = adc_data_recv;
        adc_data_flag = 0; // clear adc flag
        timer_waitMicros(1);
    }

    data->ping_dist = float_min(120., ping_holder);
    data->IR_dist = float_min(80., IR_interpret(ir_int_to_volt(adc_holder)));
//    data->IR_dist = ir_int_to_volt(adc_holder);

}

void ir_calibrate(){
    cyData data;
    char temp[50];
    while (1) {
        cyScan(90, &data, 1, 1, 5);
        sprintf(temp, "%-10.3f%-10.3f\n", data.ping_dist, data.IR_dist);
        uart_print(temp);
        timer_waitMillis(75);
    }
}

