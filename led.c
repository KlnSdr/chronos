#include "led.h"

volatile bool portHour[5] =     {LOW, LOW, LOW, LOW, LOW};
volatile bool portMinute[6] =   {LOW, LOW, LOW, LOW, LOW, LOW};

PhysicalPin physicalHour[5] = {
    {D, 5},
    {D, 4},
    {D, 6},
    {D, 7},
    {B, 0}
};
PhysicalPin physicalMinute[6] = {
    {C, 5},
    {C, 4},
    {C, 3},
    {C, 2},
    {C, 1},
    {C, 0}
};

void initIOPins() {
    // set led pins to output
    DDRB |= 1 << 0;

    DDRC = 0x3F;

    DDRD |= 1 << 4;
    DDRD |= 1 << 5;
    DDRD |= 1 << 6;
    DDRD |= 1 << 7;
}

void setPin(bool value, PhysicalPin pin) {
    volatile uint8_t *port;
    if (pin.port == B) {
        port = &PORTB;
    } else if (pin.port == C) {
        port = &PORTC;
    } else if (pin.port == D) {
        port = &PORTD;
    }

    if (value == true) {
        *port |= 1 << pin.pin;
    } else {
        *port &= ~(1 << pin.pin);
    }

}

void displayTop() {
    for (int i = 0; i < 5; ++i) {
        setPin(portHour[i], physicalHour[i]);
        _delay_us(18);
        setPin(false, physicalHour[i]);
    }
}

void displayBottom() {
    for (int i = 0; i < 6; ++i) {
        setPin(portMinute[i], physicalMinute[i]);
        _delay_us(15);
        setPin(false, physicalMinute[i]);
    }
}


void resetLedTop() {
    for (int i = 0; i < 5; ++i) {
        setPin(false, physicalHour[i]);
    }
}
void resetLedBottom() {
    for (int i = 0; i < 6; ++i) {
        setPin(false, physicalMinute[i]);
    }
}
