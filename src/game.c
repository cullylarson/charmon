#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>
#include "game.h"
#include "arb.h"
#include "sound.h"
#include "pins.h"
#include "arb.h"

#define MAX_SEQUENCE_SIZE   50
#define TURN_LENGTH_FACTOR  1000
#define END_GAME_TONE       5

uint8_t _sequence[MAX_SEQUENCE_SIZE];
uint8_t _sequenceNextValueIdx;
uint8_t _turnTime;
uint8_t _lastGuessWrong;
uint8_t _guessIdx;

// TODO -- Refine the TURN_LENGTH_FACTOR and turn length (maybe even define it in terms of F_CPU, or use a timer to tell if it's over)
// TODO -- Make the 'play sequence' go faster, depending on the level
// TODO -- Somehow need to trigger the random number seeding after a button press (so it's actually random). Or maybe use some external noise?
//         Actually, the worst case, the first value will always be the same, but the second will be actually random (since rand is seeded on each new value)

void initializeGame() {
    // reset everything.  don't put anything in the sequence, since that will be done at the beginning of the turn

    _sequenceNextValueIdx = 0;
    _lastGuessWrong = 0;
    _guessIdx = 0;
}

// generates a number between 1 and 4 (inclusive)
uint8_t generateNewGuessable() {
    srand(arbc());

    // %4 gives a number between 0-3. +1 gives a numbet between 1-4.
    return (rand() % 4) + 1;
}

void doBeginningOfTurn() {
    // generate a new value for the sequence
    _sequence[_sequenceNextValueIdx] = generateNewGuessable();
    _sequenceNextValueIdx++;

    // show the sequence to the user
    playSequence();

    // start the timer over
    _turnTime = 0;
}

void playSequence() {
    uint8_t i;
    uint8_t button;

    for(i = 0; i < _sequenceNextValueIdx; i++) {
        button = _sequence[i];

        playTone(button);
        lightButton(button);

        delayMs(getPlaySequenceDelayMs());

        stopTone();
        quenchAllButtons();
    }
}

void delayMs(uint16_t ms) {
    while(ms--) _delay_ms(1);
}

uint16_t getPlaySequenceDelayMs() {
    if(_sequenceNextValueIdx == 0) return TURN_LENGTH_FACTOR;
    else return TURN_LENGTH_FACTOR / _sequenceNextValueIdx;
}

uint8_t isGameOver() {
    // 1. update the 'turn time'
    _turnTime++;

    // game is over if time has run out, or last guess was wrong, or the sequence is full (it has reached MAX_SEQUENCE_SIZE)
    if(_lastGuessWrong) return 1;
    else if(_turnTime >= getTurnLength()) return 1;
    else if(_sequenceNextValueIdx >= MAX_SEQUENCE_SIZE) return 1;
    else return 0;
}

uint16_t getTurnLength() {
    if(_sequenceNextValueIdx == 0) return TURN_LENGTH_FACTOR;
    else return TURN_LENGTH_FACTOR / _sequenceNextValueIdx;
}

void doEndGame() {
    uint8_t i;

    playTone(END_GAME_TONE);
    lightAllButtons();
    _delay_ms(1000);

    for(i = 0; i < 5; i++) {
        quenchAllButtons();        
        _delay_ms(400);
        lightAllButtons();
    }

    _delay_ms(1000);
    quenchAllButtons();        
    stopTone();
}

uint8_t isBeginningOfTurn() {
    // is beginning of turn if no guesses have been made (guess index is 0)

    if(_guessIdx == 0) return 1;
    else return 0;
}

void doButtonDown(uint8_t button) {
    // the guess was correct
    if(button == _sequence[_guessIdx]) {
        _guessIdx++;

        // the entire sequence was guessed correctly
        if(_guessIdx >= _sequenceNextValueIdx) {
            _guessIdx = 0;
        }
    }
    // guess was wrong
    else {
        _lastGuessWrong = 1;
    }

    playTone(button);
    lightButton(button);
}

void doButtonUp(uint8_t button) {
    stopTone();
    quenchAllButtons();
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
