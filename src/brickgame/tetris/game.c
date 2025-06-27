#include "../../../include/brickgame/tetris/game.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/brickgame/common/types.h"
#include "../../../include/brickgame/tetris/backend.h"
#include "../../../include/brickgame/tetris/fsm.h"

static GameInfo_t game_info;

static void reset_game_info(void) {
  // Предположим, backend выделяет поле и next
  if (game_info.field) {
    free(game_info.field);  // Освобождаем старое поле, если нужно
  }
  if (game_info.next) {
    free(game_info.next);
  }
  game_info = backend_init_game();  // Новый запуск через backend
}

void userInput(UserAction_t action, bool hold) {
  fsm_process_input(action);
  GameState_t state = fsm_get_state();

  if (state == STATE_RUNNING) {
    // Только в режиме игры обрабатываем физику
    backend_handle_input(action, hold);
  }
}

GameInfo_t updateCurrentState() {
  GameState_t state = fsm_get_state();
  if (state == STATE_INIT) {
    reset_game_info();
  }
  if (state == STATE_RUNNING) {
    backend_update_physics(&game_info);
  }

  backend_overlay_piece(&game_info);

  game_info.pause = (state == STATE_PAUSED);
  return game_info;
}
bool isGameOver(void) { return fsm_get_state() == STATE_GAME_OVER; }