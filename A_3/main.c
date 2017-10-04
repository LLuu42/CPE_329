//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"
#include "delay.h"

#define RS BIT7
#define RW BIT6
#define E BIT5
#define DB3 BIT3
#define DB2 BIT2
#define DB1 BIT1
#define DB0 BIT0

SEND_CMD(INT CMD)
{
    P4 -> &= ~(RS + RW + E + DB3 + DB2 + DB1 + DB0);
    DELAY_us(1); //Be Safe
    P4 -> |= E;
    P4 -> |= ((CMD >> 4 ) & 0x0F);
    DELAY_us(1); // Hold E
    P4 -> &= ~E;
    DELAY_us(1);
    if (CMD == FUNC_SET)
    {
    P4 -> &= ~(RS + RW + E + DB3 + DB2 + DB1 + DB0);
    DELAY_us(1); //Be Safe
    P4 -> |= E;
    P4 -> |= ((CMD >> 4 ) & 0x0F);
    DELAY_us(1); // Hold E
    P4 -> &= ~E;
    DELAY_us(1);
    }

    P4 -> OUT &= ~(RS + RW + E + DB3 + DB2 + DB1 + DB0);
    DELAY_us(1);
    P4 -> OUT 1=E;
    P4 -> OUT 1 = CMD & 0x0F;
    DELAY_us(1);
    P4 -> OUT &= ~E;
    DELAY_us(1);
    P4 -> OUT &= ~(RS + RW + E + DB3 + DB2 + DB1 + DB0);
}

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    P4-> DIR |= RS + RW + E + DB7 + DB6 + DB5 + DB4;
    SEND_CMD(FUNC_SET);
    DELAY_us(37);
    SEND.CMD(0X0F);    //set display
    DELAY_us(37);
    SEND_CMD(0x01);    //clear
    DELAY_ms(2);






}


