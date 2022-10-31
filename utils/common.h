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
 * Il numero di volte che i player vivi vengono mischiati.
 */
#define PLAYER_RANDOMNESS_FACTOR 2
#define PLAYER_SHUFFLES(num) (PLAYER_RANDOMNESS_FACTOR * num)

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
    Boolean alive;
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
} Status;

/* Struttura rappresentate il gioco. */
typedef struct
{
    Status      status;
    Player      **players;
    int         alivePlayers;
    int         aliveHumans;
} Game;

/* Struttura rappresentante le opzioni possibili dell'aggiornamento dei dati. */
typedef enum
{
    TOT_PLAYERS, TOT_PROFILES,
    PLAYING,
    PLAYER_STATUSES,
    NUM_PROFILES,
    STARTUP
} ToUpdate;

/* Struttura rappresentante il gruppo di players. */
typedef struct
{
    Player *players;
} Group;

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

    PLAYER_EXISTS,
    SYSTEM_ERROR
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
Group *gen_groups(Game *game, int groupSize);
Group gen_group(Player **players, int size, int *shuffledIds, int *counter);

int *shuffle_players(Game *game);

Boolean check_characters(char *toCheck, char *allowed);
Boolean check_profile_entry(Game *game, char *name);
void insert_player(Game *game, char *name, Boolean verbose, Boolean isAlive);
void insert_profile(Game *game, char *name);
void insert_player_custom(Game *game, Profile *profile, Boolean verbose);

Player* find_front_man(Game *game);
Boolean front_man_in_group(Player *frontMan, Group group, int size);

void selection_sort(int *toSort, int elements);
void selection_sort_profiles(Game *game);

Boolean group_full_of_bots(Group group, int size);

#endif //FINAL_COMMON_H
