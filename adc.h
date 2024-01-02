#include <math.h>
#include "config.h"

#ifndef ADC_H
#define ADC_H

// uses AN7 (pin RC3)
void setupADC() {
    // Beginning analog-to-digital conversion process
    POT = 1; // Disables output driver
    POT_ANALOG = 1; // Configure as analog
    
    ADCON1bits.ADFM = 1; // Conversion result is right justified
    
    ADCON1bits.ADCS = 0b100; // Uses Fosc/4 = 1 MHz => 1us period
    
    ADCON1bits.ADPREF = 0b00; // Sets Vref+ as Vdd
    
    ADCON1bits.ADNREF = 0b0; // Sets Vref- as Vss
    
    ADCON0bits.CHS = 0b00111; // Selects AN7 (RC3) as the input channel
    
    ADCON0bits.ADON = 1; // Conversion is enabled
    
    // Wait required acquisition time = amp settling time + hold cap charge time + temp coefficient (in us)
    unsigned int acq_time = 2 + ( -10 * pow(10.0, -12.0) ) * (1000 + 7000 + POT_RESISTANCE) * log(0.001957) * pow(10, 6) + ( (ROOM_TEMP - 25) * 0.05 );
    
    __delay_us(acq_time);
}

// starts analog to digital conversion cycle
void startADC() {
    // Starting conversion
    ADCON0bits.ADGO = 1; // Starts AD conversion cycle
}

// checks if conversion is complete, returns 1 if complete, otherwise 0
unsigned char checkADC() {
    unsigned char check;
    check = ADCON0bits.ADGO;
    
    if(check == 0) {
        return 1; // conversion complete
    }
    else {
        return 0; // conversion in progress
    }
}

unsigned int getResultADC() {
    return (ADRESH << 8) + ADRESL;
}

#endif