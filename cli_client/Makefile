CC      := gcc
CFLAGS  := -g2 -Wall
ARFLAGS := rcs

.PHONY: all clean

all: build/cli_client

build/cli_client: client.c ../library/build/libelevator_system.a
	mkdir -p build
	$(CC) $(CFLAGS) \
		-I../library/deps/avs_commons/include_public \
		-I../library/deps/avs_commons/src \
		-I../library/include \
		-I../library/config \
		-I/usr/include \
		-L/usr/lib \
		-L../library/build \
		client.c \
		$(shell find ../library/deps/avs_commons/src -name '*.c') \
		-lelevator_system \
		-lncurses \
		-lm \
		-o build/cli_client

../library/build/libelevator_system.a:
	make -C ../library build/libelevator_system.a

clean:
	$(RM) build/*