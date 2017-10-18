#include "LCD.h"
#include "Keypad.h"

/*
 * Writes to LCD screen the mode, duty cycle, and frequency
 */
void print_LCD(unsigned char mode, unsigned char dutyCycle, unsigned char freq)
{
    send_command(display_clear);
    delay_ms(20);

    print_mode(mode);
    delay_ms(20);
    print_duty_cycle(dutyCycle);
    delay_ms(20);
    print_freq(freq);
    delay_ms(20);
}

/*
 * Writes to LCD screen the mode
 * Options: square, sine, sawtooth
 */
void print_mode(unsigned char mode)
{
    switch(mode) {

    //square wave
    case(7):
        write_square();
            break;

    //sine wave
    case(8):
        write_sine();
        break;

    //sawtooth wave
    case(9):
        write_sawtooth();
        break;
    }
}

/*
 * Helper function to write square wave
 */
void write_square()
{
    display_char(0x81, 0x53); // S
    display_char(0x82, 0x51); // Q
    display_char(0x83, 0x55); // U
    display_char(0x84, 0x41); // A
    display_char(0x85, 0x52); // R
    display_char(0x86, 0x45); // E

}

/*
 * Helper function to write sine wave
 */
void write_sine()
{
    display_char(0x81, 0x53); // S
    display_char(0x82, 0x49); // I
    display_char(0x83, 0x4E); // N
    display_char(0x84, 0x45); // E
}

/*
 * Helper function to write sawtooth
 */
void write_sawtooth()
{
    display_char(0x81, 0x53); // S
    display_char(0x82, 0x41); // A
    display_char(0x83, 0x57); // W
    display_char(0x84, 0x54); // T
    display_char(0x85, 0x48); // H
    display_char(0x86, 0x4F); // O
    display_char(0x87, 0x4F); // O
    display_char(0x88, 0x54); // T
    display_char(0x88, 0x48); // H
}

/*
 * Writes the duty cycle to the LCD screen
 * Options: 10, 20, 30, 40, 50, 60, 70, 80, 90
 */
void print_duty_cycle(unsigned char dutyCycle)
{
    display_char(0xC1, 0x44); // D
    display_char(0xC2, 0x43); // C
    display_char(0xC3, 0x3A); // :

    print_num(0xC4, dutyCycle);

    display_char(0xC5, 0x30); // 0
    display_char(0xC6, 0x25); // %
}

/*
 * Writes the frequency to the LCD screen
 * Options: 100, 200, 300, 400, 500 Hz
 */
void print_freq(unsigned char freq)
{
    display_char(0xC8, 0x46); // F
    display_char(0xC9, 0x3A); // :

    print_num(0xCA, freq);

    display_char(0xCB, 0x30); // 0
    display_char(0xCC, 0x30); // 0
    display_char(0xCD, 0x48); // H
    display_char(0xCE, 0x5A); // Z

}

void print_num(int addr, unsigned char dutyCycle)
{
    switch(dutyCycle) {
        case(1):
            display_char(addr, 0x31); // 1
            break;
        case(2):
            display_char(addr, 0x32); // 2
            break;
        case(3):
            display_char(addr, 0x33); // 3
            break;
        case(4):
             display_char(addr, 0x34); // 4
             break;
        case(5):
            display_char(addr, 0x35); // 5
            break;
        case(6):
            display_char(addr, 0x36); // 6
            break;
        case(7):
            display_char(addr, 0x37); // 7
            break;
        case(8):
            display_char(addr, 0x38); // 8
            break;
        case(9):
            display_char(addr, 0x39); // 9
            break;
    }
}


/*
 * Displays a character at specified address and character
 */
void display_char(int addr, int c)
{
    send_command(addr);
    delay_ms(0.037);
    write_char_LCD(c);
    delay_ms(1);
}

void startup()
{
    delay_ms(20);
    send_fs_cmd(function_set);
    delay_ms(0.037);
    send_command(display_set);
    delay_ms(0.037);
    send_command(display_clear);
    delay_ms(1.60);
}

void send_fs_cmd(int cmd){
    write_upper_nibble(cmd);
    write_upper_nibble(cmd);
    write_lower_nibble(cmd);
}

void send_command(int cmd){
    write_upper_nibble(cmd);
    write_lower_nibble(cmd);
}

//Write to DDRAM
void write_char_LCD(int c){

    P4->OUT &= ~(RS + RW + E + DB3 + DB2 + DB1 + DB0);  //clear all bits except bit 4
    delay_ms(0.001);
    P4->OUT |= RS;                                  //raise RS
    P4->OUT |= E;                           //raise E
    P4->OUT |= ((c >> 4) & 0x0F);                        //upper nibble
    delay_ms(0.001);
    P4->OUT &= ~E;                                           //drop E
    delay_ms(0.001);

    P4->OUT &= ~(RS + RW + E + DB3 + DB2 + DB1 + DB0);  //clear all bits except bit 4
    delay_ms(0.001);
    P4->OUT |= RS;                                  //raise RS
    P4->OUT |= E;                           //raise E
    P4->OUT |= (c & 0x0F);                          //lower nibble
    delay_ms(0.001);
    P4->OUT &= ~E;                                           //drop E
}


void write_upper_nibble(int cmd)
{
    P4->OUT &= ~(RS + RW + E + DB3 + DB2 + DB1 + DB0);  //clear all bits except bit 4
    delay_ms(0.001);
    P4->OUT |= E;                           //raise E
    P4->OUT |= ((cmd >> 4) & 0x0F);                      //upper nibble
    delay_ms(0.001);
    P4->OUT &= ~E;                                          //drop E
    delay_ms(0.001);
}

void write_lower_nibble(int cmd)
{
    P4->OUT &= ~(RS + RW + E + DB3 + DB2 + DB1 + DB0);  //clear all bits except bit 4
    delay_ms(0.001);
    P4->OUT |= E;                           //raise E
    P4->OUT |= (cmd & 0x0F);                             //lower nibble
    delay_ms(0.001);
    P4->OUT &= ~E;                                           //drop E
}

// delay function
void delay_ms(int n) {
        int i, j;
            for (j = 0; j < n; j++)
                for (i = 2400; i > 0; i--);
}

void scroll_cursor()
{
    int h = 0xC1;
    do {
        h++;
        send_command(cursor_shift);
        send_command(h);
        delay_ms(0.037);
        write_char_LCD(0x7E);
        delay_ms(50);

        send_command(h);
        delay_ms(0.037);
        write_char_LCD(0xFE);
        delay_ms(1);

    } while(h != 0xCF);
}

