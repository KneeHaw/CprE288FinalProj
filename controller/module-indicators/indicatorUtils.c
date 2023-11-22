//
// Created by Conner Ohnesorge on 11/8/2023.
//

#include "indicatorUtils.h"
#include "tm4c123gh6pm.h"

void turnLeftOn() {
    GPIO_PORTF_DATA_R |= 0x04;
}

void turnLeftOff() {
    GPIO_PORTF_DATA_R &= ~0x04;
}

void turnRightOn() {
    GPIO_PORTF_DATA_R |= 0x08;
}

void turnRightOff() {
    GPIO_PORTF_DATA_R &= ~0x08;
}
