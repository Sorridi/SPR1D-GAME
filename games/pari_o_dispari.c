/*
 * REGOLE:
 * I due player scelgono se essere pari o dispari. Dopodiché, ognuno decide quante dita della mano
 * mostrare: da 0 a 5. La somma dei numeri dei due player decreterà il vincitore, in base al fatto se essa
 * è pari o dispari.
 */
#include "../utils/common.h"
#include "../utils/shortcuts.h"
#include "pari_o_dispari.h"

/**
 * Mostra la mano in pari o dispari.
 * @param player    Il player che deve scegliere.
 * @param isHuman   Se il player e' umano (true) o meno (false).
 * @return          Il numero scelto con la mano.
 */
int show_hand(Player *player, Boolean isHuman)
{
    /*
     * allowed  -> i valori ammessi in input.
     *
     * res      -> il valore della mano.
     */
    static int allowed[ALLOWED_VALUES_SIZE] = { 0, 1, 2, 3, 4, 5 };

    int res;

    /* Se il player e' umano, chiede l'input, altrimenti genera un numero. */
    if (isHuman)
    {
        print_player_name(player);
        printf("scegli quante dita mostrare (da %d a %d)\n", SHOW_HAND_MIN, SHOW_HAND_MAX);
        res = ask_input_int(allowed, ALLOWED_VALUES_SIZE, LENGTH_ONE, false);
    }
    else
    {
        res = gen_num(SHOW_HAND_MIN, SHOW_HAND_MAX);
    }

    return res;
}

/**
 * Chiede al player se, secondo lui, la somma sara' pari o dispari.
 * @param player    Il player che deve scegliere.
 * @param isHuman   Se il player e' umano (true) o meno (false).
 * @return          Il valore della predizione.
 */
PredictValue ask_prediction(Player *player, Boolean isHuman)
{
    /*
     * allowed  -> i valori ammessi in input.
     *
     * res      -> il valore della somma.
     */
    static int allowed[PARI_DISPARI_VALUES] = { PARI, DISPARI };

    PredictValue res;

    /* Se il player e' umano, chiede l'input, altrimenti genera un numero. */
    if (isHuman)
    {
        print_player_name(player);
        printf("scegli tra pari (%d) o dispari (%d)\n", PARI, DISPARI);
        res = ask_input_int(allowed, PARI_DISPARI_VALUES, LENGTH_ONE, false);
    }
    else
    {
        res = gen_num(PARI, DISPARI);
    }

    return res;
}

/**
 * Stampa a video la scelta del player.
 * @param player        Il player che ha scelto il valore.
 * @param predictValue  Il valore scelto.
 */
void print_player_prediction(Player *player, PredictValue predictValue)
{
    /*
     * values -> il corrispondente in stringa di "PredictValue".
     */
    static const char *values[] = { "PARI", "DISPARI" };

    print_player_name(player);
    printf("sceglie %s!\n", values[predictValue]);
}

/**
 * Gioca a pari o dispari.
 * @param players   I players partecipanti.
 * @return          Il player vincitore del match.
 */
Player* play_pari_o_dispari(Player *players)
{
    /*
     * player1, player2                 -> players partecipanti.
     * winner                           -> player vincitore del match.
     *
     * handPlayer1, handPlayer2         -> il valore delle mani dei player.
     * sum                              -> la somma delle mani.
     *
     * predictPlayer1, predictPlayer2   -> valori predetti dai players.
     * correctPrediction                -> il valore corretto che doveva essere predetto.
     *
     * isP1Human, isP2Human             -> indicano se i players sono umani.
     */
    Player *player1 = &players[0];
    Player *player2 = &players[1];
    Player *winner;

    int handPlayer1, handPlayer2, sum;

    PredictValue predictPlayer1, predictPlayer2;
    PredictValue correctPrediction;

    Boolean isP1Human = is_human(player1);
    Boolean isP2Human = is_human(player2);

    /* Chiede la predizione al primo player, il secondo player avra' l'esatto opposto. */
    predictPlayer1 = ask_prediction(player1, isP1Human);
    predictPlayer2 = predictPlayer1 == PARI ? DISPARI : PARI;

    /* Stampa a video le scelte dei players. */
    print_player_prediction(player1, predictPlayer1);
    print_player_prediction(player2, predictPlayer2);
    print_blank();

    /* Ogni player mostra la propria mano. */
    handPlayer1 = show_hand(player1, isP1Human);
    handPlayer2 = show_hand(player2, isP2Human);
    sum         = handPlayer1 + handPlayer2;
    printf("La somma delle mani e': %d\n\n", sum);

    // Calcola la predizione del turno corretta.
    correctPrediction = (PredictValue) is_num_odd(sum);

    // Il player che ha predetto correttamente vince il match.
    winner = predictPlayer1 == correctPrediction ? player1 : player2;

    return winner;
}
