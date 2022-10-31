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

/**
 * Pulisce la memoria occupata.
 * @param game Il gioco in corso.
 */
void clear_game(Game *game)
{
    free(game->status.humanIds);
    free(game->status.playerStatuses);

    free(*game->status.profiles);
    free(game->status.profiles);

    free(*game->players);
    free(game->players);
}

/**
 * Azione per aggiunge un nuovo profilo.
 * @param game  Il gioco in corso.
 * @return      Se il profilo e' valido (true) o meno (false).
 */
Boolean action_profile(Game *game)
{
    // Il numero di profili totali correnti, utile per verifica la validita' dell'input dell'utente.
    int oldSize = game->status.numProfiles;

    // Chiede all'utente d'inserire il nome per il nuovo profilo.
    char *playerName = ask_input_str(ALLOWED_CHARS, PRINT_NAME_PLAYER);

    insert_profile(game, playerName);

    free(playerName);

    /* Se il nome inserito e' valido, passa al secondo menu', con piu' opzioni disponibili */
    return game->status.numProfiles != oldSize;
}

/**
 * Azione per giocare.
 * @param game Il gioco in corso.
 */
void action_play(Game *game)
{
    ask_which_profile_plays(game);

    play_scrematura(game);
    play_games(game);

    clear_game(game);
}

/**
 * Azione per caricare un salvataggio.
 * @param game Il gioco in corso.
 */
void action_load(Game *game)
{
    /*
     * i, j         -> contatori.
     * totPlayers   -> il numero totale di players.
     *
     * found        -> indica se il profilo e' stato trovato (true) o meno (false).
     * alive        -> indica se il player e' vivo (true) o meno (false).
     *
     * tempProfile  -> valore temporaneo.
     */
    int i, j;

    int totPlayers;

    Boolean found = false;
    Boolean alive;

    Profile *tempProfile;

    read_game_status(game);
    print_game_status(game);

    /* Se il gioco e' gia' in corso, ricostruisce lo stato del game e continua la partita. */
    if (game->status.playing)
    {
        totPlayers = game->status.totPlayers;
        game->status.totPlayers = 0;

        for (i = 0; i < totPlayers; ++i)
        {
            alive = game->status.playerStatuses[i];

            /* Se il player e' vivo, controlla se esso e' un anche un profilo. */
            if (alive)
            {
                for (j = 0; j < game->status.totHumans && !found; ++j)
                {
                    tempProfile = GET(game->status.profiles, game->status.humanIds[j]);

                    if (tempProfile->identifier == i)
                    {
                        found = true;
                    }
                }
            }

            /* Se esiste anche il profilo, aggiunge il player basandosi su esso, altrimenti, lo crea in modo generico. */
            if (found)
            {
                insert_player_custom(game, tempProfile, true);
                found = false;
            }
            else
            {
                insert_player(game, NULL, true, alive);
            }
        }

        update_totals(game, PLAYER_STATUSES);

        println("Il gioco riprende!");
        play_games(game);

        clear_game(game);
    }
    else
    {
        action_play(game);
    }
}

int main()
{
    /*
     * firstActions     -> le azioni disponibili nel primo menu'.
     * secondActions    -> le azioni disponibili nel secondo menu'.
     *
     * quit             -> indica se e' stato scelto di uscire dal gioco (true) o meno (false).
     * firstMenu        -> indica se si e' ancora nel primo menu' (true) o meno (false).
     *
     * menu             -> il numero del menu' corrente.
     *
     * game             -> il gioco in corso.
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

    Boolean quit      = false;
    Boolean firstMenu = true;

    int menu;

    Game game = init_game();

    print_game_new();

    // Inizializzazione del seme casuale.
    srand(time(NULL));

    while (!quit)
    {
        if (firstMenu)
        {
            menu = ask_menu_choice(firstActions, FIRST_MENU_SIZE, LENGTH_ONE);

            switch (menu)
            {
                case 0:
                    action_profile(&game);
                    break;
                case 1:
                    action_load(&game);
                    quit = true;
                    break;
                case 2:
                    quit = true;
                    break;
                default:
                    break;
            }

            firstMenu = false;
        }
        else
        {
            menu = ask_menu_choice(secondActions, SECOND_MENU_SIZE, LENGTH_ONE);

            switch (menu)
            {
                case 0:
                    action_profile(&game);
                    break;
                case 1:
                    save_game_status(&game.status);
                    break;
                case 2:
                    action_play(&game);
                    quit = true;
                    break;
                case 3:
                    quit = true;
                    break;
                default:
                    break;
            }
        }
    }

    return 0;
}