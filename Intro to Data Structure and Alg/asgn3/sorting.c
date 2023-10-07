#include "batcher.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "Hasbhqr:n:p:"

Stats stats = { 0, 0 };
//a lot of syntax errors were fixed with the help of multiple TAs/tutors
void help(void) {
    printf("Select at least one sort to perform.\nSYNOPSIS\n   A collection of comparison-based "
           "sorting algorithms.\n\nUSAGE\n   ./sorting_amd64 [-Hasbhqn:p:r:] [-n length] [-p "
           "elements] [-r seed]\n\nOPTIONS\n   -H              Display program help and usage.\n   "
           "-a              Enable all sorts.\n   -s              Enable Shell Sort.\n   -b        "
           "      Enable Batcher Sort.\n   -h              Enable Heap Sort.\n   -q              "
           "Enable Quick Sort.\n   -n length       Specify number of array elements (default: "
           "100).\n   -p elements     Specify number of elements to print (default: 100).\n   -r "
           "seed         Specify random seed (default: 13371453).\n");
    return;
}
void make_arr(uint32_t *arr, int seed, uint32_t length) {
    srandom(seed); // choose the seed
    for (uint32_t i = 0; i < length; i++) {
        arr[i] = (uint32_t) (random() & (0x3FFFFFFF));
    }
    return;
}
int main(int argc, char **argv) {
    int seed = 13371453;

    int opt = 0;

    uint32_t size = 100;

    uint32_t element = 100;

    enum sort { shell, batcher, heap, quick };

    void (*func_ptr[4])(Stats * stats, uint32_t * arr, uint32_t n)
        = { shell_sort, batcher_sort, heap_sort, quick_sort };
    char *sort_name[] = { "Shell Sort", "Batcher Sort", "Heap Sort", "Quick Sort" };
    Set set = set_empty();

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'H': help(); return 0;
        case 'a':
            set = set_insert(set, shell);
            set = set_insert(set, batcher);
            set = set_insert(set, heap);
            set = set_insert(set, quick);
            break;
        case 's': set = set_insert(set, shell); break;
        case 'b': set = set_insert(set, batcher); break;
        case 'h': set = set_insert(set, heap); break;
        case 'q': set = set_insert(set, quick); break;
        case 'n': size = atoi(optarg); break;
        case 'p': element = atoi(optarg); break;
        case 'r': seed = atoi(optarg); break;
        default: help(); return 0;
        }
    }
    if (set == 0) {
        help();
        return 0;
    }

    uint32_t *ran_arr = (uint32_t *) malloc(size * sizeof(uint32_t));

    if (ran_arr == NULL) {
        printf("Failed to allocate memory for array\n");
        return 0;
    }
    if (element > size) {
        element = size;
    }

    for (uint32_t i = 0; i < 4; i++) {

        if (set_member(set, i)) {
            reset(&stats);
            make_arr(ran_arr, seed, size);
            func_ptr[i](&stats, ran_arr, size);
            printf("%s ", sort_name[i]);
            printf("%d elements, %lu moves, %lu compares\n", size, stats.moves, stats.compares);
            for (uint32_t i = 0; i < element; i++) {
                // Print new line if i%5 so we get columns of 5
                if (i % 5 == 0 && i != 0) {
                    printf("\n");
                }
                printf("%13" PRIu32, ran_arr[i]);

                if (i == element - 1) {
                    printf("\n");
                }
            }
        }
    }
    free(ran_arr);
    return 0;
}
