#include <stdio.h>
#define EPSILON 1e-14
int loopbbp = 0;
double pi_bbp(void) {
    double pi = 0.0;
    double base = 1.0;
    double term = 1.0;
    for (int i = 0; term > EPSILON; i++) {
        term = (1.0 / base)
               * ((4.0 / ((8.0 * i) + 1.0)) - (2.0 / ((8.0 * i) + 4.0)) - (1.0 / ((8.0 * i) + 5.0))
                   - (1.0 / ((8.0 * i) + 6.0)));
        pi += term;
        base *= 16.0;
        loopbbp += 1;
    }
    return pi;
}
int pi_bbp_terms(void) {
    return loopbbp;
}
