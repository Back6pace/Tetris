#include <check.h>

#include "../brick_game/tetris/backend.h"

START_TEST(initial1_check_stats) {
  Tetris game;
  initialGame(&game);
  ck_assert_int_eq(game.gameInfo.level, 1);
  ck_assert_int_eq(game.gameInfo.score, 0);
  ck_assert_int_ne(game.figure.x, 0);

  freeSpace(&game);
}
END_TEST

START_TEST(initial2_check_fields) {
  Tetris game;
  initialGame(&game);
  ck_assert_ptr_nonnull(game.gameInfo.field);
  for (int i = 0; i < GAME_FIELD_HEIGHT; i++) {
    for (int j = 0; j < GAME_FIELD_WIDTH; j++) {
      ck_assert_int_eq(game.gameInfo.field[i][j], 0);
    }
  }
  ck_assert_ptr_nonnull(game.gameInfo.next);
  ck_assert_ptr_nonnull(game.figure.shape);
  freeSpace(&game);
}
END_TEST

Suite *test_create(void) {
  Suite *s;
  s = suite_create("s21_initial_game");
  TCase *tcase_initial = tcase_create("CREATE");
  tcase_add_test(tcase_initial, initial1_check_stats);
  tcase_add_test(tcase_initial, initial2_check_fields);
  suite_add_tcase(s, tcase_initial);
  return s;
}

START_TEST(read_high_score1) {
  Tetris game;
  remove("score.txt");
  readhighScore(&game);
  ck_assert_int_eq(game.gameInfo.high_score, 0);
}
END_TEST

START_TEST(write_high_score1) {
  Tetris game;
  initialGame(&game);

  remove("score.txt");
  writeHighScore(&game);
  int score = 0;

  FILE *f = fopen("score.txt", "r");
  if (f != NULL) {
    char line[255];
    fgets(line, 255, f);
    int s;
    sscanf(line, "%d", &s);
    score = s;
    fclose(f);
  } else {
    // Handle the case where the file could not be opened
    printf("Error: File not found. Using default score.\n");
  }
  ck_assert_int_eq(score, 0);
  freeSpace(&game);
}
END_TEST

START_TEST(write_high_score2) {
  Tetris game;
  initialGame(&game);

  game.gameInfo.score = 300;
  writeHighScore(&game);
  int score = 0;
  FILE *f = fopen("score.txt", "r");
  if (f != NULL) {
    char line[255];
    fgets(line, 255, f);
    int s;
    sscanf(line, "%d", &s);
    score = s;
    fclose(f);
  } else {
    // Handle the case where the file could not be opened
    printf("Error: File not found. Using default score.\n");
  }
  ck_assert_int_eq(score, 300);
  freeSpace(&game);
}
END_TEST

START_TEST(read_high_score2) {
  Tetris game;
  initialGame(&game);

  readhighScore(&game);
  ck_assert_int_eq(game.gameInfo.high_score, 300);
  freeSpace(&game);
}
END_TEST

Suite *test_high_score(void) {
  Suite *s;
  s = suite_create("s21_read_write_high_score");
  TCase *tcase_high_score = tcase_create("HIGH_SCORE");
  tcase_add_test(tcase_high_score, read_high_score1);
  tcase_add_test(tcase_high_score, write_high_score1);
  tcase_add_test(tcase_high_score, write_high_score2);
  tcase_add_test(tcase_high_score, read_high_score2);

  suite_add_tcase(s, tcase_high_score);
  return s;
}

START_TEST(game_pause1) {
  Tetris game;
  initialGame(&game);

  game.gameInfo.pause = STARTED;
  game_pause(&game);
  ck_assert_int_eq(game.gameInfo.pause, PAUSE);
  freeSpace(&game);
}
END_TEST

START_TEST(game_pause2) {
  Tetris game;
  initialGame(&game);

  game.gameInfo.pause = PAUSE;
  game_pause(&game);
  ck_assert_int_eq(game.gameInfo.pause, STARTED);
  freeSpace(&game);
}
END_TEST

START_TEST(game_terminate1) {
  Tetris game;
  initialGame(&game);

  game.gameInfo.pause = STARTED;
  game_terminate(&game);
  ck_assert_int_eq(game.gameInfo.pause, END);
  freeSpace(&game);
}
END_TEST

