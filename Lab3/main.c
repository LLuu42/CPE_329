#include "msp.h"

#define RS 0x20     /* P3.5 mask */
#define RW 0x40     /* P3.6 mask */
#define EN 0x80     /* P3.7 mask */

void delayMs(int n);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);
void LCD_clear(void);
void LCD_setCursorHome(void);

int main(void) {
    LCD_init();
    LCD_command(1);     /* clear display */
    delayMs(500);
    LCD_command(0x80);  /* set cursor at beginning of first line */
    LCD_data('H');      /* write the word "Hello" */
    LCD_data('e');
    LCD_data('l');
    LCD_data('l');
    LCD_data('o');
    LCD_data(' ');
    LCD_data('W');
    LCD_data('o');
    LCD_data('r');
    LCD_data('l');
    LCD_data('d');
    delayMs(5000);
    LCD_setCursorHome();
    while(1)
    {
       delayMs(500);
    }
}

void LCD_init(void) {
    P3->DIR |= RS | RW | EN; /* make P3 pins output for control */
    P4->DIR = 0xFF;         /* make P4 pins output for data */

    delayMs(30);            /* initialization sequence */
    LCD_command(0x30);
    delayMs(10);
    LCD_command(0x30);
    delayMs(1);
    LCD_command(0x30);

    LCD_command(0x38);      /* set 8-bit data, 2-line, 5x7 font */
    LCD_command(0x06);      /* move cursor right after each char */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}

void LCD_command(unsigned char command) {
    P3->OUT &= ~(RS | RW);  /* RS = 0, R/W = 0 */
    P4->OUT = command;      /* put command on data bus */
    P3->OUT |= EN;          /* pulse E high */
    delayMs(0);
    P3->OUT &= ~EN;         /* clear E */
    if (command < 4)
        delayMs(4);         /* command 1 and 2 need up to 1.64ms */
    else
        delayMs(1);         /* all others 40 us */
}

void LCD_data(unsigned char data) {
    P3->OUT |= RS;          /* RS = 1 */
    P3->OUT &= ~RW;         /* R/W = 0 */
    P4->OUT = data;         /* put data on bus */
    P3->OUT |= EN;          /* pulse E */
    delayMs(0);
    P3->OUT &= ~EN;         /* clear E */
    delayMs(1);             /* wait for controller to do the display */
}

void LCD_clear(void)
{
    P3->OUT &= ~(RS | RW);  /* RS = 0, R/W = 0 */
    P4->OUT &= BIT0;      /* put command on data bus */
    P3->OUT |= EN;          /* pulse E high */
    delayMs(0);
    P3->OUT &= ~EN;         /* clear E */
    delayMs(4);         /* command 1 and 2 need up to 1.64ms */
}

void LCD_setCursorHome(void) {
    P3->OUT &= ~(RS | RW);  /* RS = 0, R/W = 0 */
    P4->OUT = BIT7;      /* put command on data bus */
    P3->OUT |= EN;          /* pulse E high */
    delayMs(0);
    P3->OUT &= ~EN;         /* clear E */
    delayMs(1);         /* all others 40 us */
}
/* delay milliseconds when system clock is at 3 MHz */
void delayMs(int n) {
    int i, j;

    for (j = 0; j < n; j++)
        for (i = 250; i > 0; i--);      /* Delay */
}
