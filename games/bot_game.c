#include "../utils/common.h"
#include "bot_game.h"

/**
 * Gioca al "gioco dei bots".
 * @param group     Il gruppo di players partecipanti.
 * @param size      La grandezza del gruppo.
 * @return          Il bot vincitore del match.
 */
Player *play_bot_game(Group group, int size)
{
    // L'index generato, che verra' usato per indicare il vincitore del match.
    int luckyOne = gen_num(0, size - 1);

    return &group.players[luckyOne];
}