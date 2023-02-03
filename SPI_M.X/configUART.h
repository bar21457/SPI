/* 
 * File:   configUART.h
 * Author: byron
 *
 * Created on February 1, 2023, 9:45 PM
 */

#ifndef CONFIGUART_H
#define	CONFIGUART_H

void configUART_RX(uint16_t baudrate);
void configUART_TX(uint16_t baudrate);
void write_char_UART(char *character);
char read_char_UART();

#endif	/* CONFIGUART_H */