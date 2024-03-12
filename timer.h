#ifndef CHRONOS_TIMER_H
#define CHRONOS_TIMER_H

#include <avr/interrupt.h>
#include "clock.h"
#include "automaton.h"

extern volatile uint8_t turnOff;

void initTimer();

#endif //CHRONOS_TIMER_H
