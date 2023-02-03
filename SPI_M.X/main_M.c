/*
 * File:   main_M.c
 * Author: byron
 *
 * Created on February 2, 2023, 8:44 PM
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
#include "configLCD_4bits.h"

#define _XTAL_FREQ 4000000

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RB5
#define D6 RD6
#define D7 RD7

void setup(void);

void main(void) {
  
    setup();
    
    uint8_t VAL_READ;
    uint8_t VAL_W;
    
    uint8_t VAL1;
    uint8_t VAL1_U;
    uint8_t VAL1_D;
    uint8_t VAL1_C;
    
    uint8_t VAL2;
    uint8_t VAL2_U;
    uint8_t VAL2_D;
    uint8_t VAL2_C;
    
    uint8_t CONT;
    uint8_t CONT_U;
    uint8_t CONT_D;
    uint8_t CONT_C;

    char ADC1[9];
    char ADC2[9];
    char CONT_S[9];
    
    Lcd_Clear_4bits();
    Lcd_Set_Cursor_4bits(1,1);
    Lcd_Write_String_4bits("S1:");
    Lcd_Set_Cursor_4bits(1,7);
    Lcd_Write_String_4bits("S2:");
    Lcd_Set_Cursor_4bits(1,13);
    Lcd_Write_String_4bits("S3:");

    while(1)
    {   
       PORTCbits.RC1 = 0;       //Slave1 Select
       __delay_ms(1);
      
       spiWrite(VAL_READ);
       VAL1 = spiRead();
       
       __delay_ms(1);
       PORTCbits.RC1 = 1;       //Slave1 Deselect
       
      VAL1_C = (VAL1/100);
      VAL1_D = (VAL1/10)%10;
      VAL1_U = VAL1%10;
       
      sprintf(ADC1, "%u", VAL1_C);
      Lcd_Set_Cursor_4bits(2,1);
      Lcd_Write_String_4bits(ADC1);
      
      sprintf(ADC1, "%u", VAL1_D);
      Lcd_Set_Cursor_4bits(2,2);
      Lcd_Write_String_4bits(ADC1);
      
      sprintf(ADC1, "%u", VAL1_U);
      Lcd_Set_Cursor_4bits(2,3);
      Lcd_Write_String_4bits(ADC1);
      
      PORTCbits.RC2 = 0;       //Slave2 Select
       __delay_ms(1);
      
       spiWrite(VAL_READ);
       VAL_W = spiRead();
       
       if (VAL_W == 'A')
       {
           spiWrite(VAL_READ);
           VAL2 = spiRead();
       }
       
       if (VAL_W == 'C')
       {
           spiWrite(VAL_READ);
           CONT = spiRead();
       }
       
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave2 Deselect
       
      VAL2_C = (VAL2/100);
      VAL2_D = (VAL2/10)%10;
      VAL2_U = VAL2%10;
       
      sprintf(ADC2, "%u", VAL2_C);
      Lcd_Set_Cursor_4bits(2,7);
      Lcd_Write_String_4bits(ADC2);
      
      sprintf(ADC2, "%u", VAL2_D);
      Lcd_Set_Cursor_4bits(2,8);
      Lcd_Write_String_4bits(ADC2);
      
      sprintf(ADC2, "%u", VAL2_U);
      Lcd_Set_Cursor_4bits(2,9);
      Lcd_Write_String_4bits(ADC2);
      
      CONT_C = (CONT/100);
      CONT_D = (CONT/10)%10;
      CONT_U = CONT%10;
       
      sprintf(CONT_S, "%u", CONT_C);
      Lcd_Set_Cursor_4bits(2,13);
      Lcd_Write_String_4bits(CONT_S);
      
      sprintf(CONT_S, "%u", CONT_D);
      Lcd_Set_Cursor_4bits(2,14);
      Lcd_Write_String_4bits(CONT_S);
      
      sprintf(CONT_S, "%u", CONT_U);
      Lcd_Set_Cursor_4bits(2,15);
      Lcd_Write_String_4bits(CONT_S);
      
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

    setupINTOSC(FOSC_4MHZ);     //Configuramos el oscilador interno a 4MHZ
    Lcd_Init_4bits();           //Inicializamos la LCD en modo de 4 bits
    
    //Configuración del SPI
    TRISC1 = 0;
    PORTCbits.RC1 = 1;
    TRISC2 = 0;
    PORTCbits.RC2 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
}
