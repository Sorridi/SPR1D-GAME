#include "common.h"
#include "shortcuts.h"

/**
 * Genera un numero casuale in un determinato range X->Y (inclusi).
 * @param min   Estremo minore.
 * @param max   Estremo maggiore.
 * @return      Il numero casuale generato.
 */
int gen_num(int min, int max)
{
    return min + rand() % (max - min + 1);
}

/**
 * Genera i gruppi per i giochi.
 * @param players   La lista dei player.
 * @param size      La dimensione della lista dei player.
 * @param groupSize La dimensione dei gruppi da generare.
 * @return          Array di gruppi di player.
 */
Player **gen_groups(Player *players, int size, int groupSize)
{
    /*
     * i, j         -> contatori.
     * rows         -> il numero di righe della matrice, ovvero il numero di player per gruppo.
     * counter      -> contatore per scegliere l'index degli IDs.
     * shuffledIDs  -> l'array d'IDs ordinato casualmente.
     */
    int i, j;
    int rows            = size / groupSize;
    int counter         = 0;
    int *shuffledIds    = shuffle_players(size);

    Player **groups = MALLOC_ARRAY(Player *, rows); CRASH_IF_NULL(groups)

    /* Utilizzando l'array casualmente ordinato, crea i gruppi casuali. */
    for (i = 0; i < rows; ++i)
    {
        groups[i] = gen_group(players, groupSize, shuffledIds, &counter);
    }

    free(shuffledIds);

    return groups;
}

/**
 * Genera un gruppo per i giochi.
 * @param players       La lista dei player.
 * @param size          La dimensione del gruppo.
 * @param shuffledIds   L'array d'IDs ordinato casualmente.
 * @param counter       Contatore per scegliere l'index degli IDs.
 * @return              Il gruppo creato.
 */
Player *gen_group(Player *players, int size, int *shuffledIds, int *counter)
{
    /*
     * i        -> contatore.
     *
     * group    -> il nuovo gruppo di players.
     */
    int i;

    Player *group = MALLOC_ARRAY(Player, size); CRASH_IF_NULL(group)

    /* Utilizzando il counter come index, prende un ID casuale dalla lista, esso sara' uno dei player del gruppo. */
    for (i = 0; i < size; ++i, ++(*counter))
    {
        group[i] = players[shuffledIds[*counter]];
    }

    return group;
}

/**
 * Controlla il nome del player da inserire.
 * @param players   La lista dei player.
 * @param size      La dimensione della lista dei player.
 * @param name      Il nome del player da controllare.
 * @return          Se il nome e' valido (true) o meno (false).
 */
Boolean check_profile_entry(Profile *profiles, int size, char *name)
{
    // Contatore.
    int i;

    /* Controlla la validita' dei caratteri del nome. */
    if (!check_characters(name, ALLOWED_CHARS))
    {
        print_err(ILLEGAL_CHARS);
        return false;
    }

    /* Controlla la lunghezza del nome. */
    if (strlen(name) > MAX_CARATTERI)
    {
        print_err(TOO_MANY_CHARS);
        return false;
    }

    /* Controlla se il nome e' gia' occupato da altri player. */
    for (i = 0; i < size; ++i)
    {
        if (strcmp(name, profiles[i].name) == 0)
        {
            print_err(PLAYER_EXISTS);
            return false;
        }
    }

    return true;
}

/**
 * Crea un array di indexes casualmente ordinato che verra' utilizzato per creare i gruppi di players.
 * @param size  La grandezza dell'array.
 * @return      L'array casualmente ordinato.
 */
int *shuffle_players(int size)
{
    /*
     * i, j             -> contatori.
     * generated        -> il valore generato.
     * usedSize         -> la grandezza dell'array dei numeri gia' generati.
     * used             -> array di numeri gia' utilizzati.
     *
     * valueExists      -> indica se un numero e' gia' stato utilizzato in precedenza.
     */
    int i, j;
    int generated;
    int usedSize    = 0;
    int *used       = NULL;

    Boolean valueExists = false;

    /* Genera un numero casuale per ogni player. */
    for (i = 0; i < size; ++i)
    {
        generated = gen_num(0, size - 1);

        /* Controlla se il numero generato era gia' stato utilizzato in precedenza. */
        for (j = 0; j < usedSize; ++j)
        {
            /* In tal caso incrementa il counter. */
            if (generated == used[j])
            {
                valueExists = true;
                j = usedSize;
            }
        }

        /* Se il numero generato non era mai stato usato prima d'ora.  */
        if (!valueExists)
        {
            /*
             * Aggiunge una posizione all'array d'indexes utilizzati e ne assegna un elemento.
             */
            if (usedSize++ == 0)
            {
                used = MALLOC_ARRAY(int, usedSize); CRASH_IF_NULL(used)
            }
            else
            {
                used = REALLOC_ARRAY(int, used, usedSize); CRASH_IF_NULL(used)
            }

            used[usedSize - 1] = generated;
        }
        else
        {
            valueExists = false;
            i--;
        }
    }

    return used;
}

