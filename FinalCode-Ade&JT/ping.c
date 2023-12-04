#include "ping.h"

#define CLOCK_CYCLE_TIME 12000000L

int overflow;

void ping_init( ){
    // pb3
    // 16/32 BIT timer 3
    // ccp1
    // input edge time mode
    // copnfigure for 16 bit count-down mode
    // the 8-bit prescalar will be used as an extension to get a 24-bit timer  by initializing the ILR and PR registers to 0xFFFF and 0xFF respectively).
    // with the upper timer value stored in the GPTMTnPR register and the lower bits in the GPTMTnILR register

    SYSCTL_RCGCGPIO_R |= 0x2;
    GPIO_PORTB_PCTL_R |= 0x00007000;
    GPIO_PORTB_DEN_R   |= 0x8;

    SYSCTL_RCGCTIMER_R |= 0x8;

    // CONFIGURE TIMER FOR INPUT CAPTURE MODE
    // 1. Ensure the timer is disabled (the TnEN bit is cleared) before making any changes.
    TIMER3_CTL_R &= ~(0x100);

    // 2. Write the GPTM Configuration (GPTMCFG) register with a value of 0x0000.0004.
    TIMER3_CFG_R = 0x4;
    // TIMER3_PP_R &= 0; // 16BIT MODE WITH 8 BIT PRESCALAR

    // 3. In the GPTM Timer Mode (GPTMTnMR) register, write the TnCMR field to 0x1 and the TnMR
    // field to 0x3 and select a count direction by programming the TnCDIR bit.
    TIMER3_TBMR_R |= 0x4;
    TIMER3_TBMR_R |= 0x3;
    TIMER3_TBMR_R &= ~(0x10);

    // and the type of event that the timer captures is determined by the TnEVENT fields of the GPTMCTL register.
    TIMER3_CTL_R |= 0xC00;

    // 5. If a prescaler is to be used, write the prescale value to the GPTM Timer n Prescale Register (GPTMTnPR).
    // 6. Load the timer start value into the GPTM Timer n Interval Load (GPTMTnILR) register.
    // the 8-bit prescalar will be used as an extension to get a 24-bit timer  by initializing the ILR and PR registers to 0xFFFF and 0xFF respectively).
    // with the upper timer value stored in the GPTMTnPR register and the lower bits in the GPTMTnILR register
    TIMER3_TBPR_R |= 0xFF;
    TIMER3_TBILR_R |= 0xFFFF;

    // 7. If interrupts are required, set the CnEIM bit in the GPTM Interrupt Mask (GPTMIMR) register.
    // If the capture mode event interrupt is enabled in the GPTM Interrupt Mask (GPTMIMR) register,
    // the GPTM also sets the CnEMIS bit in the GPTM Masked Interrupt Status (GPTMMIS) register
    // TIMER3_MIMR_R |= 0x400; // NOT NEEDED USING POLLING INSTEAD OF NVIC
    //    NVIC_EN1_R |= 0x10;
    //   IntMasterEnable();

}

static inline void sendPulse(){
    GPIO_PORTB_AFSEL_R &= ~(0x8);
    GPIO_PORTB_DIR_R |= 0x8;
    GPIO_PORTB_DATA_R |= 0x8;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= ~(0x8);
}

void part1(){
    sendPulse();
}



int ping_read() {
    // pb3 //altfunc 7
    // 16/32 BIT timer 3
    // ccp1 TIMER B
    // input edge time mode
    // copnfigure for 16 bit count-down mode
    // the 8-bit prescalar will be used as an extension to get a 24-bit timer  by initializing the ILR and PR registers to 0xFFFF and 0xFF respectively).
    // with the upper timer value stored in the GPTMTnPR register and the lower bits in the GPTMTnILR register


    // PART 1
    // pulse
    TIMER3_CTL_R &= ~(0x100);
    TIMER3_ICR_R |=  0x400;
    sendPulse();

    // PART 2
    // change to input AND mux with timer
    GPIO_PORTB_DIR_R &= ~(0x8);
    GPIO_PORTB_AFSEL_R |= 0x8;
    TIMER3_CTL_R |= 0x100;


    // Poll the CnERIS bit in the GPTMRIS register or wait for the interrupt to be generated (if enabled).
    // In both cases, the status flags are cleared by writing a 1 to the CnECINT bit of the GPTM
    //  Interrupt Clear (GPTMICR) register. The time at which the event happened can be obtained by reading the GPTM Timer n (GPTMTnR) register.
    //When the selected input event is detected, the current timer counter value is captured in the
    //    GPTMTnR and GPTMTnPS register and is available to be read by the microcontroller. The GPTM
    //    then asserts the CnERIS bit in the GPTM Raw Interrupt Status (GPTMRIS) register
    //  and holds it until it is cleared by writing the GPTM Interrupt Clear (GPTMICR) register
    while(!(TIMER3_RIS_R &  0x400));
    int initial = (int) ( (0x0000FFFF & TIMER3_TBR_R) );
    TIMER3_ICR_R |=  0x400;

    while(!(TIMER3_RIS_R &  0x400));
    int final = (int) ( (0x0000FFFF & TIMER3_TBR_R) );
    TIMER3_ICR_R |=  0x400;

    // When software writes the TnEN bit in the GPTMCTL register, the timer is enabled for event capture.
    TIMER3_CTL_R &= ~(0x100);
    timer_waitMicros(200);
    if (initial > final) return (initial - final);

    overflow++;
    return 0xFFFF - final + initial;
}

int ping_read_time_us(int ping_read){
    return ping_read/15;
}

int ping_read_dist(int ping_read_time_us){
    return (ping_read_time_us * 34) / 2000;
}

