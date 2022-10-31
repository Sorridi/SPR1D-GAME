/*
 * REGOLE:
 * Il gioco si avvale di un singolo mazzo di carte, composto da 52 carte distribuite in 4 semi (cuori, quadri,
 * picche, fiori) e, ogni seme possiede le seguenti carte: 1 (asso), 2, 3, 4, 5, 6, 7, 8, 9, 10, fante (J), regina
 * (Q), re (K). Il valore delle carte è quello nominale presente sulla carta, tranne l’asso, che può valere 1 o
 * 11 a scelta dell’utente, e J, Q, e K che valgono 10 punti ciascuna. I semi non hanno alcuna influenza, o
 * valore particolare.
 * I 2 giocatori si sfidano uno contro l’altro, con l’obiettivo di avere in mano delle carte la cui somma sia il
 * più possibile vicina a 21, ma senza superarlo. Il gioco si svolge mediante le seguenti fasi: per ogni
 * giocatore:
 *  1.  entrambi i giocatori ricevono due carte;
 *  2.  in base alla somma delle carte ricevute, a turno, i giocatori possono decidere se chiedere
 *      un’altra carta o fermarsi, accontentandosi del punteggio ottenuto (questa fase si ripete finché il
 *      giocatore non decide di fermarsi, oppure la somma delle sue carte è >21. In quest’ultimo caso,
 *      ha già perso). Questa fase si ripete finché uno dei due giocatori non perde, oppure entrambi si
 *      sono fermati;
 * 3.   il gioco termina se uno dei due giocatori ha perso perché ha superato 21 oppure, se entrambi si
 *      sono fermati, si confrontano le somme ottenute. Chi ha la somma maggiore, vince.
 * 4.   In caso di pareggio, si ripete la partita riconsiderando l’intero mazzo, finché non si ottiene un vincitore.
 */
#include "../utils/common.h"
#include "../utils/shortcuts.h"
#include "blackjack.h"

/**
 * Chiede al player di scegliere il valore dell'asso.
 * @param profile   Il profilo del player.
 * @return          Il valore dell'asso a seconda della mano.
 */
int ask_ace_value(BlackJackProfile *profile)
{
    /*
     * aceValue -> il valore piu' vantaggioso dell'asso.
     * points   -> i punti correnti del player.
     */
    int aceValue    = SECOND_ACE_VALUE;
    int points      = profile->points;

    /* Sceglie il valore dell'asso con il quale il player potrebbe non perdere. */
    if (points + aceValue > POINTS_TO_WIN)
    {
        aceValue = FIRST_ACE_VALUE;
    }

    if (profile->isHuman)
    {
        printf("Il valore dell'asso di %s e' di %d\n", profile->player->name, aceValue);
    }

    return aceValue;
}

/**
 * Calcola il valore delle carte del player.
 * @param profile       Il profilo del player.
 * @return              Il punteggio calcolato.
 */
int calc_card_sum(BlackJackProfile *profile)
{
    /*
     * i        -> contatore.
     * index    -> valore temporaneo.
     *
     * sum      -> la somma di tutti i valori delle carte.
     * aces     -> il numero di assi.
     *
     * values   -> il valore associato ad ogni carta index => valore.
     */
    int i;
    int index;

    int sum     = 0;
    int aces    = 0;

    static int values[] =
            {
                    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10
            };

    /* Somma i valori delle carte. */
    for (i = 0; i < profile->numCards; ++i)
    {
        index = profile->cards[i].value;

        if (index == ASSO)
        {
            aces++;
        }
        else
        {
            sum += values[index];
        }
    }

    /* Calcola il valore piu' vantaggioso per ogni asso. */
    for (i = 0; i < aces; ++i)
    {
        sum += ask_ace_value(profile);
    }

    if (profile->isHuman)
    {
        printf("Il valore delle carte di ");
        print_player_name(profile->player);
        printf("e': %d\n", sum);
    }

    return sum;
}

/**
 * Stampa a video il valore di una carta.
 * @param card La carta.
 */
