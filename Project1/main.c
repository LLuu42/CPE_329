/*
 * main.c
 * Author: Lara Luu
 * Created on: Oct 2, 2017
 * Maps a 3x4 keyboard to an LCD screen on the MSP432
 */


#include "msp.h"
#include "Keypad.h"
#include "LCD.h"
#include <stdio.h>
#include <string.h>

int checkPassword(int password[]);
void display_keypad(int addr, unsigned char key);
void reset(int *curPos ,int *resFlag, int password[4]);
void initialize_hardware();

int main(void)
{
    int wrong_attempts = 4;
    int password[4] = {1, 2, 3, 4}; // init tried password
    int reset = 0;
    int cursor = 0;

    initialize_hardware();

    unsigned char key;
    while(1)
    {
        if(reset)
        {
            while(key == 0)
            {
                delay_ms(1);
                key = keypad_getkey();
            }
            if(key == 10)
            {
                reset = 0;
                send_command(display_clear);
                delay_ms(1);
                write_locked_enter_key(); // Display lock screen
                delay_ms(1);
                cursor = 0;
                password[0] = 0;
                password[1] = 0;
                password[2] = 0;
                password[3] = 0;
            }
            key = 0;
        }
        else
        {
            if(cursor >= 4 && 0 == checkPassword(password))
            {
                write_hello_world();
                wrong_attempts = 4;
                reset = 1;
            }

            else if(cursor >= 4){  /*Extra Functionality for printing incorrect message*/
                wrong_attempts--;
                write_incorrect(wrong_attempts);
                if(wrong_attempts <= 0)
                {
                    wrong_attempts = 4;
                }

                reset = 1;

            }
            else{
            while(key == 0){       //Get current keypress
               delay_ms(50);
               key = keypad_getkey();
            }
            delay_ms(100);
            if(key == 10) { //reset
                reset = 1;
            }
            else
            {
                LED_set(key);           // set LEDs according to the key code
                display_keypad(0xCB + cursor, key);
                password[cursor] = key;
                key = 0;
                cursor++;
            }
        }
    }
  }
}

void initialize_hardware()
{

    P4->DIR |= (RS | RW | E | DB3 | DB2 | DB1 | DB0);  //initialization

    //start the startup sequence
    startup();
    delay_ms(10);
    keypad_init();
    delay_ms(10);
    LED_init();
    delay_ms(10);
    write_locked_enter_key(); // Display lock screen
    delay_ms(10);
}

/*
 * Resets the program to factory settings
 */
void reset(int *curPos ,int *resFlag, int password[4])
{
    *resFlag = 0;
    send_command(display_clear);
    delay_ms(5);
    write_locked_enter_key(); // Display lock screen
    delay_ms(5);
    *curPos = 0;
    password[0] = 0;
    password[1] = 0;
    password[2] = 0;
    password[3] = 0;
}


int checkPassword(int password[]){
/*Code to check verify password against header file*/
    int pw[4] = {1,2,3,4};
    int i = 0;
    int check = 0;

    for (i=0; i<4; i++){
        if (pw[i] != password[i])
            check = 1;
    }
    return check;
}

void display_keypad(int addr, unsigned char key)
{

        switch(key){
            case 1: //1
                display_char(addr, 0x31);
                break;
            case 2: //2
                display_char(addr, 0x32);
                break;
            case 3: //3
                display_char(addr, 0x33);
                break;
            case 4: //4
                display_char(addr, 0x34);
                break;
            case 5: //5
                display_char(addr, 0x35);
                break;
            case 6: //6
                display_char(addr, 0x36);
                break;
            case 7: //7
                display_char(addr, 0x37);
                break;
            case 8: //8
                display_char(addr, 0x38);
                break;
            case 9: //9
                display_char(addr, 0x39);
                break;
            case 10: //*
                display_char(addr, 0x2A);
                break;
            case 11: //0
                display_char(addr, 0x30);
                break;
            case 12: //#
                display_char(addr, 0x23);
                break;
        }

}