START_TEST(game_terminate2) {
  Tetris game;
  initialGame(&game);

  game_terminate(&game);
  ck_assert_int_eq(game.gameInfo.pause, END);
  freeSpace(&game);
}
END_TEST

START_TEST(game_check_lose1) {
  Tetris game;
  initialGame(&game);

  game.gameInfo.pause = STARTED;
  ck_assert_int_eq(checkLose(&game), 0);
  freeSpace(&game);
}
END_TEST

START_TEST(game_check_lose2) {
  Tetris game;
  initialGame(&game);
  game.gameInfo.pause = STARTED;
  game.gameInfo.field[0][1] = 2;
  ck_assert_int_eq(checkLose(&game), 1);

  freeSpace(&game);
}
END_TEST

Suite *test_game_pause_status(void) {
  Suite *s;
  s = suite_create("s21_game_pause_status");
  TCase *tcase_pause_status = tcase_create("PAUSE_STATUS");
  tcase_add_test(tcase_pause_status, game_pause2);
  tcase_add_test(tcase_pause_status, game_pause1);
  tcase_add_test(tcase_pause_status, game_terminate1);
  tcase_add_test(tcase_pause_status, game_terminate2);
  tcase_add_test(tcase_pause_status, game_check_lose1);
  tcase_add_test(tcase_pause_status, game_check_lose2);

  suite_add_tcase(s, tcase_pause_status);
  return s;
}

START_TEST(user_input_start) {
  Tetris game;
  initialGame(&game);
  userInput(Start, 0, &game);
  ck_assert_int_eq(game.gameInfo.pause, STARTED);
  freeSpace(&game);
}
END_TEST

START_TEST(user_input_pause1) {
  Tetris game;
  initialGame(&game);
  userInput(Pause, 0, &game);
  ck_assert_int_eq(game.gameInfo.pause, NOT_START);
  freeSpace(&game);
}
END_TEST

START_TEST(user_input_pause2) {
  Tetris game;
  initialGame(&game);
  userInput(Start, 0, &game);
  userInput(Pause, 0, &game);
  ck_assert_int_eq(game.gameInfo.pause, PAUSE);
  freeSpace(&game);
}
END_TEST

START_TEST(user_input_pause3) {
  Tetris game;
  initialGame(&game);
  userInput(Start, 0, &game);
  userInput(Pause, 0, &game);
  userInput(Pause, 0, &game);

  ck_assert_int_eq(game.gameInfo.pause, STARTED);
  freeSpace(&game);
}
END_TEST

START_TEST(user_input_terminate) {
  Tetris game;
  initialGame(&game);
  userInput(Terminate, 0, &game);
  ck_assert_int_eq(game.gameInfo.pause, END);
  freeSpace(&game);
}
END_TEST

START_TEST(user_input_left2) {
  Tetris game;
  initialGame(&game);
  game.figure.currentTetramino = 0;
  cpyFigureOnField(&game.figure.shape, 0);
  for (int i = 1; i < GAME_FIELD_HEIGHT; i++) {
    for (int j = 0; j < 3; j++) {
      game.gameInfo.field[i][j] = 1;
    }
  }
  userInput(Start, 0, &game);
  game.figure.y = 3;
  int x0 = game.figure.x;
  userInput(Left, 0, &game);

  ck_assert_int_eq(game.figure.x, x0);
  freeSpace(&game);
}
END_TEST

START_TEST(user_input_left1) {
  Tetris game;
  initialGame(&game);
  userInput(Start, 0, &game);
  int x0 = game.figure.x;
  userInput(Left, 0, &game);

  ck_assert_int_eq(game.figure.x, x0 - 1);
  freeSpace(&game);
}
END_TEST

START_TEST(user_input_right1) {
  Tetris game;
  initialGame(&game);
  userInput(Start, 0, &game);
  int x0 = game.figure.x;
  userInput(Right, 0, &game);

  ck_assert_int_eq(game.figure.x, x0 + 1);
  freeSpace(&game);
}
END_TEST

