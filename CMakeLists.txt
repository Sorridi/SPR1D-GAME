cmake_minimum_required(VERSION 3.20)
project(final C)

set(CMAKE_C_STANDARD 90)

# utile per usare valgrind su linux
add_compile_options(-g -Og)

add_executable(
        SPR1D
        main.c
        utils/shortcuts.c
        utils/shortcuts.h
        utils/common.h
        utils/common.c
        games/dadi.c
        games/pari_o_dispari.c
        games/murra.c
        games/biglie.c
        games/morra_cinese.c
        games/impiccato.c
        games/tris.c
        games/ponte_di_vetro.c
        games/dadi.h
        games/biglie.h
        games/impiccato.h
        games/morra_cinese.h
        games/tris.h
        games/ponte_di_vetro.h
        games/pari_o_dispari.h
        games/murra.h
        games/blackjack.c
        games/blackjack.h
        games/indovina_numero.c
        games/indovina_numero.h
        utils/scrematura.c
        utils/file_manager.c
        utils/file_manager.h
        utils/scrematura.h
        games/bot_game.c
        games/bot_game.h)

# utile per aggiungere math.h#pow() anche su gcc linux
target_link_libraries(SPR1D PRIVATE m)