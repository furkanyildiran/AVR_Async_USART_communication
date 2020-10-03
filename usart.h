#ifndef USART_H
#define USART_H

#include <avr/io.h>
#include <avr/interrupt.h>




void USART0_initialize(uint32_t BAUD);

void USART0_transmit_char(unsigned char data);

void USART0_transmit_array(unsigned char *p);

unsigned char USART0_receive(void);

unsigned char USART0_available(void);


#endif