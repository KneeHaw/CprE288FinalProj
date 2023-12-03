/*
* Author: Ethan Rogers (kneehaw@iastate.edu)
* Date: 10/3/2023
* Description: UART Library for Cpre288 CyBot
*/

#include <string.h>
#include "uart.h"

void uart_init(void) {
    //Port B utilizes module 1 UART
    SYSCTL_RCGCUART_R |= 0b00000010; // Enable UART RCGCUART module (344)
    SYSCTL_RCGCGPIO_R |= 0b00000010; // Enable clock to GPIOB

    GPIO_PORTB_AFSEL_R |= 0b00000011;
    GPIO_PORTB_PCTL_R &= 0xFFFFFF00; // Force 0's at desired locations
    GPIO_PORTB_PCTL_R |= 0x00000011; // Force 1's at desired locations

    GPIO_PORTB_DEN_R |= 0b00000011;
    GPIO_PORTB_DIR_R &= 0b11111100; // Force 0's at desired locations
    GPIO_PORTB_DIR_R |= 0b00000001; // Force 1's at desired locations

    UART1_CTL_R &= 0b1111111111111110; //Disable UARTEN in UARTCTL (918)

//    int brdi = 8; // From BRD equations TIVA pg 896
//    int brdf = 44; // From BRD equations TIVA pg 896
    UART1_IBRD_R = 0b0000000000001000; // Integer BRD to UARTIBRD and Fractional to UARTFBRD (914, 915)
    UART1_FBRD_R = 0b0000000000101100;
    UART1_LCRH_R |= 0b01100000;// Write serial Parameters to  UARTLCRH (916) Bit5 could be changed
    UART1_CC_R = 0b0000; // Configure UART clock source (UARTCC)

    UART1_CTL_R |= 0b0000000000000001; // Enable UARTCTL
}

void uart_sendChar(char data) {
    if (UART1_FR_R & 0b10000000) { //place data to be read if transmitter is empty
        UART1_DR_R = data;
    }
    while (!(UART1_FR_R & 0b10000000)) { // wait for empty transmitter
    }
}

char uart_receive(void) {
    char data;
    while (1) {
        if (UART1_FR_R & 0b01000000) {  // Check that the receive module has data (loop until such)
            data = UART1_DR_R;
            break;
        }
    }
    return data;
}

void uart_print(const char* str) {
    int i;
    char c;
    for(i = 0; i < strlen(str); i++) {
        c = str[i];
        uart_sendChar(c);
    }
}

extern volatile char received_data;
extern volatile char new_data_flag;

void uart_interrupt_handler() {
    received_data = uart_receive();
    new_data_flag = 0x01;
}

void uart_interrupt_init() {
    UART1_IM_R |= 0b010000;
    NVIC_EN0_R |= 0b0000000001000000;
    IntRegister(INT_UART1, uart_interrupt_handler);
}

void uart_init_wrapper() {
    uart_init();
    uart_interrupt_init();
}
