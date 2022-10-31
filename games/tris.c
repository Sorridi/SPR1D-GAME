/*
 * REGOLE:
 * I due giocatori dovranno mettere, a turno, un proprio segnaposto in una cella non ancora occupata di
 * una matrice 3x3. Vince chi ottiene per primo una riga, una colonna o una diagonale interamente
 * occupata dal proprio segnaposto. In caso di pareggio, si inizia una nuova partita finché non ci sarà un
 * vincitore. Il primo player a iniziare sarà quello con numero identificativo minore; nelle eventuali
 * partite successive si inizierà a turno.
 */
#include "../utils/common.h"
#include "../utils/shortcuts.h"
#include "tris.h"
#include "ponte_di_vetro.h"

/**
 * Chiede al player di piazzare la pedina.
 * @param isHuman   Se il player e' umano (true) o meno (false).
 * @param table     La tabella.
 * @return          Le coordinate y-x della pedina piazzata.
 */
int *ask_place_pawn(Boolean isHuman, Pawn **table)
{
    /*
     * allowed  -> i valori ammessi in input.
     *
     * y, x     -> coordinate.
     * yx       -> coordinate dello slot.
     *
     * success  -> indica se la generazione casuale dello slot e' andata a buon fine (slot libero).
     */
    static int allowed[TABLE_SIDE] = { 0, 1, 2 };

    int y, x;
    int *yx = MALLOC_ARRAY(int, 2); CRASH_IF_NULL(yx)

    Boolean success;

    /* Se il player e' umano, chiede l'input. */
    if (isHuman)
    {
        println("Scegli la colonna dove inserire la tua pedina:");
        yx[0] = ask_input_int(allowed, TABLE_SIDE, LENGTH_ONE, false);

        println("Scegli la riga dove inserire la tua pedina:");
        yx[1] = ask_input_int(allowed, TABLE_SIDE, LENGTH_ONE, false);
    }
    else
    {
        /* Altrimenti, incomincia la generazione casuale alla ricerca di uno slot non occupato. */
        do
        {
            y = gen_num(LOWEST_POSITION, HIGHEST_POSITION);
            x = gen_num(LOWEST_POSITION, HIGHEST_POSITION);

            success = table[y][x] == EMPTY;

        } while (!success);

        /* La generazione ha trovato degli slot liberi, aggiorna le coordinate di destinazione. */
        yx[0] = x;
        yx[1] = y;
    }

    return yx;
}

/**
 * Piazza la pedina nello slot y-x.
 * @param table La tabella.
 * @param pawn  La pedina da piazzare.
 * @param yx    Le coordinate dello slot.
 * @return      Se la pedina e' stata piazzata (true) o meno (false).
 */
Boolean place_pawn(Pawn **table, Pawn pawn, int *yx)
{
    /*
     * x    -> coordinata della colonna.
     * y    -> coordinata della riga.
     *
     * res  -> risultato dello piazzamento.
     */
    int x = yx[0];
    int y = yx[1];

    Boolean res = false;

    if (table[y][x] == EMPTY)
    {
        table[y][x] = pawn;
        res = true;
    }

    return res;
}

/**
 * Controlla la pedina diagonalmente.
 * @param table     La tabella.
 * @param toCheck   La pedina.
 * @return          Se la pedina ha eseguito una serie di 3 (true) o meno (false).
 */
Boolean check_diagonal(Pawn **table, Pawn toCheck)
{
    // Se esiste la serie di pedine per vincere il gioco.
    Boolean res = false;

    /* Le serie da 3 diagonali occupano sempre lo slot centrale della tabella. */
    if (table[1][1] == toCheck)
    {
        res = (table[0][0] == toCheck && table[2][2] == toCheck) || (table[2][0] == toCheck && table[0][2] == toCheck);
    }

    return res;
}

/**
 * Controlla la pedina verticalmente.
 * @param table     La tabella.
 * @param toCheck   La pedina.
 * @return          Se la pedina ha eseguito una serie di 3 (true) o meno (false).
 */
