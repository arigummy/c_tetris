#ifndef S21_TETRIS_LIB_H
#define S21_TETRIS_LIB_H


#include "fsm.h"
#include "structs.h"
#include "all_funcs.h"

#include <stdbool.h>

#define DELAY_TIME 900


void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();

// -------------------------------------------

game_data_t get_all_data();
void set_sig(Signal_t sig);
void set_state(GameState_t state);

// -------------------------------------------

void compile_field();

void tick();

// -------------------------------------------

void init_game();
void end_game();
void set_pattern_1();

#endif
