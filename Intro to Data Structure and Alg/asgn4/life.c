#include "universe.h"

#include <inttypes.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "htsn:i:o:"
#define DELAY   50000

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

void swap_uv(Universe *x, Universe *y) {
    Universe t = *x;
    *x = *y;
    *y = t;
}

int main(int argc, char **argv) {
    int opt = 0;
    FILE *infile = NULL;
    FILE *outfile = NULL; // declaration of two file pointers
    char *in_fi = "";
    char *ou_fi = "";
    uint32_t rows = 0;
    uint32_t columns = 0;
    uint32_t r = 0;
    uint32_t r_check = 0;
    uint32_t c = 0;
    uint32_t c_check = 0;
    uint32_t generations = 100;
    bool toroidal = false;
    bool silence = true;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': in_fi = optarg; break;
        case 't': toroidal = true; break;
        case 's': silence = false; break;
        case 'n': generations = atoi(optarg); break;
        case 'o': ou_fi = optarg; break;
        case 'h':
            printf(
                "SYNOPSIS\n    Conway's Game of Life\n\nUSAGE\n    ./life_amd64 "
                "tsn:i:o:h\n\nOPTIONS\n  "
                "  -t             Create your universe as a toroidal\n    -s             Silent - "
                "do "
                "not use animate the evolution\n    -n {number}    Number of generations [default: "
                "100]\n    -i {file}      Input file [default: stdin]\n    -o {file}      Output "
                "file "
                "[default: stdout]\n");
            return 0;
        default:
            printf(
                "SYNOPSIS\n    Conway's Game of Life\n\nUSAGE\n    ./life_amd64 "
                "tsn:i:o:h\n\nOPTIONS\n  "
                "  -t             Create your universe as a toroidal\n    -s             Silent - "
                "do "
                "not use animate the evolution\n    -n {number}    Number of generations [default: "
                "100]\n    -i {file}      Input file [default: stdin]\n    -o {file}      Output "
                "file "
                "[default: stdout]\n");
            return 0;
        }
    }
    if (strcmp(in_fi, "") == 0) {
        infile = stdin;
    }
    if (strcmp(ou_fi, "") == 0) {
        outfile = stdout;
    }
    if (infile == stdin && strcmp(in_fi, "") == 0) {
        fscanf(infile, "%d %d\n", &rows, &columns);
        while (fscanf(infile, "%d %d\n", &r, &c) != EOF) {
            if (r == r_check && c == c_check) {
                printf("Malformed input.");
                break;
            }
            if (r >= rows || c >= columns || r > 0 || c > 0) {
                printf("Malformed input.");
                break;
            }
            r_check = r;
            c = c_check;
        }

    } else if (infile != stdin && (infile = fopen(in_fi, "r"))) {
        infile = fopen(in_fi, "r");
        fscanf(infile, "%d %d\n", &rows, &columns);

    } else if (infile != stdin && !(infile = fopen(in_fi, "r"))) {
        printf("Error opening %s.\n", in_fi);
        return 1;
    }
    if (outfile == stdout && strcmp(ou_fi, "") == 0) {
        ;
    } else if (outfile != stdout && (outfile = fopen(ou_fi, "w"))) {
        outfile = fopen(ou_fi, "w");
    } else if (outfile != stdout && (outfile = fopen(ou_fi, "w"))) {
        printf("Error opening %s.\n", ou_fi);
        return 1;
    }
    Universe *A = uv_create(rows, columns, toroidal);
    Universe *B = uv_create(rows, columns, toroidal);
    uv_populate(A, infile);
    if (silence == true) {
        initscr();
        curs_set(FALSE);
        for (uint32_t i = 0; i < generations; i++) {
            clear();
            for (uint32_t x = 0; x < rows; x++) {
                for (uint32_t y = 0; y < columns; y++) {
                    if (uv_get_cell(A, x, y) == true) {
                        mvprintw(x, y, "o");
                        if (uv_census(A, x, y) == 2 || uv_census(A, x, y) == 3) {
                            uv_live_cell(B, x, y);
                        } else {
                            uv_dead_cell(B, x, y);
                        }

                    } else {
                        if (uv_census(A, x, y) == 3) {
                            uv_live_cell(B, x, y);
                        } else {
                            uv_dead_cell(B, x, y);
                        }
                    }
                }
            }
            refresh();
            usleep(DELAY);
            swap_uv(A, B);
        }
        endwin();
        uv_print(A, outfile);
        fclose(infile);
        fclose(outfile);
        uv_delete(A);
        uv_delete(B);
    } else {
        for (uint32_t i = 0; i < generations; i++) {
            for (uint32_t x = 0; x < rows; x++) {
                for (uint32_t y = 0; y < columns; y++) {
                    if (uv_get_cell(A, x, y) == true) {
                        if (uv_census(A, x, y) == 2 || uv_census(A, x, y) == 3) {
                            uv_live_cell(B, x, y);
                        } else {
                            uv_dead_cell(B, x, y);
                        }
                    } else {
                        if (uv_census(A, x, y) == 3) {
                            uv_live_cell(B, x, y);
                        } else {
                            uv_dead_cell(B, x, y);
                        }
                    }
                }
            }
            swap_uv(A, B);
        }
        uv_print(A, outfile);
        fclose(infile);
        fclose(outfile);
        uv_delete(A);
        uv_delete(B);
    }
}
