/*
 * ir_adc.h
 *
 *  Created on: Oct 17, 2023
 *      Author: Ethan Rogers (kneehaw@iastate.edu)
 */

#ifndef IR_ADC_H_
#define IR_ADC_H_

#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"


void init_adc();

void init_adc_portb();

void adc_ss0_handler();

void init_adc_isr();

void adc_init_wrapper();

#endif /* IR_ADC_H_ */
