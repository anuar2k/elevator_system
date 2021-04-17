CC      := gcc
CFLAGS  := -g2 -Wall

.PHONY: all clean

all: build/libelevator_system.a

build/libelevator_system.a: src/elevator_system.c include/elevator_system.h
	$(CC) $(CFLAGS) \
		-Ideps/avs_commons/include_public \
		-Ideps/avs_commons/src \
		-Iconfig \
		-Iinclude \
		-c \
		$(shell find deps/avs_commons/src -name '*.c') \
		src/elevator_system.c
	$(AR) rcs build/libelevator_system.a *.o
	$(RM) *.o

clean:
	$(RM) build/*