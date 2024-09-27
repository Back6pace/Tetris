#include "brick_game/tetris/backend.h"
#include "gui/cli/frontend.h"

int main() {
  srand(time(NULL));        // Инициализация генератора случайных чисел
  int flag = start_game();  // Запуск игры
  return flag;
}