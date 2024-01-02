#ifndef CONFIG_H
#define CONFIG_H

//Configuration Settings
// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

#define I2C_SLAVE 0x27	/* was 1E Channel of i2c slave depends on soldering on back of board*/
#define _XTAL_FREQ 4000000.0    /*for 4mhz*/

#define BUZZER LATAbits.LATA2
#define BUZZER_PIN_DIR TRISAbits.TRISA2

#define POT TRISCbits.TRISC3
#define POT_ANALOG ANSELCbits.ANSC3
#define POT_RESISTANCE 10000

#define ROOM_TEMP 21.5 // In Celsius

#define ALPHA 0.5f // smoothing factor of filter

#define BUTTON_DIR TRISBbits.TRISB5
#define BUTTON PORTBbits.RB5

#endif