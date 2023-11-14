/*
 * servo.c
 *
 *  Created on: Oct 29, 2023
 *      Author: joo
 */
#include "servo.h"

int start_value = 325000;
int end_value = 355000;

void servo_init(void){
    // pb5 //altfunc 7
    // 16/32 BIT timer 1
    // ccp1 TIMER B
    // pwm mode
    // copnfigure for 24 bit count-down mode
    // oeriodic mode
    // the 8-bit prescalar will be used as an extension to get a 24-bit timer  by initializing the ILR and PR registers to 0xFFFF and 0xFF respectively).
    // with the upper timer value stored in the GPTMTnPR register and the lower bits in the GPTMTnILR register

    // SETUP GPIO
    SYSCTL_RCGCGPIO_R |= 0x2;
    GPIO_PORTB_DIR_R   |= 0x20;
    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_DEN_R |= 0x20;
    GPIO_PORTB_PCTL_R |= 0x00700000;


    // SETUP TIMER
    SYSCTL_RCGCTIMER_R |= 0x2;

    // 1. Ensure the timer is disabled (the TnEN bit is cleared) before making any changes.
    TIMER1_CTL_R &= ~(0x100);

    // 2. Write the GPTM Configuration (GPTMCFG) register with a value of 0x0000.0004.
    TIMER1_CFG_R = 0x4;

    // 3. In the GPTM Timer Mode (GPTMTnMR) register, set the TnAMS bit to 0x1, the TnCMR bit to
    // 0x0, and the TnMR field to 0x2.
    TIMER1_TBMR_R |= 0x8;
    TIMER1_TBMR_R &= ~(0x4);
    TIMER1_TBMR_R |= 0x2;


    // 8. Load the GPTM Timer n Match (GPTMTnMATCHR) register with the match value.
    TIMER1_TBPMR_R = 0xFF & (320000 >> 16);
    TIMER1_TBMATCHR_R = (0xFFFF & (320000));

    // 4. Configure the output state of the PWM signal (whether or not it is inverted) in the TnPWML field
    // of the GPTM Control (GPTMCTL) register.
    TIMER1_CTL_R &= ~(0x4000);

}

static inline int servo_move_raw(float degrees){

    int value = start_value + (degrees/ (float)180) * (end_value - start_value);

    // 1. Ensure the timer is disabled (the TnEN bit is cleared) before making any changes.
    TIMER1_CTL_R &= ~(0x100);

    // 5. If a prescaler is to be used, write the prescale value to the GPTM Timer n Prescale Register (GPTMTnPR).
    TIMER1_TBPR_R = 0xFF & (value >> 16);

     // 7. Load the timer start value into the GPTM Timer n Interval Load (GPTMTnILR) register.
     TIMER1_TBILR_R = (0xFFFF & value);

     // 9. Set the TnEN bit in the GPTM Control (GPTMCTL) register to enable the timer and begin
     // generation of the output PWM signal.
     TIMER1_CTL_R |= 0x100;


     timer_waitMillis(400);

     //Ensure the timer is disabled (the TnEN bit is cleared) before making any changes.
     TIMER1_CTL_R &= ~(0x100);

     return degrees;
}

int servo_move(float degrees){
    if(degrees > 180) degrees = 180;
    if(degrees < 0) degrees = 0;
    return servo_move_raw(degrees);
}

void servo_calibrate(){

    int button;

    int count = servo_move_raw(0);
    while ((button =  button_getButton()) > 0 ){
        if(button == 1) servo_move_raw(count - 1);
        if(button == 2) break;
    }
    lcd_printf("start_value = %d. \n", (start_value = start_value + (count * (end_value - start_value))/ (float)180 ));

    count = servo_move_raw(180);
    while ((button =  button_getButton()) > 0 ){
        if(button == 3) servo_move_raw(count + 1);
        if(button == 4) break;
    }
    lcd_printf("end_value = %d. \n", (end_value = start_value + (count * (end_value - start_value))/ (float)180 ));

}




