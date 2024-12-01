CC = gcc
C_FLAGS = -Wall -Werror -Wextra -std=c11
#C_FLAGS = -Wall
LC = lcov -c -d . -o

EXE_NAME = tetris

LIB_CMD = ar rc
GCOV_FLAGS = --coverage -fprofile-arcs -ftest-coverage

ifeq ($(shell uname -s), Linux)
TESTS_FLAGS = -lcheck -lsubunit -lm -lrt -lpthread
OPEN_CMD = xdg-open
endif
ifeq ($(shell uname -s), Darwin)
TESTS_FLAGS = -lcheck
OPEN_CMD = open
endif


SRC_PATH = brick_game/tetris/sources/
#SRC = $(wildcard $(SRC_PATH)*.c)

T_PATH = brick_game/tetris/unit_tests/
TESTS = $(wildcard $(T_PATH)*.c)

#---------------------------------------------------------

all: install gcov_report dvi dist

install: clean
	$(CC) $(C_FLAGS) gui/cli/*.c $(SRC_PATH)*.c -o ./$(EXE_NAME) -g -lncurses

test: clean
	$(CC) $(C_FLAGS) -DEBUG $(SRC_PATH)*.c $(TESTS) -o $@ $(TESTS_FLAGS)
	./$@

gcov_report: clean
	$(CC) -c $(C_FLAGS) -DEBUG $(TESTS)
	$(CC) -c $(C_FLAGS) -DEBUG $(GCOV_FLAGS) $(SRC_PATH)*.c
	$(CC) $(C_FLAGS) $(GCOV_FLAGS) *.o -o test $(TESTS_FLAGS)
	./test
	$(LC) report.info
	genhtml -o report report.info
	rm *.o *.gcda *.gcno test *.info
	$(OPEN_CMD) report/index.html

dist:
	tar -czvf yuunairo_brick_game-1.tar.gz ./

dvi:
	doxygen

check_style:
	@cp ../materials/linters/.clang-format ./
	@clang-format -n $(SRC_PATH)*.c $(T_PATH)*.c  gui/cli/* >> style 2>&1
	@if [ -s style ]; then \
	   echo "There are style errors!!! "; \
	else \
	   echo "Code is clean ( ˘⌣˘)"; \
	fi
	@rm style .clang-format

get_style:
	@cp ../materials/linters/.clang-format .
	@clang-format -i $(SRC_PATH)*.c $(T_PATH)*.c  gui/cli/*
	@rm .clang-format
	@echo "Code was stiled with clang-format!"

valgrind : clean test
	@valgrind --leak-check=full --log-file="logs" -s ./test
	@grep "ERROR SUMMARY" logs
	@rm logs test

clean:
	rm -fr ./build *.o *.gcno *.gcda *.info report html latex

uninstall: clean
	rm -f $(EXE_NAME)
