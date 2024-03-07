#ifndef CHRONOS_CLOCK_H
#define CHRONOS_CLOCK_H

#include "bool.h"
#include <stdint.h>
#include "led.h"

#define LOW false
#define HIGH true

extern volatile uint8_t hour;
extern volatile uint8_t minutes;
extern volatile uint8_t seconds;

void setHour(uint8_t);
void setMinute(uint8_t);

#endif //CHRONOS_CLOCK_H
