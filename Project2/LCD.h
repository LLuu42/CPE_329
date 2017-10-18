#ifndef LCD_H_
#define LCD_H_
#include "msp432.h"

#define RS BIT7
#define RW BIT6
#define E BIT5
#define DB3 BIT3
#define DB2 BIT2
#define DB1 BIT1
#define DB0 BIT0

#define function_set 0x28
#define display_set 0x0F
#define display_clear 0x01
//#define set_DDRAM 0x82
#define cursor_shift 0x10

void delay_ms(int n);
void send_fs_cmd(int cmd);
void send_command(int cmd);
void write_char_LCD(int cmd);

void write_upper_nibble(int cmd);
void write_lower_nibble(int cmd);

void display_char(int c, int addr);
void scroll_cursor();
void startup();

void print_LCD(unsigned char mode, unsigned char dutyCycle, unsigned char freq);

void print_mode(unsigned char mode);
void print_duty_cycle(unsigned char dutyCycle);
void print_freq(unsigned char freq);

void write_sine();
void write_sawtooth();
void write_square();

void print_num(int addr, unsigned char dutyCycle);

#endif /* LCD_H_ */
