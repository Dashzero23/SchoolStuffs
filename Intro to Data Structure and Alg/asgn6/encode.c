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

static inline int byte_to_bit_length(uint16_t byte) {
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
    // grab the input file permissions and write out the appropriate header
    struct stat prot;
    fstat(infile, &prot);

    FileHeader header = { 0, 0 };
    header.magic = MAGIC;
    header.protection = prot.st_mode;

    fchmod(outfile, header.protection);
    write_header(outfile, &header);

    // The encoding begins with the creation of a trie and nodes
    // that track the current and previous nodes in the trie
    TrieNode *root = trie_create();
    TrieNode *curr_node = root, *prev_node = NULL;

    uint8_t curr_sym = 0, prev_sym = 0;
    int next_code = START_CODE;

    while (read_sym(infile, &curr_sym)) {
        TrieNode *next_node = trie_step(curr_node, curr_sym);

        if (next_node != NULL) {
            prev_node = curr_node;
            curr_node = next_node;
        } else {
            write_pair(outfile, curr_node->code, curr_sym, byte_to_bit_length(next_code));
            curr_node->children[curr_sym] = trie_node_create(next_code);
            curr_node = root;
            next_code = next_code + 1;
        }

        if (next_code == MAX_CODE) {
            trie_reset(root);
            curr_node = root;
            next_code = START_CODE;
        }

        prev_sym = curr_sym;
    }

    if (curr_node != root) {
        write_pair(outfile, prev_node->code, prev_sym, byte_to_bit_length(next_code));
        next_code = (next_code + 1) % MAX_CODE;
    }

    // write the stop code to denote the end of compression, flush any
    // remaining pairs to the output file
    write_pair(outfile, STOP_CODE, 0, byte_to_bit_length(next_code));
    flush_pairs(outfile);

    // take out the trash
    trie_delete(root);
    close(infile);
    close(outfile);

    // print stats if verbose enabled
    if (verbose) {
        double saved_space = 100 * (1.0 - ((float) bit_to_byte(total_bits) / (float) total_syms));
        printf("Compressed file size: %" PRIu64 " bytes\n", bit_to_byte(total_bits));
        printf("Uncompressed file size: %" PRIu64 " bytes\n", total_syms);
        printf("Space saving: %.2f%%\n", saved_space);
    }

    return 0;
}
