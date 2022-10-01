#ifndef FINAL_MORRA_CINESE_H
#define FINAL_MORRA_CINESE_H

// Numero di valori che i simboli possono assumere.
#define SYMBOLS_VALUES 3

/*
 * Valori che il simbolo puo' assumere (range).
 */
#define LOWEST_SYMBOL 0
#define HIGHEST_SYMBOL 2

/* Enumerazione indicante i simboli. */
typedef enum
{
    CARTA, FORBICI, SASSO
} Symbols;

Player *play_morra_cinese(Player *players);

#endif //FINAL_MORRA_CINESE_H
