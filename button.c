#include "button.h"

volatile uint8_t prellRight = 0;
volatile uint8_t prellMiddle = 0;
volatile uint8_t prellLeft = 0;

void initButtons() {
    // activate pull-up resistors
    PORTD |= 1 << 1;
    PORTD |= 1 << 2;
    PORTD |= 1 << 3;

    // register interrupts
    EIMSK |= (1 << INT0);
    EIMSK |= (1 << INT1);
}

void prellButtons() {
    if (prellRight) {
        prellRight--;
    }
    if (prellMiddle) {
        prellMiddle--;
    }
    if (prellLeft) {
        prellLeft--;
    }
}

ISR(INT0_vect) {
    if (prellMiddle == 0) {
        prellMiddle = 13;
        middleButtonPressed();
    }
}

ISR(INT1_vect) {
    /*if (state == SLEEP) {
        state = DEFAULT;
    } else*/ if (prellRight == 0) {
        prellRight = 13;
        rightButtonPressed();
    }
}
