/**
 * written for Atmega328p
 */

#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL

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

int main (void) {
	// set pin 5 of PORTB for output
	DDRB |= (1 << (5));

	uart_init();
}

