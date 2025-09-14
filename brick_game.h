#ifndef _BRICK_GAME_H_
#define _BRICK_GAME_H_

#include <stdio.h>
#include <stdlib.h>

#include "brick_game/tetris/tetris.h"
#include "gui/cli/include/cli.h"

#define KEY_S 115
#define KEY_P 112
#define KEY_R 114
#define KEY_Q 113

// main funcs
void run_game_loop(GameState *game, GameInfo *game_info);

// adapter funcs
void transfer_game_info(GameState *game, GameInfo *game_info);
Action get_user_action(int ch);

#endif
