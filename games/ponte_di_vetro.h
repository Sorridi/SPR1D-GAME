#ifndef FINAL_PONTE_DI_VETRO_H
#define FINAL_PONTE_DI_VETRO_H

/*
 * Lunghezza e larghezza del ponte.
 */
#define BRIDGE_LENGTH 3
#define BRIDGE_WIDTH 2

// Numero di valori del vetro.
#define GLASS_VALUES 2

/*
 * Proprieta' del vetro.
 */
#define GLASS_FRAGILE 0
#define GLASS_TEMPERED 1

/*
 * Valori del vetro del ponte per l'input.
 */
#define LEFT_GLASS 0
#define RIGHT_GLASS 1

void print_turn_of(Player *player);

Player *play_ponte_di_vetro(Player *players);

#endif //FINAL_PONTE_DI_VETRO_H
