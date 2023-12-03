/*
 * PART 1:

IR 1 is connected to PB4 on Tiva, using AIN10
Choose ADC Module ACO0 or ADC1
Choose Sequence Sampler SS0, SS1, SS2, SS3

Requirements:
1. Repeatedly take samples of data
2. Displayed quantization values will be dynamic, based on conversion produced via ADC
    a. Potential readout noise can be reduced via sensor sample rate slowing and how often the displayed values are updated

Note 10.2.2.1, using interrupts to cast ADC conversion


SYSCTL_RCGCGPIO_R |= 0b00000010; // Enable clock to GPIO B
GPIO_PORTB_AFSEL_R |= 0b00010000; // Select an alternate function for PortB pin 4
GPIO_PORTB_PCTL_R &= 0xFFF0FFFF; // Force 0's at pin4, Analog Function
GPIO_PORTB_DEN_R &= 0b11101111; // Force pin 4 to be non digital
GPIO_PORTB_AMSEL_R |= 0b00010000; // Enable analog functions to pin 4 with  no isolation circuitry
GPIO_PORTB_ADCCTL_R |= 0b00010000; // Allow B pin4 to trigger ADC
GPIO_PORTB_DIR_R &= 0b11101111; // Force pin 4 to be input (voltage)


// ADCEMUX needs to be configured for external trigger
 *
 *
 *
ADC0_ACTSS_R &= 0b0; // Clear and disable SS0 in ADCACTSS
SYSCTL_RCGCADC_R |= 0b01; Set ADC Module 0's clock

ADC0_EMUX_R &= 0xFFFFFF00; //Clear bits 0-3
ADC0_EMUX_R |= 0b00000100; //SS0 Trigger Source set to GPIO

ADC0_SSMUX0_R &= 0x00000000; // Clear sample sources
ADC0_SSMUX0_R |= 0x00001010; // Set sample 0 to AIN10
 *
ADC0_SSCTL0_R &= 0x00000000; //Clear SS0 setup
ADC0_SSCTL0_R |= 0b111; //Takes one sample and sends an interrupt afterward


// If using interrupts, set ADCIM (CAN INDICATE WHEN SAMPLES ARE COMPLETED!!!!) clear interrupts via writing a 1 to ADCISC
ADC0_ACTSS_R |= 0b1; // Enable SS0 logic in ADCACTSS

*/
#include "ir_adc.h"


float ir_int_to_volt(int ir_val) {
    return ir_val * 3.3 / 4095;
}


void init_adc_portb() {
    // We know there is a constant level being sensed, what now?
    SYSCTL_RCGCGPIO_R |= 0b00000010; // Enable clock to GPIO B
    GPIO_PORTB_DIR_R &= 0b11101111; // Force pin 4 to be input (voltage)
    GPIO_PORTB_AFSEL_R |= 0b00010000; // Select an alternate function for PortB pin 4

    GPIO_PORTB_PCTL_R &= 0xFFF0FFFF; // Force 0's at pin4, Analog Function
    GPIO_PORTB_ADCCTL_R |= 0b00010000; // Allow B pin4 to trigger ADC

    GPIO_PORTB_DEN_R &= 0b11101111; // Force pin 4 to be non-digital
    GPIO_PORTB_AMSEL_R |= 0b00010000; // Enable analog functions to pin 4 with  no isolation circuitry

    GPIO_PORTB_IM_R &= 0b11101111;
//    GPIO_PORTB_IS_R |= 0b00010000; //Level sensitive
//    GPIO_PORTB_IS_R &= 0b11101111; //Edge sensitive
//    GPIO_PORTB_IBE_R &= 0b11101111;
//    GPIO_PORTB_IEV_R |= 0b00010000;
//    GPIO_PORTB_ICR_R |= 0b00010000; //shouldn't matter for edge detect
//    GPIO_PORTB_IM_R |= 0b00010000; // Interrupt for pin4
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


/*
 * Sample Averaging
 * Use ADC0_SAC_R to get some number of oversamples before it's sent to the FIFO for reading (generating an interrupt to be read)
 */
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
