/*
 * REGOLE:
 * La prima fase ha come obiettivo quello di eliminare una parte dei giocatori. In particolare, l’obiettivo
 * della scrematura è quello di ottenere un numero di giocatori rimanenti pari a una potenza di 2, in modo
 * tale che successivamente i giochi si possano svolgere a coppie o, al più, a gruppi di 4.
 * Più precisamente, dato il numero iniziale di giocatori N, proseguiranno alla fase successiva M = 2^(X-1)
 * giocatori, dove 2^X è la potenza di 2 più vicina ma strettamente minore di N.
 * Gli N giocatori verranno raggruppati in M gruppi generati casualmente (le dimensioni dei gruppi possono
 * differire tra loro di una unità al massimo). Ogni giocatore parteciperà al primo gioco, che prevede un
 * solo vincitore per ogni gruppo.
 * Il gioco di scrematura consiste nel gioco “indovina il numero”.
 */
#include "common.h"
#include "shortcuts.h"
#include "scrematura.h"

/**
 * Chiede il numero di concorrenti da aggiungere dello SPRID-GAME.
 * @return Il numero di concorrenti da aggiungere.
 */
int ask_total_players()
{
    println("Quanti concorrenti parteciperanno?");
    return ask_input_int(NULL, 0, LENGTH_MAX, MIN_PLAYERS);
}

/**
 * Aggiunge un certo numero di concorrenti.
 * @param players       I players partecipanti.
 * @param currentSize   Il numero corrente di partecipanti.
 * @return              Il nuovo numero di partecipanti.
 */
int add_concorrenti(Player **players, int currentSize)
{
    /*
     * i        -> contatore.
     * total    -> il numero di concorrenti da aggiungere.
     * num      -> il numero di giocatori attuali.
     */
    int i;
    int total = ask_total_players();

    /* Inserisce tutti i bots. */
    for (i = 0; i < total; ++i)
    {
        insert_player(players, &currentSize, NULL, true);
    }

    return currentSize;
}

/**
 * Trova il numero M = 2^(X-1) di gruppi da creare.
 * @param input Il numero di partecipanti.
 * @return      Il numero di gruppi da creare.
 */
int find_sizes(int input)
{
    /*
     * exponent     -> l'esponente da utilizzare per fare la potenza.
     * remainder    -> il valore trovato.
     */
    int exponent    = 0;
    int remainder   = 0;

    while (remainder < input)
    {
        remainder = (int) pow(POWER, ++exponent);
    }

    exponent -= 2;
    remainder = (int) pow(POWER, exponent);

    return remainder;
}

/**
 * Chiede quanti bots inserire e esegue la fase di scrematura.
 * @param profiles          I profili dei players.
 * @param numProfiles       Il numero di profili dei players.
 * @param players           I players inseriti manualmente.
 * @param numPlayers        Il quantitativo di players.
 * @param startSize         La grandezza iniziale.
 * @param statuses          Gli statuses di ogni player.
 * @return                  I player vincitori della scrematura.
 */
void play_scrematura(Player **players, int *numPlayers, int *startSize, GameStatus *gameStatus)
{
    /*
     * i, counter   -> contatore.
     * added        -> il numero di player aggiunti man mano.
     * length       -> il numero di player per ogni gruppo.
     * competitors  -> il numero totale di competitori, mirror di "*size".
     * numGroups    -> il numero di gruppi da creare e il numero di player rimanenti dopo la scrematura.
     * shuffledIds  -> gli IDs casualmente ordinati.
     * groupSizes   -> indica la quantita' di player che ci sono in ogni gruppo.
     * winnerIDs    -> la lista degli IDs dei player vincenti.
     *
     * groups       -> i gruppi di player che giocheranno indovina il numero.
     * winners      -> i vincitori del gioco indovina il numero.
     */
    int i;
    int added           = 0;
    int length          = 0;
    int counter         = 0;
    int competitors     = add_concorrenti(players, *numPlayers);
    int numGroups       = find_sizes(competitors);
    int *shuffledIds    = shuffle_players(competitors);
    int **statuses      = &gameStatus->playerStatuses;
    int *groupSizes     = MALLOC_ARRAY(int, numGroups); CRASH_IF_NULL(groupSizes)
    int *winnerIDs      = MALLOC_ARRAY(int, numGroups); CRASH_IF_NULL(winnerIDs)

    Player **groups     = MALLOC_ARRAY(Player *, numGroups); CRASH_IF_NULL(groups)
    Player *winners     = NULL;

    // Aggiorna il numero di players iniziali.
    *startSize = competitors;

    // Aggiorna il numero di player che giocheranno ai prossimi giochi.
    *numPlayers = numGroups;

    /*
     * Dato che ora si sa' il numero di partecipanti, crea l'array di statuses, che indica se un player e'
     * stato eliminato o meno, in base all'index.
     */
    *statuses = REALLOC_ARRAY(int, *statuses, competitors); CRASH_IF_NULL(*statuses)

    /*
     * Trova la grandezza di ogni gruppo.
     */
    while (added != competitors)
    {
        length++;
        for (i = 0; i < numGroups && added != competitors; ++i, ++added)
        {
            // Incrementa la dimensione attuale del gruppo.
            groupSizes[i] = length;
        }
    }

    printf("Ci sono %d partecipanti, dopo la scrematura saranno %d.\n", competitors, numGroups);

    /* Genera i gruppi. */
    for (i = 0; i < numGroups; ++i)
    {
        groups[i] = gen_group(*players, groupSizes[i], shuffledIds, &counter);
    }

    // todo stats pre and post

    /* Gioca a indovina numero. */
    winners = play_game(INDOVINA_NUMERO, NULL, groups, numGroups, 0, groupSizes);

    print_winners(winners, numGroups);

    /* Aggiorna l'array di statuses impostando i player vivi ad: ALIVE. */
    update_player_statuses(winners, numGroups, *statuses, competitors);

    /* Libera la memoria. */
    FREE_MATRIX(i, numGroups, groups)
    free(groupSizes);
    free(winnerIDs);
    free(shuffledIds);
    free(*players);

    *players = winners;
}