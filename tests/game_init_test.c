#include <check.h>

#include "tests.h"

START_TEST(game_init_basic) {
  GameState *game = init_tetris();

  ck_assert_int_eq(game->score, 0);
  ck_assert_int_eq(game->level, 1);
  ck_assert_int_eq(game->state, Waiting);

  ck_assert_int_eq(game->figure.xc, X_SIZE / 2);
  ck_assert_int_eq(game->figure.yc, 1);
  ck_assert_int_eq(game->figure.rotation, ROTATE_0);

  ck_assert_int_eq(game->tick, DEFAULT_TICK_TIME);
  ck_assert_int_eq(game->is_paused, 0);
}

START_TEST(game_init_no_max_score) {
  GameState *game = init_tetris();

  ck_assert_int_eq(game->max_score, 0);
}

START_TEST(game_init_with_max_score) {
  FILE *f = fopen(DATA_FILE_PATH, "w");
  ck_assert_ptr_nonnull(f);

  int max_score = 800;
  fprintf(f, "%d", max_score);
  fclose(f);

  GameState *game = init_tetris();
  ck_assert_int_eq(game->max_score, max_score);

  ck_assert_int_eq(remove(DATA_FILE_PATH), 0);
}

START_TEST(game_init_field) {
  GameState *game = init_tetris();

  FieldAux got;
  get_current_field(game, got);

  for (int i = 0; i < Y_SIZE - 1; i++) {
    for (int j = 0; j < X_SIZE; j++) {
      ck_assert_int_eq(got[i][j], EMPTY_CELL);
    }
  }
}

Suite *tetris_game_init_suite() {
  Suite *s = suite_create("tetris_game_init");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, game_init_basic);
  tcase_add_test(tc_core, game_init_no_max_score);
  tcase_add_test(tc_core, game_init_with_max_score);
  tcase_add_test(tc_core, game_init_field);

  suite_add_tcase(s, tc_core);
  return s;
}
