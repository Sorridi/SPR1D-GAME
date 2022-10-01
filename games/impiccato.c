/*
 * REGOLE:
 * I 4 giocatori dovranno cercare d'indovinare una parola (o frase) selezionata a caso da un dizionario di
 * parole. All’inizio, sarà noto solamente da quante lettere è composta la parola (verrà visualizzato un
 * numero di caratteri “_” pari al numero di lettere della parola). A turno, i giocatori inseriranno una lettera
 * che pensano possa essere presente, e il gioco verificherà se tale lettera è presente nella parola. Se è
 * presente, viene visualizzata la parola e i caratteri “_” saranno sostituiti con la lettera inserita, e il turno
 * passa al giocatore successivo. Se la lettera non è presente, il giocatore ha fallito e il turno passerà
 * comunque al giocatore successivo. Ogni giocatore ha un numero massimo di fallimenti pari a 4,
 * dopodiché verrà eliminato. Vince il giocatore che completa la parola (indovina l’ultima lettera che rivela
 * la parola finale), oppure l’ultimo superstite.
 */
#include "../utils/common.h"
#include "../utils/shortcuts.h"
#include "../utils/file_manager.h"
#include "impiccato.h"
#include "ponte_di_vetro.h"

/**
 * Stampa a video il player che ha utilizzato un carattere gia' esistente.
 * @param player Il player.
 */
void print_last_man(Player *player)
{
    print_player_name(player);
    println("e' il 'last man standing'!\n\n");
}

/**
 * Stampa a video il player che ha utilizzato un carattere gia' esistente.
 * @param player Il player.
 */
void print_character_used(Player *player)
{
    print_player_name(player);
    println("questo carattere e' gia' stato utilizzato!\n\n");
}

/**
 * Stampa a video il player eliminato.
 * @param player Il player.
 */
void print_player_eliminated(Player *player)
{
    print_player_name(player);
    println("e' stato eliminato!\n\n");
}

/**
 * Stampa a video il player che ha sbagliato lettera.
 * @param player Il player.
 */
void print_player_error(Player *player, int score, char letter)
{
    print_player_name(player);
    printf("la lettera '%c' non e' presente! (errori: %d su %d)\n\n\n", letter, score, MAX_TRIES);
}

/**
 * Stampa a video il player che ha indovinato la lettera.
 * @param player Il player.
 */
void print_player_ok(Player *player, char letter)
{
    print_player_name(player);
    printf("la lettera '%c' e' presente! Fantastico!\n\n\n", letter);
}

/**
 * Chiede al player di scegliere una lettera.
 * @param isHuman   Se il player e' umano (true) o meno (false).
 * @return          La lettera scelta.
 */
char ask_char(Boolean isHuman)
{
    /*
     * res          -> il carattere scelto.
     *
     * fullInput    -> l'input completo del player, che verra' parzialmente ignorato.
     */
    char res;

    char *fullInput;

    /* Se il player e' umano, chiede l'input, altrimenti genera un carattere. */
    if (isHuman)
    {
        fullInput = ask_input_str(ALLOWED_CHARS_NOSPACES, PRINT_CHAR_IMPICCATO);
        res = fullInput[0];

        free(fullInput);
    }
    else
    {
        res = NUM_TO_CHAR_OFFSET + gen_num(0, ALPHABET_LENGTH - 1);
    }

    return res;
}

/**
 * Imposta la lettera come utilizzata.
 * @param phrase    La frase segreta.
 * @param toUse     La lettera da utilizzare.
 * @return          Se il carattere non era utilizzato (true) o meno (false).
 */
Boolean use_char(Phrase *phrase, char toUse)
{
    /*
     * i    -> contatore.
     * with -> il carattere senza offset (maiuscolo -> minuscolo).
     *
     * used -> indica se il carattere era presente.
     */
    int i;
    int with = toUse - HL_OFFSET;

    Boolean used = false;

    for (i = 0; i < ALPHABET_LENGTH; ++i)
    {
        if (toUse == phrase->availableChars[i] || with == phrase->availableChars[i])
        {
            phrase->availableChars[i] = HIDDEN_CHAR;

            used    = true;
            i       = ALPHABET_LENGTH;
        }
    }

    return used;
}

/**
 * Rimpiazza ogni carattere corrispondente.
 * @param phrase    La frase segreta.
 * @param toReplace Il carattere da rimpiazzare.
 */
void replace_chars(Phrase *phrase, char toReplace)
{
    /*
     * i    -> contatore.
     * with -> il carattere senza offset (maiuscolo -> minuscolo).
     */
    int i;
    int with = toReplace - HL_OFFSET;

    for (i = 0; i < phrase->contentLength; ++i)
    {
        if (phrase->content[i] == toReplace)
        {
            phrase->contentHidden[i] = toReplace;
        }
        else if (phrase->content[i] == with)
        {
            phrase->contentHidden[i] = (char) with;
        }
    }
}

