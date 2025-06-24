/**
 * @file tetris_interface.c
 * @brief Интерфейсные функции для игры Tetris.
 *
 * Этот файл содержит функции, отвечающие за отображение игрового поля,
 * текущей фигуры, информации о состоянии игры, а также обработку экранов
 * начала и окончания игры. Также включает логику завершения игры.
 */

#include "../hdr/tetris_interface.h"

/**
 * @brief Очищает игровое поле.
 *
 * Функция очищает экран и отрисовывает границы игрового поля.
 */
void clearField() {
  clear();
  for (int i = 0; i <= ROWS; i++) {
    mvprintw(i, 0, "|");
    mvprintw(i, COLUMNS * 2 + 1, "|");
  }
  for (int j = 0; j <= COLUMNS * 2; j++) {
    mvprintw(0, j, "-");
    mvprintw(ROWS + 1, j, "-");
  }
  mvprintw(0, 0, "+");
  mvprintw(0, COLUMNS * 2 + 1, "+");
  mvprintw(ROWS + 1, 0, "+");
  mvprintw(ROWS + 1, COLUMNS * 2 + 1, "+");
}

/**
 * @brief Отрисовывает статичное игровое поле.
 *
 * Функция отрисовывает блоки на поле, которые уже заняты, отображая их как
 * "[]".
 *
 * @param field Игровое поле, представленное как двумерный массив.
 */
void drawStaticField(int **field) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      if (field[i][j]) {
        mvprintw(i + 1, j * 2 + 1, "[]");
      }
    }
  }
}

/**
 * @brief Отрисовывает текущую фигуру на игровом поле.
 *
 * Функция отрисовывает текущую фигуру на экране в соответствии с ее
 * координатами.
 *
 * @param piece Указатель на структуру Piece, представляющую текущую фигуру.
 */
void drawPiece(Piece *piece) {
  if (!piece) return;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (piece->shape[i][j]) {
        mvprintw(piece->y + i + 1, (piece->x + j) * 2 + 1, "[]");
      }
    }
  }
}

/**
 * @brief Отрисовывает информацию о текущей игре.
 *
 * Функция отображает информацию о текущем счете, уровне, высоком счете и
 * следующей фигуре.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую информацию о
 * текущей игре.
 */
void drawGameInfo(GameInfo_t *game) {

    if (!game) return;
  mvprintw(2, COLUMNS * 2 + 5, "Score: %d", game->score);
  mvprintw(3, COLUMNS * 2 + 5, "Level: %d", game->level);
  mvprintw(4, COLUMNS * 2 + 5, "High Score: %d", game->high_score);
  mvprintw(6, COLUMNS * 2 + 5, "Next:");
  if (game->next) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (game->next[i][j]) {
          mvprintw(6 + i, COLUMNS * 2 + 11 + j * 2, "[]");
        }
      }
    }
  }
}

/**
 * @brief Отображает экран начала игры с инструкциями.
 *
 * Функция очищает экран и отображает название игры, инструкции и клавиши
 * управления.
 */
void renderStartScreen() {
  clear();

  mvprintw(ROWS / 2 - 5, COLUMNS / 2, "TETRIS GAME");
  mvprintw(ROWS / 2, COLUMNS / 2 - 4, "Press ENTER to Start");
  mvprintw(ROWS / 2 + 1, COLUMNS / 2, "or q to Quit");
  mvprintw(ROWS / 2 + 4, COLUMNS / 2 + 2, "RULES");
  mvprintw(ROWS / 2 + 5, COLUMNS / 2 - 3, "Arrow Keys: Move");
  mvprintw(ROWS / 2 + 6, COLUMNS / 2 - 2, "Space: Rotate");
  mvprintw(ROWS / 2 + 7, COLUMNS / 2 - 3, "p: Pause, q: Quit");

  refresh();
}

/**
 * @brief Отрисовывает игровое поле, фигуру и информацию о текущем состоянии
 * игры.
 *
 * Функция очищает экран, отрисовывает статичное поле, текущую фигуру и
 * информацию о текущей игре.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую информацию о
 * текущей игре.
 */
void drawField(GameInfo_t *game) {
  clearField();
  drawStaticField(game->field);
  drawPiece(getCurrentPiece());
  drawGameInfo(game);
  refresh();
}

/**
 * @brief Отображает экран окончания игры.
 *
 * Функция очищает экран и отображает сообщение о завершении игры, а также
 * информацию о счете.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую информацию о
 * текущей игре.
 */
void displayGameOverScreen(GameInfo_t *game) {
  clear();
  mvprintw(ROWS / 2 - 1, COLUMNS / 2 - 5, "GAME OVER");
  mvprintw(ROWS / 2, COLUMNS / 2 - 8, "Your Score: %d", game->score);
  mvprintw(ROWS / 2 + 1, COLUMNS / 2 - 10, "Press ENTER to Restart");
  mvprintw(ROWS / 2 + 2, COLUMNS / 2 - 7, "or q to Quit");

  refresh();
}
