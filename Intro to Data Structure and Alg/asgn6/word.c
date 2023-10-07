#include "word.h"

#include "code.h"

#include <stdio.h>
#include <stdlib.h>

// constructor for a Word ADT
// FIX
Word *word_create(uint8_t *syms, uint32_t len) {
    Word *w = malloc(sizeof(Word));
    if (w == NULL) {
        return NULL;
    }

    w->len = len;

    w->syms = syms ? calloc(len, sizeof(uint8_t)) : NULL;
    if (w->syms != NULL) {
        for (uint32_t i = 0; i < len; i++) {
            w->syms[i] = syms[i];
        }
    }

    return w;
}

// creates a new word using the blueprint of the word passed in,
// appends the sym that is specified in the parameters
// FIX
Word *word_append_sym(Word *w, uint8_t sym) {
    uint32_t len = w->len + 1;
    Word *new_word = word_create(NULL, len);

    new_word->syms = calloc(new_word->len, sizeof(uint8_t));
    for (uint32_t i = 0; i < w->len; i++) {
        new_word->syms[i] = w->syms[i];
    }

    new_word->syms[w->len] = sym; // appending the new symbol

    return new_word;
}

// destructor function for a Word ADT
//
void word_delete(Word *w) {
    if (w != NULL) {
        if (w->syms != NULL) {
            free(w->syms);
            w->syms = NULL; // set syms array value NULL
        }

        free(w);
    }
}

// constructor for a word table ADT, an abstraction of a word
// FIX
WordTable *wt_create(void) {
    WordTable *wt = calloc(MAX_CODE, sizeof(Word));
    if (!wt) {
        return NULL;
    }

    for (uint16_t i = 0; i < MAX_CODE; i++) {
        wt[i] = NULL;
    }

    wt[EMPTY_CODE] = word_create(NULL, 0);
    if (!wt[EMPTY_CODE]) {
        for (uint16_t i = 0; i < MAX_CODE; i++) {
            if (wt[i]) {
                word_delete(wt[i]);
            }
        }
        free(wt);
        return NULL;
    }

    return wt;
}

// resets a wordtable to contain only the default STOP_CODE and
// EMPTY_CODE
//
void wt_reset(WordTable *wt) {
    for (int i = START_CODE; i < MAX_CODE; i++) {
        if (wt[i] != NULL) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }
}

// destructor for the entire WordTable ADT
//
void wt_delete(WordTable *wt) {
    if (wt != NULL) {
        for (int i = STOP_CODE; i < MAX_CODE; i++) {
            word_delete(wt[i]); // delete each word of entire table
            wt[i] = NULL;
        }
        free(wt);
    }
}