void print_card(Card *card)
{
    /*
     * seeds        -> corrispondenti in stringa dei semi delle carte.
     * values       -> corrispondenti in stringa dei valori delle carte.
     *
     * seedIndex    -> il seme della carta.
     * valueIndex   -> il valore della carta.
     */
    static char *seeds[] =
            {
                    "CUORI", "QUADRI", "PICCHE", "FIORI"
            };
    static char *values[] =
            {
                "ASSO", "DUE","TRE","QUATTRO", "CINQUE","SEI",
                "SETTE","OTTO","NOVE", "DIECI","J", "Q", "K"
            };

    int seedIndex   = card->seed;
    int valueIndex  = card->value;

    printf("%s di %s\n", values[valueIndex], seeds[seedIndex]);
}

/**
 * Stampa a video il risultato dei punti.
 * @param major Il profilo del player con piu' punti.
 * @param minor Il profilo del player con meno punti.
 */
void print_more_points(BlackJackProfile *major, BlackJackProfile *minor)
{
    print_blank();
    print_player_name(major->player);
    printf("ha piu' punti di ");
    print_player_name(minor->player);
    printf("%d a %d!\n", major->points, minor->points);
}

/**
 * Stampa a video il player eliminato e il suo punteggio.
 * @param profile Il profilo del player.
 */
void print_eliminated(BlackJackProfile *profile)
{
    print_player_name(profile->player);
    printf("ha fatto piu' di %d punti, ed e' stato eliminato!\n", POINTS_TO_WIN);
}

/**
 * Stampa a video l'azione intrapresa dal player.
 * @param profile       Il profilo del player.
 * @param stopFromNow   Se il player ha deciso di fermarsi dal turno seguente (true) o meno (false).
 */
void print_action_stop(BlackJackProfile *profile, Boolean stopFromNow)
{
    print_player_name(profile->player);

    if (stopFromNow)
    {
        println("ha deciso di fermarsi!");
    }
    else
    {
        println("e' fermo!");
    }
}

/**
 * Aggiorna i punti al player.
 * @param profile Il profilo del player.
 */
void update_points(BlackJackProfile *profile)
{
    profile->points = calc_card_sum(profile);
}

/**
 * Assegna una nuova carta al player.
 * @param profile       Il profilo del player.
 * @param deck          Il mazzo di carte.
 * @param announce      Se annunciare (true) o meno (false) il punteggio delle carte.
 */
void give_card(BlackJackProfile *profile, DeckOfCards *deck, Boolean announce)
{
    /*
     * player       -> il player.
     *
     * curSize      -> il nuovo numero di carte del player.
     * curPointer   -> la posizione attuale nel deck.
     *
     * drawn        -> la carta pescata dal deck.
     * cards        -> le carte del player.
     */
    Player *player  = profile->player;

    int curSize     = ++profile->numCards;
    int curPointer  = deck->position++;

    Card drawn  = deck->cards[curPointer];
    Card *cards = profile->cards;

    cards               = REALLOC_ARRAY(Card, cards, curSize); CRASH_IF_NULL(cards)
    cards[curSize - 1]  = drawn;
    profile->cards      = cards;

    /* Se il player e' umano, e' opportuno stampare a video il valore della carta appena pescata. */
    if (profile->isHuman)
    {
        print_player_name(player);
        printf("ha ricevuto: ");
        print_card(&drawn);
    }
    else
    {
        print_player_name(player);
        println("ha pescato una carta!");
    }

    if (announce)
    {
        update_points(profile);
    }
}

/**
 * Sceglie se pescare dal deck o fermarsi.
 * @param profile   Il profilo del player.
 * @return          L'azione intrapresa dal player.
 */
