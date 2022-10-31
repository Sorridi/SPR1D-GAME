/*
 * REGOLE:
 * I due giocatori possono scegliere uno tra i seguenti simboli: carta, forbici, o sasso. Si confrontano,
 * quindi, i simboli scelti dai giocatori e si dichiara il vincitore, in base alle seguenti regole:
 *      ● carta contro forbici => vince forbici
 *      ● carta contro sasso => vince carta
 *      ● forbici contro sasso => vince sasso
 *      ● se i due simboli sono uguali non vince nessuno e si ripete il gioco finché non c’è un vincitore.
 */
#include "../utils/common.h"
#include "../utils/shortcuts.h"
#include "morra_cinese.h"

/**
 * Ritorna il simbolo che vince sul simbolo in ingresso.
 * @param symbol    Il simbolo da battere.
 * @return          Il simbolo che vince.
 */
Symbols win_condition(Symbols symbol)
{
    return symbol == CARTA ? FORBICI : symbol == FORBICI ? SASSO : CARTA;
}

/**
 * Stampa a video la scelta del player.
 * @param player Il player che ha scelto il valore.
 * @param symbol Il simbolo scelto.
 */
void print_player_hand(Player *player, Symbols symbol)
{
    // Il corrispondente in stringa di "Symbols".
    static const char *values[] = { "CARTA", "FORBICI", "SASSO" };

    print_player_name(player);
    printf("sceglie %s!\n", values[symbol]);
}

/**
 * Mostra la mano in morra cinese.
 * @param player    Il player che deve scegliere.
 * @param isHuman   Se il player e' umano (true) o meno (false).
 * @return          Il simbolo scelto con la mano.
 */
Symbols show_symbol(Player *player, Boolean isHuman)
{
    /*
     * allowed  -> i valori ammessi in input.
     *
     * res      -> il simbolo scelto con la mano.
     */
    static int allowed[SYMBOLS_VALUES] = { 0, 1, 2 };

    Symbols res;

    /* Se il player e' umano, chiede l'input, altrimenti genera un numero. */
    if (isHuman)
    {
        print_player_name(player);
        println("scegli il simbolo che vuoi mostrare tra CARTA (0), FORBICI (1), SASSO (2):");
        res = ask_input_int(allowed, SYMBOLS_VALUES, LENGTH_ONE, false);
    }
    else
    {
        res = gen_num(LOWEST_SYMBOL, HIGHEST_SYMBOL);
    }

    return res;
}

/**
 * Gioca a morra cinese.
 * @param group     Il gruppo di players partecipanti.
 * @return          Il player vincitore del match.
 */
Player *play_morra_cinese(Group group)
{
    /*
     * player1, player2         -> players partecipanti.
     * winner                   -> player vincitore del match.
     *
     * gameEnded                -> indica se il gioco e' terminato.
     * isP1Human, isP2Human     -> indica se i player sono umani.
     *
     * handPlayer1, handPlayer2 -> i simboli scelti dai players.
     * handToWin                -> il simbolo da avere per vincere il match.
     */
    Player *player1 = &group.players[0];
    Player *player2 = &group.players[1];
    Player *winner;

    Boolean gameEnded;
    Boolean isP1Human = is_human(player1);
    Boolean isP2Human = is_human(player2);

    Symbols handPlayer1, handPlayer2;
    Symbols handToWin;

    do
    {
        /* Chiede ai players il simbolo che vogliono mostrare. */
        handPlayer1 = show_symbol(player1, isP1Human);
        handPlayer2 = show_symbol(player2, isP2Human);

        /* Stampa a video le scelte dei players. */
        print_player_hand(player1, handPlayer1);
        print_player_hand(player2, handPlayer2);

        /*
         * Il gioco termina soltanto se i simboli scelti dai players sono diversi.
         */
        gameEnded = handPlayer1 != handPlayer2;

        if (gameEnded)
        {
            /*
             * Constata vincitore il player che ha scelto il simbolo che vince su quello dell'avversario.
             */
            handToWin   = win_condition(handPlayer2);
            winner      = handToWin == handPlayer1 ? player1 : player2;
        }
        else
        {
            println("PAREGGIO! SI RICOMINCIA!");
        }

        print_blank();

    } while (!gameEnded);

    return winner;
}
