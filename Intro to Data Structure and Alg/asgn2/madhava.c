#include <stdio.h>
#define EPSILON 1e-14
#include "mathlib.h"
int loopmhv = 0;
double pi_madhava(void) {
    double pi = 0.0;
    double base = 1.0;
    double term = 1.0;
    for (int k = 0; absolute(term) > EPSILON; k++) {
        term = (base / (2 * k + 1));
        pi += term;
        base /= -3;
        loopmhv += 1;
    }
    double x = 12.0;
    double f = 1.0;
    double y = 1.0;
    while (x > 4) {
        x /= 4;
        f *= 2;
    }
    double guess = 0.0;
    while (absolute(y - guess) > EPSILON) {
        guess = y;
        y = (y + x / y) / 2;
    }
    pi = pi
         * (f * y); // square root code with some personal changes from the given portable.py file
    return pi;
}
int pi_madhava_terms(void) {
    return loopmhv;
}
