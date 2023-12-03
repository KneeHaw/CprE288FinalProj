/*
 * imu.c
 *
 *  Created on: Nov 14, 2023
 *      Author: kneehaw
 *      Code taken and edited from RencheckYoself Github
 */
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inc/hw_gpio.h"
#include "inc/hw_i2c.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "imu.h"
#include "init_imu.h"
#include "uart.h"

#define BNO055_ADDRESS_TX ((0x29 << 1) | 0x00)
#define BNO055_ADDRESS_RX ((0x29 << 1) | 0x01)

void init_I2C() {
    //1. Enable I2C clock via RCGCI2C
    SYSCTL_RCGCI2C_R |= 0b0010; //Enable I2C 1
    //2. Enable GPIO clock (I2C INT GPIO3 = PB2, RSTN GPIO2 = PB6, ADDR GPIO1 = PB7, SCL1 = PA6, SDA = PA7)
    SYSCTL_RCGCGPIO_R |= 0b00000011;

    timer_waitMillis(50);
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);
    HWREG(I2C1_BASE + I2C_O_FIFOCTL) = 80008000;

    GPIO_PORTB_DEN_R |= 0b10000000; //Set ADDR enable
    GPIO_PORTB_DIR_R |= 0b10000000; //Set addr as output
    GPIO_PORTB_DATA_R |= 0b10000000; //Set addr high as 0x29
}

//int generate_start_condition(uint8_t address_to_read) {
//    I2C1_MSA_R = BNO055_ADDRESS_TX;
//    I2C1_MDR_R = address_to_read;
//    I2C1_MCS_R = 0b00011;
//    while (I2C1_MCS_R & 0b00000001) {
//        lcd_printf("Waiting for BUS\nAddress %x\n%x", I2C1_MCS_R, I2C1_MSA_R);
//    }
//    if (I2C1_MCS_R & 0b00000010) {
//        lcd_printf("Not Working\nAddress");
//        return 1;
//    }
//    return 0;
//}
//
//int generate_receive() {
//    I2C1_MSA_R = BNO055_ADDRESS_RX;
//    I2C1_MCS_R = 0b00011;
//    while (I2C1_MCS_R & 0b00000001) {
//        lcd_printf("Waiting for BUS\nRead");
//    }
//    if (I2C1_MCS_R & 0b00000010) {
//        lcd_printf("Not Working\nRead");
//        return 0xFFFFFFFF;
//    }
//    return (I2C1_MDR_R);
//}
//
//int generate_stop_condition() {
//    I2C1_MCS_R = 0b00100;
//    while (I2C1_MCS_R & 0b00000001) {
//        lcd_printf("Waiting for BUS\nSTOP");
//    }
//    if (I2C1_MCS_R & 0b00000010) {
//        lcd_printf("Not Working\nSTOP");
//        return 1;
//    }
//    return 0;
//}

//void I2C_init_wrapper() {
//    init_I2C();
//}

//int result_test;
//void test_function() {
//    if (I2C1_MCS_R & 0b1) {
//        lcd_printf("I2C was already\nbusy... introducing\n|Infinite Loop|");
//        while (1) {}
//    }
//    if (generate_start_condition(0x08) == 0) {
//        result_test = generate_receive();
//        if (result_test > 0x000000FF) {
//            lcd_printf("True Failure");
//        }
//        else {
//            if (generate_stop_condition() == 1) {
//                lcd_printf("Error Stopping");
//            }
//            else {
//                lcd_printf("Received:\n%x", result_test);
//            }
//        }
//    }
//}
void load_stored_calibration();
void init_IMU_wrapper() {
    ConfigureI2C();
    init_imu();
    load_stored_calibration();
    set_imu_mode(BNO055_OPERATION_MODE_IMUPLUS);



}

void load_stored_calibration() {
//    set_imu_mode(BNO055_OPERATION_MODE_CONFIG);
    struct bno055_accel_offset_t accel_offsets;
    accel_offsets.r = 0x3E8;
    accel_offsets.x = 0;
    accel_offsets.y = 0;
    accel_offsets.z = 0;
    bno055_write_accel_offset(&accel_offsets);

    struct bno055_gyro_offset_t gyro_offsets;
    gyro_offsets.x = 0;
    gyro_offsets.z = 0;
    gyro_offsets.y = 0;
    bno055_write_gyro_offset(&gyro_offsets);

    struct bno055_mag_offset_t mag_offsets;
    mag_offsets.x = 0;
    mag_offsets.z = 0;
    mag_offsets.y = 0;
    mag_offsets.r = 0x1E0;
    bno055_write_mag_offset(&mag_offsets);
}
void calibrate_gyro() {
    int calibrated = 0;
    bool flag = true;
    volatile Calibration cal;

    // Loop until the device is fully calibrated
    while(calibrated == 0)
    {
      cal = calibrate_imu();

      lcd_printf("Gyro: %d, Acc: %d, Mag: %d, Sys: %d\nError: %i", cal.gyro, cal.accl, cal.magn, cal.syst, get_error());

      if((cal.gyro == 3) && (cal.accl == 3) && cal.magn == 3 && cal.syst == 3)
      {
        calibrated = 1;
      }
      ms_delay(10);
    }
    //Read calibration registers
    struct bno055_accel_offset_t accel_offsets;
    bno055_read_accel_offset(&accel_offsets);

    struct bno055_gyro_offset_t gyro_offsets;
    bno055_read_gyro_offset(&gyro_offsets);

    struct bno055_mag_offset_t mag_offsets;
    bno055_read_mag_offset(&mag_offsets);
    lcd_printf("%x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x", accel_offsets.r, accel_offsets.x, accel_offsets.y, accel_offsets.z,
               gyro_offsets.x, gyro_offsets.y, gyro_offsets.z, mag_offsets.x, mag_offsets.y, mag_offsets.z, mag_offsets.r);

    while(flag) {
        if (uart_get_char()) {
            flag = false;
        }
    }
}

void print_euler_angles() {
    struct bno055_euler_float_t ea; // calculate the euler angles
    bno055_convert_float_euler_hpr_deg(&ea);
    lcd_clear();
    printAngle("x", ea.r);
    printAngle("y", ea.p);
    printAngle("z", ea.h);
    ms_delay(300);
}

void printAngle(char* axis, float val) {
  char msg_buf[20]; // message

  sprintf(msg_buf, "%s = %.3f, ", axis, val);
  lcd_puts(msg_buf);
}