/**
 * Inserisce un nuovo player nella lista dei player.
 * @param players   La lista dei player correnti.
 * @param size      La dimensione della lista dei player.
 * @param name      Il nome del nuovo player da inserire.
 */
void insert_player(Player **players, int *size, char *name, Boolean verbose)
{
    /*
     * pre      -> la size del precedente insert.
     *
     * player   -> il nuovo player da inserire.
     */
    int pre;

    Player player;

    if ((*size)++ == 0)
    {
        *players = MALLOC_ARRAY(Player, *size); CRASH_IF_NULL(*players)
    }
    else
    {
        *players = REALLOC_ARRAY(Player, *players, *size); CRASH_IF_NULL(*players)
    }

    pre = *size - 1;

    /* Inizializzo i dati del player. */
    player.name         = name;
    player.identifier   = pre;

    (*players)[pre] = player;

    if (verbose)
    {
        printf("INSERITO: ");
        print_player_name(&player);
        print_blank();
    }
}

/**
 * Inserisce un nuovo player nella lista dei player.
 * @param profiles  La lista dei profili correnti.
 * @param size      La dimensione della lista dei player.
 * @param name      Il nome del nuovo player da inserire.
 */
void insert_profile(GameStatus *gameStatus, char *name)
{
    /*
     * pre      -> la size del precedente insert.
     *
     * profile  -> il nuovo profilo da inserire.
     */
    int pre;

    Profile profile;
    Profile *profiles = *gameStatus->profiles;

    /* Se il player non e' un bot, e' necessario eseguire vari checks sul nome che l'utente ha fornito in input. */
    if (check_profile_entry(profiles, gameStatus->numProfiles, name))
    {
        if (gameStatus->numProfiles++ == 0)
        {
            profiles = MALLOC_ARRAY(Profile, gameStatus->numProfiles); CRASH_IF_NULL(profiles)
        }
        else
        {
            profiles = REALLOC_ARRAY(Profile, profiles, gameStatus->numProfiles); CRASH_IF_NULL(profiles)
        }

        pre = gameStatus->numProfiles - 1;

        /* Inizializzo i dati del player. */
        strcpy(profile.name, name);
        profile.identifier      = -1;
        profile.spr1dWon        = 0;
        profile.spr1dPlayed     = 0;
        profile.gamesPlayed     = 0;
        profile.gamesWon        = 0;
        profile.finalsPlayed    = 0;

        profiles[pre] = profile;
        *gameStatus->profiles = profiles;

        printf("INSERITO PROFILO: ");
        print_profile_name(&profile);
        print_blank();
    }
}

/**
 * Controlla nel gruppo se e' presente il FrontMan dello SPR1D GAME.
 * @param group Il gruppo da controllare.
 * @param size  La grandezza del gruppo.
 * @return      Il FrontMan in caso sia presente, altrimenti, NULL.
 */
Player* find_front_man(Player *group, int size)
{
    /*
     * i        -> contatore
     *
     * player   -> player temporaneo.
     * frontMan -> il player "Riccardo Scateni".
     */
    int i;

    Player *player      = NULL;
    Player *frontMan    = NULL;

    /* Controlla tutto il gruppo alla ricerca del FrontMan. */
    for (i = 0; i < size && frontMan == NULL; ++i)
    {
        player = &group[i];

        if (is_front_man(player))
        {
            frontMan = player;
        }
    }

    return frontMan;
}

/**
 * Controlla se i caratteri del testo fornito sono ammessi.
 * @param toCheck   Il testo da controllare.
 * @param allowed   La stringa contenente tutti i caratteri ammessi.
 * @return          Se i caratteri sono ammessi (true) o meno (false).
 */
Boolean check_characters(char *toCheck, char *allowed)
{
    /*
     * i, j         -> contatori.
     * counter      -> contatore di caratteri ammessi trovati.
     * stringLen    -> lunghezze della stringa da controllare.
     * allowedLen   -> lunghezza della stringa con i caratteri ammessi.
    */
    int i, j;
    unsigned counter    = 0;
    unsigned toCheckLen = strlen(toCheck);
    unsigned allowedLen = strlen(allowed);

    if (toCheckLen == 0)
    {
        return false;
    }

    /* Compara ogni carattere da controllare con quelli ammessi. */
    for (i = 0; i < toCheckLen && toCheck[i] != STR_TERM; ++i)
    {
        for (j = 0; j < allowedLen; ++j)
        {
            /* Se il carattere ammesso è stato trovato, incrementa il counter. */
            if (toCheck[i] == allowed[j])
            {
                counter++;
            }
        }
    }

    return counter == toCheckLen;
}

