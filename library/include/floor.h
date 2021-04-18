#ifndef FLOOR_H
#define FLOOR_H

#include <stdint.h>

#include "elevator_system_common.h"

typedef struct {
    int16_t no;
    direction available_directions;
    direction requested_directions;
} floor;

int floor_cmp(const void *a, const void *b); //done

#endif //FLOOR_H
