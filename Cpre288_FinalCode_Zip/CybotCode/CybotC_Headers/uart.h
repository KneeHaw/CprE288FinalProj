/*
 * uart.h
 *
* Author: Ethan Rogers (kneehaw@iastate.edu)
* Date: 10/3/2023
* Description: UART Library for Cpre288 CyBot
*/

#ifndef UART_H_
#define UART_H_

#include "Timer.h"
#include <inc/tm4c123gh6pm.h>

void uart_init(void);

void uart_sendChar(char data);

char uart_receive(void);

void uart_print(const char *str);

void uart_interrupt_init();

void uart_init_wrapper();

char uart_get_char();

#endif /* UART_H_ */
