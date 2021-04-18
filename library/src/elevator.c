#include <elevator.h>

void elevator_step(elevator *elevator) {
    switch (elevator->state) {
        case EL_STATIONARY: {

        } break;
        case EL_BETWEEN_FLOORS: {

        } break;
        case EL_PASSING_BY: {

        } break;
    }
}

bool elevator_enqueue_floor(elevator *elevator, const floor_request *floor_request) {
    return false;
}

int elevator_search_by_no_cmp(const void *key, const void *el) {
    const uint16_t *key_no = key;
    const elevator *elevator = el;

    return *key_no - elevator->no;
}
