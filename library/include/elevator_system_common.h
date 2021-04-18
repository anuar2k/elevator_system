#ifndef ELEVATOR_SYSTEM_COMMON_H
#define ELEVATOR_SYSTEM_COMMON_H

#include <stdint.h>

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
    int16_t floor_no;
} floor_request;

#endif //ELEVATOR_SYSTEM_COMMON_H