BlackJackAction ask_draw_or_stop(BlackJackProfile *profile)
{
    /*
     * allowed      -> i valori ammessi in input.
     *
     * res          -> il valore scelto.
     * botPoints    -> i punti del bot.
     */
    static int allowed[POSSIBLE_ACTIONS] = { DRAW, STOP };

    int res;
    int points = profile->points;

    /* Se il player e' umano, chiede l'input, altrimenti genera un numero. */
    if (profile->isHuman)
    {
        print_blank();
        print_player_name(profile->player);
        printf("scegli se pescare o fermarti (%d o %d)\n", DRAW, STOP);
        res = ask_input_int(allowed, POSSIBLE_ACTIONS, LENGTH_ONE, false);
    }
    else
    {
        // Evita di pescare in caso i punti siano gia' quelli necessari per vincere il match.
        res = (points == POINTS_TO_WIN) ? STOP : gen_num(DRAW, STOP);
    }

    return res;
}

/**
 * Costruisce e inizializza il deck di carte.
 * @param deck Il deck di carte.
 */
void build_deck(DeckOfCards *deck)
{
    /*
     * i, j, k                  -> contatori.
     * toReplace, replacement   -> valori temporanei.
     *
     * tempCard                 -> carta temporanea.
     * deckCards                -> il deck di carte temporaneo.
     */
    int i, j, k = 0;
    int toReplace, replacement;

    Card tempCard;
    Card *deckCards = CALLOC_ARRAY(Card, CARD_DECK_SIZE); CRASH_IF_NULL(deckCards)

    /* Inizializza il mazzo in modo ordinato. */
    for (i = 0; i < CARD_VALUES; ++i)
    {
        for (j = 0; j < CARD_SEEDS; ++j, ++k)
        {
            tempCard.value  = i;
            tempCard.seed   = j;
            deckCards[k]    = tempCard;
        }
    }

    /* Mischia il mazzo */
    for (i = 0; i < DECK_SHUFFLES; ++i)
    {
        toReplace   = gen_num(0, CARD_DECK_SIZE - 1);
        replacement = gen_num(0, CARD_DECK_SIZE - 1);

        tempCard                = deckCards[toReplace];
        deckCards[toReplace]    = deckCards[replacement];
        deckCards[replacement]  = tempCard;
    }

    deck->cards     = deckCards;
    deck->position  = 0;
}

/**
 * Costruisce il profilo del player.
 * @param player    Il player.
 * @param profile   Il profilo del player da costruire.
 */
void build_profile(Player *player, BlackJackProfile *profile)
{
    profile->player    = player;
    profile->isHuman   = is_human(player);
    profile->cards     = NULL;
    profile->numCards  = 0;
    profile->action    = NONE;
    profile->points    = 0;
}

/**
 * Gioca a BlackJack
 * @param group     Il gruppo di players partecipanti.
 * @return          Il player vincitore del match.
 */
