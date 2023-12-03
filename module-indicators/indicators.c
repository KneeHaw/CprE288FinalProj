#include "indicators.h"
#include "tm4c123gh6pm.h"
#include "indicatorUtils.h"

#define MICROS_PER_TICK 64999UL // Number of microseconds in one timer cycle
int indicatorsInitialized = 0;
volatile int leftIndicatorState = 0;
volatile int rightIndicatorState = 0;

// Initialize the GPIO for the indicators
// PF1 - Left Indicator
// PF2 - Right Indicator
// PB3 - Brake Indicator
void initIndicators() {
    if (!indicatorsInitialized) {
        // Enable Clock for PORT F
        SYSCTL_RCGCGPIO_R |= 0x20;
        // Enable PORT F Pin 2 and 3 as a digital pins
        GPIO_PORTF_DEN_R = 0xE0;
        // Configure PORT F's Pins:  2 and 3; as digital output pins
        GPIO_PORTF_DIR_R = 0xE0;
        // Initialize ISR for PORT F Pin 1
        GPIO_PORTF_IS_R &= ~0x02;
        // Turn on the Clock to TIMER5
        SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;
        // Disable TIMER5 for setup
        TIMER3_CTL_R &= ~TIMER_CTL_TAEN;
        // Set as 16-bit timer
        TIMER3_CFG_R = TIMER_CFG_16_BIT;
        // Periodic, countdown mode
        TIMER3_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
        // Countdown time of 65ms
        TIMER3_TAILR_R = MICROS_PER_TICK - 1;
        // Clear timeout interrupt status
        TIMER3_ICR_R |= TIMER_ICR_TATOCINT;
        // 15M gives a period of 1s
        TIMER3_TAPR_R = 0xE4E1C0;
        // Allow TIMER5 timeout interrupts
        TIMER3_IMR_R |= TIMER_IMR_TATOIM;
        // Priority 7 (lowest)
        NVIC_PRI23_R |= NVIC_PRI23_INTA_M;
        // Enable TIMER5 interrupts
        NVIC_EN2_R |= (1 << 28);

        IntRegister(INT_TIMER5A, timer_clockTickHandler); // Bind the ISR
        TIMER5_CTL_R |= TIMER_CTL_TAEN; // Start TIMER5 counting

        indicatorsInitialized = 1;
    }
}

void turnLeftIndicatorOn() {
    if (!indicatorsInitialized) {
        return;
    }
    leftIndicatorState = 1;
}

void turnLeftIndicatorOff() {
    if (!indicatorsInitialized) {
        return;
    }
    leftIndicatorState = 0;
}

void turnRightIndicatorOn() {
    if (!indicatorsInitialized) {
        return;
    }
    rightIndicatorState = 1;
}

void turnRightIndicatorOff() {
     if (!indicatorsInitialized) {
        return;
    }
    rightIndicatorState = 0;
}

void turnBrakeIndicatorOn() {
    if (!indicatorsInitialized) {
        return;
    }
    GPIO_PORTB_DATA_R |= 0x08;
}

void turnBrakeIndicatorOff() {
    if (!indicatorsInitialized) {
        return;
    }

    GPIO_PORTB_DATA_R &= ~0x08;
}

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
static void timer_clockTickHandler() {
    if(leftIndicatorState) {
        turnLeftOn();
        leftIndicatorState = 1;
    } else {
        turnLeftOff();
        leftIndicatorState = 0;
    }
    if(rightIndicatorState) {
        turnRightOn();
        rightIndicatorState = 1;
    } else {
        turnRightOff();
        rightIndicatorState = 0;
    }

    TIMER3_ICR_R |= TIMER_ICR_TATOCINT;// Clear interrupt flag
}
