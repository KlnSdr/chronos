TARGET=main
MCU=atmega48a
SOURCES=main.c
INCLUDES=-I.
MCU_dude=m48

PROGRAMMER=avrispmkII
# PROGRAMMER=usbasp
# PROGRAMMER=avrisp
#auskommentieren für automatische Wahl
# PORT=-P /dev/ttyACM0
# BAUD=-b19200
BAUD=-B115200

#Ab hier nichts verändern
OBJECTS=$(SOURCES:.c=.o)
CFLAGS=-c -Og 
LDFLAGS=

all: hex eeprom

hex: $(TARGET).hex

eeprom: $(TARGET)_eeprom.hex

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -j .data -j .text $(TARGET).elf $(TARGET).hex

$(TARGET)_eeprom.hex: $(TARGET).elf
	avr-objcopy -O ihex -j .eeprom --change-section-lma .eeprom=1 $(TARGET).elf $(TARGET)_eeprom.hex

$(TARGET).elf: $(SOURCES:.c=.o) $(LIBRARY:.c=.o)
	avr-gcc $(LDFLAGS) -mmcu=$(MCU) $^ -o $(TARGET).elf

.c.o:
	avr-gcc $(CFLAGS) $(INCLUDES) -mmcu=$(MCU) $< -o $@

size:
	avr-size --mcu=$(MCU) -C $(TARGET).elf

program:
	avrdude -p$(MCU_dude) $(PORT) $(BAUD) -c$(PROGRAMMER) -Uflash:w:$(TARGET).hex:a -v

clean_tmp:
	rm -rf *.o
	rm -rf *.elf

clean:
	rm -rf *.o
	rm -rf *.elf
	rm -rf *.hex

