# elevator_system
This repository contains an example implementation of elevator scheduling algorithm, implemented in C.

## Contents
### library [*(more info inside)*](https://github.com/anuar2k/elevator_system/tree/master/library)
Elevator scheduling algorithm implementation in a form of a C library.

### cli_client [*(more info inside)*](https://github.com/anuar2k/elevator_system/tree/master/cli_client)
A CLI interface/visualisation of the algorithm.

Also available as a Docker image (based on Alpine Linux/musl), you can run it with `docker run -it anuar2k/elevator_system`.

### stm32_example
Minimal STM32CubeIDE project targeting `NUCLEO-F103RB` which proves that the library can be compiled and run on an embedded platform.