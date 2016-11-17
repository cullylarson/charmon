#include <avr/io.h>
#include "pins.h"

void setupPins() {
    // set output pins (high)
    L1_DDR |= (1 << L1);
    L2_DDR |= (1 << L2);
    L3_DDR |= (1 << L3);
    L4_DDR |= (1 << L4);

    // set input pins (low)
    B1_DDR &= ~(1 << B1);
    B2_DDR &= ~(1 << B2);
    B3_DDR &= ~(1 << B3);
    B4_DDR &= ~(1 << B4);

    // start high, which enables the pull-resistors, and also means that reading a 0 indicates button pressed (and reading 1 indicates no button pressed)
    B1_PORT |= (1 << B1);
    B2_PORT |= (1 << B2);
    B3_PORT |= (1 << B3);
    B4_PORT |= (1 << B4);

    // sound output
    SOUND_DDR |= (1 << SOUND);
}
