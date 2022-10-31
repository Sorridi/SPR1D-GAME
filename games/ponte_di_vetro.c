/*
 * REGOLE:
 * Il gioco consiste nell’attraversamento di un ponte composto da 3 coppie di vetri consecutive, in cui uno
 * dei due vetri della coppia è fragile, e l’altro no.
 * I 4 giocatori sono inizialmente disposti in modo casuale, dopodiché il primo giocatore dovrà iniziare ad
 * attraversare il ponte, e scegliere su quale tra i vetri della prima coppia passare. Se sbaglia, viene
 * eliminato. Se indovina, può cercare d'indovinare il vetro della successiva coppia. Il giocatore dietro di
 * lui può usare l’informazione data dai tentativi del giocatore precedente (fin dove è riuscito ad arrivare).
 * Vince il primo giocatore che riesce ad attraversare il ponte. Se i primi 3 giocatori vengono tutti e 3
 * eliminati, il quarto vince automaticamente (anche se non ha attraversato il ponte).
 */
#include "../utils/common.h"
#include "../utils/shortcuts.h"
#include "ponte_di_vetro.h"

/**
 * Stampa a video lo stato del ponte.
 * @param bridge    Il ponte.
 * @param turn      Il numero del turno corrente.
 */
void print_bridge(int **bridge, int turn)
{
    /*
     * values   -> il corrispondente in stringa di "Pawn".
     * i, j     -> contatori
     */
    static const char *values[GLASS_VALUES] = { " ", "V" };
    int i, j;

    // Stampa a video la numerazione delle colonne.
    println("    0   1");

    /* Stampa il valore dello slot per ogni riga e colonna. */
    for (i = 0; i < BRIDGE_LENGTH; ++i)
    {
        // Stampa a video la numerazione delle righe.
        printf("%d |", i);

        for (j = 0; j < BRIDGE_WIDTH; ++j)
        {
            /* Evita di stampare altri divisori quando non sono necessari. */
            if (j != 0)
            {
                printf("|");
            }

            /* Stampa il valore del vetro. */
            if (turn > i)
            {
                printf(" %s ", values[bridge[i][j]]);
            }
            else
            {
                printf(" %s ", values[1]);
            }
        }

        printf("|");

        /* Stampa il puntatore che indica la casella nella quale si e' arrivati. */
        if (turn == i)
        {
            printf(" <-");
        }

        print_blank();
    }

    print_blank();
}

/**
 * Genera i vetri del ponte.
 * @param bridge Il ponte.
 */
void generate_bridge(int **bridge)
{
    /*
     * i            -> contatore
     * breakableAt  -> index corrispondente al vetro da impostare a GLASS_TEMPERED
     */
    int i;
    int breakableAt;

    /* Genera l'index del vetro al quale assegnare GLASS_TEMPERED. */
    for (i = 0; i < BRIDGE_LENGTH; ++i)
    {
        breakableAt = gen_num(GLASS_FRAGILE, GLASS_TEMPERED);
        bridge[i][breakableAt] = GLASS_TEMPERED;
    }
}

/**
 * Sceglie quale vetro attraversare.
 * @param player    Il player che deve scegliere.
 * @param isHuman   Se il player e' umano (true) o meno (false).
 * @return          Il vetro scelto 0 (LEFT_GLASS) o 1 (RIGHT_GLASS).
 */
int ask_glass(Player *player, Boolean isHuman)
{
    // I valori ammessi in input.
    static int allowed[GLASS_VALUES] = { LEFT_GLASS, RIGHT_GLASS };

    /* Se il player e' umano, chiede l'input, altrimenti genera un numero. */
    if (isHuman)
    {
        print_player_name(player);
        printf("scegli il vetro sul quale salire tra SINISTRA o DESTRA (%d o %d)\n", LEFT_GLASS, RIGHT_GLASS);
        return ask_input_int(allowed, GLASS_VALUES, LENGTH_ONE, false);
    }

    return gen_num(LEFT_GLASS, RIGHT_GLASS);
}

/**
 * Stampa a video il player che giochera' il turno corrente.
 * @param player Il player.
 */
void print_turn_of(Player *player)
{
    printf("TURNO DI ");
    print_player_name(player);
    print_blank();
}

/**
 * Stampa a video il player che e' caduto dal ponte.
 * @param player Il player.
 */
void print_player_fell(Player *player)
{
    print_player_name(player);
    println("cade nel vuoto!\n");
}

/**
 * Stampa a video il player che ha attraversato il ponte.
 * @param player Il player.
 */
void print_player_cross(Player *player)
{
    print_player_name(player);
    println("ha attraversato il ponte!\n");
}

/**
 * Gioca a ponte di vetro.
 * @param group     Il gruppo di players partecipanti.
 * @return          Il player vincitore del match.
 */
Player *play_ponte_di_vetro(Group group)
{
    /*
     * i                -> contatore.
     * glassChosen      -> il vetro scelto dal player.
     * glassIndex       -> il numero del turno corrente.
     *
     * player           -> player del turno corrente.
     * winner           -> player vincitore del match.
     *
     * playerEliminated -> indica se il player e' stato eliminato.
     * gameEnded        -> indica se il gioco e' terminato.
     *
     * bridge           -> la matrice che rappresenta il ponte di vetro.
     */
    int i;
    int glassChosen;
    int glassIndex = 0;

    Player *player;
    Player *winner;

    Boolean playerEliminated;
    Boolean gameEnded = false;

    int **bridge = MALLOC_ARRAY(int *, BRIDGE_LENGTH); CRASH_IF_NULL(bridge)

    /* Inizializza gli elementi della matrice. */
    for (i = 0; i < BRIDGE_LENGTH; ++i)
    {
        bridge[i] = CALLOC_ARRAY(int, BRIDGE_WIDTH); CRASH_IF_NULL(bridge[i])
    }

    // Genera il ponte.
    generate_bridge(bridge);

    /* Fa giocare ogni player fino a quando il gioco non termina */
    for (i = 0; i < QUAD_GAME && !gameEnded; ++i)
    {
        // Nuovo turno, nuovo player.
        player = &group.players[i];

        do
        {
            /* Il gioco termina quando rimangono solo i vetri non fragili. */
            gameEnded = glassIndex == BRIDGE_LENGTH;

            if (gameEnded)
            {
                winner = player;
                print_player_cross(player);
            }
            else
            {
                print_turn_of(player);

                // Stampa a video lo stato del ponte.
                print_bridge(bridge, glassIndex);

                /*
                 * Fa scegliere al player il vetro sul quale passare, se esso e' fragile, il player viene
                 * eliminato, altrimenti, lo stesso player passa al prossimo turno.
                 */
                glassChosen         = ask_glass(player, is_human(player));
                playerEliminated    = bridge[glassIndex][glassChosen] == GLASS_FRAGILE;

                glassIndex++;

                if (playerEliminated)
                {
                    print_player_fell(player);
                }
            }
        } while (!playerEliminated && !gameEnded);
    }

    /* Libera la memoria. */
    FREE_MATRIX(i, BRIDGE_LENGTH, bridge)

    return winner;
}