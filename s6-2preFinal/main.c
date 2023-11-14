#include "servo.h"


/**
 * main.c
 */
int main2(void)
{

//    // PART 1
//    timer_init();
//    servo_init();
//    servo_move(90);


    // PART 2
    timer_init();
    servo_init();
    button_init();
    lcd_init();
    // servo_calibrate();
    int count = servo_move(90);
    int button = button_getButton();
    int dir = 1;

    // YOUR CODE HERE
    while(1)
    {

        if ((button =  button_getButton()) > 0 ){
            switch(button){
            case 1:
                count = servo_move(count + (1*dir));
                break;

            case 2:
                count = servo_move(count + (5*dir));
                break;

            case 3:
                dir *= -1;
                timer_waitMillis(200);
                break;

            case 4:
                if (dir > 0) count = servo_move(180);
                else count = servo_move(0);
                break;

            default:
                break;
            }
        }

        char* dir_string = (dir > 0) ? "<=" : ">=";
        int bass = 3200000 - 16000 - (count /(float) 180) * 16000;
        lcd_printf("pin %d pressed \nThe match is %d. \nDirection : %s \n", button, bass, dir_string);
    }


   return 0;
}