Player* play_blackjack(Group group)
{
    /*
     * i                    -> contatore.
     *
     * player1, player2     -> players partecipanti.
     * winner               -> player vincitore del match.
     *
     * gameEnded, gameDraw  -> indicano se il gioco e' terminato e se il gioco e' terminato in parita'
     *
     * deckOfCards          -> il mazzo di carte.
     * profile1, profile2   -> i profili dei players.
     */
    int i;

    Player *player1 = &group.players[0];
    Player *player2 = &group.players[1];
    Player *winner;

    Boolean gameEnded, gameDraw;

    DeckOfCards         deckOfCards;
    BlackJackProfile    profile1, profile2;

    do
    {
        gameEnded = false;

        build_deck(&deckOfCards);

        /* Crea i profili dei giocatori. */
        build_profile(player1, &profile1);
        build_profile(player2, &profile2);

        /* Assegna le carte ai giocatori. */
        for (i = 0; i < CARDS_AT_START; ++i)
        {
            give_card(&profile1, &deckOfCards, false);
            give_card(&profile2, &deckOfCards, false);
        }

        print_blank();

        /* Calcola il punteggio di ogni player. */
        update_points(&profile1);
        update_points(&profile2);

        /* In caso i player facciano il punteggio vincente al primo turno, dichiara parita'. */
        if (profile1.points == profile2.points && profile1.points > POINTS_TO_WIN)
        {
            gameEnded   = true;
            gameDraw    = true;
            println("PAREGGIO! SI RICOMINCIA!");
        }
        else
        {
            /*
             * Controlla se uno dei due player ha superato il punteggio di vittoria,
             * in caso positivo, elimina il player e fa vincere l'altro player.
             */
            if (profile1.points > POINTS_TO_WIN)
            {
                winner      = player2;
                gameEnded   = true;
                gameDraw    = false;
                print_eliminated(&profile1);
            }
            else if (profile2.points > POINTS_TO_WIN)
            {
                winner      = player1;
                gameEnded   = true;
                gameDraw    = false;
                print_eliminated(&profile2);
            }
        }

        while (!gameEnded)
        {
            /*
             * Se il player e' alla sua prima azione oppure se ha deciso di pescare nel turno precedente,
             * gli chiede l'azione che vuole intraprendere nel turno seguente.
             *
             * Se il player aveva gia' deciso di fermarsi, stampa a video che e' fermo.
             */
            if (profile1.action == NONE || profile1.action == DRAW)
            {
                profile1.action = ask_draw_or_stop(&profile1);

                /*
                 * Se il player ha deciso di pescare, ricevera' una carta, se invece ha deciso di fermarsi,
                 * verra' stampato a video il fatto che dal turno corrente in poi esso sara' fermo.
                 */
                if (profile1.action == DRAW)
                {
                    give_card(&profile1, &deckOfCards, true);
                }
                else
                {
                    print_action_stop(&profile1, true);
                }
            }
            else
            {
                print_action_stop(&profile1, false);
            }

            /*
             * Se il player e' alla sua prima azione oppure se ha deciso di pescare nel turno precedente,
             * gli chiede l'azione che vuole intraprendere nel turno seguente.
             *
             * Se il player aveva gia' deciso di fermarsi, stampa a video che e' fermo.
             */
            if (profile2.action == NONE || profile2.action == DRAW)
            {
                profile2.action = ask_draw_or_stop(&profile2);

                /*
                 * Se il player ha deciso di pescare, ricevera' una carta, se invece ha deciso di fermarsi,
                 * verra' stampato a video il fatto che dal turno corrente in poi esso sara' fermo.
                 */
                if (profile2.action == DRAW)
                {
                    give_card(&profile2, &deckOfCards, true);
                }
                else
                {
                    print_action_stop(&profile2, true);
                }
            }
            else
            {
                print_action_stop(&profile2, false);
            }

            /*
             * Controlla se uno dei due player ha superato il punteggio di vittoria,
             * in caso positivo, elimina il player e fa vincere l'altro player.
             */
            if (profile1.points > POINTS_TO_WIN)
            {
                winner      = player2;
                gameEnded   = true;
                gameDraw    = false;
                print_eliminated(&profile1);
            }
            else if (profile2.points > POINTS_TO_WIN)
            {
                winner      = player1;
                gameEnded   = true;
                gameDraw    = false;
                print_eliminated(&profile2);
            }
            else
            {
                // Se entrambi i giocatori hanno deciso di fermarsi il gioco termina.
                gameEnded = profile1.action == STOP && profile2.action == STOP;

                if (gameEnded)
                {
                    /*
                     * Controlla se uno dei due player ha superato l'altro in punteggio, in caso positivo,
                     * constata il vincitore del match, altrimenti, ricomincia il match.
                     */
                    if (profile1.points > profile2.points)
                    {
                        winner      = player1;
                        gameDraw    = false;
                        print_more_points(&profile1, &profile2);
                    }
                    else if (profile1.points < profile2.points)
                    {
                        winner      = player2;
                        gameDraw    = false;
                        print_more_points(&profile2, &profile1);
                    }
                    else
                    {
                        gameDraw = true;
                        println("PAREGGIO! SI RICOMINCIA!");
                    }
                }
            }
        }
        print_blank();

    } while (gameDraw);

    free(deckOfCards.cards);
    free(profile1.cards);
    free(profile2.cards);

    return winner;
}