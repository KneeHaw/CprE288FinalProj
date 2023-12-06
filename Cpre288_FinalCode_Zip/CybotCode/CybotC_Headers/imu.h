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

void I2C_init_wrapper(); //My own function, no longer used

void test_function(); //My own function, no longer used

void init_IMU_wrapper(); //takes care of everything IMU

void calibrate_gyro(); //Used when calibrating

#endif /* IMU_H_ */
