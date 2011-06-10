/*
 * File:   main.c
 * Author: g5pw
 *
 * Created on February 27, 2011, 4:13 PM
 */


#include <pic.h>	// Required to interface with delay routines

#ifndef _XTAL_FREQ
// Unless already defined assume 4MHz system frequency
// This definition is required to calibrate __delay_us() and __delay_ms()
#define _XTAL_FREQ 4000000
#endif

//Setting configuration bits
__CONFIG(INTIO & WDTDIS & PWRTEN & MCLRDIS & BOREN);

#define IPS (_XTAL_FREQ/4) //Hz

#define PWM_FREQ 20000 //Hz

#define PWM_INSTR (IPS/PWM_FREQ)

#define PWM_HALF (PWM_INSTR/2)


int main(void) {
    TRISIO = 0x00; // GPIO 0,1 and 2 configured as input, everything else as output
    GPIO = 0x0F;

    while (1) { // Main loop

    }
}

interrupt isr(){

}