#ifndef ALL_FUNCS_H
#define ALL_FUNCS_H


#include <stdlib.h>
#include "structs.h"
#include "fsm.h"
#include <sys/time.h>



// ------------------------------------

void init_field(int *** field);

// ------------------------------------

void init_figure(Figure_t * figure);
void generate_figure(Figure_t * figure);
void add_figure_on_ground(game_data_t * data);

// ------------------------------------

int vertical_collision(const game_data_t data);
int horizontal_collision(const game_data_t data);
int all_collision(const game_data_t data);
int inside_area(int y, int x);

// ------------------------------------

int get_max_score();
void save_max_score(game_data_t * data);

// ------------------------------------

int timer_1(double delay);
int timer_2(double delay);

#endif
