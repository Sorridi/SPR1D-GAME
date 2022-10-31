#include "common.h"
#include "shortcuts.h"
#include "file_manager.h"

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
 * @param game      Il gioco in corso.
 * @param groupSize La dimensione dei gruppi da generare.
 * @return          Array di gruppi di player.
 */
Group *gen_groups(Game *game, int groupSize)
{
    /*
     * i, j         -> contatori.
     * rows         -> il numero di righe della matrice, ovvero il numero di player per gruppo.
     * counter      -> contatore per scegliere l'index degli IDs.
     * shuffledIDs  -> l'array d'IDs ordinato casualmente.
     */
    int i, j;
    int rows            = game->alivePlayers / groupSize;
    int counter         = 0;
    int *shuffledIds    = shuffle_players(game);

    Group *groups = MALLOC_ARRAY(Group, rows); CRASH_IF_NULL(groups)

    /* Utilizzando l'array casualmente ordinato, crea i gruppi casuali. */
    for (i = 0; i < rows; ++i)
    {
        groups[i] = gen_group(game->players, groupSize, shuffledIds, &counter);
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
Group gen_group(Player **players, int size, int *shuffledIds, int *counter)
{
    /*
     * i        -> contatore.
     *
     * group    -> il nuovo gruppo di players.
     */
    int i;

    Group group;
    group.players = MALLOC_ARRAY(Player, size); CRASH_IF_NULL(group.players)

    /* Utilizzando il counter come index, prende un ID casuale dalla lista, esso sara' uno dei player del gruppo. */
    for (i = 0; i < size; ++i, ++(*counter))
    {
        group.players[i] = (*players)[shuffledIds[*counter]];
    }

    return group;
}

/**
 * Controlla il nome del player da inserire.
 * @param game  La lista dei player.
 * @param name  Il nome del player da controllare.
 * @return      Se il nome e' valido (true) o meno (false).
 */
Boolean check_profile_entry(Game *game, char *name)
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
    for (i = 0; i < game->status.numProfiles; ++i)
    {
        if (strcmp(name, GET(game->status.profiles, i)->name) == 0)
        {
            print_err(PLAYER_EXISTS);
            return false;
        }
    }

    return true;
}

/**
 * Crea un array di indexes casualmente ordinato che verra' utilizzato per creare i gruppi di players.
 * @param game  Il gioco in corso.
 * @return      L'array casualmente ordinato.
 */
int *shuffle_players(Game *game)
{
    /*
     * i, j                     -> contatori.
     * toReplace, replacement   -> valori temporanei.
     * randomized               -> l'array casualmente ordinato.
     */
    int i, j;
    int toReplace, replacement;
    int tempVal;
    int *randomized = MALLOC_ARRAY(int, game->alivePlayers); CRASH_IF_NULL(randomized)

    for (i = 0, j = 0; i < game->status.totPlayers; ++i, ++j)
    {
        if (GET(game->players, i)->alive)
        {
            randomized[j] = i;
        }
        else
        {
            --j;
        }
    }

    for (i = 0; i < PLAYER_SHUFFLES(game->alivePlayers); ++i)
    {
        toReplace   = gen_num(0, game->alivePlayers - 1);
        replacement = gen_num(0, game->alivePlayers - 1);

        tempVal                 = randomized[toReplace];
        randomized[toReplace]   = randomized[replacement];
        randomized[replacement] = tempVal;
    }

    return randomized;
}

/**
 * Inserisce un nuovo player nella lista dei player.
 * @param game      Il gioco in corso.
 * @param name      Il nome del player da inserire.
 * @param verbose   Se stampare a video che il player e' stato inserito (true) o meno (false).
 * @param isAlive   Se il player e' vivo (true) o meno (false).
 */
void insert_player(Game *game, char *name, Boolean verbose, Boolean isAlive)
{
    /*
     * status   -> lo stato del gioco in corso.
     *
     * pre      -> la size del precedente insert.
     * size     -> il numero di players totali.
     *
     * player   -> il nuovo player da inserire.
     * players  -> l'array di players.
     */
    Status *status = &game->status;

    int pre;
    int *size = &status->totPlayers;

    Player player;
    Player **players = game->players;

    // Aggiorna i dati.
    update_totals(game, TOT_PLAYERS);

    if (*size > 1)
    {
        *players = REALLOC_ARRAY(Player, *players, *size); CRASH_IF_NULL(*players)
    }

    pre = *size - 1;

    /* Inizializza i dati del player. */
    player.name         = name;
    player.identifier   = pre;
    player.alive        = isAlive;

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
 * @param game      Il gioco in corso.
 * @param profile   Il profilo del player da inserire.
 * @param verbose   Se stampare a video che il player e' stato inserito (true) o meno (false).
 */
void insert_player_custom(Game *game, Profile *profile, Boolean verbose)
{
    /*
     * status   -> lo stato del gioco in corso.
     *
     * pre      -> la size del precedente insert.
     * size     -> il numero di players totali.
     *
     * player   -> il nuovo player da inserire.
     * players  -> l'array di players.
     */
    Status *status = &game->status;

    int pre;
    int *size = &status->totPlayers;

    Player player;
    Player **players = game->players;

    // Aggiorna i dati.
    update_totals(game, TOT_PLAYERS);

    if (*size > 1)
    {
        *players = REALLOC_ARRAY(Player, *players, *size); CRASH_IF_NULL(*players)
    }

    pre = *size - 1;

    profile->identifier = profile->identifier == NOT_PLAYING_ID ? pre : profile->identifier;

    /* Inizializzo i dati del player. */
    player.name         = profile->name;
    player.identifier   = profile->identifier;
    player.alive        = true;

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
 * @param game  Il gioco in corso.
 * @param name  Il nome del nuovo player da inserire.
 */
void insert_profile(Game *game, char *name)
{
    /*
     * status   -> lo stato del gioco in corso.
     *
     * pre      -> la size del precedente insert.
     * size     -> il numero di players totali.
     *
     * profile  -> il nuovo profilo da inserire.
     * profiles -> l'array di profili.
     */
    Status *status = &game->status;

    int pre;
    int *size = &status->numProfiles;

    Profile profile = { };
    Profile **profiles = status->profiles;

    /* Se il player non e' un bot, e' necessario eseguire vari checks sul nome che l'utente ha fornito in input. */
    if (check_profile_entry(game, name))
    {
        update_totals(game, NUM_PROFILES);

        if (*size == 1)
        {
            *profiles = MALLOC_ARRAY(Profile, *size); CRASH_IF_NULL(*profiles)
        }
        else
        {
            *profiles = REALLOC_ARRAY(Profile, *profiles, *size); CRASH_IF_NULL(*profiles)
        }

        pre = *size - 1;

        /* Inizializzo i dati del player. */
        strcpy(profile.name, name);
        profile.identifier = -1;

        (*profiles)[pre] = profile;

        printf("INSERITO PROFILO: ");
        print_profile_name(&profile);
        print_blank();
    }
}

/**
 * Controlla nel gruppo se e' presente il FrontMan dello SPR1D GAME.
 * @param group Il game in corso.
 * @return      Il FrontMan in caso sia presente, altrimenti, NULL.
 */
Player* find_front_man(Game *game)
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

    for (i = 0; i < game->status.totPlayers && frontMan == NULL; ++i)
    {
        player = GET(game->players, i);

        if (is_front_man(player))
        {
            frontMan = player;
        }
    }

    return frontMan;
}

/**
 * Controlla se nel gruppo e' presente il FrontMan.
 * @param frontMan  Il FrontMan.
 * @param group     Il gruppo da controllare.
 * @param size      La grandezza del gruppo.
 * @return          Se il FrontMan e' presente nel gruppo (true) o meno (false).
 */
Boolean front_man_in_group(Player *frontMan, Group group, int size)
{
    /*
     * i        -> contatore.
     *
     * result   -> il risultato dell'azione.
     */
    int i;

    Boolean result = false;

    if (frontMan != NULL)
    {
        for (i = 0; i < size && !result; ++i)
        {
            if (group.players[i].identifier == frontMan->identifier)
            {
                result = true;
            }
        }
    }

    return result;
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
 * Sorta i profili in modo tale che l'id corrisponda allo slot da loro occupato.
 * @param game Il gioco in corso.
 */
void selection_sort_profiles(Game *game)
{
    /*
     * status       -> lo stato del gioco in corso.
     *
     * i, j, k      -> valori temporanei e contatori.
     * elements     -> il numero di profili totali in gioco.
     *
     * tempProfile  -> valore temporaeno.
     */
    Status *status = &game->status;

    int i, j, k;
    int min;
    int elements = status->totHumans;

    Profile tempProfile;

    /*
     * Sorta gli id dei profili in modo che l'ID del profilo sia associato alla casella occupata da esso
     * nell'array di profili.
     *
     * In modo tale da avere anche l'indice dei profili ordinato.
     */
    for (i = 0; i < status->numProfiles; ++i)
    {
        for (j = i; j < status->numProfiles; ++j)
        {
            tempProfile = (*status->profiles)[j];

            if (tempProfile.identifier != NOT_PLAYING_ID)
            {
                (*status->profiles)[j] = (*status->profiles)[i];
                (*status->profiles)[i] = tempProfile;
                (*game->players)[tempProfile.identifier].name = GET(status->profiles, i)->name;
            }
        }
    }

    /* Sposta ogni profilo non in gioco a destra dell'array, e ogni ID in gioco a sinistra. */
    for (i = 0; i < status->numProfiles - 1; ++i)
    {
        min = i;

        for (j = i + 1; j < status->numProfiles - elements; ++j)
        {
            if ((*status->profiles)[j].identifier < (*status->profiles)[min].identifier)
            {
                min = j;
            }
        }

        k = GET(status->profiles, i)->identifier;
        tempProfile = (*status->profiles)[k];
        (*status->profiles)[k] = (*status->profiles)[i];
        (*status->profiles)[i] = tempProfile;
    }
}

/**
 * Controlla se un gruppo e' composto da solo bots.
 * @param group Il gruppo.
 * @param size  La grandezza del gruppo.
 * @return      Se il gruppo e' composto da soli bots (true) o meno (false).
 */
Boolean group_full_of_bots(Group group, int size)
{
    // Contatore.
    int i;

    /* Controlla ogni player del gruppo in cerca di un umano, */
    for (i = 0; i < size; ++i)
    {
        if (is_human(&group.players[i]))
        {
            return false;
        }
    }

    return true;
}