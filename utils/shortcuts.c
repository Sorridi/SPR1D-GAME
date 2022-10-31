#include "common.h"
#include "shortcuts.h"

#include "../games/dadi.h"
#include "../games/pari_o_dispari.h"
#include "../games/tris.h"
#include "../games/biglie.h"
#include "../games/morra_cinese.h"
#include "../games/murra.h"
#include "../games/indovina_numero.h"
#include "../games/ponte_di_vetro.h"
#include "../games/blackjack.h"
#include "../games/impiccato.h"
#include "../games/bot_game.h"

/**
 * Stampa a video un "a capo".
 */
void print_blank()
{
    printf("\n");
}

/**
 * Stampa a video i fantastici fiorellini.
 */
void print_fiorellini()
{
    println("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
}

/**
 * Stampa a video l'introduzione al gioco.
 */
void print_game_new()
{
    print_fiorellini();
    println("\tGIOCO DELLO SPR1D GAME!");
    print_fiorellini();
}

/**
 * Stampa a video del testo con un "a capo".
 * @param text Il testo da stampare.
 */
void println(char *text)
{
    printf("%s", text);
    print_blank();
}

/**
 * Stampa a video i partecipanti del match.
 * @param group Il gruppo di players.
 * @param size  La grandezza del gruppo.
 */
void print_start_match(Group group, int size)
{
    // Contatore.
    int i;

    printf("MATCH: ");

    /* Stampa a video il nome dei player del match. */
    for (i = 0; i < size; ++i)
    {
        print_player_name(&group.players[i]);

        /* Evita di scrivere "vs" arrivato all'ultimo player. */
        if (i != size - 1)
        {
            printf("vs ");
        }
    }
    print_blank();
    print_blank();
}

/**
 * Stampa a video il vincitore del match.
 * @param player Il player vincitore.
 */
void print_end_match(Player *player)
{
    printf("PARTITA TERMINATA!\nVINCE: ");
    print_player_name(player);
    print_blank();
    print_fiorellini();
}

/**
 * Stampa a video l'inizio di un nuovo gioco.
 * @param game La tipologia del gioco.
 */
void print_new_game(Games game)
{
    /* I nomi di tutti i giochi. */
    static char *gameNames[] =
            {
                    "DADI",
                    "BIGLIE",
                    "MURRA",
                    "TRIS",
                    "MORRA CINESE",
                    "PARI O DISPARI",
                    "PONTE DI VETRO",
                    "IMPICCATO",
                    "BLACKJACK",
                    "INDOVINA NUMERO"
            };

    clear_screen();
    print_fiorellini();
    printf("NUOVO GIOCO: %s!\n", gameNames[game]);
    print_fiorellini();
}

/**
 * Stampa a video i giochi rimanenti.
 * @param game La lista di giochi rimanenti.
 */
void print_remaining_games(Games *remainingGames)
{
    /*
     * gameNames    -> i nomi di tutti i giochi, esclusi BLACKJACK e INDOVINA_NUMERO.
     *
     * i            -> contatore
     */
    static char *gameNames[] =
            {
                    "DADI",
                    "BIGLIE",
                    "MURRA",
                    "TRIS",
                    "MORRA CINESE",
                    "PARI O DISPARI",
                    "PONTE DI VETRO",
                    "IMPICCATO"
            };

    int i;

    print_fiorellini();
    println("Giochi rimanenti:");

    for (i = 0; i < NUM_GAMES; ++i)
    {
        if (remainingGames[i] != NO_GAME)
        {
            printf("\t%d |> %s\n", i, gameNames[i]);
        }
    }

    print_fiorellini();
}

/**
 * Stampa a video il messaggio di errore desiderato.
 * @param inputErr La tipologia del messaggio di errore.
 */
void print_err(InputErr inputErr)
{
    /* Messaggi di errore. */
    static char *errorNames[] =
            {
                    "Hai inserito dei caratteri non consentiti!",
                    "Sono stati inseriti troppi caratteri!",
                    "Sono stati inseriti troppi pochi caratteri!",
                    "Devi inserire una delle parole/frasi consentite!",
                    "Devi inserire diversi caratteri!",
                    "E' stato inserito un numero troppo grande!",
                    "E' stato inserito un numero troppo piccolo!",
                    "Devi inserire uno dei numeri consentiti!",
                    "Devi inserire un numero differente!",
                    "Un giocatore con lo stesso nome e' gia' presente!",
                    "Un errore di sistema e' avvenuto! :("
            };

    printf("\nERRORE: %s\n", errorNames[inputErr]);
}

/**
 * Stampa a video una stringa con un certo numero di "a capo".
 * @param text      Il testo da stampare.
 * @param newLines  Il numero di nuove righe.
 */
void println_wqt(char *text, int newLines)
{
    // Contatore.
    int i;

    println(text);

    for (i = 0; i < newLines; ++i)
    {
        print_blank();
    }
}

/**
 * Pulisci-schermo rudimentale.
 */
void clear_screen()
{
    println_wqt(EMPTY_STRING, CLEAR_SCREEN_LINES);
}

/**
 * Stampa a video il nome e l'id del player.
 * @param player Il player.
 */
void print_player_name(Player *player)
{
    if (player->name == NULL)
    {
        printf(BOTS_NAME);
    }
    else
    {
        printf("%s", player->name);
    }

    printf(" (id: %d) ", player->identifier);
}

/**
 * Stampa a video il nome e l'id del player.
 * @param player Il player.
 */
void print_profile_name(Profile *profile)
{
    printf("%s (id: %d) ", profile->name, profile->identifier);
}

/**
 * Stampa a video i gruppi creati.
 * @param groups    I gruppi.
 * @param rows      Il numero di righe.
 * @param columns   Il numero di colonne.
 */
void print_groups(Group *groups, int rows, int columns)
{
    // Contatori.
    int i, j;

    println("\nI gruppi per il prossimo gioco saranno i seguenti:");

    for (i = 0; i < rows; ++i)
    {
        printf("\nGRUPPO: %d\n", i);

        for (j = 0; j < columns; ++j)
        {
            print_tablike(&groups[i].players[j]);
        }
    }

    print_blank();
}

/**
 * Stampa a video il vincitore dello SPR1D-GAME.
 * @param winner Il player vincitore.
 */
void print_winner(Game *game)
{
    int i;

    Player *winner;

    printf("\nIL VINCITORE DELLO SPR1D-GAME E': ");

    for (i = 0; i < game->status.totPlayers; ++i)
    {
        winner = GET(game->players, i);

        if (winner->alive)
        {
            print_player_name(winner);
        }
    }

    print_blank();
}

/**
 * Stampa a video i vincitori di ogni gruppo.
 * @param winners   I player vincitori.
 * @param size      Il numero di vincitori.
 */
void print_winners(Game *game)
{
    /*
     * i        -> contatore.
     *
     * player   -> valore temporaneo di supporto.
     */
    int i;

    Player *player;

    println("\nVINCITORI DEL GIOCO:");

    for (i = 0; i < game->status.totPlayers; ++i)
    {
        player = GET(game->players, i);

        if (player->alive)
        {
            print_tablike(player);
        }
    }
}

/**
 * Stampa a video le informazione del player in modo carino.
 * @param player Il player.
 */
void print_tablike(Player *player)
{
    // Contatore.
    int i;

    printf("|");

    for (i = 0; i < TAB_CHARS; ++i)
    {
        printf("-");
    }

    printf("> ");
    print_player_name(player);
    print_blank();
}

/**
 * Esecutore di giochi.
 * @param curGame   Il gioco scelto.
 * @param groups    I gruppi di players.
 * @param rows      Il numero di gruppi.
 * @param size      La grandezza dei gruppi.
 * @param sizes     Il numero di players di ogni gruppo (opzionale).
 * @param game      Il gioco in corso.
 */
void play_game(Games curGame, Group *groups, int rows, int size, int *sizes, Game *game)
{
    /*
     * i, j             -> contatori.
     * toRemove         -> valore temporaneo per tenere gli id dei perdenti.
     *
     * group            -> gruppo dei player temporaneo.
     *
     * winner           -> vincitore del game temporaneo.
     * frontMan         -> il FrontMan del gioco in corso, se presente.
     *
     * fullOfBots       -> indica se il gruppo e' pieno di bots (true) o meno (false);
     * frontManPresent  -> indica se il FrontMan e' presente nel gruppo (true) o meno (false);
     */
    int i, j;

    int toRemove;

    Group *group;

    Player *winner;
    Player *frontMan = find_front_man(game);

    Boolean fullOfBots;
    Boolean frontManPresent;

    print_new_game(curGame);

    /* Fa giocare ogni gruppo al gioco scelto. */
    for (i = 0; i < rows; ++i)
    {
        group = &groups[i];

        /* Se il gioco ha un numero di player dinamici, rimpiazza la size con quella del gruppo corrente. */
        if (sizes != NULL)
        {
            size = sizes[i];
        }

        fullOfBots = group_full_of_bots(*group, size);

        /* Se il gioco e' composto da soli bot, genera casualmente il vincitore. */
        if (fullOfBots)
        {
            winner = play_bot_game(*group, size);
        }
        else
        {
            print_start_match(*group, size);

            frontManPresent = front_man_in_group(frontMan, *group, size);

            switch (curGame)
            {
                case DADI:
                    winner = play_dadi(*group);
                    break;
                case BIGLIE:
                    winner = play_biglie(*group);
                    break;
                case IMPICCATO:
                    winner = play_impiccato(*group);
                    break;
                case MORRA_CINESE:
                    winner = play_morra_cinese(*group);
                    break;
                case MURRA:
                    winner = play_murra(*group);
                    break;
                case PARI_O_DISPARI:
                    winner = play_pari_o_dispari(*group);
                    break;
                case PONTE_DI_VETRO:
                    winner = play_ponte_di_vetro(*group);
                    break;
                case TRIS:
                    winner = play_tris(*group);
                    break;
                case BLACKJACK:
                    winner = play_blackjack(*group);
                    break;
                case INDOVINA_NUMERO:
                    winner = play_indovina_numero(*group, size);
                    break;
                default:
                    break;
            }

            /* Se presente, il FrontMan vince sempre. */
            if (frontManPresent)
            {
                winner = frontMan;
            }

            print_end_match(winner);
        }

        /* Imposta lo stato di "morto" a tutti i perdenti. */
        for (j = 0; j < size; ++j)
        {
            toRemove = group->players[j].identifier;

            if (winner->identifier != toRemove)
            {
                GET(game->players, toRemove)->alive = false;
            }
        }
    }
}

/**
 * Gioca ogni gioco dello SPR1D-GAME.
 * @param game I gioco in corso.
 */
void play_games(Game *game)
{
    /*
     * i                -> contatore.
     * groupSize        -> la grandezza del gruppo.
     * size             -> il numero di players vivi.
     *
     * frontMan         -> il FrontMan, se presente.
     *
     * groups           -> i gruppi che giocheranno ai giochi.
     *
     * frontManExists   -> indica se il FrontMan esiste (true) o meno (false).
     * isFinal          -> indica se il gioco e' nella fase finale esiste (true) o meno (false).
     *
     * currentGame      -> il gioco che si giochera' nel turno.
     * games            -> lista per tenere traccia dei giochi ancora giocabili.
     */
    int i;
    int groupSize;
    int size = game->alivePlayers;

    Player *frontMan = NULL;

    Group *groups = NULL;

    Boolean frontManExists;
    Boolean isFinal;

    Games currentGame;
    Games *games = MALLOC_ARRAY(Games, NUM_GAMES); CRASH_IF_NULL(games)

    /* Inizializza l'array di giochi rimanenti da giocare. */
    for (i = 0; i < NUM_GAMES; ++i)
    {
        games[i] = i;
    }

    /* Controlla la presenza del FrontMan. */
    frontMan        = find_front_man(game);
    frontManExists  = frontMan != NULL;

    /* Aggiorna i dati e chiede dove salvarli. */
    update_totals(game, PLAYER_STATUSES);
    ask_save_stats(&game->status);

    // Aggiorna le statistiche dei profili in gioco.
    incr_stats(game, SPR1D_PLAYED);

    do
    {
        /*
         * Se i players rimanenti sono maggiori di DUO_GAME, e' necessario giocare altri giochi, altrimenti,
         * si giochera' la fase finale, il BLACKJACK.
         */
        if (size > DUO_GAME)
        {
            /*
             * Se i players rimanenti sono QUAD_GAME, segna ogni gioco da QUAD_GAME players come gia' giocato,
             * dato che, non sarebbe possibile giocare al BLACKJACK con solo 1 player.
             */
            if (size == QUAD_GAME)
            {
                for (i = 0; i < NUM_GAMES; ++i)
                {
                    if (is_game_quad(games[i]))
                    {
                        games[i] = NO_GAME;
                    }
                }
            }

            /*
             * Se il FrontMan esiste, gli chiede di scegliere il gioco, altrimenti, ne pesca uno casualmente.
             */
            if (frontManExists)
            {
                currentGame = front_man_chooses(games);
            }
            else
            {
                do
                {
                    currentGame = games[gen_num(0, NUM_GAMES - 1)];

                } while (currentGame == NO_GAME);
            }
        }
        else
        {
            currentGame = BLACKJACK;
            incr_stats(game, FINALS_PLAYED);
        }

        isFinal = currentGame == BLACKJACK;

        // Aggiorna le statistiche dei profili in gioco.
        incr_stats(game, GAMES_PLAYED);

        /* Genera i gruppi. */
        groupSize   = is_game_quad(currentGame) ? QUAD_GAME : DUO_GAME;
        groups      = gen_groups(game, groupSize);
        size        /= groupSize;

        print_groups(groups, size, groupSize);

        // Fa giocare tutti i gruppi allo stesso gioco.
        play_game(currentGame, groups, size, groupSize, NULL, game);

        print_winners(game);

        /* Aggiorna dati e statistiche dei profili in gioco. */
        update_totals(game, PLAYER_STATUSES);
        incr_stats(game, GAMES_WON);

        if (isFinal)
        {
            print_winner(game);

            /* Aggiorna dati e statistiche dei profili in gioco. */
            incr_stats(game, SPR1D_WON);
            update_totals(game, PLAYING);
        }

        ask_save_stats(&game->status);

        /* Libera la memoria. */
        free(groups);

    } while (!isFinal);

    free(games);
}

/**
 * Incrementa le varie statistiche dei vincitori.
 * @param game          Il gioco in corso.
 * @param statsUpdate   La statistica da incrementare.
 */
void incr_stats(Game *game, StatsUpdate statsUpdate)
{
    /*
     * i        -> contatore.
     *
     * profile  -> valore temporaneo.
     */
    int i;

    Profile *profile;

    for (i = 0; i < game->status.totHumans; ++i)
    {
        profile = GET(game->status.profiles, i);

        /* Se il profilo e' vivo, incrementa la statistica. */
        if (game->status.playerStatuses[profile->identifier])
        {
            switch (statsUpdate)
            {
                case FINALS_PLAYED:
                    profile->finalsPlayed++;
                    break;
                case SPR1D_WON:
                    profile->spr1dWon++;
                    break;
                case SPR1D_PLAYED:
                    profile->spr1dPlayed++;
                    break;
                case GAMES_WON:
                    profile->gamesWon++;
                    break;
                case GAMES_PLAYED:
                    profile->gamesPlayed++;
                    break;
                default:
                    break;
            }
        }
    }
}

/**
 * Chiede al FrontMan di scegliere il gioco da giocare.
 * @param gamesRemaining    I giochi che e' possibile scegliere.
 * @return                  Il gioco scelto dal FrontMan.
 */
Games front_man_chooses(Games *gamesRemaining)
{
    /*
     * i            -> contatore.
     * allocSize    -> il numero di giochi che e' possibile scegliere.
     * allowed      -> i giochi che e' possibile scegliere.
     */
    int i;
    int allocSize   = 0;
    int *allowed    = NULL;

    print_remaining_games(gamesRemaining);

    /* Imposta come valore ammesso ogni gioco che non e' gia' stato giocato (!NO_GAME). */
    for (i = 0; i < NUM_GAMES; ++i)
    {
        if (gamesRemaining[i] != NO_GAME)
        {
            if (++allocSize > 1)
            {
                allowed = REALLOC_ARRAY(int, allowed, allocSize); CRASH_IF_NULL(allowed)
            }
            else
            {
                allowed = MALLOC_ARRAY(int, allocSize); CRASH_IF_NULL(allowed)
            }

            allowed[allocSize - 1] = gamesRemaining[i];
        }
    }

    free(allowed);

    return ask_input_int(allowed, allocSize, LENGTH_ONE, false);
}

/**
 * Richiesta input di tipo int all'utente.
 * @param allowed       Array di int ammessi in input.
 * @param allowedSize   Grandezza dell'array.
 * @param inputLength   Numero di caratteri ammessi in input.
 * @param denyMinusThan Impedisce i numeri minori di esso di essere inseriti.
 * @return              Input inserito dall'utente
 */
int ask_input_int(int *allowed, int allowedSize, int inputLength, int denyMinusThan)
{
    /*
     * i                -> contatore.
     * value            -> il valore dell'azione selezionata dall'utente.
     * temp             -> valore temporaneo.
     * counter          -> contatore che indica se un numero era gia' stato generato in precedenza.
     *
     * allocSize        -> lunghezza temporanea della stringa.
     * actionLength     -> lunghezza della stringa in input.
     *
     * action           -> l'input dell'utente.
     *
     * valueIsAllowed   -> indica se il valore inserito è consentito.
     * valueInRange     -> indica se il valore inserito rientra nel range "allowed".
     */
    int i;
    int value, temp;

    unsigned allocSize;
    unsigned actionLength;

    char slice;
    char *action = NULL;

    Boolean valueIsAllowed;
    Boolean valueInRange = false;

    do
    {
        allocSize   = 1;
        slice       = '?';
        action      = MALLOC_ARRAY(char, allocSize); CRASH_IF_NULL(action)

        printf("Inserisci il numero:");

        /* Legge un carattere dall'input sino a quando esso non sia il terminatore di stringa. */
        while (slice != STR_TERM)
        {
            slice = getchar();

            /* Il carattere newline indichera' il terminatore della stringa in input. */
            if (slice == '\n')
            {
                slice = STR_TERM;
            }

            /* Per ogni nuovo carattere, ridimensiono la stringa che accogliera' l'input. */
            if (allocSize > 1)
            {
                action = REALLOC_ARRAY(char, action, allocSize); CRASH_IF_NULL(action)
            }

            action[allocSize - 1] = slice;
            allocSize++;
        }

        // Controlla se i caratteri inseriti sono consentiti.
        valueIsAllowed = check_characters(action, ALLOWED_NUMS);

        if (valueIsAllowed)
        {
            actionLength = strlen(action);

            /* Se l'input e' troppo grande, manda un messaggio d'errore e richiede l'input. */
            if (actionLength > inputLength)
            {
                print_err(TOO_BIG_NUM);
            }
            else
            {
                if (allowed != NULL)
                {
                    temp = AS_INT(action);

                    /* Controlla se il numero convertito e' presente nella lista allowed. */
                    for (i = 0; i < allowedSize; ++i)
                    {
                        if (temp == allowed[i])
                        {
                            valueInRange = true;
                        }
                    }

                    if (!valueInRange)
                    {
                        print_err(MUST_BE_EXACT_NUM);
                    }
                }
                else
                {
                    valueInRange = true;
                }

                // I caratteri numerici dell'azione vengono convertiti in int.
                value = AS_INT(action);

                if (denyMinusThan != false && value < denyMinusThan)
                {
                    print_err(TOO_LITTLE_NUM);
                    valueInRange = false;
                }
            }
        }
        else
        {
            print_err(ILLEGAL_CHARS);
        }

        free(action);

    } while (!valueIsAllowed || !valueInRange);

    return value;
}

/**
 * Richiesta input di tipo stringa all'utente.
 * @param allowed       Array di stringhe ammesse in input.
 * @param print         Caratteri da stampare a video.
 * @return              Input inserito dall'utente
 */
char *ask_input_str(char *allowed, char *print)
{
    /*
     * allocSize        -> lunghezza temporanea della stringa.
     *
     * slice            -> il carattere in lettura in input.
     * action           -> l'input dell'utente.
     *
     * valueIsAllowed   -> indica se il valore inserito è consentito.
     */
    int allocSize;

    char slice;
    char *action = NULL;

    Boolean valueIsAllowed;

    do
    {
        allocSize   = 1;
        slice       = '?';
        action      = MALLOC_ARRAY(char, allocSize); CRASH_IF_NULL(action)

        printf("Inserisci il %s:", print);

        /* Legge un carattere dall'input sino a quando esso non sia il terminatore di stringa. */
        while (slice != STR_TERM)
        {
            slice = getchar();

            /* Il carattere newline indichera' il terminatore della stringa in input. */
            if (slice == '\n')
            {
                slice = STR_TERM;
            }

            /* Per ogni nuovo carattere, ridimensiono la stringa che accogliera' l'input. */
            if (allocSize > 1)
            {
                action = REALLOC_ARRAY(char, action, allocSize); CRASH_IF_NULL(action)
            }

            action[allocSize - 1] = slice;
            allocSize++;
        }

        // Controlla se i caratteri inseriti sono consentiti.
        valueIsAllowed = check_characters(action, allowed);

        if (!valueIsAllowed)
        {
            print_err(ILLEGAL_CHARS);
            free(action);
        }

    } while (!valueIsAllowed);

    return action;
}

/**
 * Richiesta input di tipo stringa all'utente.
 * @param allowed       Array di stringhe ammesse in input.
 * @param print         Caratteri da stampare a video.
 * @param maxLength     Il numero massimo di caratteri che e' possibile inserire.
 * @return              Input inserito dall'utente
 */
char *ask_input_str_with_len(char *allowed, char *print, int maxLength)
{
    /*
     * allocSize        -> lunghezza temporanea della stringa.
     *
     * slice            -> il carattere in lettura in input.
     * action           -> l'input dell'utente.
     *
     * valueIsAllowed   -> indica se il valore inserito è consentito.
     */
    int allocSize;

    char slice;
    char *action = NULL;

    Boolean valueIsAllowed;

    do
    {
        allocSize   = 1;
        slice       = '?';
        action      = MALLOC_ARRAY(char, allocSize); CRASH_IF_NULL(action)

        printf("Inserisci il %s:", print);

        /* Legge un carattere dall'input sino a quando esso non sia il terminatore di stringa. */
        while (slice != STR_TERM)
        {
            slice = getchar();

            /* Il carattere newline indichera' il terminatore della stringa in input. */
            if (slice == '\n')
            {
                slice = STR_TERM;
            }

            if (maxLength != 0 && allocSize == maxLength)
            {
                slice = STR_TERM;
            }
            else
            {
                /* Per ogni nuovo carattere, ridimensiono la stringa che accogliera' l'input. */
                if (allocSize > 1)
                {
                    action = REALLOC_ARRAY(char, action, allocSize); CRASH_IF_NULL(action)
                }

                action[allocSize - 1] = slice;
                allocSize++;
            }

        }

        // Controlla se i caratteri inseriti sono consentiti.
        valueIsAllowed = check_characters(action, allowed);

        if (!valueIsAllowed)
        {
            print_err(ILLEGAL_CHARS);
            free(action);
        }

    } while (!valueIsAllowed);

    return action;
}

/**
 * Stampa a video un menu' e assegna a ogni campo un numero.
 * @param text  I testi da stampare.
 * @param rows  Il numero di campi disponibili.
 * @param len   Lunghezza massima input considerato (e' necessario che scali con rows).
 * @return      Il numero corrispondente al campo scelto.
 */
int ask_menu_choice(char *text[], int rows, int len)
{
    /*
     * i        -> contatore.
     * value    -> il valore dell'azione selezionata dall'utente.
     *
     * allowed  -> array con tutti i numeri ammessi.
     */
    int i;
    int value;

    int *allowed = MALLOC_ARRAY(int, rows); CRASH_IF_NULL(allowed)

    /*
     * Stampa a video tutte le azioni disponibili.
     */
    println_wqt(EMPTY_STRING, 2);
    print_fiorellini();
    println("Inserisci il numero dell'azione che desideri eseguire:");

    for (i = 0; i < rows; ++i)
    {
        printf("\t%d |> %s\n", i, text[i]);
    }

    /* Inizializza l'array. */
    for (i = 0; i < rows; ++i)
    {
        allowed[i] = i;
    }

    print_fiorellini();

    value = ask_input_int(allowed, rows, len, false);

    free(allowed);

    return value;
}

/**
 * Chiede all'utente quale profilo far giocare.
 * @param profiles      I profili.
 * @param numProfiles   Il numero di profili.
 * @return              Il numero corrispondente all'index profilo selezionato.
 */
void ask_which_profile_plays(Game *game)
{
    /*
     * allowedKeep      -> i valori ammessi in input.
     *
     * i                -> contatore.
     * chosenProfile    -> l'index corrispondente al profilo selezionato.
     * selectedProfiles -> numero di profili selezionati.
     * allowed          -> i valori ammessi in input.
     *
     * keepInsert       -> se continuare a selezionare profili (true) o meno (false).
     */
    static int allowedKeep[] = { ABORT_INSERT_VALUE, KEEP_INSERT_VALUE };

    int i;
    int chosenProfile;
    int selectedProfiles    = 0;
    int numProfiles         = game->status.numProfiles;
    int *allowed            = MALLOC_ARRAY(int, numProfiles); CRASH_IF_NULL(allowed)

    Boolean keepInsert;

    Profile *profiles = *game->status.profiles;

    /* Inizializza l'array. */
    for (i = 0; i < numProfiles; ++i)
    {
        allowed[i] = i;
    }

    do
    {
        keepInsert = selectedProfiles++ != numProfiles;

        if (keepInsert)
        {
            println_wqt(EMPTY_STRING, 2);
            print_fiorellini();
            println("Inserisci il numero del profilo che desideri far giocare:");

            /* Stampa a video ogni profilo che non e' gia' stato selezionato. */
            for (i = 0; i < numProfiles; ++i)
            {
                if (allowed[i] != USED_VALUE)
                {
                    printf("\t%d |> ", i);
                    print_profile_name(&profiles[i]);
                    print_blank();
                }
            }

            print_fiorellini();

            /* Chiede all'utente quale profilo far giocare e lo rende "used", inserendolo poi in gioco come player. */
            chosenProfile = ask_input_int(allowed, numProfiles, LENGTH_MAX, false);
            allowed[chosenProfile] = USED_VALUE;

            update_totals(game, TOT_PROFILES);
            insert_player_custom(game, &profiles[chosenProfile], true);

            /* Se e' ancora possibile, chiede all'utente se fermarsi o scegliere altri profili. */
            if (selectedProfiles != numProfiles)
            {
                println("\nDesideri continuare a inserire profili? (0 = stop, 1 = continua).");
                keepInsert = ask_input_int(allowedKeep, KEEP_ABORT_VALUES, LENGTH_ONE, false);
            }
        }

    } while (keepInsert);

    free(allowed);
}

/**
 * Controlla se un numero e' dispari.
 * @param num   Il numero da controllare.
 * @return      Se il numero e' dispari (true) o pari (false).
 */
Boolean is_num_odd(int num)
{
    return num % 2 == 1;
}

/**
 * Controlla se un gioco e' giocabile da QUAD_GAME players.
 * @param game  Il gioco da controllare.
 * @return      Se il gioco e' un quad-game (true) o duo-game (false);
 */
Boolean is_game_quad(Games game)
{
    return game == IMPICCATO || game == PONTE_DI_VETRO;
}

/**
 * Controlla se un player e' umano.
 * @param player    Il player da controllare.
 * @return          Se il player e' umano (true) o meno (false).
 */
Boolean is_human(Player *player)
{
    return player->name != NULL;
}

/**
 * Controlla se un player e' il FrontMan dello SPR1D GAME.
 * @param player    Il player da controllare.
 * @return          Se il player e' il FrontMan (true) o meno (false).
 */
Boolean is_front_man(Player *player)
{
    return is_human(player) && strcmp(player->name, FRONTMAN_NAME) == 0;
}

/**
 * Inizializzatore del gioco.
 * @return Il gioco parzialmente inizializzato.
 */
Game init_game()
{
    Game game = { };
    Status status = { };

    status.humanIds         = MALLOC_ARRAY(int, LENGTH_ONE); CRASH_IF_NULL(status.humanIds)
    status.profiles         = MALLOC_ARRAY(Profile *, LENGTH_ONE); CRASH_IF_NULL(status.profiles)
    game.status = status;

    game.players    = MALLOC_ARRAY(Player *, LENGTH_ONE); CRASH_IF_NULL(game.players)
    *game.players   = MALLOC_ARRAY(Player, LENGTH_ONE); CRASH_IF_NULL(*game.players)

    return game;
}

/**
 * Aggiorna i dati.
 * @param game      Il gioco in corso.
 * @param toUpdate  La categoria della quale aggiornare i dati.
 */
void update_totals(Game *game, ToUpdate toUpdate)
{
    /*
     * i, j, k  -> contatori e valori temporanei.
     *
     * isAlive  -> indica se il player e' vivo (true) o meno (false);
     *
     * status   -> lo stato del gioco in corso.
     * statuses -> gli stati dei giocatori.
     *
     * temp     -> valore temporaneo.
     */
    int i, j, k;

    Boolean isAlive;

    Status *status  = &game->status;
    int *statuses   = status->playerStatuses;

    Profile temp;

    switch (toUpdate)
    {
        case PLAYING:
            status->playing = !status->playing;

            /* Se il gioco e' terminato, imposta gli id di tutti i profili a NOT_PLAYING_ID. */
            if (!status->playing)
            {
                for (i = 0; i < status->numProfiles; ++i)
                {
                    GET(status->profiles, i)->identifier = NOT_PLAYING_ID;
                }
            }
            break;
        case NUM_PROFILES:
            status->numProfiles++;
            break;
        case TOT_PLAYERS:
            status->totPlayers++;
            break;
        case TOT_PROFILES:
            status->totHumans++;
            break;
        case STARTUP:
            /* Inizializza componenti del gioco in corso. */
            status->playerStatuses  = MALLOC_ARRAY(int, status->totPlayers); CRASH_IF_NULL(status->playerStatuses)
            status->humanIds        = REALLOC_ARRAY(int, status->humanIds, status->totHumans); CRASH_IF_NULL(status->humanIds)

            selection_sort_profiles(game);

            /* Popola l'indice dei profili con i profili che sono in gioco. */
            for (i = 0, k = 0; i < status->numProfiles; ++i, ++k)
            {
                j = GET(status->profiles, i)->identifier;

                if (j != NOT_PLAYING_ID)
                {
                    status->humanIds[k] = j;
                }
                else
                {
                    --k;
                }
            }

            update_totals(game, PLAYING);
            break;
        case PLAYER_STATUSES:
            game->alivePlayers  = 0;
            game->aliveHumans   = 0;

            /* Conta il numero di player in vita e aggiorna lo statuses. */
            for (i = 0; i < status->totPlayers; ++i)
            {
                isAlive = (*game->players)[i].alive;

                if (isAlive)
                {
                    game->alivePlayers++;
                }

                statuses[i] = isAlive;
            }

            /* Conta il numero di profili in vita. */
            for (i = 0; i < status->totHumans; ++i)
            {
                if (statuses[(*status->profiles)[i].identifier])
                {
                    game->aliveHumans++;
                }
            }
            break;
        default:
            break;
    }
}