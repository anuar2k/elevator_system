#ifndef ELEVATOR_SYSTEM_DATA_H
#define ELEVATOR_SYSTEM_DATA_H

#include <stddef.h>
#include <avsystem/commons/avs_vector.h>

typedef enum {
    DIR_NONE = 0b00,
    DIR_UP   = 0b01,
    DIR_DOWN = 0b10,
    DIR_BOTH = 0b11
} direction;

typedef enum {
    REQ_PICK_UP,
    REQ_PICK_DOWN,
    REQ_DROPOFF
} req_type;

typedef struct {
    req_type req_type;
    size_t floor_no;
} floor_request;

typedef enum {
    EL_STATIONARY = 0,
    EL_BETWEEN_FLOORS,
    EL_PASSING_BY,
    elevator_state_length
} elevator_state;

extern const size_t state_length[elevator_state_length];

typedef struct {
    size_t no;
    const char *label;
    direction available_directions;
    direction requested_directions;
} floor;

typedef struct elevator elevator;
typedef struct elevator_system elevator_system;

struct elevator {
    size_t no;
    size_t last_floor;
    direction direction;
    elevator_state state;
    size_t state_progress;
    AVS_VECTOR(floor_request) queued_floors;
    elevator_system *elevator_system;
};

struct elevator_system {
    AVS_VECTOR(elevator) elevators;
    AVS_VECTOR(floor) floors;
    size_t rr_next_elevator;
};

#endif //ELEVATOR_SYSTEM_DATA_H
