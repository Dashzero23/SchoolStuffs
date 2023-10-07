#include "code.h"
#include "io.h"
#include "trie.h"
#include "word.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define OPTIONS "vi:o:"

static inline int bit_length(uint16_t byte) {
    int bit_len = 0;

    while (byte > 0) {
        bit_len++;
        byte = byte >> 1;
    }

    return bit_len;
}

static inline uint64_t bit_to_byte(uint64_t bits) {
    uint64_t byte = bits / 8;
    return byte;
}

int main(int argc, char *argv[]) {
    // default i/o and verbose
    int opt = 0;
    int infile = STDIN_FILENO;
    int outfile = STDOUT_FILENO;
    bool in_check = false;
    bool out_check = false;
    char *input = NULL;
    char *output = NULL;
    bool verbose = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            input = optarg;
            in_check = true;
            break;
        case 'o':
            output = optarg;
            out_check = true;
            break;
        case 'v': verbose = true; break;
        }
    }
    if (in_check) {
        infile = open(input, O_RDONLY);
        if (infile == -1) {
            printf("Error opening the file %s\n", input);
            close(infile);
            return EXIT_FAILURE;
        }
    }

    if (out_check) {
        outfile = open(output, O_WRONLY | O_CREAT | O_TRUNC);
        if (outfile == -1) {
            printf("Error opening the file %s\n", output);
            close(outfile);
            return EXIT_FAILURE;
        }
    }
    FileHeader inheader = { 0, 0 };
    read_header(infile, &inheader);

    // not decodable if the magic does not match
    if (inheader.magic != MAGIC) {
        printf("Bad magic number\n");
        close(infile);
        close(outfile);
        return EXIT_FAILURE;
    }
    fchmod(outfile, inheader.protection);

    // decompression begins with the creation of an empty word table, and
    // trackers for the current symbol, current code, and next code
    WordTable *table = wt_create();
    uint8_t curr_sym = 0;
    uint16_t curr_code = 0;
    uint16_t next_code = START_CODE;

    while ((read_pair(infile, &curr_code, &curr_sym, bit_length(next_code)))) {
        table[next_code] = word_append_sym(table[curr_code], curr_sym);
        write_word(outfile, table[next_code]);
        next_code += 1;

        if (next_code == MAX_CODE) {
            wt_reset(table);
            next_code = START_CODE;
        }
    }

    // flush any reminaing words out to output file
    flush_words(outfile);

    wt_delete(table);
    close(infile);
    close(outfile);

    //print stats if verbose enabled
    if (verbose) {
        double saved_space = 100 * (1.0 - ((float) bit_to_byte(total_bits) / (float) total_syms));
        printf("Compressed file size: %" PRIu64 " bytes\n", bit_to_byte(total_bits));
        printf("Uncompressed file size: %" PRIu64 " bytes\n", total_syms);
        printf("Space saving: %.2f%%\n", saved_space);
    }

    return 0;
}
