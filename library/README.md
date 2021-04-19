# library
Elevator scheduling algorithm implementation in a form of a C library.

The ordering on pick-up/drop-off requests is inspired with [LOOK disk scheduling algorithm](https://en.wikipedia.org/wiki/LOOK_algorithm), which mimics the behavior of real elevators and prevents starvation of requests. In case of pick-up requests from a given floor, the elevator is selected in a round-robin manner.

## Dependencies:
- [`avs_commons`](https://github.com/AVSystem/avs_commons)

## Building
To build the library itself for your current platform, run `make`.

Your final program will also need `avs_commons` baked in with `AVS_COMMONS_WITH_AVS_VECTOR` and `AVS_COMMONS_WITH_AVS_UTILS` components. Refer to [avs_commons README](https://github.com/AVSystem/avs_commons/blob/master/README.md) or check out how [cli_client](https://github.com/anuar2k/elevator_system/tree/master/cli_client) does it.

## Unit tests
To run the test suite for the library, run `make test`. It uses `avs_commons` unit testing framework.

## C interface
```c
void elevator_system_init(elevator_system *system, size_t elevator_count, size_t floor_count);
```
Initializes the `elevator_system` structure.

```c
void elevator_system_free(elevator_system *system);
```
Frees the internal storage used by `elevator_system`.

```c
void elevator_system_step(elevator_system *system);
```
Advances the simulation by single step.

```c
bool elevator_system_request_pickup(elevator_system *system, size_t floor_no, direction direction);
```
Schedules a pick-up request for a given floor (outside up/down button), after which the elevator will continue moving in given direction. The elevator, which will fulfill the request is selected in round-robin manner.

Returns:
- `true`, if the request was successfully scheduled,
- `false`, if the same request was already made (a duplicate).

```c
bool elevator_system_request_dropoff(elevator_system *system, size_t elevator_no, size_t floor_no);
```
Schedules a drop-off request to a given elevator (floor selector inside the elevator).

Returns:
- `true`, if the request was successfully scheduled,
- `false`, if the same request was already made (a duplicate).
