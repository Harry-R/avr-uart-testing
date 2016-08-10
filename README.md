# avr-uart-testing

Simple code excample for testing AVRs UART communication.

## Command Line Compiling

```bash
avr-gcc -Wall -std=c11 -funsigned-char -mmcu=atmega328p -DF_CPU=1600000UL -O1 -c -o main.o main.c
avr-gcc -Wl,-Map,main.map -mmcu=atmega328p -o main.elf main.o
avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex main.elf main.hex
avr-objdump -h -S main.elf > main.lss
avr-size --format=avr --mcu=atmega328p main.elf
```
