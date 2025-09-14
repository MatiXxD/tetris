#include "../include/cli.h"

#include <ncurses.h>

void init_cli() {
  // configure ncurses
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, 1);
  nodelay(stdscr, 1);
  scrollok(stdscr, 1);
  curs_set(0);
  mouseinterval(1);

  // init colors
  start_color();
  init_pair(1, COLOR_RED, COLOR_RED);
  init_pair(2, COLOR_GREEN, COLOR_GREEN);
  init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(4, COLOR_BLUE, COLOR_BLUE);
  init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(6, COLOR_CYAN, COLOR_CYAN);
  init_pair(7, COLOR_WHITE, COLOR_WHITE);
  init_pair(8, COLOR_BLACK, COLOR_BLACK);
}

void finish_cli() { endwin(); }

void render(GameInfo *game_info) {
  refresh();

  // left side
  WINDOW *help_window = get_help_window();
  wrefresh(help_window);

  // middle part
  WINDOW *game_window = NULL;
  if (game_info->status != StatePause) {
    game_window = get_field_window(game_info);
    wrefresh(game_window);
  }

  if (game_info->status == StateInitial) {
    print_start_screen();
  } else if (game_info->status == StatePause) {
    print_pause_screen();
  } else if (game_info->status == StateGameOver) {
    print_game_over_screen();
  }

  // // right side
  WINDOW *game_info_window = get_game_info_window(game_info);
  wrefresh(game_info_window);

  // clear
  delwin(help_window);
  delwin(game_info_window);

  if (game_window != NULL) {
    delwin(game_window);
  }
}

WINDOW *get_help_window() {
  WINDOW *win = newwin(HELP_SCREEN_Y_SIZE, HELP_SCREEN_X_SIZE,
                       HELP_SCREEN_Y_OFFSET, HELP_SCREEN_X_OFFSET);
  box(win, 0, 0);

  mvwprintw(win, 0, (HELP_SCREEN_X_SIZE - 4) / 2, "HELP");
  mvwprintw(win, 2, 2, "Start: s");
  mvwprintw(win, 4, 2, "Pause: p");
  mvwprintw(win, 6, 2, "Replay: r");
  mvwprintw(win, 8, 2, "Move left: left arrow");
  mvwprintw(win, 10, 2, "Move right: right arrow");
  mvwprintw(win, 12, 2, "Rotate: up arrow");
  mvwprintw(win, 14, 2, "Move down: down arrow");
  mvwprintw(win, 16, 2, "Quit game: q");

  return win;
}

WINDOW *get_field_window(GameInfo *game_info) {
  WINDOW *win = newwin(GAME_SCREEN_Y_SIZE, GAME_SCREEN_X_SIZE,
                       GAME_SCREEN_Y_OFFSET, GAME_SCREEN_X_OFFSET);
  box(win, 0, 0);

  keypad(win, 1);
  nodelay(win, 1);

  mvwprintw(win, 0, (GAME_SCREEN_X_SIZE - 6) / 2, "TETRIS");

  GameFieldPtr field = game_info->field;
  for (int i = 0; i < GAME_FIELD_Y_SIZE; i++) {
    for (int j = 0; j < GAME_FIELD_X_SIZE; j++) {
      int color = field[i][j];
      if (color == 0) {
        color = 8;
      }

      wattron(win, COLOR_PAIR(color));
      mvwprintw(win, i + 1, BLOCK_SIZE * j + 1, "  ");
      wattroff(win, COLOR_PAIR(color));
    }
  }

  return win;
}

