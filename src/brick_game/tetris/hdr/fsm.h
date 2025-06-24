/**
 * @file fsm.h
 * @brief Определение функций и структур для обработки состояний игры и ввода
 * пользователя.
 */

#ifndef FSM_H
#define FSM_H

#include <time.h>
#include <stddef.h>
#include <unistd.h>
#include "../../common_base/brick_game.h"
#include "../hdr/game_logic.h"
#include "../hdr/piece_create.h"
#include "../hdr/piece_fix.h"
#include "../hdr/piece_move.h"
#include "game_info.h"
#include "objects.h"

typedef enum {
  kStateIdle,
  kStatePlaying,
  kStatePaused,
  kStateOver,
  kStateTerminated
} FSMState;

typedef struct {
  FSMState current_state;
  GameInfo_t game_info;
  Piece *current_piece;
  clock_t last_tick;
} FSMContext;

typedef struct {
  FSMState current;
  UserAction_t input;
  FSMState next;
  void (*handler)(FSMContext *ctx, bool hold);
} Transition;
GameInfo_t updateGameLogic(FSMState state);

#endif  // FSM_H
