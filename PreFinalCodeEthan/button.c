/*
 * button.c
 *
 *  Created on: Jul 18, 2016
 *      Author: Eric Middleton, Zhao Zhang, Chad Nelson, & Zachary Glanz.
 *
 *  @edit: Lindsey Sleeth and Sam Stifter on 02/04/2019
 *  @edit: Phillip Jone 05/30/2019: Mearged Spring 2019 version with Fall 2018
 */
 


//The buttons are on PORTE 3:0
// GPIO_PORTE_DATA_R -- Name of the memory mapped register for GPIO Port E, 
// which is connected to the push buttons
#include "button.h"


/**
 * Initialize PORTE and configure bits 0-3 to be used as inputs for the buttons.
 */
void button_init() {
	static uint8_t initialized = 0;

	//Check if already initialized
	if(initialized){
		return;
	}
	
	// 1) Turn on PORTE system clock, do not modify other clock enables
	SYSCTL_RCGCGPIO_R |= 0x10;

	// 2) Set the buttons as inputs, do not modify other PORTE wires
	GPIO_PORTE_DIR_R &= 0xF0;
	
	// 3) Enable digital functionality for button inputs,
	GPIO_PORTE_DEN_R |= 0x0F;

	
	initialized = 1;
}

/**
 * Returns the position of the rightmost button being pushed.
 * @return the position of the righttmost button being pushed. 4 is the rightmost button, 1 is the leftmost button.  0 indicates no button being pressed
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

extern volatile int button_num;

int gpioe_handler() {
    GPIO_PORTE_ICR_R = 0xFF;
    return button_getButton();
}

void init_button_interrupts() {
    // In order to configure GPIO ports to detect interrupts, you will need to visit pg. 656 in the Tiva datasheet.
    // Notice that you already followed some steps in 10.3 for initialization and configuration of the GPIO ports in the function button_init().
    // Additional steps for setting up the GPIO port to detect interrupts have been outlined below.
    // TODO: Complete code below

    // 1) Mask the bits for pins 0-3
    GPIO_PORTE_IM_R &= 0xF0;

    // 2) Set pins 0-3 to use edge sensing
    GPIO_PORTE_IS_R &= 0xF0;

    // 3) Set pins 0-3 to use both edges. We want to update the LCD
    //    when a button is pressed, and when the button is released.
    GPIO_PORTE_IBE_R |= 0x0F;

    // 4) Clear the interrupts
    GPIO_PORTE_ICR_R = 0xFF;

    // 5) Unmask the bits for pins 0-3
    GPIO_PORTE_IM_R |= 0x0F;

    NVIC_EN0_R |= 0b0000000000010000;
    IntRegister(INT_GPIOE, gpioe_handler);
}

void button_init_wrapper() {
//    init_button_interrupts();
    button_init();
}
