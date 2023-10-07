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
    FILE *pbfile = NULL;
    bool in_check = false;
    bool out_check = false;
    char *pub_file = "ss.pub";
    char *username = getenv("USER");

    char *input = NULL;
    char *output = NULL;
    int verbose = 0;
    // declaration of variables used based on the user's input on the command line
    mpz_t n;
    mpz_init(n);
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
        case 'n': pub_file = optarg; break;
        case 'v': verbose = 1; break;
        case 'h':
            printf("SYNOPSIS\n   Encrypts data using SS encryption.\n   Encrypted "
                   "data is decrypted "
                   "by the decrypt program.\n\nUSAGE\n   ./encrypt "
                   "[OPTIONS]\n\nOPTIONS\n   -h        "
                   "      Display program help and usage.\n   -v              "
                   "Display verbose program "
                   "output.\n   -i infile       Input file of data to encrypt "
                   "(default: stdin).\n   "
                   "-o outfile      Output file for encrypted data (default: "
                   "stdout).\n   -n pbfile   "
                   "    Public key file (default: ss.pub).");
            return 0;
        } // switch statement of all the command line options
    }

    if (in_check) {
        infile = fopen(input, "r");
        if (!infile) {
            printf("Error opening the file %s or it is not present in the directory\n", input);
            fclose(infile);
            return EXIT_FAILURE;
        }
    }

    if (out_check) {
        outfile = fopen(output, "w");
        if (!outfile) {
            printf("Error opening the file %s or it is not present in the directory\n", output);
            fclose(outfile);
            return EXIT_FAILURE;
        }
    }
    pbfile = fopen(pub_file, "r");
    if (!pbfile) {
        printf("Error opening the file %s or it is not present in the directory\n", pub_file);
        fclose(pbfile);
        mpz_clear(n);
        return EXIT_FAILURE;
    }

    else if (pbfile) {
        ss_read_pub(n, username, pbfile);
    }

    if (verbose == 1) { // checks if verboseose was turned on
        printf("user =%s \n", username);
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
    }

    ss_encrypt_file(infile, outfile, n);
    fclose(pbfile);
    fclose(infile);
    fclose(outfile);
    mpz_clear(n);
    // closes and frees all files and mpz_ts
}
