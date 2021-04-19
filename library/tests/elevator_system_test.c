#include <avsystem/commons/avs_unit_test.h>

#include <elevator_system_data.h>
#include <elevator_system.h>
#include <elevator.h>

void example_es(elevator_system *system) {
    floor floors[] = {
        {
            .label = "-1",
            .available_directions = DIR_UP
        },
        {
            .label = "ground",
            .available_directions = DIR_BOTH
        },
        {
            .label = "+1",
            .available_directions = DIR_BOTH
        },
        {
            .label = "+2",
            .available_directions = DIR_BOTH
        },
        {
            .label = "+3",
            .available_directions = DIR_DOWN
        },
        {
            .label = "+4",
            .available_directions = DIR_DOWN
        },
        {
            .label = "+5",
            .available_directions = DIR_DOWN
        }
    };

    elevator_system_init(system, 1, sizeof(floors) / sizeof(*floors), floors);
}

AVS_UNIT_TEST(elevator_system, queue_dropoff_on_the_way) {
    elevator_system system;
    example_es(&system);

    //somebody wants to go down from 3rd floor
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_pickup(&system, 3, DIR_DOWN));

    //but in the meanwhile, if somebody wants to leave at floor 2, so the elevator should stop there first
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_dropoff(&system, 0, 2));

    AVS_UNIT_ASSERT_EQUAL(2, AVS_VECTOR_SIZE((*system.elevators)[0].queued_floors));
    AVS_UNIT_ASSERT_EQUAL(2, (*(*system.elevators)[0].queued_floors)[0].floor_no);
    AVS_UNIT_ASSERT_EQUAL(3, (*(*system.elevators)[0].queued_floors)[1].floor_no);

    elevator_system_free(&system);
}

AVS_UNIT_TEST(elevator_system, detect_duplicated_requests) {
    elevator_system system;
    example_es(&system);

    //initial
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_pickup(&system, 3, DIR_DOWN));
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_dropoff(&system, 0, 2));
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_dropoff(&system, 0, 1));

    //duplicates
    AVS_UNIT_ASSERT_FALSE(elevator_system_request_pickup(&system, 3, DIR_DOWN));
    AVS_UNIT_ASSERT_FALSE(elevator_system_request_dropoff(&system, 0, 2));

    AVS_UNIT_ASSERT_EQUAL(3, AVS_VECTOR_SIZE((*system.elevators)[0].queued_floors));

    elevator_system_free(&system);
}

AVS_UNIT_TEST(elevator_system, reverse_order_in_DIR_DOWN) {
    elevator_system system;
    example_es(&system);

    //the elevator should pick up users starting from the highest request
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_pickup(&system, 1, DIR_DOWN));
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_pickup(&system, 2, DIR_DOWN));
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_pickup(&system, 3, DIR_DOWN));
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_pickup(&system, 4, DIR_DOWN));
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_pickup(&system, 5, DIR_DOWN));

    AVS_UNIT_ASSERT_EQUAL(5, (*(*system.elevators)[0].queued_floors)[0].floor_no);
    AVS_UNIT_ASSERT_EQUAL(4, (*(*system.elevators)[0].queued_floors)[1].floor_no);
    AVS_UNIT_ASSERT_EQUAL(3, (*(*system.elevators)[0].queued_floors)[2].floor_no);
    AVS_UNIT_ASSERT_EQUAL(2, (*(*system.elevators)[0].queued_floors)[3].floor_no);
    AVS_UNIT_ASSERT_EQUAL(1, (*(*system.elevators)[0].queued_floors)[4].floor_no);

    for (size_t i = 0; i < 1000; i++) {
        elevator_system_step(&system);
    }

    //the elevators stays at the floor it visited last
    AVS_UNIT_ASSERT_EQUAL(1, (*system.elevators)[0].last_floor);

    elevator_system_free(&system);
}

//symmetrical case as above, but the elevator start at top floor first
AVS_UNIT_TEST(elevator_system, reverse_order_in_DIR_UP) {
    elevator_system system;
    example_es(&system);

    (*system.elevators)[0].last_floor = 6;
    
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_pickup(&system, 1, DIR_UP));
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_pickup(&system, 2, DIR_UP));
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_pickup(&system, 3, DIR_UP));
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_pickup(&system, 4, DIR_UP));
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_pickup(&system, 5, DIR_UP));

    AVS_UNIT_ASSERT_EQUAL(1, (*(*system.elevators)[0].queued_floors)[0].floor_no);
    AVS_UNIT_ASSERT_EQUAL(2, (*(*system.elevators)[0].queued_floors)[1].floor_no);
    AVS_UNIT_ASSERT_EQUAL(3, (*(*system.elevators)[0].queued_floors)[2].floor_no);
    AVS_UNIT_ASSERT_EQUAL(4, (*(*system.elevators)[0].queued_floors)[3].floor_no);
    AVS_UNIT_ASSERT_EQUAL(5, (*(*system.elevators)[0].queued_floors)[4].floor_no);

    for (size_t i = 0; i < 1000; i++) {
        elevator_system_step(&system);
    }

    //the elevators stays at the floor it visited last
    AVS_UNIT_ASSERT_EQUAL(5, (*system.elevators)[0].last_floor);

    elevator_system_free(&system);
}

AVS_UNIT_TEST(elevator_system, elevator_comes_back_to_floor_with_both_requests) {
    elevator_system system;
    example_es(&system);

    //if there are two requests, to travel up and down from given floor, the elevators should plan
    //stopping there twice
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_pickup(&system, 2, DIR_UP));
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_pickup(&system, 2, DIR_DOWN));
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_pickup(&system, 4, DIR_DOWN));

    AVS_UNIT_ASSERT_EQUAL(2, (*(*system.elevators)[0].queued_floors)[0].floor_no);
    AVS_UNIT_ASSERT_EQUAL(REQ_PICK_UP, (*(*system.elevators)[0].queued_floors)[0].req_type);
    AVS_UNIT_ASSERT_EQUAL(4, (*(*system.elevators)[0].queued_floors)[1].floor_no);
    AVS_UNIT_ASSERT_EQUAL(REQ_PICK_DOWN, (*(*system.elevators)[0].queued_floors)[1].req_type);
    AVS_UNIT_ASSERT_EQUAL(2, (*(*system.elevators)[0].queued_floors)[2].floor_no);
    AVS_UNIT_ASSERT_EQUAL(REQ_PICK_DOWN, (*(*system.elevators)[0].queued_floors)[2].req_type);

    elevator_system_free(&system);
}

AVS_UNIT_TEST(elevator_system, elevator_if_idle_chooses_direction_of_the_first_request) {
    elevator_system system1;
    elevator_system system2;

    example_es(&system1);
    example_es(&system2);

    (*system1.elevators)[0].last_floor = 2;
    (*system2.elevators)[0].last_floor = 2;

    AVS_UNIT_ASSERT_TRUE(elevator_system_request_dropoff(&system1, 0, 3));
    AVS_UNIT_ASSERT_TRUE(elevator_system_request_dropoff(&system2, 0, 1));

    elevator_system_step(&system1);
    elevator_system_step(&system1);
    elevator_system_step(&system2);
    elevator_system_step(&system2);

    AVS_UNIT_ASSERT_EQUAL(DIR_UP, (*system1.elevators)[0].direction);
    AVS_UNIT_ASSERT_EQUAL(DIR_DOWN, (*system2.elevators)[0].direction);

    elevator_system_free(&system1);
    elevator_system_free(&system2);   
}
