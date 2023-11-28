/*
 * buttons.h
 *
 *  Created on: Sep 17, 2023
 *      Author: joo & ife
 */
#ifndef BUTTONS_H_
#define BUTTONS_H_


#include <stdint.h>
#include <inc/tm4c123gh6pm.h>


//initialize the push buttons
void button_init();


///Non-blocking call
///Returns highest value button being pressed, 0 if no button pressed
uint8_t button_getButton();



#endif /* MOVEMENT_H_ */
