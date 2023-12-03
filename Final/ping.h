/*
 * ping.h
 *
 *  Created on: Oct 18, 2023
 *      Author: kneehaw
 */

#ifndef PING_H_
#define PING_H_

#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include <stdio.h>
#include "uart.h"


void init_pb3_ping();

void activate_ping();

void ping_init_wrapper();

float get_ping();

#endif /* PING_H_ */
