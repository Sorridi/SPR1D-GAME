#ifndef FINAL_SHORTCUTS_H
#define FINAL_SHORTCUTS_H

#include "file_manager.h"

/*
 * Macro per controllare la validita' di un puntatore, in questo software, viene utilizzata per controllare se
 * l'allocazione dinamica e' andata a buon fine, altrimenti, il programma verra' chiuso forzatamente.
 */
#define CRASH_IF_NULL(pointer)                              \
if (pointer == NULL)                                        \
{                                                           \
    println("\n(!) Impossibile allocare memoria! (!)\n");   \
    exit(-1);                                               \
}

/*
 * Macro per allocazione e ri-allocazione della memoria.
 */
#define MALLOC_ARRAY(type, size) (type *) malloc(sizeof(type) * (size))
#define CALLOC_ARRAY(type, size) (type *) calloc((size), sizeof(type))
#define REALLOC_ARRAY(type, arr, size) (type *) realloc(arr, sizeof(type) * (size))

// Macro utility per stampare l'address di una variabile.
#define ADDR(x) printf("ADDR = %p\n", x);

/* Macro utility per debuggare l'array di players. */
#define DEBUG_PLAYERS(players, size)                                                \
printf("STARTING DEBUG\n");                                                         \
int m;                                                                              \
for (m = 0; m < size; ++m)                                                          \
{                                                                                   \
    printf("[%d] %d %s\n", m, GET(players, m)->identifier, GET(players, m)->name);  \
}                                                                                   \
print_blank();

#define SWAP_PTRS(type, x, y)   \
type temp = *x;                 \
*x = *y;                        \
*y = temp;

#define FREE_MATRIX(counter, size, matrix)          \
for (counter = 0; counter < size; ++counter)        \
{                                                   \
    free(matrix[counter]);                          \
}                                                   \
free(matrix);

// Non mi piace usare "a->b[c][d]", preferisco "&(*a->b)[d]" :)
#define GET(matrix, item) (&(*matrix)[item])

// Numero di nuove linee utilizzate per pulire lo schermo usando "println_wqt()".
#define CLEAR_SCREEN_LINES 24

// Una semplice stringa vuota.
#define EMPTY_STRING ""

// Il nome del FrontMan dello SPR1D-GAME.
#define FRONTMAN_NAME "Riccardo Scateni"

/*
 * Valori per continuare o fermare l'inserimento di nuovi profili.
 */
#define ABORT_INSERT_VALUE 0
#define KEEP_INSERT_VALUE 1
#define KEEP_ABORT_VALUES 2

// Definisce il valore usato per l'inserimento di nuovi profili.
#define USED_VALUE (-999)

// Il valore dell'id dei profili che non stanno giocando.
#define NOT_PLAYING_ID (-1)

/* Struttura indicante le opzioni possibili dell'aggiornamento statistiche. */
typedef enum
{
    SPR1D_WON, SPR1D_PLAYED,
    GAMES_WON, GAMES_PLAYED,
    FINALS_PLAYED
} StatsUpdate;

void play_game(Games curGame, Group *groups, int rows, int size, int *sizes, Game *game);
void play_games(Game *game);

void incr_stats(Game *game, StatsUpdate statsUpdate);

Games front_man_chooses(Games *gamesRemaining);

void clear_screen();
void print_blank();
void print_fiorellini();
void print_game_new();
void println(char *text);
void println_wqt(char *text, int newLines);
void print_err(InputErr inputErr);
void print_start_match(Group group, int size);
void print_end_match(Player *player);
void print_new_game(Games game);
void print_remaining_games(Games *games);
void print_player_name(Player *player);
void print_profile_name(Profile *profile);
void print_groups(Group *groups, int rows, int columns);
void print_winner(Game *game);
void print_winners(Game *game);
void print_tablike(Player *player);

int ask_input_int(int *allowed, int allowedSize, int inputLength, int denyMinusThan);
char *ask_input_str(char *allowed, char *print);
char *ask_input_str_with_len(char *allowed, char *print, int maxLength);
int ask_menu_choice(char *text[], int rows, int len);

void ask_which_profile_plays(Game *game);

Boolean is_num_odd(int num);
Boolean is_game_quad(Games game);
Boolean is_human(Player *player);
Boolean is_front_man(Player *player);

Game init_game();
void update_totals(Game *game, ToUpdate toUpdate);

#endif //FINAL_SHORTCUTS_H
