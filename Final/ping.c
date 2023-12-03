/*
 * ping.c
 *
 *  Created on: Oct 18, 2023
 *      Author: kneehaw
 */


/*
 * ---- PRE LAB ----
 * 1. The registers that I used are important
 * 2. 34300 / 2
 * 3. GPTM Raw gives all interrupt statuses, MIS gives the interrupt status of unmasked bits (not set by IMR)
 *
 *
 * In order to implement ping sensor:
 * ---- PART 1 ---- Activating the Sensor
 * PB3 Must be first initialized as an output, sending a specific length low, high, low pulse
 * 750uS low, 2-5uS high, 750uS low (use timer module?)
 *
 *
 * ---- PART 2 ---- Getting a response
 * PB3 Must then be switched to an input that finds the first rising edge, tracking the time until a subsequent falling edge
 * Ensure that when using the GPTM (general purpose timer module) the GPTMTnILR is NOT set to 0xFFFF
 *
 * Input Edge Time Mode
 * 0. Enable RCGCTIMER to correct timer
 * 1. Disable TnEN (timer enable)
 * 2. Write GPTMCFG with 0x0000004
 * 3. Write TnCMR of GPTMTnMR to 0x1 and TnMR to 0x3, select count direction via TnCDIR (down)
 * 4. Configure type of event being captured in GPTMCTL's TnEVENT field
 * 5. Prescale configure in CPTMTnPR
 * 6. Load timer start value into GPTMTnILR
 * 7. interrupts, use CnEIM of GPTMIMR
 * 8. Set TnEN of CPTMCTL to enable the timer and start counting
 * 9. Poll CnERIS or set interrupt up to know when value has been generated, read value from GPTMTnR, clear interrupt by writing GPTMICR's CnECINT
 */

#include "ping.h"

void init_pb3_ping() {
    SYSCTL_RCGCGPIO_R |= 0b00000010; // Enable clock to GPIO B
    timer_waitMillis(50);
    GPIO_PORTB_DEN_R |= 0b00001000; // Force pin 3 to be digital
    GPIO_PORTB_PCTL_R |= 0x7000; // T3CCP1 16/32b timer select if pin3 is using alternate function
}

void activate_ping() {
    GPIO_PORTB_AFSEL_R &= 0b11110111; // Disable alternate function for Pin3
    GPIO_PORTB_DIR_R |= 0b00001000; // Force pin 3 to be output

    GPIO_PORTB_DATA_R &= 0b11110111; //Force LOW 1/2
    GPIO_PORTB_DATA_R |= 0b00001000; //Force HIGH 1/1
    timer_waitMicros(10);
    GPIO_PORTB_DATA_R &= 0b11110111; //Force LOW 2/2
}

void init_timer_ping() {
    SYSCTL_RCGCTIMER_R |= 0b001000; //Enable clock to timer 3 module
    timer_waitMillis(50);
    TIMER3_CTL_R &= 0b111111011111111; //Disable Timer 3B
    TIMER3_CFG_R &= 0b000; // Clear config
    TIMER3_CFG_R |= 0b100; // Set config to 0x4

    TIMER3_TBMR_R |= 0b000100000111; //Set capture mode edge-time(1xx) and timer mode capture(x11)
    TIMER3_TBMR_R &= 0b111111100111; //Clear TBAMS and set counting down

    TIMER3_CTL_R &= 0b001111111111111;
    TIMER3_CTL_R |= 0b000111000000000; //Enable both edges detection

    TIMER3_TBPR_R = 0xFF;
    TIMER3_TBILR_R = 0xFFFF;

    TIMER3_CTL_R |= 0b000000100000000; // enable timer 3B
}

int counter;
volatile int flag;
int start_time;
int end_time;
volatile int holder;
float total_time;

void timer_interrupt_handler() {
    if (TIMER3_RIS_R & 0b10000000000) {
        holder = TIMER3_TBR_R;
        TIMER3_ICR_R |= 0b10000000000; //Clear timer interrupt for edge
        flag = 1;
    }
}

void init_timer_interrupt() {
    NVIC_EN1_R |= 0x00000010; //Timer3B interrupt
    IntRegister(INT_TIMER3B, timer_interrupt_handler);
}


float get_ping() {
    holder = 0;
    counter = 0;
    start_time = 0;
    end_time = 0;
    flag = 0;

    activate_ping();

    GPIO_PORTB_DIR_R &= 0b11110111; // Force pin 3 to be input
    GPIO_PORTB_AFSEL_R |= 0b00001000; // Select alternate function for porbB
    TIMER3_ICR_R |= 0b010000000000; //Clear timer interrupt for edge
    TIMER3_IMR_R |= 0b010000000000; //Enable timer interrupt for edge

    while (1) {
        if (flag && (start_time == 0) && (counter == 0)) {
            start_time = holder;
            counter = 1;
            flag = 0;
        }
        else if (flag && (end_time == 0) && (counter == 1)) {
            end_time = holder;
            TIMER3_IMR_R &= 0b101111111111; //Disable timer interrupt for edge
            counter = 2;
        }
        else if (counter == 2) {
            if ((start_time - end_time) > 0) {
                total_time = ((float)(start_time - end_time)) / 16000000;
            }
            else {
                total_time = ((float)(start_time + (0xFFFFFF - end_time))) / 16000000;
            }
            break;
        }

    }
    return total_time * (34300 / 2); //return distance in cm (34300cm/s)
}

void ping_init_wrapper() {
    init_pb3_ping();
    init_timer_ping();
    init_timer_interrupt();
}
