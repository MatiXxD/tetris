#include "brick_game.h"

#include "gui/cli/include/cli.h"

int main() {
  GameInfo game_info;
  GameState *game = init_tetris();

  init_cli();
  run_game_loop(game, &game_info);
  finish_cli();

  return 0;
}

void run_game_loop(GameState *game, GameInfo *game_info) {
  int is_game_finished = 0;

  while (!is_game_finished) {
    // update game info
    game = game_state();
    transfer_game_info(game, game_info);

    // get input
    int ch = getch();

    // render game
    render(game_info);
    timeout(10);

    // update state
    is_game_finished = update_state(game, get_user_action(ch));
  }
}
