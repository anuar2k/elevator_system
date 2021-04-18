#include <floor.h>

int floor_cmp(const void *a, const void *b) {
    const floor *floor_a = a;
    const floor *floor_b = b;

    return floor_a->no - floor_b->no;
}
