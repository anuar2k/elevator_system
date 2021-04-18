#include <elevator.h>

void handle_stationary(elevator* elevator);
void handle_between_floors(elevator* elevator);
void handle_passing_by(elevator* elevator);

void elevator_step(elevator *elevator) {
    if (elevator_state_finished(elevator)) {
        switch (elevator->state) {
            case EL_STATIONARY:         handle_stationary(elevator); break;
            case EL_BETWEEN_FLOORS:     handle_between_floors(elevator); break;
            case EL_PASSING_BY:         handle_passing_by(elevator); break;
            case elevator_state_length: assert(false); break;
        }
    }

    elevator->state_progress++;
}

void handle_stationary(elevator *elevator) {
    size_t queued_no = AVS_VECTOR_SIZE(elevator->queued_floors);

    if (queued_no == 0) {
        elevator_switch_state(elevator, EL_STATIONARY);
    }
    else {
        floor_request *head = AVS_VECTOR_FRONT(elevator->queued_floors);

        if (head->floor_no == elevator->last_floor) {
            while (head && head->floor_no == elevator->last_floor) {
                AVS_VECTOR_REMOVE_AT(&elevator->queued_floors, 0);
                head = AVS_VECTOR_FRONT(elevator->queued_floors);
            }
            elevator_switch_state(elevator, EL_STATIONARY);
        }
        else {
            elevator->direction = head->floor_no > elevator->last_floor ? DIR_UP : DIR_DOWN;
            elevator_switch_state(elevator, EL_BETWEEN_FLOORS);
        }
    }
}

void handle_between_floors(elevator *elevator) {
    size_t queued_no = AVS_VECTOR_SIZE(elevator->queued_floors);
    assert(queued_no > 0);
    assert(!(elevator->direction == DIR_DOWN && elevator->last_floor == 0));
    assert(!(elevator->direction == DIR_UP   && elevator->last_floor == AVS_VECTOR_SIZE(elevator->elevator_system->floors)) - 1);

    size_t next_floor = elevator->last_floor + (elevator->direction == DIR_UP ? 1 : -1);
    floor_request *head = AVS_VECTOR_FRONT(elevator->queued_floors);

    if (head->floor_no == next_floor) {
        elevator_switch_state(elevator, EL_STATIONARY);
    }
    else {
        elevator_switch_state(elevator, EL_PASSING_BY);
    }

    elevator->last_floor = next_floor;
}

void handle_passing_by(elevator *elevator) {
    elevator_switch_state(elevator, EL_BETWEEN_FLOORS);
}

bool elevator_enqueue_floor(elevator *elevator, const floor_request *floor_request) {
    return false;
}

int elevator_search_by_no_cmp(const void *key, const void *el) {
    const size_t *key_no = key;
    const elevator *elevator = el;

    return *key_no - elevator->no;
}

bool elevator_state_finished(elevator *elevator) {
    return elevator->state_progress == state_length[elevator->state];
}

void elevator_switch_state(elevator *elevator, elevator_state target_state) {
    elevator->state_progress = 0;
    elevator->state = target_state;
}
