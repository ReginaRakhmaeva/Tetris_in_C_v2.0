/**
 * @file objects.h
 * @brief Определение структур данных, используемых в игре Тетрис.
 */

#ifndef OBJECTS_H
#define OBJECTS_H

#include <ncurses.h>

#include "defines.h"

/**
 * @struct Piece
 * @brief Структура, представляющая фигуру Тетриса.
 */
typedef struct {
  int x;           /**< Координата X фигуры на поле */
  int y;           /**< Координата Y фигуры на поле */
  int shape[4][4]; /**< Матрица 4x4, представляющая форму фигуры */
} Piece;

#endif  // OBJECTS_H
