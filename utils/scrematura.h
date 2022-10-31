#ifndef FINAL_SCREMATURA_H
#define FINAL_SCREMATURA_H

// Il numero minimo di giocatori che possono partecipare al gioco (escludendo il giocatore controllato dall'utente).
#define MIN_PLAYERS 15

int add_concorrenti(Game *game);

int find_sizes(int input);

void play_scrematura(Game *game);

#endif //FINAL_SCREMATURA_H
