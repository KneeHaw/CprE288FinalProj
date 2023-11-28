/**
 * main.c
 */
#include "movement.h"
#include "Timer.h"
#include "lcd.h"
#include "servo.h"
//#include "button.h"
#include "adc.h"
//#include "cyBot_Scan.h"
#include "uart.h"
#include "stdio.h"
#include <stdlib.h>
#include "open_interface.h"
#include <inc/tm4c123gh6pm.h>

typedef struct {
    int first_angle;
    int second_angle;
    int width;
    int distance;
} object_t;
typedef object_t* object_p;

void add_obj(object_p* arr_obj, int index, int f_angle, int s_angle, int dist);
void print_string(char* text);
void manual_mode(oi_t* sensor_data);
bool auto_mode(object_p ans, oi_t* sensor_data);
extern volatile char c;
extern volatile char d;

int main(){
    uart_init();
    uart_interrupt_init();
   // cyBOT_init_Scan(0b0111);
    timer_init();
    servo_init();
    //button_init();
    lcd_init();
    adc_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    //right_calibration_value = 259000;
    //left_calibration_value = 1293250;

    //object_p arr_obj[5];
    //*arr_obj = calloc(sizeof(object_t), 5);
    //cyBOT_Scan_t scan;
    char text[100];
    int i;
    int read;
    int previous_obj_distance = 0;
    int previous_obj_angle = 0;
    int num_scans = 0;
    lcd_printf("FINAL PROJTEST");

//    oi_free(sensor_data);
    while(1)
    {
        if (1){

            read = 0;
            sprintf(text, "Degrees        Distance(cm)            ADC Values\n");
            print_string(text);
            object_p arr_obj[5];

            int num_obj=0, startAngle=0;
            int currentDist = -100;
            //cyBOT_Scan(0, &scan);
            servo_move(50);
            timer_waitMillis(100);

            for( i = 52; i <= 136; i+=2){
                oi_setWheels(15, 15);
                servo_move(i);
                //cyBOT_Scan(i, &scan);
                int j;
                for (j = 0; j < 5; j++){
                    read += adc_read();
                }
                read = read /5;
                int val = 4.321 + (81568170 - 4.321)/(1 + pow(read/0.688, 1.984));

                val = 10* ((int)val / 10);
                if (val >= 150){
                    val = 150;
                }

                sprintf(text, "%-20d %-20d %-10d\n", i, val);
                print_string(text);
                timer_waitMillis(10);
                if(abs(currentDist - val) < 20) {

                }
                else{
                    // CODE TO DETECT OBJECTS, OBJECTS CROSSING LEFT, RIGHT AND STAGNANT OBJECTS
                    if (i-startAngle > 4 && currentDist < 50 && currentDist != -100 && val > currentDist){
                        add_obj(arr_obj, num_obj++, startAngle, i, currentDist);
                        // compare present object detected with object detected in previous scan
                        num_scans++;
                        if (num_scans > 1){
                            // check if object is crossing to the left
                           if(currentDist < previous_obj_distance && i > previous_obj_angle){
                               lcd_printf("CIVILIAN CROSSING LEFT");
                               print_string("CIVILIAN CROSSING LEFT \n");
                               oi_setWheels(0, 0);
                               // add code to determine how long to wait
                               timer_waitMillis(5000);
                           }
                           // check if object is crossing to the right
                           else if(currentDist < previous_obj_distance && i < previous_obj_angle){
                               lcd_printf("CIVILIAN CROSSING RIGHT");
                               print_string("CIVILIAN CROSSING RIGHT \n");
                               oi_setWheels(0, 0);
                               // add code to determine how long to wait
                               timer_waitMillis(5000);
                           }
                           // check if object is a tall object
                           else if (currentDist < previous_obj_distance && i == previous_obj_angle){
                               lcd_printf("TALL OBJECT OBSTRUCTION");
                               print_string("TALL OBJECT OBSTRUCTION \n");
                               oi_setWheels(0, 0);
                               // add code to determine how long to wait
                               timer_waitMillis(5000);
                           }
                        }
                        previous_obj_distance = currentDist;
                        previous_obj_angle = i;
                    }
                    startAngle = i;
                    currentDist = val;
                }


            }

            sprintf(text, "Object#      Angle     Distance(cm)     Width\n");
            print_string(text);
            for(i = 0; i < num_obj; i++) {
                object_p ans = arr_obj[i];

                sprintf(text, "%10d %10d %10d %10d \n", i, (ans->first_angle + ans->second_angle) / 2, ans->distance, ans->width);
                print_string(text);
            }
            //send something
            sprintf(text, "end");
            print_string("end \n");


            if(num_obj > 0){
                object_p ans = arr_obj[0];
                for(i = 1; i < num_obj; i++) {
                    object_p current = arr_obj[i];

                    if(current->width < ans->width) {
                        ans = current;
                    }

                }

//                if(c == 'm') {
//                    manual_mode(sensor_data);
//                }
//                else if(c == 'h') {
//                    if(!auto_mode(ans, sensor_data)) break;
//
//                }
            }
//            else{
//                move_forward(sensor_data, 5);
//            }


        }
    }

    return 0;
}

