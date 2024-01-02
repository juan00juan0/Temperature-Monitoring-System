#ifndef TIMER1_H
#define TIMER1_H

// Resets and configures Timer 1 to count micro seconds
void Timer1_Init() {
    T1CONbits.TMR1ON = 0; // Ensure the timer is stopped to write to it
    TMR1H = 0x00;
    TMR1L = 0x00;
    T1CONbits.TMR1CS = 0; // Choose the clock source (0 = Fosc/4)
    T1CONbits.T1CKPS = 0; // Set prescaler (0 = 1:1, for 1 MHz it's not needed)
}

// start timer and return current time
unsigned int Timer1_Start() {
    unsigned int current_time;
    current_time = (TMR1H << 8) + TMR1L;
    T1CONbits.TMR1ON = 1;
    return current_time;
}

// Turns Timer 1 off, clears TMR1 registers, and returns time before reset
unsigned int Timer1_Reset() {
    unsigned int current_time = (TMR1H << 8) + TMR1L;
    T1CONbits.TMR1ON = 0;
    TMR1H = 0x00;
    TMR1L = 0x00;
    return current_time;
}

// Turns Timer 1 off
unsigned int Timer1_Stop() {
    T1CONbits.TMR1ON = 0;
    unsigned int current_time = (TMR1H << 8) + TMR1L;
    return current_time;
}

#endif