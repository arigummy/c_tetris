#ifndef FRONTEND_H
#define FRONTEND_H

#include <ncurses.h>

#include "../../brick_game/tetris/includes/s21_tetris_lib.h"

void init_ncurses();
void start_my_colors();
void end_ncurses();

void draw_all_game(GameInfo_t data);
void draw_console();
void draw_field(GameInfo_t data);
void draw_stats(GameInfo_t data);
void draw_figure(int** bitmap, int y, int x);

// ----------------------------

void draw_menu(int state);

void draw_banner();

void draw_pause_state();
void draw_exit_sate();

#endif
