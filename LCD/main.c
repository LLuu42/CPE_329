//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"

#define RS BIT7
#define RW BIT6
#define E BIT5
#define DB3 BIT3 // D7
#define DB2 BIT2 // D6
#define DB1 BIT1 //D5
#define DB0 BIT0 // D4


void delayMs(int n) {
    int i, j;
    for (j = 0; j < n; j++)
        for (i = 2400; i > 0; i--);      /* Delay */
}

//sends two nibbles (high and low)
void set_function(int CMD)
{
    //high nibble
    P4-> OUT &= ~(RS + RW + E + DB3 + DB2 + DB1 + DB0);
    delayMs(.001);
    P4 -> OUT |= E; // sets enable to high (before doing anything)
    P4 -> OUT |= ((CMD >>4) & 0x0F); // shifts to low nibble, masks everything else to zero
    delayMs(.001);
    P4-> OUT &= ~E; //turns off E to be safe
    delayMs(.001);

    P4-> OUT &= ~(RS + RW + E + DB3 + DB2 + DB1 + DB0);
    delayMs(.001);
    P4 -> OUT |= E; // sets enable to high (before doing anything)
    P4 -> OUT |= ((CMD >>4) & 0x0F); // shifts to low nibble, masks everything else to zero
    delayMs(.001);
    P4-> OUT &= ~E; //turns off E to be safe
    delayMs(.001);



    // low nibble
    P4-> OUT &= ~(RS + RW + E + DB3 + DB2 + DB1 + DB0);
    delayMs(.001);
    P4-> OUT |= E;
    P4-> OUT |= CMD & (0x0F);
    delayMs(.001);
    P4-> OUT & ~E;
    delayMs(.001);
}


//one high, one low
//clear display and on/off
void send_command(int CMD)
{
    //high nibble
    P4-> OUT &= ~(RS + RW + E + DB3 + DB2 + DB1 + DB0);
    delayMs(.001);
    P4 -> OUT |= E; // sets enable to high (before doing anything)
    P4 -> OUT |= ((CMD >>4) & 0x0F); // shifts to low nibble, masks everything else to zero
    delayMs(.001);
    P4-> OUT &= ~E; //turns off E to be safe
    delayMs(.001);

    // low nibble
    P4-> OUT &= ~(RS + RW + E + DB3 + DB2 + DB1 + DB0);
    delayMs(.001);
    P4-> OUT |= E;
    P4-> OUT |= CMD & (DB3 + DB2 + DB1 + DB0);
    delayMs(.001);
    P4-> OUT & ~E;
    delayMs(.001);

}

void write_data(int CMD)
{
    //high nibble
    P4-> OUT &= ~(RS + RW + E + DB3 + DB2 + DB1 + DB0);
    delayMs(.001);
    P4 -> OUT |= RS; // Datasheet wants RS to be high
    P4 -> OUT |= E; // sets enable to high (before doing anything)
    P4 -> OUT |= ((CMD >>4) & 0x0F); // shifts to low nibble, masks everything else to zero
    delayMs(.001);
    P4-> OUT &= ~E; //turns off E to be safe
    delayMs(.001);

    // low nibble
    P4-> OUT &= ~(RS + RW + E + DB3 + DB2 + DB1 + DB0);
    delayMs(.001);
    P4 -> OUT |= RS; // Datasheet wants RS to be high
    P4-> OUT |= E;
    P4-> OUT |= CMD & (0x0F);
    delayMs(.001);
    P4-> OUT & ~E;
    delayMs(.001);
}

void main(void)
{
	
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
    delayMs(.001);
    P4->DIR |= (RS|RW|E|DB3|DB2|DB1|DB0);         /* make P4 pins output for data */
    delayMs(20);

    set_function(0x28); //func set
    delayMs(.001);
    send_command(0x0C); //clear
    delayMs(.001);
    send_command(0x01); //display
    delayMs(.001);

    //write a letter
    send_command(0xC1);
    delayMs(.001);
    write_data(0x42);
    delayMs(.001);

    //write another letter
    send_command(0xC2);
    delayMs(.001);

    write_data(0x43);
    delayMs(.001);



}


