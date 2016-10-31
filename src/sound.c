#include "sound.h"
#include "pins.h"
#include <avr/interrupt.h> // won't work without including this, for some reason

void setupSound() {
    // Timer/Counter 1

    // CTC
    TCCR1B |= (1 << WGM12);

    // Enable compare A match interrupt
    TIMSK1 |= (1 << OCIE1A);

    // Count to this value
    OCR1A = 1000;

    // Prescale 8
    TCCR1B |= (1 << CS11);
}

// Will toggle at 500Hz
ISR(TIMER1_COMPA_vect) {
    TOGGLE(SOUND_PORT, SOUND);

    return 0;
}
