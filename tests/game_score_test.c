#include <check.h>

#include "tests.h"

START_TEST(game_clear_one_line) {
  GameState *game = init_tetris();

  update_state(game, Start);
  ck_assert_int_eq(game->score, 0);

  for (int i = Y_SIZE - 1; i > Y_SIZE - 2; i--) {
    for (int j = 0; j < X_SIZE; j++) {
      game->field[i][j] = 1;
    }
  }

  game->state = Attaching;
  update_state(game, -1);
  ck_assert_int_eq(game->score, CLEAR_ONE_LINE_SCORE);
}

START_TEST(game_clear_four_lines) {
  GameState *game = init_tetris();

  update_state(game, Start);
  ck_assert_int_eq(game->score, 0);

  for (int i = Y_SIZE - 1; i > Y_SIZE - 5; i--) {
    for (int j = 0; j < X_SIZE; j++) {
      game->field[i][j] = 1;
    }
  }

  game->state = Attaching;
  update_state(game, -1);
  ck_assert_int_eq(game->score, CLEAR_FOUR_LINES_SCORE);
}

START_TEST(game_max_score_handle) {
  GameState *game = init_tetris();

  update_state(game, Start);
  ck_assert_int_eq(game->score, 0);

  for (int i = Y_SIZE - 1; i > Y_SIZE - 5; i--) {
    for (int j = 0; j < X_SIZE; j++) {
      game->field[i][j] = 1;
    }
  }

  game->state = Attaching;
  update_state(game, -1);
  ck_assert_int_eq(game->score, CLEAR_FOUR_LINES_SCORE);

  ck_assert_int_eq(game->max_score, 0);
  handle_max_score(game);
  ck_assert_int_eq(game->max_score, CLEAR_FOUR_LINES_SCORE);

  remove(DATA_FILE_PATH);
}

Suite *tetris_game_score_suite() {
  Suite *s = suite_create("tetris_game_score");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, game_clear_one_line);
  tcase_add_test(tc_core, game_clear_four_lines);
  tcase_add_test(tc_core, game_max_score_handle);

  suite_add_tcase(s, tc_core);
  return s;
}
