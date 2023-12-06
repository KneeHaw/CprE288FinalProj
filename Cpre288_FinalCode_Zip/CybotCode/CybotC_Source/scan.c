/*
 * scan.c
 *
 * Author: Ethan Rogers (kneehaw@iastate.edu)
 * Date: 10/3/2023
 * Description: Scan Library for Cpre288 CyBot
 *
 * --Heavily edited Dec 1, 2023 through Dec 5 2023
 */

#include <math.h>
#include <stdbool.h>
#include "scan.h"
#include "custom_math.h"

#define E 2.718281828459045 //There is a math.h number, but whatevs

/*
 * Note timing optimizations to ensure a fast scan (turning 180 deg in an avg of .94sec)
 * This time could be reduced with less noisy peripherals
 * Assuming less samples, ~.6s was the lowest reasonable time @ 2deg per turn with servo being the bottleneck
 */

volatile char adc_data_flag = 0;
volatile int adc_data_recv = 0;

void get_sensor_data(int degrees, distance_sensor_data* data, char ping, char ir, int wait_time) {
    int i;

    turn_servo_to_deg(degrees);

    // The per degree wait time is roughly 6-7ms
    // This is the time required to make the move steadily and be ready for another movement
    timer_waitMillis(wait_time);

    // Only waste time on ping if getting distance
    if (ping) {
        float ping_array[20]; //More scans [x] means more likely to get the correct distance!
        for (i = 0; i < 20; i++) {
            ping_array[i] = fminf(100., get_ping());
            timer_waitMicros(15); // Found to reduce error
        }
        sort_float_array(ping_array, 20);
        data->ping_dist = get_median_of_float_array(ping_array, 20); // Implement median-of-data filtering
    }

    //Used for potential object detection, not true distance
    if (ir) {
        float adc_array[7]; //More scans [x] means more likely to get the correct distance!
        for (i = 0; i < 7; i++) {
            ADC0_PSSI_R |= 0x1; // Initiate SS0, this bit is automatically cleared once sampling initiates
            while (adc_data_flag == 0) {} // Wait for ADC flag to be set
            adc_array[i] = (fmaxf(10., fminf(80., IR_interpret(adc_data_recv))));
            adc_data_flag = 0; // clear adc flag
            timer_waitMicros(5); // Found to reduce error
        }
        sort_float_array(adc_array, 7);
        data->IR_dist = get_median_of_float_array(adc_array, 7); // Implement median-of-data filtering
    }
}

objs_t routine_scan(int end_deg, int start_deg, int deg_inc) {
    char temp[50];
    int i; // Increments over degrees
    distance_sensor_data data;
    objs_t objects;

    /*
     * IT IS IMPORTANT TO UNDERSTAND THESE VARIABLES AND THEIR USES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     */
    float previous_edge = 0; //Previous scan's IR data, initialized to 0 to skip first degree
    float current_edge; //Current scan's IR data
    float entry_barrier; // Provide wiggle room for noise in data gathering and defines start of object case
    float exit_barrier; // Provide wiggle room for noise in data gathering and defines end of object case
    float saved_edge; // Save the entry barrier of a potential object to reduce noise effects
    int num_detections = 0; // Track how many scans saw the object at distance x, reset per potential object
    int previous_detection = 0; //Do not detect on the next degree if detection has just occurred, reduce error
    int min_detects = 2; // An object at distance x must be seen this many times to be recorded
    int start_ang; // Start angle of potential object

    turn_servo_to_deg(start_deg);
    timer_waitMillis(850); // 100ms longer than minimum wait time for snapping from 180deg to 0deg
    objects.num_objs = 0;
    for (i = start_deg; i <= end_deg; i+= deg_inc) {

        current_edge = 0;
        get_sensor_data(i, &data, 0, 1, 6);
        current_edge = data.IR_dist;
        sprintf(temp, "%-10d%-10.3f\n", i, current_edge);
        uart_print(temp);

        // BEGIN OBJECT DETECTION LOGIC
        if (previous_edge > 0) {
            entry_barrier = 0.9  * previous_edge;
            exit_barrier = 1.1 * previous_edge;

            if ((current_edge < entry_barrier)) { // This would indicate a potential item
                start_ang = i;
                num_detections = 1;
                previous_detection = 1;
                saved_edge = entry_barrier;
            }

            else if ((current_edge < exit_barrier) && (current_edge < saved_edge) && (previous_detection == 1)) {
                // Increment detections of potential object
                num_detections += 1;
            }

            else if ((current_edge > exit_barrier) && (num_detections >= min_detects) && (previous_detection == 1)) {
                // [[[ Detection requirements have been met ]]]

                //Record object's angular data
                objects.tot_ang[objects.num_objs] = i - start_ang;
                objects.midd_ang[objects.num_objs] = start_ang + ((objects.tot_ang[objects.num_objs] / 2) - 2);

                //Use ping sensor to snap to the middle angle of a detected object and record its distance
                get_sensor_data(objects.midd_ang[objects.num_objs], &data, 1, 0, 500);
                objects.distance[objects.num_objs] = data.ping_dist;

                //Prepare for more scanning and potentially more objects
                objects.num_objs += 1;
                previous_detection = 0;
                turn_servo_to_deg(i);
                timer_waitMillis(500);
            }

            else if ((current_edge > exit_barrier) && (num_detections < min_detects) && (previous_detection == 1)) {
                //Cancel dection because of early exit case (noise likely caused this)
                previous_detection = 0;
            }
        }
        // END OBJECT DETECTION LOGIC
        previous_edge = current_edge;
    }

    // Print all pertinent data to terminal
    sprintf(temp, "%-5s%-5s%-5s%-10s\n", "Obj#", "Deg", "TotA", "Dist");
    uart_print(temp);
    for (i = 0; i < objects.num_objs; i++) {
        sprintf(temp, "%-5d%-5d%-5d%-10.3f\n", i + 1, start_ang, objects.tot_ang[i], objects.distance[i]);
        uart_print(temp);
    }
    return objects;
}

// An optional calibration function that can be turned on when using a new CyBot
void ir_calibrate(int i) {
    distance_sensor_data data;
    char temp[50];
    get_sensor_data(90, &data, 1, 1, 6);
    sprintf(temp, "%-10.3f%-10.3f\n", data.ping_dist, data.IR_dist);
    uart_print(temp);
}

