#define LOW false
#define F_CPU 1000000UL
#define HIGH true

#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>


void initPins();

enum Port {
    B, C, D
};
typedef struct PhysicalPin {
    enum Port port;
    uint8_t pin;
} PhysicalPin;

volatile bool portHour[5] = {LOW, LOW, LOW, LOW, LOW};
volatile bool portMinute[6] = {LOW, LOW, LOW, LOW, LOW, LOW};
PhysicalPin physicalHour[5] = {{D, 5},
                               {D, 4},
                               {D, 6},
                               {D, 7},
                               {B, 0}};
PhysicalPin physicalMinute[6] = {{C, 5},
                                 {C, 4},
                                 {C, 3},
                                 {C, 2},
                                 {C, 1},
                                 {C, 0}};

void setPin(bool, PhysicalPin);

void display();

void resetLED();


int main() {

    initPins();
    portHour[1] = HIGH;
    portHour[3] = HIGH;
    while (true) {
        _delay_ms(20);
        display();
        _delay_us(120);
        resetLED();
    }


    return 0;
}

void initPins() {
    //LED
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

void display() {
    for (int i = 0; i < 4; ++i) {
        setPin(portHour[i], physicalHour[i]);
    }
    for (int i = 0; i < 5; ++i) {
        setPin(portMinute[i], physicalMinute[i]);
    }
}

void resetLED() {
    for (int i = 0; i < 4; ++i) {
        setPin(false, physicalHour[i]);
    }
    for (int i = 0; i < 5; ++i) {
        setPin(false, physicalMinute[i]);
    }
}

