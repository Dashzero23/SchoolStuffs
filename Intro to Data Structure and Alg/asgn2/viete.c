#include <stdio.h>
#define EPSILON 1e-14
#include "mathlib.h"
int loopviete = 1;
double pi_viete(void) {
    double pi = 2;
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
    pi = (f * y) / 2; // square root code with some personal changes from the given portable.py file
    double temp = pi;
    double term = 0.0;
    double tempPi = 0.0;
    for (int i = 1; absolute(pi - tempPi) > EPSILON; i++) {
        term = 2.0 + 2 * (temp);
        double f = 1.0;
        double y = 1.0;
        while (term > 4) {
            term /= 4;
            f *= 2;
        }
        double guess = 0.0;
        while (absolute(y - guess) > EPSILON) {
            guess = y;
            y = (y + term / y) / 2;
        }
        term = (f * y)
               / 2; // square root code with some personal changes from the given portable.py file
        temp = term;
        tempPi = pi;
        pi *= term;
        loopviete += 1;
    }
    pi = 2 / pi;
    return pi;
}
int pi_viete_factors(void) {
    return loopviete;
}
