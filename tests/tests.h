#ifndef _TETRIS_TESTS_H_
#define _TETRIS_TESTS_H_

#include <check.h>

#include "../brick_game/tetris/tetris.h"

// tests funcs
Suite *tetris_game_init_suite();
Suite *tetris_game_flow();
Suite *tetris_game_move();
Suite *tetris_game_score_suite();

#endif
