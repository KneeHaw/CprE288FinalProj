/*
 * Author: Ethan Rogers (kneehaw@iastate.edu)
 * Created on: Oct 17, 2023
 */

#include "ir_adc.h"

void init_adc_portb() {
    SYSCTL_RCGCGPIO_R |= 0b00000010; // Enable clock to GPIO B
    GPIO_PORTB_DIR_R &= 0b11101111; // Force pin 4 to be input (voltage)
    GPIO_PORTB_AFSEL_R |= 0b00010000; // Select an alternate function for PortB pin 4

    GPIO_PORTB_PCTL_R &= 0xFFF0FFFF; // Force 0's at pin4, Analog Function
    GPIO_PORTB_ADCCTL_R |= 0b00010000; // Allow B pin4 to trigger ADC

    GPIO_PORTB_DEN_R &= 0b11101111; // Force pin 4 to be non-digital
    GPIO_PORTB_AMSEL_R |= 0b00010000; // Enable analog functions to pin 4 with  no isolation circuitry

    GPIO_PORTB_IM_R &= 0b11101111;
}


void init_adc() {
    SYSCTL_RCGCADC_R |= 0x00000001; //Set ADC Module 0's clock
    timer_waitMillis(50); // Wait for clock to reach components

    ADC0_ACTSS_R &= 0xFFFEFFF0; // Clear and disable SS0 in ADCACTSS

    ADC0_EMUX_R &= 0xFFFF0000; //Clear bits 0-15 ADC can now be triggered via ADCPSSI SSn setting

    ADC0_SSMUX0_R &= 0x00000000; // Clear sample sources
    ADC0_SSMUX0_R |= 0x0000000A; // Set sample 0 to AIN10

    ADC0_SSCTL0_R &= 0x00000000; //Clear SS0 setup
    ADC0_SSCTL0_R |= 0x00000006; //Takes one sample and sends an interrupt afterward

    ADC0_SAC_R |= 0b011; // Set 8x over-sampling in ADC

    ADC0_ACTSS_R |= 0x00000001; // Enable SS0 logic in ADCACTSS
}

extern volatile char adc_data_flag;
extern volatile int adc_data_recv;

void adc_ss0_handler() {
    if (ADC0_RIS_R & 0b1) { //check for SS0) interrupt
        adc_data_flag = 1;
        adc_data_recv = ADC0_SSFIFO0_R;
        ADC0_ISC_R |= 0b1; // Clear interrupt
    }
    // if RIS do something
    // Can not use ISR and just check for RIS to be set (sample) number of times
}

void init_adc_isr() {
    ADC0_IM_R &= 0xFFF0FFF0; //Clear digital comparator interrupts and sample sequencer interrupts
    ADC0_IM_R |= 0x00000001; //Enable interrupt to interrupt controller for SS0
    NVIC_EN0_R |= 0x00004000; //ADC0 SS0 Interrupt bit number = 14
    IntRegister(INT_ADC0SS0, adc_ss0_handler);
}

void adc_init_wrapper() {
    init_adc_portb();
    init_adc();
    init_adc_isr();
}
