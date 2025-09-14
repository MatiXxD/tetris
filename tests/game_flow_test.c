#include <check.h>

#include "tests.h"

START_TEST(game_flow_start) {
  GameState *game = init_tetris();

  int finished = update_state(game, Start);
  ck_assert_int_eq(finished, GAME_NOT_FINISHED);
  ck_assert_int_eq(game->state, Spawn);

  update_state(game, -1);
  ck_assert_int_eq(game->state, Moving);
}

START_TEST(game_flow_pause) {
  GameState *game = init_tetris();

  int finished = update_state(game, Pause);
  ck_assert_int_eq(finished, GAME_NOT_FINISHED);
  ck_assert_int_eq(game->is_paused, 0);

  game->state = Moving;
  finished = update_state(game, Pause);
  ck_assert_int_eq(finished, GAME_NOT_FINISHED);
  ck_assert_int_eq(game->is_paused, 1);

  finished = update_state(game, Pause);
  ck_assert_int_eq(finished, GAME_NOT_FINISHED);
  ck_assert_int_eq(game->is_paused, 0);
}

START_TEST(game_flow_quit) {
  GameState *game = init_tetris();

  int finished = update_state(game, Exit);
  ck_assert_int_eq(finished, GAME_IS_FINISHED);
}

START_TEST(game_flow_replay) {
  GameState *game = init_tetris();
  game->state = GameOver;

  int finished = update_state(game, MoveRight);
  ck_assert_int_eq(finished, GAME_NOT_FINISHED);
  ck_assert_int_eq(game->state, GameOver);

  finished = update_state(game, Replay);
  ck_assert_int_eq(finished, GAME_NOT_FINISHED);
  ck_assert_int_eq(game->state, Waiting);
}

Suite *tetris_game_flow() {
  Suite *s = suite_create("tetris_game_flow");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, game_flow_start);
  tcase_add_test(tc_core, game_flow_pause);
  tcase_add_test(tc_core, game_flow_quit);
  tcase_add_test(tc_core, game_flow_replay);

  suite_add_tcase(s, tc_core);
  return s;
}
