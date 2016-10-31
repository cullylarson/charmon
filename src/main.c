#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pins.h"
#include "sound.h"
#include "buttons.h"

void setup();

int main(void) {
    setup();

    while(1) {
    }
}

void setup() {
    setupPins();
    setupSound();

    sei();
}
