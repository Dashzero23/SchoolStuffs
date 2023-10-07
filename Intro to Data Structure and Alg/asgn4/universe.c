#include "universe.h"

#include <stdlib.h>

uint32_t previous(uint32_t n, uint32_t i);
uint32_t next(uint32_t n, uint32_t i);

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *x = (Universe *) malloc(sizeof(Universe));
    x->rows = rows;
    x->cols = cols;
    x->toroidal = toroidal;
    x->grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t i = 0; i < rows; i++) {
        x->grid[i] = (bool *) calloc(cols, sizeof(bool));
    }
    return x;
}

void uv_delete(Universe *u) {
    for (uint32_t i = 0; i < u->rows; i++) {
        free(u->grid[i]);
        u->grid[i] = NULL;
    }
    free(u->grid);
    u->grid = NULL;
    free(u);
    u = NULL;
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    uint32_t neg = -1;
    if (r >= u->rows || c >= u->cols || r == neg || c == neg) {
        return;
    } else {
        u->grid[r][c] = true;
    }
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    uint32_t neg = -1;
    if (r >= u->rows || c >= u->cols || r == neg || c == neg) {
        ;
    } else {
        u->grid[r][c] = false;
    }
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    uint32_t neg = -1;
    if (r >= u->rows || c >= u->cols || r == neg || c == neg) {
        return false;
    }
    return u->grid[r][c];
}

bool uv_populate(Universe *u, FILE *infile) {
    uint32_t ro, col;
    uint32_t neg = -1;
    while (fscanf(infile, "%d %d\n", &ro, &col) != EOF) {
        if (ro >= u->rows || col >= u->cols || ro == neg || col == neg) {
            return false;
        } else {

            uv_live_cell(u, ro, col);
        }
    }
    return true;
}
uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    int counter = 0;
    if (u->toroidal == true) {
        if (u->grid[r][previous(u->cols, c)] == true) {
            counter++;
        }
        if (u->grid[r][next(u->cols, c)] == true) {
            counter++;
        }
        if (u->grid[previous(u->rows, r)][c] == true) {
            counter++;
        }
        if (u->grid[next(u->rows, r)][c] == true) {
            counter++;
        }
        if (u->grid[previous(u->rows, r)][previous(u->cols, c)] == true) {
            counter++;
        }
        if (u->grid[previous(u->rows, r)][next(u->cols, c)] == true) {
            counter++;
        }
        if (u->grid[next(u->rows, r)][previous(u->cols, c)] == true) {
            counter++;
        }
        if (u->grid[next(u->rows, r)][next(u->cols, c)] == true) {
            counter++;
        }

    } else {
        if (uv_get_cell(u, r, c - 1) == true) {
            counter++;
        }
        if (uv_get_cell(u, r, c + 1) == true) {
            counter++;
        }
        if (uv_get_cell(u, r - 1, c) == true) {
            counter++;
        }
        if (uv_get_cell(u, r + 1, c) == true) {
            counter++;
        }
        if (uv_get_cell(u, r - 1, c - 1) == true) {
            counter++;
        }
        if (uv_get_cell(u, r - 1, c + 1) == true) {
            counter++;
        }
        if (uv_get_cell(u, r + 1, c - 1) == true) {
            counter++;
        }
        if (uv_get_cell(u, r + 1, c + 1) == true) {
            counter++;
        }
    }
    return counter;
}
uint32_t previous(uint32_t max, uint32_t cur) {
    uint32_t prev = (max + cur - 1) % max;
    return prev;
}

uint32_t next(uint32_t max, uint32_t cur) {
    uint32_t next = (max + cur + 1) % max;
    return next;
}

void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t i = 0; i < u->rows; i++) {
        for (uint32_t j = 0; j < u->cols; j++) {
            if (u->grid[i][j] == false) {
                fprintf(outfile, ".");
            } else if (u->grid[i][j] == true) {
                fprintf(outfile, "o");
            }
        }
        fprintf(outfile, "\n");
    }
}
