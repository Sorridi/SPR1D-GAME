#ifndef FINAL_IMPICCATO_H
#define FINAL_IMPICCATO_H

// Utility per la generazione casuale, converte un numero in stringa.
#define NUM_TO_CHAR_OFFSET 97

// Numero massimo di errori che e' possibile commettere.
#define MAX_TRIES 4

// Il numero di players che devono essere eliminati per constatare il vincitore.
#define DELETED_PLAYER_TO_WIN 3

/*
 * L'alfabeto e la lunghezza di esso.
 */
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define ALPHABET_LENGTH 26

// Il carattere nascosto
#define HIDDEN_CHAR '_'

/* Struttura rappresentante la frase segreta. */
typedef struct
{
    char    *content;
    char    *contentHidden;
    char    *availableChars;
    int     contentLength;
} Phrase;

Player* play_impiccato(Group group);

#endif //FINAL_IMPICCATO_H
