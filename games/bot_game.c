#include "../utils/common.h"
#include "bot_game.h"

/**
 * Gioca al "gioco dei bots".
 * @param players   I bots partecipanti.
 * @param size      La grandezza del gruppo.
 * @return          Il bot vincitore del match.
 */
Player *play_bot_game(Player *players, int size)
{
    // L'index generato, che verra' usato per indicare il vincitore del match.
    int luckyOne = gen_num(0, size - 1);

    return &players[luckyOne];
}