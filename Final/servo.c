/*
 * servo.c
 *
 *  Created on: Oct 25, 2023
 *      Author: kneehaw
 */

/*
 * ---- PRELAB ----
 * 1. How does the servo motor work?
 *    A: The servo will rotate its shaft according to the pulse width it receives. This pulse must be received at least once per 20ms
 *       Pulses of widths 1, 1.5, and 2 milliseconds will rotate the servo to 0, 90, and 180 degrees respectively.
 * 2. What Timer1 registers are relevant to this lab?
 *
 * 3. Given clock = 16MHz, 20ms would have how many clock cycles?
 *    A: 320,000 clock cycles (0.02seconds * 16,000,000cycles/second)
 *
 * 4.  Describe timer programming for the PWM signal in terms of high and low pulse. When is the output pin set, when is it cleared?
 *
 *
 * ---- LAB PART 1 ----
 * Features for the project:
 * 1. servo_pos = num_ms * 90 - 90, num_ms = (servo_pos + 90) / 90, num_cycles = 16000 * ((servo_pos + 90) / 90)
 * 2. PB5 is the output pins for the servo
 * 3. This pin will be configured to alternate function 7, T1CCP1
 * 4. This timer, Timer1B, will be a 16-bit timer with an 8bit prescaler (MSBs are in prescalaer)
 * 5. This timer must be a periodic PWN 24-bit count-down timer
 * 6. PWM signal should start high when initialized to the ILR and PR registers
 * 7. PWM will stay high until the TnR and TnPR matches a value in the TnMATCHR and TnPMR
 * 8. The counter should continue counting down to 0 with a low signal, reloading start values
 *
 */

#include "servo.h"
float deg_val = 177.78;  //positive = clockwise, negative = counter clockwise
int zero_clock = 58585;
int pi_clock = 29935;

void init_pb5_servo() {
    SYSCTL_RCGCGPIO_R |= 0b00000010; // Enable clock to GPIO B
    timer_waitMillis(50);
    GPIO_PORTB_DEN_R |= 0b00100000; // Force pin 5 to be digital
    GPIO_PORTB_DIR_R |= 0b00100000; // Force pin 5 to be output
    GPIO_PORTB_AFSEL_R |= 0b00100000;
    GPIO_PORTB_PCTL_R |= 0x700000; // T1CCP1 16/32b timer select if pin5 is using alternate function
}


int get_cycles(int degrees) {

    return (int)((16000 * ((degrees + 90) / 90) & 0x00FFFFFF) / 2);
}

void init_timer_servo() {
    SYSCTL_RCGCTIMER_R |= 0b10; //Enable clock to timer 1 module
    timer_waitMillis(50);
    TIMER1_CTL_R &= 0b111111011111111; //Disable Timer 1B
    TIMER1_CFG_R &= 0b000; // Clear config
    TIMER1_CFG_R = 0b100; // Set config to 0x4

    TIMER1_TBMR_R &= 0b000000000000;
    TIMER1_TBMR_R |= 0b100100001010; //Set periodic count down PWM

    TIMER1_CTL_R &= 0b001000011111111; // No inverting, no other settings relevant

    TIMER1_TBPR_R = 0x02; // Fulfilling 20ms requirement, clock is as slow as possible while still operational
    TIMER1_TBILR_R = 0xFFFF;

    int desired_degrees = 90;


    TIMER1_TBMATCHR_R = (TIMER1_TBILR_R - (get_cycles(desired_degrees) & 0x0000FFFF)); //Match lower 16bits
    TIMER1_TBPMR_R = TIMER1_TBPR_R;
    TIMER1_CTL_R |= 0b000000100000000; // enable timer 1B
}

void turn_servo_to_deg(int degrees) {
    int temp_val = zero_clock - (int)(((float)degrees / 180.) * (float)(zero_clock - pi_clock));
    TIMER1_TBMATCHR_R = temp_val;
}

void servo_calibrate() {
    int temp_val;
    char exit_calib_flag = 0;
    while(1) {
        switch (button_getButton()) {
        case 4:
            temp_val = TIMER1_TBMATCHR_R - 50;
            TIMER1_TBMATCHR_R = temp_val;
            break;
        case 3:
            temp_val = TIMER1_TBMATCHR_R + 50;
            TIMER1_TBMATCHR_R = temp_val;
            break;
        case 2:
            break;
        case 1:
            exit_calib_flag = 1;
        default:
            break;
        }
        if (exit_calib_flag) {
            break;
        }
        lcd_printf("Value: %d", TIMER1_TBMATCHR_R);
        timer_waitMillis(15);
    }
}

void servo_init_wrapper() {
    init_pb5_servo();
    init_timer_servo();
}
