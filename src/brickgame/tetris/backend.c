#include "../../../include/brickgame/tetris/backend.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../../include/brickgame/tetris/fsm.h"  // Для установки GAME_OVER

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define FIGURE_SIZE 4

typedef struct {
  int shape[FIGURE_SIZE][FIGURE_SIZE];
  int x, y;
} Tetromino;

// Global constant: список всех фигур (в положении 0)
static const int FIGURES[7][FIGURE_SIZE][FIGURE_SIZE] = {
    // I
    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    // O
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    // T
    {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
    // S
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
    // Z
    {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    // J
    {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    // L
    {{0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}}};

static int field[FIELD_HEIGHT][FIELD_WIDTH];
static Tetromino current_piece;
static Tetromino next_piece;
static GameInfo_t info;

static void rotate_clockwise(int src[FIGURE_SIZE][FIGURE_SIZE],
                             int dst[FIGURE_SIZE][FIGURE_SIZE]) {
  for (int y = 0; y < FIGURE_SIZE; ++y) {
    for (int x = 0; x < FIGURE_SIZE; ++x) {
      dst[x][FIGURE_SIZE - 1 - y] = src[y][x];
    }
  }
}

static void spawn_piece(Tetromino *dst) {
  int id = rand() % 7;
  memcpy(dst->shape, FIGURES[id], sizeof(dst->shape));
  dst->x = 3;
  dst->y = -2;
}

static void clear_lines() {
  int lines_cleared = 0;
  for (int y = FIELD_HEIGHT - 1; y >= 0; --y) {
    int full = 1;
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      if (!field[y][x]) {
        full = 0;
        break;
      }
    }

    if (full) {
      lines_cleared++;
      for (int ty = y; ty > 0; --ty) {
        memcpy(field[ty], field[ty - 1], sizeof(field[0]));
      }
      memset(field[0], 0, sizeof(field[0]));
      y++;  // Перепроверим текущую строку после сдвига
    }
  }

  // Счёт и уровень — простая система
  if (lines_cleared > 0) {
    info.score += 100 * lines_cleared;
    if (info.score > info.high_score) info.high_score = info.score;
    info.level = 1 + info.score / 500;
    info.speed = info.level;  // Пропорционально
  }
}

GameInfo_t backend_init_game(void) {
  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      field[y][x] = 0;
    }
  }
  spawn_piece(&current_piece);
  spawn_piece(&next_piece);

  info.field = (int **)malloc(FIELD_HEIGHT * sizeof(int *));
  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    info.field[i] = field[i];
  }

  info.next = (int **)malloc(FIGURE_SIZE * sizeof(int *));
  for (int i = 0; i < FIGURE_SIZE; ++i) {
    info.next[i] = next_piece.shape[i];
  }

  info.score = 0;
  info.high_score = 0;
  info.level = 1;
  info.speed = 1;
  info.pause = 0;

  return info;
}

static int check_collision(int dx, int dy) {
  for (int y = 0; y < FIGURE_SIZE; ++y) {
    for (int x = 0; x < FIGURE_SIZE; ++x) {
      if (current_piece.shape[y][x]) {
        int nx = current_piece.x + x + dx;
        int ny = current_piece.y + y + dy;

        if (nx < 0 || nx >= FIELD_WIDTH || ny >= FIELD_HEIGHT) return 1;
        if (ny >= 0 && field[ny][nx]) return 1;
      }
    }
  }
  return 0;
}
static int check_spawn_failure() {
  for (int y = 0; y < FIGURE_SIZE; ++y) {
    for (int x = 0; x < FIGURE_SIZE; ++x) {
      if (current_piece.shape[y][x]) {
        int fx = current_piece.x + x;
        int fy = current_piece.y + y;
        if (fy >= 0 && field[fy][fx]) return 1;
      }
    }
  }
  return 0;
}

static void fix_piece() {
  for (int y = 0; y < FIGURE_SIZE; ++y) {
    for (int x = 0; x < FIGURE_SIZE; ++x) {
      if (current_piece.shape[y][x]) {
        int fx = current_piece.x + x;
        int fy = current_piece.y + y;
        if (fy >= 0 && fy < FIELD_HEIGHT && fx >= 0 && fx < FIELD_WIDTH) {
          field[fy][fx] = 1;
        }
      }
    }
  }

  clear_lines();

  current_piece = next_piece;
  spawn_piece(&next_piece);

  // Проверка: можем ли поместить новую фигуру?
  if (check_spawn_failure()) {
    fsm_set_state(STATE_GAME_OVER);
  }
}

void backend_update_physics(GameInfo_t *info_ptr) {
  (void)info_ptr;
  if (!check_collision(0, 1)) {
    current_piece.y += 1;
  } else {
    fix_piece();
  }
}

static int try_rotate() {
  int rotated[FIGURE_SIZE][FIGURE_SIZE];
  rotate_clockwise(current_piece.shape, rotated);

  // Пробуем без сдвига и со сдвигами влево/вправо
  const int offsets[] = {0, -1, 1, -2, 2};
  for (size_t i = 0; i < sizeof(offsets) / sizeof(offsets[0]); ++i) {
    int dx = offsets[i];
    Tetromino temp = current_piece;
    temp.x += dx;
    memcpy(temp.shape, rotated, sizeof(rotated));

    int collision = 0;
    for (int y = 0; y < FIGURE_SIZE; ++y) {
      for (int x = 0; x < FIGURE_SIZE; ++x) {
        if (temp.shape[y][x]) {
          int fx = temp.x + x;
          int fy = temp.y + y;
          if (fx < 0 || fx >= FIELD_WIDTH || fy >= FIELD_HEIGHT) {
            collision = 1;
            break;
          }
          if (fy >= 0 && field[fy][fx]) {
            collision = 1;
            break;
          }
        }
      }
      if (collision) break;
    }

    if (!collision) {
      current_piece = temp;
      return 1;  // Успешная ротация
    }
  }

  return 0;  // Ротация невозможна
}

void backend_handle_input(UserAction_t action, bool hold) {
  switch (action) {
    case Left:
      if (!check_collision(-1, 0)) current_piece.x -= 1;
      break;
    case Right:
      if (!check_collision(1, 0)) current_piece.x += 1;
      break;
    case Down:
      if (hold) {
        // Мгновенное падение до упора
        while (!check_collision(0, 1)) {
          current_piece.y += 1;
        }
        fix_piece();  // После касания зафиксировать
      } else {
        // Стандартное пошаговое падение
        if (!check_collision(0, 1)) current_piece.y += 1;
      }
      break;
    case Action:
      try_rotate();
      break;
    default:
      break;
  }
}
///////////////
// Вернёт field с учётом текущей фигуры
void backend_overlay_piece(GameInfo_t *info_ptr) {
  static int temp_field[FIELD_HEIGHT][FIELD_WIDTH];

  // Скопировать поле
  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      temp_field[y][x] = field[y][x];
    }
  }

  // Наложить current_piece
  for (int y = 0; y < FIGURE_SIZE; ++y) {
    for (int x = 0; x < FIGURE_SIZE; ++x) {
      if (current_piece.shape[y][x]) {
        int fx = current_piece.x + x;
        int fy = current_piece.y + y;
        if (fx >= 0 && fx < FIELD_WIDTH && fy >= 0 && fy < FIELD_HEIGHT) {
          temp_field[fy][fx] = 1;
        }
      }
    }
  }

  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    info_ptr->field[i] = temp_field[i];
  }
}
