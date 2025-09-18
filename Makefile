CC = gcc
# CFLAGS = -g
CFLAGS = -Wall -Werror -Wextra
LDFLAGS = -lncurses -L. -ltetris -lgui
ARFLAGS = rcs

GCOV_FLAGS = --coverage
LCOV_FLAGS = --no-external
REMOVE_PATTERNS = "*/tests/*" "*/brick_game_test.c"

TEST_SRC = brick_game_test.c $(wildcard tests/*.c)
TEST_BIN = brick_game_test

TETRIS_BIN = tetris_game
TETRIS_SRC = brick_game.c adapter.c
TETRIS_OBJ = $(TETRIS_SRC:.c=.o)
TETRIS_DATA_FILE = data.txt

TETRIS_LIB = libtetris.a
TETRIS_LIB_SRC = $(wildcard brick_game/tetris/src/*.c)
TETRIS_LIB_OBJ = $(TETRIS_LIB_SRC:.c=.o)

GUI_LIB = libgui.a
GUI_LIB_SRC = $(wildcard gui/cli/src/*.c)
GUI_LIB_OBJ = $(GUI_LIB_SRC:.c=.o)

CHECK_LIB = $(shell pkg-config --cflags --libs check)

CVG_NAME = coverage.out
CVG_DIR = coverage

OS = $(shell uname)


%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(TETRIS_LIB): $(TETRIS_LIB_OBJ)
	@$(AR) $(ARFLAGS) $(TETRIS_LIB) $(TETRIS_LIB_OBJ)
	@ranlib $(TETRIS_LIB)

$(GUI_LIB): $(GUI_LIB_OBJ)
	@$(AR) $(ARFLAGS) $(GUI_LIB) $(GUI_LIB_OBJ)
	@ranlib $(GUI_LIB)

$(TETRIS_BIN): $(TETRIS_OBJ) $(TETRIS_LIB) $(GUI_LIB)
	@$(CC) $(CFLAGS) $(TETRIS_OBJ) $(LDFLAGS) -o $@

$(TEST_BIN): $(TEST_SRC) $(TETRIS_LIB_SRC)
	@$(CC) $(CFLAGS) $(TEST_SRC) $(TETRIS_LIB_SRC) -o $(TEST_BIN) $(CHECK_LIB)


.PHONY: all
all: tetris

.PHONY: tetris
tetris: $(TETRIS_BIN)

.PHONY: install
install: tetris

.PHONY: uninstall
uninstall: clean

.PHONY: dist
dist: clean
	@tar -czf ../tetris.tar.gz .

.PHONY: dvi
dvi:
	@xelatex -output-directory=docs docs/docs.tex

.PHONY: run
run: tetris
	@./$(TETRIS_BIN)

.PHONY: libs
libs: tetris-lib gui-lib

.PHONY: tetris-lib
tetris-lib: $(TETRIS_LIB)

.PHONY: gui-lib
gui-lib: $(GUI_LIB)

.PHONY: build
build: $(TETRIS_LIB) $(GUI_LIB)

.PHONY: test
test: clean $(TEST_BIN)
	@./$(TEST_BIN)

.PHONY: gcov_report
gcov_report: CFLAGS += $(GCOV_FLAGS)
gcov_report: clean $(TEST_BIN)
	@./$(TEST_BIN)
	@mkdir $(CVG_DIR) && mv *.gcno *.gcda $(CVG_DIR)/
	@lcov -d . -c -o $(CVG_NAME) $(LCOV_FLAGS)
	@lcov --remove $(CVG_NAME) $(REMOVE_PATTERNS) -o $(CVG_NAME)
	@genhtml $(CVG_NAME) -o $(CVG_DIR)
	@if [ "$(OS)" = "Linux" ]; then xdg-open $(CVG_DIR)/index.html; fi
	@if [ "$(OS)" = "Darwin" ]; then open $(CVG_DIR)/index.html; fi

.PHONY: clean
clean:
	@$(RM) $(TETRIS_BIN) $(TETRIS_DATA_FILE) $(TEST_BIN) $(CVG_NAME) $(TETRIS_LIB) $(GUI_LIB) $(TETRIS_LIB_OBJ) $(GUI_LIB_OJB) *.o *.gcno *.gcda
	@$(RM) -r $(CVG_DIR)

