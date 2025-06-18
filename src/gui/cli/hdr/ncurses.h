/**
 * @file ncurses.h
 * @brief
 */

#ifndef NCURSES_H
#define NCURSES_H

#include <locale.h>
#include <ncurses.h>

#include "../../../brick_game/tetris/header/game_logic.h"
#include "tetris_interface.h"

/**
 * @brief Инициализирует ncurses для работы с текстовым интерфейсом.
 */
void initNcurses();

/**
 * @brief Очищает ресурсы ncurses перед завершением работы.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую информацию об игре.
 */
void cleanupNcurses(GameInfo_t *game);

/**
 * @brief Очищает ncurses перед выходом из стартового экрана.
 */
void cleanupNcursesstart();

#endif  // NCURSES_H
