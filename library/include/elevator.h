#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <stdbool.h>
#include <avsystem/commons/avs_vector.h>

#include "elevator_system_data.h"

void elevator_step(elevator *elevator);
bool elevator_enqueue_floor(elevator *elevator, const floor_request *floor_request);
int elevator_search_by_no_cmp(const void *a, const void *b);

bool elevator_state_finished(elevator *elevator);
void elevator_switch_state(elevator *elevator, elevator_state target_state);

#endif //ELEVATOR_H
