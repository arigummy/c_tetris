#include "../includes/fsm.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "../includes/all_funcs.h"
#include "../includes/s21_tetris_lib.h"

// ------------------ fsm -------------------

void spawn(game_data_t* data);
void falling(game_data_t* data);
void moveright(game_data_t* data);
void moveleft(game_data_t* data);
void rotate(game_data_t* data);
void shifting(game_data_t* data);
void attaching(game_data_t* data);
void gameover(game_data_t* data);
void timeout(game_data_t* data);

// --------------- support -------------------

void rotate_figure(Figure_t* figure, int size);
int find_filled_lines(game_data_t* data);
int delete_filled_line(game_data_t* data);
void fill_line(game_data_t* data, int start_y, int value);
void mudslide(game_data_t* data, int y);
int get_score(int lines);
int get_level(int score);
void set_speed(game_data_t* data);

typedef void (*action)(game_data_t* data);

action fsm_table[6][7] = {
    {spawn, spawn, spawn, spawn, spawn, spawn, spawn},
    {rotate, moveright, moveleft, falling, gameover, timeout, NULL},
    {shifting, shifting, shifting, shifting, shifting, shifting, shifting},
    {attaching, attaching, attaching, attaching, attaching, attaching,
     attaching},
    {gameover, gameover, gameover, gameover, gameover, gameover, gameover},
    {timeout, timeout, timeout, timeout, gameover, timeout, timeout},
};

void game_action(game_data_t* data) {
  action act = fsm_table[data->state][data->sig];

  if (act) act(data);
  if (data->state == MOVING) tick();
}

void spawn(game_data_t* data) {
  data->figure.x = data->next_figure.x;
  data->figure.y = data->next_figure.y;
  data->figure.type = data->next_figure.type;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      data->figure.bitmap[i][j] = data->next_figure.bitmap[i][j];
    }
  }
  generate_figure(&data->next_figure);

  data->state = MOVING;
}
void falling(game_data_t* data) {
  while (!horizontal_collision(*data)) {
    data->figure.y++;
  }

  data->state = ATTACHING;
}
void moveright(game_data_t* data) {
  if (vertical_collision(*data) != 1) data->figure.x++;
}
void moveleft(game_data_t* data) {
  if (vertical_collision(*data) != -1) data->figure.x--;
}
void rotate(game_data_t* data) {
  Figure_t old_figure;
  init_figure(&old_figure);
  old_figure.x = data->figure.x;
  old_figure.y = data->figure.y;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      old_figure.bitmap[i][j] = data->figure.bitmap[i][j];
    }
  }

  int size = 3;
  if (data->figure.type == 0 || data->figure.type == 3) size = 4;

  rotate_figure(&data->figure, size);

  if (vertical_collision(*data) == -1) {
    while (all_collision(*data) == 1) {
      data->figure.x++;
    }
  }

  if (vertical_collision(*data) == 1) {
    while (all_collision(*data) == 1) {
      data->figure.x--;
    }
  }

  if (all_collision(*data)) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        data->figure.bitmap[i][j] = old_figure.bitmap[i][j];
      }
    }
    data->figure.x = old_figure.x;
  }

  free(old_figure.bitmap);
}
void rotate_figure(Figure_t* figure, int size) {
  int buf[4][4] = {0};

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      buf[i][j] = figure->bitmap[j][size - i - 1];
    }
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      figure->bitmap[i][j] = 0;
    }
  }

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      figure->bitmap[i][j] = buf[i][j];
    }
  }
}

void shifting(game_data_t* data) {
  if (horizontal_collision(*data))
    data->state = ATTACHING;
  else {
    data->figure.y++;
    data->state = MOVING;
  }
}

void attaching(game_data_t* data) {
  add_figure_on_ground(data);

  int lines = 0;

  if (find_filled_lines(data)) {
    lines = delete_filled_line(data);
  }

  data->info.score += get_score(lines);
  data->info.level = get_level(data->info.score);
  set_speed(data);

  data->state = SPAWN;

  for (int i = 0; i < X_SIZE; i++) {
    if (data->ground[0][i]) data->state = GAMEOVER;
  }
}

int find_filled_lines(game_data_t* data) {
  for (int y = Y_SIZE - 1; y > 0; y--) {
    int pixels = 0;
    for (int x = 0; x < X_SIZE; x++) {
      if (data->ground[y][x]) pixels++;
    }
    if (pixels == 10) return y;
  }

  return 0;
}

int delete_filled_line(game_data_t* data) {
  int lines = 0;
  for (int i = 0, y = find_filled_lines(data); i < 4 || y > 0; i++, y--) {
    int pixels = 0;
    for (int x = 0; x < X_SIZE; x++) {
      if (data->ground[y][x]) pixels++;
    }
    if (pixels == 10) {
      fill_line(data, y, 0);
      mudslide(data, y);
      lines++;
      y++;
    }
  }

  return lines;
}

void fill_line(game_data_t* data, int y, int value) {
  if (y >= 0 && y < Y_SIZE) {
    for (int x = 0; x < X_SIZE; x++) {
      data->ground[y][x] = value;
    }
  }
}

void mudslide(game_data_t* data, int start_y) {
  for (int y = start_y; y > 0; y--) {
    for (int x = 0; x < X_SIZE; x++) {
      data->ground[y][x] = data->ground[y - 1][x];
    }
  }
}

int get_score(int lines) {
  switch (lines) {
    case 1:
      return 100;
    case 2:
      return 300;
    case 3:
      return 700;
    case 4:
      return 1500;
    default:
      return 0;
  }
}

int get_level(int score) {
  if (score < 600)
    return 1;
  else if (score < 1200)
    return 2;
  else if (score < 1800)
    return 3;
  else if (score < 2400)
    return 4;
  else if (score < 3000)
    return 5;
  else if (score < 3600)
    return 6;
  else if (score < 4200)
    return 7;
  else if (score < 4800)
    return 8;
  else if (score < 5400)
    return 9;
  else if (score < 6000)
    return 10;

  return 1;
}

void set_speed(game_data_t* data) {
  data->time_data.delay =
      (double)((float)DELAY_TIME - ((float)data->info.level * 80.0)) / 1000.0;
}

void gameover(game_data_t* data) {
  save_max_score(data);
  data->info.level = 0;
}

void timeout(game_data_t* data) {
  if (data->state == MOVING) {
    data->info.pause = 1;
    data->time_data.last_shift = clock() - data->time_data.last_shift;
    data->state = PAUSE;
  } else if (data->sig != NOSIG) {
    data->info.pause = 0;
    data->time_data.last_shift = clock() + (1000 - data->time_data.last_shift);
    data->state = MOVING;
  }
}
