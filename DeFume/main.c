/*
 * File:   main.c
 * Author: g5pw
 *
 * Created on February 27, 2011, 4:13 PM
 */
#include <pic.h>	// Required to interface with delay routines

#define _XTAL_FREQ 32000000

#define DEBOUNCE_DELAY 10
#define THRESHOLD_DELAY 20
#define REPEAT_DELAY 5

//Setting configuration bits
__CONFIG(FOSC_INTOSC & WDTE_OFF & PWRTE_ON & MCLRE_OFF); //Config word 1
__CONFIG(PLLEN_ON & LVP_OFF);

const char cmd_on = 0b10000001;

void pwm_init(unsigned char period) {
    CCP1SEL = 1; //CCP1 on RA5;
    PR2 = period; // Select 31250Hz PWM period
    TRISA |= 0x20; // Everything is input (tri/stated)
    CCP1CON = 0x0C; // Select PWM single output
}

void pwm_start(int pulse_width) {
    CCP1CON |= (pulse_width & 3) << 4;
    CCPR1L = pulse_width >> 2; //Setting duty cycle: MSByte
    TMR2IF = 0; // Tlear Timer 2 interrupt flag
    T2CON = 0x04; // TMR2 Prescaler:1, Enable timer 2
    while (TMR2IF = 0); // Wait until timer2=PS2 (TMR2IF=1)
    TMR2IF = 0; // Clear Timer2 interrupt flag
    TRISA &= 0xDF; //Enable RA5 as output
}

void pwm_set(int pulse_width) {
    CCP1CON |= (pulse_width & 3) << 4;
    CCPR1L = pulse_width >> 2; //Setting duty cycle: MSByte
}

// Prevent overflows
void pwm_dec() {
    if (CCPR1L < 0x2) {
        CCPR1L = 0;
    } else {
        CCPR1L -= 1;
    }
}

// Prevent overflows
void pwm_inc() {
    if (CCPR1L > 0xFD) {
        CCPR1L = 0xFF;
    } else {
        CCPR1L += 1;
    }
}

int main(void) {
    OSCCON = 0xF0;  // Configure 32 MHz clock

    TRISA = 0x0F;   // RA0-3 are inputs
    ANSELA = 0x00;  // Inputs are NOT analog
    nWPUEN = 0;     // Global pullups enabled
    WPUA = 0x0F;    // Pullup resistors on pins RA0-3
    IOCAN = 0x0F;   // Enable interrupts on change
    IOCAP = 0x08;
    IOCIE = 1;      // Global IOC enable
    GIE = 1;        // Enable global interrupts

    pwm_init(0xFF);
    pwm_start(512);

    while (1) {     // Main loop

    }
}

// TODO Test repetition
interrupt void isr() {
    if (IOCIF) {
        __delay_ms(DEBOUNCE_DELAY); // Debounce delay

        if (IOCAF0) { // On/Off Button pressed
            TRISA5 = ~TRISA5;
            IOCAF0 = 0;
        }
        if (IOCAF1) { // Plus button pressed
            pwm_inc();
            __delay_ms(THRESHOLD_DELAY);
            while (RA1 == 0) { // Repeat action until user depresses key
                pwm_inc();
                __delay_ms(REPEAT_DELAY);
            }
            IOCAF1 = 0;
        }
        if (IOCAF2) { // Minus Button pressed
            pwm_dec();
            __delay_ms(THRESHOLD_DELAY);
            while (RA2 == 0) { // Repeat action until user depresses key
                pwm_dec();
                __delay_ms(REPEAT_DELAY);
            }
            IOCAF2 = 0;
        }
        if (IOCAF3) { // Got serial data from IR decoder
            // code to detect serial data
        }
        IOCIF = 0;
    }
}