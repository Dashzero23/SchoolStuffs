#include "randstate.h"

#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    mpz_t r, temp_a, temp_b;
    mpz_init(r);
    mpz_init_set(temp_a, a);
    mpz_init_set(temp_b, b);
    while (mpz_cmp_ui(temp_b, 0) > 0) {
        mpz_mod(r, temp_a, temp_b);
        mpz_set(temp_a, temp_b);
        mpz_set(temp_b, r);
    }

    mpz_set(g, temp_a);
    mpz_clear(r);
    mpz_clear(temp_a);
    mpz_clear(temp_b);
}

void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    mpz_t q, r, x1, x2, y1, y2, temp;
    mpz_inits(q, r, x1, x2, y1, y2, temp, NULL);

    // Set initial values
    mpz_set_ui(x1, 1);
    mpz_set_ui(x2, 0);
    mpz_set(y1, a);
    mpz_set(y2, n);

    while (mpz_cmp_ui(y2, 0) != 0) {
        // Divide y1 by y2
        mpz_tdiv_qr(q, r, y1, y2);

        // Compute x1 = x1 - q * x2
        mpz_mul(temp, q, x2);
        mpz_sub(temp, x1, temp);
        mpz_set(x1, x2);
        mpz_set(x2, temp);

        // Update y1 and y2
        mpz_set(y1, y2);
        mpz_set(y2, r);
    }

    if (mpz_cmp_ui(y1, 1) == 0) {
        // Compute inverse = x1 mod mod
        mpz_mod(o, x1, n);
    } else {
        // Modular inverse does not exist
        mpz_set_ui(o, 0);
    }

    mpz_clears(q, r, x1, x2, y1, y2, temp, NULL);
}

void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    // Initialize variables for the computation
    mpz_t b, e, m, r, t;
    mpz_init_set(b, a); // b = base
    mpz_init_set(e, d); // e = exponent
    mpz_init_set(m, n); // m = modulus
    mpz_init_set_ui(r, 1); // r = 1
    mpz_inits(t, NULL);

    // Handle special cases
    /* if (mpz_sgn(e) < 0 || mpz_sgn(m) == 0 || (mpz_sgn(b) == 0 && mpz_sgn(e) >
  0)) { mpz_set_ui(o, 0); mpz_clears(b, e, m, r, t, NULL);
  } */

    // Compute the result using repeated squaring
    while (mpz_cmp_ui(e, 0) > 0) {
        if (mpz_odd_p(e) != 0) {
            mpz_mul(t, r, b); // t = r * b
            mpz_mod(r, t, m); // r = t % m
        }
        mpz_mul(t, b, b); // t = b * b
        mpz_mod(b, t, m); // b = t % m
        mpz_div_2exp(e, e, 1); // e = e / 2
    }
    mpz_set(o, r); // result = r

    // Clear variables used in the computation
    mpz_clears(b, e, m, r, t, NULL);
}

bool is_prime(mpz_t n, uint64_t iters) {
    // special cases
    if (mpz_cmp_ui(n, 2) < 0) {
        return false;
    }
    if (mpz_cmp_ui(n, 2) == 0) {
        return true;
    }
    if (mpz_cmp_ui(n, 3) == 0) {
        return true;
    }
    mpz_t s, r, d, a, j, y, two, bound;
    mpz_inits(s, r, d, a, j, y, two, bound, NULL);
    mpz_sub_ui(bound, n, 2);
    mpz_set_ui(two, 2);
    // make d = 2^s * r
    mpz_sub_ui(d, n, 1);
    mpz_set_ui(s, 0);
    mpz_set(r, d);

    while (mpz_even_p(r)) {
        mpz_tdiv_q_2exp(r, r, 1);
        mpz_add_ui(s, s, 1);
    }
    mpz_sub_ui(s, s, 1);
    // Implement Miller Rabin method
    for (uint64_t i = 0; i < iters; i++) {
        mpz_urandomm(a, state, bound);

        if (mpz_cmp_ui(a, 2) < 0) {
            mpz_add_ui(a, a, 2);
        }

        pow_mod(y, a, r, n);

        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, d) != 0) {

            mpz_set_ui(j, 1);
            while (mpz_cmp(j, s) <= 0 && mpz_cmp(y, d) != 0) {

                pow_mod(y, y, two, n);
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(s, r, d, a, j, y, two, bound, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }

            if (mpz_cmp(y, d) != 0) { // if y!= n−1
                mpz_clears(s, r, d, a, j, y, two, bound, NULL);
                return false;
            }
        }
    }

    mpz_clears(s, r, d, a, j, y, two, bound, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t min;
    mpz_init(min);

    mpz_ui_pow_ui(min, 2, bits);
    mpz_urandomb(p, state, bits);
    mpz_add(p, p, min);

    while (is_prime(p, iters) == false) {
        mpz_urandomb(p, state, bits);
        mpz_add(p, p, min);
    }

    mpz_clear(min);
}
