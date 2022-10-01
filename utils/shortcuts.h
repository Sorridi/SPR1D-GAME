#ifndef FINAL_SHORTCUTS_H
#define FINAL_SHORTCUTS_H

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

#define DEBUG_VARS(x, y) printf("x -> %x | y -> %x\n", &x, &y);
#define DEBUG_PTRS(x, y) printf("x -> %x | y -> %x\n", x, y);

#define SWAP_PTRS(type, x, y)   \
type temp = *x;                 \
*x = *y;                        \
*y = temp;

#define FOR_PRINT(counter, size, var)               \
println("\nSTART FOR-PRINT");                       \
for (counter = 0; counter < size; ++counter)        \
{                                                   \
    printf("%s %x\n", var[counter], &var[counter]); \
}                                                   \
println("END FOR-PRINT\n");

#define FOR_PRINT_PLAYERS(counter, size, var)               \
println("\nSTART FOR-PRINT-PLAYERS");                       \
for (counter = 0; counter < size; ++counter)                \
{                                                           \
    print_player_name(var);                                 \
    print_blank();                                          \
}                                                           \
println("END FOR-PRINT-PLAYERS\n");

#define FREE_MATRIX(counter, size, matrix)          \
for (counter = 0; counter < size; ++counter)        \
{                                                   \
    free(matrix[counter]);                          \
}                                                   \
free(matrix);

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

Player *play_game(Games game, Games *games, Player **groups, int rows, int size, int *sizes);
void play_games(Player *startPlayers, int currentSize, int startSize, GameStatus *gameStatus, Player *eliminatedPlayers);

Games front_man_chooses(Games *gamesRemaining);

void clear_screen();
void print_blank();
void print_fiorellini();
void println(char *text);
void println_wqt(char *text, int newLines);
void print_err(InputErr inputErr);
void print_start_match(Player *players, int size);
void print_end_match(Player *player);
void print_new_game(Games game);
void print_remaining_games(Games *games);
void print_player_name(Player *player);
void print_profile_name(Profile *profile);
void print_groups(Player **groups, int rows, int columns);
void print_winner(Player *winner);
void print_winners(Player *winners, int size);
void print_tablike(Player *player);

int ask_input_int(int *allowed, int allowedSize, int inputLength, int denyMinusThan);
char *ask_input_str(char *allowed, char *print);
int ask_menu_choice(char *text[], int rows, int len);

int *ask_which_profile_plays(GameStatus *gameStatus);

Boolean is_num_odd(int num);
Boolean is_game_quad(Games game);
Boolean is_human(Player *player);
Boolean is_front_man(Player *player);

#endif //FINAL_SHORTCUTS_H
