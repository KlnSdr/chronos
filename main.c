#define F_CPU 1000000UL

#include "bool.h"
#include "io.h"

#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t prellRight = 0;
volatile uint8_t prellMiddle = 0;
volatile uint8_t prellLeft = 0;

enum State {
    DEFAULT,
    SET_HOUR,
    SET_MINUTE
};

volatile uint8_t hour = 0;
volatile uint8_t minutes = 0;
volatile uint8_t seconds = 0;

volatile enum State state = DEFAULT;

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

    initIOPins();
    sei();

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
