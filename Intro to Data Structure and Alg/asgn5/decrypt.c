#include "ss.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define OPTIONS "i:o:n:vh"

int main(int argc, char **argv) {
    int opt = 0;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *pvfile = NULL;
    bool in_check = false;
    bool out_check = false;
    char *priv_file = "ss.priv";

    char *input = NULL;
    char *output = NULL;
    int verbose = 0;
    // declaration of variables used based on the user's input on the command line
    mpz_t pq;
    mpz_init(pq);
    mpz_t d;
    mpz_init(d);
    // initialization of all mpz_ts being used

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
        case 'n': priv_file = optarg; break;
        case 'v': verbose = 1; break;
        case 'h':
            printf("SYNOPSIS\n   Decrypts data using SS decryption.\n   Encrypted "
                   "data is encrypted "
                   "by the encrypt program.\n\nUSAGE\n   ./decrypt "
                   "[OPTIONS]\n\nOPTIONS\n   -h        "
                   "      Display program help and usage.\n   -v              "
                   "Display verbose program "
                   "output.\n   -i infile       Input file of data to decrypt "
                   "(default: stdin).\n   "
                   "-o outfile      Output file for decrypted data (default: "
                   "stdout).\n   -n pvfile   "
                   "    Private key file (default: ss.priv).");
            return 0;
        } // switch statement of all the command line options
    }

    if (in_check) {
        if (fopen(input, "r") == NULL) {
            printf("Error opening the file %s or it is not present in the directory\n", input);
            return EXIT_FAILURE;
        } else {
            infile = fopen(input, "r");
        }
    }

    if (out_check) {
        if (fopen(output, "w") == NULL) {
            printf("Error opening the file %s or it is not present in the directory\n", output);
            return EXIT_FAILURE;
        } else {
            outfile = fopen(output, "w");
        }
    }

    if (fopen(priv_file, "r") == NULL) {
        printf("Error opening the file %s or it is not present in the directory\n", priv_file);
        return EXIT_FAILURE;
    }

    else {
        pvfile = fopen(priv_file, "r");
        ss_read_priv(pq, d, pvfile);
    }

    if (verbose == 1) { // checks if verbose was turned on
        gmp_printf("pq (%d bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    ss_decrypt_file(infile, outfile, d, pq);

    fclose(infile);
    fclose(outfile);
    fclose(pvfile);
    mpz_clear(pq);
    mpz_clear(d);
    // closes and clears all files and mpz_ts
}