void manual_mode(oi_t* sensor_data){
    while(1){
        d = uart_receive();
        if (d == 'w'){
           move_forward(sensor_data, 5);
           print_string("end \n");
       }
       if (d == 's'){
           move_backward(sensor_data, 5);
           print_string("end \n");
       }
       if (d == 'd'){
           turn_clockwise(sensor_data, MOVE_RIGHT_SMALL);
           print_string("end \n");
       }
       if (d == 'a'){
           turn_clockwise(sensor_data, MOVE_LEFT_SMALL);
           print_string("end \n");
       }
       if (d == ' '){
           oi_setWheels(0, 0);
           print_string("end \n");
       }
       if (d == 'f'){
           //oi_free(sensor_data);
           d = ' ';
           return;
       }
       d = ' ';
       if (sensor_data->bumpLeft){
           print_string("I HAVE BEEN BUMPED ON THE LEFT BRO!\n");
           print_string("end \n");
       }
       if (sensor_data->bumpRight){
           print_string("I HAVE BEEN BUMPED ON THE RIGHT BRO!\n");
           print_string("end \n");
       }
       oi_update(sensor_data);
       print_string("end \n");
    }
}

bool auto_mode(object_p ans, oi_t* sensor_data){
    int degrees = (ans->first_angle + ans->second_angle) / 2;
    //cyBOT_Scan(degrees, scan);
    servo_move(degrees);
    //                    int opposite = ans->distance * sin(degrees);
    //                    int rotate = atan(opposite / (8 + ans->distance));
//    oi_t *sensor_data = oi_alloc();
//    oi_init(sensor_data);
    int turn = 0, rotate = 0;
    if (degrees >= 0 && degrees <= 90){
        rotate = 90 - degrees;
        turn = -rotate+10;
    }
    else if (degrees > 90 && degrees <= 180){
        rotate = 180 - degrees;
        turn = rotate - 10;
    }
    turn_clockwise(sensor_data, turn);
    bool val = move_forward_with_bumps(sensor_data, ans->distance-10);
    print_string("Moved successfully!\n");
    print_string("end \n");
    oi_update(sensor_data);

    return val;
}



void print_string(char* text){
    int j;
    for ( j = 0; j < strlen(text); j++){
        uart_sendChar(text[j]);
    }
}


/**
 * function to add an obj to object_array
 */
void add_obj(object_p arr_obj[10], int index, int f_angle, int s_angle, int dist){
    object_p ans = malloc(sizeof(object_t));
    ans->first_angle = f_angle;
    ans->second_angle = s_angle;
    ans->width = s_angle - f_angle;
    ans->distance = dist;

    arr_obj[index] = ans;
}

