#include <ncurses.h>

#include "../../brick_game/tetris/includes/s21_tetris_lib.h"
#include "frontend.h"

#define ESCAPE 27
#define SPACE 32
#define ENTER 10

#include <unistd.h>

int game_loop();
int game_menu();
int finish(GameInfo_t data);

UserAction_t get_input();

// ----------------------------------------

int main() {
  init_ncurses();
  init_game();

  for (;;) {
    if (game_menu()) {
      int res = game_loop();
      if (res)
        break;
      else {
        end_game();
        init_game();
      }
    } else
      break;
  }

  end_game();

  end_ncurses();

  return 0;
}

// ----------------------------------------

int game_loop() {
  GameInfo_t data;
  int level = 0;

  for (; data.level;) {
    userInput(get_input(), 0);
    data = updateCurrentState();
    draw_all_game(data);
    if (data.pause) draw_pause_state();
    if (data.level != 0) level = data.level;
  }
  data.level = level;
  return finish(data);
}

UserAction_t get_input() {
  switch (getch()) {
    case ENTER:
      return Start;
    case 'P':
    case 'p':
      return Pause;
    case ESCAPE:
      return Terminate;
    case KEY_LEFT:
      return Left;
    case KEY_RIGHT:
      return Right;
    case KEY_UP:
      return Up;
    case KEY_DOWN:
      return Down;
    case SPACE:
      return Action;
    default:
      return Nosig;
  }
}

// ------------------------------------------------------------

typedef enum { m_start = 0, m_help, m_exit } menu_state;

int game_menu() {
  clear();

  menu_state state = m_start;
  int res = 1;

  for (;;) {
    UserAction_t input = get_input();
    if (input == Down)
      state += state < 2 ? 1 : 0;
    else if (input == Up)
      state -= state > 0 ? 1 : 0;

    draw_menu(state);

    if (input == Start && state == m_start) break;
    if (input == Start && state == m_exit) {
      res = 0;
      break;
    }
  }
  clear();

  return res;
}

int finish(GameInfo_t data) {
  clear();
  draw_console();
  draw_field(data);
  mvprintw(2, 30, "Score: %d   ", data.score);
  mvprintw(4, 30, "Max score: %d   ", data.high_score);
  mvprintw(6, 30, "Level: %d    ", data.level);
  if (data.high_score < data.score) mvprintw(10, 35, "New record!!!");

  for (;;) {
    switch (get_input()) {
      case Terminate:
        return 1;
      case Nosig:
        break;
      default:
        return 0;
    }
    draw_exit_sate();
  }
}
