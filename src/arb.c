#include <stdint.h>
#include <avr/interrupt.h> // won't work without including this, for some reason

volatile uint32_t _arbitraryCounts = 0;

void setupArb() {
    // Timer 2

    // CTC
    TCCR2A |= (1 << WGM21);

    // Prescale 1
    TCCR2B |= (1 << CS20);

    // count to this value
    OCR2A = 1;

    // enable compare A match interrupt
    TIMSK2 |= (1 << OCIE2A);
}

// get arbitrary counts
uint32_t arbc() {
    return _arbitraryCounts;
}

// called occasionally
ISR(TIMER2_COMPA_vect) {
    _arbitraryCounts++;
};
