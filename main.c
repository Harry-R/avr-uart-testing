/**
 * written for Atmega328p
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#ifndef F_CPU
 #define F_CPU 16000000UL
#endif

#define BLINK_DELAY_MS 1000

#define BAUDRATE 9600UL
#define UBRR_VAL (((F_CPU+BAUDRATE * 8)/(BAUDRATE * 16) - 1))

#define UART_MAX_COMMAND_LENGTH 10
#define COMMAND_END 'E'

volatile uint8_t uart_rcv_complete = 0;
volatile uint8_t uart_rcv_count = 0;
volatile uint8_t uart_buffer[UART_MAX_COMMAND_LENGTH + 1];


volatile uint8_t command[UART_MAX_COMMAND_LENGTH];

void uart_init() {
	// enable global interrupt
	sei();
	 // Enable receiving and sending in UART register
	 UCSR0B |= (1<<RXEN0);
	 UCSR0B |= (1<<TXEN0);
	 // enable RX interrupt
	 UCSR0B |= (1<<RXCIE0);
	 // set frame format: 8N1
	 UCSR0C = (1 << UCSZ01)|(1 << UCSZ00);
	 // write Baud rate to high and low baud register
	 UBRR0H = UBRR_VAL >> 8;
	 UBRR0L = UBRR_VAL & 0xFF;
}

void uart_writec(uint8_t response) {
	// wait until sending is possible */
	while (!(UCSR0A & (1<<UDRE0))) {
	}
	// write to UDR
	UDR0 = response;
}

ISR(USART_RX_vect) {
	uint8_t nextChar;
	// read data
	nextChar = UDR0;
	// if buffer is not in use
	if(uart_rcv_complete == 0) {
		// write to uart command array if max length and command end are not reached
		if(uart_rcv_count < UART_MAX_COMMAND_LENGTH && nextChar != COMMAND_END)
		{
			  uart_buffer[uart_rcv_count] = nextChar;
			  uart_rcv_count++;
		} else {
			// add command end char
			uart_buffer[uart_rcv_count] = COMMAND_END;
			// reset counter, set complete flag
			uart_rcv_count = 0;
			uart_rcv_complete = 1;
		}
	}
}


int main (void) {

	uart_init();

	while(1) {
		if (uart_rcv_complete == 1) {
			int i;
			// write from buffer to command array
			for (i=0; i<=UART_MAX_COMMAND_LENGTH; i++) {
				if (uart_buffer[i] != COMMAND_END) {
					command[i] = uart_buffer[i];
					uart_writec(uart_buffer[i]);
				} else {
					break;
				}
			}
			// reset complete flag
			uart_rcv_complete = 0;
			int j;
			// send back for debugging
			for (j=0; j<=UART_MAX_COMMAND_LENGTH; j++) {
				if (uart_buffer[i] != COMMAND_END) {
					uart_writec(uart_buffer[j]);
				} else {
					break;
				}
			}
		}
	}
}



