#include "gaps.h"
#include "stats.h"

#include <inttypes.h>
#include <stdio.h>

void shell_sort(Stats *stats, uint32_t *arr, uint32_t len) {

    for (uint32_t x = 0; x < GAPS; x++) {

        uint32_t val = gaps[x];

        for (uint32_t i = val; i < len; i++) {

            uint32_t j = i;

            uint32_t temp = arr[i];

            while (j >= val && cmp(stats, temp, arr[j - val]) == -1) {

                arr[j] = move(stats, arr[j - val]);

                j -= val;
            }

            arr[j] = move(stats, temp);
        }
    }
}
