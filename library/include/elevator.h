#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <stdint.h>
#include <avsystem/commons/avs_vector.h>

#include "elevator_system_common.h"
#include "elevator.h"
#include "floor.h"

typedef enum {
    EL_STATIONARY,
    EL_BETWEEN_FLOORS,
    EL_PASSING_BY
} elevator_state;

typedef struct {
    uint16_t no;
    int16_t last_floor;
    direction direction;
    elevator_state state;
    AVS_VECTOR(floor_request) queued_floors;
} elevator;

void elevator_step(elevator *elevator);
bool elevator_enqueue_floor(elevator *elevator, const floor_request *floor_request);
int elevator_search_by_no_cmp(const void *a, const void *b); //done

#endif //ELEVATOR_H
