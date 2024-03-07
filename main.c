#define F_CPU 1000000UL

#include "led.h"
#include "button.h"
#include "clock.h"
#include <avr/interrupt.h>

#include <util/delay.h>

int main() {
    GTCCR |= (1 << TSM) | (1 << PSRASY);
    ASSR |= (1 << AS2);
    TCCR2A = (1 << WGM21);
    TCCR2B |= (1 << CS22) | (1 << CS21);

    OCR2A = 128 - 1;
    TIMSK2 |= (1 << OCIE2A);
    GTCCR &= ~(1 << TSM);

    initIOPins();
    initButtons();
    sei();

    setHour(9);
    setMinute(24);

    while (1) {
        prellButtons();

        if (PIND & (1 << 1)) {} else {
            leftButtonPressed();
        }
        
        _delay_ms(20);
        display();
        _delay_us(120);
        resetLED();
    }

    return 0;
}

ISR(TIMER2_COMPA_vect) {
    TCCR2B = TCCR2B;
    if (minutes == 59) {
        setMinute(0);
    } else {
        setMinute(minutes + 1);
    }

    while (ASSR & ((1 << TCN2UB) | (1 << OCR2AUB) | (1 << OCR2BUB) | (1 << TCR2AUB) | (1 << TCR2BUB)));
}
