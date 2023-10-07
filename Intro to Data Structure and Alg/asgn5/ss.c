#include "ss.h"

#include "numtheory.h"

#include <gmp.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 256

void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    uint64_t pbits = nbits / 5 + (random() % (nbits / 5));
    uint64_t qbits = nbits - (2 * pbits);

    // Generate primes p and q
    make_prime(p, pbits, iters);
    make_prime(q, qbits, iters);
    while (mpz_divisible_p(p, q) || mpz_divisible_p(q, p)) {
        make_prime(q, qbits, iters);
    }

    // Compute n
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);
}

void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    mpz_t lambda, GrCmDv, q_minus1, p_minus1, n;
    // generates the private key by getting the lcm(p-1, q-1) and finding the
    // modular inverse of e, the public key
    mpz_init(lambda);
    mpz_init(n);
    mpz_init(p_minus1);
    mpz_init(q_minus1);
    mpz_init(GrCmDv);

    mpz_mul(pq, p, q);

    mpz_mul(n, p, p);
    mpz_mul(n, n, q);

    mpz_sub_ui(p_minus1, p, 1);
    mpz_sub_ui(q_minus1, q, 1);
    // get lcm by doing (ab)/(gcd(ab))
    mpz_mul(lambda, q_minus1, p_minus1);
    gcd(GrCmDv, q_minus1, p_minus1);
    mpz_fdiv_q(lambda, lambda, GrCmDv);
    mod_inverse(d, n, lambda);

    mpz_clear(lambda);
    mpz_clear(p_minus1);
    mpz_clear(q_minus1);
    mpz_clear(GrCmDv);
}

void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n%s\n", n, username);
    // gmp_fprintf(pbfile, "%Zx\n", n);
    // gmp_fprintf(pbfile, "%s\n", username);
}

void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", pq);
    gmp_fprintf(pvfile, "%Zx\n", d);
    // writes n and d to the private key file using gmp_fprintf
}

void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n%s\n", n, username);
    // gmp_printf("key = %Zd\n", n);
    // gmp_printf("name = %s\n", username);
    // gmp_fscanf(pbfile, "%Zx\n", n);
    // gmp_fscanf(pbfile, "%s\n", username);
}

void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n%Zx\n", pq, d);
}

void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    pow_mod(c, m, n, n);
    // mpz_powm(c, m, n, n);
    // gmp_printf("Encrypted data = %Zd\n", c);
}

void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    mpz_t m, out;
    mpz_inits(m, out, NULL);

    size_t k = ((0.5 * mpz_sizeinbase(n, 2) - 1) / 8);
    // printf("k = %zu\n", k);
    uint8_t *block = (uint8_t *) (calloc(k, sizeof(uint8_t)));
    block[0] = 0XFF;
    size_t j;
    while ((j = fread(&block[1], 1, k - 1, infile)) > 0) {
        // printf("j = %zu\n", j);
        // j = fread(&block[1], 1, k - 1, infile);
        // printf("Read block\n");
        mpz_import(m, j + 1, 1, 1, 1, 0, block);
        // gmp_printf("m = %Zd\n", m);
        ss_encrypt(out, m, n);
        // printf("Encrypted block\n");
        // gmp_printf("out = %Zd\n", out);
        gmp_fprintf(outfile, "%Zx\n", out);
    }
    free(block);

    mpz_clears(m, out, NULL);
}

void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    pow_mod(m, c, d, pq);
}
void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    size_t k = ((0.5 * mpz_sizeinbase(pq, 2) - 1) / 8);
    // printf("k = %zu\n", k);
    uint8_t *block = (uint8_t *) (calloc(k, sizeof(uint8_t)));
    // char hexstring[k * 2 + 2]; // +2 for the newline and null terminator
    size_t j;
    mpz_t c;
    mpz_t m;
    mpz_init(c);
    mpz_init(m);
    while (gmp_fscanf(infile, "%Zx\n", c) != EOF) {
        // gmp_fscanf(infile, "%Zd\n", c);
        // printf("hex = %s", hexstring);
        // mpz_set_str(c, hexstring, 16);
        // gmp_printf("c = %Zd\n", c);
        ss_decrypt(m, c, d, pq);
        // gmp_printf("m = %Zd\n", m);
        mpz_export(block, &j, 1, sizeof(uint8_t), 1, 0, m);
        fwrite(&block[1], sizeof(uint8_t), j - 1, outfile);
    }

    free(block);
    mpz_clear(c);
    mpz_clear(m);
}
