#include "config.h"

#ifndef PWM_BUZZER_H
#define PWM_BUZZER_H

void setupPWM(char period, char dc) {
    // Set up for PWM begins
    BUZZER_PIN_DIR = 1; // Set, disables CCP3 output driver
    
    PR2 = period; // higher period, lower frequency
    
    // configure to 8'b00001100, PWM mode
    CCP3CON = 0x0C; // bits 4 and 5 are the LSB of the duty cycle, 2'b00
    
    // higher dc, higher volume
    CCPR3L = dc; // higher bits of duty cycle, 8'b10000000, DC should be ~0.5
    
    CCPTMRS = 0x00; // sets CT3SEL to 2'b00, uses Timer 2 for PWM
    
    PIR1bits.TMR2IF = 0; // Clear Timer 2 interrupt flag
    
    T2CON = 0x02; // Configure Timer 2 prescale value, 8'b00000010
    
    T2CONbits.TMR2ON = 1; // Enable timer 2
    
    while(!PIR1bits.TMR2IF); // wait for timer 2 to overflow
    // Setup complete
}

void startPWM() {
    BUZZER_PIN_DIR = 0; // clear, enables CCP3 output driver
}

void stopPWM() {
    BUZZER_PIN_DIR = 1; // set, disables CCP3 output driver
}

// higher period, lower frequency
void changePeriodPWM(char period) {
    PR2 = period;
}

// higher duty cycle, higher volume
void changeDutyCyclePWM(char dc) {
    CCPR3L = dc;
}

#endif