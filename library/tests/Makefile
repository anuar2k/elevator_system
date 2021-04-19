CC       := gcc
CFLAGS   := -g2 -Wall
SRC      := $(wildcard ../src/*.c)
TEST_SRC := $(wildcard *.c)
INCLUDES := $(wildcard ../include/*.h)

.PHONY: all test clean

all: test

test: test_runner
	./test_runner --verbose

test_runner: $(SRC) $(TEST_SRC) $(INCLUDES)
	$(CC) $(CFLAGS) \
		-DUNIT_TESTING \
		-I../deps/avs_commons/include_public \
		-I../deps/avs_commons/src \
		-Iconfig \
		-I../include \
		-I. \
		$(shell find ../src ../deps/avs_commons/src -name '*.c') \
		-lm \
		-o test_runner

clean:
	$(RM) test_runner
	