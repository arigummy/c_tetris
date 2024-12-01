#include "frontend.h"
//#include <time.h>
#include <locale.h>
#include <ncurses.h>
#include <unistd.h>

#define COLOR_VELVETE 17
#define COLOR_ORANGE 18
#define COLOR_PINK 19

void init_ncurses() {
  setlocale(LC_ALL, "");

  initscr();

  cbreak();
  noecho();

  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(FALSE);

  start_my_colors();
}

void end_ncurses() {
  clear();
  refresh();
  resetty();

  endwin();
}

void start_my_colors() {
  start_color();  // Инициализация цветов
  use_default_colors();

  init_color(COLOR_ORANGE, 1000, 500, 0);
  init_color(COLOR_PINK, 1000, 500, 650);
  init_color(COLOR_VELVETE, 450, 0, 1000);

  init_pair(10, 240, -1);  // Серый
  init_pair(1, COLOR_RED, -1);
  init_pair(2, COLOR_ORANGE, -1);  // Оранжевый
  init_pair(3, COLOR_YELLOW, -1);
  init_pair(4, COLOR_PINK, -1);  // Розовый
  init_pair(5, COLOR_GREEN, -1);
  init_pair(6, COLOR_BLUE, -1);
  init_pair(7, COLOR_MAGENTA, -1);

  init_pair(11, COLOR_WHITE, 240);
  init_pair(12, COLOR_WHITE, COLOR_WHITE);
}

void draw_all_game(GameInfo_t data) {
  draw_console();
  draw_field(data);
  draw_stats(data);

  // refresh();
}

void draw_field(GameInfo_t data) {
  for (int y = 0; y < Y_SIZE; y++) {
    for (int x = 0; x < X_SIZE; x++) {
      if (data.field[y][x] && data.field[y][x] != 13) {
        attron(A_BOLD);
        attron(COLOR_PAIR(data.field[y][x]));
        mvprintw(y + 1, x * 2 + 1, "[]");
        // mvprintw(y+1, x*2+1+1, "]");
        attroff(COLOR_PAIR(data.field[y][x]));
        attroff(A_BOLD);
      } else if (data.field[y][x] == 13) {
        attron(COLOR_PAIR(12));
        mvprintw(y + 1, x * 2 + 1, "  ");
        attroff(COLOR_PAIR(12));
      } else if (!data.field[y][x] || data.field[y][x] == 20) {
        attron(COLOR_PAIR(10));
        mvprintw(y + 1, x * 2 + 1, "<>");
        // mvprintw(y+1, x*2+1+1, ">");
        attroff(COLOR_PAIR(10));
      }
    }
  }
}

void draw_console() {
  // chtype empty_block = ACS_BULLET | A_DIM;

  chtype vertical_line = ACS_VLINE;
  chtype horizontal_line = ACS_HLINE;
  chtype ul_corner = ACS_ULCORNER;
  chtype ur_corner = ACS_URCORNER;
  chtype ll_corner = ACS_LLCORNER;
  chtype lr_corner = ACS_LRCORNER;

  for (int y = 0; y < 25; y += 21) {
    for (int x = 0; x < X_SIZE; x++) {
      mvaddch(y, x * 2 + 1, horizontal_line);
      mvaddch(y, x * 2 + 1 + 1, horizontal_line);
    }
  }

  for (int y = 0; y < Y_SIZE; y++) {
    for (int x = 0; x < 25; x += 21) {
      mvaddch(y + 1, x, vertical_line);
    }
  }

  mvaddch(0, 0, ul_corner);
  mvaddch(0, X_SIZE * 2 + 1, ur_corner);
  mvaddch(Y_SIZE + 1, X_SIZE * 2 + 1, lr_corner);
  mvaddch(Y_SIZE + 1, 0, ll_corner);
}

void draw_stats(GameInfo_t data) {
  mvprintw(2, 30, "Score: %d   ", data.score);
  mvprintw(4, 30, "Max score: %d   ", data.high_score);
  mvprintw(6, 30, "Level: %d    ", data.level);
  draw_figure(data.next, 10, 30);
}

void draw_figure(int **bitmap, int y, int x) {
  for (int y_ = 0; y_ < 4; y_++) {
    for (int x_ = 0; x_ < 4; x_++) {
      if (bitmap[y_][x_]) {
        attron(A_BOLD);
        attron(COLOR_PAIR(bitmap[y_][x_]));
        mvprintw(y + y_, x + x_ * 2, "[]");
        attroff(COLOR_PAIR(bitmap[y_][x_]));
        attroff(A_BOLD);
      } else {
        attron(COLOR_PAIR(10));
        mvprintw(y + y_, x + x_ * 2, "<>");
        attroff(COLOR_PAIR(10));
      }
    }
  }
}

// ---------------------------------------------------

void set_menu_back() {}

void draw_menu(int state) {
  draw_banner();

  mvprintw(10, 16, "  Start  ");

  mvprintw(12, 16, "  Help   ");

  mvprintw(14, 16, "  EXIT   ");

  mvprintw(10 + state * 2, 16, ">");
  mvprintw(10 + state * 2, state == 0 ? 24 : 23, "<");
}

void draw_pause_state() {
  static int state = 0;
  if (timer_1(0.6)) {
    state = !state;
  }
  if (state) {
    attron(COLOR_PAIR(11));
    mvprintw(8, 8, "Pause");
    attroff(COLOR_PAIR(11));
  }
}

void draw_exit_sate() {
  static int state = 0;
  if (timer_1(0.6)) {
    state = !state;
  }
  if (state) {
    attron(COLOR_PAIR(11));
    mvprintw(14, 30, "Press any key to return main menu");
    attroff(COLOR_PAIR(11));
  } else
    mvprintw(14, 30, "                                       ");
}

void draw_banner() {
  FILE *file = fopen("gui/cli/banner.txt", "r");

  if (file) {
    if (file) {
      char str[124];

      for (int i = 0; fgets(str, 124, file); i++) {
        mvprintw(i, 0, "%s", str);
      }

      refresh();
      fclose(file);
    }
  }
}