START_TEST(user_input_right2) {
  Tetris game;
  initialGame(&game);
  game.figure.currentTetramino = 0;
  cpyFigureOnField(&game.figure.shape, 0);
  for (int i = 1; i < GAME_FIELD_HEIGHT; i++) {
    for (int j = 7; j < GAME_FIELD_WIDTH; j++) {
      game.gameInfo.field[i][j] = 1;
    }
  }
  userInput(Start, 0, &game);
  game.figure.y = 3;
  int x0 = game.figure.x;
  userInput(Right, 0, &game);

  ck_assert_int_eq(game.figure.x, x0);
  freeSpace(&game);
}
END_TEST

START_TEST(user_input_down) {
  Tetris game;
  initialGame(&game);
  userInput(Start, 0, &game);
  int y0 = game.figure.y;
  userInput(Down, 0, &game);

  ck_assert_int_eq(game.figure.y, y0 + 1);
  freeSpace(&game);
}
END_TEST

START_TEST(user_input_rotate) {
  Tetris game;
  initialGame(&game);
  userInput(Start, 0, &game);
  int index = game.figure.currentTetramino;
  userInput(Action, 0, &game);

  ck_assert_int_eq(index + 7, game.figure.currentTetramino);
  freeSpace(&game);
}
END_TEST

START_TEST(user_input_rotate2) {
  Tetris game;
  initialGame(&game);
  userInput(Start, 0, &game);

  game.figure.currentTetramino = 7;
  cpyFigureOnField(&game.figure.shape, 7);

  for (int i = 0; i < 5; i++) {
    userInput(Down, 0, &game);

    userInput(Right, 0, &game);
  }
  for (int i = 0; i < 5; i++) {
    userInput(Right, 0, &game);
  }
  int index = game.figure.currentTetramino;
  userInput(Action, 0, &game);

  ck_assert_int_eq(index, game.figure.currentTetramino);
  freeSpace(&game);
}
END_TEST

START_TEST(user_input_rotate3) {
  Tetris game;
  initialGame(&game);
  game.figure.currentTetramino = 7;
  cpyFigureOnField(&game.figure.shape, 7);
  for (int i = 1; i < GAME_FIELD_HEIGHT; i++) {
    for (int j = 0; j < GAME_FIELD_WIDTH; j++) {
      if (j != game.figure.x + 1) game.gameInfo.field[i][j] = 1;
    }
  }
  userInput(Start, 0, &game);
  game.figure.y = 3;
  int index = game.figure.currentTetramino;
  userInput(Action, 0, &game);

  ck_assert_int_eq(game.figure.currentTetramino, index);
  freeSpace(&game);
}
END_TEST

Suite *test_game_user_input_action(void) {
  Suite *s;
  s = suite_create("s21_game_user_input_action");
  TCase *tcase_user_input = tcase_create("USER_INPUT");
  tcase_add_test(tcase_user_input, user_input_start);
  tcase_add_test(tcase_user_input, user_input_pause1);
  tcase_add_test(tcase_user_input, user_input_pause2);
  tcase_add_test(tcase_user_input, user_input_pause3);
  tcase_add_test(tcase_user_input, user_input_terminate);

  tcase_add_test(tcase_user_input, user_input_left1);
  tcase_add_test(tcase_user_input, user_input_left2);
  tcase_add_test(tcase_user_input, user_input_right1);
  tcase_add_test(tcase_user_input, user_input_right2);

  tcase_add_test(tcase_user_input, user_input_down);
  tcase_add_test(tcase_user_input, user_input_rotate);
  tcase_add_test(tcase_user_input, user_input_rotate2);
  tcase_add_test(tcase_user_input, user_input_rotate3);

  suite_add_tcase(s, tcase_user_input);
  return s;
}

//////////////////// CHANGING_SCORE ////////////////////

START_TEST(change_score1) {
  Tetris game;
  initialGame(&game);
  changeScore(&game, 1);

  ck_assert_int_eq(game.gameInfo.score, 100);
  freeSpace(&game);
}
END_TEST

