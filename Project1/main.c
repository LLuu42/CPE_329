/*
 * main.c
 * Author: Lara Luu
 * Created on: Oct 2, 2017
 * Maps a 3x4 keyboard to an LCD screen on the MSP432
 */


#include "msp.h"
#include "Keypad.h"
#include "LCD.h"

int main(void) {
    unsigned char key;

    P4->DIR |= (RS | RW | E | DB3 | DB2 | DB1 | DB0);  //initialization

    //start the startup sequence
    startup();


    keypad_init();
    LED_init();

    while(1) {
        key = keypad_getkey();  /* read the keypad */

        switch(key){
            case 1:
                display_char(0x81, 0x31);
                break;
            case 2:
                display_char(0x81, 0x32);
                break;
            case 3:
                display_char(0x81, 0x33);
                break;
            case 4:
                display_char(0x81, 0x34);
                break;
            case 5:
                display_char(0x81, 0x35);
                break;
            case 6:
                display_char(0x81, 0x36);
                break;
            case 7:
                display_char(0x81, 0x37);
                break;
            case 8:
                display_char(0x81, 0x38);
                break;
            case 9:
                display_char(0x81, 0x39);
                break;
            case 10:
                display_char(0x81, 0x2A);
                break;
            case 11:
                display_char(0x81, 0x30);
                break;
            case 12:
                display_char(0x81, 0x23);
                break;
        }

        LED_set(key);           /* set LEDs according to the key code */
    }
}