Boolean check_vertical(Pawn **table, Pawn toCheck)
{
    /*
     * i, j     -> contatori.
     * counter  -> contatore per il numero di pedine.
     */
    int i, j;
    int counter = 0;

    /* Controlla ogni riga e colonna. */
    for (i = 0; i < TABLE_SIDE; ++i)
    {
        for (j = 0; j < TABLE_SIDE; ++j)
        {
            /* Se la pedina nello slot e' quella desiderata, incrementa la serie. */
            if (table[j][i] == toCheck)
            {
                counter++;
            }
        }

        if (counter == PAWN_SERIES_WIN)
        {
            return true;
        }

        counter = 0;
    }

    return false;
}

/**
 * Controlla la pedina orizzontalmente.
 * @param table     La tabella.
 * @param toCheck   La pedina.
 * @return          Se la pedina ha eseguito una serie di 3 (true) o meno (false).
 */
Boolean check_horizontal(Pawn **table, Pawn toCheck)
{
    /*
     * i, j     -> contatori.
     * counter  -> contatore per il numero di pedine.
     */
    int i, j;
    int counter = 0;

    /* Controlla ogni riga e colonna. */
    for (i = 0; i < TABLE_SIDE; ++i)
    {
        for (j = 0; j < TABLE_SIDE; ++j)
        {
            /* Se la pedina nello slot e' quella desiderata, incrementa la serie. */
            if (table[i][j] == toCheck)
            {
                counter++;
            }
        }

        if (counter == PAWN_SERIES_WIN)
        {
            return true;
        }

        counter = 0;
    }

    return false;
}

/**
 * Controlla la tabella in cerca di serie da 3 (orizzontale, verticale, diagonale).
 * @param table     La tabella.
 * @param toCheck   La pedina.
 * @return          Se esiste una serie da 3 (true) o meno (false).
 */
Boolean check_table(Pawn **table, Pawn toCheck)
{
    return check_horizontal(table, toCheck) || check_vertical(table, toCheck) || check_diagonal(table, toCheck);
}

/**
 * Controlla se la tabella e' piena di pedine.
 * @param table La tabella.
 * @return Se la tabella e' piena (true) o meno (false).
 */
Boolean check_table_filled(Pawn **table)
{
    /*
     * i, j     -> contatori.
     * target   -> l'area della tabella.
     * counter  -> contatore di slot occupati.
     */
    int i, j;
    int target  = TABLE_SIDE * TABLE_SIDE;
    int counter = 0;

    /* Controlla ogni riga e colonna. */
    for (i = 0; i < TABLE_SIDE; ++i)
    {
        for (j = 0; j < TABLE_SIDE; ++j)
        {
            /* Se la pedina nello slot non e' occupata, incrementa il contatore. */
            if (table[i][j] != EMPTY)
            {
                counter++;
            }
        }
    }

    return counter == target;
}

/**
 * Pulisce la tabella dalle pedine.
 * @param table La tabella.
 */
void clear_table(Pawn **table)
{
    // Contatori.
    int i, j;

    /* Seleziona ogni riga e pulisce ogni colonna. */
    for (i = 0; i < TABLE_SIDE; ++i)
    {
        for (j = 0; j < TABLE_SIDE; ++j)
        {
            table[i][j] = EMPTY;
        }
    }
}

/**
 * Stampa a video lo stato della tabella.
 * @param table La tabella.
 */
void print_table(Pawn **table)
{
    /*
     * values   -> il corrispondente in stringa di "Pawn".
     *
     * i, j     -> contatori
     */
    static const char *values[PAWN_VALUES] = { " ", "O", "X" };

    int i, j;

    // Stampa a video la numerazione delle colonne.
    println("    0   1   2");

    /* Stampa il valore dello slot per ogni riga e colonna. */
    for (i = 0; i < TABLE_SIDE; ++i)
    {
        // Stampa a video la numerazione delle righe.
        printf("%d |", i);

        for (j = 0; j < TABLE_SIDE; ++j)
        {
            /* Evita di stampare altri divisori quando non sono necessari. */
            if (j != 0)
            {
                printf("|");
            }

            // Stampa il valore della pedina.
            printf(" %s ", values[table[i][j]]);
        }

        printf("|");
        print_blank();
    }

    print_blank();
}

