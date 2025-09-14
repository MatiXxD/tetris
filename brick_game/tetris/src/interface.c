#include "../tetris.h"

GameState *init_tetris() {
  srand(time(NULL));
  return init_game_state();
}

void get_current_field(GameState *game, FieldAuxPtr field) {
  FieldPtr inner_field = game->field;

  for (int i = 1; i < Y_SIZE; i++) {
    for (int j = 0; j < X_SIZE; j++) {
      field[i - 1][j] = inner_field[i][j];
    }
  }
}

int update_state(GameState *game, Action action) {
  // handle exit
  if (action == Exit) {
    return GAME_IS_FINISHED;
  }

  // handle start, gameover
  if (game->state == Waiting) {
    if (action == Start) {
      start_game(game);
    }
    return GAME_NOT_FINISHED;
  } else if (game->state == GameOver) {
    if (action == Replay) {
      reset_game_state(game);
    }
    return GAME_NOT_FINISHED;
  }

  // handle pause
  if (action == Pause) {
    pause_game(game);
    return GAME_NOT_FINISHED;
  } else if (game->is_paused) {
    return GAME_NOT_FINISHED;
  }

  // main events
  if (game->state == Spawn) {
    spawn_next_figure(game);
  } else if (game->state == Moving) {
    if (action == MoveDown) {
      move_figure_down(game);
    } else if (action == MoveLeft) {
      move_figure_left(game);
    } else if (action == MoveRight) {
      move_figure_right(game);
    } else if (action == Rotate) {
      make_rotation(game);
    }
    tick_time(game);
  } else if (game->state == Shifting) {
    move_figure_down(game);
  } else if (game->state == Attaching) {
    attach_figure(game);
  }

  return GAME_NOT_FINISHED;
}
