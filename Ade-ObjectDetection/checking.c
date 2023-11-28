/**
 * main.c
 */
#include "movement.h"
#include "Timer.h"
#include "lcd.h"
#include "servo.h"
//#include "button.h"
#include "adc.h"
//#include "cyBot_Scan.h"
#include "uart.h"
#include "stdio.h"
#include <stdlib.h>
#include "open_interface.h"
#include <inc/tm4c123gh6pm.h>

int main5(){
    uart_init();
    uart_interrupt_init();
    timer_init();
    servo_init();

    lcd_init();
    adc_init();
    servo_move(50);
    timer_waitMillis(100);
    servo_move(0);
   // servo_move(47);
   // servo_move(49);
}
