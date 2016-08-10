# avr-uart-testing

Simple code excample for testing AVRs UART communication.

## Command Line Compiling

```bash
# compiling
avr-gcc -Wall -std=c11 -funsigned-char -mmcu=atmega328p -DF_CPU=1600000UL -Os -c -o main.o main.c
# linking
avr-gcc -Wl,-Map,main.map -mmcu=atmega328p -o main.elf main.o
# objcopy, objdump, ...
avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex main.elf main.hex
avr-objdump -h -S main.elf > main.lss
avr-size --format=avr --mcu=atmega328p main.elf

# avrdude (programming)
avrdude -pm328p -carduino -b57600 -P/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0 -patmega328p -Uflash:w:main.hex:a
```
