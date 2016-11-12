#include <avr/io.h>

#ifndef PINS_H
#define PINS_H

#define B1          PD0
#define B1_PORT     PORTD
#define B1_DDR      DDRD
#define B1_IN       PIND

#define B2          PD1
#define B2_PORT     PORTD
#define B2_DDR      DDRD
#define B2_IN       PIND

#define B3          PD2
#define B3_PORT     PORTD
#define B3_DDR      DDRD
#define B3_IN       PIND

#define B4          PD3
#define B4_PORT     PORTD
#define B4_DDR      DDRD
#define B4_IN       PIND

#define L1          PD5
#define L1_PORT     PORTD
#define L1_DDR      DDRD

#define L2          PD6
#define L2_PORT     PORTD
#define L2_DDR      DDRD

#define L3          PD7
#define L3_PORT     PORTD
#define L3_DDR      DDRD

#define L4          PB0
#define L4_PORT     PORTB
#define L4_DDR      DDRB

#define SOUND       PB1
#define SOUND_PORT  PORTB
#define SOUND_DDR   DDRB

// write digital "high" to pin <pn> on port <port>
#define GOHI(port, pn) port |= (1<<pn)

// write digital "low" to pin <pn> on port <port>
#define GOLO(port, pn) port &= ~(1<<pn)

#define TOGGLE(port, pn) port ^= (1<<pn)

// if that bit is zero, the switch is being pressed. if it's 1, not being pressed
#define READ(port, pn) (port & (1<<pn)) == 0

void setupPins();

#endif
