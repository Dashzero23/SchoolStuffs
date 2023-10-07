#include <stdio.h>
#define EPSILON 1e-14
#include "mathlib.h"
int loop = 0;
double e(void) {
    double base = 1.0;
    double e = 0.0;
    double term = 1.0;
    for (int i = 1; term > EPSILON; i++) {
        term = 1 / base;
        base *= (i + 1);
        e += term;
        loop += 1;
    }
    e += 1;
    return e;
}
int e_terms(void) {
    return loop;
}
