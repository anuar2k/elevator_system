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

    elevator_system_init(&system, 1, sizeof(floors) / sizeof(*floors), floors);

    bool x = elevator_system_request_pickup(&system, 3, DIR_DOWN);
    bool y = elevator_system_request_pickup(&system, 3, DIR_DOWN);
    bool z = elevator_system_request_pickup(&system, 1, DIR_DOWN);
    printf("%d %d %d\n", x, y, z);

    for (size_t i = 0; i < 50; i++) {
        elevator_system_step(&system);
        printf("floor: %zu\n", (*system.elevators)[0].last_floor);
    }
    for (size_t i = 0; i < AVS_VECTOR_SIZE(system.floors); i++) {
        floor *floors = *system.floors;
        printf("label: %s\n", floors[i].label);
    }
    
    elevator_system_free(&system);
}
