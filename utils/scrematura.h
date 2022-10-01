#ifndef FINAL_SCREMATURA_H
#define FINAL_SCREMATURA_H

#define MIN_PLAYERS 16

int add_concorrenti(Player **players, int currentSize);

int find_sizes(int input);

void play_scrematura(Player **players, int *numPlayers, int *startSize, GameStatus *gameStatus);

#endif //FINAL_SCREMATURA_H
