// backend.h - Чистая игровая логика для Tetris
#ifndef BRICKGAME_TETRIS_BACKEND_H_
#define BRICKGAME_TETRIS_BACKEND_H_

#include "../../../include/brickgame/common/types.h"

// Инициализация игры (первый запуск или рестарт)
GameInfo_t backend_init_game(void);

// Обработка действия игрока
void backend_handle_input(UserAction_t action, bool hold);

// Обновление физики игры (например, падение вниз по таймеру)
void backend_update_physics(GameInfo_t *info);



//////////////////
void backend_overlay_piece(GameInfo_t *info_ptr);
#endif  // BRICKGAME_TETRIS_BACKEND_H_ 