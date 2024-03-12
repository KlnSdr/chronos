#ifndef IO_H
#define IO_H

#define F_CPU 1000000UL

#define LOW false
#define HIGH true

#include <avr/io.h>
#include "bool.h"
#include <util/delay.h>

typedef enum Port {
    B, C, D
} Port;

typedef struct PhysicalPin {
    Port port;
    uint8_t pin;
} PhysicalPin;

volatile bool portHour[5];
volatile bool portMinute[6];
PhysicalPin physicalHour[5];
PhysicalPin physicalMinute[6];

void initIOPins();
void setPin(bool, PhysicalPin);
void displayTop();
void displayBottom();
void resetLedTop();
void resetLedBottom();
#endif
