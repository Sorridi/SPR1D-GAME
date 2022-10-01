#ifndef FINAL_PARI_O_DISPARI_H
#define FINAL_PARI_O_DISPARI_H

/* Enumerazione indicante il valore predetto/scelto. */
typedef enum
{
    PARI,
    DISPARI
} PredictValue;

// Numero di valori che il valore di predizione puo' assumere.
#define PARI_DISPARI_VALUES 2

// Numero di valori che la mano puo' assumere.
#define ALLOWED_VALUES_SIZE 6

/*
 * Valori che la mano puo' assumere.
 */
#define SHOW_HAND_MIN 0
#define SHOW_HAND_MAX 5

Player* play_pari_o_dispari(Player *players);

PredictValue ask_prediction(Player *player, Boolean isHuman);

int show_hand(Player *player, Boolean isHuman);

void print_player_prediction(Player *player, PredictValue predictValue);

#endif //FINAL_PARI_O_DISPARI_H
