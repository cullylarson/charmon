#include <util/delay.h>
#include "pins.h"
#include "buttons.h"

#define DEBOUNCE_WAIT 10

uint8_t retWhicheverPressed(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
uint8_t getButtonDown();
void waitForButtonUp(uint8_t buttonDown);

uint8_t readButton(uint8_t button) {
    switch(button) {
        case 1: return READ(B1_IN, B1);
        case 2: return READ(B2_IN, B2);
        case 3: return READ(B3_IN, B3);
        case 4: return READ(B4_IN, B4);
        default: return 0;
    }
}

// reads the present, rightnow, momentary, transient value of all the
// buttons and returns whichever is pressed. 0 means no button pressed.
uint8_t readPressedButtonNow() {
    uint8_t b1 = readButton(1);
    uint8_t b2 = readButton(2);
    uint8_t b3 = readButton(3);
    uint8_t b4 = readButton(4);

    return retWhicheverPressed(b1, b2, b3, b4);
}

// finds out which of the buttons is pressed. 0 means no button pressed.
// does debouncing, to make sure the button was pressed.  will return on
// button up.
uint8_t getPressedButton() {
    uint8_t buttonDown = getButtonDown();
    waitForButtonUp(buttonDown);
    return buttonDown;
}

// finds out which button is currently down. does debounce.
uint8_t getButtonDown() {
    uint8_t lastButton = 255;
    uint8_t count = 0;
    uint8_t pressed;

    // consider a button down if it's down for four counts
    while(count <= 4) {
        pressed = readPressedButtonNow();

        if(pressed == lastButton) {
            count++;
        }
        else {
            lastButton = pressed;
            count = 0;
        }

        _delay_ms(DEBOUNCE_WAIT);
    }

    return lastButton;
}

// waits for a button to come up (assuming it's already down). does debounce.
void waitForButtonUp(uint8_t buttonDown) {
    uint8_t count = 0;
    uint8_t pressed;

    // if the button is 0, then return 0 because it probably won't change
    if(buttonDown == 0) return;

    // now wait for the button to change state
    while(count <= 4) {
        pressed = readPressedButtonNow();

        // button is different
        if(pressed != buttonDown) {
            count++;
        }
        // otherwise it's the same
        else {
            count = 0;
        }

        _delay_ms(DEBOUNCE_WAIT);
    }
}

// takes a set of button pressed values (1 or 0) and returns the first
// that is pressed (1). will return 0 if none pressed.
uint8_t retWhicheverPressed(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) {
    if(b1) return 1;
    else if(b2) return 2;
    else if(b3) return 3;
    else if(b4) return 4;
    else return 0;
}
