#ifndef FINAL_MURRA_H
#define FINAL_MURRA_H

// Numero di valori della predizione.
#define PREDICTIONS_VALUES 9

/*
 * Valori che la predizione puo' assumere (range).
 */
#define PREDICT_SUM_MIN 2
#define PREDICT_SUM_MAX 10

// I valori della mano.
#define HAND_VALUES 5

/*
 * Valori che la mano puo' assumere (range).
 */
#define LOWEST_HAND_VALUE 1
#define HIGHEST_HAND_VALUE 5

Player *play_murra(Group group);

#endif //FINAL_MURRA_H
