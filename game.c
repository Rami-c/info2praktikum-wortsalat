#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RAND_TRIES_PER_WORD 10
#define EMPTY_CHAR 0

typedef struct {
    int zeile;
    int spalte;
} Position;

typedef enum {
    RICHTUNG_HORIZONTAL = 0,
    RICHTUNG_VERTIKAL   = 1
} Richtung;

// Creates the word salad by placing words randomly and filling empty spaces
int createWordSalad(char salad[MAX_SEARCH_FIELD_LEN][MAX_SEARCH_FIELD_LEN],
                    unsigned int searchFieldLen,
                    const char words[][MAX_WORD_LEN],
                    unsigned int wordCount)
{
    unsigned int i, j;

    // 1. Feld initialisieren (alles leer machen)
    for (i = 0; i < searchFieldLen; i++) {
        for (j = 0; j < searchFieldLen; j++) {
            salad[i][j] = EMPTY_CHAR;
        }
    }

    int placedWords = 0;

    // 2. Alle Wörter versuchen zu platzieren
    if (wordCount > 0) {
        for (unsigned int w = 0; w < wordCount; w++) {

            const char *word = words[w];
            size_t len = strlen(word);

            // Wörter, die länger als das Spielfeld sind, können nicht platziert werden
            if (len == 0 || len > searchFieldLen) {
                continue;
            }

            int placed = 0;

            // Mehrere zufällige Versuche pro Wort
            for (int attempt = 0; attempt < MAX_RAND_TRIES_PER_WORD && !placed; attempt++) {

                Richtung dir = (rand() % 2 == 0) ? RICHTUNG_HORIZONTAL : RICHTUNG_VERTIKAL;
                Position pos;

                if (dir == RICHTUNG_HORIZONTAL) {
                    int maxStartCol = (int)searchFieldLen - (int)len;
                    if (maxStartCol < 0) {
                        break; // passt sowieso nicht
                    }

                    pos.zeile  = rand() % (int)searchFieldLen;
                    pos.spalte = rand() % (maxStartCol + 1);

                } else { // vertikal
                    int maxStartRow = (int)searchFieldLen - (int)len;
                    if (maxStartRow < 0) {
                        break; // passt sowieso nicht
                    }

                    pos.zeile  = rand() % (maxStartRow + 1);
                    pos.spalte = rand() % (int)searchFieldLen;
                }

                // Prüfen, ob das Wort an dieser Position kollisionsfrei passt
                int ok = 1;
                for (size_t k = 0; k < len; k++) {
                    int r = pos.zeile + (dir == RICHTUNG_VERTIKAL   ? (int)k : 0);
                    int c = pos.spalte + (dir == RICHTUNG_HORIZONTAL ? (int)k : 0);

                    char existing = salad[r][c];
                    if (existing != EMPTY_CHAR && existing != word[k]) {
                        ok = 0;
                        break;
                    }
                }

                if (ok) {
                    // Wort tatsächlich eintragen
                    for (size_t k = 0; k < len; k++) {
                        int r = pos.zeile + (dir == RICHTUNG_VERTIKAL   ? (int)k : 0);
                        int c = pos.spalte + (dir == RICHTUNG_HORIZONTAL ? (int)k : 0);
                        salad[r][c] = word[k];
                    }
                    placed = 1;
                    placedWords++;
                }
            }
        }
    }

    // 3. Alle noch leeren Felder mit zufälligen Buchstaben füllen
    for (i = 0; i < searchFieldLen; i++) {
        for (j = 0; j < searchFieldLen; j++) {
            if (salad[i][j] == EMPTY_CHAR) {
                salad[i][j] = 'A' + (rand() % 26);
            }
        }
    }

    return placedWords;
}

// Prints the word salad to console
void showWordSalad(const char salad[MAX_SEARCH_FIELD_LEN][MAX_SEARCH_FIELD_LEN],
                   unsigned int searchFieldLen)
{
    for (unsigned int i = 0; i < searchFieldLen; i++) {
        for (unsigned int j = 0; j < searchFieldLen; j++) {
            printf("%c ", salad[i][j]);
        }
        printf("\n");
    }
}
