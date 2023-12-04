/*
 * ping.h
 *
 *  Created on: Oct 22, 2023
 *      Author: joo
 */

#ifndef PING_H_
#define PING_H_

#include "Timer.h"


void ping_init(void);
void part1();
int ping_read(void);
int ping_read_time_us(int ping_read);
int ping_read_dist(int ping_read_time_us);

#endif /* PING_H_ */
