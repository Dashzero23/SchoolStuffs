#include <stdio.h>
#define EPSILON 1e-14
#include "mathlib.h"
int loopeuler = 0;
double pi_euler(void) {
    double base = 1.0;
    double pi = 0.0;
    double term = 1.0;
    for (int i = 0; term > EPSILON; i++) {
        term = 1 / (base * base);
        base += 1;
        pi += term;
        loopeuler += 1;
    }
    pi *= 6;
    double f = 1.0;
    double y = 1.0;
    while (pi > 4) {
        pi /= 4;
        f *= 2;
    }
    double guess = 0.0;
    while (absolute(y - guess) > EPSILON) {
        guess = y;
        y = (y + pi / y) / 2;
    }
    pi = f * y;
    // square root code with some personal changes from the given portable.py file
    return pi;
}
int pi_euler_terms(void) {
    return loopeuler;
}
