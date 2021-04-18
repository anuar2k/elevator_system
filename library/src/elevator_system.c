#include <elevator_system.h>

#include <stdlib.h>
#include <stdbool.h>
#include <avsystem/commons/avs_vector.h>

void elevator_system_init(elevator_system *system, uint16_t elevator_count, uint16_t floor_count, const floor *floors) {
    system->elevators = AVS_VECTOR_NEW(elevator);
    system->floors = AVS_VECTOR_NEW(floor);
    system->rr_next_elevator = 0;

    for (uint16_t i = 0; i < floor_count; i++) {
        floor to_insert = floors[i];
        to_insert.requested_directions = DIR_NONE;

        AVS_VECTOR_PUSH(&system->floors, &to_insert);
    }

    AVS_VECTOR_SORT(&system->floors, floor_cmp);

    for (uint16_t i = 0; i < elevator_count; i++) {
        elevator to_insert = {
            .no = i,
            .last_floor = (*system->floors)[0].no,
            .direction = DIR_NONE,
            .state = EL_STATIONARY,
            .queued_floors = AVS_VECTOR_NEW(floor_request)
        };

        AVS_VECTOR_PUSH(&system->elevators, &to_insert);
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

bool elevator_system_request_pickup(elevator_system *system, int16_t floor_no, direction direction) {
    floor_request floor_request = {
        .req_type = direction == DIR_UP ? REQ_PICK_UP : REQ_PICK_DOWN,
        .floor_no = floor_no
    };

    elevator *elevator = bsearch(
        &system->rr_next_elevator,
        *system->elevators,
        AVS_VECTOR_SIZE(system->elevators),
        sizeof(*system->elevators),
        elevator_search_by_no_cmp
    );

    if (++system->rr_next_elevator == AVS_VECTOR_SIZE(system->elevators)) {
        system->rr_next_elevator = 0;
    }

    return elevator_enqueue_floor(elevator, &floor_request);
}

bool elevator_system_request_dropoff(elevator_system *system, uint16_t elevator_no, int16_t floor_no) {
    floor_request floor_request = {
        .req_type = REQ_DROPOFF,
        .floor_no = floor_no
    };

    return elevator_enqueue_floor(&(*system->elevators)[elevator_no], &floor_request);
}
