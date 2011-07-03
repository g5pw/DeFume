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
__CONFIG(PLLEN_ON & LVP_OFF);

volatile char oldPORTA = 0x0F;

void pwm_init(unsigned char period){
    CCP1SEL = 1;            //CCP1 on RA5;
    PR2 = period;           // Select 31250Hz PWM period
    TRISA |= 0x20;           // Everything is input (tri/stated)
    CCP1CON = 0x0C;         // Select PWM single output
}

void pwm_start(int pulse_width){
    CCP1CON |= (pulse_width & 3) << 4;
    CCPR1L = pulse_width >> 2;           //Setting duty cycle: MSByte
    TMR2IF = 0;             // Tlear Timer 2 interrupt flag
    T2CON = 0x04;           // TMR2 Prescaler:1, Enable timer 2
    while(TMR2IF = 0);      // Wait until timer2=PS2 (TMR2IF=1)
    TMR2IF = 0;             // Clear Timer2 interrupt flag
    TRISA &= 0xDF;          //Enable RA5 as output
}

void pwm_set(int pulse_width){
    CCP1CON |= (pulse_width & 3) << 4;
    CCPR1L = pulse_width >> 2;           //Setting duty cycle: MSByte
}

int main(void) {
    OSCCON = 0xF0;    // Configure 32 MHz clock
    
    TRISA = 0x0F;           // Everything is an input
    ANSELA = 0x00;
    nWPUEN = 0;
    WPUA = 0x0F;            // Pullup resistors on all pins except RA5
    IOCAN = 0x0F;           // Enable interrupts on change
    IOCIE = 1;
    GIE = 1;

    pwm_init(0xFF);
    pwm_start(512);
    RA4=1;

    while (1) { // Main loop
        
    }
}

interrupt isr(){
    if(IOCIF){
        if(IOCAF0){
            TRISA5 = ~TRISA5;
            IOCAF0 = 0;
        }
        if(IOCAF1){
            CCPR1L += 4;
            IOCAF1 = 0;
        }
        if(IOCAF2){
            CCPR1L -= 4;
            IOCAF2 = 0;
        }
        __delay_ms(10);
        IOCIF = 0;
    }
}