/**
 * Crea un dizionario vuoto.
 * @return Il dizionario creato.
 */
FileDictionary createEmptyDictionary()
{
    /*
     * i                -> contatore.
     *
     * fileDictionary   -> il dizionario.
     *
     * contents         -> il contenuto ti tutto il dizionario.
     */
    int i;

    FileDictionary fileDictionary;

    char **contents = MALLOC_ARRAY(char *, MAX_DICTIONARY_ENTRIES); CRASH_IF_NULL(contents)

    /* Inizializza i contenuti. */
    for (i = 0; i < MAX_DICTIONARY_ENTRIES; ++i)
    {
        contents[i] = CALLOC_ARRAY(char, MAX_ENTRY_LENGTH); CRASH_IF_NULL(contents[i])
    }

    /* Inizializza i dati. */
    fileDictionary.rows     = 0;
    fileDictionary.name     = DEFAULT_DICTIONARY;
    fileDictionary.contents = contents;

    return fileDictionary;
}

/**
 * Controlla se il carattere e' presente nella frase segreta.
 * @param phrase    La frase segreta.
 * @param toCheck   Il carattere da controllare.
 * @return          Se il carattere e' presente (true) o meno (false).
 */
Boolean char_present(Phrase *phrase, char toCheck)
{
    /*
     * i            -> contatore.
     * with         -> il carattere senza offset (maiuscolo -> minuscolo).
     *
     * isPresent    -> indica se la lettere e' presente.
     */
    int i;
    int with = toCheck - HL_OFFSET;

    Boolean isPresent = false;

    /* Controlla se il carattere e' presente. */
    for (i = 0; i < phrase->contentLength; ++i)
    {
        /* Controlla sia in caso il carattere sia minuscolo che maiuscolo. */
        if (phrase->content[i] == toCheck || phrase->content[i] == with)
        {
            isPresent   = true;
            i           = phrase->contentLength;
        }
    }

    return isPresent;
}

/**
 * Controlla se la frase e' effettivamente ancora segreta (ha ancora caratteri nascosti).
 * @param phrase    La frase segreta.
 * @return          Se la frase e' ancora segreta (true) o meno (false).
 */
Boolean is_hidden(Phrase *phrase)
{
    /*
     * i        -> contatore.
     *
     * isSecret -> indica se la parola e' ancora segreta.
     */
    int i;

    Boolean isSecret = false;

    /* Controlla se esiste un carattere nascosto. */
    for (i = 0; i < phrase->contentLength; ++i)
    {
        if (phrase->contentHidden[i] == HIDDEN_CHAR)
        {
            isSecret    = true;
            i           = phrase->contentLength;
        }
    }

    return isSecret;
}

/**
 * Crea una frase segreta.
 * @param fileDictionary    Il dizionario
 * @return                  La frase segreta creata.
 */
Phrase createEmptyPhrase(FileDictionary *fileDictionary)
{
    /*
     * i, j                 -> contatori.
     * generated            -> il numero generato utilizzato come index.
     *
     * piece                -> lettera temporanea.
     *
     * phrase               -> la frase segreta.
     *
     * alphabetIncluded     -> indica se e' incluso nell'alfabeto.
     */
    int i, j;
    int generated = gen_num(0, fileDictionary->rows - 1);

    char piece;

    Phrase phrase;

    Boolean alphabetIncluded = false;

    /*
     * Inizializza i dati.
     */
    phrase.content          = fileDictionary->contents[generated];
    phrase.contentLength    = strlen(phrase.content);
    phrase.contentHidden    = CALLOC_ARRAY(char, phrase.contentLength + 1); CRASH_IF_NULL(phrase.contentHidden)
    strcpy(phrase.contentHidden, phrase.content);
    phrase.availableChars   = CALLOC_ARRAY(char, ALPHABET_LENGTH + 1); CRASH_IF_NULL(phrase.availableChars)
    strcpy(phrase.availableChars, ALPHABET);
    
    /* Nasconde ogni carattere dell'alfabeto nella frase che verra' mostrata ai players. */
    for (i = 0; i < phrase.contentLength; ++i)
    {
        piece = phrase.content[i];

        for (j = 0; j < ALPHABET_LENGTH; ++j)
        {
            /* Controlla il carattere in caso fosse minuscolo e maiuscolo. */
            if ((piece == ALPHABET[j]) || (piece + HL_OFFSET == ALPHABET[j]))
            {
                alphabetIncluded = true;
                j = ALPHABET_LENGTH;
            }
        }

        /* Se presente nell'alfabeto lo nasconde, altrimenti lascia il carattere originario. */
        if (alphabetIncluded)
        {
            piece = HIDDEN_CHAR;
        }

        phrase.contentHidden[i] = piece;
        alphabetIncluded        = false;
    }

    return phrase;
}

