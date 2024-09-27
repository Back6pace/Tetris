#include "frontend.h"

int start_game() {
  initialize_interface();
  Tetris game = {0};

  int flag = initialGame(&game);
  if (flag == ERROR) {
    return 0;
  }
  double cnt = 1;
  while (game.gameInfo.pause == NOT_START) {
    mvprintw(9, 6, "GAME READY");
    mvprintw(11, 5, "Press  ENTER");
    mvprintw(13, 7, "to Start");
    catch_input(&game);
    draw_interface(&game);
  }

  while (game.gameInfo.pause == PAUSE) {
    catch_input(&game);
    draw_interface(&game);
  }

  while (game.gameInfo.pause != END) {
    catch_input(&game);
    if (game.gameInfo.pause == STARTED) {
      if (cnt >= (1.3 - game.speed * SPEED_RATE)) {
        updateCurrentState(&game);
        cnt = 0.0;
      }
      cnt += READ_DELAY * 0.001;
    }
    draw_interface(&game);
  }

  while (game.gameInfo.pause == END) {
    catch_input(&game);
    draw_interface(&game);
  }
  clearField(&game);
  endwin();
  return flag;
}

void catch_input(Tetris* game) {
  int button = getch();
  switch (button) {
    case 10:
      userInput(Start, 0, game);
      break;
    case 'p':
      userInput(Pause, 0, game);
      break;
    case 'q':
      userInput(Terminate, 0, game);
      break;
    case KEY_LEFT:
      userInput(Left, 0, game);
      break;
    case KEY_RIGHT:
      userInput(Right, 0, game);
      break;
    case KEY_UP:
      userInput(Action, 0, game);
      break;
    case KEY_DOWN:
      userInput(Down, 0, game);
      break;
    default:
      break;
  }
}

void initialize_interface() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  timeout(20);
}

void init_colors() {
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  init_pair(2, COLOR_WHITE, COLOR_BLACK);

  bkgd(COLOR_PAIR(1));
}

void draw_interface(Tetris* game) {
  clear();
  print_rectangle(0, 21, 0, 21);
  print_rectangle(0, 21, 21, 38);
  initialGamebar();
  draw_next_figure(game);

  mvprintw(10, 31, "%d", game->gameInfo.high_score);
  mvprintw(13, 31, "%d", game->gameInfo.score);
  mvprintw(16, 31, "%d", game->gameInfo.level);

  if (game->gameInfo.pause != PAUSE && game->gameInfo.pause != END) {
    game_field(game);
  }

  if (game->gameInfo.pause == PAUSE) {
    mvprintw(2, 24, "GAME PAUSED");
    mvprintw(9, 6, "Press P Key");
    mvprintw(11, 6, "to Continue");
  }

  if (game->gameInfo.pause == END) {
    mvprintw(2, 25, "GAME OVER");
    mvprintw(9, 3, "Press ENTER Key");
    mvprintw(11, 7, "to Exit");
  }
  if (game->gameInfo.pause == STARTED) {
    draw_figure(game);
  }
}

void game_field(Tetris* game) {
  for (int i = 0; i < GAME_FIELD_HEIGHT; i++) {
    for (int j = 0; j < GAME_FIELD_WIDTH; j++) {
      if (game->gameInfo.field[i][j] != 0) {
        attron(COLOR_PAIR(2));
        mvaddch(i + 1, j * 2 + 1, ACS_CKBOARD);
        mvaddch(i + 1, j * 2 + 2, ACS_CKBOARD);
        attroff(COLOR_PAIR(2));
      }
    }
  }
}

void draw_figure(Tetris* game) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      // Проверяем, если элемент фигуры активен и находится в пределах экрана
      if (game->figure.shape[i][j] == 1) {
        int x_pos = (game->figure.x + j) * 2 + 1;
        int y_pos = game->figure.y + i + 1;

        if (y_pos >= 1 && y_pos < GAME_FIELD_HEIGHT + 1 && x_pos >= 1 &&
            x_pos < GAME_FIELD_WIDTH * 2 + 1) {
          attron(COLOR_PAIR(2));
          mvaddch(y_pos, x_pos, ACS_CKBOARD);
          mvaddch(y_pos, x_pos + 1, ACS_CKBOARD);
          attroff(COLOR_PAIR(2));
        }
      }
    }
  }
}

void draw_next_figure(Tetris* game) {
  int m = 5;   // Стартовая позиция по вертикали
  int n = 30;  // Стартовая позиция по горизонтали

  // Рисуем следующую фигуру
  for (int i = 2; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->gameInfo.next[i][j] == 1) {
        attron(COLOR_PAIR(2));
        mvaddch(m, n, ACS_CKBOARD);
        mvaddch(m, n + 1, ACS_CKBOARD);
        attroff(COLOR_PAIR(2));
      }
      n += 2;
    }
    n = 30;
    m++;
  }
}

void initialGamebar() {
  // Tetris Label
  print_rectangle(1, 3, 22, 37);
  mvprintw(2, 26, "Tetris");

  // Next
  print_rectangle(5, 7, 22, 28);
  mvprintw(6, 23, "Next");

  // High Score
  print_rectangle(8, 11, 22, 28);
  mvprintw(9, 23, "HIGH");
  mvprintw(10, 23, "Score");
  print_rectangle(8, 11, 29, 37);

  // Score
  print_rectangle(12, 14, 22, 28);
  mvprintw(13, 23, "Score");
  print_rectangle(12, 14, 29, 37);

  // Level
  print_rectangle(15, 17, 22, 28);
  mvprintw(16, 23, "Level");
  print_rectangle(15, 17, 29, 37);

  print_rectangle(18, 20, 24, 36);
  mvprintw(19, 26, "noellacl");
}

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  mvaddch(top_y, left_x, ACS_ULCORNER);

  for (int i = left_x + 1; i < right_x; i++) {
    mvaddch(top_y, i, ACS_HLINE);
  }

  mvaddch(top_y, right_x, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    mvaddch(i, left_x, ACS_VLINE);

    mvaddch(i, right_x, ACS_VLINE);
  }

  mvaddch(bottom_y, left_x, ACS_LLCORNER);
  for (int i = left_x + 1; i < right_x; i++) {
    mvaddch(bottom_y, i, ACS_HLINE);
  }
  mvaddch(bottom_y, right_x, ACS_LRCORNER);
}

void clearField(Tetris* game) {
  freeSpace(game);
  printf("\033[H\033[J");
}