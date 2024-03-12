#define F_CPU 1000000UL

#include "led.h"
#include "button.h"
#include "clock.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "automaton.h"

#include <util/delay.h>

volatile uint8_t turnOff = 0;

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
    setMinute(0);

    state = DEFAULT;

    while (1) {
        while (state == SLEEP) {
            prellButtons();
            turnOff = 0;
            set_sleep_mode(SLEEP_MODE_PWR_SAVE);
            sleep_mode();
        }
        prellButtons();

        if (PIND & (1 << 1)) {} else {
            leftButtonPressed();
        }
        
        _delay_ms(18);
        displayTop();
//        resetLedTop();
        displayBottom();
//        resetLedBottom();
//        _delay_us(90);

    }

    return 0;
}

ISR(TIMER2_COMPA_vect){
    TCCR2B = TCCR2B;
    if (minutes == 59) {
        setMinute(0);
        setHour(hour + 1);
    } else {
        setMinute(minutes + 1);
    }

    if (state == DEFAULT) {
        if (turnOff == 10) {
            state = SLEEP;
            turnOff = 0;
        } else {
            turnOff++;
        }
    }

    while (ASSR & ((1 << TCN2UB) | (1 << OCR2AUB) | (1 << OCR2BUB) | (1 << TCR2AUB) | (1 << TCR2BUB)));
}
