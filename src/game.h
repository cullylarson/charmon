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

#endif
