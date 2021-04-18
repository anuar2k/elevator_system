CC       := gcc
CFLAGS   := -g2 -Wall
SRC      := $(wildcard src/*.c)
OBJ      := $(patsubst src/%.c, obj/%.o, $(SRC))
INCLUDES := $(wildcard include/*.h)

.PHONY: all clean

all: build/libelevator_system.a

build/libelevator_system.a: $(OBJ)
	mkdir -p build
	$(AR) rcs build/libelevator_system.a obj/*.o

obj/%.o: src/%.c $(INCLUDES)
	mkdir -p obj
	$(CC) $(CFLAGS) \
		-Ideps/avs_commons/include_public \
		-Ideps/avs_commons/src \
		-Iconfig \
		-Iinclude \
		-c \
		$< -o $@

clean:
	$(RM) build/* obj/*