/*
* Author: Ethan Rogers (kneehaw@iastate.edu)
* Date: 10/XX/2023
 *
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
#include "music.h"
#include "imu.h"
#include "instruction_set.h"
#include "boundary.h"


volatile char received_data;
volatile char new_data_flag = 0;
char exit_flag = 0;
int bump = 0;

void exit_main() {
    exit_flag = 1;
}

objs_t* saved_obj;


void handle_input(char input, oi_t* self) {
    if (input == 'A') {
        get_instructions(self, 'A');
    } else if (input == 'B') {
        get_instructions(self, 'B');
    } else if (input == 'C') {
        get_instructions(self, 'C');
    } else if (input == 'D') {
        get_instructions(self, 'D');
    } else if (input == 'E') {
        get_instructions(self, 'E');
    } else if (input == 'F') {
        get_instructions(self, 'F');
    } else if (input == 'x') {
        exit_main();
    } else if (input == 'o') {
        servo_calibrate();
    } else if (input == 'e') {
        turn_deg(self, 180., 50);
    } else if (input == 'w') {
        oi_setWheels(200, 200);
    } else if (input == 'a') {
        oi_setWheels(100, -100);
    } else if (input == 's') {
        oi_setWheels(-200, -200);
    } else if (input == 'd') {
        oi_setWheels(-100, 100);
    } else if (input == 'b') {
        lcd_printf("Battery: %.1f", (100 * (float) self->batteryCharge / (float) self->batteryCapacity));
    } else {
        oi_setWheels(0, 0);
    }
}

int main(void) {
    //BEGIN INIT
    lcd_init();
    timer_init();
    uart_init_wrapper();
    adc_init_wrapper();
    ping_init_wrapper();
    button_init_wrapper();
    servo_init_wrapper();
    init_IMU_wrapper();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    //END INIT

    //BEGIN EXTERNAL INSTRUCTIONS HANDLER
    char received_char;
    lcd_printf("Waiting for order...");

    while (exit_flag == 0) {
        received_char = uart_get_char();
        handle_input(received_char, sensor_data);
    }
    //END INSTRUCTIONS HANDLER

    //BEGIN STOPPING ROOMBA
    oi_setWheels(0, 0);
    oi_free(sensor_data);
    //END STOPPING ROOMBA

    return 0;
}
