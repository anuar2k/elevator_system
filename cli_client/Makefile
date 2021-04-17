CC      := gcc
CFLAGS  := -g2 -Wall
ARFLAGS := rcs

.PHONY: all clean

all: build/client

build/client: client.c ../library/build/libelevator_system.a
	$(CC) $(CFLAGS) \
		-I../library/deps/avs_commons/include_public \
		-I../library/include \
		-I../library/config \
		-L../library/build \
		client.c \
		-lelevator_system \
		-o build/client

../library/build/libelevator_system.a:
	make -C ../library build/libelevator_system.a

clean:
	$(RM) build/*