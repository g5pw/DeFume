/*
 * File:   main.c
 * Author: g5pw
 *
 * Created on February 27, 2011, 4:13 PM
 */


#include <pic.h>	// Required to interface with delay routines

//Setting configuration bits
__CONFIG(FOSC_INTOSC & WDTE_OFF & PWRTE_ON & MCLRE_OFF); //Config word 1
__CONFIG(PLLEN_ON);


int main(void) {
    OSCCON = 0b11110000;    // Configure 32 MHz clock

    CCP1SEL = 1;            //CCP1 on RA5;
    TRISA = 0x3F;           // Everything is input (tri/stated)
    PR2 = 0xFF;             // Select 31250Hz PWM period
    CCP1CON = 0x0C;         // Select PWM single output
    CCP1CON |= 0x30;        //Setting duty cycle: LSBits
    CCPR1L = 0x0;           //Setting duty cycle: MSByte
    TMR2IF = 0;             // Tlear Timer 2 interrupt flag
    T2CON = 0x04;           // TMR2 Prescaler:1, Enable timer 2
    while(TMR2IF = 0);      // Wait until timer2=PS2 (TMR2IF=1)
    TMR2IF = 0;             // Clear Timer2 interrupt flag
    TRISA |= 0x20;          //Enable RA5 as output



    while (1) { // Main loop
        
    }
}

interrupt isr(){

}