/*
 * adc.c
 *
 *  Created on: Oct 17, 2023
 *      Author: joo and adeife
 */
#include "adc.h"


void adc_init()
{
    // pb4
    // ain10
    // adc0
    // ss3

    // SETUP  clocks
    SYSCTL_RCGCGPIO_R |= 0x2;
    SYSCTL_RCGCADC_R |=  0x1;

    // SETUP  gpio
    GPIO_PORTB_DIR_R &= ~(0x10);
    GPIO_PORTB_AFSEL_R |= 0x10;
    GPIO_PORTB_DEN_R   &= ~(0x10);
    GPIO_PORTB_AMSEL_R |= 0x10;



    //SETUP ADC
    // ENABLE DITHER
    ADC0_CTL_R |= 0x40;

    // This register controls the amount of hardware averaging applied to conversion results. The final
    // conversion result stored in the FIFO is averaged from 2 AVG consecutive ADC samples at the specified ADC speed.
    ADC0_SAC_R |= 0x6;



    // SAMPLE SEQUENCE CONFIG
    // sample sequencer config most likely ss3 since uses one sample

    // 1. disable sample sequencer ADCACTSS
    ADC0_ACTSS_R &= ~(0x1);

    // 2. config trigger event in ADCEMUX
    //The trigger is initiated by setting the SSn bit in the ADCPSSI register in the processor
    ADC0_EMUX_R |= 0x0;
    // ADCTSSEL not needed, CAUSE NOT USING PWM

    // 3. For each sample in the sample sequence, configure the corresponding input source in the ADCSSMUXn register
    ADC0_SSMUX0_R |= 0xaaaaaaaa;

    // 4. For each sample in the sample sequence, configure the sample control bits
    // in the corresponding nibble in the ADCSSCTLn register.
    // ADCSSCTL3 IEn bit is set, enabling a raw interrupt.
    ADC0_SSCTL0_R |= 0x64444444;

     // 5. enable sample sequencer ADCACTSS
     ADC0_ACTSS_R |= 0x1;



}


int adc_read(){
    // CHECK BUSY
    while(ADC0_ACTSS_R & 0x10000);

    // Sampling is then initiated by setting the SSn bit in
    // the ADC Processor Sample Sequence Initiate (ADCPSSI) register.
    ADC0_PSSI_R |= 0x88000001;

    // check ADCRIS. A sample has completed conversion and the respective
    // ADCSSCTL2 IEn bit is set, enabling a raw interrupt.
    while(!(ADC0_RIS_R & 0x1));

    // clear interrupt
    ADC0_ISC_R |= 0x1;

    // or maybe use ADC Sample Sequence FIFO 0 Status (ADCSSFSTAT0)
    // just a check, if fifo empty is 1, and full is 0, return 0xffff, which is more than 12bits
    if((ADC0_SSFSTAT3_R & 0x1000 == 0) && (ADC0_SSFSTAT3_R & 0x100 == 1)) return 0xffff;

    // After a sample sequence completes execution, the result data can be retrieved from the ADC
    // Sample Sequence Result FIFO (ADCSSFIFOn) registers. The FIFOs are simple circular buffers
    // that read a single address to "pop" result data
    int i;
    int result=0;
    for(i=0; i<8; i++) result += (ADC0_SSFIFO0_R & 0xfff) / (double) 8;

    return result;
}





