#include "../tetris.h"

GameState *game_state() {
  static GameState game;
  return &game;
}

GameState *init_game_state() {
  GameState *game = game_state();
  reset_game_state(game);

  game->max_score = read_max_score();

  return game;
}

int read_max_score() {
  FILE *f = fopen(DATA_FILE_PATH, "r");
  if (f == NULL) {
    return 0;
  }

  int max_score = 0;
  if (fscanf(f, "%d", &max_score) != 1) {
    fclose(f);
    return 0;
  }

  fclose(f);
  return max_score;
}

void reset_game_state(GameState *game) {
  game->score = 0;
  game->level = 1;
  game->tick = DEFAULT_TICK_TIME;

  game->state = Waiting;

  game->figure.xc = X_SIZE / 2;
  game->figure.yc = 1;
  game->figure.shape = get_random_shape();
  game->figure.rotation = ROTATE_0;
  update_figure_coord(&game->figure);

  game->next_shape = get_random_shape();

  // clear field
  for (int i = 0; i < Y_SIZE; i++) {
    for (int j = 0; j < X_SIZE; j++) {
      game->field[i][j] = EMPTY_CELL;
    }
  }
}

void tick_time(GameState *game) {
  if (--game->tick <= 0) {
    game->tick = DEFAULT_TICK_TIME - LVL_TICK * (game->level - 1);
    game->state = Shifting;
  }
}

void place_figure_on_field(GameState *game) {
  // should be checked before call place_figure, i did this check just in case
  if (check_borders(&game->figure) == CHECK_BORDER_FAIL) {
    return;
  }

  for (int i = 0; i < FIG_COORD_ROWS; i++) {
    int x = game->figure.coords[i][0];
    int y = game->figure.coords[i][1];

    game->field[y][x] = game->figure.shape + EMPTY_CELL_OFFSET;
  }
}

void remove_figure_from_field(GameState *game) {
  // should be checked before call place_figure, i did this check just in case
  if (check_borders(&game->figure) == CHECK_BORDER_FAIL) {
    return;
  }

  for (int i = 0; i < FIG_COORD_ROWS; i++) {
    int x = game->figure.coords[i][0];
    int y = game->figure.coords[i][1];

    game->field[y][x] = EMPTY_CELL;
  }
}

void start_game(GameState *game) {
  // this check just in case
  if (game->state == Waiting) {
    game->state = Spawn;
  }
}

void pause_game(GameState *game) {
  if (game->state != GameOver || game->state != Waiting) {
    game->is_paused ^= 1;
  }
}

void spawn_next_figure(GameState *game) {
  game->figure.xc = X_SIZE / 2;
  game->figure.yc = 1;
  game->figure.shape = game->next_shape;
  game->figure.rotation = ROTATE_0;
  update_figure_coord(&game->figure);

  game->next_shape = get_random_shape();
  get_coord_for_shape(game->next_fig_coord, game->next_shape);

  // check if lose
  if (check_collision(game, &game->figure) == CHECK_COLLISION_FAIL) {
    game->state = GameOver;
    handle_max_score(game);
    return;
  }

  place_figure_on_field(game);

  // change game state
  game->state = Moving;
}

int check_collision(GameState *game, Figure *fig_moved) {
  if (check_borders(fig_moved) == CHECK_BORDER_FAIL) {
    return CHECK_COLLISION_FAIL;
  }

  FieldPtr field = game->field;
  for (int i = 0; i < FIG_COORD_ROWS; i++) {
    int x = fig_moved->coords[i][0];
    int y = fig_moved->coords[i][1];

    if (field[y][x] != EMPTY_CELL) {
      return CHECK_COLLISION_FAIL;
    }
  }

  return CHECK_COLLISION_OK;
}

void make_rotation(GameState *game) {
  Figure tmp = copy(&game->figure);
  rotate_clockwise(&tmp);

  // remove current fig from field before collision check
  remove_figure_from_field(game);

  if (check_collision(game, &tmp) == CHECK_COLLISION_FAIL) {
    place_figure_on_field(game);
    return;
  }

  // no collision -> rotate figure and place on field
  rotate_clockwise(&game->figure);
  place_figure_on_field(game);
}

void move_figure_left(GameState *game) {
  Figure tmp = copy(&game->figure);
  move_fig(&tmp, MOVE_LEFT_DELTA, 0);

  // remove current fig from field before collision check
  remove_figure_from_field(game);

  if (check_collision(game, &tmp) == CHECK_COLLISION_FAIL) {
    place_figure_on_field(game);
    return;
  }

  // no collision -> move figure left and place on field
  move_fig(&game->figure, MOVE_LEFT_DELTA, 0);
  place_figure_on_field(game);
}

void move_figure_right(GameState *game) {
  Figure tmp = copy(&game->figure);
  move_fig(&tmp, MOVE_RIGHT_DELTA, 0);

  // remove current fig from field before collision check
  remove_figure_from_field(game);

  if (check_collision(game, &tmp) == CHECK_COLLISION_FAIL) {
    place_figure_on_field(game);
    return;
  }

  // no collision -> move figure right and place on field
  move_fig(&game->figure, MOVE_RIGHT_DELTA, 0);
  place_figure_on_field(game);
}

// can be triggered by inactivity (time ticked) or by move down action
void move_figure_down(GameState *game) {
  Figure tmp = copy(&game->figure);
  move_fig(&tmp, 0, MOVE_DOWN_DELTA);

  // remove current fig from field before collision check
  remove_figure_from_field(game);

  if (check_collision(game, &tmp) == CHECK_COLLISION_FAIL) {
    place_figure_on_field(game);
    game->state = Attaching;
    return;
  }

  // no collision -> move figure down and place on field
  move_fig(&game->figure, 0, MOVE_DOWN_DELTA);
  place_figure_on_field(game);
  game->state = Moving;
}

int clear_lines(GameState *game) {
  int lines_cleared = 0;

  FieldPtr field = game->field;
  for (int i = 0; i < Y_SIZE; i++) {
    int is_full_line = 1;
    for (int j = 0; j < X_SIZE; j++) {
      if (field[i][j] == EMPTY_CELL) {
        is_full_line = 0;
        break;
      }
    }

    if (is_full_line) {
      for (int ii = i; ii > 0; ii--) {
        for (int jj = 0; jj < X_SIZE; jj++) {
          field[ii][jj] = field[ii - 1][jj];
        }
      }

      for (int jj = 0; jj < X_SIZE; jj++) {
        field[0][jj] = EMPTY_CELL;
      }

      lines_cleared++;
    }
  }

  return lines_cleared;
}

void handle_score(GameState *game, int cleared_lines) {
  static int scores[5] = {
      0,
      CLEAR_ONE_LINE_SCORE,
      CLEAR_TWO_LINES_SCORE,
      CLEAR_THREE_LINES_SCORE,
      CLEAR_FOUR_LINES_SCORE,
  };

  // check just in case, cleared_lines can't be more than 4
  if (cleared_lines > 4) {
    cleared_lines = 4;
  }

  game->score += scores[cleared_lines];
  game->level = 1 + game->score / SCORE_TO_LVLUP;
}

void handle_max_score(GameState *game) {
  if (game->score < game->max_score) {
    return;
  }

  game->max_score = game->score;

  FILE *f = fopen(DATA_FILE_PATH, "w");
  if (f == NULL) {
    return;
  }

  fprintf(f, "%d", game->max_score);
  fclose(f);
}

void attach_figure(GameState *game) {
  handle_score(game, clear_lines(game));
  game->state = Spawn;
}
