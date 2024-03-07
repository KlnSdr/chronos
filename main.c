#define F_CPU 1000000UL
#define LOW false
#define HIGH true

#include "bool.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t prellRight = 0;
volatile uint8_t prellMiddle = 0;
volatile uint8_t prellLeft = 0;

void initPins();

enum Port {
    B, C, D
};

enum State {
    DEFAULT,
    SET_HOUR,
    SET_MINUTE
};

typedef struct PhysicalPin {
    enum Port port;
    uint8_t pin;
} PhysicalPin;

volatile uint8_t hour = 0;
volatile uint8_t minutes = 0;
volatile uint8_t seconds = 0;

volatile enum State state = DEFAULT;

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
void rightButtonPressed();
void middleButtonPressed();
void leftButtonPressed();

void setHour(uint8_t);
void setMinute(uint8_t);


int main() {
    EIMSK |= (1 << INT0);
    EIMSK |= (1 << INT1);

    GTCCR |= (1 << TSM) | (1 << PSRASY);
    ASSR |= (1 << AS2);
    TCCR2A = (1 << WGM21);
    TCCR2B |= (1 << CS22) | (1 << CS21);

    OCR2A = 128 - 1;
    TIMSK2 |= (1 << OCIE2A);
    GTCCR &= ~(1 << TSM);

    initPins();

    setHour(9);
    setMinute(24);

    while (true) {

        if (prellRight) {
            prellRight--;
        }
        if (prellMiddle) {
            prellMiddle--;
        }
        if (prellLeft) {
            prellLeft--;
        }

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

ISR(INT0_vect) {
    if (prellMiddle == 0) {
        prellMiddle= 50;
        middleButtonPressed();
    }
}

ISR(INT1_vect) {
    if (prellRight == 0) {
        prellRight = 50;
        rightButtonPressed();
    }
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

void rightButtonPressed(){
    if (state == SET_HOUR) {
        if(hour == 23){
            setHour(0);
        }else{
            setHour(hour+1);
        }
    } else if (state == SET_MINUTE) {
        if (minutes == 59) {
            setMinute(0);
        } else {
            setMinute(minutes + 1);
        }
    }
}

void leftButtonPressed(){
    if (prellLeft == 0) {
        prellLeft = 50;
        if (state == SET_HOUR) {
            if(hour == 0){
                setHour(23);
            }else{
                setHour(hour-1);
            }
        } else if (state == SET_MINUTE) {
            if (minutes == 0) {
                setMinute(59);
            } else {
                setMinute(minutes - 1);
            }
        }
    }
}

void middleButtonPressed(){
    switch (state) {
        case DEFAULT:
            state = SET_HOUR;
            break;
        case SET_HOUR:
            state = SET_MINUTE;
            break;
        case SET_MINUTE:
            state = DEFAULT;
            break;
    }
}
