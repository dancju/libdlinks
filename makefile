CC = g++
CFLAGS = -std=c++20 -g -Wall
LDLIBS = -lboost_coroutine -lboost_unit_test_framework

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(SRC))
TEST_SRC = $(wildcard test_src/*.cpp)
TEST_BIN = $(patsubst test_src/%.cpp, test_bin/%, $(TEST_SRC))

all: $(OBJ)

obj/%.o: src/%.cpp src/%.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(TEST_BIN)
	@for target in $(TEST_BIN); do \
		echo Running $$target; \
		$$target; \
		echo; \
	done

test_bin/%: $(OBJ) test_src/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^

clean:
	rm $(OBJ) $(TEST_BIN)
