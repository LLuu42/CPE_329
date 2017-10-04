/*
 * Keypad.c
 * Author: Lara Luu
 * Created on: Oct 2, 2017
 * Creates functions to read from a 3x4 keypad
 * Maps keypad pins to P5
 */


#include "Keypad.h"

/* this function initializes Port 4 that is connected to the keypad.
 * All pins are configured as GPIO input pin. The column pins have
 * the pull-up resistors enabled.
 */
void keypad_init(void) {
    KEYPAD->DIR = 0;
    KEYPAD->REN = COLUMNS;     /* enable pull resistor for column pins */
    KEYPAD->OUT = COLUMNS;     /* make column pins pull-ups */
}

/*
 * This is a non-blocking function to read the keypad.
 * If a key is pressed, it returns a unique code for the key. Otherwise,
 * a zero is returned.
 */
char keypad_getkey(void) {
    int row, col;
    const char row_select[] = {0x01, 0x02, 0x04, 0x10}; /* one row is active */

    /* check to see any key pressed */
    KEYPAD->DIR |= ROWS;            /* make all row pins output */
    KEYPAD->OUT &= ~ROWS;           /* drive all row pins low */
    delay();                    /* wait for signals to settle */
    col = KEYPAD->IN & COLUMNS;        /* read all column pins */
    KEYPAD->OUT |= ROWS;            /* drive all ROWS high before disable them */
    KEYPAD->DIR &= ~ROWS;           /* disable all row pins drive */
    if (col == COLUMNS)            /* if all COLUMNS are high */
        return 0;               /* no key pressed */

    /* If a key is pressed, it gets here to find out which key.
     * It activates one row at a time and read the input to see
     * which column is active. */
    for (row = 0; row < 4; row++) {
        KEYPAD->DIR &= ROWS;                /* disable all ROWS */
        KEYPAD->DIR |= row_select[row];     /* enable one row at a time */
        KEYPAD->OUT &= ~row_select[row];    /* drive the active row low */
        delay();                        /* wait for signal to settle */
        col = KEYPAD->IN & COLUMNS;            /* read all COLUMNS */
        KEYPAD->OUT |= row_select[row];     /* drive the active row high */
        if (col != COLUMNS) break;         /* if one of the input is low, some key is pressed. */
    }
    KEYPAD->OUT |= ROWS;                    /* drive all ROWS high before disable them */
    KEYPAD->DIR &= ROWS;                    /* disable all ROWS */
    if (row == 4)
        return 0;                       /* if we get here, no key is pressed */

    /* gets here when one of the ROWS has key pressed, check which column it is */
    if (col == COL1) return row * 3 + 1;    /* key in column 0 */
    if (col == COL2) return row * 3 + 2;    /* key in column 1 */
    if (col == COL3) return row * 3 + 3;    /* key in column 2 */

    return 0;   /* just to be safe */
}

/* Initialize tri-color LEDs on the LaunchPad board.
 * P2.0 - red
 * P2.1 - green
 * P2.2 - blue
 */
void LED_init(void) {
    P2->DIR = 0x07;         /* make LED pins output */
    P2->OUT &= ~0x07;       /* turn the LEDs off */
}

/* turn on or off the LEDs according to bit 2-0 of the value */
void LED_set(int value) {
    value &= 0x07;          /* only bit 2-0 are affected */
    P2->OUT = (P2->OUT & ~0x07) | value;
}

/* make a small delay */
void delay(void) {
}
