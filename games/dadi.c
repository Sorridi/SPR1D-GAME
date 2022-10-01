/*
 * REGOLE:
 * I due player hanno in mano 2 dadi. A turno, i player tirano i dadi. Il primo player che fa un tiro la
 * cui somma è minore del tiro precedente, perde. Il player che inizia a tirare è scelto a caso.
 */
#include "../utils/common.h"
#include "../utils/shortcuts.h"
#include "dadi.h"

/**
 * Tira, usando la generazione casuale in un range X->Y, il dado.
 * @return Il valore del dado tirato.
 */
int throw_dice()
{
    return gen_num(VALORE_DADO_MIN, VALORE_DADO_MAX);
}

/**
 * Stampa a video il fatto che il player ha eseguito un punteggio inferiore a quello precedente.
 * @param player Il player che ha tirato.
 * @param oldSum La somma dei dadi precedente.
 */
void print_lesser_points(Player *player, int oldSum)
{
    printf("\nIl punteggio di ");
    print_player_name(player);
    printf("e' inferiore a quello precedente (%d)!\n\n", oldSum);
}

/**
 * Stampa a video il valore dei dadi tirati dal player.
 * @param player    Il player che ha tirato.
 * @param sum       La somma dei dadi.
 */
void print_throw_dices(Player *player, int sum)
{
    print_player_name(player);
    printf("tira i dadi e fa... %d!\n", sum);
}

/**
 * Gioca a dadi.
 * @param players   I players partecipanti.
 * @return          Il player vincitore del match.
 */
Player* play_dadi(Player *players)
{
    /*
     * player1, player2             -> players partecipanti.
     * winner                       -> player vincitore del match.
     *
     * dice1, dice2                 -> valori temporanei dei dadi tirati.
     * sumPlayer1, sumPlayer2       -> somme dei dadi nel turno corrente.
     * oldSumPlayer1, oldSumPlayer2 -> le somme dei dadi del turno precedente.
     *
     * gameEnded                    -> indica se il gioco e' terminato.
     */
    Player *player1 = &players[0];
    Player *player2 = &players[1];
    Player *winner;

    int dice1, dice2;
    int sumPlayer1, oldSumPlayer1 = 0;
    int sumPlayer2, oldSumPlayer2 = 0;

    Boolean gameEnded;

    do
    {
        /* Lancia i dadi per il primo player. */
        dice1       = throw_dice();
        dice2       = throw_dice();
        sumPlayer1  = dice1 + dice2;

        // Stampa a video i dadi tirati
        print_throw_dices(player1, sumPlayer1);

        /*
         * Se la somma del turno precedente e' maggiore alla somma del turno corrente, il giocatore avra' perso
         * il match, e verra' constatato come vincitore l'altro giocatore.
         */
        gameEnded = sumPlayer1 < oldSumPlayer1;

        if (gameEnded)
        {
            winner = player2;
            print_lesser_points(player1, oldSumPlayer1);
        }
        else
        {
            /* Lancia i dadi per il secondo player. */
            dice1       = throw_dice();
            dice2       = throw_dice();
            sumPlayer2  = dice1 + dice2;

            // Stampa a video i dadi tirati
            print_throw_dices(player2, sumPlayer2);

            /*
             * Se la somma del turno precedente e' maggiore alla somma del turno corrente, il giocatore avra' perso
             * il match, e verra' constatato come vincitore l'altro giocatore.
             */
            gameEnded = sumPlayer2 < oldSumPlayer2;

            if (gameEnded)
            {
                winner = player1;
                print_lesser_points(player2, oldSumPlayer2);
            }
        }

        /* Le somme del turno corrente saranno quelle vecchie rispetto al prossimo turno. */
        oldSumPlayer1 = sumPlayer1;
        oldSumPlayer2 = sumPlayer2;

    } while (!gameEnded);

    return winner;
}
