#include "../hdr/fsm.h"

#include <stddef.h>
#include <time.h>

static FSMContext fsm_ctx = {kStateIdle, {0}, NULL, 0};

static void HandleStart(FSMContext *ctx, bool hold) {
  (void)hold;
  initializeGame(&ctx->game_info);
  spawnNewPiece(&ctx->current_piece);
  ctx->last_tick = clock();
}

static void HandlePause(FSMContext *ctx, bool hold) {
  (void)hold;
  ctx->game_info.pause = !ctx->game_info.pause;
}

static void HandleTerminate(FSMContext *ctx, bool hold) {
  (void)hold;
  saveHighScore(ctx->game_info.high_score);
  if (ctx->current_piece != NULL) {
    free(ctx->current_piece);
    ctx->current_piece = NULL;
  }
}

static void HandleLeft(FSMContext *ctx, bool hold) {
  (void)hold;
  if (canMoveLeft(ctx->current_piece, ctx->game_info.field)) {
    ctx->current_piece->x--;
  }
}

static void HandleRight(FSMContext *ctx, bool hold) {
  (void)hold;
  if (canMoveRight(ctx->current_piece, ctx->game_info.field)) {
    ctx->current_piece->x++;
  }
}

static void HandleDown(FSMContext *ctx, bool hold) {
  (void)hold;
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
  (void)hold;
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

GameInfo_t updateGameLogic(FSMState state) {
  GameInfo_t game;
  initializeGame(&game);

  switch (state) {
    case kStateIdle:
      // Ожидание старта — пустая инициализация
      break;

    case kStatePlaying:
      // Запуск игры или продолжение
      game.pause = 0;
      break;

    case kStatePaused:
      game.pause = 1;
      break;

    case kStateOver:
      // Здесь можно добавить анимацию или экран проигрыша
      break;

    case kStateTerminated:
      // Сброс или очистка
      break;
  }

  return game;
}