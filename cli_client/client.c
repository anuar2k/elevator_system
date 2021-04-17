#include <stdio.h>

#include <elevator_system.h>

int main() {
    elevator_system system;
    floor zero = {
        .no = 0,
        .available_directions = DIR_BOTH
    };

    elevator_system_init(&system, 10, 1, &zero);
    for (uint16_t i = 0; i < AVS_VECTOR_SIZE(system.elevators); i++) {
        printf("elevator no: %hd\n", (*system.elevators)[i].no);
    }
    elevator_system_free(&system);
}
