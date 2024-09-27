#ifndef BACKEND_H
#define BACKEND_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define GAME_FIELD_HEIGHT 20
#define GAME_FIELD_WIDTH 10
#define NORM 0
#define ERROR 1
#define STARTED 2
#define NOT_START 3
#define PAUSE 4
#define END 5

/**
 * Перечисление всех возможных действий игрока
 */
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

/**
 * Структура для хранения информации о текущем состоянии игры
 */
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef struct {
  int x;
  int y;
  int **shape;
  int currentTetramino;
  int nextTetramino;
} Figure_t;

typedef struct {
  GameInfo_t gameInfo;
  Figure_t figure;
  double speed;
  bool flag;
} Tetris;

/**
 * Принимает действие игрока
 */
void userInput(UserAction_t action, bool hold, Tetris *game);

/**
 * Функция для отрисовки текущего состояния игры
 */
GameInfo_t updateCurrentState(Tetris *game);

/**
 * Функция для создания игрового поля
 */
int createField(int ***field, int m, int n);
/**
 * @brief Инициализация игрового поля, происходит при запуске игры, задает
 * начальные параметры
 */
int initialGame(Tetris *game);
void readhighScore(Tetris *game);
void writeHighScore(Tetris *game);
/**
 * @brief Освобождает память, выделенную для игрового поля и фигур.
 */
void freeSpace(Tetris *game);

/**
 * @brief Инициализация фигуры
 */
void initFigure(Tetris *game);

/**
 * @brief Копирует данные фигуры в игровое поле.
 */
void cpyFigureOnField(int ***field, int index);

/**
 * @brief Обновляет уровень игры
 */
void updateLevelandSpeed(Tetris *game);

/**
 * @brief Функция отвечает за удаление заполненных строк на игровом поле после
 * добавления текущей фигуры
 */
void fullFillBlock(Tetris *game);

/**
 * @brief Функция для изменения счета
 */
void changeScore(Tetris *game, int cnt);

void moveDown(Tetris *game);
void game_terminate(Tetris *game);
void game_pause(Tetris *game);
void game_start(Tetris *game);

/**
 * @brief Проверяет, можно ли переместить фигуру
 */
int isValidPosition(Tetris *game, int diffX, int diifY);

/**
 * @brief Проверяет возможность блокировки фигуры
 */
void checkLockFigure(Tetris *game);
/**
 * @brief Фиксирует фигуру на поле
 */
void lockFigure(Tetris *game);
void move_left(Tetris *game);
void move_right(Tetris *game);
void rotate(Tetris *game);
int isValidToRotate(Tetris *game);
int checkLose(Tetris *game);
#endif