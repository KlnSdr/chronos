#define LOW false
#define F_CPU 1000000UL
#define HIGH true

#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t prellRight = 0;

void initPins();

enum Port {
    B, C, D
};
typedef struct PhysicalPin {
    enum Port port;
    uint8_t pin;
} PhysicalPin;

volatile uint8_t hour = 0;
volatile uint8_t minutes = 0;

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
void pushRightButton();

void setHour(uint8_t);
void setMinute(uint8_t);


int main() {
    EIMSK |= (1 << INT1);
    initPins();

    setHour(9);
    setMinute(24);

    while (true) {

        if (prellRight) {
            prellRight--;
        }
        
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

    sei();
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
    for (int i = 0; i < 5; ++i) {
        setPin(portHour[i], physicalHour[i]);
    }
    for (int i = 0; i < 6; ++i) {
        setPin(portMinute[i], physicalMinute[i]);
    }
}

void resetLED() {
    for (int i = 0; i < 5; ++i) {
        setPin(false, physicalHour[i]);
    }
    for (int i = 0; i < 6; ++i) {
        setPin(false, physicalMinute[i]);
    }
}

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

ISR(INT1_vect) {
    if (prellRight == 0) {
        prellRight = 50;
        pushRightButton();
    }
}

void pushRightButton(){
    if(minutes == 59){
        setMinute(0);
    }else{
        setMinute(minutes+1);
    }
}
