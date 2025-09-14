#include <check.h>

#include "tests.h"

START_TEST(game_move_left) {
  GameState *game = init_tetris();

  update_state(game, Start);
  update_state(game, -1);

  int xc = game->figure.xc, yc = game->figure.yc;
  update_state(game, MoveLeft);

  ck_assert_int_eq(game->figure.xc, xc - 1);
  ck_assert_int_eq(game->figure.yc, yc);
}

START_TEST(game_move_right) {
  GameState *game = init_tetris();

  update_state(game, Start);
  update_state(game, -1);

  int xc = game->figure.xc, yc = game->figure.yc;
  update_state(game, MoveRight);

  ck_assert_int_eq(game->figure.xc, xc + 1);
  ck_assert_int_eq(game->figure.yc, yc);
}

START_TEST(game_move_down) {
  GameState *game = init_tetris();

  update_state(game, Start);
  update_state(game, -1);

  int xc = game->figure.xc, yc = game->figure.yc;
  update_state(game, MoveDown);

  ck_assert_int_eq(game->figure.xc, xc);
  ck_assert_int_eq(game->figure.yc, yc + 1);
}

START_TEST(game_move_rotate) {
  GameState *game = init_tetris();

  update_state(game, Start);
  update_state(game, -1);

  update_state(game, Rotate);
  ck_assert_int_eq(game->figure.rotation, ROTATE_90);

  update_state(game, Rotate);
  update_state(game, Rotate);
  update_state(game, Rotate);
  ck_assert_int_eq(game->figure.rotation, ROTATE_0);
}

Suite *tetris_game_move() {
  Suite *s = suite_create("tetris_game_move");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, game_move_left);
  tcase_add_test(tc_core, game_move_right);
  tcase_add_test(tc_core, game_move_down);
  tcase_add_test(tc_core, game_move_rotate);

  suite_add_tcase(s, tc_core);
  return s;
}
