/*
 * Keypad.h
 * Author: Lara Luu
 * Created on: Oct 2, 2017
 */



#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "msp.h"

void delay(void);
void keypad_init(void);
char keypad_getkey(void);
void LED_init(void);
void LED_set(int value);

#define KEYPAD P5
#define ROWS 0x17
#define COLUMNS 0xE0
#define COL1 0xC0
#define COL2 0xA0
#define COL3 0x60

#endif /* KEYPAD_H_ */
