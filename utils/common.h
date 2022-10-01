#ifndef FINAL_COMMON_H
#define FINAL_COMMON_H

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <stdlib.h>
#include <time.h>
#include <dirent.h>

/*
 * Dimensioni dei menu' principali.
 */
#define FIRST_MENU_SIZE 3
#define SECOND_MENU_SIZE 4

// Numero massimo di caratteri utilizzabili nel nome dei player.
#define MAX_CARATTERI 64

// La potenza sulla quale basarsi per la scrematura.
#define POWER 2

// Utile per la conversione char* -> int.
#define RADIX 10

// Converte una stringa in un numero intero.
#define AS_INT(var) strtol(var, NULL, RADIX)

/*
 * Numeri e caratteri ammessi in input.
 */
#define ALLOWED_NUMS "0123456789"
#define ALLOWED_CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ "
#define ALLOWED_CHARS_NOSPACES "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

// Il valore per eseguire il salto da carattere miniscolo a maiuscolo e viceversa.
#define HL_OFFSET 32

/*
 * Il numero di caratteri ammessi in input, in richiesta di stringa.
 */
#define ALLOWED_CHARS_LEN strlen(ALLOWED_CHARS)
#define ALLOWED_CHARS_NOSPACES_LEN strlen(ALLOWED_CHARS_NOSPACES)

// Il terminatore di stringa.
#define STR_TERM '\0'

/*
 * Grandezze consentite delle squadre.
 */
#define DUO_GAME 2
#define QUAD_GAME 4

/*
 * Numeri indicanti la lunghezza possibile dei numeri dell'input.
 */
#define LENGTH_ONE 1
#define LENGTH_TWO 2
#define LENGTH_THREE 3
#define LENGTH_MAX 10

// Il numero che e' possibile giocare (senza contare INDOVINA_NUMERO, BLACKJACK e NO_GAME).
#define NUM_GAMES 8

/*
 * Messaggi da stampare a video.
 */
#define PRINT_NAME_PLAYER "nome del player"
#define PRINT_CHAR_IMPICCATO "carattere della frase segreta"
#define PRINT_SAVE_NAME "nome del file dove salvare i dati"

// Il nome dei bots.
#define BOTS_NAME "Concorrente"

/*
 * Lo stato dei players.
 */
#define STATUS_ELIMINATED 0
#define STATUS_ALIVE 1

// Il numero di spazi che '\t' fornisce.
#define TAB_CHARS 8

// L'ID di chi non gioca
#define ID_NOT_PLAYING (-1)

/* Enumerazione rappresentante il classico valore Booleano */
typedef enum
{
    false, true
} Boolean;

/* Struttura rappresentante il player. */
typedef struct
{
    int     identifier;
    char    *name;
} Player;

/* Struttura rappresentante il player umano e le sue statistiche. */
typedef struct
{
    int     identifier;
    char    name[MAX_CARATTERI];
    int     spr1dWon;
    int     spr1dPlayed;
    int     finalsPlayed;
    int     gamesPlayed;
    int     gamesWon;
} Profile;

/* Struttura rappresentante il file di salvataggio. */
typedef struct
{
    int     numProfiles;    // intero K indicante il numero di profili giocatore creati, indipendentemente dalla partita;
    Profile **profiles;     // array di K profili giocatore;
    int     playing;        // un intero indicante se è in corso una partita.
    int     totPlayers;     // numero N di giocatori totali della partita (sia CPU che utente);
    int     totHumans;      // numero M di giocatori utente nella partita in corso;
    int     *humanIds;      // array di M interi contenente gli indici indicanti i giocatori utente che stanno partecipando alla partita;
    int     *playerStatuses;// array di N interi, indicante se i giocatori sono ancora in gioco o sono eliminati
} GameStatus;

/* Enumerazione rappresentante i tipi di errori possibili in input. */
typedef enum
{
    ILLEGAL_CHARS,
    TOO_MANY_CHARS,
    TOO_LITTLE_CHARS,
    MUST_BE_EXACT_CHARS,
    MUST_BE_DIFFERENT_CHARS,

    TOO_BIG_NUM,
    TOO_LITTLE_NUM,
    MUST_BE_EXACT_NUM,
    MUST_BE_DIFFERENT_NUM,

    PLAYER_EXISTS
} InputErr;

/* Enumerazione rappresentante i tipi di giochi. */
typedef enum
{
    DADI,
    BIGLIE,
    MURRA,
    TRIS,
    MORRA_CINESE,
    PARI_O_DISPARI,

    PONTE_DI_VETRO,
    IMPICCATO,

    BLACKJACK,
    INDOVINA_NUMERO,
    NO_GAME
} Games;

int gen_num(int estremoMinore, int estremoMaggiore);
Player **gen_groups(Player *players, int size, int groupSize);
Player *gen_group(Player *players, int size, int *shuffledIds, int *counter);

int *shuffle_players(int size);

Boolean check_characters(char *toCheck, char *allowed);
Boolean check_profile_entry(Profile *profiles, int size, char *name);
void insert_player(Player **players, int *size, char *name, Boolean verbose);
void insert_profile(GameStatus *gameStatus, char *name);

Player* find_front_man(Player *group, int size);

void selection_sort(int *toSort, int elements);

Boolean group_full_of_bots(Player *group, int size);

void update_player_statuses(Player *winners, int playerSize, int *statuses, int size);

void incr_stats_winners(GameStatus *gameStatus, Player *winners, int numWinners, Boolean isFinal);
void incr_stats(GameStatus *gameStatus, Player *players, int numPlayers, Boolean isFinal);
void update_totals(Player *players, int numPlayers, GameStatus *gameStatus);

#endif //FINAL_COMMON_H
