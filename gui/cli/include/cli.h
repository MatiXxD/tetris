#ifndef _CLI_GUI_H_
#define _CLI_GUI_H_

#include <ncurses.h>

#include "const.h"

#define GAME_FIELD_X_SIZE 10
#define GAME_FIELD_Y_SIZE 20

// game field allias
typedef int GameField[GAME_FIELD_Y_SIZE][GAME_FIELD_X_SIZE];
typedef int (*GameFieldPtr)[GAME_FIELD_X_SIZE];

// fig coord allias
typedef int FigCoord[FIG_COORD_Y][FIG_COORD_X];

// possible state of game
typedef enum {
  StateInitial,
  StatePause,
  StateGameOver,
  StatePlaying
} GameStatus;

// represents game info to be displayed
typedef struct {
  int score;
  int max_score;
  int lvl;
  FigCoord next_fig;
  GameStatus status;
  GameField field;
} GameInfo;

// funcs
void init_cli();
void finish_cli();

void render(GameInfo *game_info);

WINDOW *get_help_window();
WINDOW *get_field_window(GameInfo *game_info);
WINDOW *get_game_info_window(GameInfo *game_info);

void print_next_figure_on_win(WINDOW *win, GameInfo *game_info);
void print_start_screen();
void print_pause_screen();
void print_game_over_screen();

#endif
