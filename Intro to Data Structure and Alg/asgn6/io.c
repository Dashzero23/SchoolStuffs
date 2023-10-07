#include "io.h"

#include "code.h"
#include "endian.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BITSIZE 8 // size of a char in bits

uint64_t total_syms = 0;
uint64_t total_bits = 0;

static uint8_t word_buf[BLOCK];
static int word_buf_index = 0;

static uint8_t bit_buf[BLOCK] = { 0 };
static int bit_index = 0;

// wrapper function for syscall read(), used to buffer BLOCK of data
// from an input file
//
int read_bytes(int infile, uint8_t *buf, int to_read) {
    ssize_t j = 0;
    ssize_t x = 0;

    while ((j = read(infile, buf + x, to_read - x)) > 0) {
        x += j;
    }
    if (x == to_read) {
        return to_read;
    } else {
        return x;
    }
}

int write_bytes(int outfile, uint8_t *buf, int to_write) {
    ssize_t j = 0;
    ssize_t x = 0;

    while ((j = write(outfile, buf + x, to_write - x)) > 0) {
        x += j;
    }
    if (x == to_write) {
        return to_write;
    } else {
        return x;
    }
}

// Reads file header and verifies magic number
void read_header(int infile, FileHeader *header) {
    int num_bytes = read_bytes(infile, (uint8_t *) header, sizeof(FileHeader));
    // Check if the read was successful
    if (num_bytes != sizeof(FileHeader)) {
        printf("Error reading Header");
        return;
    }
    // Swap endianness if necessary
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }
    if (header->magic != 0xBAADBAAC) {
        printf("Invalid Magic Number");
        return;
    }
    total_bits += (sizeof(FileHeader) * BITSIZE);
}

// Writes file header with swapped endianness if necessary
void write_header(int outfile, FileHeader *header) {
    // Swap endianness if necessary
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }

    int written = write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));
    // Check if all bytes were written
    if (written != sizeof(FileHeader)) {
        // Handle write error
        printf("Error writing header");
        exit(EXIT_FAILURE);
    }
    total_bits += (sizeof(FileHeader) * BITSIZE);
}

// Reads a symbol and updates end-of-buffer flag
bool read_sym(int infile, uint8_t *sym) {
    static uint8_t buf[BLOCK];
    static int pos = 0;
    static int end = 0;
    if (pos == end) {
        end = read_bytes(infile, buf, BLOCK);
        pos = 0;
        if (end == 0) {
            return false;
        }
    }
    *sym = buf[pos++];
    total_syms += 1;
    return true;
}
// writes out code and sym pairs to an output file
//
void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {
    int pair_size = (bitlen + 7) / 8;
    if (big_endian()) {
        swap16(code);
    }

    for (int i = 0; i < bitlen; i++) {
        if (bit_index == BITSIZE * BLOCK) {
            write_bytes(outfile, bit_buf, BLOCK);
            memset(bit_buf, 0, BLOCK);
            bit_index = 0;
        }

        if (code >> (i % 16) != 0) {
            bit_buf[bit_index / 8] |= (1 << (bit_index % 8));
        }

        bit_index += 1;
    }

    for (int i = 0; i < BITSIZE; i++) {
        if (bit_index == BITSIZE * BLOCK) {
            write_bytes(outfile, bit_buf, BLOCK);
            memset(bit_buf, 0, BLOCK);
            bit_index = 0;
        }

        if (((sym >> (i % 8)) & 1) != 0) {
            bit_buf[bit_index / 8] |= (1 << (bit_index % 8));
        }

        bit_index += 1;
    }

    total_bits += (bitlen + BITSIZE) * (pair_size - 1) + bitlen + BITSIZE;
}

// flushes any remaining pairs from the bit buffer
//
void flush_pairs(int outfile) {
    int to_write = (bit_index + 7) / 8;
    write_bytes(outfile, bit_buf, to_write);
}

// buffers code and sym pairs from an input file and then outputs the code
// and sym to their specified parameters, respectively
//
bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {
    *code = 0, *sym = 0;

    for (int i = 0; i < bitlen; i++) {
        if (!bit_index) {
            read_bytes(infile, bit_buf, BLOCK);
        }

        *code |= ((bit_buf[bit_index / 8] >> (bit_index % 8)) & 1) << i;
        bit_index = (bit_index + 1) % (BITSIZE * BLOCK);
    }

    for (int i = 0; i < BITSIZE; i++) {
        if (!bit_index) {
            read_bytes(infile, bit_buf, BLOCK);
        }

        *sym |= ((bit_buf[bit_index / 8] >> (bit_index % 8)) & 1) << i;
        bit_index = (bit_index + 1) % (BITSIZE * BLOCK);
    }

    if (big_endian()) {
        *code = swap16(*code);
    }

    total_bits += (bitlen + BITSIZE);

    return *code != STOP_CODE;
}

// buffers word syms, writes them to an output file when full
//
void write_word(int outfile, Word *w) {
    //printf("%" PRIu32 "\n", w->len);
    for (uint32_t i = 0; i < w->len; i++) {
        word_buf[word_buf_index++] = w->syms[i];
        if (word_buf_index == BLOCK) {
            write(outfile, word_buf, BLOCK);
            word_buf_index = 0;
        }
    }
    total_syms += w->len;
}

void flush_words(int outfile) {
    if (word_buf_index > 0) {
        write(outfile, word_buf, word_buf_index);
        word_buf_index = 0;
    }
}
