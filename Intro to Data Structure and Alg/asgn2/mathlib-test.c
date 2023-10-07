#include "mathlib.h"

#include <math.h>
#include <stdio.h>
#include <unistd.h>
#define OPTIONS "aebmrvnsh:"

// void usage(char *exec) {
//    fprintf(stderr,
//        "SYNOPSIS\n","   A test harness for the small numerical library.\n","\n""USAGE\n","   %s [-aebmrvnsh]\n","\n""OPTIONS\n","  -a   Runs all tests.\n","  -e   Runs e test.\n""  -b   Runs BBP pi test.\n","  -m   Runs Madhava pi test.", "  -r   Runs Euler pi test.", "  -v   Runs Viete pi test.","  -n   Runs Newton square root tests.", "  -s   Print verbose statistics.","  -h   Display program synopsis and usage.", exec);
//}

int main(int argc, char **argv) {

    int opt = 0;
    int term = 0;
    int e_case = 0;
    int b_case = 0;
    int m_case = 0;
    int r_case = 0;
    int v_case = 0;
    int n_case = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
            term = 0;
            e_case = 0;
            b_case = 0;
            m_case = 0;
            r_case = 0;
            v_case = 0;
            n_case = 0;
        case 'a':
            e_case = 1;
            b_case = 1;
            m_case = 1;
            r_case = 1;
            v_case = 1;
            n_case = 1;
            break;
        case 'e': e_case = 1; break;
        case 'b': b_case = 1; break;
        case 'm': m_case = 1; break;
        case 'r': r_case = 1; break;
        case 'v': v_case = 1; break;
        case 'n': n_case = 1; break;
        case 's': term = 1; break;
        case 'h':
            printf("SYNOPSIS\n");
            printf("\nUSAGE\n");
            printf(" ./mathlib-test [-aebmrvnsh]\n");
            printf("\nOPTIONS\n");
            printf("  -a   Runs all tests.\n");
            printf("  -e   Runs e test.\n");
            printf("  -b   Runs BBP pi test.\n");
            printf("  -v   Runs Viete pi test.\n");
            printf("  -n   Runs Newton square root tests.\n");
            printf("  -s   Print verbose statistics.\n");
            printf("  -h   Display program synopsis and usage.\n");
            break;
        default:
            printf("SYNOPSIS\n");
            printf("\nUSAGE\n");
            printf(" ./mathlib-test [-aebmrvnsh]\n");
            printf("\nOPTIONS\n");
            printf("  -a   Runs all tests.\n");
            printf("  -e   Runs e test.\n");
            printf("  -b   Runs BBP pi test.\n");
            printf("  -v   Runs Viete pi test.\n");
            printf("  -n   Runs Newton square root tests.\n");
            printf("  -s   Print verbose statistics.\n");
            printf("  -h   Display program synopsis and usage.\n");
            break;
        }
    }
    if (e_case == 1) {
        printf("e() = %10.15f, M_E = %10.15f, diff =  %10.15f\n", e(), M_E, absolute(M_E - e()));
        if (term == 1) {
            printf("e() terms = %d\n", e_terms());
        }
    }
    if (b_case == 1) {
        printf("pi_bbp() = %10.15f", pi_bbp());
        printf(", M_PI = %10.15f", M_PI);
        printf(", diff =  %10.15f", absolute(M_PI - pi_bbp()));
        printf("\n");
        if (term == 1) {
            printf("pi_bbp() terms = %d\n", pi_bbp_terms());
        }
    }
    if (m_case == 1) {
        printf("pi_madhava() = %10.15f", pi_madhava());
        printf(", M_PI = %10.15f", M_PI);
        printf(", diff =  %10.15f", absolute(M_PI - pi_madhava()));
        printf("\n");
        if (term == 1) {
            printf("pi_madhava() terms = %d\n", pi_madhava_terms());
        }
    }
    if (r_case == 1) {
        printf("pi_euler() = %10.15f", pi_euler());
        printf(", M_PI = %10.15f", M_PI);
        printf(", diff =  %10.15f", absolute(M_PI - pi_euler()));
        printf("\n");
        if (term == 1) {
            printf("pi_euler() terms = %d\n", pi_euler_terms());
        }
    }
    if (v_case == 1) {
        printf("pi_viete() = %10.15f", pi_viete());
        printf(", M_PI = %10.15f", M_PI);
        printf(", diff =  %10.15f", absolute(M_PI - pi_viete()));
        printf("\n");
        if (term == 1) {
            printf("pi_viete() factors = %d\n", pi_viete_factors());
        }
    }
    if (n_case == 1) {
        for (double i = 0; i < 101; i++) {
            double val = i / 10;
            printf("sqrt_newton(%.6f) = %10.15f", val, sqrt_newton(val));
            printf(" sqrt(%.6f) = %10.15f", val, sqrt(val));
            printf(" diff = %10.15f", absolute(sqrt_newton(val) - sqrt(val)));
            printf("\n");
            if (term == 1) {
                printf("sqrt_newton(%.1f) iters = %d\n", i / 10, sqrt_newton_iters());
            }
        }
    }
    return 0;
}
