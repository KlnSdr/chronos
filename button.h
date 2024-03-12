#ifndef BUTTON_H
#define BUTTON_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include "buttonHandler.h"
#include "automaton.h"

extern volatile uint8_t prellRight;
extern volatile uint8_t prellMiddle;
extern volatile uint8_t prellLeft;

void initButtons();
void prellButtons();
#endif
