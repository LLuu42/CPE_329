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

//FREQUENCIES
#define FIVE 0x03C5
#define FOUR 0x04B5
#define THREE 0x0649
#define TWO 0x0960
#define ONE 0x12E0

void Drive_DAC(unsigned int level);
void initTimer();
void initDac();
void keyCommand(unsigned char key);
void initialize_hardware();

volatile unsigned int TempDAC_Value = 2048;
int index = 0;
//the delay that will be set for each freq
static int timerDelay = ONE;

// gbl variables for Duty Cycle and Freq
unsigned char dutyCycle = 5;
unsigned char freq = 1;
unsigned char mode = 9;

int sawtooth[50] = {66, 132, 198, 264, 330, 396, 462, 528, 594, 660,
                    726, 792, 858, 924, 990, 1056, 1122, 1188, 1254,
                    1320, 1386, 1452, 1518, 1584, 1650, 1716, 1782,
                    1848, 1914, 1980, 2046, 2112, 2178, 2244, 2310,
                    2376, 2442, 2508, 2574, 2640, 2706, 2772, 2838,
                    2904, 2970, 3036, 3102, 3168, 3234, 3300
};

int sinewave_offset[50] = {88, 176, 264, 352, 440, 528, 616, 704,
                           792, 880, 968, 1056, 1144, 1232, 1320,
                           1408, 1496, 1584, 1672, 1760, 1848, 1936,
                           2024, 2112, 2200, 2288, 2376, 2464, 2552,
                           2640, 2728, 2816, 2904, 2992, 3080, 3168,
                           3256, 3344, 3432, 3520, 3608, 3696, 3784,
                           3872, 3960, 4048, 4136, 4224, 4312, 4400
};

int main(void)
{
    unsigned char key = 0;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;   // Stop watchdog timer

    // DCO = 24 MHz, SMCLK and MCLK = DCO
    CS->KEY = CS_KEY_VAL;
    CS->CTL0 = 0;
    CS->CTL0 = CS_CTL0_DCORSEL_4;   // DCO = 24 MHz
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;

    initialize_hardware();

    print_LCD(mode, dutyCycle, freq);

    initDac();
    initTimer();
    P6->DIR |= BIT0;
    P6->OUT &= 0;
    while (1)
    {
        while(key == 0)
        {
            delay_ms(5/freq);
            key = keypad_getkey();
        }
        delay_ms(10);
        keyCommand(key);
        print_LCD(mode, dutyCycle, freq);
        key = 0;
    }

}


void initTimer()
{
    // DCO = 24 MHz, SMCLK and MCLK = DCO
    CS->KEY = CS_KEY_VAL;
    CS->CTL0 = 0;
    CS->CTL0 = CS_CTL0_DCORSEL_4;   // DCO = 24 MHz
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;

    // Configure port bits for SPI
    P4->DIR |= BIT1;                     // Will use BIT4 to activate /CE on the DAC
    P1SEL0 |= BIT6 + BIT5;               // Configure P1.6 and P1.5 for UCB0SIMO and UCB0CLK
    P1SEL1 &= ~(BIT6 + BIT5);            //

    // SPI Setup
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;   // Put eUSCI state machine in reset

    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST |   // Remain eUSCI state machine in reset
                      EUSCI_B_CTLW0_MST   |   // Set as SPI master
                      EUSCI_B_CTLW0_SYNC  |   // Set as synchronous mode
                      EUSCI_B_CTLW0_CKPL  |   // Set clock polarity high
                      EUSCI_B_CTLW0_MSB;      // MSB first

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK; // SMCLK
    EUSCI_B0->BRW = 0x01;                         // divide by 16, clock = fBRCLK/(UCBRx)
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;      // Initialize USCI state machine, SPI
                                                  // now waiting for something to
                                                  // be placed in TXBUF

    EUSCI_B0->IFG |= EUSCI_B_IFG_TXIFG;  // Clear TXIFG flag

    /* Enable interrupts */
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
    TIMER_A0->CCR[0] = 0xEC23;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
            TIMER_A_CTL_MC__CONTINUOUS;
     SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Enable sleep on exit from ISR
     // Enable global interrupt
    __enable_irq();

    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);

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
}

