#include <elevator.h>
#include <string.h>

void handle_stationary(elevator *elevator);
void handle_between_floors(elevator *elevator);
void handle_passing_by(elevator *elevator);
size_t request_weight(elevator *elevator, const floor_request *req);
direction destination_direction(elevator *elevator, const floor_request *req);

void elevator_step(elevator *elevator) {
    if (elevator_state_finished(elevator)) {
        switch (elevator->state) {
            case EL_STATIONARY:         handle_stationary(elevator);     break;
            case EL_BETWEEN_FLOORS:     handle_between_floors(elevator); break;
            case EL_PASSING_BY:         handle_passing_by(elevator);     break;
            case elevator_state_length: assert(false);                   break;
        }
    }

    elevator->state_progress++;
}

void handle_stationary(elevator *elevator) {
    const size_t queued_no = AVS_VECTOR_SIZE(elevator->queued_floors);

    if (queued_no == 0) {
        elevator_switch_state(elevator, EL_STATIONARY);
        elevator->direction = DIR_NONE;
        return;
    }

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

void handle_between_floors(elevator *elevator) {
    const size_t queued_no = AVS_VECTOR_SIZE(elevator->queued_floors);
    assert(queued_no > 0);
    assert(!(elevator->direction == DIR_DOWN && elevator->last_floor == 0));
    assert(!(elevator->direction == DIR_UP   && elevator->last_floor == AVS_VECTOR_SIZE(elevator->elevator_system->floors) - 1));

    const size_t next_floor = elevator->last_floor + (elevator->direction == DIR_UP ? 1 : -1);
    const floor_request *head = AVS_VECTOR_FRONT(elevator->queued_floors);

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
    const size_t queued_no = AVS_VECTOR_SIZE(elevator->queued_floors);
    const size_t incoming_weight = request_weight(elevator, floor_request);

    size_t last_weight;
    size_t i = 0;

    while (i < queued_no && incoming_weight > (last_weight = request_weight(elevator, &(*elevator->queued_floors)[i]))) {
        i++;
    }

    //duplicated request
    if (i < queued_no && incoming_weight == last_weight) {
        return false;
    }

    //we have to insert the request -> increase vector size, move elements after insertion point
    AVS_VECTOR_PUSH(&elevator->queued_floors, floor_request);
    
    memmove(
        &(*elevator->queued_floors)[i + 1], 
        &(*elevator->queued_floors)[i], 
        (queued_no - i) * sizeof(**elevator->queued_floors)
    );

    (*elevator->queued_floors)[i] = *floor_request;

    return true;
}

bool elevator_state_finished(elevator *elevator) {
    return elevator->state_progress == state_length[elevator->state];
}

void elevator_switch_state(elevator *elevator, elevator_state target_state) {
    elevator->state_progress = 0;
    elevator->state = target_state;
}

size_t request_weight(elevator *elevator, const floor_request *req) {
    const size_t floor_count = AVS_VECTOR_SIZE(elevator->elevator_system->floors);

    size_t distance = 0;
    if (req->floor_no != elevator->last_floor || elevator->state != EL_STATIONARY) {
        //the direction of elevator, when stopping at req->floor_no
        direction dest_dir = destination_direction(elevator, req);

        //following simulation can be turned into a O(1), gigantic pattern match
        size_t sim_floor = elevator->last_floor;
        direction sim_dir;
        if (elevator->direction == DIR_NONE) {
            sim_dir = req->floor_no > elevator->last_floor ? DIR_UP : DIR_DOWN;
        }
        else {
            sim_dir = elevator->direction;
        }

        do {
            distance++;

            if (sim_dir == DIR_UP) {
                if (sim_floor == floor_count - 1) { //bounce back
                    sim_dir = DIR_DOWN;
                }
                else {
                    sim_floor++;
                }
            }
            else {
                if (sim_floor == 0) { //bounce back
                    sim_dir = DIR_UP;
                }
                else {
                    sim_floor--;
                }
            }
        }
        while (sim_dir != dest_dir || sim_floor != req->floor_no);
    }

    //multiply distance and add 1 if REQ_DROPOFF to have a total order on all requests
    return distance * 2 + (req->req_type == REQ_DROPOFF ? 1 : 0);
}

direction destination_direction(elevator *elevator, const floor_request *req) {
    if (req->req_type == REQ_PICK_UP)         return DIR_UP;
    if (req->req_type == REQ_PICK_DOWN)       return DIR_DOWN;
    if (req->floor_no > elevator->last_floor) return DIR_UP;
    if (req->floor_no < elevator->last_floor) return DIR_DOWN;
    return elevator->direction == DIR_UP ? DIR_DOWN : DIR_UP;
}
