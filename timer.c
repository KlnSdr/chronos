#include "timer.h"
volatile uint8_t turnOff = 0;

void initTimer() {
    GTCCR |= (1 << TSM) | (1 << PSRASY);
    ASSR |= (1 << AS2);
    TCCR2A = (1 << WGM21);
    TCCR2B |= (1 << CS22) | (1 << CS21);

    OCR2A = 128 - 1;
    TIMSK2 |= (1 << OCIE2A);
    GTCCR &= ~(1 << TSM);
}

void stopTimer() {
    GTCCR |= (1 << TSM) | (1 << PSRASY);
}

void startTimer() {
    GTCCR &= ~(1 << TSM);
}

ISR(TIMER2_COMPA_vect) {
    TCCR2B = TCCR2B;

    seconds++;

    if (seconds == 60) {
        seconds = 0;
        minutes++;
    }

    if (minutes == 60) {
        minutes = 0;
        hour++;
    }

    if (hour == 24) {
        setHour(0);
    }

    setMinute(minutes);
    setHour(hour);

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
