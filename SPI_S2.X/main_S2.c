/*
 * File:   main_S2.c
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

int CONT;
int FLAG_B;
uint8_t VAL_READ;

void __interrupt() isr(void){
   if(SSPIF == 1 && FLAG_B == 0)
   {
        VAL_READ = spiRead();
        spiWrite('A');
        __delay_ms(1);
        VAL_READ = spiRead();
        spiWrite(ADRESH);
        
        SSPIF = 0;
    }
   
   if(SSPIF == 1 && FLAG_B == 1){
        VAL_READ = spiRead();
        spiWrite('C');
        __delay_ms(1);
        VAL_READ = spiRead();
        spiWrite(CONT);
        
        FLAG_B = 0;
        SSPIF = 0;
    }
   
   if (INTCONbits.RBIF == 1) // Interrupción del PORTB
    {
        if(!PORTBbits.RB0)
        {
            __delay_ms(20); //Esperar 20ms
            if(!PORTBbits.RB0) //Si el botón sigue presionado después de 20ms
            {
                CONT ++;           //Incrementamos en 1 el valor del PORTD
                FLAG_B = 1;
            }
        }
        else if(!PORTBbits.RB1)
        {
            __delay_ms(20); //Esperar 20ms
            if(!PORTBbits.RB1) //Si el botón sigue presionado después de 20ms
            {
                CONT --;           //Decrementamos en 1 el valor del PORTD   
                FLAG_B = 1;
            }
        }
        INTCONbits.RBIF = 0; // Bajamos la bandera de interrupción del PORTB
    }
}

void main(void) {
    
    setup();

    while(1)
    {
       readADC(0);
       PORTD = CONT;
    }
    return;
}

//******************************************************************************
// Función para configurar GPIOs
//******************************************************************************
void setup (void){
    
    ANSEL = 0;
    ANSELH = 0;

    TRISB = 0b00000011;     //Configuración del PORTB como input
    TRISD = 0;              //Configuración del PORTD como output

    PORTB = 0;              //Limpiamos el PORTB
    PORTD = 0;              //Limpiamos el PORTD
    
    OPTION_REGbits.nRBPU = 0;   //Habilitamos los pull-ups del PORTB
    WPUBbits.WPUB0 = 1;         //Habilitamos el pull-up del RB0
    WPUBbits.WPUB1 = 1;         //Habilitamos el pull-up del RB1
    IOCB = 0b00000011;

    // Configuración de interrupciones
    INTCONbits.GIE = 1; // Habilitamos interrupciones globales
    INTCONbits.PEIE = 1; // Habilitamos interrupciones periféricas
    INTCONbits.RBIE = 1;    //Habilitamos las interrupciones del PORTB (RBIE)
    INTCONbits.RBIF = 0;    //Bajamos la bandera de interrupción del PORTB (RBIF)
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    
    TRISAbits.TRISA5 = 1;       // Slave Select

    setupINTOSC(FOSC_4MHZ);
    setupADC(0);
    
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
}
