/*
 * REGOLE:
 * La CPU genera casualmente un numero
 * compreso tra 0 e 999 e, a turno (in ordine di numero identificativo), i giocatori del gruppo provano ad
 * indovinare il numero. La CPU, a ogni tentativo dirà se il numero inserito è maggiore o minore del numero
 * da indovinare. Vince il giocatore che per primo indovina il numero, gli altri giocatori vengono subito
 * eliminati.
 */
#include "../utils/common.h"
#include "../utils/shortcuts.h"
#include "indovina_numero.h"

/**
 * Chiede al player di scegliere un numero.
 * @param player    Il player che deve scegliere.
 * @param isHuman   Se il player e' umano (true) o meno (false).
 * @return          Il numero scelto.
 */
int guess_number(Player *player, Boolean isHuman)
{
    // Il numero scelto.
    int res;

    /* Se il player e' umano, chiede l'input, altrimenti genera un numero. */
    if (isHuman)
    {
        print_player_name(player);
        printf("scegli un numero (da %d a %d)\n", GUESS_MIN, GUESS_MAX);
        res = ask_input_int(NULL, 0, LENGTH_THREE, false);
    }
    else
    {
        res = gen_num(GUESS_MIN, GUESS_MAX);
    }

    return res;
}

/**
 * Stampa a video il numero scelto dal player.
 * @param player    Il player che ha scelto il numero.
 * @param guess     Il numero scelto dal player.
 * @param toGuess   Il numero da indovinare.
 */
void print_guess(Player *player, int guess, int toGuess)
{
    print_player_name(player);
    printf("ha scelto il numero %d ", guess);

    if (toGuess > guess)
    {
        printf("(MAGGIORE)\n");
    }
    else if (toGuess < guess)
    {
        printf("(MINORE)\n");
    }
    else
    {
        printf("(INDOVINATO!)\n");
    }
}

/**
 * Gioca a indovina il numero.
 * @param group     Il gruppo di players partecipanti.
 * @return          Il player vincitore del match.
 */
Player *play_indovina_numero(Group group, int size)
{
    /*
     * i, j             -> contatori.
     * numGuessed       -> il numero indovinato.
     * numToGuess       -> il numero da indovinare (da GUESS_MIN a GUESS_MAX).
     * idsSort          -> array di utility da ordinare in modo crescente.
     *
     * winner           -> il player vincitore del match.
     * player           -> il player del turno corrente.
     * playersPlaying   -> i players partecipanti sortati in ordine di IDs.
     *
     * gameEnded        -> indica se il gioco e' terminato.
     */
    int i, j;
    int numGuessed;
    int numToGuess  = gen_num(GUESS_MIN, GUESS_MAX);
    int *idsSort    = MALLOC_ARRAY(int, size); CRASH_IF_NULL(idsSort)

    Player *player;
    Player *winner;
    Player *playersPlaying = MALLOC_ARRAY(Player, size); CRASH_IF_NULL(playersPlaying)

    Boolean gameEnded = false;
    Boolean found;

    /* Inserisce ogni id in un array che verra' ordinato in modo crescente. */
    for (i = 0; i < size; ++i)
    {
        idsSort[i] = group.players[i].identifier;
    }

    /* Ordina l'array che verra' utilizzato per creare l'ordine di gioco. */
    selection_sort(idsSort, size);

    /* Basandosi sull'array d'IDs, costruisce l'array dei giocatori ordinato per IDs. */
    for (i = 0; i < size; ++i)
    {
        found = false;

        for (j = 0; j < size && !found; ++j)
        {
            if (idsSort[i] == group.players[j].identifier)
            {
                playersPlaying[i] = group.players[j];
                found = true;
            }
        }
    }

    do
    {
        for (i = 0; i < size && !gameEnded; ++i)
        {
            // Nuovo turno, nuovo player.
            player = &playersPlaying[i];

            /* Fa scegliere un numero al player, se esso era il numero da indovinare, il player vince il gioco. */
            numGuessed  = guess_number(player, is_human(player));
            gameEnded   = numGuessed == numToGuess;

            // Stampa a video la scelta del player.
            print_guess(player, numGuessed, numToGuess);

            if (gameEnded)
            {
                winner = player;
            }
        }
    } while (!gameEnded);

    print_blank();

    /* Imposta il corretto vincitore usando la referenza dell'array di players principale. */
    for (i = 0; i < size; ++i)
    {
        if (group.players[i].identifier == winner->identifier)
        {
            winner = &group.players[i];
            i = size;
        }
    }

    free(idsSort);
    free(playersPlaying);

    return winner;
}
