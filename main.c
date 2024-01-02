//Header files
#include <xc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include  "i2c.h"
#include  "i2c_LCD.h"

#include <stdint.h>
#include "config.h"
#include "pwm_buzzer.h"
#include "dht11.h"
#include "adc.h"

//LCD Prototypes
void I2C_LCD_Command(unsigned char,unsigned char);
void I2C_LCD_SWrite(unsigned char,unsigned char *, char);
void I2C_LCD_Init(unsigned char);
void I2C_LCD_Pos(unsigned char,unsigned char);
unsigned char I2C_LCD_Busy(unsigned char);

// PWM Prototypes
void setupPWM(char period, char dc);
void startPWM();
void stopPWM();
void changePeriodPWM(char period);
void chaneDutyCyclePWM(char dc);

// DHT11 Prototypes
void startDHT();
unsigned char responseDHT();
unsigned char readDHT();

// ADC Prototypes
void setupADC();
void startADC();
char checkADC();
unsigned int getResultADC();

// POT to TEMP conversion function
unsigned char convertPotValToTemp(unsigned int pot_val) {
    unsigned char temp_val;
    
    temp_val = (char) ( pot_val * (50.0 / 1023.0) );
    
    return temp_val;
}

void main(void) {
    // Program mode
    unsigned char mode = 0; // 0: dht sampling mode, 1: config trigger point
    unsigned char button_mode = 0; // 0: program, 1: set trigger
    
    // Temperature and threshold variables
    unsigned char temperature, trigger = 22;
    
    // Button variables
    unsigned char old_state = 1, new_state = 1;
    unsigned char raw_state_old = 1, raw_state_new = 1;
    unsigned counter = 0;
    
    //DHT data variables and checksum
    unsigned char response, hInt, hDec, tInt, tDec;
    unsigned char sum_last_8bits, checksum;
    
    // LCD variables and initialization
    unsigned char Sout[16];
    unsigned char * Sptr;
    
    //ADC result variables
    unsigned int ad_result, ad_result_new;
    
    //LED RA5 for debugging purposes
    TRISA5 = 0;
    
    
//    TRISC7 = 0;
    ANSC7 = 0;
    
    // Configure internal oscillator to 4 MHz
    OSCCON = 0x68;
    
    // Configure button pin as input
    BUTTON_DIR = 1;
    ANSELBbits.ANSB5 = 0;
    
    // Setup PWM for passive buzzer
    setupPWM(0xFF, 0x02);
    
    // Initialize LCD through I2C
    i2c_Init();
    I2C_LCD_Init(I2C_SLAVE);
    
    // Setup and start ADC cycle
    setupADC();
    startADC();
    
    TRISCbits.TRISC6 = 0;
    RC6 = 1;
    
    __delay_ms(1500);
    
    while(1) {
        
        // get button pin's raw state
//        raw_state_new = BUTTON;
        
        // if raw_states have not changed increment counter
//        if(raw_state_new == raw_state_old) {
//            counter++;
//            
//            if(counter > 50) {
//                counter = 0; // reset counter to prevent overflow
//            }
//        }
//        else {
//            counter = 0; // reset counter if button is bouncing
//        }
        
        // when button is stable, update button's new states
//        if(counter >= 20) {
//            new_state = raw_state_new;
//        }
        
        ///////// DEBUG CODE START
        new_state = BUTTON;
        ///////// DEBUG CODE END
        
        
        // if button is pressed and released, change to trigger point config mode
        if(new_state == 1 && old_state == 0 && button_mode == 0) {
            mode = 1;
            button_mode = 2;
        }
        
        // trigger point program mode
        if(mode == 1) {
            startADC(); // start ADC conversion cycle
            
            while( !checkADC() ); // wait for GO to clear, indicating completion
            
            // raw value
            ad_result_new = getResultADC();
            // filtered value
            ad_result = ALPHA * ad_result_new + (1 - ALPHA) * ad_result;
            
            temperature = convertPotValToTemp(ad_result);
            
            I2C_LCD_Command(I2C_SLAVE, 0x01);
            
            I2C_LCD_Pos(I2C_SLAVE, 0x00);
            sprintf(Sout, "Configure Mode");
            I2C_LCD_SWrite(I2C_SLAVE, Sout, 14);
            
            I2C_LCD_Pos(I2C_SLAVE, 0x40);
            sprintf(Sout, "Trigger: %d C ", temperature);
            I2C_LCD_SWrite(I2C_SLAVE, Sout, 13);
            
            // Button pressed, set new trigger point
            if(new_state == 1 && old_state == 0 && button_mode == 1) {
                trigger = temperature;
                
                // switch back to normal mode
                mode = 0;
                button_mode = 0;
            }
            
            if(button_mode == 2) {
                button_mode = 1;
            }
        }
        else { // read from DHT mode
            startDHT();

            response = responseDHT();

            if(response == 1) { // DHT has sent response
                
                // Green LED turns on when sampling from DHT11
                RA5 = 0;
                hInt = readDHT();
                hDec = readDHT();
                tInt = readDHT();
                tDec = readDHT();
                checksum = readDHT();
                RA5 = 1;

                sum_last_8bits = (hInt + hDec + tInt + tDec) & 0xFF;

                // checksum verification success
                if(checksum == sum_last_8bits) {
                    
                    // temperature passes the trigger point (22 C in this case)
                    if(tInt >= trigger) {
                        I2C_LCD_Command(I2C_SLAVE, 0x01);

                        I2C_LCD_Pos(I2C_SLAVE, 0x00);
                        sprintf(Sout, "Temperature is");
                        I2C_LCD_SWrite(I2C_SLAVE, Sout, 14);

                        I2C_LCD_Pos(I2C_SLAVE, 0x40);
                        sprintf(Sout, "too high!");
                        I2C_LCD_SWrite(I2C_SLAVE, Sout, 9);

                        // generate siren using buzzer
                        startPWM();
                        for(int i = 255; i > 61; i--) {
                            changePeriodPWM(i);
                            __delay_ms(1);
                        }
                        stopPWM();

                    }
                    else {
                        I2C_LCD_Command(I2C_SLAVE, 0x01);

                        I2C_LCD_Pos(I2C_SLAVE, 0x00);
                        sprintf(Sout, "Temp: %d.%d C ", tInt, tDec);
                        I2C_LCD_SWrite(I2C_SLAVE, Sout, 13);

                        I2C_LCD_Pos(I2C_SLAVE, 0x40);
                        sprintf(Sout, "Humidity: %d.%d %%", hInt, hDec);
                        I2C_LCD_SWrite(I2C_SLAVE, Sout, 16);
                    }

                } else { // checksum verification fails
                    I2C_LCD_Command(I2C_SLAVE, 0x01);

                    I2C_LCD_Pos(I2C_SLAVE, 0x40);
                    sprintf(Sout, "checksum error");
                    I2C_LCD_SWrite(I2C_SLAVE, Sout, 14);
                }

            } else { // no response from DHT
                I2C_LCD_Command(I2C_SLAVE, 0x01);

                I2C_LCD_Pos(I2C_SLAVE, 0x00);
                sprintf(Sout, "Error!");
                I2C_LCD_SWrite(I2C_SLAVE, Sout, 6);

                I2C_LCD_Pos(I2C_SLAVE, 0x40);
                sprintf(Sout, "No response!");
                I2C_LCD_SWrite(I2C_SLAVE, Sout, 12);

                __delay_ms(1000);
            }
        }
        
//        raw_state_old = raw_state_new; // update raw old states
        old_state = new_state; // update button old state
        
        __delay_ms(500);
    }
    
    
    return;
}
