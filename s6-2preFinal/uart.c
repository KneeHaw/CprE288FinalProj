/*
*
*   uart.c
*
*
*
*
*
*   @author
*   @date
*/

#include "uart.h"
#include "timer.h"
#define REPLACE_ME 0x00

// Global varibles
volatile char c = 'h';
volatile char d = ' ';


void uart_init()
{
    SYSCTL_RCGCGPIO_R |= 0x2;      // enable clock GPIOB (page 340)
    SYSCTL_RCGCUART_R |= 0x2;      // enable clock UART1 (page 344)
    GPIO_PORTB_AFSEL_R |= 0x3;      // sets PB0 and PB1 as U1RX and U1TX as peripherals (page 671)
    GPIO_PORTB_PCTL_R &= 0xffffff11;
    GPIO_PORTB_PCTL_R  |= 0x11;       // pmc0 and pmc1       (page 688)  also refer to page 650
    GPIO_PORTB_DEN_R   |= 0x3;        // enables pb0 and pb1
    GPIO_PORTB_DIR_R   |= 0x2;        // sets pb1 as output
    GPIO_PORTB_DIR_R   &= ~(0x1);   // pb0 as input

    //compute baud values [UART clock= 16 MHz], clock_div in HSE = 16 by default
    // use a baud rate = 115200, #Date bits = 8, #Stop bits =1, Parity = None
    // BRD = BRDI + BRDF = UARTSysClk / (ClkDiv * Baud Rate) = 16M / (16*115200) = 8.68
    // UARTFBRD[DIVFRAC] = integer(BRDF * 64 + 0.5) = (0.68 * 64 + 0.5) = 44.06
    int fbrd = 44;
    int    ibrd = 8;


    UART1_CTL_R &= ~(0x1);      // disable UART1 (page 918)
    UART1_IBRD_R = ibrd;        // write integer portion of BRD to IBRD
    UART1_FBRD_R = fbrd;   // write fractional portion of BRD to FBRD
    UART1_LCRH_R |= 0x60;        // write serial communication parameters (page 916) * 8bit and no parity
    // UART1_CC_R   = REPLACE_ME;          // use system clock as clock source (page 939)
    UART1_CTL_R |= 0x1;        // enable UART1

}


char reverseBits(char c) {
    c = ((c & 0x55) << 1) | ((c & 0xAA) >> 1);
    c = ((c & 0x33) << 2) | ((c & 0xCC) >> 2);
    c = ((c & 0x0F) << 4) | ((c & 0xF0) >> 4);
    return c;
}



void uart_sendChar(char data)
{
   //TODO
    // put value in txfifo
    // wait for the UARTFR.BUSY = 0
    //if(UART1_FR_R & 0b){
//    char back = ((data << 7) & 0x80) | ((data << 6) & 0x40) | ((data << 5) & 0x20) | ((data << 4) & 0x10)
//            | () | () | () | ();

    while(UART1_FR_R & 0x20);
    UART1_DR_R = data;
}


char uart_receive(void)
{
    while(UART1_FR_R & 0x10);
    return ((char) (UART1_DR_R & 0xff));
}

void uart_sendStr(const char *data)
{
    //TODO
    int i = 0;
    while(data[i] != 0){
        uart_sendChar(data[i++]);
    }
}


// _PART3
void uart1_handler(void){

    // STEP1: Check the Masked Interrup Status
    if(UART1_MIS_R & 0x10) {
        //STEP2:  Copy the data
        d = (char) (UART1_DR_R & 0xff);
        if( d == 't') {
            if( c=='m') c = 'h';
            else if(c == 'h') c = 'm';
        }


        //STEP3:  Clear the interrupt
        UART1_ICR_R |= 0x10;
    }

}

void uart_interrupt_init()
{
    // Enable interrupts for receiving bytes through UART1
    UART1_IM_R |= 0x10; //enable interrupt on receive - page 924

    // Find the NVIC enable register and bit responsible for UART1 in table 2-9
    // Note: NVIC register descriptions are found in chapter 3.4
    NVIC_EN0_R |= 0x40; //enable uart1 interrupts - page 104

    // Find the vector number of UART1 in table 2-9 ! UART1 is 22 from vector number page 104
    IntRegister(INT_UART1, uart1_handler); //give the microcontroller the address of our interrupt handler - page 104 22 is the vector number

}
