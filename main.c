/**
 * written for Atmega328p
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BLINK_DELAY_MS 1000

#define BAUDRATE 9600UL
#define UBRR_VAL (((F_CPU+BAUDRATE * 8)/(BAUDRATE * 16) - 1))

#define UART_MAX_COMMAND_LENGTH 10U
#define COMMAND_START 'S'
#define COMMAND_END 'E'

volatile uint8_t uart_rcv_complete = 0;
volatile uint8_t uart_rcv_count = 0;
volatile uint8_t uart_buffer[UART_MAX_COMMAND_LENGTH + 1];

volatile uint8_t command[UART_MAX_COMMAND_LENGTH];

/**
 * initializes the UART
 */
void uart_init() {
	// enable global interrupt
	sei();
	// enable receiving and sending
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	// enable RX interrupt
	UCSR0B |= (1 << RXCIE0);
	// set frame format: 8N1
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	// write Baud rate to high and low baud register
	UBRR0H = UBRR_VAL >> 8;
	UBRR0L = UBRR_VAL & 0xFF;
}

/**
 * writes a char to the UART data register
 */
void uart_writechar(uint8_t data) {
	// wait until sending is possible
	while (!(UCSR0A & (1 << UDRE0))) {
	}
	// write to UDR
	UDR0 = data;
}

/**
 * interrupt routine: called when the UART receives a char, writes the chars to uart_buffer array
 */
ISR(USART_RX_vect) {
	uint8_t nextChar;
	// read data
	nextChar = UDR0;

	switch(nextChar) {
		case COMMAND_START:
			uart_rcv_count = 0;
			break;
		case COMMAND_END:
			for (uint8_t i = 0; i < (uart_rcv_count); i++) {
				// write from UART buffer to command array
				command[i] = uart_buffer[i];
				uart_buffer[i] = 0x00;
			}
			uart_rcv_complete = 1;
			break;
		default:
			if (uart_rcv_count > UART_MAX_COMMAND_LENGTH) {
				// reset UART buffer
				for (uint8_t i = 0; i <= UART_MAX_COMMAND_LENGTH; i++) {
					uart_buffer[i] = 0x00;
				}
				return;
			}
			uart_buffer[uart_rcv_count] = nextChar;
			uart_rcv_count++;
	}
}

int main(void) {

	uart_init();

	/**
	 * main loop
	 */
	while (1) {
		// handle received command
		if (uart_rcv_complete == 1) {
			uint8_t i;
			// send command array back for debugging
			for (i = 0; i <= UART_MAX_COMMAND_LENGTH; i++) {
				if (command[i] != 0x00) {
					uart_writechar(command[i]);
				} else {
					break;
				}
				command[i] = 0x00;
			}
			// reset complete flag
			uart_rcv_complete = 0;
		}
	}
}

