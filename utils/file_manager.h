#ifndef FINAL_FILE_MANAGER_H
#define FINAL_FILE_MANAGER_H

// L'estensione da utilizzare per i salvataggi.
#define SAVE_EXTENSION ".bin"

/*
 * Utility sizes.
 */
#define SIZEOF_INT sizeof(int)
#define SIZEOF_PROFILE sizeof(Profile)

// Quante entries leggere da file.
#define READ_ONE 1

/*
 * Caratteri utility.
 */
#define NEW_LINE '\n'
#define NO_LINE '\0'
#define RESET_LINE '\r'

// Le directory da ignorare.
#define IGNORE_DIR '.'

/*
 * Caratteri massimi di: nome files, ogni entry del dizionario.
 */
#define MAX_FILE_NAME 256
#define MAX_ENTRY_LENGTH 120

// Il numero massimo di entries del dizionario.
#define MAX_DICTIONARY_ENTRIES 50

/*
 * Modalita' di apertura dei files.
 */
#define READ_MODE "r"
#define WRITE_BIN "wb"
#define WRITE_MODE "w"

/*
 * Le varie path utilizzate per la gestione dei files.
 */
#define DEFAULT_DICTIONARY "../dictionary/default.txt"
#define CUSTOM_DICTIONARY "../dictionary/custom.txt"
#define SAVE_PATH "../saves/"
#define SAVE_PATH_LEN strlen(SAVE_PATH)

/*
 * Numero di azioni e azioni possibili.
 */
#define SAVE_ACTIONS 2
#define ACTION_NO 0
#define ACTION_YES 1

#define REVERSE_FOR(counter, var, res, element)     \
for (counter = var; counter >= 0; --counter)        \
{                                                   \
    if (element[counter] == NEW_LINE)               \
    {                                               \
        res = counter;                              \
        counter = 0;                                \
    }                                               \
}

typedef struct
{
    int     rows;
    char    *name;
    char    **contents;
} FileDictionary;

void readGameStatus(GameStatus *gameStatus);

void saveGameStatus(GameStatus *gameStatus);

void readDictionary(FileDictionary *fileDictionary);

void print_game_status(GameStatus *gameStatus);

void ask_save_stats(GameStatus *gameStatus);

#endif //FINAL_FILE_MANAGER_H