/**
 * Fa giocare ad un player il turno di Tris.
 * @param player    Il player.
 * @param table     La tabella.
 * @param pawn      La pedina del player.
 * @param isHuman   Se il player e' umano (true) o meno (false).
 * @return          Se il player ha vinto il match (true) o meno (false).
 */
Boolean turn_of(Player *player, Pawn **table, Pawn pawn, Boolean isHuman)
{
    /*
     * pawnPosition -> le coordinate y-x della pedina.
     *
     * outcome      -> indica se il valore inserito dal player e' andato a buon fine.
     */
    int *pawnPosition;

    Boolean outcome;

    print_turn_of(player);

    do
    {
        /* Chiede la posizione nella quale piazzare la pedina. */
        pawnPosition    = ask_place_pawn(isHuman, table);
        outcome         = place_pawn(table, pawn, pawnPosition);

        if (!outcome)
        {
            println("Non puoi piazzare la pedina in questa casella!");
        }

        // Dato che la variabile e' stata allocata dinamicamente dentro "place_pawn(...)", libero la memoria.
        free(pawnPosition);

    } while (!outcome);

    print_table(table);

    return check_table(table, pawn);
}

/**
 * Gioca a tris.
 * @param group     Il gruppo di players partecipanti.
 * @return          Il player vincitore del match.
 */
Player *play_tris(Group group)
{
    /*
     * i                        -> contatore.
     *
     * player1, player2         -> players partecipanti.
     * winner                   -> player vincitore del match.
     *
     * gameEnded                -> indica se il gioco e' terminato.
     * tableFilled              -> indica se la tabella e' piena di pedine.
     * isP1Human, isP2Human     -> indicano se i player sono umani.
     * startCon                 -> indica se il player che deve iniziare e' diverso da quello default.
     *
     * table                    -> la tabella da gioco.
     */
    int i;

    Player *player1 = &group.players[0];
    Player *player2 = &group.players[1];
    Player *winner;

    Boolean gameEnded;
    Boolean tableFilled;
    Boolean isP1Human   = is_human(player1);
    Boolean isP2Human   = is_human(player2);
    Boolean startCon    = player1->identifier > player2->identifier;

    Pawn **table = MALLOC_ARRAY(Pawn*, TABLE_SIDE); CRASH_IF_NULL(table)

    /* Inizializza gli elementi della matrice. */
    for (i = 0; i < TABLE_SIDE; ++i)
    {
        table[i] = CALLOC_ARRAY(Pawn, TABLE_SIDE); CRASH_IF_NULL(table[i])
    }

    /* Se la condizione di precedenza delle REGOLE non e' stata rispettata all'inizializzazione, rimedia. */
    if (startCon)
    {
        player1 = &group.players[1];
        player2 = &group.players[0];

        isP1Human = is_human(player1);
        isP2Human = is_human(player2);
    }

    print_table(table);

    do
    {
        /*
         * Dopo aver fatto giocare il primo player controlla se
         * ha vinto il gioco seguendo la condizione di vittoria delle REGOLE,
         * in caso positivo, constata il vincitore del match, altrimenti,
         * fa giocare il secondo player e ricontrolla per una nuova possibile vincita.
         */
        gameEnded = turn_of(player1, table, PAWN_PLAYER1, isP1Human);

        if (gameEnded)
        {
            winner = player1;
        }
        else
        {
            /*
             * Controlla se la tabella ha tutte le caselle piene, in caso positivo,
             * ricomincia il gioco, altrimenti, continua il turno.
             *
             * NOTA: Non e' necessario eseguire questo check anche per il secondo player, dato che un pareggio puo'
             * avvenire solo al termine del turno del primo player.
             */
            tableFilled = check_table_filled(table);

            if (tableFilled)
            {
                println("PAREGGIO! SI RICOMINCIA!\n");
                clear_table(table);
            }
            else
            {
                gameEnded = turn_of(player2, table, PAWN_PLAYER2, isP2Human);

                if (gameEnded)
                {
                    winner = player2;
                }
            }
        }
    } while (!gameEnded);

    /* Libera la memoria. */
    FREE_MATRIX(i, TABLE_SIDE, table);

    return winner;
}
