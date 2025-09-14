#ifndef _TETRIS_GAME_STATE_H_
#define _TETRIS_GAME_STATE_H_

#include "const.h"
#include "figures.h"

// all of possible actions
typedef enum {
  Start,
  Pause,
  Replay,
  Exit,
  MoveLeft,
  MoveRight,
  MoveDown,
  Rotate
} Action;

// all of possible states
typedef enum { Waiting, Spawn, Moving, Shifting, Attaching, GameOver } State;

// allias
typedef int Field[Y_SIZE][X_SIZE];
typedef int (*FieldPtr)[X_SIZE];

// GameState represent state of tetris game
typedef struct {
  int score, max_score;
  int level;
  int tick;
  int is_paused;
  State state;
  Figure figure;
  FigureShape next_shape;
  FigureCoord next_fig_coord;
  Field field;
} GameState;

// funcs
GameState *game_state();
GameState *init_game_state();
int read_max_score();
void reset_game_state(GameState *game);

void tick_time(GameState *game);

void start_game(GameState *game);
void pause_game(GameState *game);

void spawn_next_figure(GameState *game);
void place_figure_on_field(GameState *game);
void remove_figure_from_field(GameState *game);
int check_collision(GameState *game, Figure *fig_moved);

void make_rotation(GameState *game);
void move_figure_down(GameState *game);
void move_figure_left(GameState *game);
void move_figure_right(GameState *game);

int clear_lines(GameState *game);
void handle_score(GameState *game, int cleared_lines);
void handle_max_score(GameState *game);
void attach_figure(GameState *game);

#endif
