#include <stdlib.h>
#include <time.h>

#include "../includes/fsm.h"
#include "../includes/s21_tetris_lib.h"

static game_data_t game_data;

GameInfo_t updateCurrentState() {
  game_action(&game_data);
  compile_field();
  game_data.info.next = game_data.next_figure.bitmap;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game_data.next_figure.bitmap[i][j]) {
        game_data.info.next[i][j] = game_data.next_figure.type + 1;
      }
    }
  }
  game_data.info.speed = (int)(game_data.time_data.delay * 1000);

  return game_data.info;
}

//-----------------------------------------------

game_data_t get_all_data() { return game_data; }

//-----------------------------------------------

void userInput(UserAction_t action, bool hold) {
  if (!hold) {
    switch (action) {
      case Action:
        game_data.sig = SPACE_BTN;
        break;
      case Left:
        game_data.sig = LEFT_BTN;
        break;
      case Right:
        game_data.sig = RIGHT_BTN;
        break;
      case Up:
        game_data.sig = UP_BTN;
        break;
      case Terminate:
        game_data.sig = ESCAPE_BTN;
        break;
      case Pause:
        game_data.sig = PAUSE_BTN;
        break;
      default:
        game_data.sig = NOSIG;
        break;
    }
  }
}

void init_game() {
  srand(time(NULL));

  init_field(&game_data.ground);
  init_field(&game_data.info.field);

  init_figure(&game_data.figure);
  init_figure(&game_data.next_figure);
  generate_figure(&game_data.figure);
  generate_figure(&game_data.next_figure);

  game_data.time_data.last_shift = clock();
  game_data.time_data.delay = (double)DELAY_TIME / 1000;

  game_data.sig = NOSIG;
  game_data.state = SPAWN;

  game_data.info.level = 1;
  game_data.info.score = 0;
  game_data.info.high_score = get_max_score();
}

void end_game() {
  if (game_data.ground) free(game_data.ground);
  if (game_data.ground) free(game_data.info.field);

  if (game_data.figure.bitmap) free(game_data.figure.bitmap);
  if (game_data.next_figure.bitmap) free(game_data.next_figure.bitmap);
}

void compile_field() {
  for (int y = 0; y < Y_SIZE; y++) {
    for (int x = 0; x < X_SIZE; x++) {
      game_data.info.field[y][x] = game_data.ground[y][x];
    }
  }

  int x = game_data.figure.x;
  int y = game_data.figure.y;

  if (game_data.figure.bitmap != NULL) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (game_data.figure.bitmap[i][j]) {
          game_data.info.field[y + i][x + j] = game_data.figure.type + 1;
        }
      }
    }
  }
}

void tick() {
  if (game_data.state == MOVING) {
    if ((double)(clock() - game_data.time_data.last_shift) / CLOCKS_PER_SEC >=
        game_data.time_data.delay) {
      game_data.time_data.last_shift = clock();
      game_data.state = SHIFTING;
    }
  }
}

void set_sig(Signal_t sig) { game_data.sig = sig; }

void set_state(GameState_t state) { game_data.state = state; }

void set_pattern_1() {
  for (int x = 0; x < X_SIZE; x++) {
    game_data.ground[19][x] = 3;
  }
  game_data.ground[18][2] = 3;
  game_data.ground[18][5] = 3;

  for (int x = 0; x < X_SIZE; x++) {
    game_data.ground[17][x] = 3;
  }
}
