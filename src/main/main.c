/**
 * @file main.c
 * @brief Главный файл игры.
 *
 * Этот файл содержит основную точку входа в игру.
 */

#include "main.h"

#include <locale.h>
#include <stdlib.h>
#include <time.h>

#include "../brick_game/common_base/brick_game.h"
#include "../brick_game/common_base/game_logic.c"
#include "../brick_game/tetris/hdr/fsm.h"
#include "../gui/cli/hdr/ncurses.h"
#include "../gui/cli/hdr/tetris_interface.h"

int main(void) {
  initNcurses();
  gameLoop();
  return 0;
}
