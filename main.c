/**
 * written for Atmega328p
 */

#include <avr/io.h>
#include <util/delay.h>

#ifndef F_CPU
 #define F_CPU 16000000UL
#endif

#define BLINK_DELAY_MS 1000

#define BAUDRATE 38400
#define UBRR_VAL (((F_CPU+BAUDRATE * 8)/(BAUDRATE * 16) - 1))

uint8_t command = 0;

void uart_init() {
	 // Enable receiver pin in UART register
	 UCSRB |= (1<<RXEN);
	 // set frame format: 8N1
	 UCSRC = (1<<URSEL)|(1 << UCSZ1)|(1 << UCSZ0);
	 // write Baud rate to high and low baud register
	 UBRRH = UBRR_VAL >> 8;
	 UBRRL = UBRR_VAL & 0xFF;
}

uint8_t uart_readc() {
	// wait until input is available
    while (!(UCSRA & (1<<RXC)));
    // read input from UDR and return
    return UDR;
}

int main (void) {
	// set pin 5 of PORTB for output
	DDRB |= (1 << (5));

	uart_init();

	while(1) {
		// If input char is available
		if (UCSRA & (1<<RXC)) {
			command = uart_readc();
		}
		// Check command
		if (command == 1) {
			PORTB = (0b00100000);
		} else {
			PORTB = (0b00000000);
		}
	}
}


