/*
 * File:   main.c
 * Author: g5pw
 *
 * Created on February 27, 2011, 4:13 PM
 */


#include <pic.h>	// Required to interface with delay routines

#define _XTAL_FREQ 32000000

//Setting configuration bits
__CONFIG(FOSC_INTOSC & WDTE_OFF & PWRTE_ON & MCLRE_OFF); //Config word 1
__CONFIG(PLLEN_ON);


void pwm_init(unsigned char period){
    CCP1SEL = 1;            //CCP1 on RA5;
    TRISA = 0x3F;           // Everything is input (tri/stated)
    PR2 = period;             // Select 31250Hz PWM period
    CCP1CON = 0x0C;         // Select PWM single output
}

void pwm_start(int pulse_width){
    CCP1CON |= (pulse_width & 3) << 4;
    CCPR1L = pulse_width >> 2;           //Setting duty cycle: MSByte
    TMR2IF = 0;             // Tlear Timer 2 interrupt flag
    T2CON = 0x04;           // TMR2 Prescaler:1, Enable timer 2
    while(TMR2IF = 0);      // Wait until timer2=PS2 (TMR2IF=1)
    TMR2IF = 0;             // Clear Timer2 interrupt flag
    TRISA |= 0x20;          //Enable RA5 as output
}

int main(void) {
    OSCCON = 0b11110000;    // Configure 32 MHz clock

    pwm_init(0xFF);
    pwm_start(512);




    while (1) { // Main loop
        
    }
}

interrupt isr(){

}