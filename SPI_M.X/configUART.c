/*
 * File:   configUART.c
 * Author: byron
 *
 * Created on February 1, 2023, 9:46 PM
 */


#include <xc.h>
#include "configUART.h"

#define _XTAL_FREQ 8000000

void configUART_RX(uint16_t baudrate){
    
    SPBRG = baudrate;

    TXSTAbits.SYNC = 0;         // Trabajaremos de forma asincrona
    RCSTAbits.SPEN = 1;         // habilitamos mÃ³dulo UART
}

void configUART_TX(uint16_t baudrate){
    SPBRG = baudrate;
    
    TXSTAbits.TXEN = 1;         // Habilitamos la transmision
    
    PIR1bits.TXIF = 0;
    
    RCSTAbits.CREN = 1;         // Habilitamos la recepcion
}

void write_char_UART (char *character){
    
    int i;
    for (i = 0; character[i] != '\0'; i++){
        if(TXSTAbits.TRMT == 1){
        //TXREG = PORTB+48;
        TXREG=character[i];
        __delay_us(40);
        }
    }
}

char read_char_UART(){
    if(PIR1bits.RCIF == 1){
            PIR1bits.RCIF = 0;
            return RCREG;
            }
}
