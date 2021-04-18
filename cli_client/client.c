#include <stdio.h>
#include <stddef.h>

#include <elevator_system.h>

int main() {
    elevator_system system;
    floor floors[] = {
        {
            .available_directions = DIR_UP,
            .label = "-1"
        },
        {
            .available_directions = DIR_BOTH,
            .label = "parter"
        },
        {
            .available_directions = DIR_DOWN,
            .label = "1"
        },
        {
            .available_directions = DIR_DOWN,
            .label = "2"
        }
    };

    elevator_system_init(&system, 10, sizeof(floors) / sizeof(*floors), floors);

    for (size_t i = 0; i < AVS_VECTOR_SIZE(system.elevators); i++) {
        printf("elevator no: %zu\n", (*system.elevators)[i].no);
    }
    for (size_t i = 0; i < AVS_VECTOR_SIZE(system.floors); i++) {
        floor *floors = *system.floors;
        printf("floor no: %zu; label: %s\n", floors[i].no, floors[i].label);
    }
    
    elevator_system_free(&system);
}