WINDOW *get_game_info_window(GameInfo *game_info) {
  WINDOW *win = newwin(GAME_INFO_SCREEN_Y_SIZE, GAME_INFO_SCREEN_X_SIZE,
                       GAME_INFO_SCREEN_Y_OFFSET, GAME_INFO_SCREEN_X_OFFSET);
  box(win, 0, 0);

  mvwprintw(win, 0, (GAME_INFO_SCREEN_X_SIZE - 4) / 2, "INFO");
  mvwprintw(win, 3, 2, "Max score %d", game_info->max_score);
  mvwprintw(win, 6, 2, "Score %d", game_info->score);
  mvwprintw(win, 9, 2, "Level %d", game_info->lvl);

  print_next_figure_on_win(win, game_info);

  return win;
}

void print_next_figure_on_win(WINDOW *win, GameInfo *game_info) {
  int next_fig[NEXT_FIG_BLOCK_SIZE][NEXT_FIG_BLOCK_SIZE];
  for (int i = 0; i < NEXT_FIG_BLOCK_SIZE; i++) {
    for (int j = 0; j < NEXT_FIG_BLOCK_SIZE; j++) {
      next_fig[i][j] = 0;
    }
  }

  int xc = 1, yc = 1;
  for (int i = 0; i < FIG_COORD_Y; i++) {
    int x = xc + game_info->next_fig[i][0];
    int y = yc + game_info->next_fig[i][1];

    next_fig[x][y] = 1;
  }

  mvwprintw(win, 12, 2, "Next figure:");
  for (int i = 0; i < NEXT_FIG_BLOCK_SIZE; i++) {
    for (int j = 0; j < NEXT_FIG_BLOCK_SIZE; j++) {
      if (next_fig[i][j]) {
        wattron(win, COLOR_PAIR(7));
        mvwprintw(win, i + 14, j * BLOCK_SIZE + 4, "  ");
        wattroff(win, COLOR_PAIR(7));
      }
    }
  }
}

void print_start_screen() {
  WINDOW *win = newwin(START_SCREEN_Y_SIZE, START_SCREEN_X_SIZE,
                       START_SCREEN_Y_OFFSET, START_SCREEN_X_OFFSET);
  box(win, 0, 0);

  mvwprintw(win, 1, (START_SCREEN_X_SIZE - 6) / 2, "TETRIS");
  mvwprintw(win, 3, (START_SCREEN_X_SIZE - 12) / 2, "press 's' to");
  mvwprintw(win, 4, (START_SCREEN_X_SIZE - 5) / 2, "start");

  wrefresh(win);
  delwin(win);
}

void print_pause_screen() {
  WINDOW *win = newwin(PAUSE_SCREEN_Y_SIZE, PAUSE_SCREEN_X_SIZE,
                       PAUSE_SCREEN_Y_OFFSET, PAUSE_SCREEN_X_OFFSET);
  box(win, 0, 0);

  mvwprintw(win, 1, (PAUSE_SCREEN_X_SIZE - 6) / 2, "PAUSED");
  mvwprintw(win, 3, (PAUSE_SCREEN_X_SIZE - 12) / 2, "press 'p' to");
  mvwprintw(win, 4, (PAUSE_SCREEN_X_SIZE - 8) / 2, "continue");

  wrefresh(win);
  delwin(win);
}

void print_game_over_screen() {
  WINDOW *win = newwin(GAME_OVER_SCREEN_Y_SIZE, GAME_OVER_SCREEN_X_SIZE,
                       GAME_OVER_SCREEN_Y_OFFSET, GAME_OVER_SCREEN_X_OFFSET);
  box(win, 0, 0);

  mvwprintw(win, 1, (PAUSE_SCREEN_X_SIZE - 10) / 2, "GAME OVER");
  mvwprintw(win, 3, (PAUSE_SCREEN_X_SIZE - 12) / 2, "press 'r' to");
  mvwprintw(win, 4, (PAUSE_SCREEN_X_SIZE - 8) / 2, "restart");
  mvwprintw(win, 6, (PAUSE_SCREEN_X_SIZE - 12) / 2, "press 'q' to");
  mvwprintw(win, 7, (PAUSE_SCREEN_X_SIZE - 4) / 2, "quit");

  wrefresh(win);
  delwin(win);
}
