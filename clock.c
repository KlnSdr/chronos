#include "clock.h"

volatile uint8_t hour = 0;
volatile uint8_t minutes = 0;
volatile uint8_t seconds = 0;

void setHour(uint8_t value) {
    hour = value;

    for (int i = 0; i < 5; i++) {
        if ((value >> i) & 1) {
            portHour[i] = HIGH;
        } else {
            portHour[i] = LOW;
        }
    }
}

void setMinute(uint8_t value) {
    minutes = value;
    for (int i = 0; i < 6; i++) {
        if ((value >> i) & 1) {
            portMinute[i] = HIGH;
        } else {
            portMinute[i] = LOW;
        }
    }
}
