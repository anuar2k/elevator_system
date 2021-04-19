# library
Elevator scheduling algorithm implementation in a form of a C library.



Dependencies:
- `avs_commons`

## Building
To build the library itself for your current platform, run `make`.

Your final program will also need `avs_commons` baked in with `AVS_COMMONS_WITH_AVS_VECTOR` and `AVS_COMMONS_WITH_AVS_UNIT` components. Refer to [avs_commons README](https://github.com/AVSystem/avs_commons/blob/master/README.md) or check out how [cli_client](https://github.com/anuar2k/elevator_system/tree/master/cli_client) does it.

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

