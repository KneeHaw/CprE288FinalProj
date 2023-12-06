/*
 * button.c
 *
 *  Created on: Jul 18, 2016
 *      Author: Eric Middleton, Zhao Zhang, Chad Nelson, & Zachary Glanz.
 *
 *  @edit: Lindsey Sleeth and Sam Stifter on 02/04/2019
 *  @edit: Phillip Jone 05/30/2019: Merged Spring 2019 version with Fall 2018
 *  @edit: Ethan Rogers 9/xx/2023: Implemented non-interrupt basic functionality
 */
 


//The buttons are on PORTE 3:0
#include "button.h"
/*
 * Initialize PORTE and configure bits 0-3 to be used as inputs for the buttons.
 */
void button_init() {
	SYSCTL_RCGCGPIO_R |= 0x10;  // 1) Turn on PORTE system clock, do not modify other clock enables
	GPIO_PORTE_DIR_R &= 0xF0;  // 2) Set the buttons as inputs, do not modify other PORTE wires
	GPIO_PORTE_DEN_R |= 0x0F;  // 3) Enable digital functionality for button inputs
}

/*
 * return the position of the righttmost button being pushed. 4 is the rightmost button, 1 is the leftmost button.
 * 0 indicates no button being pressed
 */

int button_getButton() {
    if ((GPIO_PORTE_DATA_R & 0b00001000) == 0) {
        return 4;
    }
    if ((GPIO_PORTE_DATA_R & 0b00000100) == 0) {
        return 3;
    }
    if ((GPIO_PORTE_DATA_R & 0b00000010) == 0) {
        return 2;
    }
    if ((GPIO_PORTE_DATA_R & 0b00000001) == 0) {
        return 1;
    }
    return 0;
}

void button_init_wrapper() {
    button_init();
}
