/**
 * written for Atmega328p
 */

#include <avr/io.h>
#include <util/delay.h>

#ifndef F_CPU
 #define F_CPU 16000000UL
#endif

#define BLINK_DELAY_MS 1000

#define BAUDRATE 9600UL
#define UBRR_VAL (((F_CPU+BAUDRATE * 8)/(BAUDRATE * 16) - 1))

uint8_t command = 0;

void uart_init() {
	 // Enable receiver pin in UART register
	 UCSR0B |= (1<<RXEN0);
	 UCSR0B |= (1<<TXEN0);
	 // set frame format: 8N1
	 UCSR0C = (1 << UCSZ01)|(1 << UCSZ00);
	 // write Baud rate to high and low baud register
	 UBRR0H = UBRR_VAL >> 8;
	 UBRR0L = UBRR_VAL & 0xFF;
}

uint8_t uart_readc() {
	// wait until input is available
    while (!(UCSR0A & (1<<RXC0)));
    // read input from UDR and return
    return UDR0;
}

void uart_writec(uint8_t response) {
	// wait until sending is possible */
	while (!(UCSR0A & (1<<UDRE0))) {
	}
	// write to UDR
	UDR0 = response;
}

int main (void) {

	uart_init();

	while(1) {
		// If input char is available
		if (UCSR0A & (1<<RXC0)) {
			// read command
			command = uart_readc();
			// send response for debugging
			uart_writec(command);

		}
	}
}


