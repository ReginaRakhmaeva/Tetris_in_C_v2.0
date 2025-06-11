/**
 * @file ncurses.c
 * @brief
 *
 *
 */

#include "../hdr/ncurses.h"

/**
 * @brief Инициализирует библиотеку ncurses для отображения игры в консоли.
 *
 * Функция настраивает ncurses для работы с терминалом: отключает буферизацию
 * ввода, скрывает курсор, устанавливает режим обработки клавиш и задает таймаут
 * для обновления экрана.
 */
void initNcurses() {
  initscr();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
}

/**
 * @brief Завершает работу с ncurses и очищает память, выделенную для поля и
 * фигур.
 *
 * Функция завершает работу с ncurses и освобождает память, выделенную для
 * игрового поля и фигур.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую информацию о
 * текущей игре.
 */
void cleanupNcurses(GameInfo_t *game) {
  endwin();
  for (int i = 0; i < ROWS; i++) {
    free(game->field[i]);
  }
  free(game->field);

  for (int i = 0; i < 4; i++) {
    free(game->next[i]);
  }
  free(game->next);
}

/**
 * @brief Завершает работу с ncurses без очистки данных игры.
 *
 * Функция завершает работу с ncurses без освобождения памяти, связанной с
 * игровыми данными.
 */
void cleanupNcursesstart() {
  endwin();  // Завершение работы с ncurses
}

/**
 * @brief Показывает экран окончания игры и обрабатывает ввод.
 *
 * Функция отображает экран окончания игры и обрабатывает пользовательский ввод
 * для перезапуска игры или выхода.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую информацию о
 * текущей игре.
 */
void showGameOverScreen(GameInfo_t *game) {
  displayGameOverScreen(game);
  cleanupNcurses(game);
}
