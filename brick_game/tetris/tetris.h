#ifndef _TETRIS_LOGIC_H_
#define _TETRIS_LOGIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// includes
#include "include/const.h"
#include "include/figures.h"
#include "include/game_state.h"

// allias
typedef int FieldAux[Y_SIZE - 1][X_SIZE];
typedef int (*FieldAuxPtr)[X_SIZE];

// interface funcs
GameState *init_tetris();
void get_current_field(GameState *game, FieldAuxPtr field);
int update_state(GameState *game, Action action);

#endif
