#ifndef FINAL_TRIS_H
#define FINAL_TRIS_H

// Lunghezza dei lati della tabella.
#define TABLE_SIDE 3

/*
 * Simboli delle pedine dei player.
 */
#define PAWN_PLAYER1 O
#define PAWN_PLAYER2 X

// Numero di valori delle pedine.
#define PAWN_VALUES 3

/*
 * Il valore massimo e minimo della casella nella quale e' possibile piazzare le pedine.
 */
#define LOWEST_POSITION 0
#define HIGHEST_POSITION 2

// Il numero di caselle da occupare per vincere il match.
#define PAWN_SERIES_WIN 3

/* Enumerazione indicante le pedine. */
typedef enum
{
    EMPTY, O, X
} Pawn;

Player *play_tris(Group group);

#endif //FINAL_TRIS_H
