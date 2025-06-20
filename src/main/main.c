/**
 * @file main.c
 * @brief Главный файл игры.
 *
 * Этот файл содержит основную точку входа в игру.
 */

#include "../hdr/main.h"

#include "../hdr/ncurses.h"
/**
 * @brief Основная функция программы.
 *
 * Функция инициализирует библиотеку ncurses, устанавливает локаль.
 * Отображает стартовый экран с приглашением начать игру
 * (`renderStartScreen`). Запускает основной игровой цикл (`game_loop`).
 *
 * @return Возвращает код завершения программы (успех или ошибка).
 */
int main(void) {
  WIN_INIT(50);
  setlocale(LC_ALL, "");
  renderStartScreen();

  return 0;
}
