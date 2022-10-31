/*
 * REGOLE:
 * Ogni player ha in mano un numero di biglie pari a 10. A ogni turno, entrambi i giocatori decidono
 * quante biglie scommettere (tra quelle che hanno a disposizione), e il turno consiste in uno dei due
 * giocatori che prova ad indovinare se il player avversario ha scommesso un numero di biglie pari o
 * dispari. Se il player indovina, riceverà dall’avversario un numero di biglie pari a quelle scommesse
 * dall’avversario. Se non indovina, sarà l’avversario a ricevere un numero di biglie pari al numero
 * scommesso dal player. Il gioco termina quando uno dei due giocatori rimane con zero biglie in mano
 * (quest’ultimo perde e viene eliminato).
 */
#include "../utils/common.h"
#include "../utils/shortcuts.h"
#include "pari_o_dispari.h"
#include "biglie.h"

/**
 * Stampa a video la vincita del turno.
 * @param player    Il player che ha vinto il turno.
 * @param gain      Le biglie guadagnate dal player.
 */
void print_player_won_turn(Player *player, int gain)
{
    print_player_name(player);
    printf("ha vinto il turno guadagnando %d biglie!\n", gain);
}

/**
 * Stampa a video la scommessa.
 * @param player    Il player che ha scommesso.
 * @param marbles   Le biglie che il player possiede.
 * @param bet       Le biglie scommesse.
 */
void print_player_has_bet(Player *player, int marbles, int bet)
{
    print_player_name(player);
    printf("ha scommesso %d biglie (su %d).\n", bet, marbles);
}

/**
 * Sceglie il numero di biglie da scommettere.
 * @param player    Il player che deve scegliere.
 * @param isHuman   Se il player e' umano (true) o meno (false).
 * @param marbles   Le biglie che il player possiede.
 * @return          Il numero di biglie scelte da scommettere.
 */
int ask_marbles_amount(Player *player, Boolean isHuman, int marbles)
{
    /*
     * i        -> contatore.
     * res      -> quantita' di biglie scelte.
     * allowed  -> i valori ammessi in input.
     */
    int i;
    int res;
    int *allowed = MALLOC_ARRAY(int, marbles); CRASH_IF_NULL(allowed)

    /* Inizializza l'array. */
    for (i = 0; i < marbles; ++i)
    {
        allowed[i] = i + 1;
    }

    /* Se il player e' umano, chiede l'input, altrimenti genera un numero. */
    if (isHuman)
    {
        print_player_name(player);
        printf("scegli quante biglie scommettere (da %d a %d):\n", MIN_BET, marbles);
        res = ask_input_int(allowed, marbles, LENGTH_TWO, false);
    }
    else
    {
        res = gen_num(MIN_BET, marbles);
    }

    free(allowed);

    return res;
}

/**
 * Gioca a biglie.
 * @param group     Il gruppo di players partecipanti.
 * @return          Il player vincitore del match.
 */
Player *play_biglie(Group group)
{
    /*
     * marblesPlayer1, marblesPlayer2   -> biglie dei players.
     * betPlayer1, betPlayer2           -> biglie scommesse nel turno.
     * marblesBounty                    -> la somma delle biglie scommesse nel turno.
     *
     * player1, player2                 -> players partecipanti.
     * winner                           -> player vincitore del match.
     *
     * predictPlayer1, predictPlayer2   -> valori predetti dai players.
     * correctPrediction                -> il valore corretto che doveva essere predetto per vincere il turno.
     *
     * gameEnded                        -> indica se il gioco e' terminato.
     * isP1Human, isP2Human             -> indicano se i players sono umani.
     */
    int marblesPlayer1 = START_MARBLES;
    int marblesPlayer2 = START_MARBLES;
    int betPlayer1, betPlayer2;
    int marblesBounty;

    Player *player1 = &group.players[0];
    Player *player2 = &group.players[1];
    Player *winner;

    PredictValue predictPlayer1, predictPlayer2;
    PredictValue correctPrediction;

    Boolean gameEnded;
    Boolean isP1Human = is_human(player1);
    Boolean isP2Human = is_human(player2);

    do
    {
        /* Chiede ai players il numero di biglie che voglio scommettere e calcola la somma. */
        betPlayer1      = ask_marbles_amount(player1, isP1Human, marblesPlayer1);
        betPlayer2      = ask_marbles_amount(player2, isP2Human, marblesPlayer2);
        marblesBounty   = betPlayer1 + betPlayer2;

        /* Chiede la predizione al primo player, il secondo player avra' l'esatto opposto.  */
        predictPlayer1 = ask_prediction(player1, isP1Human);
        predictPlayer2 = predictPlayer1 == PARI ? DISPARI : PARI;

        /* Stampa a video le scelte dei players. */
        print_player_prediction(player1, predictPlayer1);
        print_player_prediction(player2, predictPlayer2);
        print_blank();

        // Calcola la predizione del turno corretta.
        correctPrediction = (PredictValue) is_num_odd(marblesBounty);

        /* Stampa a video le biglie scommesse dai players. */
        print_player_has_bet(player1, marblesPlayer1, betPlayer1);
        print_player_has_bet(player2, marblesPlayer2, betPlayer2);

        /*
         * Se il primo player ha predetto correttamente, gli vengono assegnate le
         * biglie scommesse dal secondo player (al quale vengono sottratte).
         * In caso contrario, accade l'opposto.
         */
        if (correctPrediction == predictPlayer1)
        {
            marblesPlayer1 += betPlayer2;
            marblesPlayer2 -= betPlayer2;
            print_player_won_turn(player1, betPlayer2);
        }
        else
        {
            marblesPlayer2 += betPlayer1;
            marblesPlayer1 -= betPlayer1;
            print_player_won_turn(player2, betPlayer1);
        }

        /*
         * Controlla se uno dei due player ha vinto il gioco seguendo la condizione di vittoria delle REGOLE,
         * in caso positivo, constata il vincitore del match.
         */
        gameEnded = marblesPlayer1 == MARBLES_TO_LOSE;

        if (gameEnded)
        {
            winner = player2;
        }
        else
        {
            gameEnded = marblesPlayer2 == MARBLES_TO_LOSE;

            if (gameEnded)
            {
                winner = player1;
            }
        }

        print_blank();

    } while (!gameEnded);

    return winner;
}
