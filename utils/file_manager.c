#include "../utils/common.h"
#include "../utils/shortcuts.h"
#include "file_manager.h"

/**
 * Chiede all'utente se vuole salvare lo stato del game.
 * @param gameStatus Lo stato corrente del game.
 */
void ask_save_stats(GameStatus *gameStatus)
{
    /*
     * allowed  -> i valori ammess in input.
     *
     * res      -> il valore scelto dall'utente.
     */
    static int allowed[SAVE_ACTIONS] = { ACTION_NO, ACTION_YES };

    Boolean res;

    printf("\nDesideri salvare i dati? (%d = no, %d = si)\n", ACTION_NO, ACTION_YES);
    res = ask_input_int(allowed, SAVE_ACTIONS, LENGTH_ONE, false);

    if (res)
    {
        saveGameStatus(gameStatus);
    }
}

/**
 * Chiede all'utente il nome del file dove salvare i dati.
 * @return Il nome del file da salvare.
 */
char *ask_where_to_save()
{
    /*
     * fileName     -> il nome del file di salvataggio.
     * saveFilePath -> la path al file di salvataggio.
     */
    char *fileName      = CALLOC_ARRAY(char, MAX_FILE_NAME); CRASH_IF_NULL(fileName)
    char *saveFilePath  = CALLOC_ARRAY(char, SAVE_PATH_LEN + MAX_FILE_NAME); CRASH_IF_NULL(saveFilePath)

    fileName = ask_input_str(ALLOWED_CHARS, PRINT_SAVE_NAME);

    /* Costruisce la path del file di salvataggio. */
    strcat(saveFilePath, fileName);
    strcat(saveFilePath, SAVE_EXTENSION);

    free(fileName);

    return saveFilePath;
}

/**
 * Chiede all'utente quale salvataggio caricare.
 * @return Il nome del file da caricare.
 */
char *ask_where_to_load()
{
    /*
     * i            -> contatore.
     * input        -> il valore scelto dall'utente.
     * counter      -> il numero di file di salvataggio presenti.
     * allowed      -> i numeri ammessi in input.
     *
     * d            -> utile per leggere il meta-data di ogni file di salvataggio.
     *
     * res          -> il nome del file di salvataggio scelto dall'utente.
     * fileNames    -> i nomi dei file di salvataggio presenti.
     *
     * dir          -> la directory di salvataggio.
     */
    int i;
    int input;
    int counter = 0;
    int *allowed;

    struct dirent *d;

    char *res           = NULL;
    char **fileNames    = CALLOC_ARRAY(char *, 1); CRASH_IF_NULL(fileNames)

    DIR *dir = opendir(SAVE_PATH); CRASH_IF_NULL(dir)

    println_wqt(EMPTY_STRING, 2);
    print_fiorellini();

    println("Inserisci il numero del file di salvataggio che desideri caricare:");

    /* Prende ogni file dalla directory di salvataggio. */
    while ((d = readdir(dir)) != NULL)
    {
        /* Ignora ogni file che inizia per "." e "..", ovvero current-folder e previous-folder. */
        if (d->d_name[0] != IGNORE_DIR)
        {
            fileNames[counter] = CALLOC_ARRAY(char, MAX_FILE_NAME); CRASH_IF_NULL(fileNames[counter])

            strcpy(fileNames[counter], d->d_name);
            printf("\t%d |> %s\n", counter, fileNames[counter]);

            fileNames = REALLOC_ARRAY(char *, fileNames, ++counter + 1); CRASH_IF_NULL(fileNames)
        }
    }

    closedir(dir);
    print_fiorellini();

    if (counter > 0)
    {
        allowed = MALLOC_ARRAY(int, counter); CRASH_IF_NULL(allowed)

        for (i = 0; i < counter; ++i)
        {
            allowed[i] = i;
        }

        res     = CALLOC_ARRAY(char, MAX_FILE_NAME); CRASH_IF_NULL(res)
        input   = ask_input_int(allowed, counter, LENGTH_MAX, false);

        strcpy(res, fileNames[input]);

        free(allowed);
    }
    else
    {
        println("Non sono presenti salvataggi!");
    }

    FREE_MATRIX(i, counter, fileNames)

    return res;
}

/**
 * Legge un file di salvataggio.
 */
void readGameStatus(GameStatus *gameStatus)
{
    int i;
    int totHumans;
    int totPlayers;
    int **humanIds       = &gameStatus->humanIds;
    int **playerStatuses = &gameStatus->playerStatuses;

    FILE *file = NULL;

    char *fileName      = ask_where_to_load();
    char *saveFilePath  = NULL;

    if (fileName != NULL)
    {
        saveFilePath = CALLOC_ARRAY(char, SAVE_PATH_LEN + MAX_FILE_NAME); CRASH_IF_NULL(saveFilePath)

        /* Costruisce la path del file di salvataggio. */
        strcpy(saveFilePath, SAVE_PATH);
        strcat(saveFilePath, fileName);

        file = fopen(saveFilePath, READ_MODE); CRASH_IF_NULL(file)

        // Legge il numero di profili presenti nel file.
        fread(&gameStatus->numProfiles, SIZEOF_INT, READ_ONE, file);

        /* Crea i profili. */
        *gameStatus->profiles = MALLOC_ARRAY(Profile, gameStatus->numProfiles); CRASH_IF_NULL(*gameStatus->profiles)

        fread(*gameStatus->profiles, SIZEOF_PROFILE, gameStatus->numProfiles, file);
        fread(&gameStatus->playing,  SIZEOF_INT, READ_ONE, file);

        /* Se il gioco e' in corso, prende le statistiche relative. */
        if (gameStatus->playing)
        {
            fread(&totPlayers,  SIZEOF_INT, READ_ONE, file);
            fread(&totHumans,   SIZEOF_INT, READ_ONE, file);

            gameStatus->totPlayers  = totPlayers;
            gameStatus->totHumans   = totHumans;

            if (totHumans > 1)
            {
                *humanIds = REALLOC_ARRAY(int, *humanIds, totHumans); CRASH_IF_NULL(*humanIds)
            }
            fread(*humanIds, SIZEOF_INT, totHumans, file);

            if (totPlayers > 1)
            {
                *playerStatuses = REALLOC_ARRAY(int, *playerStatuses, totPlayers); CRASH_IF_NULL(*playerStatuses)
            }
            fread(*playerStatuses, SIZEOF_INT, totPlayers, file);
        }

        fclose(file);
        free(saveFilePath);
    }

    free(fileName);

    println("I dati sono stati caricati!");
}

