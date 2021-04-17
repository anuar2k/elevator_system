#include <elevator_system.h>

void elevator_system_init(elevator_system *system, uint16_t elevator_count, uint16_t floor_count, floor *floors) {
    system->elevators = AVS_VECTOR_NEW(elevator);
    system->floors = AVS_VECTOR_NEW(floor);

    for (uint16_t i = 0; i < elevator_count; i++) {
        elevator to_insert = {
            .no = i,
            .last_floor = floors[0].no,
            .direction = DIR_NONE,
            .state = EL_STAT_CLOSED,
            .queued_floors = AVS_VECTOR_NEW(floor_request)
        };

        AVS_VECTOR_PUSH(&system->elevators, &to_insert);
    }

    for (uint16_t i = 0; i < floor_count; i++) {
        floors[i].requested_directions = DIR_NONE;

        AVS_VECTOR_PUSH(&system->floors, &floors[i]);
    }
}

void elevator_system_free(elevator_system *system) {
    elevator *elevator_ptr;
    AVS_VECTOR_CLEAR(&system->elevators, elevator_ptr) {
        AVS_VECTOR_DELETE(&elevator_ptr->queued_floors);
    }

    AVS_VECTOR_DELETE(&system->elevators);
    AVS_VECTOR_DELETE(&system->floors);
}

void elevator_system_step(elevator_system *system) {
    for (uint16_t i = 0; i < AVS_VECTOR_SIZE(system->elevators); i++) {
        elevator_step(&(*system->elevators)[i]);
    }
}

void elevator_step(elevator *elevator) {
    switch (elevator->state) {
        case EL_STAT_CLOSED:
        break;
        case EL_STAT_OPENING:
        break;
        case EL_STAT_OPENED:
        break;
        case EL_EN_ROUTE:
        break;
        case EL_ARRIVING:
        break;
    }
}