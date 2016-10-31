#include <avr/interrupt.h> // won't work without including this, for some reason
#include <stdint.h>
#include "sound.h"
#include "pins.h"

void setupSound() {
    // Timer/Counter 1

    // CTC
    TCCR1B |= (1 << WGM12);

    // Prescale 8
    TCCR1B |= (1 << CS11);
}

void playTone(uint8_t tone) {
    uint16_t counter;

    switch(tone) {
        case 1:
            counter = 1000; // 500 Hz
            break;
        case 2:
            counter = 500; // 1000 Hz
            break;
        case 3:
            counter = 333; // 1500 Hz
            break;
        case 4:
            counter = 250; // 2000 Hz
            break;
        case 5:
        default:
            counter = 2500; // 200 Hz
            break;
    }

    // count to this value
    OCR1A = counter;

    // enable compare A match interrupt ( don't enable it yet, or the sound will start playing)
    TIMSK1 |= (1 << OCIE1A);
}

void stopTone() {
    // just disable the interrupt
    TIMSK1 &= ~(1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
    // square wave
    TOGGLE(SOUND_PORT, SOUND);
}
