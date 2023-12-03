/*
 * imu.h
 *
 *  Created on: Nov 14, 2023
 *      Author: kneehaw
 */

#ifndef IMU_H_
#define IMU_H_
#include "bno055.h"
#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "lcd.h"

void I2C_init_wrapper();

void test_function();

void init_IMU_wrapper();

void calibrate_gyro();

void print_euler_angles();

void printAngle(char* axis, float val);
#endif /* IMU_H_ */
