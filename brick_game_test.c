#include "tests/tests.h"

int main() {
  int number_failed = 0;
  Suite *suites[] = {
      tetris_game_init_suite(),
      tetris_game_flow(),
      tetris_game_move(),
      tetris_game_score_suite(),
  };

  for (size_t i = 0; i < sizeof(suites) / sizeof(Suite *); i++) {
    SRunner *sr = srunner_create(suites[i]);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);
  }

  return (number_failed == 0) ? 0 : 1;
}
