#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pins.h"
#include "buttons.h"

void setup();

int main(void) {
    setup();

    while(1) {
        GOHI(L1_PORT, L1);
        _delay_ms(1000);
        GOLO(L1_PORT, L1);
        _delay_ms(1000);
    }
}

void setup() {
    setupPins();

    sei();
}
