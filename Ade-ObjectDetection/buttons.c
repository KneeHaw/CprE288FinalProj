/*
 * buttons.c
 *
 *  Created on: Sep 17, 2023
 *      Author: joo & ife
 */


#include "buttons.h"


/**
 * Initialize PORTE and configure bits 0-3 to be used as inputs for the buttons.
 */
void button_init() {
    static uint8_t initialized = 0;

    //Check if already initialized
    if(initialized){
        return;
    }

    // delete warning after implementing
    // #warning "Unimplemented function: void button_init()"

    // Reading: To initialize and configure GPIO PORTE, visit pg. 656 in the
    // Tiva datasheet.

    // Follow steps in 10.3 for initialization and configuration. Some steps
    // have been outlined below.

    // Ignore all other steps in initialization and configuration that are not
    // listed below. You will learn more about additional steps in a later lab.

    // 1) Turn on PORTE system clock, do not modify other clock enables
    SYSCTL_RCGCGPIO_R |= (0b00010000);

    // 2) Set the buttons as inputs, do not modify other PORTE wires
    GPIO_PORTE_DIR_R &= (0xfffffff0);
    GPIO_PORTE_DEN_R |= (0x0F);

    // gpio pull down resistor is not working. U shud ask?
    GPIO_PORTE_PDR_R |= (0x0F);

    initialized = 1;
}



/**
 * Returns the position of the rightmost button being pushed.
 * @return the position of the rightmost button being pushed. 4 is the rightmost button, 1 is the leftmost button.  0 indicates no button being pressed
 */
uint8_t button_getButton() {

    // #warning "Unimplemented function: uint8_t button_getButton(void)"   // delete warning after implementing

    //
    // DELETE ME - How bitmasking works
    // ----------------------------------------
    // In embedded programming, often we only care about one or a few bits in a piece of
    // data.  There are several bitwise operators that we can apply to data in order
    // to "mask" the bits that we don't care about.
    //
    //  | = bitwise OR      & = bitwise AND     ^ = bitwise XOR     ~ = bitwise NOT
    //        << x = shift left by x bits        >> x = shift right by x bits
    //
    // Let's say we want to know if push button 3 (S3) of GPIO_PORTE_DATA_R is
    // pushed.  Since push buttons are high (1) initially, and low (0) if pushed, PORTE should
    // look like:
    // GPIO_PORTE_DATA_R => 0b???? ?0?? if S3 is pushed
    // GPIO_PORTE_DATA_R => 0b???? ?1?? if S3 is not pushed
    //
    // This is not useful: There are 128 different 8 bit numbers that have the 3rd bit high or low.
    // We can make it more clear if we mask the other 7 bits:
    //
    // Bitwise AND:
    // (GPIO_PORTE_DATA_R & 0b0000 0100) => 0b0000 0000 if S3 is pushed
    // (GPIO_PORTE_DATA_R & 0b0000 0100) => 0b0000 0100 if S3 is not pushed
    //
    // Bitwise OR:
    // (GPIO_PORTE_DATA_R | 0b1111 1011) => 0b1111 1011 if S3 is pushed
    // (GPIO_PORTE_DATA_R | 0b1111 1011) => 0b1111 1111 if S3 is not pushed
    //
    // Other techniques (Shifting and bitwise AND)
    // ((GPIO_PORTE_DATA_R >> 2) & 1) => 0 if S3 is pushed
    // ((GPIO_PORTE_DATA_R >> 2) & 1) => 1 if S3 is not pushed

    // TODO: Write code below -- Return the left must button position pressed

    // INSERT CODE HERE!
    if ((GPIO_PORTE_DATA_R & 0b00001000) == 0b00000000){
        return 4;
    }
    else if ((GPIO_PORTE_DATA_R & 0b00000100) == 0b00000000){
        return 3;
    }
    else if ((GPIO_PORTE_DATA_R & 0b00000010) == 0b00000000){
        return 2;
    }
    else if ((GPIO_PORTE_DATA_R & 0b00000001) == 0b00000000){
        return 1;
    }


    return 0; // EDIT ME
}