/**
 * Stampa a video lo stato del game.
 * @param gameStatus Lo stato del game.
 */
void print_game_status(GameStatus *gameStatus)
{
    int i;

    Profile profile;

    print_blank();
    print_fiorellini();

    printf("Numero profili giocatore: %d\n",    gameStatus->numProfiles);
    printf("Partita in corso: %s\n",            gameStatus->playing ? "si" : "no");

    println("\nLista dei profili:\n");

    for (i = 0; i < gameStatus->numProfiles; ++i)
    {
        profile = (*gameStatus->profiles)[i];

        printf("\t");
        print_profile_name(&profile);
        printf("\n\tSPR1D-GAME vinti: %d\n",    profile.spr1dWon);
        printf("\tSPR1D-GAME giocati: %d\n",    profile.spr1dPlayed);
        printf("\tFinali giocate: %d\n",        profile.finalsPlayed);
        printf("\tTotale giochi giocati: %d\n", profile.gamesPlayed);
        printf("\tTotale giochi vinti: %d\n\n", profile.gamesWon);
    }

    if (gameStatus->playing)
    {
        printf("Numero di player totali: %d\n", gameStatus->totPlayers);
        printf("Numero di umani totali: %d\n",  gameStatus->totHumans);

        println("\nIndici umani:");

        for (i = 0; i < gameStatus->totHumans; ++i)
        {
            printf("%d ", gameStatus->humanIds[i]);
        }

        println("\nStatus dei players:");

        for (i = 0; i < gameStatus->totPlayers; ++i)
        {
            printf("%d ", gameStatus->playerStatuses[i]);
        }
    }

    print_blank();
    print_fiorellini();
    print_blank();
}

/**
 * Salva i dati del gioco.
 * @param gameStatus Lo stato del game.
 */
void saveGameStatus(GameStatus *gameStatus)
{
    FILE *file = NULL;

    char *fileName      = ask_where_to_save();
    char *saveFilePath  = CALLOC_ARRAY(char, SAVE_PATH_LEN + MAX_FILE_NAME); CRASH_IF_NULL(saveFilePath)

    /* Costruisce la path del file di salvataggio. */
    strcat(saveFilePath, SAVE_PATH);
    strcat(saveFilePath, fileName);

    file = fopen(saveFilePath, WRITE_BIN); CRASH_IF_NULL(file)

    fwrite(&gameStatus->numProfiles,    SIZEOF_INT, READ_ONE, file);
    fwrite(*gameStatus->profiles,       SIZEOF_PROFILE, gameStatus->numProfiles, file);
    fwrite(&gameStatus->playing,        SIZEOF_INT, READ_ONE, file);

    /* Se il gioco e' in corso, salva le statistiche relative. */
    if (gameStatus->playing)
    {
        fwrite(&gameStatus->totPlayers,     SIZEOF_INT, READ_ONE, file);
        fwrite(&gameStatus->totHumans,      SIZEOF_INT, READ_ONE, file);
        fwrite(gameStatus->humanIds,        SIZEOF_INT, gameStatus->totHumans, file);
        fwrite(gameStatus->playerStatuses,  SIZEOF_INT, gameStatus->totPlayers, file);
    }

    fclose(file);
    free(fileName);
    free(saveFilePath);

    println("I dati sono stati salvati!");
}

/**
 * Legge il dizionario dal file.
 * @param fileDictionary Il dizionario.
 */
void readDictionary(FileDictionary *fileDictionary)
{
    /*
     * i, j     -> contatori.
     * rows     -> il numero di righe rappresentante il contenuto del dizionario.
     *
     * file     -> il file da leggere.
     * contents -> il contenuto del file.
     */
    int i, j;
    int rows;
    int length;

    FILE *file = fopen(fileDictionary->name, READ_MODE); CRASH_IF_NULL(file)

    char *contents = CALLOC_ARRAY(char, MAX_ENTRY_LENGTH); CRASH_IF_NULL(contents)

    // Legge la prima riga del file.
    fgets(contents, MAX_ENTRY_LENGTH, file);

    rows = AS_INT(contents);

    if (rows > MAX_DICTIONARY_ENTRIES)
    {
        rows = MAX_DICTIONARY_ENTRIES;
    }

    /* Legge il resto delle righe del file. */
    for (i = 1; i < rows; ++i)
    {
        fgets(contents, MAX_ENTRY_LENGTH, file);

        REVERSE_FOR(j, strlen(contents), length, contents)

        /* Rimuove caratteri indesiderati. */
        if (contents[length - 1] == RESET_LINE)
        {
            contents[length - 1] = NO_LINE;
        }
        contents[length] = NO_LINE;

        // Salva il contenuto della riga letta.
        strcpy(fileDictionary->contents[i - 1], contents);
    }

    fclose(file);
    free(contents);

    fileDictionary->rows = rows - 2;
}