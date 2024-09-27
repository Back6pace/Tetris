#ifndef FRONTEND_H
#define FRONTEND_H

#include "../../brick_game/tetris/backend.h"

#define SPEED_RATE 0.150
#define READ_DELAY 50

int start_game();
/**
 * @brief Инициализация функций для отработки библиотеки ncurses
 */
void initialize_interface();
/**
 * @brief Инициализация цветов
 *
 * @param 1 пара цвета: белый фон, чёрный текст
 * @param 2 пара цвета: чёрный цвет для всех фигур
 */
void init_colors();
/**
 * @brief Отрисовка интерфейса
 */
void draw_interface(Tetris *game);
/**
 * @brief Отрисовывает игровое поле
 *
 * @param game Указатель на структуру Tetris.
 */
void game_field(Tetris *game);
/**
 * @brief Отрисовывает текущую фигуру на игровом поле.
 *
 */
void draw_figure(Tetris *game);
/**
 * @brief Отрисовывает фигуру в поле доп. информации про следующую фигуру
 *
 */
void draw_next_figure(Tetris *game);
/**
 * @brief Отображает начальную игровую панель с информацией о следующей фигуре,
 * счёте, уровне и ника разработчика.
 */
void initialGamebar();
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void clearField(Tetris *game);
void catch_input(Tetris *game);
#endif