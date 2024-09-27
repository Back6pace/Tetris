#include "backend.h"

void userInput(UserAction_t action, bool hold, Tetris *game) {
  (void)hold;
  if (game->gameInfo.pause == STARTED) {
    switch (action) {
      case Left:
        move_left(game);
        break;
      case Right:
        move_right(game);
        break;
      case Action:
        rotate(game);
        break;
      case Down:
        updateCurrentState(game);
        break;
      default:
        break;
    }
  }
  switch (action) {
    case Start:
      game_start(game);
      break;
    case Pause:
      game_pause(game);
      break;
    case Terminate:
      game_terminate(game);
      break;
    default:
      break;
  }
}

void game_start(Tetris *game) { game->gameInfo.pause = STARTED; }

void game_pause(Tetris *game) {
  if (game->gameInfo.pause == PAUSE || game->gameInfo.pause == STARTED) {
    game->gameInfo.pause = game->gameInfo.pause == 2 ? PAUSE : STARTED;
  }
}

void game_terminate(Tetris *game) {
  writeHighScore(game);
  game->gameInfo.pause = END;
}

void moveDown(Tetris *game) { (game->figure.y)++; }

void move_left(Tetris *game) {
  if (isValidPosition(game, -1, 0)) {
    (game->figure.x)--;
  }
}

void move_right(Tetris *game) {
  if (isValidPosition(game, 1, 0)) {
    (game->figure.x)++;
  }
}

void rotate(Tetris *game) {
  if (isValidToRotate(game)) {
    game->figure.currentTetramino = (game->figure.currentTetramino + 7) % 28;
    cpyFigureOnField(&game->figure.shape, game->figure.currentTetramino);
  }
}

int isValidToRotate(Tetris *game) {
  int flag = 1;
  int **tempShape;
  int tempNorm = createField(&tempShape, 4, 4);
  if (tempNorm == NORM) {
    int nextIndex = (game->figure.currentTetramino + 7) % 28;
    cpyFigureOnField(&tempShape, nextIndex);

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (tempShape[i][j] == 1) {
          int newY = game->figure.y + i;
          int newX = game->figure.x + j;

          if (newX < 0 || newX >= GAME_FIELD_WIDTH ||
              newY >= GAME_FIELD_HEIGHT) {
            flag = 0;
          }

          // Проверка на коллизию с уже установленными блоками
          if (newY >= 0 && game->gameInfo.field[newY][newX] != 0) {
            flag = 0;
          }
        }
      }
    }
    for (int i = 0; i < 4; i++) free(tempShape[i]);
    free(tempShape);
  }

  return flag;
}

int isValidPosition(Tetris *game, int diffX, int diffY) {
  int flag = 1;
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (game->figure.shape[y][x]) {  // Проверка фигуры
        int newX = game->figure.x + x + diffX;
        int newY = game->figure.y + y + diffY;
        if (newX < 0 || newX >= GAME_FIELD_WIDTH || newY < 0 ||
            newY >= GAME_FIELD_HEIGHT || game->gameInfo.field[newY][newX]) {
          flag = 0;
        }
      }
    }
  }
  return flag;
}

GameInfo_t updateCurrentState(Tetris *game) {
  if (game->gameInfo.pause == STARTED) {
    checkLockFigure(game);
    moveDown(game);
  }
  return game->gameInfo;
}

void checkLockFigure(Tetris *game) {
  for (int i = 3; i >= 0; i--) {
    for (int j = 3; j >= 0; j--) {
      int ptr = game->figure.shape[i][j];
      if (game->figure.y + i == GAME_FIELD_HEIGHT - 1) {
        lockFigure(game);

      } else if (ptr == 1 && game->figure.y + i + 1 >= 0 &&
                 (game->gameInfo.field[game->figure.y + i + 1]
                                      [game->figure.x + j] != 0)) {
        if (checkLose(game) == 1) {
          game->gameInfo.pause = END;
        } else {
          lockFigure(game);
        }
      }
    }
  }
}

int checkLose(Tetris *game) {
  int flag = 0;
  for (int i = 0; i < GAME_FIELD_WIDTH; i++) {
    if (game->gameInfo.field[0][i]) {
      flag = 1;
    }
  }
  return flag;
}

void lockFigure(Tetris *game) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->figure.shape[i][j] == 1 && game->figure.y + i >= 0 &&
          game->figure.x + j >= 0 && game->figure.x + j < GAME_FIELD_WIDTH &&
          !game->gameInfo.field[game->figure.y + i][game->figure.x + j]) {
        game->gameInfo.field[game->figure.y + i][game->figure.x + j] = 1;
      }
    }
  }
  initFigure(game);
}

/**
 * Матрица для хранения фигуры
 */
static bool TETROMINOS[28][4][4] = {
    // Фигуры с поворотом на 0°
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}},  // I
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}},  // L
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}},  // J
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}},  // O
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}},  // S
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}},  // T
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}},  // Z

    // Фигуры с поворотом на 90°
    {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},  // I
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},  // L
    {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}},  // J
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}},  // O
    {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}},  // S
    {{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}},  // T
    {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}},  // Z

    // Фигуры с поворотом на 180°
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}},  // I
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}},  // L
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}},  // J
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}},  // O
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}},  // S
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}},  // T
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}},  // Z

    // Фигуры с поворотом на 270°
    {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},  // I
    {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}},  // L
    {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},  // J
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}},  // O
    {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}},  // S
    {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}},  // T
    {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}}   // Z
};

