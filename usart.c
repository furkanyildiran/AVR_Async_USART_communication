#include "usart.h"

#define F_CPU 16000000UL
#define USART0_TX_ON UCSR0B |= (1<<UDRIE0)
#define USART0_TX_OFF UCSR0B &= ~(1<<UDRIE0)
#define USART0_BUFFER_MASK 63
volatile unsigned int tx_head=0,tx_tail=0,rx_head=0,rx_tail;
volatile unsigned char receive_BUFFER[64];
volatile unsigned char transmit_BUFFER[64];


void USART0_initialize(uint32_t BAUD){
	cli();
	uint32_t BAUDRATE=0;
	BAUDRATE = F_CPU/(BAUD*16UL)-1;
	if(BAUD>=115200){
		BAUDRATE = F_CPU/(BAUD*8UL)-1;
		UCSR0A |= (1<<U2X0);
	}
	UBRR0H = (BAUDRATE)>>8;
	UBRR0L = BAUDRATE;
	UCSR0B |= (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	sei();
}
ISR(USART_UDRE_vect){
	UDR0 = transmit_BUFFER[tx_tail];
	tx_tail=(tx_tail+1)&USART0_BUFFER_MASK;
	if (tx_head==tx_tail)
	USART0_TX_OFF;
}

void USART0_transmit_char(unsigned char data){
	transmit_BUFFER[tx_head] = data;
	tx_head = (tx_head+1)&USART0_BUFFER_MASK;
	while(tx_head==tx_tail);
	USART0_TX_ON;
}
void USART0_transmit_array(unsigned char *p){
	while(*p != '\0'){
		USART0_transmit_char(*(p++));
	}
}
ISR(USART_RX_vect){
	receive_BUFFER[rx_head]=UDR0;
	rx_head = (rx_head+1)&USART0_BUFFER_MASK;
}

unsigned char USART0_receive(void){
	unsigned int i=rx_tail;
	rx_tail = (rx_tail+1)&USART0_BUFFER_MASK;
	while(rx_tail==rx_head);
	return receive_BUFFER[i];
}
unsigned char USART0_available(void){
	if(rx_head==rx_tail)
	return 0;
	return 1;
}

