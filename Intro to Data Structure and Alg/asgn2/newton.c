#include <stdio.h>
#define EPSILON 1e-14
#include "mathlib.h"
int loopnewton = 0;
double sqrt_newton(double val) {
    double x = 1.0;
    double tempX = x;
    double nextX = 0.0;
    loopnewton = 0;
    for (int i = 0; absolute(tempX - nextX) > EPSILON; i++) {
        double fx = x * x - val;
        double derivfx = 2 * x;
        nextX = x - fx / derivfx;
        tempX = x;
        x = nextX;
        loopnewton += 1;
    }
    return x;
}
int sqrt_newton_iters(void) {
    return loopnewton;
}
