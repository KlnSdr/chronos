#define F_CPU 1000000UL

#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t prellMiddle = 0;
volatile uint8_t prellRight = 0;
volatile uint8_t STATE = 1;
volatile uint8_t counter = 0;

int main() {
    wdt_disable();

    EIMSK |= (1 << INT0);
    EIMSK |= (1 << INT1);

    // LEDs
    DDRB |= 1 << 0;

    DDRC = 0x3F;

    DDRD |= 1 << 4;
    DDRD |= 1 << 5;
    DDRD |= 1 << 6;
    DDRD |= 1 << 7;

    // Buttons
    PORTD |= 1 << 1;
    PORTD |= 1 << 2;
    PORTD |= 1 << 3;

    sei();

    // wdt_enable(WDTO_8S);
    while (1) {
        _delay_ms(20);
        if (prellMiddle) {
            prellMiddle--;
        }
        if (prellRight) {
            prellRight--;
        }

        if (PIND & (1 << 1)) {} else {
            STATE = 2;
        }

        // =============================

        if (STATE == 0) {
            if (counter < 50) {
                PORTB |= 1 << 0;

                PORTC = 0x3F;

                PORTD |= 0xF0;
            }

            _delay_us(120);
            PORTB &= ~(1 << 0);
            PORTC = 0x00;
            PORTD &= ~(0xF0);
        } else if (STATE == 1) {
            if (counter < 50) {
                PORTC = 0x2A;
                PORTD |= 0x90;
            } else {
                PORTB |= 1 << 0;
                PORTC = 0x15;
                PORTD |= 0x60;
            }
            _delay_us(120);
            PORTB &= ~(1 << 0);
            PORTC = 0x00;
            PORTD &= ~(0xF0);
        } else if (STATE == 2) {
            PORTB &= ~(1 << 0);
            PORTC = 0x00;
            PORTD &= ~(0xF0);
        }
        // =============================
        counter++;
        if (counter == 100) {
            counter = 0;
        }

    }
}

ISR(INT0_vect) {
    if (prellMiddle == 0) {
        prellMiddle = 50;
        STATE = 0;
    }
}

ISR(INT1_vect) {
    if (prellRight == 0) {
        prellRight = 50;
        STATE = 1;
    }
}
