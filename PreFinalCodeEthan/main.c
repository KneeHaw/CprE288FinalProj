/*
* Author: Ethan Rogers (kneehaw@iastate.edu)
* Date: 10/XX/2023
* Description: Lab X
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"
#include "open_interface.h"
#include "Timer.h"
#include "uart.h"
#include "ir_adc.h"
#include "scan.h"
#include <math.h>
#include "movement.h"
#include "ping.h"
#include "servo.h"
#include "button.h"


volatile char received_data;
volatile char new_data_flag = 0;
char exit_flag = 0;
int bump = 0;

objs_t saved_obj;


void handle_input(char input, oi_t* sensor_data) {
    new_data_flag = 0x00;
    if (input == 'w') {
        lcd_printf("Moving Forward!");
        oi_setWheels(200, 200);
    }
    else if (input == 's') {
        lcd_printf("Moving Backward!");
        oi_setWheels(-200, -200);
    }
    else if (input == 'a') {
        lcd_printf("Turning Left!");
        oi_setWheels(200, -200);
    }
    else if (input == 'd') {
        lcd_printf("Turning Right!");
        oi_setWheels(-200, 200);
    }
    else if (input == 'q') {
        oi_setWheels(0, 0);
        saved_obj = perform_detailed_scan(sensor_data, 2, 360);
        lcd_printf("'g' = YES AUTO\nOther = CANCEL\nTO: %d, %.2f", saved_obj.smlst_width_ang, saved_obj.smlst_width_dist);
        while ((new_data_flag & 0xFF) == 0) {}
        char c = received_data;
        switch (c) {
        case 'g':
            turn_deg(sensor_data, 90, 100);
            turn_deg(sensor_data, saved_obj.smlst_width_ang, 100); //Turn to the smallest object's middle
            move_distance(sensor_data, 10 * (saved_obj.smlst_width_dist - 14), 100, 0);

            lcd_printf("Movement Complete\n%.3f", saved_obj.smlst_width_dist - 14);
        default:
            lcd_printf("Default State!");
        }
    }
    else if (input == 'e') {
        oi_setWheels(0, 0);
        saved_obj = perform_detailed_scan(sensor_data, 2, 180);
    }
    else if (input == 'l') {
        ir_calibrate();
    }
    else if (input == 'o') {
        oi_setWheels(0, 0);
        servo_calibrate();
    }
    else if (input == 'x') {
        oi_setWheels(0, 0);
        exit_flag = 1;
    }
    else {
        oi_setWheels(0, 0);
    }
    new_data_flag = 0x00;
}

int main(void) {
    uart_init_wrapper();
    adc_init_wrapper();
    ping_init_wrapper();
    button_init_wrapper();
    servo_init_wrapper();

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    lcd_init();
    timer_init();
    char c;

    lcd_printf("Waiting to drive...");
    while (exit_flag == 0) {
        while ((new_data_flag & 0xFF) == 0) {}
        c = received_data;
        handle_input(c, sensor_data);
    }
    oi_setWheels(0, 0);
    oi_free(sensor_data);

    return 0;
}
