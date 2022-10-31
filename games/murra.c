/*
 * REGOLE:
 * I due giocatori generano due numeri:
 *      ● un numero compreso tra 1 e 5, estremi inclusi, rappresentante il numero scelto;
 *      ● un numero compreso tra 2 e 10, estremi inclusi, rappresentante la somma dei due numeri scelti dai giocatori.
 * Vince il giocatore che riesce a indovinare la somma. In caso di parità, si procede a oltranza finché non
 * c’è un solo vincitore.
 */
#include "../utils/common.h"
#include "../utils/shortcuts.h"
#include "murra.h"

/**
 * Mostra la mano in murra.
 * @param player    Il player che deve scegliere.
 * @param isHuman   Se il player e' umano (true) o meno (false).
 * @return          Il numero scelto con la mano.
 */
int show_murra_hand(Player *player, Boolean isHuman)
{
    /*
     * allowed  -> i valori ammessi in input.
     *
     * res      -> il valore della mano.
     */
    static int allowed[HAND_VALUES] = { 1, 2, 3, 4, 5 };

    int res;

    /* Se il player e' umano, chiede l'input, altrimenti genera un numero. */
    if (isHuman)
    {
        print_player_name(player);
        printf("scegli quante dita mostrare (da %d a %d)\n", LOWEST_HAND_VALUE, HIGHEST_HAND_VALUE);
        res = ask_input_int(allowed, HAND_VALUES, LENGTH_ONE, false);
    }
    else
    {
        res = gen_num(LOWEST_HAND_VALUE, HIGHEST_HAND_VALUE);
    }

    return res;
}

/**
 * Stampa a video il valore della predizione e quante dita il player mostra.
 * @param player        Il player che ha predetto.
 * @param hand          La mano del player.
 * @param prediction    La predizione del player.
 */
void print_hand_prediction(Player *player, int hand, int prediction)
{
    print_player_name(player);
    printf("ha predetto %d e mostra %d dita!\n", prediction, hand);
}

/**
 * Chiede al player quanto sara' la somma delle mani dei players.
 * @param player    Il player che deve scegliere.
 * @param isHuman   Se il player e' umano (true) o meno (false);
 * @return          Il valore predetto.
 */
int ask_sum_prediction(Player *player, Boolean isHuman)
{
    /*
     * allowed  -> i valori ammessi in input.
     *
     * res      -> il valore della somma.
     */
    static int allowed[PREDICTIONS_VALUES] = { 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    int res;

    /* Se il player e' umano, chiede l'input, altrimenti genera un numero. */
    if (isHuman)
    {
        print_player_name(player);
        printf("scegli quanto e' la somma delle mani (da %d a %d)\n", PREDICT_SUM_MIN, PREDICT_SUM_MAX);
        res = ask_input_int(allowed, PREDICTIONS_VALUES, LENGTH_TWO, false);
    }
    else
    {
        res = gen_num(PREDICT_SUM_MIN, PREDICT_SUM_MAX);
    }

    return res;
}

/**
 * Gioca a murra.
 * @param group     Il gruppo di players partecipanti.
 * @return          Il player vincitore del match.
 */
Player *play_murra(Group group)
{
    /*
     * player1, player2         -> players partecipanti.
     * winner                   -> player vincitore del match.
     *
     * gameEnded                -> indica se il gioco e' terminato.
     * isP1Human, isP2Human     -> indica se i player sono umani.
     *
     * handPlayer1, handPlayer2 -> il valore delle mani dei players.
     * sumPlayer1, sumPlayer2   -> le somme del turno predette dai players.
     * trueSum                  -> la vera somma delle mani del turno.
     */
    Player *player1 = &group.players[0];
    Player *player2 = &group.players[1];
    Player *winner;

    Boolean gameEnded;
    Boolean isP1Human = is_human(player1);
    Boolean isP2Human = is_human(player2);

    int handPlayer1, handPlayer2;
    int sumPlayer1, sumPlayer2;
    int trueSum;

    do
    {
        /* Chiede la predizione della somma ai players. */
        sumPlayer1 = ask_sum_prediction(player1, isP1Human);
        sumPlayer2 = ask_sum_prediction(player2, isP2Human);

        /* Chiede ai players di mostrare le mani. */
        handPlayer1 = show_murra_hand(player1, isP1Human);
        handPlayer2 = show_murra_hand(player2, isP2Human);
        trueSum     = handPlayer1 + handPlayer2;

        /* Stampa a video le scelte dei player. */
        print_hand_prediction(player1, handPlayer1, sumPlayer1);
        print_hand_prediction(player2, handPlayer2, sumPlayer2);

        printf("La somma del turno da predire era %d.\n", trueSum);

        /*
         * Il gioco termina soltanto se le predizioni dei players sono diverse.
         */
        gameEnded = sumPlayer1 != sumPlayer2;

        if (!gameEnded)
        {
            println("PAREGGIO! SI RICOMINCIA!");
        }
        else
        {
            /*
             * Se il player ha predetto correttamente il valore della somma, vince.
             */
            if (trueSum == sumPlayer1)
            {
                winner = player1;
            }
            else
            {
                /*
                 * Se il player ha predetto correttamente il valore della somma, vince.
                 */
                if (trueSum == sumPlayer2)
                {
                    winner = player2;
                }
                else
                {
                    println("NESSUNO HA INDOVINATO! PAREGGIO! SI RICOMINCIA!");
                    gameEnded = false;
                }
            }
        }
        print_blank();

    } while (!gameEnded);

    return winner;
}