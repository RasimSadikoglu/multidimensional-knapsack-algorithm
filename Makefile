CXX:=g++
VERBOSITY_FLAGS:=-Iinclude -std=c++23 -Ofast -Wall -Werror -Wextra -Wpedantic \
	-Wconversion -Wcast-align -Wunused -Wpointer-arith -Wcast-qual \
	-Wno-missing-braces -Wold-style-cast # -Wshadow
MACROS:=
PERFORMANCE_MACROS:=

DEPS:=greedy knapsack utility
OBJS:=main greedy knapsack utility
DIRS:=bin obj results
BIN:=bin/main

FILE_NAME:=sample1.txt

_DEPS:=$(patsubst %, include/%.h, $(DEPS))
_OBJS:=$(patsubst %, obj/%.o, $(OBJS))

$(BIN): $(DIRS) $(_OBJS)
	$(CXX) $(_OBJS) -o $@

obj/%.o: src/%.cpp $(_DEPS)
	$(CXX) $(CXXFLAGS) $(OPT_FLAGS) $(VERBOSITY_FLAGS) $(MACROS) -c $< -o $@

run: bin/main
	bin/main $(FILE_NAME)

debug: bin/main
	gdb bin/main

valgrind: bin/main
	valgrind --leak-check=full --show-leak-kinds=all bin/main

$(DIRS):
	mkdir $@

.PHONY: clean
clean:
	rm -rf ${DIRS} .vscode