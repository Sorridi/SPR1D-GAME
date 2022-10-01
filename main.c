/*
 * NOME DEL PROGRAMMA: SPR1D GAME
 * AUTORE: ANTONIO MASALA (MATRICOLA 66133)
 * SCOPO DEL PROGRAMMA: Giocare all'indie "SPR1D GAME"
 * IN DATA: 05-05-2022 (D-M-Y).
 */
#include "utils/common.h"
#include "utils/shortcuts.h"
#include "utils/scrematura.h"
#include "utils/file_manager.h"

int main()
{
    /*
     * i, j             -> contatori.
     *
     * firstActions     -> le entries del primo menu'.
     * secondActions    -> le entries del secondo menu'.
     *
     * startSize        -> il numero iniziale di players.
     * mutableSize      -> il numero di players man mano che si gioca.
     * choice           -> la scelta del menu'.
     * oldSize          -> la dimensione vecchia del numero di giocatori presenti, utile per verificare l'aggiunta.
     * profilesPlaying  -> i profili in gioco.
     *
     * stopInsert       -> indica se il menu' deve essere ancora proposto al giocatore.
     * isFirstMenu      -> indica se il menu' corrente e' il primo.
     * validInsert      -> indica se il giocatore proposto dal player e' valido.
     * found            -> indica se il valore e' stato trovato.
     *
     * players          -> la lista di players.
     * tempPlayers      -> la lista di players temporanea.
     *
     * playerName       -> valore temporaneo rappresentante il nome del player.
     *
     * gameStatus       -> lo stato del gioco.
     */
    static char *firstActions[FIRST_MENU_SIZE] =
            {
                    "Inserisci nuovo player",
                    "Carica un file di salvataggio",
                    "Chiudi il gioco"
            };
    static char *secondActions[SECOND_MENU_SIZE] =
            {
                    "Inserisci nuovo player",
                    "Salva i dati",
                    "Gioca la partita",
                    "Chiudi il gioco"
            };

    int i, j;
    int startSize   = 0;
    int mutableSize = 0;
    int choice;
    int oldSize;
    int *profilesPlaying;

    Boolean stopInsert      = false;
    Boolean isFirstMenu     = true;
    Boolean validInsert;
    Boolean found           = false;
    Boolean a = false;

    Player *players     = NULL;
    Player *tempPlayers = NULL;

    char *playerName    = NULL;

    GameStatus gameStatus = { };
    gameStatus.profiles         = MALLOC_ARRAY(Profile *, 1); CRASH_IF_NULL(gameStatus.profiles)
    gameStatus.humanIds         = CALLOC_ARRAY(int, 1); CRASH_IF_NULL(gameStatus.humanIds)
    gameStatus.playerStatuses   = CALLOC_ARRAY(int, 1); CRASH_IF_NULL(gameStatus.playerStatuses)

    // Inizializzazione del seme casuale.
    srand(time(NULL));

    print_fiorellini();
    println("\tGIOCO DELLO SPR1D GAME!");
    print_fiorellini();

    do
    {
        /*
         * Stampa il menu' e chiede l'input all'utente.
         */
        if (isFirstMenu)
        {
            choice = ask_menu_choice(firstActions, FIRST_MENU_SIZE, LENGTH_ONE);

            switch (choice)
            {
                case 0:
                    /*
                     * Chiede all'utente d'inserire il nome per il nuovo giocatore.
                     */
                    oldSize = gameStatus.numProfiles;

                    playerName = ask_input_str(ALLOWED_CHARS, PRINT_NAME_PLAYER);
                    insert_profile(&gameStatus, playerName);

                    validInsert = gameStatus.numProfiles != oldSize;

                    /* Se il nome inserito e' valido, passa al secondo menu', con piu' opzioni disponibili */
                    if (validInsert)
                    {
                        isFirstMenu = false;
                    }

                    free(playerName);
                    break;
                case 1:
                    readGameStatus(&gameStatus);
                    print_game_status(&gameStatus);

                    if (gameStatus.playing)
                    {
                        /*
                         * Tentativo fallito di far funzionare il load delle partite, la struttura era ben solida
                         * ed ho dovuto riscriverla in buona parte, purtroppo il mio sistema di giocatori non utilizza
                         * gli id come posizioni, ma da le posizioni a prescindere. :D
                         */
                        for (i = 0; i < gameStatus.totPlayers; i++)
                        {
                            for (j = 0; j < gameStatus.totHumans && !found; ++j)
                            {
                                if (gameStatus.playerStatuses[i])
                                {
                                    found = true;
                                    a = ((*gameStatus.profiles)[gameStatus.humanIds[j]].identifier == i);
                                }
                            }
                            j--;

                            if (found && a)
                            {
                                insert_player(&tempPlayers, &mutableSize, (*gameStatus.profiles)[gameStatus.humanIds[j]].name, true);
                            }
                            else if (found)
                            {
                                insert_player(&tempPlayers, &mutableSize, NULL, true);
                            }

                            found = false;
                        }

                        startSize = mutableSize;

                        println("Il gioco riprende!");
                        play_games(tempPlayers, mutableSize, startSize, &gameStatus, NULL);

                        free(players);
                        free(tempPlayers);
                        free(gameStatus.humanIds);
                        free(gameStatus.playerStatuses);
                        stopInsert = true;
                    }
                    else
                    {
                        isFirstMenu = false;
                    }
                    break;
                case 2:
                    free(players);
                    free(tempPlayers);
                    free(gameStatus.humanIds);
                    free(gameStatus.playerStatuses);
                    stopInsert = true;
                    break;
                default:
                    break;
            }
        }
        else
        {
            choice = ask_menu_choice(secondActions, SECOND_MENU_SIZE, LENGTH_ONE);

            switch (choice)
            {
                case 0:
                    /*
                     * Chiede all'utente d'inserire il nome per il nuovo giocatore.
                     */
                    playerName  = ask_input_str(ALLOWED_CHARS, PRINT_NAME_PLAYER);
                    insert_profile(&gameStatus, playerName);

                    free(playerName);
                    break;
                case 1:
                    saveGameStatus(&gameStatus);
                    break;
                case 2:
                    /*
                     * Chiede e aggiunge i profili che giocheranno lo SPR1D-GAME come players.
                     */
                    profilesPlaying = ask_which_profile_plays(&gameStatus);

                    for (i = 0; i < gameStatus.numProfiles; ++i)
                    {
                        if (profilesPlaying[i] == USED_VALUE)
                        {
                            insert_player(&players, &mutableSize, (*gameStatus.profiles)[i].name, true);
                        }
                    }

                    /* Gioca la scrematura e la partita. */
                    play_scrematura(&players, &mutableSize, &startSize, &gameStatus);
                    play_games(players, mutableSize, startSize, &gameStatus, NULL);
                case 3:
                    free(profilesPlaying);
                    free(players);
                    free(gameStatus.humanIds);
                    free(gameStatus.playerStatuses);
                    stopInsert  = true;
                    break;
                default:
                    break;
            }
        }
    } while (!stopInsert);

    return 0;
}