/**
 * Stampa a video la frase segreta.
 * @param phrase La frase segreta.
 */
void print_phrase(Phrase *phrase)
{
    /*
     * i            -> contatore.
     * count        -> il numero di caratteri ancora disponibili.
     */
    int i;

    println("La frase segreta:\n");
    println(phrase->contentHidden);

    printf("\nLe lettere che non sono state ancora usate sono: ");

    /* Stampa ogni carattere che e' ancora disponibile. */
    for (i = 0; i < ALPHABET_LENGTH; ++i)
    {
        printf("%c ", phrase->availableChars[i]);
    }

    print_blank();
    print_blank();
}

/**
 * Stampa a video la frase (non piu') segreta.
 * @param phrase La frase segreta.
 */
void print_unhidden_phrase(Phrase *phrase)
{
    printf("La frase segreta era: ");
    println(phrase->content);
}

/**
 * Gioca a impiccato.
 * @param players   I player partecipanti.
 * @return          Il player vincitore del match.
 */
Player* play_impiccato(Player *players)
{
    /*
     * i                -> contatore.
     * playerScores     -> gli stati dei players.
     *
     * player           -> il player del turno corrente.
     * alivePlayer      -> l'ultimo player rimanente.
     * winner           -> il player vincitore del match.
     *
     * gameEnded        -> indica se il gioco e' terminato.
     * isUsed           -> indica se il carattere e' gia' stato utilizzato.
     * isHidden         -> indica se la frase e' ancora segreta.
     * isPresent        -> indica se il carattere scelto e' presente nella frase.
     * isLastPlayer     -> indica se esiste solo 1 player rimanente.
     *
     * fileDictionary   -> il dizionario.
     *
     * phrase           -> la frase segreta.
     */
    int i, j;
    int currentScore;
    int counter         = 0;
    int *playerScores   = CALLOC_ARRAY(int, QUAD_GAME); CRASH_IF_NULL(playerScores)

    char input;

    Player *player;
    Player *alivePlayer;
    Player *winner;

    Boolean gameEnded   = false;
    Boolean isUsed;
    Boolean isHidden;
    Boolean isPresent;
    Boolean isLastPlayer;

    FileDictionary fileDictionary = createEmptyDictionary();
    readDictionary(&fileDictionary);

    Phrase phrase = createEmptyPhrase(&fileDictionary);

    do
    {
        /* Fa giocare ogni player fino a quando il gioco non termina */
        for (i = 0; i < QUAD_GAME; ++i)
        {
            player          = &players[i];
            currentScore    = playerScores[i];

            if (currentScore != MAX_TRIES)
            {
                print_turn_of(player);
                print_phrase(&phrase);

                input       = ask_char(is_human(player));
                isPresent   = char_present(&phrase, input);

                /* Se il carattere non esiste, incrementa il numero di errori del player. */
                if (!isPresent)
                {
                    currentScore++;
                    playerScores[i] = currentScore;

                    /* In caso il player ha fatto il numero massimo di errori, esso viene eliminato. */
                    if (currentScore == MAX_TRIES)
                    {
                        print_player_eliminated(player);

                        /* Controlla se esistono altri player in gioco. */
                        for (j = 0; j < QUAD_GAME; ++j)
                        {
                            if (playerScores[j] == MAX_TRIES)
                            {
                                counter++;
                            }
                            else
                            {
                                alivePlayer = &players[j];
                            }
                        }

                        isLastPlayer = counter == DELETED_PLAYER_TO_WIN;

                        /* Se non esistono altri player in gioco, determina il vincitore. */
                        if (isLastPlayer)
                        {
                            winner      = alivePlayer;
                            gameEnded   = true;
                            i           = QUAD_GAME;
                            print_last_man(winner);
                        }

                        counter = 0;
                    }
                    else
                    {
                        print_player_error(player, playerScores[i], input);
                    }
                }
                else
                {
                    isUsed = use_char(&phrase, input);

                    /* Se il carattere e' stato utilizzato, lo rimpiazza. */
                    if (isUsed)
                    {
                        replace_chars(&phrase, input);
                        print_player_ok(player, input);

                        isHidden = is_hidden(&phrase);

                        /* Se la frase non e' piu' nascosta, determina il vincitore. */
                        if (!isHidden)
                        {
                            winner      = player;
                            gameEnded   = true;

                            print_unhidden_phrase(&phrase);
                        }
                    }
                    else
                    {
                        print_character_used(player);
                    }
                }
            }
        }
    } while (!gameEnded);

    /* Libera la memoria */
    free(playerScores);
    free(phrase.contentHidden);
    free(phrase.availableChars);
    FREE_MATRIX(i, MAX_DICTIONARY_ENTRIES, fileDictionary.contents)

    return winner;
}