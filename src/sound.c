#include "sound.h"
#include "pins.h"
#include <avr/interrupt.h> // won't work without including this, for some reason

int extraTime = 0;

void setupSound() {
    // Timer/Counter 1

    // CTC
    TCCR1B |= (1 << WGM12);

    // Enable compare A match interrupt
    TIMSK1 |= (1 << OCIE1A);

    // Count to 255
    OCR1A = 255;

    // Prescale 1024 (this starts the timer)
    TCCR1B |= (1 << CS12) | (1 << CS10);
}

ISR(TIMER1_COMPA_vect) {
    extraTime++;

    if(extraTime > 33) {
        extraTime = 0;
        TOGGLE(L1_PORT, L1);
    }

    return 0;
}