void keyCommand(unsigned char key)
{
    switch(key)
    {
        case 1:
            // set 100Hz
            timerDelay = ONE;
            freq = 1;
            break;
        case 2:
            // set 200Hz
            timerDelay = TWO;
            freq = 2;
            break;
        case 3:
            // set 300Hz
            timerDelay = THREE;
            freq = 3;
            break;
        case 4:
            // set 400Hz
            timerDelay = FOUR;
            freq = 4;
            break;
        case 5:
            // set 500Hz
            timerDelay = FIVE;
            freq = 5;
            break;
        case 7:
            // set Square wave
            mode = 7;
            break;
        case 8:
            // set Sine wave
            mode = 8;
            break;
        case 9:
            // set Sawtooth wave
            mode = 9;
            break;
        case 10:
            // decrease duty cycle by 10%
            if(dutyCycle != 1)
                dutyCycle -= 1;
            break;
        case 11:
            // reset to 50% duty cycle
            dutyCycle = 5;
            break;
        case 12:
            // increase duty cycle by 10%
            if(dutyCycle != 9)
                dutyCycle += 1;
            break;
        default:
            // default case do nothing
            break;
    }
}

void TA0_0_IRQHandler(void)
{
    int temp_limit;
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  // reset interrupt
    index %= 50;    //loops back to 0

    //if we have a sawtooth wave
    if (mode == 9) {
        TempDAC_Value = sawtooth[index++];
    }

        //square wave
    else if (mode == 7) {
        //set the limit for high or low based off of the duty cycle
        //insert duty cycle variable here
        temp_limit = 50 * (dutyCycle * .1);
        if (index++ < temp_limit) {
            //5 V
            TempDAC_Value = 3300;
        }
        else {
            TempDAC_Value = 0;
        }
    }

    //sine wave
    else { //sine wave
        //use lookup table
        TempDAC_Value = sinewave_offset[index++];
    }

    //output to DAC
    Drive_DAC(TempDAC_Value);
    TIMER_A0->CCR[0] += timerDelay;
}
// initialize DAC
void initDac()
{
    // Configure port bits for SPI
      P1->DIR |= BIT7;                     // Will use BIT7 to activate /CE on the DAC
      P1SEL0 |= BIT6 + BIT5;               // Configure P1.6 and P1.5 for UCB0SIMO and UCB0CLK
      P1SEL1 &= ~(BIT6 + BIT5);            //

      // SPI Setup
      EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;   // Put eUSCI state machine in reset

      EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST |   // Remain eUSCI state machine in reset
                        EUSCI_B_CTLW0_MST   |   // Set as SPI master
                        EUSCI_B_CTLW0_SYNC  |   // Set as synchronous mode
                        EUSCI_B_CTLW0_CKPL  |   // Set clock polarity high
                        EUSCI_B_CTLW0_MSB;      // MSB first

      EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK; // SMCLK
      EUSCI_B0->BRW = 0x01;                         // divide by 16, clock = fBRCLK/(UCBRx)
      EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;      // Initialize USCI state machine, SPI
                                                    // now waiting for something to
                                                    // be placed in TXBUF

      EUSCI_B0->IFG |= EUSCI_B_IFG_TXIFG;  // Clear TXIFG flag
}

void Drive_DAC(unsigned int level){
  unsigned int DAC_Word = 0;
  int i;

  DAC_Word = (0x1000) | (level & 0x0FFF);   // 0x1000 sets DAC for Write
                                            // to DAC, Gain = 2, /SHDN = 1
                                            // and put 12-bit level value
                                            // in low 12 bits.

  P4->OUT &= ~BIT1;                                   // Clear P4.1 (drive /CS low on DAC)
                                                      // Using a port output to do this for now

  EUSCI_B0->TXBUF = (unsigned char) (DAC_Word >> 8);  // Shift upper byte of DAC_Word
                                                      // 8-bits to right

  while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));              // USCI_A0 TX buffer ready?

  EUSCI_B0->TXBUF = (unsigned char) (DAC_Word & 0x00FF);     // Transmit lower byte to DAC

  while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));      // Poll the TX flag to wait for completion

  for(i = 200; i > 0; i--);                          // Delay 200 16 MHz SMCLK periods
                                                     //to ensure TX is complete by SIMO

  P4->OUT |= BIT1;                                   // Set P4.1   (drive /CS high on DAC)

  return;
}
