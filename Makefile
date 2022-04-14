.PHONY: redo clean debug

ARG := 

CXX ?= clang++
CFLAGS := -Wall -Wextra -ffast-math
CFLAGS += -ftree-vectorize -march=native 

SRC = $(wildcard src/*.cpp)
SRC += $(wildcard src/frontend/*.cpp)
SRC_C_H = $(SRC) $(wildcard src/include/*.h)

ANALYZE = -fanalyzer
ifeq ($(CXX), clang++)
ANALYZE = -Xanalyzer
CFLAGS += 
else
CFLAGS += 
endif


STRICT  = -Werror
CSTD = -std=c++17
CSTD_LINT = --std=c++17
DEBUG  = -g
BIN  = ./build/rotate

run: all
	@$(BIN) $(ARG)

all:
	@cmake --build build


redo: clean gen build
	@echo done

gen:
	@cmake -S . -B build -G Ninja

scan:
	@rm -rf output
	@scan-build -o ./output $(CXX) $(SRC) -o $(BIN) $(CFLAGS) $(DEBUG) $(CSTD) $(LIB)
	@scan-view ./output/*

fast:
	$(CXX) $(SRC) -o $(BIN) $(CFLAGS) $(ANALYZE) $(CSTD) $(LIB) -Ofast

afl:
	afl-$(CXX) $(SRC) -o $(BIN) $(CFLAGS) $(DEBUG) $(CSTD) $(LIB)

debug:
	$(CXX) $(SRC) -o $(BIN) $(CFLAGS) $(ANALYZE) $(DEBUG) $(CSTD) $(LIB) -fsanitize=undefined -DDEBUG_MODE

hidden_debug:
	@$(CXX) $(SRC) -o $(BIN) $(CFLAGS) $(ANALYZE) $(DEBUG) $(CSTD) $(LIB) -fsanitize=undefined -DDEBUG_MODE



clean:
	@rm -rf output*
	@rm -rf build/


memcheck: debug
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./$(BIN) ./main.vr

lint:
	@cppcheck  $(SRC_C_H) $(CSTD_LINT) --enable=all --check-config --quiet

format:
	@clang-format $(SRC_C_H) -i

release:
	$(CXX) $(SRC) -O2 -Ofast -o $(BIN) $(CFLAGS) $(CSTD) $(LIB) $(ANALYZE) -Werror $(STRICT)