int createField(int ***field, int m, int n) {
  int flag = NORM;

  *field = (int **)calloc(m, sizeof(int *));
  if (*field == NULL) {
    return ERROR;
  }
  for (int i = 0; i < m; i++) {
    (*field)[i] = (int *)calloc(n, sizeof(int));
    if ((*field)[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free((*field)[j]);
      }
      free(*field);
      *field = NULL;
      return ERROR;
    }
  }
  return flag;
}

int initialGame(Tetris *game) {
  game->gameInfo.score = 0;
  game->gameInfo.level = 1;
  game->gameInfo.pause = NOT_START;
  readhighScore(game);
  game->speed = 1;
  int flag = NORM;
  flag =
      createField(&(game->gameInfo.field), GAME_FIELD_HEIGHT, GAME_FIELD_WIDTH);
  if (flag == NORM) {
    flag = createField(&(game->gameInfo.next), 4, 4);
  }
  if (flag == NORM) {
    createField(&(game->figure.shape), 4, 4);
  }
  if (flag == NORM) {
    int randomIndex = rand() % 7;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        game->gameInfo.next[i][j] = TETROMINOS[randomIndex][i][j];
      }
    }
    game->figure.nextTetramino = randomIndex;
    initFigure(game);
  }
  return flag;
}

void initFigure(Tetris *game) {
  fullFillBlock(game);
  game->figure.currentTetramino = game->figure.nextTetramino;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game->figure.shape[i][j] = game->gameInfo.next[i][j];
    }
  }
  int randomIndex = rand() % 7;
  cpyFigureOnField(&game->gameInfo.next, randomIndex);
  game->figure.nextTetramino = randomIndex;
  game->figure.x = GAME_FIELD_WIDTH / 2 - 2;
  game->figure.y = -3;
}

void cpyFigureOnField(int ***field, int index) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      (*field)[i][j] = TETROMINOS[index][i][j];
    }
  }
}

void fullFillBlock(Tetris *game) {
  int cnt = 0;  // Счетчик очищенных строк

  for (int i = 0; i < GAME_FIELD_HEIGHT; i++) {
    int sum = 0;  // Сумма элементов в строке

    // Проверяем, заполнена ли строка полностью
    for (int j = 0; j < GAME_FIELD_WIDTH; j++) {
      if (game->gameInfo.field[i][j] == 1) {
        sum += 1;
      }
    }

    // Если строка заполнена
    if (sum == GAME_FIELD_WIDTH) {
      cnt += 1;  // Увеличиваем счетчик очищенных строк

      // Очищаем текущую строку
      for (int j = 0; j < GAME_FIELD_WIDTH; j++) {
        game->gameInfo.field[i][j] = 0;
      }

      // Сдвигаем все строки выше текущей вниз
      for (int k = i - 1; k >= 0; k--) {
        for (int j = 0; j < GAME_FIELD_WIDTH; j++) {
          game->gameInfo.field[k + 1][j] = game->gameInfo.field[k][j];
        }
      }

      // Очищаем верхнюю строку после сдвига
      for (int j = 0; j < GAME_FIELD_WIDTH; j++) {
        game->gameInfo.field[0][j] = 0;
      }

      // Проверяем строку снова, так как она теперь сдвинута
      i--;
    }
  }
  changeScore(game, cnt);
  writeHighScore(game);
  updateLevelandSpeed(game);
}

void changeScore(Tetris *game, int cnt) {
  if (cnt == 1) {
    game->gameInfo.score += 100;
  } else if (cnt == 2) {
    game->gameInfo.score += 300;
  } else if (cnt == 3) {
    game->gameInfo.score += 700;
  } else if (cnt == 4) {
    game->gameInfo.score += 1500;
  }
}

void readhighScore(Tetris *game) {
  FILE *fp = fopen("score.txt", "r");
  if (fp == NULL) {
    game->gameInfo.high_score = 0;
  } else {
    char scorem[256];
    if (fgets(scorem, 256, fp) != NULL) {
      int write_score;
      if (sscanf(scorem, "%d", &write_score) == 1) {
        game->gameInfo.high_score = write_score;
      } else {
        game->gameInfo.high_score = 0;
      }
    } else {
      game->gameInfo.high_score = 0;
    }
    fclose(fp);
  }
}

void writeHighScore(Tetris *game) {
  if (game->gameInfo.score >= game->gameInfo.high_score) {
    FILE *fp = fopen("score.txt", "w");
    game->gameInfo.high_score = game->gameInfo.score;
    if (fp != NULL) {
      fprintf(fp, "%d", game->gameInfo.score);
      fclose(fp);
    }
  }
}

void updateLevelandSpeed(Tetris *game) {
  if (game->gameInfo.score / 6000) {
    game->gameInfo.level = 10;
  } else {
    game->gameInfo.level = 1 + game->gameInfo.score / 600;
  }
  game->speed = game->gameInfo.level;
}

void freeSpace(Tetris *game) {
  for (int i = 0; i < 4; i++) {
    free(game->figure.shape[i]);
  }
  free(game->figure.shape);
  for (int i = 0; i < 4; i++) {
    free(game->gameInfo.next[i]);
  }
  free(game->gameInfo.next);
  for (int i = 0; i < GAME_FIELD_HEIGHT; i++) {
    free(game->gameInfo.field[i]);
  }
  free(game->gameInfo.field);
}