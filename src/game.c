#include <stdlib.h>
#include <stdint.h>
#include <avr/interrupt.h> // won't work without including this, for some reason
#include <util/delay.h>
#include "game.h"
#include "sound.h"
#include "pins.h"

#define MAX_SEQUENCE_SIZE   200
#define END_GAME_TONE       5

uint8_t _sequence[MAX_SEQUENCE_SIZE];
uint8_t _sequenceNextValueIdx;
uint8_t _lastGuessWrong;
uint8_t _beginningOfTurn;
uint8_t _guessIdx;
uint8_t _skipSrandOnce;

volatile uint16_t _turnTime = 0; // should count in approx. milliseconds
volatile uint32_t _totalTime = 0; // should count in approx. milliseconds

// TODO -- Everything assumes 8MHz clock speed. If that works, need to make sure system clocks to 8MHz in production.
// TODO -- Consider switching to 8Mhz clock so we're guaranteed that speed.
// TODO -- Consider a switch for different difficulties.

uint8_t isFirstTurn();
void playSequence();
uint16_t getPlaySequenceDelayMs();
void lightButton(uint8_t button);
void lightAllButtons();
void quenchAllButtons();
uint16_t getTurnLength();
uint16_t getPlaySequenceDelayMs();
void delayMs(uint16_t ms);
uint8_t generateNewGuessable();
void doInitialRandSeed();

void setupGame() {
    // Seed the random number generator
    doInitialRandSeed();

    // Timer/Counter 0

    // CTC
    TCCR0A |= (1 << WGM01);

    // Prescale 256
    TCCR0B |= (1 << CS01) | (1 << CS00);

    // count to this value
    OCR0A = 125;

    // enable compare A match interrupt
    TIMSK0 |= (1 << OCIE0A);
}

void doInitialRandSeed() {
    // we're doing this first seed, so no need to do it again the next time we srand
    _skipSrandOnce = 1;

    // Initial seed of the random number generator on pin 28 (PC5/ADC5)

    // voltage reference: AVCC with external capacitor at AREF pin (this is just for noise, so I don't think I need a cap)
    ADMUX |= (1 << REFS0);

    // read from ADC5
    ADMUX |= (1 << MUX0) | (1 << MUX2);

    // apparently you need to to ADC at between 50kHz and 200kHz for 10-bit converstion, so
    // we need to prescale the clock. Assuming 8Mhz, scaling by 64 will give us 125kHz sampling
    // rate. this is also good in case our clock speed is a bit slow (we'll still be in the range).
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1); // scale by 64

    ADCSRA |= (1 << ADEN); // enable ADC
    ADCSRA |= (1 << ADSC); // start conversion

    // wait until ADC is done
    while(ADCSRA & (1 << ADSC));

    // read the first byte
    uint8_t byte1 = ADCL;
    uint8_t trashByte = ADCH; // apparently you also need to read from ADCH, or the next conversion won't produce a new value?

    // start conversion again
    ADCSRA |= (1 << ADSC);

    // wait until ADC is done again
    while (ADCSRA & (1 << ADSC));

    // read the second byte
    uint8_t byte2 = ADCL;
    trashByte = ADCH;

    uint16_t seed = (byte1 << 8) | byte2;

    srand(seed);

    // disable ADC
    ADCSRA &= ~(1 << ADEN);
}

// about every millisecond
ISR(TIMER0_COMPA_vect) {
    _turnTime++;
    _totalTime++;
}

void initializeNewGame() {
    // reset everything.  don't put anything in the sequence, since that will be done at the beginning of the turn

    _beginningOfTurn = 1;
    _sequenceNextValueIdx = 0;
    _lastGuessWrong = 0;
    _guessIdx = 0;
    _turnTime = 0;

    uint8_t i;

    for(i = 0; i < 3; i++) {
        lightAllButtons();
        _delay_ms(400);
        quenchAllButtons();
        _delay_ms(400);
    }
}

