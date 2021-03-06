/*
 *
 *Lab 1: Blinking LED, Clock Control, and Software Delay on MSP432
 *Created on: 18 September 2017
 *Authors: Sarahanne Heredia, Lara Luu
 *
 */

#include "msp.h"
#include "stdio.h"

#define FREQ_1_5_MHZ 1500000
#define FREQ_3_MHZ 3000000
#define FREQ_6_MHZ 6000000
#define FREQ_12_MHZ 12000000
#define FREQ_24_MHZ 24000000
#define FREQ_48_MHZ 48000000


void delay_ms (int n, int f);
void delay_ns (int n, int f);
void set_DCO(int f);

/*
 *  @main
 *  driver used to test delay_ms and delay_ns accuracies
 *  blinks led on and off continuously
 *  @return void
 */
int main(void) {
    int clk = FREQ_12_MHZ;

    set_DCO(clk);        //**Make sure frequency is consistent with delay arguments**

    P2->SEL1 &= ~BIT1;      // configure P2.1 as simple I/O
    P2->SEL0 &= ~BIT1;
    P2->DIR |= BIT1;        // P2.1 set as output pin

    while (1) {
        P2->OUT |= BIT1;            // turn on P2.1 green LED
        delay_ns(1000, clk); //delays next operation for 1 second
        P2->OUT &= ~BIT1;           // turn off P2.1 green LED
        delay_ns(1000, clk); //delays next operation for 1 second
    }

    return 0;
}




/*
 *  @delay_ms
 *
 *  delay milliseconds when system clock is at frequencies specified
 *  @return void
 */
void delay_ms (int n, int f) {
    int i, j;

    int ms = f / 10000;//= std * (f / FREQ_3_MHZ);

    for (j = 0; j < n; j++)
        for (i = ms; i > 0; i--);  /* Delay 1 ms*/
}

/*
 *  @delay_ns
 *
 *  delay nanoseconds when system clock is at frequencies specified
 *  smallest pulse achieved was 2.4 us, calculations reflect this
 *  @return void
 */
void delay_ns(const int delay, const int freq) {
    int j;
    const int scale = delay*freq/10800; //calculates the scale needed to
    for (j = scale; j; j--); //loop used to delay by nanoseconds
}

/*
 *  @set_DCO
 *
 *  sets the frequencies of the clock based on the argument
 *  frequencies range from 1.5 MHz to 40MHz
 *  keeps clock at default 3Mhz if frequency argument does not exist
 *  @return void
 */

void set_DCO(int f) {
    CS->KEY = CS_KEY_VAL;
    CS->CTL0 = 0;
    if(f == FREQ_1_5_MHZ)               //1.5MHz
        CS->CTL0 = CS_CTL0_DCORSEL_0;
    else if(f == FREQ_3_MHZ)            //3MHz
        CS->CTL0 = CS_CTL0_DCORSEL_1;
    else if(f == FREQ_6_MHZ)            //6MHz
        CS->CTL0 = CS_CTL0_DCORSEL_2;
    else if(f == FREQ_12_MHZ)           //12MHz
        CS->CTL0 = CS_CTL0_DCORSEL_3;
    else if(f == FREQ_24_MHZ)           //24MHz
        CS->CTL0 = CS_CTL0_DCORSEL_4;
    else if(f == FREQ_48_MHZ)           //48MHz
        CS->CTL0 = CS_CTL0_DCORSEL_5;
    else                          //error case: if frequency specified does not exist
        fprintf(stdout, "Frequency specified does not exist, setting to default 3MHz.\n");
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;
}




