#define F_CPU 1000000UL

#include "led.h"
#include "button.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "automaton.h"
#include "timer.h"
#include <util/delay.h>

int main() {
    initTimer();
    initIOPins();
    initButtons();
    sei();

    setHour(1);
    setMinute(1);

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
        displayHours();
        displayMinutes();
    }

    return 0;
}
