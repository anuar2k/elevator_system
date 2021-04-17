#include <stdint.h>
#include <stdbool.h>
#include <avsystem/commons/avs_vector.h>

typedef enum {
    DIR_NONE = 0b00,
    DIR_UP   = 0b01,
    DIR_DOWN = 0b10,
    DIR_BOTH = 0b11
} direction;

typedef enum {
    EL_STAT_CLOSED,
    EL_STAT_OPENING,
    EL_STAT_OPENED,
    EL_STAT_CLOSING,
    EL_EN_ROUTE,
    EL_ARRIVING
} elevator_state;

typedef enum {
    REQ_PICK_UP,
    REQ_PICK_DOWN,
    REQ_DROPOFF
} req_type;

typedef struct {
    int16_t no;
    direction available_directions;
    direction requested_directions;
} floor;

typedef struct {
    req_type req_type;
    int16_t floor_no;
} floor_request;

typedef struct {
    uint16_t no;
    int16_t last_floor;
    direction direction;
    elevator_state state;
    AVS_VECTOR(floor_request) queued_floors;
} elevator;

typedef struct {
    AVS_VECTOR(elevator) elevators;
    AVS_VECTOR(floor) floors;
} elevator_system;

void elevator_system_init(elevator_system *system, uint16_t elevator_count, uint16_t floor_count, floor floors[]);
void elevator_system_free(elevator_system *system);

void elevator_system_step(elevator_system *system);
void elevator_system_request_pickup(elevator_system *system, floor_request *request);
void elevator_system_request_dropoff(elevator_system *system, uint16_t elevator_no, int16_t floor_no);

void elevator_step(elevator *elevator);