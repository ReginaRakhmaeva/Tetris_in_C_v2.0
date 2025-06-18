#include "fsm.h"

#include <stddef.h>
#include <time.h>

#include "../../common_base/brick_game.h"
#include "../hdr/game_logic.h"
#include "../hdr/piece_create.h"
#include "../hdr/piece_fix.h"
#include "../hdr/piece_move.h"
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

static FSMContext fsm_ctx = {kStateIdle, {0}, NULL, 0};

/* --- Обработчики --- */
static void HandleStart(FSMContext *ctx, bool hold) {
  initializeGame(&ctx->game_info);
  spawnNewPiece(&ctx->current_piece);
  ctx->last_tick = clock();
}

static void HandlePause(FSMContext *ctx, bool hold) {
  ctx->game_info.pause = !ctx->game_info.pause;
}

static void HandleTerminate(FSMContext *ctx, bool hold) {
  saveHighScore(ctx->game_info.high_score);
  if (ctx->current_piece != NULL) {
    free(ctx->current_piece);
    ctx->current_piece = NULL;
  }
}

static void HandleLeft(FSMContext *ctx, bool hold) {
  if (canMoveLeft(ctx->current_piece, ctx->game_info.field)) {
    ctx->current_piece->x--;
  }
}

static void HandleRight(FSMContext *ctx, bool hold) {
  if (canMoveRight(ctx->current_piece, ctx->game_info.field)) {
    ctx->current_piece->x++;
  }
}

static void HandleDown(FSMContext *ctx, bool hold) {
  if (!updatePiecePosition(ctx->current_piece, &ctx->game_info,
                           &ctx->last_tick)) {
    fixPiece(ctx->game_info.field, ctx->current_piece);
    int cleared = clearFullLines(ctx->game_info.field);
    updateScore(&ctx->game_info, cleared);
    updateLevelAndSpeed(&ctx->game_info);
    spawnNewPiece(&ctx->current_piece);
  }
}

static void HandleAction(FSMContext *ctx, bool hold) {
  rotatePiece(ctx->current_piece, ctx->game_info.field);
}

static void HandleNoOp(FSMContext *ctx, bool hold) {
  (void)ctx;
  (void)hold;
}

static const Transition kFsmTable[] = {
    {kStateIdle, Start, kStatePlaying, HandleStart},
    {kStatePlaying, Pause, kStatePaused, HandlePause},
    {kStatePaused, Pause, kStatePlaying, HandlePause},
    {kStatePlaying, Terminate, kStateTerminated, HandleTerminate},
    {kStatePlaying, Left, kStatePlaying, HandleLeft},
    {kStatePlaying, Right, kStatePlaying, HandleRight},
    {kStatePlaying, Down, kStatePlaying, HandleDown},
    {kStatePlaying, Action, kStatePlaying, HandleAction}};

void userInput(UserAction_t action, bool hold) {
  size_t transitions = sizeof(kFsmTable) / sizeof(Transition);
  for (size_t i = 0; i < transitions; ++i) {
    if (kFsmTable[i].current == fsm_ctx.current_state &&
        kFsmTable[i].input == action) {
      fsm_ctx.current_state = kFsmTable[i].next;
      kFsmTable[i].handler(&fsm_ctx, hold);
      return;
    }
  }
  HandleNoOp(&fsm_ctx, hold);
}

GameInfo_t updateCurrentState() {
  fsm_ctx.game_info = updateGameLogic(fsm_ctx.current_state);
  return fsm_ctx.game_info;
}