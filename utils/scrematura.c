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
int add_concorrenti(Game *game)
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
        insert_player(game, NULL, true, true);
    }

    return game->status.totPlayers;
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
 * @param profiles Il gioco in corso.
 */
void play_scrematura(Game *game)
{
    /*
     * i, counter   -> contatore.
     * added        -> il numero di player aggiunti man mano.
     * length       -> il numero di player per ogni gruppo.
     * competitors  -> il numero totale di competitori, mirror di "*size".
     * shuffledIds  -> gli IDs casualmente ordinati.
     * numGroups    -> il numero di gruppi da creare e il numero di player rimanenti dopo la scrematura.
     * groupSizes   -> indica la quantita' di player che ci sono in ogni gruppo.
     *
     * groups       -> i gruppi di player che giocheranno indovina il numero.
     * winners      -> i vincitori del gioco indovina il numero.
     */
    int i;
    int counter         = 0;
    int added           = 0;
    int length          = 0;
    int competitors     = add_concorrenti(game);
    int numGroups       = find_sizes(competitors);
    int *groupSizes     = MALLOC_ARRAY(int, numGroups); CRASH_IF_NULL(groupSizes)
    int *shuffledIds;

    Group *groups = MALLOC_ARRAY(Group, numGroups); CRASH_IF_NULL(groups)

    /* Inizializza i dati. */
    update_totals(game, STARTUP);
    update_totals(game, PLAYER_STATUSES);

    // Mischia i players, in modo da creare i gruppi casuali.
    shuffledIds = shuffle_players(game);

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
        groups[i] = gen_group(game->players, groupSizes[i], shuffledIds, &counter);
    }

    play_game(INDOVINA_NUMERO, groups, numGroups, 0, groupSizes, game);

    // Aggiorna i dati.
    update_totals(game, PLAYER_STATUSES);

    print_winners(game);

    /* Libera la memoria. */
    free(groups);
    free(groupSizes);
    free(shuffledIds);
}