void doBeginningOfTurn() {
    _beginningOfTurn = 0;
    // start guessing from the beginning
    _guessIdx = 0;

    // generate a new value for the sequence
    _sequence[_sequenceNextValueIdx] = generateNewGuessable();
    _sequenceNextValueIdx++;

    // if this isn't the first turn, wait a little before
    // playing the sequence so it doesn't come right after
    // the last button press
    if(!isFirstTurn()) _delay_ms(600);

    // show the sequence to the user
    playSequence();

    // start the timer over
    _turnTime = 0;
}

uint8_t isFirstTurn() {
    return _sequenceNextValueIdx == 1;
}

uint8_t isGameOver() {
    // game is over if:
    // last guess was wrong
    if(_lastGuessWrong) return 1;
    // or, time has run out
    else if(_turnTime >= getTurnLength()) return 1;
    // or, the sequence is full (it has reached MAX_SEQUENCE_SIZE)
    else if(_sequenceNextValueIdx >= MAX_SEQUENCE_SIZE) return 1;
    else return 0;
}

void doEndGame() {
    uint8_t i;

    playTone(END_GAME_TONE);
    lightAllButtons();
    _delay_ms(1000);

    for(i = 0; i < 3; i++) {
        quenchAllButtons();
        _delay_ms(400);
        lightAllButtons();
        _delay_ms(400);
    }

    _delay_ms(600);
    quenchAllButtons();
    stopTone();
    _delay_ms(1000);
}

uint8_t isBeginningOfTurn() {
    return _beginningOfTurn;
}

void doButtonDown(uint8_t button) {
    playTone(button);
    lightButton(button);

    // the guess was correct
    if(button == _sequence[_guessIdx]) {
        _guessIdx++;

        // reset the turn time on each correct guess
        _turnTime = 0;

        // the entire sequence was guessed correctly
        if(_guessIdx >= _sequenceNextValueIdx) {
            _beginningOfTurn = 1;
        }
    }
    // guess was wrong
    else {
        _lastGuessWrong = 1;
    }
}

void doButtonUp(uint8_t button) {
    stopTone();
    quenchAllButtons();
}

uint16_t getTurnLength() {
    switch(_sequenceNextValueIdx) {
        case 0:
        case 1: return 5000;
        case 2: return 3000;
        default: return 2000;
    }
}

uint16_t getPlaySequenceDelay() {
    switch(_sequenceNextValueIdx) {
        case 0:
        case 1: return 500;
        case 2: return 400;
        case 3: return 300;
        case 4: return 250;
        default: return 200;
    }
}

void playSequence() {
    uint8_t i;
    uint8_t button;
    uint16_t playDelay = getPlaySequenceDelay();

    uint8_t lastIdx = _sequenceNextValueIdx - 1;

    for(i = 0; i <= lastIdx; i++) {
        button = _sequence[i];

        playTone(button);
        lightButton(button);

        delayMs(playDelay);

        stopTone();
        quenchAllButtons();

        // don't delay on last, since the lights will just be off while the user waits
        if(i != lastIdx) delayMs(playDelay);
    }
}

void delayMs(uint16_t ms) {
    while(ms--) _delay_ms(1);
}

void lightAllButtons() {
    lightButton(1);
    lightButton(2);
    lightButton(3);
    lightButton(4);
}

void lightButton(uint8_t button) {
    switch(button) {
        case 1:
            GOHI(L1_PORT, L1);
            break;
        case 2:
            GOHI(L2_PORT, L2);
            break;
        case 3:
            GOHI(L3_PORT, L3);
            break;
        case 4:
        default:
            GOHI(L4_PORT, L4);
            break;
    }
}

void quenchAllButtons() {
    GOLO(L1_PORT, L1);
    GOLO(L2_PORT, L2);
    GOLO(L3_PORT, L3);
    GOLO(L4_PORT, L4);
}

// generates a number between 1 and 4 (inclusive)
uint8_t generateNewGuessable() {
    // this first time this is called, it will already have been seeded by random noise
    if(_skipSrandOnce) {
        _skipSrandOnce = 0;
    }
    else {
        srand(_totalTime);
    }

    // %4 gives a number between 0-3. +1 gives a number between 1-4.
    return (rand() % 4) + 1;

}
