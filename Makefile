
CC ?= clang
DMD ?= dmd
CFLAGS = -Wall 
CFLAGS += -Wextra 
CFLAGS += -Wpedantic
SRC = $(wildcard src/*.c)
SRC_C_H = $(SRC) $(wildcard src/include/*.h)
ANALYZE = -fanalyzer
STRICT  = -Werror 
CSTD = -std=gnu18
CSTD_LINT = --std=c11
DEBUG  = -g
BIN  = rotate


run: 
	$(CC) $(SRC) -o $(BIN) $(CFLAGS) $(CSTD) $(LIB)

zig:
	zig cc $(SRC) -o $(BIN) $(CFLAGS) $(CSTD) $(LIB)


test:
	$(DMD) -run test.d 

scan:
	-rm -rf output
	scan-build -o ./output make
	scan-view ./output/*   

strict:
	$(CC) $(SRC) -o $(BIN) $(CFLAGS) $(DEBUG) $(STRICT) $(CSTD) $(LIB)

afl:
	afl-clang $(SRC) -o $(BIN) $(CFLAGS) $(DEBUG) $(CSTD) $(LIB)  

debug:
	gcc-10 $(SRC) -o $(BIN) $(CFLAGS) $(ANALYZE) $(DEBUG) $(CSTD) $(LIB)

clean:
	-rm $(BIN) $(wildcard *.o)  
	-rm -rf output*

memcheck:
	-make debug
	valgrind --leak-check=full --track-origins=yes -s ./$(BIN) ./main.vr

lint:
	cppcheck  $(SRC_C_H) $(CSTD_LINT)

edit:
	vim ./src/

pdf:
	render50 src/*.c src/include/*.h -o code.pdf
