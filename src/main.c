#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pins.h"
#include "sound.h"
#include "buttons.h"
#include "game.h"
#include "arb.h"

void setup();

int main(void) {
    uint8_t button = 0;

    setup();

    initializeNewGame();

    while(1) {
        if(isGameOver()) {
            doEndGame();
            initializeNewGame();
        }

        if(isBeginningOfTurn()) {
            doBeginningOfTurn();
        }

        button = getButtonDown();
        if(button != 0) {
            doButtonDown(button);
            waitForButtonUp(button);
            doButtonUp(button);
        }
    }
}

void setup() {
    setupPins();
    setupSound();
    setupArb();
    setupGame();

    sei();
}
