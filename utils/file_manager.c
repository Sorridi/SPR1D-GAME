#include "../utils/common.h"
#include "../utils/shortcuts.h"
#include "file_manager.h"

/**
 * Chiede all'utente se vuole salvare lo stato del game.
 * @param status Lo stato corrente del game.
 */
void ask_save_stats(Status *status)
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
        save_game_status(status);
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
    char *fileName      = NULL;
    char *saveFilePath  = CALLOC_ARRAY(char, SAVE_PATH_LEN + MAX_FILE_NAME); CRASH_IF_NULL(saveFilePath)

    fileName = ask_input_str_with_len(ALLOWED_CHARS, PRINT_SAVE_NAME, MAX_FILE_NAME);

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

    /* Se sono presenti file di salvataggio, chiede all'utente quale desidera scegliere. */
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
 * Legge un file di salvataggio e carica i dati.
 * @param game Il gioco nel quale caricare i dati.
 */
void read_game_status(Game *game)
{
    /*
     * status           -> lo stato del gioco.
     *
     * totHumans        -> il numero totale di profili in gioco.
     * totPlayers       -> il numero totale di players in gioco.
     * humanIds         -> gli indici umani
     * playerStatuses   -> gli stati dei players.
     *
     * file             -> il file da leggere.
     *
     * fileName         -> il nome del file.
     * saveFilePath     -> la path del file.
     */
    Status *status = &game->status;

    int totHumans;
    int totPlayers;
    int **humanIds       = &status->humanIds;
    int **playerStatuses = &status->playerStatuses;

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
        fread(&status->numProfiles, SIZEOF_INT, READ_ONE, file);

        /* Crea i profili. */
        *status->profiles = MALLOC_ARRAY(Profile, status->numProfiles); CRASH_IF_NULL(*status->profiles)

        fread(*status->profiles, SIZEOF_PROFILE, status->numProfiles, file);
        fread(&status->playing,  SIZEOF_INT, READ_ONE, file);

        /* Se il gioco e' in corso, prende le statistiche relative. */
        if (status->playing)
        {
            fread(&totPlayers,  SIZEOF_INT, READ_ONE, file);
            fread(&totHumans,   SIZEOF_INT, READ_ONE, file);

            status->totPlayers  = totPlayers;
            status->totHumans   = totHumans;

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

        println("I dati sono stati caricati!");
    }
    else
    {
        print_err(SYSTEM_ERROR);
    }

    free(fileName);
}

/**
 * Stampa a video lo stato del game.
 * @param game Il gioco del quale stampare a video i dati.
 */
void print_game_status(Game *game)
{
    /*
     * i        -> contatore.
     *
     * status   -> lo stato del game.
     *
     * profile  -> valore temporaneo.
     */
    int i;

    Status status = game->status;

    Profile profile;

    print_blank();
    print_fiorellini();

    printf("Numero profili giocatore: %d\n",    status.numProfiles);
    printf("Partita in corso: %s\n",            status.playing ? "si" : "no");

    println("\nLista dei profili:\n");

    for (i = 0; i < status.numProfiles; ++i)
    {
        profile = (*status.profiles)[i];

        printf("\t");
        print_profile_name(&profile);
        printf("\n\tSPR1D-GAME vinti: %d\n",    profile.spr1dWon);
        printf("\tSPR1D-GAME giocati: %d\n",    profile.spr1dPlayed);
        printf("\tFinali giocate: %d\n",        profile.finalsPlayed);
        printf("\tTotale giochi giocati: %d\n", profile.gamesPlayed);
        printf("\tTotale giochi vinti: %d\n\n", profile.gamesWon);
    }

    if (status.playing)
    {
        printf("Numero di player totali: %d\n", status.totPlayers);
        printf("Numero di umani totali: %d\n",  status.totHumans);

        println("\nIndici umani:");

        for (i = 0; i < status.totHumans; ++i)
        {
            printf("%d ", status.humanIds[i]);
        }

        println("\nStatus dei players:");

        for (i = 0; i < status.totPlayers; ++i)
        {
            printf("%d ", status.playerStatuses[i]);
        }
    }

    print_blank();
    print_fiorellini();
    print_blank();
}

/**
 * Salva i dati del gioco.
 * @param status Lo stato del game.
 */
void save_game_status(Status *status)
{
    /*
     * file         -> file di salvataggio.
     *
     * fileName     -> il nome del file.
     * saveFilePath -> la path del file.
     */
    FILE *file = NULL;

    char *fileName      = ask_where_to_save();
    char *saveFilePath  = CALLOC_ARRAY(char, SAVE_PATH_LEN + MAX_FILE_NAME); CRASH_IF_NULL(saveFilePath)

    /* Costruisce la path del file di salvataggio. */
    strcat(saveFilePath, SAVE_PATH);
    strcat(saveFilePath, fileName);

    file = fopen(saveFilePath, WRITE_BIN); CRASH_IF_NULL(file)

    fwrite(&status->numProfiles,    SIZEOF_INT, READ_ONE, file);
    fwrite(*status->profiles,       SIZEOF_PROFILE, status->numProfiles, file);
    fwrite(&status->playing,        SIZEOF_INT, READ_ONE, file);

    /* Se il gioco e' in corso, salva le statistiche relative. */
    if (status->playing)
    {
        fwrite(&status->totPlayers,     SIZEOF_INT, READ_ONE, file);
        fwrite(&status->totHumans,      SIZEOF_INT, READ_ONE, file);
        fwrite(status->humanIds,        SIZEOF_INT, status->totHumans, file);
        fwrite(status->playerStatuses,  SIZEOF_INT, status->totPlayers, file);
    }

    free(fileName);
    free(saveFilePath);
    fclose(file);

    println("I dati sono stati salvati!");
}

/**
 * Legge il dizionario dal file.
 * @param fileDictionary Il dizionario.
 */
void read_dictionary(FileDictionary *fileDictionary)
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

    // Esclude la prima e l'ultima riga.
    fileDictionary->rows = rows - 2;
}