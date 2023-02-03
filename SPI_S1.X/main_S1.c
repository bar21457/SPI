/*
 * File:   main_S1.c
 * Author: byron
 *
 * Created on February 2, 2023, 10:18 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT        // Oscillator Selection bits 
                                            // (INTOSC oscillator: I/O function 
                                            // on RA6/OSC2/CLKOUT pin, I/O 
                                            // function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and 
                                // can be enabled by SWDTEN bit of the WDTCON 
                                // register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR 
                                // pin function is digital input, MCLR 
                                // internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code 
                                // protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code 
                                // protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit 
                                // (Internal/External Switchover mode disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
                                // (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin 
                                // has digital I/O, HV on MCLR must be used for 
                                // programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out 
                                // Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 
                                // (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "configINTOSC.h"
#include "configSPI.h"
#include "configADC.h"

#define _XTAL_FREQ 4000000

void setup(void);

void __interrupt() isr(void){
   if(SSPIF == 1){
        PORTD = spiRead();
        spiWrite(ADRESH);
        SSPIF = 0;
    }
}

void main(void) {
    
    setup();

    while(1)
    {
       readADC(0);
    }
    return;
}

//******************************************************************************
// Función para configurar GPIOs
//******************************************************************************
void setup (void){
    
    ANSEL = 0;
    ANSELH = 0;

    TRISB = 0;              //Configuración del PORTB como input
    TRISD = 0;              //Configuración del PORTD como output

    PORTB = 0;              //Limpiamos el PORTB
    PORTD = 0;              //Limpiamos el PORTD

    // Configuración de interrupciones
    INTCONbits.GIE = 1; // Habilitamos interrupciones globales
    INTCONbits.PEIE = 1; // Habilitamos interrupciones periféricas
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    
    TRISAbits.TRISA5 = 1;       // Slave Select

    setupINTOSC(FOSC_4MHZ);
    setupADC(0);
    
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
}
