#include "set.h"
#define SET_MAX 32
//helped by multiple TAs/tutors
Set set_empty(void) {
    return 0x00000000; // Empty set is 0s for each bit.
}

bool set_member(Set s, uint8_t x) {
    return s & (1 << (x % SET_MAX));
}

Set set_insert(Set s, uint8_t x) {
    return s | (1 << (x % SET_MAX));
}

Set set_remove(Set s, uint8_t x) {
    return s & ~(1 << (x % SET_MAX));
}

Set set_union(Set s, Set t) {
    return s | t;
}

Set set_intersect(Set s, Set t) {
    return s & t;
}

Set set_difference(Set s, Set t) {
    return s & ~t;
}

Set set_universal(void) {
    return 0;
}
