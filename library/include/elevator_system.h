#ifndef ELEVATOR_SYSTEM_H
#define ELEVATOR_SYSTEM_H

#include <stdint.h>
#include <stdbool.h>
#include <avsystem/commons/avs_vector.h>

#include "elevator_system_common.h"
#include "elevator.h"
#include "floor.h"

typedef struct {
    AVS_VECTOR(elevator) elevators;
    AVS_VECTOR(floor) floors;
    uint16_t rr_next_elevator;
} elevator_system;

void elevator_system_init(elevator_system *system, uint16_t elevator_count, uint16_t floor_count, const floor *floors);
void elevator_system_free(elevator_system *system);

void elevator_system_step(elevator_system *system);
bool elevator_system_request_pickup(elevator_system *system, int16_t floor_no, direction direction);
bool elevator_system_request_dropoff(elevator_system *system, uint16_t elevator_no, int16_t floor_no);

#endif //ELEVATOR_SYSTEM_H
