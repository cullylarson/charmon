#ifndef BUTTONS_H
#define BUTTONS_H

uint8_t readButton(uint8_t button);
uint8_t getPressedButton();
uint8_t getButtonDown();
void waitForButtonUp(uint8_t buttonDown);

#endif
