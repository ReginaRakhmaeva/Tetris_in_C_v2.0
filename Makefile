# === Конфигурация ===
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -std=c11 -I./src -g
LDFLAGS := -lncurses

# Папки
SRC_DIR     := src
MAIN_DIR    := $(SRC_DIR)/main
TETRIS_HDR  := $(SRC_DIR)/brick_game/tetris/hdr
TETRIS_SRC  := $(SRC_DIR)/brick_game/tetris/src
CLI_HDR     := $(SRC_DIR)/gui/cli/hdr
CLI_SRC     := $(SRC_DIR)/gui/cli/src

# === Файлы ===

# backend object files (библиотека)
TETRIS_OBJS := \
  $(TETRIS_SRC)/fsm.o \
  $(TETRIS_SRC)/game_info.o \
  $(TETRIS_SRC)/game_logic.o \
  $(TETRIS_SRC)/piece_craete.o \
  $(TETRIS_SRC)/piece_fix.o \
  $(TETRIS_SRC)/piece_move.o

# frontend object files
CLI_OBJS := \
  $(CLI_SRC)/ncurses.o \
  $(CLI_SRC)/tetris_interface.o

# main
MAIN_OBJS := $(MAIN_DIR)/main.o

# output
TARGET := tetris
STATIC_LIB := libtetris.a

# === Правила ===

.PHONY: all clean

all: $(TARGET)

# финальный бинарник
$(TARGET): $(STATIC_LIB) $(MAIN_OBJS) $(CLI_OBJS)
	$(CC) $(CFLAGS) -o $@ $(MAIN_OBJS) $(CLI_OBJS) -L. -ltetris $(LDFLAGS)

# библиотека бэкенда
$(STATIC_LIB): $(TETRIS_OBJS)
	ar rcs $@ $^

# компиляция *.c → *.o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TETRIS_OBJS) $(CLI_OBJS) $(MAIN_OBJS) $(STATIC_LIB) $(TARGET)
