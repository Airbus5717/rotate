.PHONY: redo clean debug all

ARG := 
CXX ?= clang++
SRC = $(wildcard src/*.cpp)
SRC += $(wildcard src/**/*.cpp)
SRC_C_H = src/**/*.cpp src/**/*.hpp
STRICT  = -Werror
CSTD = -std=c++11
CSTD_LINT = --std=c++11
DEBUG  = -g -DDEBUG -ggdb3 -pg	
BIN  = ./build/vr
CFLAGS := -Wall -Wextra -Wpedantic -ffast-math -Wno-unused 
CFLAGS += -finline-functions -fno-strict-aliasing -funroll-loops
CFLAGS += -march=native -mtune=native -Wwrite-strings -fno-exceptions
CFLAGS += -static


ANALYZE := 
ifeq ($(CXX), gcc)
ANALYZE = -fanalyzer
CFLAGS += -Wcast-align=strict
else ifeq ($(CXX), clang++)
#ANALYZE = -Xanalyzer
CFLAGS += -Wcast-align
else
CFLAGS += 
endif



all: format
	@cmake --build build


redo: clean gen build all
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
	$(CXX) $(SRC) -o $(BIN) $(CFLAGS) $(ANALYZE) $(DEBUG) $(CSTD) $(LIB)

debug_gdb: debug
	gdb --tui $(BIN)
#hidden_debug:
#	@$(CXX) $(SRC) -o $(BIN) $(CFLAGS) $(ANALYZE) $(DEBUG) $(CSTD) $(LIB) -fsanitize=undefined



clean:
	@rm -rf output*
	@rm -rf build/


memcheck: debug
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s $(BIN) ./main.vr

lint:
	@cppcheck  $(SRC_C_H) $(CSTD_LINT) --enable=all --check-config --quiet 

format:
	@clang-format -i src/**/*.cpp src/**/*.hpp 

release:
	$(CXX) $(SRC) -O2 -Ofast -o $(BIN) $(CFLAGS) $(CSTD) $(LIB) $(ANALYZE) -Werror $(STRICT)
