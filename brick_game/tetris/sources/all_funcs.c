#include "../includes/all_funcs.h"

#include <stdio.h>

// -----------------------------------------------------------------------------------

void init_field(int ***dest) {
  int **res;
  res = (int **)malloc(Y_SIZE * X_SIZE * sizeof(int) + Y_SIZE * sizeof(int *));

  int *ptr = (int *)(res + Y_SIZE);

  for (int i = 0; i < Y_SIZE; i++) {
    res[i] = (int *)(ptr + (X_SIZE * i));
  }

  for (int i = 0; i < Y_SIZE; i++) {
    for (int j = 0; j < X_SIZE; j++) {
      res[i][j] = 0;
    }
  }

  *dest = res;
}

// -----------------------------------------------------------------------------------

void init_figure(Figure_t *figure) {
  figure->bitmap = (int **)malloc(4 * 4 * sizeof(int) + 4 * sizeof(int *));
  int *ptr = (int *)(figure->bitmap + 4);

  for (int i = 0; i < 4; i++) {
    figure->bitmap[i] = ptr + (4 * i);
  }
}

int figures[7][4][4] = {
    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};

void generate_figure(Figure_t *figure) {
  int type = rand() % 7;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      figure->bitmap[i][j] = figures[type][i][j];
    }
  }

  figure->type = type;
  figure->x = 3;
  figure->y = 0;
}

void add_figure_on_ground(game_data_t *data) {
  int x = data->figure.x;
  int y = data->figure.y;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (data->figure.bitmap[i][j]) {
        data->ground[y + i][x + j] = data->figure.type + 1;
      }
    }
  }
}

// -----------------------------------------------------------------------------------

int vertical_collision(const game_data_t data) {
  int x = data.figure.x;
  int y = data.figure.y;

  for (int i = 3; i >= 0; i--) {
    for (int j = 0; j < 4; j++) {
      if (data.figure.bitmap[i][j]) {
        if ((x + j + 1) >= X_SIZE) return 1;
        if ((x + j - 1) < 0)
          return -1;
        else if (inside_area(y + i, x + j + 1)) {
          if (data.ground[y + i][x + j + 1]) return 1;
          if (data.ground[y + i][x + j - 1]) return -1;
        }
      }
    }
  }

  return 0;
}

int horizontal_collision(const game_data_t data) {
  int x = data.figure.x;
  int y = data.figure.y;

  for (int i = 3; i >= 0; i--) {
    for (int j = 0; j < 4; j++) {
      if (data.figure.bitmap[i][j]) {
        if ((y + i + 1) >= Y_SIZE)
          return 1;
        else if (inside_area(y + i + 1, x + j)) {
          if (data.ground[y + i + 1][x + j]) return 1;
        }
      }
    }
  }

  return 0;
}

int all_collision(const game_data_t data) {
  int x = data.figure.x;
  int y = data.figure.y;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (data.figure.bitmap[i][j]) {
        if (!inside_area(y + i, x + j)) {
          return 1;
        }
        if (data.ground[y + i][x + j]) return 1;
      }
    }
  }

  return 0;
}

int inside_area(int y, int x) {
  int res = 1;

  if (y >= Y_SIZE || y < 0) res = 0;
  if (x >= X_SIZE || x < 0) res = 0;

  return res;
}

// -----------------------------------------------------------------------------------

int get_max_score() {
  int score = 0;

  FILE *file = fopen("max_score.txt", "r");

  if (file) {
    fscanf(file, "%d", &score);
    fclose(file);
  }

  return score;
}

void save_max_score(game_data_t *data) {
  if (data->info.score > data->info.high_score) {
    FILE *file = fopen("max_score.txt", "w");

    fprintf(file, "%d", data->info.score);

    fclose(file);
  }
}

static clock_t lst_1;
static clock_t lst_2;

int timer_1(double delay) {
  if ((double)(clock() - lst_1) / CLOCKS_PER_SEC >= delay) {
    lst_1 = clock();
    return 1;
  }
  return 0;
}

int timer_2(double delay) {
  if ((double)(clock() - lst_2) / CLOCKS_PER_SEC >= delay) {
    lst_2 = clock();
    return 1;
  }
  return 0;
}
