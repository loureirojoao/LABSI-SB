/*
 * serial.c
 *
 * Created: 19/12/2019 21:16:07
 *  Author: luis
 */ 
#include <avr/io.h>                                                              /* Include AVR std. library file */
#include <avr/interrupt.h>                                                /* Include AVR interrupt library file */
#include <util/delay.h>                                                          /* Include delay header file */
#include <inttypes.h>                                                            /* Include integer type header file */
#include <stdlib.h>                                                              /* Include standard library file */
#include <stdio.h>
#include <stdint.h>

typedef struct USARTRX
{
	char receiver_buffer;
	unsigned char status;
	unsigned char receive 	: 1;
	unsigned char error	: 1;

}USARTRX_st;

volatile USARTRX_st rxUSART = {0,0,0,0};
char TxBuffer[50];

/* Functions */

void SendMessage(char * buffer)
{
	unsigned char i=0;
	while(buffer[i]!='\0')
	{
		while((UCSR0A & 1<<UDRE0)==0);
		UDR0=buffer[i];
		i++;
	}
}


void serial_init()
{	
	
	/* USART */
	UBRR0H=0;
	UBRR0L=51;									// BAUDRATE 38400
	UCSR0A=(1<<U2X0);							// Double Speed
	UCSR0B=(1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);	// RX Complete Interrupt Enable, Receiver Enable; transmite Enable
	UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);				// 8 bits
	
	sprintf(TxBuffer, "Init successfully!!!\r\n");
	SendMessage(TxBuffer);
}


ISR (USART_RX_vect)
{
	rxUSART.status = UCSR0A;
	if(rxUSART.status & ((1<<FE0) | (1<<DOR0) | (1<<UPE0)))
	rxUSART.error = 1;
	rxUSART.receiver_buffer = UDR0;
	rxUSART.receive = 1;
}