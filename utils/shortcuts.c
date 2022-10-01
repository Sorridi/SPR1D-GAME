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
#include "file_manager.h"

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
 * @param players   Gli sfidanti.
 * @param size      Il numero di sfidanti.
 */
void print_start_match(Player *players, int size)
{
    // Contatore.
    int i;

    printf("MATCH: ");

    /* Stampa a video il nome dei player del match. */
    for (i = 0; i < size; ++i)
    {
        print_player_name(&players[i]);

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
    // Il nome di ogni gioco.
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
     * gameNames    -> il nome di ogni gioco, esclusi BLACKJACK e INDOVINA_NUMERO.
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

    /* Stampa a video il nome di ogni gioco che non e' stato ancora giocato. */
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
    // Ogni messaggio di errore corrispondente all'errori.
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
                    "Un giocatore con lo stesso nome e' gia' presente!"
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
void print_groups(Player **groups, int rows, int columns)
{
    // Contatori.
    int i, j;

    println("\nI gruppi per il prossimo gioco saranno i seguenti:");

    /* Stampa a video ogni gruppo. */
    for (i = 0; i < rows; ++i)
    {
        printf("\nGRUPPO: %d\n", i);

        for (j = 0; j < columns; ++j)
        {
            print_tablike(&groups[i][j]);
        }
    }

    print_blank();
}

/**
 * Stampa a video il vincitore dello SPR1D-GAME.
 * @param winner Il player vincitore.
 */
void print_winner(Player *winner)
{
    printf("\nIL VINCITORE DELLO SPR1D-GAME E': ");
    print_player_name(winner);
    print_blank();
}

/**
 * Stampa a video i vincitori di ogni gruppo.
 * @param winners   I player vincitori.
 * @param size      Il numero di vincitori.
 */
void print_winners(Player *winners, int size)
{
    // Contatore.
    int i;

    println("\nVINCITORI DEL GIOCO:");

    for (i = 0; i < size; ++i)
    {
        print_tablike(&winners[i]);
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
 * @param game              Il gioco scelto.
 * @param gamesRemaining    I giochi rimanenti.
 * @param groups            I players partecipanti.
 * @param rows              Il numero di gruppi.
 * @param size              La grandezza dei gruppi.
 * @param sizes             Il numero di players.
 * @return                  I players vincitori.
 */
Player *play_game(Games game, Games *gamesRemaining, Player **groups, int rows, int size, int *sizes)
{
    /*
     * i            -> contatore.
     *
     * group        -> gruppo dei player temporaneo.
     * winner       -> vincitore del game temporaneo.
     * winners      -> array di player che hanno vinto il gioco.
     * frontMan     -> il FrontMan, se presente.
     *
     * fullOfBots   -> indica se il gruppo e' pieno di bots (true) o meno (false);
     */
    int i;

    Player *group;
    Player *winner;
    Player *winners     = MALLOC_ARRAY(Player, rows); CRASH_IF_NULL(winners)
    Player *frontMan    = NULL;

    Boolean fullOfBots;

    print_new_game(game);

    /* Fa giocare ogni gruppo al gioco scelto. */
    for (i = 0; i < rows; ++i)
    {
        group = groups[i];

        if (sizes != NULL)
        {
            size = sizes[i];
        }

        fullOfBots = group_full_of_bots(group, size);

        if (fullOfBots)
        {
            winner = play_bot_game(group, size);
        }
        else
        {
            print_start_match(group, size);

            frontMan = find_front_man(group, size);

            switch (game)
            {
                case DADI:
                    winner = play_dadi(group);
                    break;
                case BIGLIE:
                    winner = play_biglie(group);
                    break;
                case IMPICCATO:
                    winner = play_impiccato(group);
                    break;
                case MORRA_CINESE:
                    winner = play_morra_cinese(group);
                    break;
                case MURRA:
                    winner = play_murra(group);
                    break;
                case PARI_O_DISPARI:
                    winner = play_pari_o_dispari(group);
                    break;
                case PONTE_DI_VETRO:
                    winner = play_ponte_di_vetro(group);
                    break;
                case TRIS:
                    winner = play_tris(group);
                    break;
                case BLACKJACK:
                    winner = play_blackjack(group);
                    break;
                case INDOVINA_NUMERO:
                    winner = play_indovina_numero(group, size);
                    break;
                default:
                    break;
            }

            /* Se presente, il FrontMan vince sempre. */
            if (frontMan != NULL)
            {
                winner = frontMan;
            }

            print_end_match(winner);
        }

        winners[i] = *winner;
    }

    if (gamesRemaining != NULL && game < NUM_GAMES)
    {
        // Non sara' piu' possibile giocare al gioco appena giocato.
        gamesRemaining[game] = NO_GAME;
    }

    return winners;
}

/**
 * Gioca ogni gioco dello SPR1D-GAME.
 * @param profiles      I profili.
 * @param startPlayers  I players iniziali.
 * @param statuses      Gli statuses dei players.
 * @param currentSize   Il numero di players attuali.
 * @param startSize     Il numero di players iniziali.
 * @param gameStatus    Lo stato corrente del game.
 */
void play_games(Player *startPlayers, int currentSize, int startSize, GameStatus *gameStatus, Player *eliminatedPlayers)
{
    /*
     * i                -> contatore.
     * groupSize        -> la grandezza del gruppo.
     *
     * currentPlayers   -> l'array mutabile di players.
     * frontMan         -> il FrontMan, se presente.
     * groups           -> i gruppi di players che giocheranno ai giochi.
     * winners          -> i vincitori di ogni gruppo.
     *
     * frontManExists   -> indica se il FrontMan esiste (true) o meno (false).
     *
     * currentGame      -> il gioco che si giochera' nel turno.
     * games            -> lista per tenere traccia dei giochi ancora giocabili.
     */
    int i, j;
    int groupSize;
    int *statuses = gameStatus->playerStatuses;

    Player *currentPlayers  = NULL;
    Player *frontMan        = NULL;
    Player *winners         = NULL;
    Player **groups         = NULL;

    Boolean frontManExists;
    Boolean isFinal;
    Boolean found;

    Games currentGame;
    Games *games = MALLOC_ARRAY(Games, NUM_GAMES); CRASH_IF_NULL(games)

    /* Inizializza l'array di giochi rimanenti da giocare. */
    for (i = 0; i < NUM_GAMES; ++i)
    {
        games[i] = i;
    }

    /* Inizializzo i player dell'array mutabile. */
    if (eliminatedPlayers != NULL)
    {
        currentSize = 0;

        for (i = 0; i < startSize; ++i)
        {
            for (j = 0; j < startSize; ++j)
            {
                if (startPlayers[i].identifier == eliminatedPlayers[j].identifier)
                {
                    printf("%d == %d\n", startPlayers[i].identifier, eliminatedPlayers[j].identifier);
                    found = true;
                    j = startSize;
                }
            }

            if (!found)
            {
                printf("NOT ELIMINATE %s\n", startPlayers[i].name);

                if (currentSize++ == 0)
                {
                    currentPlayers = MALLOC_ARRAY(Player, currentSize); CRASH_IF_NULL(currentPlayers)
                }
                else
                {
                    currentPlayers = REALLOC_ARRAY(Player, currentPlayers, currentSize); CRASH_IF_NULL(currentPlayers)
                }

                currentPlayers[currentSize - 1] = startPlayers[i];
            }

            found = false;
        }

        currentSize++;
    }
    else
    {
        currentPlayers = MALLOC_ARRAY(Player, startSize); CRASH_IF_NULL(currentPlayers)

        for (i = 0; i < currentSize; ++i)
        {
            currentPlayers[i] = startPlayers[i];
        }
    }

    /* Controlla la presenza del FrontMan. */
    frontMan        = find_front_man(currentPlayers, currentSize);
    frontManExists  = frontMan != NULL;

    // Aggiorna le varie statistiche.
    update_totals(currentPlayers, currentSize, gameStatus);

    /* Aggiorna l'array di statuses impostando i player vivi ad: ALIVE. */
    update_player_statuses(currentPlayers, currentSize, statuses, startSize);
    gameStatus->playing = true;

    ask_save_stats(gameStatus);

    do
    {
        /*
         * Se i players rimanenti sono maggiori di DUO_GAME, e' necessario giocare altri giochi, altrimenti,
         * si giochera' la fase finale, il BLACKJACK.
         */
        if (currentSize > DUO_GAME)
        {
            /*
             * Se i players rimanenti sono QUAD_GAME, segna ogni gioco da QUAD_GAME players come gia' giocato,
             * dato che, non sarebbe possibile giocare al BLACKJACK con solo 1 player.
             */
            if (currentSize == QUAD_GAME)
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
        }

        isFinal = currentGame == BLACKJACK;

        // Aggiorna le varie statistiche.
        incr_stats(gameStatus, currentPlayers, currentSize, isFinal);

        /* Genera i gruppi. */
        groupSize   = is_game_quad(currentGame) ? QUAD_GAME : DUO_GAME;
        groups      = gen_groups(currentPlayers, currentSize, groupSize);
        currentSize /= groupSize;

        print_groups(groups, currentSize, groupSize);

        /* Fa giocare tutti i gruppi allo stesso gioco. */
        winners = play_game(currentGame, games, groups, currentSize, groupSize, NULL);

        /* I player vincitori giocheranno ai giochi seguenti. */
        currentPlayers = REALLOC_ARRAY(Player, currentPlayers, currentSize); CRASH_IF_NULL(currentPlayers)

        /* Crea un mirror dei vincitori nei player correnti. */
        for (i = 0; i < currentSize; ++i)
        {
            currentPlayers[i] = winners[i];
        }

        print_winners(currentPlayers, currentSize);

        /* Aggiorna l'array di statuses impostando i player vivi ad: ALIVE. */
        update_player_statuses(currentPlayers, currentSize, statuses, startSize);

        // Aggiorna le varie statistiche.
        update_totals(currentPlayers, currentSize, gameStatus);
        incr_stats_winners(gameStatus, currentPlayers, currentSize, isFinal);

        if (isFinal)
        {
            gameStatus->playing = false;
            print_winner(&winners[0]);
        }

        ask_save_stats(gameStatus);

        /* Libera la memoria. */
        FREE_MATRIX(i, currentSize, groups)
        free(winners);

    } while (!isFinal);

    free(games);
    free(statuses);
    free(currentPlayers);
}

/**
 * Pulisci-schermo rudimentale.
 */
void clear_screen()
{
    println_wqt(EMPTY_STRING, CLEAR_SCREEN_LINES);
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
     *
     * chosenGame   -> il gioco scelto.
     */
    int i;
    int allocSize   = 0;
    int *allowed;

    Games chosenGame;

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

    chosenGame = ask_input_int(allowed, allocSize, LENGTH_ONE, false);

    free(allowed);

    return chosenGame;
}

/**
 * Richiesta input di tipo int all'utente.
 * @param allowed       Array di int ammessi in input.
 * @param allowedSize   Grandezza dell'array.
 * @param inputLength   Numero di caratteri ammessi in input.
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
int *ask_which_profile_plays(GameStatus *gameStatus)
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
    int numProfiles         = gameStatus->numProfiles;
    int *allowed            = MALLOC_ARRAY(int, numProfiles); CRASH_IF_NULL(allowed)

    Boolean keepInsert;

    Profile *profiles = *gameStatus->profiles;

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

            /* Chiede all'utente quale profilo far giocare e lo rende "used". */
            chosenProfile = ask_input_int(allowed, numProfiles, LENGTH_MAX, false);
            allowed[chosenProfile] = USED_VALUE;

            print_profile_name(&profiles[chosenProfile]);
            println("e' stato scelto per scendere in battaglia!");

            /* Se e' ancora possibile, chiede all'utente se fermarsi o scegliere altri profili. */
            if (selectedProfiles != numProfiles)
            {
                println("\nDesideri continuare a inserire profili? (0 = stop, 1 = continua).");
                keepInsert = ask_input_int(allowedKeep, KEEP_ABORT_VALUES, LENGTH_ONE, false);
            }
        }

    } while (keepInsert);

    return allowed;
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
    return player->name != NULL && strcmp(player->name, FRONTMAN_NAME) == 0;
}