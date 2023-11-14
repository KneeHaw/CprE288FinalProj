/*
*
*   uart.h
*
*   Used to set up the UART
*   uses UART1 at 115200
*
*
*   @author Dane Larson
*   @date 07/18/2016
*   Phillip Jones updated 9/2019, removed WiFi.h
*/

#ifndef ADC_H_
#define ADC_H_

#include "Timer.h"

void adc_init(void);

int adc_read();


#endif
