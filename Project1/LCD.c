#include "LCD.h"

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

void write_hello_world()
{
    display_char(0x81, 0x48); // H
    display_char(0x82, 0x45); // E
    display_char(0x83, 0x4C); // L
    display_char(0x84, 0x4C); // L
    display_char(0x85, 0x4F); // O
    display_char(0x86, 0x2C); // ,
    display_char(0x87, 0x57); // W
    display_char(0x88, 0x4F); // O
    display_char(0x89, 0x52); // R
    display_char(0x8A, 0x4C); // L
    display_char(0x8B, 0x44); // D
    display_char(0x8C, 0x21); // !
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

void display_char(int addr, int c)
{
    send_command(addr);         //This sends the letter to address 0x01
    delay_ms(0.037);
    write_char_LCD(c);   //This displays the letter H
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