/**
 * Ordina un array di interi utilizzando l'implementazione vista in ASD del selection sort.
 * @param toSort    L'array da ordinare.
 * @param elements  Il numero di elementi dell'array.
 */
void selection_sort(int *toSort, int elements)
{
    int i, j;
    int min;

    for (i = 0; i < elements - 1; ++i)
    {
        min = i;

        for (j = i + 1; j < elements; ++j)
        {
            if (toSort[j] < toSort[min])
            {
                min = j;
            }
        }
        SWAP_PTRS(int, &toSort[min], &toSort[i]);
    }
}

/**
 * Controlla se un gruppo e' composto da solo bots.
 * @param group Il gruppo.
 * @param size  La grandezza del gruppo.
 * @return      Se il gruppo e' composto da soli bots (true) o meno (false).
 */
Boolean group_full_of_bots(Player *group, int size)
{
    // Contatore.
    int i;

    /* Controlla ogni player del gruppo in cerca di un umano, */
    for (i = 0; i < size; ++i)
    {
        if (is_human(&group[i]))
        {
            return false;
        }
    }

    return true;
}

/**
 * Aggiorna gli stati dei player.
 * @param players       I players ancora in vita.
 * @param playerSize    Il numero di players.
 * @param statuses      Gli status di tutti i player che hanno giocato finora.
 * @param size          Il numero di status.
 */
void update_player_statuses(Player *winners, int playerSize, int *statuses, int size)
{
    // Contatore.
    int i;

    /* Resetto tutti gli status. */
    for (i = 0; i < size; ++i)
    {
        statuses[i] = STATUS_ELIMINATED;
    }

    /* Imposto gli status nuovi ai player che sono ancora in vita. */
    for (i = 0; i < playerSize; ++i)
    {
        statuses[winners[i].identifier] = STATUS_ALIVE;
    }
}

/**
 * Incrementa le varie statistiche dei vincitori.
 * @param gameStatus    Lo stato corrente del game.
 * @param winners       I vincitori.
 * @param numWinners    Il numero di vincitori.
 * @param isFinal       Se e' la finale (true) o meno (false).
 */
void incr_stats_winners(GameStatus *gameStatus, Player *winners, int numWinners, Boolean isFinal)
{
    int i, j;
    int counter = 0;

    Profile *profile;

    /* Filtra i profili vincitori e incrementa le loro statistiche. */
    for (i = 0; i < gameStatus->numProfiles; ++i)
    {
        profile = &(*gameStatus->profiles)[i]; // non mi piace usare "a->b[c][d]", preferisco "&(*a->b)[d]" :)

        for (j = 0; j < numWinners; ++j)
        {
            if (winners[j].identifier == profile->identifier)
            {
                counter++;
                profile->gamesWon++;

                if (isFinal)
                {
                    profile->spr1dWon++;
                }
            }
        }
    }

    gameStatus->totHumans = counter;
}

/**
 * Aggiorna il numero di umani e players in gioco.
 * @param players       I players.
 * @param numPlayers    Il numero di players.
 * @param gameStatus    Lo stato corrente del gioco.
 */
void update_totals(Player *players, int numPlayers, GameStatus *gameStatus)
{
    /*
     * i, j     -> contatori.
     * counter  -> numero totale di umani.
     */
    int i, j;

    int counter = 0;

    for (i = 0; i < gameStatus->numProfiles; ++i)
    {
        for (j = 0; j < numPlayers; ++j)
        {
            if ((*gameStatus->profiles)[i].identifier == players[j].identifier)
            {
                counter++;
            }
        }
    }

    gameStatus->totHumans   = counter;
    gameStatus->totPlayers  = numPlayers;
}

/**
 * Incrementa le varie statistiche di tutti i players.
 * @param gameStatus    Lo stato corrente del game.
 * @param players       I players.
 * @param numPlayers    Il numero di players.
 * @param isFinal       Se e' la finale (true) o meno (false).
 */
void incr_stats(GameStatus *gameStatus, Player *players, int numPlayers, Boolean isFinal)
{
    int i, j;

    Profile *profile;

    /* Filtra i profili player e incrementa le loro statistiche. */
    for (i = 0; i < gameStatus->numProfiles; ++i)
    {
        profile = &(*gameStatus->profiles)[i]; // non mi piace usare "a->b[c][d]", preferisco "&(*a->b)[d]" :)

        for (j = 0; j < numPlayers; ++j)
        {
            if (players[j].identifier == profile->identifier)
            {
                profile->gamesPlayed++;

                if (isFinal)
                {
                    profile->finalsPlayed++;
                }
            }
        }
    }
}