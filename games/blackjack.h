#ifndef FINAL_BLACKJACK_H
#define FINAL_BLACKJACK_H

// Le carta assegnate all'inizio del match.
#define CARDS_AT_START 2

// Punti necessari per vincere il match.
#define POINTS_TO_WIN 21

/*
 * Numero di: carte, valori e semi.
 */
#define CARD_DECK_SIZE 52
#define CARD_VALUES 13
#define CARD_SEEDS 4

/*
 * Il numero di volte che il mazzo viene mischiato.
 */
#define DECK_RANDOMNESS_FACTOR 3
#define DECK_SHUFFLES (DECK_RANDOMNESS_FACTOR * CARD_DECK_SIZE)

/*
 * Valori possibili dell'asso.
 */
#define FIRST_ACE_VALUE 1
#define SECOND_ACE_VALUE 11

// Numero di azioni possibili.
#define POSSIBLE_ACTIONS 2

/* Enumerazione indicante le azioni. */
typedef enum
{
    DRAW, STOP, NONE
} BlackJackAction;

/* Enumerazione indicante i semi delle carte. */
typedef enum
{
    CUORI, QUADRI, PICCHE, FIORI
} Seed;

/* Enumerazione indicante i valori delle carte. */
typedef enum
{
    ASSO, DUE, TRE, QUATTRO, CINQUE, SEI, SETTE, OTTO, NOVE, DIECI, J, Q, K
} Value;

/* Struttura indicante la carta. */
typedef struct
{
    Seed    seed;
    Value   value;
} Card;

/* Struttura indicante il profilo player di black jack. */
typedef struct
{
    Player          *player;
    Boolean         isHuman;
    Card            *cards;
    int             numCards;
    BlackJackAction action;
    int             points;
    Boolean         isFirstAction;
} BlackJackProfile;

/* Struttura indicante il mazzo di carte. */
typedef struct
{
    Card    *cards;
    int     position;
} DeckOfCards;

Player* play_blackjack(Group group);

#endif //FINAL_BLACKJACK_H
