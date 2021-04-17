#include <elevator_system/elevator_system.h>

void init_elevator_system(elevator_system *system, uint16_t elevator_count, uint16_t floor_count, floor *floors) {
    system->elevators = AVS_VECTOR_NEW(elevator);
    system->floors = AVS_VECTOR_NEW(floor);

    for (uint16_t i = 0; i < elevator_count; i++) {
        elevator to_insert = {
            .no = i,
            .direction = DIR_NONE,
            .elevator_state = EL_STAT_CLOSED,
            .queued_floors = AVS_VECTOR_NEW(floor_request)
        };

        AVS_VECTOR_PUSH(&system->elevators, &to_insert);
    }

    for (uint16_t i = 0; i < floor_count; i++) {
        floors[i].requested_directions = DIR_NONE;

        AVS_VECTOR_PUSH(&system->floors, &floors[i]);
    }
}

void free_elevator_system(elevator_system *system) {
    elevator *elevator_ptr;
    AVS_VECTOR_CLEAR(&system->elevators, elevator_ptr) {
        AVS_VECTOR_DELETE(&elevator_ptr->queued_floors);
    }

    AVS_VECTOR_DELETE(&system->elevators);
    AVS_VECTOR_DELETE(&system->floors);
}

