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

    // You can get the frequency for a counter value here:  https://www.easycalculation.com/engineering/electrical/avr-timer-calculator.php
    // Just put '8000000' for the System Clock Frequency, 16-bit Timer Resolution, Clk/8 Prescaler, and Calculate Using 'Total Timer Ticks'.
    // Then put your 'counter' value into the 'Total Timer Ticks' box and hit Calculate.  The frequency of the sound is one half of the value
    // shown in the 'New Frequency' box.  So, for 2500 ticks, you get 400 as the 'New Frequency', which is a 200 Hz sound.
    //
    // To get specific frequency, Calculate Using 'New Frequency' and enter two times the sound frequency you want.  So if you want a 200 Hz
    // sound, enter 400 Hz.  The counter value to produce that sound is in the 'Total Timer Ticks' box.
    //
    // The original Simon game used these frequencies:
    // 415 Hz – G#4 (true pitch 415.305 Hz)
    // 310 Hz – D#4 (true pitch 311.127 Hz)
    // 252 Hz ‐ B3 (true pitch 247.942 Hz)
    // 209 Hz – G#3 (true pitch 207.652 Hz)
    //
    // I chose the frequencies mentioned here:  http://www.waitingforfriday.com/index.php/Reverse_engineering_an_MB_Electronic_Simon_game#Sound_frequencies_and_timing
    //
    // G4 391.995 Hz
    // E4 329.628 Hz
    // C4 261.626 Hz
    // G3 195.998 Hz

    switch(tone) {
        case 1:
            counter = 2551; // 196 Hz
            break;
        case 2:
            counter = 1912; // 261.5 Hz
            break;
        case 3:
            counter = 1517; // 329.5 Hz
            break;
        case 4:
            counter = 1275; // 392 Hz
            break;
        case 5:
        default:
            counter = 5000; // 100 Hz
            break;
    }

    // count to this value
    OCR1A = counter;

    // enable compare A match interrupt
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
