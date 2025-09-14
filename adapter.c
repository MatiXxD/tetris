#include "brick_game.h"

void transfer_game_info(GameState *game, GameInfo *game_info) {
  game_info->max_score = game->max_score;
  game_info->score = game->score;
  game_info->lvl = game->level;

  if (game->is_paused) {
    game_info->status = StatePause;
  } else if (game->state == GameOver) {
    game_info->status = StateGameOver;
  } else if (game->state == Waiting) {
    game_info->status = StateInitial;
  } else {
    game_info->status = StatePlaying;
  }

  get_current_field(game, game_info->field);
  get_coord_for_shape(game_info->next_fig, game->next_shape);
}

Action get_user_action(int ch) {
  switch (ch) {
  case KEY_S:
    return Start;
  case KEY_P:
    return Pause;
  case KEY_R:
    return Replay;
  case KEY_Q:
    return Exit;
  case KEY_LEFT:
    return MoveLeft;
  case KEY_RIGHT:
    return MoveRight;
  case KEY_UP:
    return Rotate;
  case KEY_DOWN:
    return MoveDown;
  }

  return -1;
}
