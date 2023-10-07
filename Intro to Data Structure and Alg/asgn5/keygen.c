#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

#include <gmp.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define OPTIONS "b:i:n:d:s:vh"

// implementation of lines 78 to 79 derived from Simon Kwong(Student 13s Winter
// Quarter)

int main(int argc, char **argv) {
    int opt = 0;
    uint64_t nbits = 256;
    uint64_t iters = 50;
    bool check_seed = false;
    int seed = 0;
    int verbose = 0;
    char *pub = "ss.pub";
    char *priv = "ss.priv";
    FILE *pbfile = NULL;
    FILE *pvfile = NULL;
    // declaration of variables that are going to be used based on the input in
    // the command line from the user
    mpz_t p, q, n, d, pq;
    mpz_inits(p, q, n, d, pq, NULL);
    // initialization of all mpz_ts

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b': nbits = (uint64_t) atoi(optarg); break;
        case 'i': iters = (uint64_t) atoi(optarg); break;
        case 'n': pub = optarg; break;
        case 'd': priv = optarg; break;
        case 's':
            seed = (uint64_t) atoi(optarg);
            check_seed = true;
            break;
        case 'v': verbose = 1; break;
        case 'h':
            printf("SYNOPSIS\n   Generates an SS public/private key pair.\n\nUSAGE\n "
                   "  ./keygen "
                   "[OPTIONS]\n\nOPTIONS\n   -h              Display program help "
                   "and usage.\n   "
                   "-v              Display verbose program output.\n   -b bits      "
                   "   Minimum "
                   "bits needed for public key n (default: 256).\n   -i iterations   "
                   "Miller-Rabin "
                   "iterations for testing primes (default: 50).\n   -n pbfile       "
                   "Public key "
                   "file (default: ss.pub).\n   -d pvfile       Private key file "
                   "(default: "
                   "ss.priv).\n   -s seed         Random seed for testing.");
        }
    }

    pbfile = fopen(pub, "w");
    pvfile = fopen(priv,
        "w"); // opens the private and public key files for writing
    // based on what the user inputs

    uint64_t fd = fileno(pvfile);
    fchmod(fd,
        S_IRUSR | S_IWUSR); // gets the username so as to verify who's using the keys

    if (check_seed == false) {
        randstate_init(time(NULL)); // initialization of seed
    } else {
        randstate_init(seed);
    }
    if (nbits <= 0) {
        return 0;
    }
    ss_make_pub(p, q, n, nbits, iters);
    ss_make_priv(d, pq, p, q);
    char *user = getenv("USER");
    ss_write_pub(n, user, pbfile);
    ss_write_priv(pq, d, pvfile);

    if (verbose == 1) { // prints the variables and their bit sizes if verbose is turned on
        printf("user = %s \n", user);
        gmp_printf("p (%d bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q (%d bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("pq (%d bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(d);
    mpz_clear(pq);
    fclose(pvfile);
    fclose(pbfile);
    randstate_clear();
    // frees all mpz_ts and closes all files
}
