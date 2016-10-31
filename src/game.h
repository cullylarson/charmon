#ifndef GAME_H
#define GAME_H

void setupGame();
void initializeNewGame();
void doBeginningOfTurn();
uint8_t isGameOver();
void doEndGame();
uint8_t isBeginningOfTurn();
void doButtonDown(uint8_t button);
void doButtonUp(uint8_t button);
uint8_t generateNewGuessable();
void playSequence();
uint16_t getPlaySequenceDelayMs();
void lightButton(uint8_t button);
void lightAllButtons();
void quenchAllButtons();
uint16_t getTurnLength();
uint16_t getPlaySequenceDelayMs();
void delayMs(uint16_t ms);

#endif