START_TEST(change_score2) {
  Tetris game;
  initialGame(&game);
  changeScore(&game, 2);

  ck_assert_int_eq(game.gameInfo.score, 300);
  freeSpace(&game);
}
END_TEST
START_TEST(change_score3) {
  Tetris game;
  initialGame(&game);
  changeScore(&game, 3);

  ck_assert_int_eq(game.gameInfo.score, 700);
  freeSpace(&game);
}
END_TEST
START_TEST(change_score4) {
  Tetris game;
  initialGame(&game);
  changeScore(&game, 4);

  ck_assert_int_eq(game.gameInfo.score, 1500);
  freeSpace(&game);
}
END_TEST

START_TEST(change_score5) {
  Tetris game;
  initialGame(&game);
  changeScore(&game, 1);
  changeScore(&game, 2);
  changeScore(&game, 3);
  changeScore(&game, 4);

  ck_assert_int_eq(game.gameInfo.score, 2600);
  freeSpace(&game);
}
END_TEST

Suite *test_game_changing_score(void) {
  Suite *s;
  s = suite_create("s21_game_changing_score");
  TCase *tcase_change_score = tcase_create("CHANGING_SCORE");
  tcase_add_test(tcase_change_score, change_score1);
  tcase_add_test(tcase_change_score, change_score2);
  tcase_add_test(tcase_change_score, change_score3);
  tcase_add_test(tcase_change_score, change_score4);
  tcase_add_test(tcase_change_score, change_score5);

  suite_add_tcase(s, tcase_change_score);
  return s;
}

//////////////////// LOCKING_FIGURE ////////////////////

START_TEST(locking_figure1) {
  Tetris game;
  initialGame(&game);
  game.figure.currentTetramino = 0;
  cpyFigureOnField(&game.figure.shape, 0);
  userInput(Start, 0, &game);
  for (int i = 0; i < 20; i++) {
    updateCurrentState(&game);
  }

  ck_assert_int_eq(game.gameInfo.field[GAME_FIELD_HEIGHT - 1][4], 1);
  freeSpace(&game);
}
END_TEST
START_TEST(locking_figure2) {
  Tetris game;
  initialGame(&game);
  game.figure.currentTetramino = 0;
  cpyFigureOnField(&game.figure.shape, 0);
  game.gameInfo.field[1][5] = 1;
  userInput(Start, 0, &game);
  for (int i = 0; i < 20; i++) {
    updateCurrentState(&game);
  }

  ck_assert_int_eq(game.gameInfo.pause, END);
  freeSpace(&game);
}
END_TEST
START_TEST(attaching_figure1) {
  Tetris game;
  initialGame(&game);
  for (int i = 0; i < GAME_FIELD_WIDTH; i++) {
    game.gameInfo.field[5][i] = 1;
  }
  game.gameInfo.field[4][4] = 2;
  game.gameInfo.field[6][4] = 2;
  userInput(Start, 0, &game);
  fullFillBlock(&game);

  ck_assert_int_eq(game.gameInfo.field[6][4], 2);
  ck_assert_int_eq(game.gameInfo.field[5][4], 2);

  freeSpace(&game);
}
END_TEST
Suite *test_game_locking_figures(void) {
  Suite *s;
  s = suite_create("s21_game_lock_figure");
  TCase *tcase_lock_figure = tcase_create("CHANGING_SCORE");
  tcase_add_test(tcase_lock_figure, locking_figure1);
  tcase_add_test(tcase_lock_figure, locking_figure2);
  tcase_add_test(tcase_lock_figure, attaching_figure1);

  suite_add_tcase(s, tcase_lock_figure);
  return s;
}
END_TEST
// MAIN //
static int run_test_suite(Suite *test_suite) {
  int number_failed = 0;
  SRunner *sr = NULL;

  sr = srunner_create(test_suite);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed);
}

int main(void) {
  int number_failed = 0;
  Suite *suite_array[] = {

      test_create(),
      test_high_score(),
      test_game_pause_status(),
      test_game_user_input_action(),
      test_game_changing_score(),
      test_game_locking_figures(),

      NULL};

  for (size_t i = 0; suite_array[i]; ++i) {
    number_failed += run_test_suite(suite_array[i]);
  }
  remove("score.txt");
  printf("NUMBER FAILED %d\n", number_failed);
}